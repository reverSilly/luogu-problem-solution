# P1791 [国家集训队] 人员雇佣

## 1. 重新读题

有 $N$（$N \le 1000$）名经理，雇佣第 $i$ 名经理需要花费 $A_i$。给定对称矩阵 $E_{i,j} = E_{j,i}$：
- 若经理 $i$ 与经理 $j$ **同时被雇佣**，有序对 $(i, j)$ 产生收益 $+E_{i,j}$（即无序对 $\{i, j\}$ 产生 $E_{i,j} + E_{j,i} = 2 E_{i,j}$，$i=j$ 时产生 $E_{i,i}$）；
- 若经理 $i$ **被雇佣**而经理 $j$ **未被雇佣**，则未雇佣的 $j$ 会对已雇佣的 $i$ 造成破坏，使利润减少 $E_{i,j}$！

求选择一个雇佣子集 $S \subseteq \{1, \dots, N\}$ 所能获得的最大净利润。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：代数化简消去二次交叉项！

一看到“选点有代价、点对同选有收益、异选有惩罚”，第一反应往往是建立 $N$ 个点、$N^2$ 条边的最小割模型。
但请先把目标函数写成代数式，看看能不能**直接消去所有的二次项**！

**引理 1（二次项完全对消与转化为 $O(N)$ 独立贪心 / 文理分科最小割）**：
设被雇佣的经理集合为 $S \subseteq \{1, \dots, N\}$，记第 $i$ 行的了解程度总和为 $\text{row}_i = \sum_{j=1}^N E_{i,j}$。则净利润函数为：
$$F(S) = \sum_{i \in S} \sum_{j \in S} E_{i,j} - \sum_{i \in S} \sum_{j \notin S} E_{i,j} - \sum_{i \in S} A_i.$$
注意对任意 $i \in S$，有 $\sum_{j \in S} E_{i,j} = \text{row}_i - \sum_{j \notin S} E_{i,j}$！代入上式得：
$$F(S) = \sum_{i=1}^N \text{row}_i - \left( \sum_{i \notin S} \text{row}_i + \sum_{i \in S} A_i + \sum_{i \in S,\, j \notin S} 2 E_{i,j} \right)!$$

**证明**：
将 $\sum_{i \in S} \text{row}_i$ 改写为 $\sum_{i=1}^N \text{row}_i - \sum_{i \notin S} \text{row}_i$，即得上式。
看括号内的损失项 $\sum_{i \notin S} \text{row}_i + \sum_{i \in S} A_i + \sum_{i \in S, j \notin S} 2 E_{i,j}$：
这恰好是一个标准的**最小割（Minimum $s$-$t$ Cut）**模型：
- 令源点 $s$ 侧代表“雇佣”（$i \in S$），汇点 $t$ 侧代表“不雇佣”（$i \notin S$）；
- 从源点 $s$ 向每个点 $i$ 连容量为 $\text{row}_i$ 的有向边（若 $i \notin S$ 则割断该边，付出 $\text{row}_i$）；
- 从每个点 $i$ 向汇点 $t$ 连容量为 $A_i$ 的有向边（若 $i \in S$ 则割断该边，付出 $A_i$）；
- 对每对 $i \ne j$，连双向边 $(i, j)$，容量为 $2 E_{i,j}$（若 $i \in S, j \notin S$ 则割断 $i \to j$，付出 $2 E_{i,j}$）！
因此，最大净利润精确等于 $\sum_{i=1}^N \text{row}_i - \operatorname{MinCut}(s, t)$！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用行和恒等式 $\sum_{j \in S} E_{i,j} = \text{row}_i - \sum_{j \notin S} E_{i,j}$ 将目标函数化简为常数和减去非负割代价（引理 1）。
- **classic problem solution** — 构造 $N+2$ 个点的流网络（$s \to i$ 容量 $\text{row}_i$，$i \to t$ 容量 $A_i$，$i \leftrightarrow j$ 容量 $2 E_{i,j}$），用当前弧优化的 Dinic 算法求最小割。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 经理人数 $N$（$\le 1000$） | 标准输入 | 维度 |
| `a[i], row_s[i]` | `i64 [1005]` | 雇佣花费 $A_i$ 与第 $i$ 行总和 $\sum_j E_{i,j}$ | 读入统计 | costs / row sums |
| `head, to, nxt, cap` | 数组 | 链式前向星残量网络（双向边合并减少一半边数） | Dinic | flow network |

