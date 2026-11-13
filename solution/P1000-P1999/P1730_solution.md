# P1730 最小密度路径

## 1. 重新读题

给定一张 $N$ 个点（$1 \le N \le 50$）、$M$ 条边（$1 \le M \le 1000$）的加权有向无环图（DAG），边权 $1 \le W \le 10^5$。一条路径的“密度”定义为**路径上所有边的权值和除以边的条数**。

接下来有 $Q$（$1 \le Q \le 10^5$）次独立询问，每次给定起点 $X$ 和终点 $Y$，求从 $X$ 到 $Y$ 的最小路径密度（保留 $3$ 位小数）；若不可达，输出 `OMG!`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：为什么不能直接套 0-1 分数规划二分？

一看到“权值和除以边数最小”，第一反应往往是 0-1 分数规划（二分答案 $\lambda$，将每条边权减去 $\lambda$ 后求最短路）。
但看一眼数据范围：$N \le 50$ 极小，而询问数 $Q \le 10^5$ 极大！如果对每次询问都二分 $50$ 次再跑 DAG 最短路，复杂度高达 $O(Q M \log(\frac{W}{\varepsilon})) \approx 5 \times 10^9$，必定超时。

反过来，正因为 $N \le 50$，在有向无环图（甚至任何简单路径）中，从 $X$ 到 $Y$ 的路径所含的**边数 $k$ 只能取 $1, 2, \dots, N-1$**！

### 2.2 按边数分层的全源最短路 DP（附完整证明）

**引理 1（按边数分层解耦原理）**：定义 $dp[k][i][j]$ 为从顶点 $i$ 到顶点 $j$ 恰好经过 $k$ 条有向边的最小权值和（不存在则为 $+\infty$）。则从 $X$ 到 $Y$ 的最小密度路径的密度等于
$$\text{ans}[X][Y] = \min_{1 \le k \le N-1} \frac{dp[k][X][Y]}{k},$$
且 $dp[k][i][j]$ 满足递推式：
$$dp[k][i][j] = \min_{(u \to j,\, w) \in E} \big( dp[k-1][i][u] + w \big).$$

**证明**：
任意从 $X$ 到 $Y$ 的有向路径若包含 $k$ 条边，由于图为 $N$ 个点的有向无环图，路径不可能经过重复顶点，故必有 $1 \le k \le N - 1$。在边数固定为 $k$ 的前提下，分母 $k$ 是常数，最小化比值 $\frac{\sum w}{k}$ 等价于最小化分子 $\sum w$，其最小值即为 $dp[k][X][Y]$。再对所有可能的边数 $k \in \{1, \dots, N-1\}$ 取最小值，即遍历了所有合法路径。$\blacksquare$

预处理所有 $dp[k][i][j]$ 和 $\text{ans}[i][j]$ 只需 $O(N^2 M + N^3)$ 时间，随后每次询问直接 $O(1)$ 查表输出！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 针对“点数 $N \le 50$ 极小、询问数 $Q \le 10^5$ 极大”的特征，放弃单次二分分数规划，抓住边数 $k \in [1, N-1]$ 的有限性。
- **classic problem solution** — 预处理状态 $dp[k][i][j]$（从 $i$ 到 $j$ 恰好走 $k$ 步的最短路），在 $O(N^2 M)$ 时间内算出全源最优密度表，`O(1)` 响应每次查询（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m, q` | `int` | 点数、边数、询问数 | 标准输入 | 维度 |
| `dp[k][i][j]` | `int [52][52][52]` | 从 $i$ 到 $j$ 恰好走 $k$ 条边的最小权值和 | `dp[0][i][i]=0`，其余 `inf` | layered DP |
| `ans[i][j]` | `double [52][52]` | 从 $i$ 到 $j$ 的最小路径密度 | 预处理求最小比值 | answer table |

### 4.2 算法步骤

1. 读入 $N, M$ 及所有有向边 $(u, v, w)$（若两点间有重边，仅保留最小边权 $g[u][v]$）。
2. 初始化 `dp` 全为 `inf`，对每个起点 $i$ 置 `dp[0][i][i] = 0`。
3. 枚举步数 $k = 1 \dots N-1$、起点 $i = 1 \dots N$、中转点 $u = 1 \dots N$（若 `dp[k-1][i][u] == inf` 则跳过）、终点 $v = 1 \dots N$：用 `dp[k-1][i][u] + g[u][v]` 更新 `dp[k][i][v]`。
4. 对所有点对 $(i, j)$ 计算 `ans[i][j] = min_{1 <= k < N} (double)dp[k][i][j] / k`。
5. 读入 $Q$ 次询问 $(x, y)$，$O(1)$ 查表输出。

### 4.3 复杂度分析

- **时间复杂度**：预处理耗时 $O(N^4)$（不超过 $50^4 \approx 6.25 \times 10^6$ 次基本操作），每次询问 $O(1)$，总时间复杂度为 $O(N^4 + Q)$。
- **空间复杂度**：三维数组 `dp[52][52][52]` 占约 $550\text{ KB}$，空间复杂度为 $O(N^3)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1730 最小密度路径
// https://www.luogu.com.cn/problem/P1730
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};
constexpr int inf{0x3f3f3f3f};

int n,m,q;
int g[mxn][mxn];
int dp[mxn][mxn][mxn];
double ans[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	memset(g,0x3f,sizeof(g));
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;
		g[u][v]=min(g[u][v],w);
	}

	memset(dp,0x3f,sizeof(dp));
	for(int i=1;i<=n;++i)
		dp[0][i][i]=0;

	for(int k=1;k<n;++k)
		for(int i=1;i<=n;++i)
			for(int u=1;u<=n;++u)
				if(dp[k-1][i][u]<inf)
					for(int v=1;v<=n;++v)
						if(g[u][v]<inf)
							dp[k][i][v]=min(dp[k][i][v],dp[k-1][i][u]+g[u][v]);

	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
		{
			ans[i][j]=1e18;
			for(int k=1;k<n;++k)
				if(dp[k][i][j]<inf)
					ans[i][j]=min(ans[i][j],(double)dp[k][i][j]/k);
		}

	cin>>q;
	cout<<fixed<<setprecision(3);
	while(q--)
	{
		int x,y;
		cin>>x>>y;
		if(ans[x][y]>1e17)
			cout<<"OMG!\n";
		else
			cout<<ans[x][y]<<'\n';
	}
	return 0;
}
```
