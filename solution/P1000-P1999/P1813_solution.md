# P1813 拯救小 tim

## 1. 重新读题

有 $n$ 个路口、$m$ 条**单向**道路。第 $i$ 条路从 $x_i$ 到 $y_i$，只在时刻区间 $[b_i,e_i]$ 内开放，通过它需要 $c_i$ 的时间；要求**整段通过过程**都在开放区间内，即若在时刻 $s$ 出发穿这条路，必须满足 $s\ge b_i$ 且 $s+c_i\le e_i$。可以在任意路口（包括起点）等待，时刻从 $0$ 开始。

要从 $s$ 到 $t$，最小化「到达 $t$ 的时刻 $-$ 离开 $s$ 的时刻」（在起点等待的时间不计入，途中等待计入）。

约束：$n\le100$，$m\le1000$，$0\le b<e\le10000$，$0\le c\le10000$。无解输出 `Impossible`。

**规模暗示**：所有开放区间右端点 $e\le10^4$，而每次通行结束时刻 $\le e\le10^4$，所以**所有有意义的时刻都在 $[0,10^4]$ 内**；$n\cdot10^4\approx10^6$ 个状态，正好可以做「按时刻递推」的 DP，不能对每对点只存一个最短路值（时间是绝对量，越早到不一定越好，还要看能否省钱/省等待）。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先问：普通的「最早到达」为什么不够

如果只求最快到达，可以对每个点记「最早到达时刻」，因为更早到一定不劣于更晚到（可以等着）。但这题的目标是「到达 $-$ 离开」，离开时刻是自由的。

先想一个错误做法：令 $dis[v]$ 为从 $s$ 出发到 $v$ 的最短**路上时间**，按 Dijkstra 松弛。反例：某条路 $[0,100]$ 需要 $50$，另一条路 $[60,100]$ 需要 $10$。若先把某个点按第一条路「到达」了，时间 $50$，路上时间 $50$；实际上可以等到 $60$ 再走第二条路，路上时间只有 $10$。同一个点、同一个到达时刻附近，**离开时刻的信息被丢掉了**，所以不能只存一个值。

正确做法是把「离开 $s$ 的时刻」作为被最大化的量保留下来。

### 2.2 把「等待」规范化

**引理 1（只需考虑「被迫等待」的方案）**
存在一个最优方案，使得：

- 在起点 $s$ 的等待不单独处理（把「离开 $s$ 的时刻」直接看作一个可自由选择的量 $\tau$）；
- 在任意中间点 $u$ 到达后，只在**下一条路尚未开放**时等待，并且恰好等到它的 $b$；
- 到达终点后不再等待。

**证明**：设某方案在中间点 $u$ 于时刻 $t$ 到达，准备走 $(u,v,[b,e],c)$，却等到了 $s_0>\max(t,b)$ 才出发。改成 $s_0'=\max(t,b)$ 出发：$s_0'\ge b$，且 $s_0'\le s_0$，于是新出发时刻更早、到达 $v$ 的时刻更早，之后所有行程整体可以只提前不推后；每条后续路的出发时刻 $\ge$ 其开放下界（因为遇到下界会等下界的 $\max$ 处理，或者说提前到达后仍然可以等到 $b$ 再走），也更容易满足上界 $e$。因此方案仍合法，而到达终点的时刻不变晚（只会更早或不变），$\tau$ 不变，目标值不增大。反复消除这类多余等待即得所需方案。$\blacksquare$

于是在中间点，状态只需要是「在时刻 $t$ 位于 $u$」；下一步走某条边时出发时刻固定为 $\max(t,b)$。

### 2.3 状态设计：为每个「点 + 时刻」保存最晚出发时刻

定义

$$D[u][t]=\max\{\tau:\text{存在合法行程，在时刻 }\tau\text{ 离开 }s\text{，并在时刻 }t\text{ 恰好位于 }u\},$$

不可达记 $-1$。

**为什么最大化 $\tau$ 而不是最小化到达**：同一到达时刻 $t$ 下，目标 $t-\tau$ 要最小等价于 $\tau$ 最大。把每个到达时刻下的「最好出发时刻」都记下来，最后统一取 $\min$。

