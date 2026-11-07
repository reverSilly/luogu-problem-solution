# P1930 [USACO3.3] 亚瑟王的宫殿

## 1. 重新读题

### 1.1 问题重述（人话版）

一个 $R\times C$ 的棋盘（$R\le40$，$C\le26$），上面有一个国王和若干骑士，没有两个骑士同格（也可能一个骑士都没有）。要把所有棋子移动到同一个格子（集合点任意）。

- 国王每一步可以走到周围 $8$ 个相邻格之一（不能出界）。
- 骑士每一步走“日”字（不能出界）。
- 玩家可以选定**一个**骑士，让它先与国王在某个格子相遇，之后国王跟着这个骑士一起按骑士规则走；两人一起走时每一步只算 $1$ 步。
- 其余骑士各自走向集合点。

求最少总步数。

### 1.2 关键约束与规模暗示

- 行数 $R\le40$，列数 $C\le26$，格子数 $V=RC\le1040$。
- “所有棋子集中到同一格”需要枚举集合点，$V$ 很小。
- $V=1040$ 时，$O(V^2)$ 的两两最短路（每个格子跑一次 BFS）只需约 $10^7$ 次运算，是可行的。
- 但“枚举集合点 × 枚举护送骑士 × 枚举相遇点”是 $O(V^2K)$，最坏 $1040^3\approx1.1\times10^9$，需要优化。
- 输入形如字母+数字的位置对（如 `D 4` 或 `D4`），列用大写字母，行从 $1$ 开始。

## 2. 思维链（Chain-of-thought）

### 2.1 两种棋子的距离

**引理 1（国王到任意格的最短步数是切比雪夫距离）**

国王从 $(r_1,c_1)$ 到 $(r_2,c_2)$ 的最少步数为
$$
K=\max\bigl(|r_1-r_2|,\ |c_1-c_2|\bigr).
$$

**证明**：

- 下界：国王每步至多让行坐标变化 $1$、列坐标变化 $1$，所以走 $t$ 步后行差、列差都不超过 $t$，故 $t\ge\max(|\Delta r|,|\Delta c|)$。
- 可达：先沿对角线走 $\min(|\Delta r|,|\Delta c|)$ 步，把其中一个坐标对齐，再沿直线走剩下的差。总步数正好是 $\max(|\Delta r|,|\Delta c|)$。

两边夹住，结论成立。$\blacksquare$

骑士没有解析公式（棋盘有边界），但棋盘只有 $V\le1040$ 个格子，边权全为 $1$，从**每个格子**跑一次 BFS 就能得到所有格子对之间的骑士最短路。记 $d(u,v)$ 为骑士从 $u$ 到 $v$ 的最少步数。总时间 $O(V(V+8V))=O(V^2)$。

### 2.2 固定集合点与护送方案时的代价

设集合点为 $t$，被国王“搭车”的骑士为 $i$（其起点为 $p_i$），国王与它在格子 $m$ 相遇。

**引理 2（代价公式）**

在上述选择下，最少总步数为
$$
C(t,i,m)=\sum_{j\ne i}d(p_j,t)
+\underbrace{d(p_i,m)+d(m,t)}_{\text{骑士 }i\text{ 的路线}}
+\underbrace{K(m)}_{\text{国王单独走到 }m},
$$
其中 $K(m)$ 是国王起点到 $m$ 的切比雪夫距离。

**证明**：

- 除骑士 $i$ 外的每个骑士都只从自己的起点走到 $t$，互不影响，最少就是 $d(p_j,t)$。
- 骑士 $i$ 先走到 $m$，再和国王一起从 $m$ 走到 $t$；两段都按骑士规则走，最少是 $d(p_i,m)+d(m,t)$。由于 $m$ 到 $t$ 这一段只有一次“共同移动”，不会重复计费。
- 国王在与骑士相遇前必须自己从起点走到 $m$，最少 $K(m)$（引理 1）。
- 各棋子的移动可以独立安排，先到 $m$ 的一方在原地等待即可，等待不产生步数；因此各段最短路径可以同时取到，总代价就是各段之和。$\blacksquare$

