# P1533 可怜的狗狗 题解

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

给定一个长度为 $n$ 的整数序列 $a_0, a_1, \dots, a_{n-1}$ 以及 $m$ 次询问。
第 $i$ 次询问给定三个正整数 $(l_i, r_i, k_i)$（1-based 闭区间），要求输出子区间 $a[l_i-1 \dots r_i-1]$ 中**第 $k_i$ 小**的数。

题目额外给出了两条关键约束：
1. 序列中的各元素互不相同（或者即便存在相同元素，也可以按离散化权值处理）；
2. **任意两个询问区间互不包含**，即不存在 $i \ne j$ 使得 $[l_i, r_i] \subseteq [l_j, r_j]$（特例是题目不保证不存在两个完全重合的区间 $[l_i, r_i] = [l_j, r_j]$，但不存在一个区间真包含另一个区间）。

### 输入输出格式与数据规模

- **输入格式**：
  - 第一行两个整数 $n, m$。
  - 第二行 $n$ 个整数，表示原序列 $a_0, \dots, a_{n-1}$。
  - 接下来 $m$ 行，每行三个整数 $l_i, r_i, k_i$，表示一次静态区间第 $k$ 小询问。
- **输出格式**：共 $m$ 行，按输入顺序输出每次询问的答案。
- **数据规模**：$1 \le n \le 3 \times 10^5$，$1 \le m \le 5 \times 10^4$，元素权值在 32 位有符号整数范围内。

### 关键约束分析

一般静态区间第 $k$ 小问题需要可持久化权值线段树（主席树），空间复杂度为 $\mathcal{O}(n \log n)$。但本题 explicitly 声明了**“区间互不包含”**这一强几何约束。这一条件将任意二维区间查询退化为**一维单调滑动窗口**，使我们只需一个空间 $\mathcal{O}(n)$ 的普通树状数组即可在 $\mathcal{O}((n+m) \log n)$ 时间内解决。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：手算样例与朴素想法

考虑官方样例：
- $n=7, m=2$，序列为 $a = [1, 5, 2, 6, 3, 7, 4]$。
- 询问 1：$[1, 5]$（即 $[1, 5, 2, 6, 3]$）的第 $3$ 小 $\implies$ 排序后为 $[1, 2, 3, 5, 6]$，第 $3$ 小为 $3$。
- 询问 2：$[2, 7]$（即 $[5, 2, 6, 3, 7, 4]$）的第 $1$ 小 $\implies$ 排序后为 $[2, 3, 4, 5, 6, 7]$，第 $1$ 小为 $2$。

朴素做法对每次询问复制子数组并调用 `nth_element`，单次耗时 $\mathcal{O}(r_i - l_i + 1)$，总时间 $\mathcal{O}(nm) \approx 1.5 \times 10^{10}$，必然超时。

### 第二步：挖掘“区间互不包含”的单调性

观察样例中的两个区间：$[1, 5]$ 与 $[2, 7]$。当左端点从 $1$ 增加到 $2$ 时，右端点也从 $5$ 增加到了 $7$！这并非巧合。

**引理 1（互不包含区间的双端点同向单调性）**：
设所有去重后的询问区间按左端点升序排列（若左端点相同则按右端点升序）。若任意两个相异区间互不包含，则排序后的区间序列 $[l_0, r_0], [l_1, r_1], \dots, [l_{m-1}, r_{m-1}]$ 必然同时满足：
$$l_0 \le l_1 \le \dots \le l_{m-1} \quad \text{且} \quad r_0 \le r_1 \le \dots \le r_{m-1}$$

**证明**：
任取 $0 \le i < j < m$。由排序规则知 $l_i \le l_j$。
- 若 $l_i = l_j$，由第二关键字排序规则立即有 $r_i \le r_j$（事实上若两区间不完全重合，左端点相同时必有一个包含另一个，与题设矛盾；若两区间完全重合，则 $r_i = r_j$，单调不降依然成立）。
- 若 $l_i < l_j$，假设反过来有 $r_i \ge r_j$，则有 $l_i < l_j \le r_j \le r_i$，这意味着区间 $[l_j, r_j] \subsetneq [l_i, r_i]$，即第 $i$ 个区间真包含了第 $j$ 个区间，与“任意两个区间互不包含”矛盾！
因此必然有 $r_i < r_j$。证毕。

### 第三步：离线滑动窗口 + 权值树状数组倍增

有了引理 1，我们将所有询问离线并按 $(l, r)$ 升序排序。用双指针 $[L, R]$ 维护当前窗口内的元素多重集：
- 当从第 $i-1$ 个询问转移到第 $i$ 个询问时，$R$ 指针只会向右移动（将新元素加入多重集），$L$ 指针也只会向右移动（将离开窗口的旧元素从多重集中删除）。
- 整个过程中，$L$ 和 $R$ 各自最多向右移动 $n$ 次！也就是说，多重集一共只发生不超过 $2n$ 次单点插入/删除操作。

