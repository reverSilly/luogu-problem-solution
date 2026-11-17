# P2926 [USACO08DEC] Patting Heads S — 题解

## 一、解题过程：人类思考叙事

**题意重述（剥离背景）。** 有 $N$ 头奶牛（$1\le N\le 10^5$），第 $i$ 头抽到一个整数 $A_i$（$1\le A_i\le 10^6$，**可以重复**）。奶牛 $i$ 会去拍**所有其他**奶牛 $j$ 的头，条件是 $A_i$ 能被 $A_j$ 整除，即 $A_j\mid A_i$。对每头奶牛输出她拍了多少下。

两个必须先抠清楚的口径：

1. **"围成一圈"是幌子。** 题面说她"绕着圈走一圈，拍所有其他奶牛 $j$"——只要满足整除条件就拍，**和座位相邻无关**。所以答案就是满足 $A_j\mid A_i$ 的 $j\ne i$ 的个数，圆圈结构完全不参与计算。
2. **整除方向。** 中文"$A_i$ 能被 $A_j$ 整除"= $A_i$ 是被除数、$A_j$ 是除数 = $A_j\mid A_i$。用题面 Hint 验证：$A=(2,1,2,3,4)$，第一头（$A_1=2$）拍了第二头（$A_2=1$，$1\mid2$ ✓）和第三头（$A_3=2$，$2\mid2$ ✓），没拍第四头（$3\nmid2$）、第五头（$4\nmid2$）。方向确认无误。

**暴力为什么不行。** 对每头 $i$ 枚举每头 $j$ 判整除，$O(N^2)=10^{10}$，再算上取模常数，妥妥超时。瓶颈在于"奶牛对"太多，但**数值只有 $10^6$ 种**——这提示我们把视角从"牛"切换到"值"。

**关键翻转：贡献法。** 与其问"牛 $i$ 被哪些值整除"，不如问"值 $v$ 能整除哪些数"。设 $\texttt{cnt}[v]$ 为抽到值 $v$ 的奶牛数。值 $v$ 会对**所有 $v$ 的倍数** $m=v,2v,3v,\dots$ 产生贡献：凡是 $A_i=m$ 的牛，都要拍那些值为 $v$ 的牛（共 $\texttt{cnt}[v]$ 头）。于是令

$$\texttt{sum}[m]=\sum_{v\mid m}\texttt{cnt}[v]=\text{值整除 }m\text{ 的奶牛总数}.$$

牛 $i$ 的答案就是 $\texttt{sum}[A_i]-1$：因为 $A_i\mid A_i$，$\texttt{cnt}[A_i]$ 里把**她自己**也算进去了，而题目要拍的是"其他"奶牛，扣掉这一头。注意**其余同值的牛是要拍的**（她们是不同的牛，$A_j=A_i$ 满足 $A_j\mid A_i$），所以只扣 $1$，不是扣 $\texttt{cnt}[A_i]$。

**怎么算 sum 才快。** 直接对每个 $m$ 枚举它的约数是 $O(V\sqrt V)$ 级别；换成"对每个值 $v$，把 $\texttt{cnt}[v]$ 加到它的所有倍数上"，就是埃氏筛的循环结构，总次数是调和级数 $\sum_{v=1}^{V}\lfloor V/v\rfloor=O(V\log V)\approx1.4\times10^7$，轻松通过。

**手造数据核对。**

第一组直接用 Hint：$N=5$，$A=(2,1,2,3,4)$。$\texttt{cnt}:1\to1,2\to2,3\to1,4\to1$。
- $\texttt{sum}[2]=\texttt{cnt}[1]+\texttt{cnt}[2]=3$，牛 1、牛 3 答案 $3-1=2$；
- $\texttt{sum}[1]=\texttt{cnt}[1]=1$，牛 2 答案 $0$；
- $\texttt{sum}[3]=\texttt{cnt}[1]+\texttt{cnt}[3]=2$，牛 4 答案 $1$；
- $\texttt{sum}[4]=\texttt{cnt}[1]+\texttt{cnt}[2]+\texttt{cnt}[4]=4$，牛 5 答案 $3$。