把总和拆开：
$$
C(t,i,m)=\sum_{j}d(p_j,t)+\bigl(d(p_i,m)-d(p_i,t)\bigr)+d(m,t)+K(m). \tag{1}
$$

**推论（国王独自前往的情况已被包含）**

取 $m=t$，则 $d(p_i,m)-d(p_i,t)=0$、$d(m,t)=0$，代价变成 $\sum_j d(p_j,t)+K(t)$，正是“国王自己走到 $t$”的方案。因此不必单独讨论“国王不搭车”。

### 2.3 直接枚举太慢，交换 $\min$ 的顺序

目标是
$$
\min_t\left[\sum_j d(p_j,t)+\min_{i,m}\Bigl(d(p_i,m)-d(p_i,t)+d(m,t)+K(m)\Bigr)\right].
$$
对固定的 $t$，内层要对每个骑士 $i$ 和每个相遇点 $m$ 枚举，代价 $O(KV)$；外层 $V$ 个 $t$，总 $O(V^2K)=1.1\times10^9$，太慢。

关键观察：在式 (1) 里，$i$ 只出现在 $d(p_i,m)-d(p_i,t)$ 这一项；$d(m,t)+K(m)$ 只与 $m$ 有关。因此可以交换两个 $\min$：

$$
\min_{i,m}\Bigl(d(p_i,m)-d(p_i,t)+d(m,t)+K(m)\Bigr)
=\min_m\left[d(m,t)+K(m)+\min_i\bigl(d(p_i,m)-d(p_i,t)\bigr)\right]. \tag{2}
$$

**引理 3（交换最小值的合法性）**

式 (2) 成立。

**证明**：设 $F(i,m)=d(p_i,m)-d(p_i,t)+d(m,t)+K(m)$。则
$$
\min_i\min_m F(i,m)=\min_m\min_i F(i,m)
$$
是显然的集合等式：两边都等于 $\{F(i,m)\}$ 中的最小值。再把与 $i$ 无关的 $d(m,t)+K(m)$ 提到内层 $\min_i$ 外面即可。$\blacksquare$

于是对每个 $t$，只需先求出
$$
g_t(m)=\min_i\bigl(d(p_i,m)-d(p_i,t)\bigr),\qquad \forall m,
$$
再取 $\min_m\bigl(g_t(m)+d(m,t)+K(m)\bigr)$。

### 2.4 用多源最短路一次求出 $g_t$

直接对每个 $m$ 枚举 $i$ 仍然是 $O(KV)$，总共 $O(V^2K)$。注意 $d(p_i,m)$ 正是“从格子 $p_i$ 出发的骑士最短路”，于是
$$
g_t(m)=\min_i\bigl(d(p_i,m)+c_i\bigr),\qquad c_i=-d(p_i,t).
$$
这正是一张**边权为 $1$ 的无向图**上的多源最短路：把每个骑士起点 $p_i$ 作为源，初始距离设为 $c_i$，求到每个 $m$ 的最小值。

**引理 4（带初始标号的多源 Dijkstra 正确性）**

在一张所有边权非负的图上，若对每个源 $s$ 令初始距离 $D(s)=c_s$（可为负），其余点 $+\infty$，用优先队列反复取出当前最小者并松弛，则最终
$$
D(v)=\min_s\bigl(c_s+d(s,v)\bigr).
$$

**证明**：把每个源看成一个与超级源相连、边权为 $c_s$ 的节点。虽然这些“入场边”可能为负，但它们只出现在路径开头，之后的图边全部非负。

设某个点 $u$ 被取出时 $D(u)$ 已经是最终值。反证：若存在更优路径 $P$ 从某个源到 $u$，设 $w$ 是 $P$ 上第一个尚未被取出的点，前缀权为 $L$。由于前缀上的点都已被取出并松弛过，$D(w)\le c_s+L$。因为剩余边权非负，$c_s+L\le$ 整条路径权 $<$ 我们假设的更优值 $<D(u)$，故 $D(w)<D(u)$，与 $u$ 是当前最小者矛盾。$\blacksquare$

（实现上直接以 $c_i$ 为初始值压入优先队列即可；$c_i\ge-V$，不会溢出。）

