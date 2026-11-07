# P1907 设计道路

## 1. 重新读题

### 1.1 问题重述（人话版）

平面上有 $n+2$ 个点：$1\sim n$ 是普通路口，$n+1$ 是 Caesar 所在的码头，$n+2$ 是你家。**任意两点之间都有一条边**（完全图）。每条边有两种身份：

- **Rome Road**：边权为 $d_R\times$ 两点间欧氏距离；
- **Dirt Road**：边权为 $d_D\times$ 两点间欧氏距离。

题目给出 $d_D,d_R$（保证 $d_R<d_D$，即 Rome Road 更「便宜」但未必更短），以及一张 Rome Road 的边表；剩下所有边都是 Dirt Road。特别地，**与 $n+1$、$n+2$ 相连的所有边都是 Dirt Road**。

求 $n+1$ 到 $n+2$ 的最短路边权和，保留 $4$ 位小数。

### 1.2 关键约束与规模暗示

- $n\le 1000$，所以点数 $V=n+2\le 1002$；完全图的边数约 $V^2/2\approx5\times10^5$。
- Rome Road 不超过 $200$ 条——这个数字很小，说明 Rome Road 是「稀疏的特殊边」，但 Dijkstra 本身并不需要利用这一点。
- $0.1\le d_R<d_D\le10^3$，$0\le x_i,y_i\le10^4$，坐标最多 $2$ 位小数：边权都是非负实数，Dijkstra 适用。
- $V\le1002$ 时，**邻接矩阵 + $O(V^2)$ 的朴素 Dijkstra** 只要约 $10^6$ 次松弛，完全够用；甚至把完全图显式建出来跑堆优化也可以。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先明确图的规模与边权

点集大小 $V=n+2\le1002$。对任意一对 $(i,j)$，边权

$$
w(i,j)=c(i,j)\cdot\sqrt{(x_i-x_j)^2+(y_i-y_j)^2},\qquad
c(i,j)=\begin{cases}d_R,&(i,j)\in S\\ d_D,&(i,j)\notin S\end{cases}
$$

全部非负（$d_R,d_D>0$，距离 $\ge0$）。要求单源最短路。

### 2.2 能不能直接连一条边？

直觉上「两点之间线段最短」，但这里有一条**直接边 $n+1\to n+2$ 是 Dirt Road**，而绕路可能走很多 Rome Road。由于 $d_R<d_D$，绕远路但走便宜的 Rome Road 完全可能更优。

反例（也是后面的自测）：$d_D=100,d_R=1$，码头与家水平相距 $9$，中间有两个路口几乎贴在两端，且它们之间是 Rome Road。直连代价 $900$，而「短 Dirt 段 + 长 Rome 段 + 短 Dirt 段」的代价约为 $100+9+100=209$。所以必须老老实实跑最短路，不能只看直线。

### 2.3 选哪种最短路实现

- 边数 $E=\Theta(V^2)\approx5\times10^5$。
- 堆优化 Dijkstra：$O(E\log V)\approx5\times10^5\times10\approx5\times10^6$，可以。
- 朴素 Dijkstra（邻接矩阵，每轮 $O(V)$ 找最小 + $O(V)$ 松弛）：$O(V^2)\approx10^6$，更快更短，且不需要存边表。
- 本题 $V$ 小、图稠密，**朴素 $O(V^2)$ 是更自然的选择**；边权可以在松弛时用坐标现算，连矩阵都不用存。

**引理 1（Dijkstra 适用）**：所有边权非负，因此 Dijkstra 求出的 $dis[n+2]$ 就是真正的最短路长度。

**证明**：Dijkstra 的正确性只要求边权非负。这里每条边权是「正系数 $\times$ 非负距离」，故 $\ge0$。$\square$

**引理 2（只需考虑简单路径）**：由于边权非负，最短路一定可以取成不重复经过点的简单路径，因此把每个点只确定一次距离的 Dijkstra 不会漏解。

**证明**：若最短路重复经过某点，把两次出现之间的环删掉，路径长度不减（删掉的是非负权回路），得到更短或等长的简单路径。$\square$

### 2.4 细节

