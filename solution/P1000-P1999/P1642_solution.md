# P1642 规划 题解

> 标签：0/1 分数规划 / 二分答案 / 树形 DP / 树上背包 · 难度：省选/NOI-

## 1. 重新读题

**问题重述（人话版）**

给定一棵包含 $N$ 个工厂的树，第 $i$ 个工厂有产值 $w_i$ 和污染值 $c_i$。现在要拆除其中恰好 $M$ 个工厂，使得**保留下来的 $K = N - M$ 个工厂在原树上仍然构成一个连通块**，并最大化保留工厂的比值：
$$\frac{\sum_{i \in S} w_i}{\sum_{i \in S} c_i}.$$
输出该最大比值，保留一位小数。

**约束与规模暗示**

- $1 < N < 100$，$1 \le M < N$，$1 \le w_i, c_i \le 10000$。
- 规模暗示：$N < 100$ 允许 $O(N^2)$ 的树上背包作为单次判定器，配合二分答案（0/1 分数规划）在 $O(N^2 \log(\frac{W}{\varepsilon}))$ 内解决。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：从“拆 $M$ 个点”到“选大小为 $K = N - M$ 的连通子图”

题目说“拆除 $M$ 个工厂，使得剩下的工厂依然连成一片”，这在图论上完全等价于：**在树上选出一个包含恰好 $K = N - M$ 个顶点的连通导出子图 $S$**。而在有根树上，任何非空连通子图 $S$ 都有唯一深度最小的顶点 $u$，即 $S$ 是以某个节点 $u$ 为根、大小恰为 $K$ 的树上连通块。

### 2.2 归约到经典算法：0/1 分数规划 + 树上连通块背包 DP

**引理 1（0/1 分数规划的线性化判定充要条件）**

记可行集合 $\mathcal{F}$ 为树上所有大小恰为 $K = N - M$ 的连通点集。对于任意实数 $\lambda \ge 0$：
$$\max_{S \in \mathcal{F}} \frac{\sum_{i \in S} w_i}{\sum_{i \in S} c_i} \ge \lambda \iff \max_{S \in \mathcal{F}} \sum_{i \in S} (w_i - \lambda c_i) \ge 0.$$

**证明**：
因为 $c_i \ge 1 > 0$ 且 $K \ge 1$，对任意 $S \in \mathcal{F}$ 都有分母 $\sum_{i \in S} c_i > 0$。故不等式 $\frac{\sum_{i \in S} w_i}{\sum_{i \in S} c_i} \ge \lambda$ 两边同乘正数 $\sum_{i \in S} c_i$ 保持方向不变，化为 $\sum_{i \in S} (w_i - \lambda c_i) \ge 0$。对所有 $S \in \mathcal{F}$ 取最大值即证。$\blacksquare$

**引理 2（树上背包的上下界裁剪复杂度为 $O(N K)$）**

赋予每个点权值 $\text{val}[u] = w_u - \lambda c_u$，设 $dp[u][j]$ 表示在 $u$ 的子树中、包含 $u$ 且大小恰为 $j$ 的连通块的最大权值和。在合并子树 $v$ 到 $u$ 时，只枚举 $i \in [1, \min(K, \text{sz}[u])]$ 与 $j \in [1, \min(K - i, \text{sz}[v])]$，单次 DFS 总时间复杂度为 $O(N K) \le O(N^2)$。