每个 $t$ 跑一次这样的 Dijkstra：$O((V+K)\log V)$。总复杂度
$$
O\bigl(V^2+V(K+V)\log V\bigr),
$$
在 $V,K\le1040$ 时约 $10^7\sim10^8$ 次运算，可以通过。

### 2.5 目标可行性

**引理 5（某个骑士到不了 $t$ 时该集合点无解）**

若存在骑士 $i$ 使 $d(p_i,t)=+\infty$，则任何方案都不可能把全部棋子集中到 $t$。

**证明**：该骑士无论如何只能到达它所在的连通分量内的格子（骑士图可能不连通，例如很小的棋盘或退化棋盘）。若 $t$ 不在这个分量里，它永远到不了 $t$。$\blacksquare$

因此枚举 $t$ 时，只要有一个骑士不可达就跳过。若所有 $t$ 都不可行，说明输入无解（正常数据不会出现）；代码保留一个极大哨兵。

### 2.6 没有骑士的边界

若输入中只有国王，那么“所有棋子集中到同一格”已经成立，答案是 $0$，不需要任何移动。必须特判，否则对空骑士集合求 $\min$ 会得到无意义的结果。

### 2.7 算法总览

1. 对所有格子跑骑士 BFS，得到 $d(u,v)$。
2. 枚举集合点 $t$：
   1. 求 $\text{sum}_t=\sum_i d(p_i,t)$；若有骑士不可达则跳过。
   2. 以 $c_i=-d(p_i,t)$ 为初值，在骑士图上跑多源 Dijkstra，得到 $g_t(m)$。
   3. 枚举 $m$，用 $g_t(m)+d(m,t)+K(m)$ 更新该 $t$ 的最优值。
3. 所有 $t$ 取最小即为答案。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 国王距离是切比雪夫，骑士距离用棋盘 BFS；固定“集合点 + 护送骑士 + 相遇点”后代价可加。
- **reduce to easier problem** — 把总和拆开，$m=t$ 自动覆盖“国王独走”，无需单独分类。
- **exchange the minima** — 交换 $\min_i$ 与 $\min_m$，把 $i$ 相关的项压成一个关于 $m$ 的函数 $g_t(m)$。
- **classic problem solution** — $g_t$ 是带初始标号的多源 Dijkstra，每换一个 $t$ 重跑一次。
- **pitfall** — 骑士图可能不连通要判不可达；没有骑士时答案为 $0$；坐标可能是 `D 4` 也可能是 `D4`。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `R,C` | `int` | 行数、列数 | 读入 | — |
| `V` | `int` | 格子数 $RC$ | `R*C` | vertices |
| `kdis[u][v]` | `int[1100][1100]` | 骑士从 $u$ 到 $v$ 的最少步数 | 每格 BFS | knight distance |
| `kr,kc` | `int` | 国王起点（0-based） | 第一个位置 | king |
| `knight[]`,`kcnt` | `int[1100]`,`int` | 各骑士起点编号 | 后续位置 | — |
| `dist[]` | `int[1100]` | 多源 Dijkstra 的当前距离 | 每次 `inf` | — |
| `t,m` | `int` | 集合点、国王与骑士的相遇点 | 枚举 | target/meet |

### 4.2 算法步骤

1. 读入 $R,C$，再读入所有位置记号，第一个是国王，其余是骑士；兼容 `D 4` 与 `D4` 两种写法。
2. 若没有骑士，输出 $0$ 并结束。
3. 对每个格子 $u$ 跑一次 BFS，填好 `kdis[u][*]`（不可达为 `inf`）。
4. 令 `ans=inf`，枚举集合点 $t=0\dots V-1$：
   1. 计算 `sum=Σ kdis[knight[i]][t]`；若有某项为 `inf`，跳过该 $t$。
   2. 初始化 `dist[*]=inf`，对每个骑士 $i$ 令初值 `-kdis[knight[i]][t]` 压入小根堆（同格取最小）。
   3. 按骑士的 $8$ 个方向做 Dijkstra，边权 $1$，得到 $g_t(m)$。
   4. 枚举 $m$：若 `dist[m]` 或 `kdis[m][t]` 为 `inf` 则跳过；否则用
      `dist[m]+kdis[m][t]+max(|mx-kr|,|my-kc|)`
      更新该 $t$ 的最优值。
   5. 用 `sum+最优值` 更新 `ans`。