**初始状态**：在起点可以任意时刻出发，故

$$D[s][t]=t,\qquad t=0,1,\dots,T,$$

其中 $T=\max e_i$（所有有意义时刻的上界；任何一次通行结束时刻都 $\le$ 它对应的 $e_i\le T$）。

**转移**：从 $(u,t)$ 走边 $(u,v,[b,e],c)$：

$$s_0=\max(t,b),\qquad \text{若 }s_0+c\le e,\ \text{则 } D[v][s_0+c]\leftarrow\max\big(D[v][s_0+c],\,D[u][t]\big).$$

（出发时刻被推迟到 $b$，是引理 1 允许的被迫等待；出发时刻本身 $\tau$ 不变，所以直接把 $D[u][t]$ 传过去。）

**引理 2（转移的完备性与正确性）**
按上述方式从初始状态松弛，得到的 $D[u][t]$ 恰好等于定义中的最大值。

**证明**：对时刻 $t$ 归纳。

- 任何合法行程到达 $u$ 于时刻 $t$，看它最后一个动作：若还没走过任何边，则 $u=s$ 且 $t=\tau$ 是出发时刻，被 $D[s][t]=t$ 覆盖；否则最后一条边是 $(x,u,[b,e],c)$，行程在某个时刻 $t'$ 位于 $x$，由引理 1 可在 $\max(t',b)$ 出发，于是 $t=\max(t',b)+c$。由归纳假设 $D[x][t']$ 不小于该行程的出发时刻，转移会把一个不小于它的值写入 $D[u][t]$。所以 DP 值不小于任何真实行程的出发时刻。
- 反过来，$D$ 的每个值都来自一条真实行程（初始状态是真实的；转移对应的「在 $x$ 等到 $\max(t,b)$ 再走」也是合法动作）。所以 DP 值不会超过定义中的最大可能值。

两边夹逼即得相等。$\blacksquare$

### 2.4 计算顺序与零耗时边

每条边转移后的时刻 $s_0+c\ge t$，所以按 $t=0,1,\dots,T$ 递增处理即可：处理到 $t$ 时，$D[\cdot][t]$ 已被所有更早时刻的转移更新完毕，不会再变。

唯一的例外是 $c=0$：转移后仍在时刻 $t$。这在同层内是一个「取 max 的传播」，用一个小队列处理：同层内某点的 $D$ 变大时，把它重新入队，继续沿 $c=0$ 的边传播即可（数值只会变大，必然收敛；每次传播都是把更大的 $\tau$ 推给邻点）。

**答案**：

$$\text{ans}=\min_{\,0\le t\le T,\ D[t_{\text{终点}}][t]\ge0}\big(t-D[t_{\text{终点}}][t]\big).$$

若没有任何可达状态，输出 `Impossible`。

