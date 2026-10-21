# P1828 [USACO3.2] 香甜的黄油 Sweet Butter

> 难度：普及/提高- · 标签：图论, 最短路, USACO

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $P$ 个牧场、$C$ 条**双向**路、$N$ 头奶牛。每头牛已经在某个牧场上（可以多头挤在同一牧场）。选一个牧场放糖，所有牛沿着最短路走过去，希望路程**总和**最小。输出这个最小和。

### 1.2 关键约束与规模暗示

- $1\le N\le 500$，$2\le P\le 800$，$1\le C\le 1450$，$1\le D\le 255$。
- 图很稀疏：$C$ 只比 $P$ 略大。Floyd 是 $800^3\approx 5\cdot 10^8$，常数再小也悬；对每个牧场做一次堆优化 Dijkstra 是 $800\times (P+C)\log P$，大约 $10^7$ 级操作。
- 边权为正。题面没有承诺任意两点连通，所以某个放糖点若有牛走不到，这个点不能当答案。
- 样例 $3$ 头牛在牧场 $2,3,4$，五条边，放在 $4$ 号时路程和是 $3+5+0=8$。

## 2. 思维链（Chain-of-thought）

糖的位置是枚举对象，牛只贡献「有多少头在这个点」。

**引理 1（按牧场计数后，目标和是加权最短路）**

设牧场从 $0$ 编号，`cnt[v]` 是站在 $v$ 的牛数。糖放在 $s$ 时的总路程是 $\sum_v cnt[v]\cdot d(v,s)$。答案是所有 $s$ 里这个和的最小值；若某头牛的牧场到不了 $s$，该 $s$ 作废。

**证明**：每头牛独立走自己到 $s$ 的最短路，同一牧场的牛路径长度相同，所以可以先乘上 `cnt[v]` 再相加。总路程是各牛路程之和，没有牛与牛之间的额外约束，因此最小值就是逐个 $s$ 取最小。不可达时最短路不存在，这个 $s$ 不满足「所有牛都能到达」。$\blacksquare$

因为边无向，$d(v,s)=d(s,v)$。对每个 $s$ 从 $s$ 跑一次单源，就能得到全部 $d(s,v)$。

**引理 2（正权、堆优化 Dijkstra 可用来求这个和）**

边权在 $[1,255]$。对固定源点，二叉堆 Dijkstra 在弹出时确定的距离是最短路。

**证明**与有向正权图相同：设下一个被确定的点距离不是最短，取一条真最短路上第一个还没确定的点，它的前驱已经确定，松弛会给出更小的距离并更早弹出，矛盾。无向边看成两条有向边，权不变，证明一字不改。$\blacksquare$

$P=800$ 次这种 Dijkstra 在稀疏图上足够快。暴力对拍改用 Floyd，只在 $P\le 28$ 的随机连通图上和标程比：三重循环枚举中转，和 Dijkstra 不是同一实现。连通性用一棵随机树打底，保证样例那种「每个放糖点都可达」在小数据里成立；标程本身仍会跳过不可达点，以免遇到断图时把哨兵加进答案。

单路距离最大不超过 $(P-1)\cdot 255<3\cdot 10^5$，乘上 $N\le 500$ 后总和 $<2\cdot 10^8$，放进 `i64` 毫无压力。哨兵用 `1145141919810`，比任何合法总和都大，又不会在 `dis[v]+w` 时溢出（只有已经到达的点才会松弛）。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 同一牧场的牛可以合并成计数，总路程是 $\sum cnt[v]\cdot d(v,s)$（引理 1）。
- **classic problem solution** — 无向正权图对每个可能的放糖点跑一次 Dijkstra（引理 2）。
- **consider easier version** — $P$ 很小就用 Floyd 做对照；随机数据先生成树，保证小图连通。
- **boundary discipline** — 牧场号读入后 `--`；`dis` 每轮重新填成 `inf`，不能把全局 $0$ 当成无穷；有牛不可达的点直接丢弃。

## 4. 真实解法

### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `n,p,c` | `int` | 牛数、牧场数、边数 | 读入 | $P$ 用小写 `p` |
| `cnt` | `int [805]` | 每个牧场的牛数 | 全局 $0$，读入 `++` | count |
| `g` | `vector<pair<int,int>> [805]` | 无向邻接表 | 每条边推两次 | graph |
| `dis` | `i64 [805]` | 当前源点的最短路 | 每轮 `fill_n` 成 `inf` | distance |
| `vis` | `bool [805]` | 是否已确定 | 每轮清 `false` | 弹出即确定 |
| `inf` | `i64` | 不可达 | `1145141919810` | 大于合法答案 |

### 4.2 算法步骤

1. 读牛所在牧场，`--` 之后 `cnt` 加一。每条无向边两端都加入邻接表。
2. 对每个牧场 $s=0..p-1$ 做 Dijkstra。
3. 若所有 `cnt[i]>0` 的点 `dis[i]` 都小于哨兵，把 $\sum cnt[i]\cdot dis[i]$ 拿去更新答案。
4. 输出答案。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

- 时间 $O\bigl(P(P+C)\log P\bigr)$，空间 $O(P+C)$。
- 编译：`g++ -O2 -std=c++17 -Wall -Wextra -Werror`，无警告。
- 样例标程与 Floyd 都是 `8`。
- 随机：$2\le P\le 28$、先生成树再加边、$N\le 40$，120 组标程 = Floyd，耗时 $0.66$ s；另 3 组边界（一头牛、四头牛挤在连通三分图、两头牛一条边）一致。
- 最大规模 $N=500,P=800,C=1450$ 的随机树加边：标程答案 `178726`，耗时 $0.0957$ s。
- 合计 `125 tests OK`。

## 5. 示例代码

### 5.1 标程（`P1828.cpp`）

```cpp
//
// 题目：P1828 [USACO3.2] 香甜的黄油 Sweet Butter
// https://www.luogu.com.cn/problem/P1828
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxp{805};
constexpr i64 inf{1145141919810LL};

int n,p,c;
int cnt[mxp];
vector<pair<int,int>>g[mxp];
i64 dis[mxp];
bool vis[mxp];

void dij(int s)
{
	fill_n(dis,p,inf);
	fill_n(vis,p,false);
	priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
	dis[s]=0;
	Q.emplace(0,s);
	while(!Q.empty())
	{
		auto[dd,u]=Q.top();
		Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		for(auto[v,w]:g[u])
			if(dis[v]>dd+w)
			{
				dis[v]=dd+w;
				Q.emplace(dis[v],v);
			}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>p>>c;
	for(int i=0,x;i<n;++i)
	{
		cin>>x;
		--x;
		++cnt[x];
	}
	for(int i=0,a,b,d;i<c;++i)
	{
		cin>>a>>b>>d;
		--a;--b;
		g[a].push_back({b,d});
		g[b].push_back({a,d});
	}
	i64 ans=inf;
	for(int s=0;s<p;++s)
	{
		dij(s);
		i64 cur=0;
		bool ok=true;
		for(int i=0;i<p;++i)
			if(cnt[i])
			{
				if(dis[i]>=inf)
				{
					ok=false;
					break;
				}
				cur+=cnt[i]*dis[i];
			}
		if(ok)
			ans=min(ans,cur);
	}
	cout<<ans<<'\n';
	return 0;
}
```