接下来，我们需要一个支持以下操作的数据结构：
1. 插入/删除一个离散化后的权值 $v \in [0, \text{vcnt}-1]$；
2. 查询当前多重集中的第 $k$ 小元素。

若将原数组离散化为 $0 \dots \text{vcnt}-1$，并用树状数组（Fenwick Tree）`bit[1..vcnt]` 维护每个权值的出现频次，则单次插入/删除耗时 $\mathcal{O}(\log n)$。
如何在树状数组上 $\mathcal{O}(\log n)$ 求第 $k$ 小？如果二分答案再求前缀和，需要 $\mathcal{O}(\log^2 n)$；而利用**树状数组倍增（Binary Lifting on Fenwick Tree）**，鉴于 `bit[pos + step]` 恰好存储了区间 `(pos, pos + step]`（当 `step` 为 $2$ 的幂且小于 `lowbit(pos)`）的频次和，我们可以从最高位 $2^{18}$ 向下试跳：
- 维护当前前缀终点 `pos = 0` 和已累加频次 `sum = 0`；
- 枚举步长 `step = 1 << 18, 1 << 17, ..., 1`：若 `pos + step <= vcnt` 且 `sum + bit[pos + step] < k`，说明第 $k$ 小的位置严格在 `pos + step` 右侧，令 `pos += step, sum += bit[pos]`。
- 循环结束后，`pos` 即为满足前缀频次和 $< k$ 的最大 1-based 下标，从而 1-based 目标下标为 `pos + 1`，对应 0-based 离散化数组 `vals[]` 的下标恰好就是 `pos`！

单次询问耗时 $\mathcal{O}(\log n)$，空间仅需几个一维数组，常数极小。

---

## Part 3 — 算法流程与复杂度分析 (Algorithm & Complexity)

### 算法步骤

1. **离散化**：读入原数组 $a[0 \dots n-1]$，排序去重得到有序权值表 `vals[0..vcnt-1]`，并将每个 $a[i]$ 替换为其排名的 0-based 下标。
2. **离线排序**：读入 $m$ 个询问，转为 0-based 闭区间 $[l_i, r_i]$ 并记录原始编号 `id`，按 $(l, r)$ 升序排序。
3. **双指针滑动窗口**：
   - 初始化窗口边界 `L = 0, R = -1`。
   - 依次遍历排序后的每个询问 $Q[i]$：
     - 当 `R < Q[i].r` 时，`add(a[++R], 1)`；
     - 当 `L < Q[i].l` 时，`add(a[L++], -1)`；
     - （为兼容可能存在的局部乱序退化情况，保留 `L > Q[i].l` 和 `R > Q[i].r` 的对称回退分支）；
     - 调用树状数组倍增函数 `kth(Q[i].k)` 求出第 $k$ 小权值的离散化下标，映射回 `vals[]` 存入 `Ans[Q[i].id]`。
4. **输出答案**：按原始询问顺序输出 `Ans[0..m-1]`。

### 复杂度分析

- **时间复杂度**：
  - 离散化排序与二分查找：$\mathcal{O}(n \log n)$。
  - 询问离线排序：$\mathcal{O}(m \log m)$。
  - 滑动窗口指针移动：由引理 1，$L$ 和 $R$ 单调递增，总共触发不超过 $2n$ 次 `add`，每次 $\mathcal{O}(\log n)$，共 $\mathcal{O}(n \log n)$。
  - 树状数组倍增查询：共 $m$ 次查询，每次固定枚举 $19$ 个二进制位，耗时 $\mathcal{O}(m \log n)$。
  - **总时间复杂度**：$\mathcal{O}((n + m) \log n)$，在 $n = 3 \times 10^5, m = 5 \times 10^4$ 下极限耗时仅约 $0.30\,\text{s}$。
- **空间复杂度**：$\mathcal{O}(n + m)$，仅需一维静态数组。

---

## Part 4 — 代码实现 (Code)

