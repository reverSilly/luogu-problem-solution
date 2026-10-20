# P1559 运动员最佳匹配问题

> 标签：搜索 · 剪枝 · 模拟退火 · 二分图 · 状压 DP · 难度：省选/NOI-
> 本地题面：`P1559.md`（$n\le 20$，保证数据在 `int` 范围内）。
> 验证：$400$ 组随机用例（$n\le 8$，含负数优势值）标程与全排列暴力逐字节一致；$n=20$ 最坏规模 $0.0914$ s。

## 1. 重新读题

**问题重述（人话版）**

男女运动员各大 $n$ 人。给两个 $n\times n$ 矩阵：

- $P_{i,j}$：**男**运动员 $i$ 和**女**运动员 $j$ 配对时，「男运动员」那一侧的竞赛优势；
- $Q_{i,j}$：**女**运动员 $i$ 和**男**运动员 $j$ 配合时，「女运动员」那一侧的竞赛优势。

男 $i$ 与女 $j$ 配对时，这一对的总优势 = $P_{i,j}\times Q_{j,i}$（注意乘积的两个下标一个是 $P_{i,j}$、一个是 $Q_{j,i}$，**不是** $Q_{i,j}$）。

要找一个「男女一一配对」（即一个 $n$ 元完美匹配，一组排列 $\sigma$：男 $i$ 配女 $\sigma(i)$），使 $\sum_i P_{i,\sigma(i)}\times Q_{\sigma(i),\,i}$ 最大，输出这个最大值。

**题面里容易漏掉的条款**

1. 「$P_{i,j}$ 不一定等于 $Q_{j,i}$，反过来 $P_{i,j}\ne Q_{i,j}$ 也未必成立」$\Rightarrow$ 权重只能按题面**逐字**取 $P_{i,j}\times Q_{j,i}$，不能想当然写成 $P_{i,j}\times Q_{i,j}$。
2. 「$n$ 个男、$n$ 个女，一一配对」$\Rightarrow$ 是一个**方阵的完美匹配**（排列问题），不是一般二分图的最大权匹配（虽然算法同源）。
3. 「保证题目数据均在 `int` 范围内」$\Rightarrow$ 单个 $P$、$Q$ 元素及最终答案量级不大；但乘积的中间过程建议仍用 64 位，稳妥。
4. $n\le20$ 很小，但 $20!\approx2.4\times10^{18}$ 极大 $\Rightarrow$ 全排列枚举**不可行**，必须用多项式/指数级但小的算法。

**数据范围与规模感受**

$$1\le n\le 20.$$

- $n!\approx 2.4\times10^{18}$：全排列暴力只能用于 $n\le 8$ 左右的对拍。
- $2^n\le 2^{20}\approx 1.05\times10^6$：**状压 DP** 的状态数完全可接受。
- 状态数 $\times$ 每个状态的转移数 $\approx n\cdot 2^n\le 20\times2^{20}\approx2.1\times10^7$：时间充裕。

这就是典型的「$n\le20$」提示——**状压 DP**。

## 2. 思维链（Chain-of-thought）

### 2.1 explore its property：把问题翻译成"带权完美匹配"

**引理 1（权重定义）**

定义 $w_{i,j}=P_{i,j}\times Q_{j,i}$。则男 $i$ 配女 $j$ 的总优势恰为 $w_{i,j}$，问题成为：求排列 $\sigma$ 最大化 $\sum_{i=1}^{n}w_{i,\sigma(i)}$。

**证明**：题面原文「男运动员 $i$ 和女运动员 $j$ 配对组成混合双打的男女双方竞赛优势为 $P_{i,j}\times Q_{j,i}$」，逐字对应 $w_{i,j}$ 的定义。一个「男女一一配对」即双射 $\sigma:\{$男$\}\to\{$女$\}$，总优势即各配对之和 $\sum_i w_{i,\sigma(i)}$。$\blacksquare$

> 这一步的作用是**先消掉"两套矩阵"带来的视觉干扰**：算好 $w$ 之后，题目退化为最经典的**指派问题（Assignment Problem）求最大权**。

### 2.2 consider easier version：全排列枚举