**证明**：
在内层二重循环中，每执行一次状态转移 $(i, j)$，等价于在当前已合并的 $u$ 子树部分取前 $i$ 个点、在 $v$ 子树取前 $j$ 个点；从点对视角看，任意两个节点 $(x, y)$ 仅在它们的最近公共祖先 $\operatorname{LCA}(x, y) = u$ 合并对应子树分支时才可能对循环次数产生至多 $1$ 次贡献，故整棵树合并的总转移次数不超过 $\binom{N}{2} = O(N^2)$。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将“删去 $M$ 个点后剩余点连通”转化为“在树上选大小恰为 $K = N - M$ 的连通块”。
- **consider easier version** — 若点权固定，求树上大小为 $K$ 的最大权连通块即为经典树上连通块背包。
- **classic problem solution** — 应用 0/1 分数规划二分比值 $\lambda$，将分式最大化归约为点权 $w_i - \lambda c_i$ 的最大权连通块符号判定（引理 1）。
- **classic problem solution** — 用带子树大小上界裁剪的树上背包在 $O(NK)$ 时间内求出 $\max_u dp[u][K]$（引理 2）。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `n, m, k` | `int` | 总点数 $N$、拆除数 $M$、保留数 $K=N-M$ | 读入/计算 | 连通块规模 |
| `w[mxn], c[mxn]` | `int [105]` | 每个工厂的产值与污染值 | 读入 | 分子与分母项 |
| `val[mxn]` | `double [105]` | 二分判定下的折算点权 $w_i - \lambda c_i$ | 动态计算 | 0/1 分数规划权值 |
| `dp[u][j]` | `double [105][105]` | $u$ 子树内含 $u$ 且大小为 $j$ 的最大权值和 | `dp[u][1]=val[u]`，余 `-inf` | 树上背包表 |

### 4.2 算法步骤

1. 读入 $N, M$（令 $K = N - M$）、数组 `w[]`、`c[]` 以及树的 $N-1$ 条边。
2. 在区间 $[0, 10000]$ 上固定迭代二分 $60$ 次：
   - 令 `mid = (l + r) / 2`，赋权 `val[i] = w[i] - mid * c[i]`；
   - 以 $0$ 为根执行树上背包 `dfs(0, -1)`；
   - 若存在节点 $i$ 满足 `sz[i] >= k && dp[i][k] >= 0` 则 `l = mid`，否则 `r = mid`。
3. 按保留一位小数输出 `l`。

### 4.3 复杂度与验证报告

- **时间复杂度**：$O(\text{iter} \cdot N K)$，其中 $\text{iter} = 60$，$N < 100$，总运算量约 $3 \times 10^5$。
- **空间复杂度**：$O(N^2)$。
- **对拍验证**：`appendix/P1642/P1642_check.py` 中与 $O(2^N \cdot N)$ 连通子集穷举基准完成 **400 组随机树对拍**全部一致；极限规模 $N = 99$ 实测耗时 **0.0121 s**。

## 5. 示例代码

```cpp
//
// 题目：P1642 规划
// https://www.luogu.com.cn/problem/P1642
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr double inf{1e18};

int n,m,k,w[mxn],c[mxn],sz[mxn];
double val[mxn],dp[mxn][mxn],tmp[mxn];
vector<int>graph[mxn];

void dfs(int u,int fa)
{
	sz[u]=1;
	dp[u][1]=val[u];
	for(int j=2;j<=k;++j)
		dp[u][j]=-inf;

	for(int v:graph[u])
	{
		if(v==fa)
			continue;
		dfs(v,u);
		int lim=min(k,sz[u]+sz[v]);
		for(int j=1;j<=lim;++j)
			tmp[j]=dp[u][j];
		for(int i=1;i<=min(k,sz[u]);++i)
		{
			if(dp[u][i]<-inf/2)
				continue;
			for(int j=1;j<=min(k-i,sz[v]);++j)
				if(dp[v][j]>-inf/2)
					tmp[i+j]=max(tmp[i+j],dp[u][i]+dp[v][j]);
		}
		sz[u]+=sz[v];
		for(int j=1;j<=lim;++j)
			dp[u][j]=tmp[j];
	}
}

bool check(double mid)
{
	for(int i=0;i<n;++i)
		val[i]=w[i]-mid*c[i];
	dfs(0,-1);
	for(int i=0;i<n;++i)
		if(sz[i]>=k&&dp[i][k]>=0)
			return true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	k=n-m;
	for(int i=0;i<n;++i)
		cin>>w[i];
	for(int i=0;i<n;++i)
		cin>>c[i];
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	double l=0,r=1e4;
	for(int it=0;it<60;++it)
	{
		double mid=(l+r)/2;
		if(check(mid))
			l=mid;
		else
			r=mid;
	}
	cout<<fixed<<setprecision(1)<<l<<'\n';
	return 0;
}
```