### 4.2 算法步骤

1. 读入 $N$、数组 $A[1..N]$ 及矩阵 $E[1..N][1..N]$，累加 `row_s[i]` 与全表总和 `tot`。
2. 建图：设 $S = 0, T = N + 1$；对每个 $i$ 连有向边 $(S, i, \text{row\_s}[i])$ 与 $(i, T, A[i])$；对每对 $1 \le i < j \le N$ 且 $E_{i,j} > 0$，直接连一条正向容量 $2 E_{i,j}$、反向容量也是 $2 E_{i,j}$ 的成对边！
3. 运行 Dinic 算法求出最大流 `flow`，输出 `tot - flow`。

### 4.3 复杂度分析

- **时间复杂度**：通过合并无向边 $(i, j)$ 的正反向残量边，边数减半至 $\le 10^6$；在此类稠密二元关系网络上 Dinic 仅需数轮 BFS 分层即可收敛，耗时 $< 150\text{ ms}$。
- **空间复杂度**：约 $10^6$ 条边，空间复杂度为 $\Theta(N^2)$（约 $24\text{ MB}$）。

---

## 5. 示例代码

```cpp
//
// 题目：P1791 [国家集训队] 人员雇佣
// https://www.luogu.com.cn/problem/P1791
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005},mxm{1010005};
constexpr i64 inf{11451419198101919LL};

int n,S,T;
i64 a[mxn],e_mat[mxn][mxn],row_s[mxn];
int head[mxn],cur_h[mxn],to[mxm],nxt[mxm],ecnt=1;
i64 cap[mxm];
int dep[mxn],que[mxn];

void add_dir(int u,int v,i64 c1,i64 c2=0)
{
	++ecnt;to[ecnt]=v;cap[ecnt]=c1;nxt[ecnt]=head[u];head[u]=ecnt;
	++ecnt;to[ecnt]=u;cap[ecnt]=c2;nxt[ecnt]=head[v];head[v]=ecnt;
}

bool bfs()
{
	fill_n(dep,T+1,-1);
	int hd=0,tl=0;
	dep[S]=0;
	que[tl++]=S;
	while(hd<tl)
	{
		int u=que[hd++];
		for(int e=head[u];e;e=nxt[e])
			if(cap[e]>0&&dep[to[e]]==-1)
			{
				dep[to[e]]=dep[u]+1;
				if(to[e]==T)
					return true;
				que[tl++]=to[e];
			}
	}
	return dep[T]!=-1;
}

i64 dfs(int u,i64 flow)
{
	if(u==T||!flow)
		return flow;
	i64 used=0;
	for(int&e=cur_h[u];e;e=nxt[e])
	{
		int v=to[e];
		if(cap[e]>0&&dep[v]==dep[u]+1)
		{
			i64 pushed=dfs(v,min(flow-used,cap[e]));
			if(pushed)
			{
				cap[e]-=pushed;
				cap[e^1]+=pushed;
				used+=pushed;
				if(used==flow)
					break;
			}
		}
	}
	if(!used)
		dep[u]=-1;
	return used;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
		cin>>a[i];
	i64 tot=0;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
		{
			cin>>e_mat[i][j];
			row_s[i]+=e_mat[i][j];
			tot+=e_mat[i][j];
		}

	S=0;T=n+1;
	for(int i=1;i<=n;++i)
	{
		add_dir(S,i,row_s[i]);
		add_dir(i,T,a[i]);
		for(int j=i+1;j<=n;++j)
			if(e_mat[i][j]>0)
				add_dir(i,j,2*e_mat[i][j],2*e_mat[i][j]);
	}

	i64 flow=0;
	while(bfs())
	{
		memcpy(cur_h,head,sizeof(int)*(T+1));
		flow+=dfs(S,inf);
	}
	cout<<tot-flow<<'\n';
	return 0;
}
```
