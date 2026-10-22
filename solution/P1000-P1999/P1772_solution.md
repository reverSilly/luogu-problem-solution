# P1772 [ZJOI2006] 物流运输

## 1. 重新读题

共有 $n$ 天（$1 \le n \le 100$）和 $m$ 个码头（$1 \le m \le 20$），每天需要从 $1$ 号码头运货到 $m$ 号码头。给定 $e$ 条双向航线及各自的单位成本，以及若干条封锁信息“码头 $p$ 在第 $[a, b]$ 天不可用”。

如果连续若干天使用完全相同的运输路线，期间无需支付改道费；每次改变运输路线需要额外支付成本 $k$。求完成全部 $n$ 天运输的最小总成本。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：按“不改道的连续天数区间”分段 DP

既然每次改变路线需要额外支付 $k$，任何 $n$ 天的运输方案都可以唯一划分为若干个**连续使用同一条路线的时间段** $[j+1, i]$！

**引理 1（时间段内最优固定路线与分段 DP 方程）**：
在第 $[l, r]$ 天内始终使用同一条路线，该路线合法当且仅当它不经过任何在 $[l, r]$ 期间曾被封锁哪怕一天的码头。因此，定义 $\text{cost}[l][r]$ 为仅使用在 $[l, r]$ 期间**全程可用**的码头时，从 $1$ 到 $m$ 的最短路长度（不可达则为 $+\infty$）。
设 $dp[i]$ 表示前 $i$ 天的最小总成本，则：
$$dp[i] = \min_{0 \le j < i,\ \text{cost}[j+1][i] < \infty} \Big( dp[j] + (i - j) \cdot \text{cost}[j+1][i] + (j > 0 ? k : 0) \Big).$$

**证明**：
在时间段 $[j+1, i]$ 内固定使用一条合法路线 $P$，共行驶 $(i - j)$ 天，其运输成本为 $(i - j) \cdot \text{len}(P)$，且与 $[j+1, i]$ 之外的决策完全无关，故必然应当取所有在 $[j+1, i]$ 全程合法的路线中最短的一条，即 $\text{cost}[j+1][i]$。对最后一段的起点 $j+1$ 枚举取最小值（首段 $j=0$ 不收改道费，其余加改道费 $k$），即覆盖了所有分段方案。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将“改道收费”转化为将时间轴 $[1, n]$ 划分为若干连续同路线区间 $[j+1, i]$。
- **classic problem solution** — 枚举所有时间区间 $[l, r]$ 用 Dijkstra 预处理全勤可用图上的最短路 $\text{cost}[l][r]$，再用 $O(n^2)$ 线性 DP 求最小总代价（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m, k, e` | `int` | 天数、码头数、改道费、航线数 | 标准输入 | 维度 |
| `ban[p][d]` | `bool [25][105]` | 码头 $p$ 在第 $d$ 天是否关闭 | 读入标记 | banned |
| `cost[l][r]` | `i64 [105][105]` | 区间 $[l, r]$ 全程可用码头下的最短路 | Dijkstra 预处理 | interval shortest path |
| `dp[i]` | `i64 [105]` | 前 $i$ 天的最小总成本 | DP 计算 | DP table |

### 4.2 算法步骤

1. 读入 $n, m, k, e$、双向边及封锁区间，标记 `ban[p][d] = true`。
2. 枚举所有区间 $1 \le l \le r \le n$：标出在 $d \in [l, r]$ 中出现过 `ban[p][d]` 的禁用码头，在剩余可用码头上跑一次从 $1$ 到 $m$ 的 Dijkstra 得到 `cost[l][r]`。
3. 依次计算 $i = 1 \dots n$ 的 `dp[i]`：枚举 $j = 0 \dots i-1$，若 `cost[j+1][i] < inf`，用 `dp[j] + (i - j) * cost[j+1][i] + (j ? k : 0)` 更新 `dp[i]`。
4. 输出 `dp[n]`。

### 4.3 复杂度分析

- **时间复杂度**：共有 $\frac{n(n+1)}{2} \approx 5000$ 个时间区间，每次 Dijkstra 耗时 $O(m^2 + e)$，预处理与 DP 总时间复杂度为 $O(n^2(m^2 + e))$（运算量约 $2 \times 10^6$，耗时 $< 5\text{ ms}$）。
- **空间复杂度**：代价矩阵 `cost[105][105]` 与图结构，空间复杂度为 $O(n^2 + m + e)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1772 [ZJOI2006] 物流运输
// https://www.luogu.com.cn/problem/P1772
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 inf{1145141919810LL};

int n,m,k,e_cnt;
vector<pair<int,int>>graph[25];
bool ban[25][105], dead[25],vis[25];
i64 dis[25],cost[105][105],dp[105];

i64 dijkstra()
{
	fill_n(dis+1,m,inf);
	fill_n(vis+1,m,false);
	if(dead[1]||dead[m])
		return inf;
	dis[1]=0;
	for(int it=1;it<=m;++it)
	{
		int u=0;
		for(int i=1;i<=m;++i)
			if(!vis[i]&&!dead[i]&&(u==0||dis[i]<dis[u]))
				u=i;
		if(!u||dis[u]==inf)
			break;
		vis[u]=true;
		for(auto[v,w]:graph[u])
			if(!dead[v]&&dis[u]+w<dis[v])
				dis[v]=dis[u]+w;
	}
	return dis[m];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k>>e_cnt;
	for(int i=0,u,v,w;i<e_cnt;++i)
	{
		cin>>u>>v>>w;
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}
	int d;
	cin>>d;
	for(int i=0,p,a,b;i<d;++i)
	{
		cin>>p>>a>>b;
		for(int t=a;t<=b;++t)
			ban[p][t]=true;
	}

	for(int l=1;l<=n;++l)
	{
		fill_n(dead+1,m,false);
		for(int r=l;r<=n;++r)
		{
			for(int p=1;p<=m;++p)
				if(ban[p][r])
					dead[p]=true;
			cost[l][r]=dijkstra();
		}
	}

	for(int i=1;i<=n;++i)
	{
		dp[i]=inf;
		for(int j=0;j<i;++j)
			if(cost[j+1][i]<inf)
				dp[i]=min(dp[i],dp[j]+(i-j)*cost[j+1][i]+(j>0?k:0));
	}
	cout<<dp[n]<<'\n';
	return 0;
}
```