5. 输出 `ans`。

### 4.3 复杂度分析

- 预处理全源骑士 BFS：$O(V^2)$（$V\le1040$）。
- 每个集合点一次多源 Dijkstra：$O((V+K)\log V)$；共 $V$ 次。
- 总时间 $O\bigl(V^2+V(K+V)\log V\bigr)$。
- 空间 $O(V^2)$，约 $4.3$ MB。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1100};
constexpr int inf{1145141919};

int R,C,V;
int kdis[mxn][mxn];
int dist[mxn];
int kr,kc;
int knight[mxn],kcnt;

const int dr[8]={-2,-2,-1,-1,1,1,2,2};
const int dc[8]={-1,1,-2,2,-2,2,-1,1};

void bfs(int src)
{
	for(int i=0;i<V;++i)
		kdis[src][i]=inf;
	queue<int>q;
	kdis[src][src]=0;
	q.push(src);
	while(!q.empty())
	{
		int u=q.front();
		q.pop();
		int ux=u/C,uy=u%C;
		for(int k=0;k<8;++k)
		{
			int vx=ux+dr[k],vy=uy+dc[k];
			if(vx<0||vx>=R||vy<0||vy>=C)
				continue;
			int v=vx*C+vy;
			if(kdis[src][v]>kdis[src][u]+1)
			{
				kdis[src][v]=kdis[src][u]+1;
				q.push(v);
			}
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>R>>C;
	V=R*C;
	vector<string>tok;
	string s;
	while(cin>>s)
		tok.push_back(s);
	int at=0;
	auto getpos=[&]()
	{
		string a=tok[at++];
		int c=a[0]-'A';
		int r;
		if(a.size()>1)
			r=stoi(a.substr(1))-1;
		else
			r=stoi(tok[at++])-1;
		return make_pair(r,c);
	};
	auto[kx,ky]=getpos();
	kr=kx;
	kc=ky;
	while(at<(int)tok.size())
	{
		auto[x,y]=getpos();
		knight[kcnt++]=x*C+y;
	}
	if(kcnt==0)
	{
		cout<<0<<'\n';
		return 0;
	}

	for(int v=0;v<V;++v)
		bfs(v);

	int ans=inf;
	for(int t=0;t<V;++t)
	{
		int sum=0;
		bool ok=true;
		for(int i=0;i<kcnt;++i)
		{
			int d=kdis[knight[i]][t];
			if(d>=inf)
			{
				ok=false;
				break;
			}
			sum+=d;
		}
		if(!ok)
			continue;

		// dist[m]=min_i(knightdist(pos_i,m)-knightdist(pos_i,t))
		fill_n(dist,V,inf);
		priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>>pq;
		for(int i=0;i<kcnt;++i)
		{
			int val=-kdis[knight[i]][t];
			if(val<dist[knight[i]])
			{
				dist[knight[i]]=val;
				pq.push({val,knight[i]});
			}
		}
		while(!pq.empty())
		{
			auto[d,u]=pq.top();
			pq.pop();
			if(d!=dist[u])
				continue;
			int ux=u/C,uy=u%C;
			for(int k=0;k<8;++k)
			{
				int vx=ux+dr[k],vy=uy+dc[k];
				if(vx<0||vx>=R||vy<0||vy>=C)
					continue;
				int v=vx*C+vy;
				if(dist[v]>d+1)
				{
					dist[v]=d+1;
					pq.push({dist[v],v});
				}
			}
		}

		int pen=inf;
		for(int m=0;m<V;++m)
		{
			if(dist[m]>=inf||kdis[m][t]>=inf)
				continue;
			int mx=m/C,my=m%C;
			int ck=max(abs(mx-kr),abs(my-kc));
			pen=min(pen,dist[m]+kdis[m][t]+ck);
		}
		ans=min(ans,sum+pen);
	}
	cout<<ans<<'\n';
	return 0;
}
```
