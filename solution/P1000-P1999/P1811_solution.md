# P1811 最短路

> 难度：普及/提高- · 标签：最短路、广度优先搜索 BFS、Special Judge

## 1. 重新读题
### 1.1 问题重述（人话版）

无向图，\(N\) 个点 \(M\) 条边，每条边长度都是 1。另外有 \(K\) 条有序禁令 \((A,B,C)\)：刚从 \(A\) 走到 \(B\)，下一步不许走到 \(C\)。从 \(B\) 走到 \(A\) 再走到 \(C\) 不受这条禁令影响。在这些禁令下，求 \(1\) 到 \(N\) 的最短路，输出长度 \(S\) 以及任意一条合法的点序列（\(S+1\) 个点）。答案不唯一，评测是 Special Judge。

### 1.2 关键约束与规模暗示

- \(N\le 3000\)，\(M\le 20000\)，\(K\le 100000\)。边权全是 1，最短路用 BFS，不必 Dijkstra。
- 禁令看的是「上一个点、当前点、下一个点」三元组，所以状态不能只记当前点。样例路径 `1 3 2 3 4` 两次经过点 3，普通的「点不重复」最短路会漏掉它。
- 状态应是「从哪个点走进当前点」。这样的状态数和有向边数同阶，大约 \(2M\)。
- \(N=1\) 时路径长度是 0，只输出点 1。题面数据保证 1 能到 \(N\)（这题对应 CF 59E 的保证）；标程在走不到时输出一行 `-1`，只作为防御，对拍里双方约定一致。

## 2. 思维链（Chain-of-thought）

**引理 1（下一步是否合法只取决于有序对 \((pre,u)\)）**

走在点 \(u\)、并且是从 \(pre\) 走进来的时候，走向邻居 \(v\) 合法，当且仅当边 \(u-v\) 存在，且 \((pre,u,v)\) 不在禁令集合里。更早的路径对这个判断没有影响。

**证明**：禁令的定义是三条连续的点 \((A,B,C)\)，检查的是路径上相邻的三元组。站在 \(u\)、前驱是 \(pre\) 时，即将形成的三元组只有 \((pre,u,v)\)。题面没有「整条路径上不能出现某种模式」的其它限制，边集也不随时间变化。因此未来所有决策的合法性都由 \((pre,u)\) 决定。起点还没有任何前驱，不存在三元组，第一步不受禁令约束。\(\square\)

**引理 2（状态图上的 BFS 是最短路）**

把状态定义成 \((pre,u)\)：当前在 \(u\)，前驱是 \(pre\)（起点用 \(pre=-1\)）。从 \((pre,u)\) 向每个合法邻居 \(v\) 连一条权为 1 的边，到达状态 \((u,v)\)。在这张状态图上从 \((-1,1)\) 做 BFS，第一次到达「当前点是 \(N\)」的状态时，经过的边数就是原问题的最短路长度。

**证明**：原图每一步对应状态图的一条边，权都是 1，所以原路径长度等于状态路径长度。引理 1 保证状态图的边集恰好是原图里合法的一步，不多也不少。BFS 在单位权图上第一次到达某状态时，距离就是最短距离：若有更短的走到同一状态的路径，它会在更早的层被扩展到（标准 BFS 归纳：第 \(d\) 层弹出的状态距离都是 \(d\)）。

同一个状态不必第二次入队。设第一次到达 \((pre,u)\) 的距离是 \(d\)。任何更晚到达它的走法距离 \(\ge d\)，而从这个状态出发的后继完全相同，所以更晚的副本不会产生更短的后续。点可以重复（样例里的 3），状态不重复。\(\square\)

**引理 3（沿前驱指针走回去得到一条同样短的具体路径）**

扩展 \((pre,u)\to v\) 时记下「状态 \((u,v)\) 的再上一跳是 \(pre\)」。从终止状态 \((endpre,\,N)\) 开始，反复输出当前点并把当前点换成前驱，直到前驱是 \(-1\)。倒过来就是一条从 1 到 \(N\) 的合法路径，边数等于 BFS 距离。

**证明**：每次入队都对应一条原图的边（邻居表里才有 \(v\)），并且当时检查过禁令。指针保存的是入队时的 \(pre\)，所以倒回去的每连续三点都是当时允许的三元组。起点状态的前驱是 \(-1\)，倒推在这里停止，序列以 1 开头。长度等于入队时写入的距离，而那个距离在引理 2 里已经是最短。\(\square\)

邻接表按输入顺序挂边，BFS 在同一层里按这个顺序扩展。样例因此得到 `4` 和 `1 3 2 3 4`，与题面样例一致；其它同长度的路径也算对，对拍只比长度并检查路径合法。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 禁令是有序三元组，点可以重复，状态必须带上一个点。
- **reduce to easier problem** — 把 \((pre,u)\) 当成新图的顶点，边权仍是 1。
- **classic problem solution** — 状态图 BFS，第一次走到 \(N\) 即最短，再顺着前驱还原路径。

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| \(n,m,k\) | `int` | 点数、边数、禁令数 | 读入 | 点改 0 下标 |
| `g[u]` | `vector<int>[3005]` | 无向邻接表 | 空 | 自环只挂一次 |
| `ban` | `unordered_set<i64>` | 禁令 \((a,b,c)\) 压成一个整数 | 空 | 三元键 `a,b,c` |
| `vis,distmp,from` | 哈希表 | 状态 \((pre,u)\) 是否到过、距离、再上一跳 | 起点距离 0 | `enc(pre,u)=(pre+1)*mxn+u` |
| `enddist` | `int` | 最短路长度 \(S\) | 走到 \(n-1\) 时写入 | 第一行输出 |