输出 `2 0 2 1 3`，与 Hint"第一头拍第二第三、第二头没拍"逐条吻合。

第二组专打重复值：$N=3$，$A=(5,5,5)$。每头都拍另外两头（$5\mid5$），答案 `2 2 2`。$\texttt{sum}[5]=\texttt{cnt}[5]=3$，$3-1=2$ ✓——验证"只扣自己、不扣同值伙伴"。

第三组专打值 $1$：$N=4$，$A=(1,1,2,3)$。$1$ 整除一切，两头值为 $1$ 的牛互相拍（各 $1$ 下）；值为 $2$ 的牛拍两头 $1$（$2$ 下）；值为 $3$ 的牛拍两头 $1$（$2$ 下）。输出 `1 1 2 2` ✓。

---

## 二、思维链：完整证明

**记号。** 值域上界 $V=10^6$。奶牛下标 $0\le i<N$，值 $A_i\in[1,V]$。$\texttt{cnt}[v]=|\{i:A_i=v\}|$。定义 $\texttt{sum}[m]=\sum_{v\mid m}\texttt{cnt}[v]$（$v$ 取遍 $m$ 的正约数）。答案 $\textit{ans}_i=|\{j\ne i:A_j\mid A_i\}|$。

---

### 引理 1（圆圈无关，答案即整除计数）

奶牛 $i$ 拍头的次数等于 $|\{j\ne i:A_j\mid A_i\}|$，与奶牛的座位排列无关。

*证明*：题面规定奶牛 $i$ "绕着圈走一圈，拍**所有**其他奶牛 $j$，只要 $A_i$ 能被 $A_j$ 整除"。这是一个对**全体** $j\ne i$ 的遍历，触发条件仅为 $A_j\mid A_i$，不含任何关于 $j$ 与 $i$ 相邻或距离的限制。圆圈只保证她能"走一圈回到原位"，即遍历能覆盖所有其他牛，不增删任何被拍对象。故被拍集合恰为 $\{j\ne i:A_j\mid A_i\}$，其大小即答案。证毕。

---

### 引理 2（按值分桶 + 扣自身）

$\textit{ans}_i=\texttt{sum}[A_i]-1=\Bigl(\sum_{v\mid A_i}\texttt{cnt}[v]\Bigr)-1$。

*证明*：把满足 $A_j\mid A_i$ 的牛 $j$（**暂含 $j=i$**）按值分类。对 $A_i$ 的每个正约数 $v$，值恰为 $v$ 的牛有 $\texttt{cnt}[v]$ 头，且她们都满足 $v\mid A_i$；不同约数对应的牛集互不相交（一头牛只有一个值）。故

$$|\{j:A_j\mid A_i\}|=\sum_{v\mid A_i}\texttt{cnt}[v]=\texttt{sum}[A_i].$$

这个集合包含 $j=i$ 当且仅当 $A_i\mid A_i$，而整除自反恒成立，且 $i$ 在 $\texttt{cnt}[A_i]$ 中恰被计一次。题目要 $j\ne i$，故减去这唯一的一个自身元素：$\textit{ans}_i=\texttt{sum}[A_i]-1$。

特别地，若还有别的牛 $j\ne i$ 满足 $A_j=A_i$，她们是**不同**的牛、满足 $A_j\mid A_i$，应当被拍，**不在扣除之列**——扣除的只是 $i$ 自己这一个下标。证毕。

---

### 引理 3（倍数枚举恰好实现 sum 的定义）

执行"对每个 $v\in[1,V]$ 且 $\texttt{cnt}[v]>0$，令 $m=v,2v,3v,\dots\le V$，每次 $\texttt{sum}[m]\mathrel{+}=\texttt{cnt}[v]$"后，对所有 $m\in[1,V]$ 有 $\texttt{sum}[m]=\sum_{v\mid m}\texttt{cnt}[v]$。

