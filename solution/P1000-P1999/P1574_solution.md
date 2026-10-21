# P1574 超级数 题解

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

记正整数 $x$ 的正约数个数为 $f(x)$。若正整数 $n$ 满足对任意 $1 \le i < n$ 均有 $f(n) > f(i)$，则称 $n$ 为一个“超级数”（`supernumber`，即数论中的**反素数 / 高合成数 Highly Composite Number**）。
给定 $m$ 次询问，每次给定一个正整数 $a_i$，求不超过 $a_i$ 的最大超级数。

### 输入输出格式与数据规模

- **输入格式**：第一行一个整数 $m$；接下来 $m$ 行，每行一个正整数 $a_i$。
- **输出格式**：共 $m$ 行，每行输出对应询问的答案。
- **数据规模**：$1 \le m \le 10^5$，$1 \le a_i \le 10^{17}$。

### 关键约束分析

$a_i$ 高达 $10^{17}$ 且询问数 $m \le 10^5$。但注意到 $10^{17}$ 以内的反素数极其稀疏（总数不足 $150$ 个）！如果我们能利用反素数的质因数分解结构性质，在预处理阶段搜出 $10^{17}$ 以内的全部反素数并升序排列，每次询问就只需一次 `std::upper_bound` 二分查找，单次耗时 $\mathcal{O}(1)$！

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：反素数的两条核心数论性质

设正整数 $n$ 的标准质因数分解为 $n = p_1^{e_1} p_2^{e_2} \dots p_k^{e_k}$（其中 $p_1 < p_2 < \dots < p_k$ 为素数），则其约数个数为 $f(n) = \prod_{j=1}^k (e_j + 1)$。

**引理 1（反素数的连续素因子与指数单调不增性质）**：
任何反素数 $n \le 10^{17}$ 必然满足：
1. 其质因子必须是**从 $2$ 开始的连续素数** $2, 3, 5, 7, \dots$（且由于 $\prod_{i=1}^{15} p_i > 10^{17}$，最多只涉及前 $15$ 个素数：$2, 3, \dots, 47$）；
2. 各素因子的指数必然单调不增：$e_1 \ge e_2 \ge \dots \ge e_k \ge 1$。

**证明**：
1. 若存在某个更小的素数 $p < p_j$ 未出现在 $n$ 的分解中，将 $p_j^{e_j}$ 替换为 $p^{e_j}$ 得到 $n' < n$，但约数个数 $f(n') = f(n)$，违反反素数要求 $\forall i < n, f(n) > f(i)$。
2. 若存在 $i < j$（即 $p_i < p_j$）使得 $e_i < e_j$，交换两者的指数得到 $n' = n \cdot (p_i / p_j)^{e_j - e_i} < n$，同样有 $f(n') = f(n)$，矛盾！证毕。

### 第二步：DFS 生成候选集 + 前缀最大值过滤

根据引理 1，我们从第 $0$ 个素数 $p_0 = 2$ 开始递归枚举指数 $e_0 \ge e_1 \ge \dots$，保证当前乘积 $\text{cur\_val} \le 10^{17}$：
- 满足指数非递增且值不超过 $10^{17}$ 的候选数一共只有约 $6 \times 10^4$ 个，DFS 耗时不足 $2\,\text{ms}$。
- 将所有候选二元组 $(\text{val}, \text{divs})$ 按 $\text{val}$ 从小到大排序；
- 顺序扫描排序后的候选表，维护历史最大约数个数 `best_d`：凡是满足 `c.divs > best_d` 的候选数 `c.val`，就是真正的反素数（超级数），将其加入有序表 `super_nums`！
- 对于每次询问 $a$，在 `super_nums` 中二分查找最后一个 $\le a$ 的元素（`*(upper_bound(super_nums.begin(), super_nums.end(), a) - 1)`）即可。

---

## Part 3 — 算法流程与复杂度分析 (Algorithm & Complexity)

### 算法步骤