最直白：枚举全部 $n!$ 个排列，逐个求和取最大。由引理 1，它一定正确。

**引理 2（暴力正确性）**

对每个排列 $\sigma$ 计算 $\sum_i w_{i,\sigma(i)}$ 并取最大值，结果即答案。

**证明**：候选解集合恰为全体排列（每对男女一一配对的全部可能），逐一遍历取最大即为全局最优。$\blacksquare$

但 $n!$ 在 $n=20$ 时达 $2.4\times10^{18}$，**不可行**（只做小规模基准）。

### 2.3 reduce to easier problem：只差"用了哪些女生"这一件事

关键观察：**我们不需要记住"哪几个男生配给了哪几个女生"的完整配对，只需要记住"前 $k$ 个男生用掉了哪些女生"**。

为什么可以只按"前 $k$ 个男生"来推进？因为**男生可以被任意编号、并按顺序 $0,1,2,\dots$ 依次决定其配对**，每一次决定只增加"该男生"与"他所选的女生"，是一个可逐步构造的过程。

**引理 3（状态只需"女生使用集合"）**

设男生按 $0,1,\dots,n-1$ 依次配对。当已为男生 $0..k-1$ 选定伴侣后，设已用女生集合为 $M$。此时**剩余男生 $k..n-1$ 的最优解只依赖于 $M$**（与前面具体谁配给了谁无关）。

**证明**：设前 $k$ 个男生到女生集合 $M$ 的配对之和为 $s$（一个定值，一旦 $\sigma$ 在 $0..k-1$ 上确定即固定）。剩余目标为 $\max_{\sigma'} \sum_{i=k}^{n-1}w_{i,\sigma'(i)}$，其中 $\sigma'$ 是 $\{k,\dots,n-1\}\to\{$女$\}\setminus M$ 的双射。该子问题**只用到"剩余男生集合"（由 $k$ 决定）与"剩余女生集合"（由 $M$ 决定）**，与 $0..k-1$ 的具体指派 $\sigma(0),..,\sigma(k-1)$ 无关。因此可以用"$M$ 下的最优剩余值"来定义状态。$\blacksquare$

于是定义状压 DP：

$$dp[M]=\text{把男生 }0,\dots,\text{popcount}(M)-1\text{ 配给女生集合 }M\text{ 时的最大优势之和}.$$

其中 $M$ 是女生使用集合（位掩码），`popcount(M)` 就是"已配对男生个数" $k$。初值 $dp[\varnothing]=0$；转移：从未用女生 $j$ 配给"下一个男生 $k$"，只增加 $w_{k,j}$：

$$dp[M\cup\{j\}]=\max\bigl(dp[M\cup\{j\}],\;dp[M]+w_{k,j}\bigr),\quad k=\text{popcount}(M),\;j\notin M.$$

答案 $dp[\text{全集}]$。

**引理 4（DP 正确性）**

按上式状态定义与转移，$dp[M]$ 的值等于"把男生 $0..\text{popcount}(M)-1$ 一一配给 $M$ 中女生"的最大优势和；最终 $dp[\text{全集}]$ 即原题答案。

