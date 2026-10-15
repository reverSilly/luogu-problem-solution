# P1128 [HNOI2001] 求正整数 — 完整题解

---

## 1. Re-read the problem（重读题目）

**题意重述**

给定一个正整数 $n$（$1\le n\le 5\times10^4$），求**恰好**有 $n$ 个正因子的最小正整数 $m$。

例如 $n=4$，$m=6$ 因为 $\tau(6)=4$（因子为 $1,2,3,6$），且不存在更小的正整数有 $4$ 个因子。

**输入**：一行一个整数 $n$。

**输出**：一行一个整数 $m$。

**约束**：

- $n\le 5\times 10^4$。
- $m$ 可能极大，需高精度输出。

---

## 2. Chain-of-thought with full proofs（思维链与完整证明）

### 2.1 因子个数定理

若 $m$ 的标准分解为

$$
m = p_1^{a_1} p_2^{a_2} \cdots p_k^{a_k}, \quad p_1 < p_2 < \cdots < p_k
$$

则其正因子个数为

$$
\tau(m) = (a_1+1)(a_2+1)\cdots(a_k+1)
$$

**证明**：每个素因子 $p_i$ 可取 $0,1,\dots,a_i$ 共 $a_i+1$ 种指数，由乘法原理得 $\prod (a_i+1)$。$\square$

因此问题化为：找一组非负整数指数 $a_1\ge a_2\ge\cdots\ge a_k\ge 1$（$a_i=0$ 的素因子不出现），使得

$$
\prod_{i=1}^k (a_i+1) = n
$$

且 $m = \prod p_i^{a_i}$ 最小，其中 $p_i$ 为第 $i$ 小的素数。

### 2.2 最小化条件：小素数配大指数

**引理 1（指数排序不等式）**：在最优解中，指数序列必然非增：$a_1 \ge a_2 \ge \cdots \ge a_k$。换言之，越小的素数获得的指数**不小于**越大的素数。

**证明**：设存在 $i<j$ 使得 $a_i < a_j$。交换 $a_i$ 和 $a_j$（即把 $p_i$ 的指数改为 $a_j$，$p_j$ 的指数改为 $a_i$），乘积变为

$$
m' = p_i^{a_j} p_j^{a_i} \prod_{t\ne i,j} p_t^{a_t}
$$

原乘积为 $m = p_i^{a_i} p_j^{a_j} \prod_{t\ne i,j} p_t^{a_t}$。比较：