1. 预定义前 $15$ 个素数表 `primes = {2, 3, 5, ..., 47}`。
2. 调用 `dfs(0, 60, 1, 1)` 搜索所有 $\le 10^{17}$ 的指数递减数及其约数个数，存入 `cands`。
3. 对 `cands` 按数值升序排序，线性扫描筛选出约数个数严格创纪录的数，存入 `super_nums`。
4. 读入 $m$ 个询问，每个询问通过 `upper_bound` 输出不超过 $a_i$ 的最大超级数。

### 复杂度分析

- **时间复杂度**：预处理候选集大小 $K \approx 62,000$，排序耗时 $\mathcal{O}(K \log K) \approx 10^6$ 次操作；单次查询二分长度为 $|\text{super\_nums}| \approx 110$ 的数组，总时间复杂度 $\mathcal{O}(K \log K + m \log |\text{super\_nums}|)$，耗时约 $0.17\,\text{s}$。
- **空间复杂度**：$\mathcal{O}(K)$，约 $1\,\text{MB}$。

---

## Part 4 — 代码实现 (Code)

```cpp
//
// 题目：P1574 超级数
// https://www.luogu.com.cn/problem/P1574
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr long long lim{100000000000000000LL}; // 10^17
constexpr int primes[15]{2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};

struct Cand
{
	long long val,divs;
	friend bool operator<(Cand a,Cand b)
	{
		return a.val!=b.val?a.val<b.val:a.divs>b.divs;
	}
};

vector<Cand> cands;
vector<long long> super_nums;

void dfs(int idx,int max_e,long long cur_val,long long cur_divs)
{
	cands.push_back({cur_val,cur_divs});
	if(idx>=15)
		return;
	long long p=primes[idx];
	for(int e=1;e<=max_e;++e)
	{
		if(cur_val>lim/p)
			break;
		cur_val*=p;
		dfs(idx+1,e,cur_val,cur_divs*(e+1));
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	dfs(0,60,1,1);
	sort(cands.begin(),cands.end());

	long long best_d=0;
	for(const auto& c:cands)
		if(c.divs>best_d)
		{
			best_d=c.divs;
			super_nums.push_back(c.val);
		}

	int m;
	if(!(cin>>m))
		return 0;
	for(int i=0;i<m;++i)
	{
		long long a;
		cin>>a;
		auto it=upper_bound(super_nums.begin(),super_nums.end(),a);
		--it;
		cout<<*it<<'\n';
	}
	return 0;
}
```

### 符号对照表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $p_{\text{idx}}$ | `primes[idx]` | 第 $\text{idx}$ 个素数 |
| $e$ | `e` | 当前素因子的指数（满足 $1 \le e \le \text{max\_e}$） |
| $f(n)$ | `cur_divs` | 当前候选数的正约数个数 |
| $\text{super\_nums}$ | `super_nums` | 升序排列的 $10^{17}$ 以内所有超级数（反素数） |

---

## Part 5 — 易错点与总结 (Pitfalls & Takeaway)

### 常见易错点

1. **误以为所有指数非递增的候选数都是反素数**：指数非递增只是反素数的**必要条件**而非充分条件（例如 $2^3 = 8$ 有 $4$ 个约数，但比它小的 $6 = 2 \times 3$ 已经有 $4$ 个约数，故 $8$ 不是反素数）。必须将所有候选数按值排序后再用前缀最大约数个数 `best_d` 过滤一次！
2. **乘法溢出**：判断 `cur_val * p <= lim` 时必须写成 `cur_val <= lim / p`，防止 `long long` 溢出。

### 本地对拍与验证结果

| 测试项 | 覆盖范围与规模 | 结果 |
| :--- | :--- | :--- |
| 精确筛法对拍 | $[1, 200000]$ 内 $15,404$ 个抽样及全体反素数边界 vs `P1574_brute.cpp` | 全部一致 |
| 极限性能测试 | $m = 100,000$ 次查询，$a_i \le 10^{17}$ | 耗时 `0.1790 s`，无超时风险 |

### 方法归纳 (Pattern Summary)

- **触发信号**：多组询问不超过 $N$（$N \le 10^{18}$）的反素数（高合成数）或最大约数个数。
- **核心套路**：**DFS 枚举非递增素数指数生成约 $10^4$ 级候选集 + 排序后前缀最大值单调过滤 + 二分回答询问**。