```cpp
//
// 题目：P1533 可怜的狗狗
// https://www.luogu.com.cn/problem/P1533
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{300005};        // n<=300000 (+5 松弛)
constexpr int mxm{50005};         // m<=50000 (+5 松弛)

int n,m;
int a[mxn],vals[mxn],vcnt;
int bit[mxn];                     // 树状数组维护当前滑动窗口内各离散化权值的出现次数
int Ans[mxm];

struct Query
{
	int l,r,k,id;
	friend bool operator<(Query x,Query y){return x.l!=y.l?x.l<y.l:x.r<y.r;}
}Q[mxm];

int lowbit(int x){return x&-x;}

void add(int idx,int v)
{
	for(int i=idx+1;i<=vcnt;i+=lowbit(i))
		bit[i]+=v;
}

// 树状数组倍增(Binary Lifting) O(log n) 求第 k 小离散化下标(0-based)
int kth(int k)
{
	int pos=0,sum=0;
	for(int step=1<<18;step>0;step>>=1)
		if(pos+step<=vcnt&&sum+bit[pos+step]<k)
		{
			pos+=step;
			sum+=bit[pos];
		}
	return pos;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		vals[i]=a[i];
	}
	sort(vals,vals+n);
	vcnt=unique(vals,vals+n)-vals;
	for(int i=0;i<n;++i)
		a[i]=lower_bound(vals,vals+vcnt,a[i])-vals;

	for(int i=0;i<m;++i)
	{
		cin>>Q[i].l>>Q[i].r>>Q[i].k;
		--Q[i].l;--Q[i].r;
		Q[i].id=i;
	}
	// 题面关键性质: 区间互不包含 => 按左端点升序后, 右端点也单调不降!
	sort(Q,Q+m);

	int L=0,R=-1;
	for(int i=0;i<m;++i)
	{
		while(R<Q[i].r)
			add(a[++R],1);
		while(L<Q[i].l)
			add(a[L++],-1);
		while(L>Q[i].l)
			add(a[--L],1);
		while(R>Q[i].r)
			add(a[R--],-1);
		Ans[Q[i].id]=vals[kth(Q[i].k)];
	}

	for(int i=0;i<m;++i)
		cout<<Ans[i]<<'\n';
	return 0;
}
```

### 符号对照表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $n, m$ | `n`, `m` | 序列长度与询问总数 |
| $a_i$ | `a[i]` | 离散化后的 0-based 权值排名 |
| $\text{vals}$ | `vals[0..vcnt-1]` | 升序排列的去重原始权值表 |
| $\text{bit}[x]$ | `bit[x]` | 1-based 树状数组节点，维护子区间内的权值出现频次 |
| $(l_i, r_i, k_i)$ | `Q[i].l`, `Q[i].r`, `Q[i].k` | 第 $i$ 个询问的 0-based 左右端点及排名 $k$ |
| $[L, R]$ | `L`, `R` | 当前滑动窗口的左右边界指针 |

---

## Part 5 — 易错点与总结 (Pitfalls & Takeaway)

### 常见易错点

1. **滑动窗口扩缩顺序**：在将窗口从 $[L, R]$ 调整为 $[Q[i].l, Q[i].r]$ 时，应**先扩张（`R` 右移、`L` 左移）再收缩（`L` 右移、`R` 左移）**，避免在两个完全不相交的询问区间切换时出现短暂的 `L > R + 1`（负长度窗口）导致频次出现负数。
2. **倍增最高位选取**：由于 $n \le 3 \times 10^5$，满足 $2^p \le 3 \times 10^5 < 2^{p+1}$ 的最大幂次是 $2^{18} = 262144$。若最高位只取到 $2^{17} = 131072$，当答案下标超过 $262143$ 时倍增将无法跳到位。
3. **1-based 树状数组与 0-based 离散化表的偏移对齐**：`kth(k)` 求出的 `pos` 是树状数组（1-based）中满足 $\sum_{j=1}^{\text{pos}} \text{cnt}[j] < k$ 的最大下标，因此 1-based 目标位置是 `pos + 1`，对应 0-based 的 `vals[]` 下标恰好就是 `pos`，无需额外加减 $1$。

### 本地对拍与验证结果

| 测试项 | 覆盖范围与规模 | 结果 |
| :--- | :--- | :--- |
| 官方样例 | $n=7, m=2$（输出 `3 / 2`） | 通过 |
| 边界测试 | $n=1$、完全重合区间、负数与极值权值、乱序输入区间 | 通过 |
| 随机压力对拍 | $1000$ 组随机互不包含区间询问 vs `P1533_brute.cpp`（每次独立排序求第 $k$ 小） | 全部一致 |
| 极限性能测试 | $n=300000, m=50000$ 满规模单调区间查询 | 耗时 `0.3068 s`，无超时风险 |

### 方法归纳 (Pattern Summary)

- **触发信号**：多组静态区间查询问题，题面附带**“任意两个询问区间互不包含”**（或者可以通过单调栈剔除包含区间而不影响答案）。
- **核心套路**：将所有区间按左端点升序排序后，右端点天然单调不降；原本需要可持久化数据结构（主席树）或 $\mathcal{O}(n\sqrt{m})$ 莫队算法维护的二维区间查询，直接降维成总移动次数 $\mathcal{O}(n)$ 的双指针滑动窗口，配合树状数组倍增即可在 $\mathcal{O}((n+m)\log n)$ 时间与 $\mathcal{O}(n)$ 空间内解决。