**证明**（对 $|M|$ 归纳）：
- 基例 $|M|=0$：无男生需配，和为 $0$，$dp[\varnothing]=0$ ✓。
- 归纳步：设对所有 $|M|=k$ 成立。取 $|M'|=k+1$。任何给男生 $0..k$ 配 $M'$ 的方案，都必为"某女生 $j\in M'$ 配给了男生 $k"，其余是给男生 $0..k-1$ 配 $M'\setminus\{j\}$ 的方案。于是
  $$dp\text{ 的最优}(M')=\max_{j\in M'}\bigl(dp[M'\setminus\{j\}]+w_{k,j}\bigr),$$
  而 $dp[M'\setminus\{j\}]$（归纳假设）已是后者的最优，故上式即所有方案的枚举最大值，恰为转移式所算。$\blacksquare$

**引理 5（该匹配模型与原题的等价）**

状压 DP 得到的 $\max$ 与引理 1 的带权完美匹配最大值相等。

**证明**：由引理 3、4，$dp[\text{全集}]$ 遍历了所有"男生 $0..n-1$ 到女生全集的双射"，即全部排列；每一步取 $\max$，故等于 $\max_\sigma\sum_i w_{i,\sigma(i)}$。$\blacksquare$

### 2.4 discuss by category：为什么用状压而不是 KM / 费用流

本题是最大权完美匹配，也可用 **KM（匈牙利）算法** $O(n^3)$ 或费用流解决。但：

- $n\le20$ 时 $2^n$ 极小，状压 DP $O(n\cdot2^n)\approx2\times10^7$ **实现最短、常数最小、最好验证**；
- KM/费用流在权可为负、需要"最大化"时还要处理一堆细节（取负、可行性、补边），性价比不如状压；
- 因此本题的正解取**状压 DP**，把 KM 作为同族方法在 §3 记录。

**溢出与哨兵体检**：

- 单元素 $P,Q$ 保证在 `int` 范围、答案也在 `int` 范围，但 $P\times Q$ 的中间乘积、以及累加过程一律用 `long long`，避免中间溢出。
- `dp` 是"取最大"，非法状态需用**极小哨兵**填充。全局数组默认为 $0$，若不填哨兵会把"不可达状态 $0$"当成合法解而算错（本项目风格指南的经典坑）。本实现用 `-1145141919810LL`（114514 家族），远小于任何可能答案（$\ge -n\cdot(\max|P||Q|)\approx -4\times10^{10}$），故安全。

## 3. 模式总结（Pattern Summary）

- explore its property — 把"两套矩阵的配对优势"直接展开成单权重 $w_{i,j}=P_{i,j}Q_{j,i}$，题目归约为最大权完美匹配。
- consider easier version — 先写 $n!$ 全排列暴力拿到"绝对正确但太慢"的基准（$n\le8$ 用于对拍）。
- reduce to easier problem — 观察到按男生编号顺序推进时，历史只需保留"女生使用集合"，从而把 $n!$ 搜索压成 $2^n$ 状态。
- classic problem solution — 状压 DP：$dp[M]$（$|M|=$ 已配男生数）＋逐位转移；答案 $dp[$全集$]$。
- discuss by category — 同族方法 KM/费用流 $O(n^3)$ 与状压 $O(n2^n)$ 的取舍；并用"哨兵填充"防止 0 被误当合法状态。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `n` | `int` | 男女各 $n$ 人 | 读入 | — |
| `P[mxn][mxn]` | `int` | 男优势矩阵 | 读入 | P |
| `Q[mxn][mxn]` | `int` | 女优势矩阵 | 读入 | Q |
| `dp[mxs]` | `i64[]` | `dp[mask]`=把男生 $0..\text{popcount}(mask)-1$ 配给女生集合 `mask` 的最大优势和 | `nf` | DP 表，$2^n$ 项 |
| `nf` | `i64` | 不可达哨兵 `-1145141919810` | — | 114514 家族 |
| `lim` | `int` | $2^n$ | — | 状态总数 |
| `mask` | `int` | 女生使用集合（位掩码） | 遍历 | — |
| `k` | `int` | `popcount(mask)`＝已配好男生数 | 每状态算 | — |
| `j` | `int` | 尝试配给下一个男生的女生 | 遍历 $0..n-1$ | — |

### 4.2 编号步骤

1. 读 $n$、矩阵 $P$、矩阵 $Q$。
2. `lim = 1<<n`；`fill_n(&dp[0], mxs, nf)` 用哨兵**整体**填充；`dp[0]=0`。
3. 对 `mask` 从 $0$ 到 `lim-1`：
   1. 若 `dp[mask]==nf`（不可达）跳过；
   2. `k=popcount(mask)`；若 `k==n` 跳过（已配满）；
   3. 对每个未在 `mask` 中的女生 `j`，用 `dp[mask]+(i64)P[k][j]*Q[j][k]` 更新 `dp[mask|(1<<j)]` 取最大。
4. 输出 `dp[lim-1]`。

### 4.3 复杂度

- 时间：$O(n\cdot2^n)\le20\times2^{20}\approx2.1\times10^7$；实测 $n=20$ $0.0914$ s。
- 空间：`dp` 为 `1<<20` 个 `i64`，约 $8$ MB；$P,Q$ 各 $25\times25$ `int`。

### 4.4 关键不变量

1. `popcount(mask)` 恒等于已配好的男生个数（本实现按男生顺序推进）。
2. `dp[mask]` 为哨兵 `nf` $\iff$ 该状态不可达（实际不会发生，因为任何 $k=\text{popcount}$ 的集合都可达；哨兵主要用于防止 0 被误判）。
3. 任何时刻 `mask` 的位数等于"已经处理过的男生下标上界 $+1$"。

### 4.5 验证状态小结

| 项目 | 状态 |
|---|---|
| 随机对拍 | **通过**：$400$ 组随机用例（$n\le8$，$P,Q\in[-30,30]$ 含负值零值），状压 DP 与全排列暴力逐字节一致 |
| 手工用例 | **通过**：$n=1,P=[5],Q=[7]\to35$ |
| 最坏规模计时 | $n=20$ 随机 $\to$ **$0.0914$ s** |
| 残余风险 | 无；权重逐字按 $P_{i,j}Q_{j,i}$ 计算（已在 §2.1 引理 1 强调） |

## 5. 示例代码

### 5.1 主解：状压 DP（与 `appendix/P1559/P1559.cpp` 逐字节一致）

```cpp
//
// 题目：P1559 运动员最佳匹配问题
// https://www.luogu.com.cn/problem/P1559
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};                                // n<=20 (+5 松弛)
constexpr int mxs{1<<20};
constexpr i64 nf{-1145141919810LL};                   // 114514 家族哨兵

int n;
int P[mxn][mxn],Q[mxn][mxn];
i64 dp[mxs];                                          // dp[mask]=已把前 popcount(mask) 名男生配给 mask 中的女生时的最大优势和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>P[i][j];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>Q[i][j];

	int lim=1<<n;
	fill_n(&dp[0],mxs,nf);                            // 全局数组默认 0,必须显式填哨兵
	dp[0]=0;
	for(int mask=0;mask<lim;++mask)
	{
		if(dp[mask]==nf)
			continue;
		int k=__builtin_popcount((unsigned)mask);     // 已配好的男生数
		if(k==n)
			continue;
		for(int j=0;j<n;++j)
			if(!(mask>>j&1))
			{
				int nm=mask|(1<<j);
				i64 v=dp[mask]+(i64)P[k][j]*Q[j][k];  // 第 k 名男生 配 第 j 名女生
				if(v>dp[nm])
					dp[nm]=v;
			}
	}
	cout<<dp[lim-1]<<'\n';
	return 0;
}
```

### 5.2 对照实现：全排列暴力（仅用于小规模对拍）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 小数据朴素基准:枚举女生的全排列(n<=8)
int P[25][25],Q[25][25];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>P[i][j];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>Q[i][j];
	vector<int> p(n);
	iota(p.begin(),p.end(),0);
	i64 best=-1145141919810LL;
	do
	{
		i64 s=0;
		for(int i=0;i<n;++i)
			s+=(i64)P[i][p[i]]*Q[p[i]][i];   // 男生 i 配女生 p[i]
		if(s>best)
			best=s;
	}while(next_permutation(p.begin(),p.end()));
	cout<<best<<'\n';
	return 0;
}
```

### 5.3 易错点清单

- **权重下标写错**：是 $P_{i,j}\times Q_{j,i}$，不是 $Q_{i,j}$（§2.1 引理 1）。
- **忘记 `fill_n` 哨兵**：全局 `i64` 默认 $0$，若不填哨兵，`dp[mask]==nf` 的不可达判定失效（本题所有状态都可达，影响较小，但仍应遵循规范）。
- **中间乘积用 `int`**：$P\times Q$ 可能超出 `int`，务必先转 `i64` 再乘。
- **`dp` 数组只开到 $2^n$**：$n=20$ 时 `1<<20` 刚好，`mxs` 给了 $2^{20}$ 即够；若 `n` 变大需重算。
- **状态与男生数脱钩**：本解法依赖"$k=\text{popcount}(mask)$ 是下一个待配男生"这一约定，转移时 `j` 只能给男生 `k`，不要给错人。