*证明*：考察任意一对 $(v,m)$，$1\le v\le m\le V$。内层循环以步长 $v$ 从 $v$ 递增，访问的 $m$ 恰为 $v$ 的倍数；故"$\texttt{cnt}[v]$ 被加到 $\texttt{sum}[m]$"发生**当且仅当** $v\mid m$，且此时只发生**一次**（外层每个 $v$ 只跑一遍，内层每个倍数只访问一次）。于是

$$\texttt{sum}[m]=\sum_{\substack{1\le v\le V\\ v\mid m,\ \texttt{cnt}[v]>0}}\texttt{cnt}[v]=\sum_{v\mid m}\texttt{cnt}[v],$$

最后一步因为 $\texttt{cnt}[v]=0$ 的项贡献为 $0$，跳过与否不改变和。初值 $\texttt{sum}\equiv0$（全局数组零初始化），无残留。证毕。

---

### 引理 4（调和级数复杂度）

倍数枚举的总内层迭代次数为 $\sum_{v=1}^{V}\lfloor V/v\rfloor\le V(1+\ln V)=O(V\log V)$。

*证明*：对固定 $v$，内层 $m=v,2v,\dots,\lfloor V/v\rfloor v$ 共 $\lfloor V/v\rfloor$ 次。求和并用 $\lfloor V/v\rfloor\le V/v$：

$$\sum_{v=1}^{V}\Bigl\lfloor\frac Vv\Bigr\rfloor\le V\sum_{v=1}^{V}\frac1v=V\cdot H_V,$$

其中 $H_V$ 为第 $V$ 个调和数。由积分比较 $\sum_{v=1}^{V}\frac1v\le1+\int_1^V\frac{dx}x=1+\ln V$，得 $\le V(1+\ln V)$。代入 $V=10^6$：$10^6\times(1+13.8155)\approx1.48\times10^7$ 次加法。代码用 `if(cnt[v])` 跳过未出现的值，只会**减少**迭代（最坏全出现即上界）。读入 $O(N)$、输出 $O(N)$。总时间 $O(N+V\log V)$。证毕。

---

### 引理 5（数值范围与溢出）

所有量都在 `int` 范围内：$\texttt{cnt}[v]\le N\le10^5$，$\texttt{sum}[m]\le N\le10^5$，$\textit{ans}_i\in[0,N-1]$。

*证明*：$\texttt{cnt}[v]$ 是牛数的子集大小，$\le N$。$\texttt{sum}[m]=\sum_{v\mid m}\texttt{cnt}[v]$ 统计的是"值整除 $m$ 的牛"，每头牛至多被计一次（其值唯一），故 $\le N$。$\textit{ans}_i=\texttt{sum}[A_i]-1\ge\texttt{cnt}[A_i]-1\ge0$（自身总在），且 $\le N-1$。$N\le10^5\ll2^{31}-1$，无溢出。证毕。

---

### 定理

程序对每头奶牛输出 $\texttt{sum}[A_i]-1$，即她拍其他奶牛的次数；总时间 $O(N+V\log V)$，空间 $O(N+V)$。