$$
\frac{m'}{m} = p_i^{a_j-a_i} p_j^{a_i-a_j}
= \left(\frac{p_i}{p_j}\right)^{a_j-a_i}
$$

因为 $p_i < p_j$ 且 $a_j-a_i > 0$，所以比值 $< 1$，即 $m' < m$。因此非增的指数排列给出更小的 $m$。$\square$

**引理 2（最小素数引理）**：最优解中出现的素数必然是前 $k$ 个最小素数 $2,3,5,7,11,\dots$。

**证明**：若最优解使用了素数 $q$ 而跳过了某个更小的素数 $p$，将 $q$ 替换为 $p$ 会减小乘积而不改变指数序列和因子个数，与最优性矛盾。$\square$

由引理 2，我们只需考虑前若干个素数。$k$ 的上界：因为每个 $a_i+1\ge 2$，最多需要 $\lfloor\log_2 n\rfloor \le 16$ 个素数。取前 $20$ 个素数绰绰有余。

### 2.3 问题规约

定义 $d_i = a_i+1\;(d_i\ge 2)$，则条件化为：

1. $\displaystyle\prod_{i=1}^k d_i = n$，每个 $d_i$ 是 $n$ 的因子且 $d_i\ge 2$。
2. $d_1 \le d_2 \le \cdots \le d_k$（等价于 $a_1\ge a_2\ge\cdots\ge a_k$，因为 $d_i = a_i+1$）。
3. 最小化 $m = \displaystyle\prod_{i=1}^k p_i^{\,d_i-1}$。

于是问题转为：将 $n$ 划分成一个**不减**的因子序列 $d_1\le d_2\le\cdots\le d_k$（每个 $d_i\ge 2$），使得 $\prod p_i^{d_i-1}$ 最小。

### 2.4 搜索算法

枚举所有因子序列——这是一个标准的**因子分解型 DFS**。

**DFS 框架**：

```
dfs(rem, idx, last_d, cur_log, cur_val)
```

参数含义：

| 参数 | 含义 |
|------|------|
| `rem` | 未被分解的剩余部分 |
| `idx` | 下一个可用的素数下标（$p_{idx}$） |
| `last_d` | 上一个使用的 $d_{i-1}$ 值（确保 $d_i\ge last_d$，即因子不减） |
| `cur_log` | 当前累积的对数值 $\sum a_i\ln p_i$ |
| `cur_val` | 当前累积的精确乘积（bigint） |

**递归终止**：当 `rem == 1` 时，找到一个完整分解。若 `cur_val < ans` 则更新答案。

**转移**：枚举所有能整除当前 `rem` 的 $d$ 因子（$d\ge 2$ 且 $d\ge last_d$），递归：

```
dfs(rem/d, idx+1, d, cur_log + (d-1)*ln(p[idx]), cur_val * p[idx]^(d-1))
```

### 2.5 剪枝

**剪枝 A（对数剪枝）**：

维护当前最佳答案的对数值 $\ln ans$。若 `cur_log` **已经**大于或等于 $\ln ans$，则后续无论乘上多少素数乘积只会更大，可直接剪枝。

正确性：$\ln$ 是单调递增函数，且后续项均为正数，故 $cur\_log \ge \ln ans \Rightarrow cur\_val \ge ans$。

**剪枝 B（素数用尽）**：

若素数数组用完但 `rem > 1`，则无法继续分解，直接返回。

**剪枝 C（指数上界）**：

$d \le rem$ 且 $d\mid rem$，这是显然的可行性条件。

### 2.6 初值估计

搜索需要初始上界才能剪枝。直接计算 $2^{n-1}$ 过于庞大，我们采用**素数因子分解启发式**：

对 $n$ 做素因子分解 $n = \prod q_j^{e_j}$，取每个 $d = q_j$ 重复 $e_j$ 次，得到一组因子序列。排序后按 $d_i$ 不减赋值给素数，计算对应乘积作为初始答案。

例如 $n = 2^4\cdot 5^5$，取 $d$ 序列 $[2,2,2,2,5,5,5,5,5]$，排序后为 $[2,2,2,2,5,5,5,5,5]$ → 指数 $[1,1,1,1,4,4,4,4,4]$ → $m = 2^4\cdot3^4\cdot5^4\cdot7^4\cdot11^4\cdot13\cdot17\cdot19\cdot23 \approx 10^{18}$。

该启发式总能给出一个有效上界，且计算成本远低于 $2^{n-1}$。

### 2.7 高精度（Big Integer）

$m$ 可能极大（最大约 $2^{49999}$，约 $15000$ 位十进制）。需实现高精度乘法。

高精度结构体设计：

```cpp
struct bigint
{
    vector<int> d; // 小端序十进制，d[0] 为个位
    // 支持乘以小整数, 乘以大整数, 比较
};
```

用**二进制快速幂**计算 $p^a$，再用大数乘法累乘到结果上。单次幂运算 $O(\log a\cdot L^2)$，其中 $L$ 为位数。

### 2.8 复杂度分析

- **素因子表**：前 20 个素数，常数。
- **因子分解**：$O(\sqrt n)$ 生成 $n$ 的因子表，因子数 $\le 100$（$n\le 50000$ 时最多 $100$ 个因子）。
- **DFS 分支数**：$n$ 的乘法分拆数，当 $n=2^{15}=32768$ 时最大（全分解为 $2\times2\times\cdots$ 一条链），分支数不超过 $O(n^\epsilon)$，实际 $\le 10^4$。
- **瓶颈**：高精度乘法。每次状态转移需计算 $p^{d-1}$（快速幂，$O(\log d\cdot L^2)$）。但由于剪枝 A 充分，实际搜索空间极小。

总时间复杂度：$O(\text{分支数}\cdot \log n\cdot L^2)$，在 $n\le 50000$ 下轻松通过。

---

## 3. Pattern summary（模式总结）

| 子问题 | 核心技巧 | 说明 |
|--------|----------|------|
| 因子个数与指数对应 | $\tau(m)=\prod(a_i+1)$ | 将"最小化 $m$"转化为搜索指数序列 |
| 指数与素数的分配 | 排序不等式：大指数配小素数 | 确保指数非增即可自动满足 |
| 枚举因子序列 | DFS 分解 $n$ 的因子 | 按因子不减顺序搜索，配合对数剪枝 |
| 避免大整数过早计算 | 对数剪枝 | 用 `double` 比较对数，避免大量高精度运算 |
| 初始上界 | 素数因子启发式 | 将 $n$ 的素因子各作为 $d_i$ 构造初始解 |
| 高精度 | Big integer + 二进制快速幂 | 支持 $2^{49999}$ 级别输出 |

**本题本质**：将**最小化乘积**问题通过排序不等式转化为**枚举不减因子序列**的搜索问题，利用对数剪枝避免高精度爆炸。

---

## 4. Real solution with notation glossary（最终解法与符号表）

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 输入，需要的因子个数 |
| $\tau(m)$ | $m$ 的正因子个数 |
| $p_i$ | 第 $i$ 小的素数（$p_1=2,p_2=3,\dots$） |
| $a_i$ | $m$ 中 $p_i$ 的指数 |
| $d_i = a_i+1$ | 每个因子对 $\tau(m)$ 的贡献，$d_i\ge 2$ |
| $k$ | 使用的素数个数 |
| $\mathit{rem}$ | 当前待分解的剩余部分 |
| $\mathit{last\_d}$ | 上一个使用的 $d$ 值（保证因子序列不减） |
| $\ln ans$ | 当前最优答案的自然对数 |
| $\mathit{cur\_log}$ | 当前累积的自然对数 |
| $\mathit{cur\_val}$ | 当前累积的精确大整数乘积 |

### 算法流程

```
1. 读入 n
2. 预计算前 20 个素数 primes[]
3. 分解 n 的素因子，生成 n 的所有因子 divs[]
4. 用素数因子启发式计算初始 ans（bigint）和 ans_log（double）
5. DFS(rem=n, idx=0, last_d=2, cur_log=0, cur_val=1):
   a. 若 rem == 1:
        若 cur_val < ans: 更新 ans, ans_log
   b. 若 idx >= prime_cnt: 返回
   c. 遍历 divs 中所有满足 d >= 2 && d >= last_d && d <= rem && rem%d==0 的 d:
        exp = d - 1
        nlog = cur_log + exp * log(primes[idx])
        若 nlog >= ans_log - eps: continue (对数剪枝)
        nval = cur_val * pow_big(primes[idx], exp)
        DFS(rem/d, idx+1, d, nlog, nval)
6. 输出 ans 的十进制表示
```

### 关键细节

- **因子顺序**：DFS 中枚举 $d$ 时使用**升序**（从小到大），这保证 $d_i$ 序列是不减的（$d_i\ge last\_d$ 自动满足）。
  
  等等，升序遍历 $d$ 与指数非增的关系：$d_i = a_i+1$，$d_i$ 不减 $\Leftrightarrow$ $a_i$ 非增。所以升序遍历 $d$ 正是为了满足引理 1 的排序不等式。

- **对数精度**：使用 `double` 比较，$\epsilon = 10^{-12}$ 避免舍入误差。
- **pow_big**：用二进制快速幂计算 $p^{\text{exp}}$，避免逐次乘法。
- **初始 ans**：素数因子启发式总成立，因为 $\prod (q_j+1-1) = \prod q_j^{e_j} = n$，确保 $\tau = n$。

---

## 5. Example code（示例代码）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64 = long long;

constexpr int primes[]{2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
constexpr int PC{20};
constexpr double eps{1e-12};

// --- big integer ---
struct bigint
{
	vector<int> d; // little-endian base-10

	bigint() : d{1} {}

	bigint(int x)
	{
		if (x == 0) d = {0};
		else
		{
			while (x) { d.push_back(x % 10); x /= 10; }
		}
	}

	bool operator<(const bigint &o) const
	{
		if (d.size() != o.d.size())
			return d.size() < o.d.size();
		for (int i = (int)d.size() - 1; i >= 0; --i)
			if (d[i] != o.d[i])
				return d[i] < o.d[i];
		return false;
	}

	friend bigint operator*(const bigint &a, const bigint &b)
	{
		bigint r;
		r.d.assign(a.d.size() + b.d.size(), 0);
		for (size_t i = 0; i < a.d.size(); ++i)
		{
			i64 carry = 0;
			for (size_t j = 0; j < b.d.size() || carry; ++j)
			{
				i64 sum = r.d[i + j] +
					(i64)a.d[i] * (j < b.d.size() ? b.d[j] : 0) + carry;
				r.d[i + j] = (int)(sum % 10);
				carry = sum / 10;
			}
		}
		while (r.d.size() > 1 && r.d.back() == 0)
			r.d.pop_back();
		return r;
	}
};

bigint pow_big(int base, int exp)
{
	bigint r{1};
	bigint b{base};
	while (exp)
	{
		if (exp & 1) r = r * b;
		b = b * b;
		exp >>= 1;
	}
	return r;
}

// --- globals ---
int n;
vector<int> divs;
bigint ans_big;
double ans_log{1e100};

void gen_divisors(int x)
{
	// factor x
	vector<pair<int,int>> fac;
	int t{x};
	for (int i = 2; i * i <= t; ++i)
	{
		if (t % i == 0)
		{
			int cnt{0};
			while (t % i == 0) { t /= i; ++cnt; }
			fac.push_back({i, cnt});
		}
	}
	if (t > 1) fac.push_back({t, 1});

	vector<int> cur;
	function<void(int,int)> dfs_gen = [&](int pos, int val)
	{
		if (pos == (int)fac.size())
		{
			cur.push_back(val);
			return;
		}
		int p{fac[pos].first}, e{fac[pos].second};
		int mul{1};
		for (int i = 0; i <= e; ++i)
		{
			dfs_gen(pos + 1, val * mul);
			mul *= p;
		}
	};
	dfs_gen(0, 1);
	sort(cur.begin(), cur.end());
	divs.swap(cur);
}

void dfs(int rem, int pid, int last_d, double clog, const bigint &cval)
{
	if (rem == 1)
	{
		if (cval < ans_big)
		{
			ans_big = cval;
			ans_log = clog;
		}
		return;
	}
	if (pid >= PC) return;

	for (int d : divs)
	{
		if (d < last_d) continue;
		if (d > rem) break;
		if (rem % d != 0) continue;

		int exp = d - 1;
		double nlog = clog + exp * log((double)primes[pid]);
		if (nlog >= ans_log - eps) continue; // 对数剪枝

		bigint nval = cval * pow_big(primes[pid], exp);
		dfs(rem / d, pid + 1, d, nlog, nval);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n;
	gen_divisors(n);

	// --- 初始上界：素数因子启发式 ---
	{
		vector<int> exps;
		int tn{n};
		for (int i = 2; i * i <= tn; ++i)
			while (tn % i == 0) { exps.push_back(i - 1); tn /= i; }
		if (tn > 1) exps.push_back(tn - 1);
		sort(exps.rbegin(), exps.rend()); // 大指数优先（等价于 di 不减）

		ans_big = bigint{1};
		ans_log = 0.0;
		for (size_t i = 0; i < exps.size(); ++i)
		{
			ans_big = ans_big * pow_big(primes[i], exps[i]);
			ans_log += exps[i] * log((double)primes[i]);
		}
	}

	// --- 主 DFS ---
	bigint start{1};
	dfs(n, 0, 2, 0.0, start);

	// --- 输出 ---
	for (int i = (int)ans_big.d.size() - 1; i >= 0; --i)
		cout << (char)('0' + ans_big.d[i]);
	cout << '\n';

	return 0;
}
```

### 代码要点说明

| 部分 | 说明 |
|------|------|
| `primes[PC]` | 前 20 个素数，足够 $n\le 50000$ |
| `bigint` 小端序十进制 | `d[0]` 为个位，比较时从高位开始 |
| `gen_divisors` | 分解 $n$ 并生成所有因子，排序后全局可用 |
| `pow_big` | 二进制快速幂，$O(\log\text{exp})$ 次乘法 |
| 初始上界 | 将 $n$ 的每个素因子 $q$ 作为 $d=q$，指数 $q-1$ |
| `last_d` 参数 | 确保因子序列 $d_i$ 单调不减 |
| 对数剪枝 | 当前 $\log\ge ans\_log$ 时跳过整条分支 |
| DFS 升序遍历 | $d$ 升序自然满足 $d_i\ge last\_d$ |
| 输出循环 | 从高位到低位输出十进制数字 |

---

**总结**：本题将"最小化具有给定因子个数的正整数"抽象为**乘法分拆 + 排序不等式 + 搜索**的组合优化问题。核心难点在于指数对数值的巨大范围（需要高精度）和对数剪枝的设计。通过 $n$ 的因子分解和对数剪枝，将指数级搜索空间压缩到可接受范围，是高精度搜索题的经典范例。