### 4.2 算法步骤

1. 建无向图。自环不要正反各挂一次。禁令端点减 1 后插入哈希集合。
2. \(n=1\)：输出 `0` 和 `1`。
3. 队列放入状态 \((-1,0)\)。弹出 \((pre,u)\) 后扫描邻居 \(v\)：前驱存在且三元组被禁就跳过；状态 \((u,v)\) 到过就跳过；否则记录距离和 `from`，若 \(v\) 是终点则停止。
4. 没走到终点就输出 `-1`。否则从终点顺着 `from` 收集点，反转后第一行输出距离，第二行输出 1 下标的点，空格分隔，行末无多余空格。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

- 每个点 \(u\) 的每种到达方式最多扩展一次，到达方式数 \(\le \deg(u)\)，扩展时再扫一遍邻居，最坏 \(\sum_u \deg(u)^2\)。另加 \(O(K)\) 建禁令表。随机图度数很小；纯链 \(N=3000\) 只有 \(2999\) 条边。
- 编译 warn=0。
- 样例输出与题面逐字符相同：`4` / `1 3 2 3 4`。
- 暴力是同一 BFS，但距离和前驱放在二维数组里（\(n\le 8\)），不走哈希。另外用 Python 检查：边存在、没有踩禁令、起终点正确、点数 \(=S+1\)，并且两边的 \(S\) 相等。固定 8 组（单点、直连、禁掉唯一的一步、无边、样例结构）加随机 250 组。共 259 组，随机段 3.250s。
- 满数据计时：\(N=3000\)，\(M=20000\)，\(K=100000\) 的随机图（含一条 \(1..N\) 的链），标程 0.059s，最短路长度 4，路径 `1 2 2295 2999 3000` 已另行检查合法。纯链 \(M=N-1=2999\) 时 0.005s，答案 2999。

## 5. 示例代码
### 5.1 标程（`P1811.cpp`）

```cpp
//
// 题目：P1811 最短路
// https://www.luogu.com.cn/problem/P1811
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{3005};

struct hsh
{
	size_t operator()(i64 x)const
	{
		uint64_t z{(uint64_t)x};
		z^=z>>30;
		z*=0xbf58476d1ce4e5b9ULL;
		z^=z>>27;
		z*=0x94d049bb133111ebULL;
		z^=z>>31;
		return (size_t)z;
	}
};

int n,m,k;
vector<int> g[mxn];
unordered_set<i64,hsh> ban,vis;
unordered_map<i64,int,hsh> from,distmp;

i64 enc(int pre,int u)
{
	return (i64)(pre+1)*mxn+u;
}

i64 tri(int a,int b,int c)
{
	return ((i64)a*mxn+b)*mxn+c;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	for(int i=0;i<m;++i)
	{
		int u,v;
		cin>>u>>v;
		--u;
		--v;
		g[u].push_back(v);
		if(u!=v)
			g[v].push_back(u);
	}
	ban.reserve((size_t)k*2+1);
	for(int i=0;i<k;++i)
	{
		int a,b,c;
		cin>>a>>b>>c;
		--a;
		--b;
		--c;
		ban.insert(tri(a,b,c));
	}
	if(n==1)
	{
		cout<<"0\n1\n";
		return 0;
	}
	queue<pair<int,int>> q;
	int spre{-1},su{0};
	vis.insert(enc(spre,su));
	distmp[enc(spre,su)]=0;
	q.push({spre,su});
	bool got{false};
	int endpre{-1};
	int enddist{0};
	while(!q.empty()&&!got)
	{
		int pre{q.front().first};
		int u{q.front().second};
		q.pop();
		for(int v:g[u])
		{
			if(pre!=-1&&ban.count(tri(pre,u,v)))
				continue;
			i64 nk{enc(u,v)};
			if(vis.count(nk))
				continue;
			vis.insert(nk);
			from[nk]=pre;
			distmp[nk]=distmp[enc(pre,u)]+1;
			if(v==n-1)
			{
				got=true;
				endpre=u;
				enddist=distmp[nk];
				break;
			}
			q.push({u,v});
		}
	}
	if(!got)
	{
		cout<<"-1\n";
		return 0;
	}
	vector<int> seq;
	int cur{n-1};
	int pre{endpre};
	seq.push_back(cur);
	while(pre!=-1)
	{
		seq.push_back(pre);
		int ppre{from[enc(pre,cur)]};
		cur=pre;
		pre=ppre;
	}
	reverse(seq.begin(),seq.end());
	cout<<enddist<<'\n';
	for(int i=0;i<(int)seq.size();++i)
	{
		if(i)
			cout<<' ';
		cout<<seq[i]+1;
	}
	cout<<'\n';
	return 0;
}
```