*证明*：引理 1 把问题化为整除计数；引理 2 证明答案 $=\texttt{sum}[A_i]-1$；引理 3 证明倍数枚举正确算出 $\texttt{sum}$；引理 4 给出 $O(V\log V)$ 的时间；引理 5 保证 `int` 不溢出。空间：数组 $a$ 占 $O(N)$，$\texttt{cnt},\texttt{sum}$ 各 $O(V)$。串联即得。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **explore its property** — "围成一圈"是幌子：拍打条件只看 $A_j\mid A_i$，与座位无关，答案就是整除计数。
2. **consider easier version** — 暴力枚举牛对 $O(N^2)=10^{10}$ 超时，确认必须按值域而非按牛加速。
3. **reduce to easier problem** — 贡献法翻转视角：从"对每头牛找约数"改成"对每个值找倍数"，把牛对枚举塌缩成值域筛。
4. **classic problem solution** — 值域桶 $\texttt{cnt}$ + 埃氏筛式倍数枚举求 $\texttt{sum}[m]=\sum_{v\mid m}\texttt{cnt}[v]$，调和级数 $O(V\log V)$。
5. **discuss by category** — 重复值用桶合并；$A_i\mid A_i$ 把自身算进去了，答案要 $-1$（但同值的其他牛照拍，只扣自己）。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 | 初值 / 记忆点 |
|------|------|------|---------------|
| $N$ | 奶牛数，$\le10^5$ | `int n` | 读入 |
| $V$ | 值域上界 $10^6$ | `constexpr mxv{1000005}` | 数组尺寸 = 约束 +5 |
| `a[i]` | 第 $i$ 头牛的值 $A_i$ | `int[mxn]` | 读入，0-based |
| `cnt[v]` | 抽到值 $v$ 的奶牛数 | `int[mxv]` | 全局默认 $0$，读入时 `++cnt[a[i]]` |
| `sum[m]` | 值整除 $m$ 的奶牛总数 $=\sum_{v\mid m}\texttt{cnt}[v]$ | `int[mxv]` | 全局默认 $0$，倍数枚举累加 |
| `v` | 外层枚举的"除数"值 | `int` | $1..V$，仅 `cnt[v]>0` 时展开 |
| `m` | 内层枚举的 $v$ 的倍数 | `int` | $v,2v,\dots\le V$ |

### 算法步骤

1. I/O 加速三行。
2. 读 $N$；对每头牛读 $A_i$ 存入 `a[i]`，并 `++cnt[a[i]]`。
3. 对 $v=1..V$：若 `cnt[v]>0`，对 $m=v,2v,\dots\le V$ 执行 `sum[m]+=cnt[v]`。
4. 对每头牛 $i$ 输出 `sum[a[i]]-1` 与 `'\n'`。

### 复杂度

- 时间：$O(N+V\log V)$，$V=10^6$ 时倍数枚举约 $1.48\times10^7$ 次加法。
- 空间：$O(N+V)$，两个 $10^6$ 级 `int` 数组约 $8$ MB。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；Hint 数据输出 `2 0 2 1 3` 逐条吻合；另造"全同值""含值 1"两组手算数据全对；$N=10^5$、值随机满值域的极限数据正常跑完。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2926 [USACO08DEC] Patting Heads S
// 值域桶 cnt[v]；每个出现过的值 v 给它的全体倍数 m 贡献 cnt[v]（调和级数 O(V log V)）
// 奶牛 i 的答案 = sum[A_i]-1（A_i 整除自身，要扣掉自己这一拍）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005},mxv{1000005};

int n,a[mxn],cnt[mxv],sum[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		++cnt[a[i]];
	}
	for(int v=1;v<mxv;++v)
		if(cnt[v])
			for(int m=v;m<mxv;m+=v)
				sum[m]+=cnt[v];	// sum[m]=值整除 m 的奶牛总数
	for(int i=0;i<n;++i)
		cout<<sum[a[i]]-1<<'\n';
	return 0;
}
```

### 代码说明

- **核心是那个双重循环的方向**：外层除数 $v$、内层倍数 $m$。这正是埃氏筛的结构，复杂度由调和级数保证；反过来"对每个 $m$ 试除找约数"会慢得多。
- `if(cnt[v])` 跳过没出现的值，是零成本的剪枝（最坏不影响上界）。
- `sum[a[i]]-1` 的 $-1$ 扣的是**自己**（$A_i\mid A_i$），不是扣同值伙伴——引理 2 专门论证了这点，漏掉或扣多都会错。
- `cnt`、`sum` 都靠全局零初始化，本题语义下 $0$ 恰好是正确初值（计数从 $0$ 累加），无需 `fill`。
- 全程 `int` 足够（引理 5），不必开 `long long`。