- 读入顺序是：$d_D,d_R$；$n$；接下来 $n$ 行点 $1\sim n$ 的坐标；然后若干行 Rome Road 的 $(u,v)$，以 `0 0` 结束；再两行分别是点 $n+1$、$n+2$ 的坐标。**Rome Road 列表夹在中间**，读的时候要把它们按点对存进一个布尔矩阵。
- 内部下标 0-based：点 $1\sim n$ 变成 $0\sim n-1$，点 $n+1$ 变成 $n$，点 $n+2$ 变成 $n+1$。
- 只有 $1\le u,v\le n$ 的 Rome Road，不会出现在 $n+1,n+2$ 上（题目保证）。
- 输出 `fixed<<setprecision(4)`。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 图是完全图，边权 = 系数 $\times$ 欧氏距离，全部非负；绕路可能因 Rome Road 更便宜。
- **reduce to easier problem** — 不必利用 Rome Road 稀疏性，直接把它当作稠密图上的单源最短路。
- **classic problem solution** — 邻接矩阵的朴素 Dijkstra，$O(V^2)$，边权现算。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `dD,dR` | `double` | Dirt / Rome 的系数 | 读入 | 系数 |
| `n` | `int` | 普通路口数 | 读入 | 节点数 |
| `x[i],y[i]` | `double [1005]` | 第 $i$ 个点坐标（0-based，$n,n+1$ 为起终点） | 读入 | 坐标 |
| `rome[i][j]` | `bool [1005][1005]` | 边 $(i,j)$ 是否为 Rome Road | `false` | 身份 |
| `dis[i]` | `double [1005]` | 从源点 $n$ 到 $i$ 的当前最短路 | $+\infty$，`dis[n]=0` | distance |
| `vis[i]` | `bool [1005]` | 是否已确定最短路 | `false` | visited |

### 4.2 算法步骤

1. 读入 $d_D,d_R$、$n$、前 $n$ 个点坐标。
2. 循环读 `u,v`，读到 `0 0` 停止；把 `u-1,v-1` 的 `rome` 矩阵双向置 `true`。
3. 读入点 $n$、$n+1$ 的坐标。
4. 初始化 `dis[n]=0`，其余 $+\infty$。重复 $V=n+2$ 轮：
   - 在未访问点中找 `dis` 最小者 `u`（找不到或为无穷则退出）；
   - 标记 `vis[u]`；
   - 对所有未访问的 `v`，用 `dis[u]+w(u,v)` 松弛，其中
     `w = hypot(x[u]-x[v], y[u]-y[v]) * (rome[u][v] ? dR : dD)`。
5. 输出 `dis[n+1]`，保留 4 位小数。

### 4.3 复杂度分析

- **时间复杂度**：$O(V^2)$，$V\le1002$，约 $10^6$ 次松弛（每次一个 `hypot`）。
- **空间复杂度**：$O(V^2)$ 的布尔矩阵（约 $1$ MB）与 $O(V)$ 的 `dis/vis`。

---

## 5. 示例代码

```cpp
//
// 题目：P1907 设计道路
// https://www.luogu.com.cn/problem/P1907
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n;
double dD,dR;
double x[mxn],y[mxn];
bool rome[mxn][mxn];
double dis[mxn];
bool vis[mxn];

double w(int i,int j)
{
	double d=hypot(x[i]-x[j],y[i]-y[j]);
	return d*(rome[i][j]?dR:dD);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>dD>>dR;
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];

	int u,v;
	while(cin>>u>>v)
	{
		if(u==0&&v==0)
			break;
		--u;--v;
		rome[u][v]=rome[v][u]=true;
	}

	cin>>x[n]>>y[n];
	cin>>x[n+1]>>y[n+1];

	int V=n+2,src=n,dst=n+1;
	fill_n(dis,V,1e100);
	dis[src]=0;
	for(int it=0;it<V;++it)
	{
		int a{-1};
		for(int i=0;i<V;++i)
			if(!vis[i]&&(a==-1||dis[i]<dis[a]))
				a=i;
		if(a==-1||dis[a]>1e90)
			break;
		vis[a]=true;
		for(int b=0;b<V;++b)
			if(!vis[b])
			{
				double nd=dis[a]+w(a,b);
				if(nd<dis[b])
					dis[b]=nd;
			}
	}

	cout<<fixed<<setprecision(4)<<dis[dst]<<'\n';
	return 0;
}
```