证明：任何方案对应某个到达时刻 $t$ 与出发时刻 $\tau\le D[t_{\text{终点}}][t]$，故其目标 $\ge t-D[t_{\text{终点}}][t]$；而 $D[t_{\text{终点}}][t]$ 本身由某条真实方案取到，故下界可达到，取 $\min$ 即最优。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 时间是绝对量，单点只存「最短路」会丢掉出发时刻；目标可改写成「同一到达时刻下最大化出发时刻」。
- **normalize waiting** — 多余等待可以删掉，只保留「等边开放」的被迫等待（引理 1）。
- **state augmentation** — 状态扩成「点 $\times$ 时刻」，$D[u][t]$ 存最晚出发时刻；时刻上界 $10^4$，状态数 $10^6$。
- **classic problem solution** — 按时刻递增做 DP，边只会走向不早于当前时刻；零耗时边在同层用队列传播。
- **defer reconstruction** — 不记录路径，最后对终点所有到达时刻取 $\min(t-D)$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n,m,s,t` | `int` | 点数、边数、起点、终点（内部 0-based） | 读入 | — |
| `edge{v,b,e,c}` | `struct` | 一条有向边 | 读入 | window + cost |
| `graph[u]` | `vector<edge>[105]` | 点 $u$ 的出边 | 读入 | adjacency |
| `latest[u][tm]` | `int[105][10005]` | $D[u][tm]$，最晚出发时刻 | `-1`（不可达） | latest departure |
| `maxT` | `int` | $\max e_i$ | 读入时统计 | 时刻上界 |
| `dep` | `int` | 当前状态的 $D[u][tm]$ | 出队时取 | departure |
| `ans` | `int` | 最小路上时间 | `1145141919` | answer |

### 4.2 算法步骤

1. 读入 $n,m,s,t$，把 $s,t$ 转成 0-based；读边，同时令 `maxT=max(maxT,e)`。
2. `latest` 全部置 `-1`；对 $tm=0\dots maxT$ 令 `latest[s][tm]=tm`。
3. 对 $tm=0\dots maxT$：
   - 把当前所有 `latest[u][tm]>=0` 的点入队（同层队列）；
   - 出队 $u$，取 `dep=latest[u][tm]`；对每条出边 `(v,b,e,c)`：
     - `st=max(tm,b)`；若 `st+c>e` 跳过；
     - `nt=st+c`；若 `latest[v][nt]<dep`，更新；
     - 若 `nt==tm` 且 $v$ 不在队列中，入队（零耗时边同层传播）。
4. 扫描 `tm`，若 `latest[t][tm]>=0`，用 `tm-latest[t][tm]` 更新 `ans`。
5. `ans` 仍为哨兵则输出 `Impossible`，否则输出 `ans`。

### 4.3 复杂度

- 状态数 $n(T+1)\le100\times10001\approx10^6$；每个状态扫描其出边，总转移量 $O(T\cdot m)\le10^7$（零耗时边的同层传播至多把等值沿边再推一遍，量级相同）。
- 时间 $O(T(n+m))$；空间 $O(nT)$（`latest` 约 $4$ MB）。

---

## 5. 示例代码

```cpp
//
// 题目：P1813 拯救小 tim
// https://www.luogu.com.cn/problem/P1813
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr int mxT{10005};
constexpr int inf{1145141919};

struct edge{int v,b,e,c;};

int n,m,s,t;
vector<edge> graph[mxn];
int latest[mxn][mxT];   // D[u][tm]：最晚出发时刻，-1 表示不可达

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>s>>t;
	--s;--t;
	int maxT{0};
	for(int i=0,x,y,b,e,c;i<m;++i)
	{
		cin>>x>>y>>b>>e>>c;
		--x;--y;
		graph[x].push_back({y,b,e,c});
		maxT=max(maxT,e);
	}

	memset(latest,-1,sizeof(latest));
	for(int tm=0;tm<=maxT;++tm)
		latest[s][tm]=tm;

	static bool inq[mxn];
	for(int tm=0;tm<=maxT;++tm)
	{
		memset(inq,0,sizeof(inq));
		queue<int> cand;
		for(int u=0;u<n;++u)
			if(latest[u][tm]>=0)
			{
				cand.push(u);
				inq[u]=true;
			}
		while(!cand.empty())
		{
			int u=cand.front();cand.pop();
			inq[u]=false;
			int dep=latest[u][tm];
			for(auto&ed:graph[u])
			{
				int st=max(tm,ed.b);
				if(st+ed.c>ed.e)
					continue;
				int nt=st+ed.c;
				if(nt>maxT)
					continue;
				if(latest[ed.v][nt]<dep)
				{
					latest[ed.v][nt]=dep;
					// c==0 的边仍在同一时刻，需要同层继续传播
					if(nt==tm&&!inq[ed.v])
					{
						cand.push(ed.v);
						inq[ed.v]=true;
					}
				}
			}
		}
	}

	int ans{inf};
	for(int tm=0;tm<=maxT;++tm)
		if(latest[t][tm]>=0)
			ans=min(ans,tm-latest[t][tm]);
	if(ans==inf)
		cout<<"Impossible\n";
	else
		cout<<ans<<'\n';
	return 0;
}
```
