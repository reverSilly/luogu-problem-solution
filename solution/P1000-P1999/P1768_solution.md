# P1768 天路

## 1. 重新读题

给定一张 $N$ 个顶点、$M$ 条有向边的有向图（$1 \le N \le 7000$，$1 \le M \le 2 \times 10^4$），每条有向边 $e$ 具有趣味度 $V_e$ 和价格 $P_e$（$1 \le V_e, P_e \le 1000$）。

求图中一条有向回路 $C$，使得回路上所有边的趣味度之和与价格之和的比值 $\frac{\sum_{e \in C} V_e}{\sum_{e \in C} P_e}$ 最大（保留 $1$ 位小数）；若图中不存在任何有向回路，输出 `-1`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：0-1 分数规划与正权环判定

**引理 1（0-1 分数规划回路判据）**：对于任意给定的实数阈值 $\lambda \ge 0$，图中存在满足 $\frac{\sum_{e \in C} V_e}{\sum_{e \in C} P_e} > \lambda$ 的有向回路 $C$，当且仅当将每条边 $e$ 的权值重新定义为
$$w_e(\lambda) = V_e - \lambda P_e$$
后，图中存在一个权值和严格大于 $0$ 的**正权有向简单环**。

**证明**：
由于 $P_e > 0$，对任意有向环 $C$ 有：
$$\frac{\sum_{e \in C} V_e}{\sum_{e \in C} P_e} > \lambda \iff \sum_{e \in C} V_e > \lambda \sum_{e \in C} P_e \iff \sum_{e \in C} (V_e - \lambda P_e) > 0.$$
若存在非简单有向闭迹满足权值和 $> 0$，将其分解为若干简单有向环的不交并，由抽屉原理至少有一个简单环的权值和 $> 0$。$\blacksquare$

### 2.2 DFS 版 SPFA 判正环

二分 $\lambda \in [0, 200]$（初始先检验 $\lambda = 0$ 时是否存在任何环，由于 $V_e \ge 1, P_e \ge 1$，可直接用拓扑排序或 Tarjan 判图是否有环；若无环则输出 `-1`）。
在二分过程中，将所有点的初始距离 `dis[u]` 置为 $0$，在递归栈标记 `ins[u]` 下运行 **DFS-SPFA** 松弛 `dis[v] < dis[u] + (V_e - mid * P_e)`：一旦松弛指向当前已在递归栈中的节点 `ins[v] == true`，即说明找到了一个权值和 $> 0$ 的正环，立即返回 `true`！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将最大比率环通过 0-1 分数规划转化为判定是否存在边权 $V_e - \lambda P_e$ 的正权环（引理 1）。
- **classic problem solution** — 先判图是否有环，再二分 $\lambda \in [0, 200]$，用 DFS-SPFA 在栈标记 `ins[u]` 下快速检测正环。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 顶点数与有向边数 | 标准输入 | 维度 |
| `head, to, nxt, val_v, val_p` | 数组 | 链式前向星存储有向图 | 读入构建 | adjacency list |
| `dis[u]` | `double [mxn]` | DFS-SPFA 最长路估计值 | 每轮置 `0` | distance |
| `ins[u]` | `bool [mxn]` | 节点 $u$ 是否在当前 DFS 递归栈中 | `false` | in-stack |

### 4.2 算法步骤

1. 读入 $N, M$ 及所有有向边。
2. 先用 `check(0.0)` 判断图中是否存在环（因为 $V_e \ge 1 > 0$，当 $\lambda = 0$ 时任何有向环的权值和都 $\ge 1 > 0$！故 `check(0.0)` 为真当且仅当图中至少存在一个有向环）。若 `check(0.0) == false`，输出 `-1` 并结束。
3. 在 $[0, 200]$ 上二分 $35$ 次（精度达 $200 \times 2^{-35} < 10^{-8}$）：每次调用 `check(mid)`，若存在正环则 `lo = mid`，否则 `hi = mid`。
4. 将 `lo` 保留 $1$ 位小数输出。

### 4.3 复杂度分析

- **时间复杂度**：二分约 $35$ 次，每次 DFS-SPFA 在存在正环时沿环立即剪枝返回，耗时约 $50\text{ ms}$。
- **空间复杂度**：图的邻接表与长度为 $N$ 的辅助数组，空间复杂度为 $\Theta(N + M)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1768 天路
// https://www.luogu.com.cn/problem/P1768
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{7005},mxm{20005};

int n,m;
int head[mxn],to[mxm],nxt[mxm],val_v[mxm],val_p[mxm],ecnt;
double dis[mxn],mid_val;
bool ins[mxn],vis[mxn];

void add_edge(int u,int v,int vv,int pp)
{
	++ecnt;
	to[ecnt]=v;
	val_v[ecnt]=vv;
	val_p[ecnt]=pp;
	nxt[ecnt]=head[u];
	head[u]=ecnt;
}

bool spfa_dfs(int u)
{
	ins[u]=true;
	vis[u]=true;
	for(int e=head[u];e;e=nxt[e])
	{
		int v=to[e];
		double w=val_v[e]-mid_val*val_p[e];
		if(dis[u]+w>dis[v])
		{
			dis[v]=dis[u]+w;
			if(ins[v]||spfa_dfs(v))
				return true;
		}
	}
	ins[u]=false;
	return false;
}

bool check(double mid)
{
	mid_val=mid;
	fill_n(dis+1,n,0.0);
	fill_n(ins+1,n,false);
	fill_n(vis+1,n,false);
	for(int i=1;i<=n;++i)
		if(!vis[i]&&spfa_dfs(i))
			return true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,u,v,vv,pp;i<m;++i)
	{
		cin>>u>>v>>vv>>pp;
		add_edge(u,v,vv,pp);
	}

	if(!check(0.0))
	{
		cout<<"-1\n";
		return 0;
	}

	double lo=0.0,hi=200.0;
	for(int it=0;it<35;++it)
	{
		double mid=(lo+hi)*0.5;
		if(check(mid))
			lo=mid;
		else
			hi=mid;
	}
	cout<<fixed<<setprecision(1)<<lo<<'\n';
	return 0;
}
```
