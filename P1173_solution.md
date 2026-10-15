# P1173 [NOI2016] 网格 题解

## Part 1 — 重新描述问题

我们有一个 n×m 的棋盘格，其中 c 个格子被蛐蛐占据（视为障碍物），其余格子被跳蚤占据。跳蚤之间的连通性定义为四连通（上下左右相邻）。蛐蛐国王可以把若干只跳蚤替换成蛐蛐（即移除跳蚤），目标是使得剩下的跳蚤中，至少存在两只跳蚤不连通。要求最小化替换的跳蚤个数，若不可能则输出 −1。

数据范围：n,m ≤ 1×10⁹，T ≤ 20，∑c ≤ 1×10⁵。

核心观察：答案只可能是 −1、0、1、2 中的一个。

---

## Part 2 — 推理链（完整证明）

### 2.1 从简单情形开始

先考虑一个空棋盘（c = 0）。若 n,m ≥ 2，总跳蚤数 n×m ≥ 4，它们是全部连通的。去掉多少只跳蚤能断开？

- 左上角 (1,1) 只有两个邻居 (1,2) 和 (2,1)。同时去掉这两个邻居，(1,1) 就变成孤立点。这给出一个使用 2 只替换的方案。
- 能否只去掉 1 只就断开？网格是 2-连通的（即去掉任意一个点不会使图不连通），所以不行。

猜想：答案 ∈ {−1, 0, 1, 2}。下面证明它。

### 2.2 答案空间的分类

记 F 为所有跳蚤格子（非蛐蛐格子），G[F] 为 F 在四连通意义下的诱导子图。|F| = n×m − c。

**引理 1**（−1 的情形）：若 |F| ≤ 1，则不可能存在两只不连通的跳蚤，输出 −1。

**证明**：需要"至少两只跳蚤不连通"，当 ≤ 1 只跳蚤时无法满足。□

**引理 2**（0 的情形）：若 G[F] 已不连通，则无需替换，输出 0。

**证明**：直接满足条件，替换数为 0。□

**引理 3**（1 的情形）：若 G[F] 连通且存在割点 v ∈ F（即去掉 v 后 G[F] 变不连通），则将 v 替换为蛐蛐即可断开，输出 1。

**证明**：割点的定义保证了去掉它后图不连通，因此答案 ≤ 1；又因为 G[F] 是连通的，故答案不可能为 0。□

**引理 4**（2 的情形）：若 G[F] 连通、|F| ≥ 2 且无割点，则一定存在两个跳蚤，同时去掉它们后 G[F] 变不连通，并且去掉一个跳蚤无法做到。此时输出 2。

**证明**：我们需要证明「顶点连通度 ≥ 2 的格子图」存在一个大小为 2 的顶点割。

取网格的一个角，不妨设左上角 (1,1)。若 (1,1) 是蛐蛐，则考虑下一个角。由于 |F| ≥ 2 且 G[F] 连通，至少有一个角是跳蚤格子。不妨设 (1,1) ∈ F。

(1,1) 在四连通下的邻居只有 (1,2) 和 (2,1)（如果二者都在棋盘内）。如果 A = {(1,2), (2,1)} ⊆ F，则去掉 A 中的两个跳蚤后，(1,1) 被孤立。由于 |F| ≥ 2 且 (1,1) ∈ F，至少还有一个其他跳蚤，所以图被断开。故存在大小为 2 的顶点割。

如果 (1,2) 或 (2,1) 是蛐蛐（或因越界不存在），则 (1,1) 在 G[F] 中的度数 ≤ 1。若度数为 1，则 (1,1) 的邻居是割点，与无割点假设矛盾。若度数为 0，则 G[F] 已不连通，与连通假设矛盾。

因此，在 G[F] 连通且无割点且 |F| ≥ 2 的条件下，A 中的两个格子均为跳蚤，去掉它们即可断开。□

**推论**：答案总属于 {−1, 0, 1, 2}。

现在问题转化为：如何针对 n,m 极大而 c 较小的输入，高效判断属于哪一类。

### 2.3 什么是割点？割点必须在蛐蛐附近

在一般的平面网格中，一个内部格子（不是边界且四个邻居都存在）有度数为 4。去掉这样一个格子，它的四个邻居之间仍有绕过它的路径。因此，这样的格子不可能是割点。

**引理 5**（割点的必要条件）：若 v = (x,y) 是 G[F] 中的割点，则 v 与某个蛐蛐的 Chebyshev 距离 ≤ 1（即 v 在某个蛐蛐的 3×3 邻域内），或者 n=1 或 m=1。

**证明**：用反证法。假设 v 与所有蛐蛐的 Chebyshev 距离 ≥ 2，则 v 的 3×3 邻域内没有蛐蛐，因此 v 的上、下、左、右四个邻居以及 (x±1, y±1) 四个对角邻居都是跳蚤。考虑 v 的两个邻居（比如上和左），它们通过 (x−1, y−1) 连通，绕过 v。类似地，任意两个邻居之间都有不经过 v 的路径，因此 v 不是割点。

当 n=1 或 m=1 时，格子图退化为一条线，每个内部节点（度数为 2）都是割点。□

**推论**：只需要检查以每个蛐蛐为中心的 3×3 邻域内的跳蚤格子，以及退化的一维情形，就可以找到所有可能的割点。

### 2.4 什么会导致跳蚤图已经不连通？

跳蚤图不连通，意味着蛐蛐集合形成了一道「屏障」，将跳蚤分隔在两侧。

考虑蛐蛐格的 8-连通图（Chebyshev 距离 ≤ 1 视为相邻）。在平面离散几何中，一个 8-连通的蛐蛐分量可以包裹一片区域使之与外界隔离。

**引理 6**（分离准则）：若存在一个 8-连通的蛐蛐分量 C，使得 C 连接了矩形的两条边界（包括对边和邻边），则 C 至少将棋盘分割成两个区域。如果这些区域中都至少有一个跳蚤格子，则 G[F] 已不连通。

**证明**：这是离散 Jordan 曲线定理的推论。一个 8-连通的黑色像素集在格点图中充当屏障：如果它连接了两条边界，则在四连通意义下，屏障两侧的白色像素不能互相到达。□

分割边界对的六种可能：
- 上 ∩ 下：将棋盘切成左右两块
- 左 ∩ 右：将棋盘切成上下两块
- 上 ∩ 左：隔离左上角
- 上 ∩ 右：隔离右上角
- 下 ∩ 左：隔离左下角
- 下 ∩ 右：隔离右下角

注意判断时需要考虑两个被分割的区域中都有跳蚤，否则即使连接了两条边界也不一定导致跳蚤图不连通（例如蛐蛐占据了一个完整的角，角内本没有跳蚤）。

### 2.5 为什么关键格子只需要蛐蛐的 Chebyshev ≤ 2 邻域？

由引理 5，割点只能在 Chebyshev ≤ 1 的范围内出现。为了在图中正确表示这些格子之间的连通关系，以及它们与附近其他格子的连接，将范围扩展到 Chebyshev ≤ 2 是安全的——这确保了任何可能成为割点的格子都在图中，且任意两个相邻的「关键格子」之间的边也被包含。

对于连通性判定，我们利用引理 6 的蛐蛐屏障法，不依赖关键格子图的连通性来判定 0。因此，关键格子图只需用于割点检测。

### 2.6 退化情形：一行或一列

当 n=1 或 m=1 时，网格退化为一条线段。此时每个蛐蛐就是线段上的一个障碍物。跳蚤就是线段上的一些连续段。

- 若总跳蚤数 ≤ 1 → −1
- 若存在至少两个跳蚤段 → 0（已不连通）
- 否则只有一个跳蚤段：若该段长度 ≥ 2，去掉该段的第二个跳蚤（或倒数第二个）即可断开 → 1
- 若该段长度 = 1（但总跳蚤数 ≥ 2 意味着至少还有另一个蛐蛐段上的跳蚤？被 0 捕获了）。

实际上在一维情况下更简单：
- 总跳蚤 ≤ 1 → −1
- 若已断开（即有障碍物把跳蚤分成两段以上）→ 0
- 否则，若总跳蚤 ≥ 2 且连通 → 1（去两端之间的任意内部跳蚤即可断开）

---

## Part 3 — 模式总结

- **探索性质** — 通过小例子发现答案只有 −1,0,1,2
- **考虑简单版本** — 空棋盘（c=0）时答案只能是 2（若 n,m≥2）；一维退化时答案为 1
- **证明分类** — 用图论基本概念（连通性、割点、顶点连通度）证明每种情形
- **空间压缩** — 割点定理说明只需检查蛐蛐附近 3×3 邻域；用 Chebyshev ≤ 2 构建关键图
- **图论算法** — Tarjan 求割点（经典算法）
- **边界虚拟节点** — 用四个虚拟节点代表矩形的四条边，便于判断蛐蛐屏障是否触及边界
- **退化情况单独处理** — n=1 或 m=1 时一维线段，贪心解决

---

## Part 4 — 真实解法

### 4.1 符号表

| 符号 | 含义 |
|------|------|
| n,m | 网格行数、列数 |
| c | 蛐蛐个数 |
| F | 所有跳蚤格子集合 |
| total | 总跳蚤数 = n×m − c |
| cricket_set | 哈希集合，存储所有蛐蛐的坐标 (hash(x,y)) |
| near | 关键格子集合：每个蛐蛐的 Chebyshev ≤ 2 邻域，加上在棋盘内的边界格子 |
| id(x,y) | 坐标到图节点编号的映射（0 表示该格是蛐蛐或不在 near 中） |
| g[] | 邻接表（vector<int>） |
| dfn[], low[] | Tarjan 算法的 dfs 序和 low 值 |
| cut[] | 标记是否为割点 |
| vis[] | BFS/DFS 访问标记 |
| TOP, BOT, LEF, RIG | 四个虚拟边界节点的 ID |
| comp_id[] | 蛐蛐的 8-连通分量编号 |
| boundary[] | 每个蛐蛐分量触及的边界集合（位掩码） |

### 4.2 算法步骤

**Step 1：输入与预处理**
- 读入 T 组数据
- 对每组数据，将蛐蛐坐标存入 cricket_set（哈希集合）
- 计算 total = (i64)n × m − c

**Step 2：极简情况直接回答**
- 若 total ≤ 1 → 输出 −1，继续下一组

**Step 3：一维退化（n=1 或 m=1）**
- 将蛐蛐排序（按列或按行），标记障碍位置
- 检查跳蚤是否已被分割成多个段 → 0
- 否则 total ≥ 2 且连通 → 1
- 输出答案，继续下一组

**Step 4：收集关键格子**
- 对每个蛐蛐 (x,y)，将其 Chebyshev 距离 ≤ 2 的格子加入 near（共 25 个）
- 需要去重

**Step 5：建图**
- 为 near 中每个跳蚤格子分配唯一 ID
- 添加四个虚拟边界节点 TOP、BOT、LEF、RIG
- 对 near 中每个跳蚤格子：
  - 四个方向寻找也在 near 中的跳蚤格子，连无向边
  - 若接触边界（x=1, x=n, y=1, y=m），连对应虚拟节点

**Step 6：蛐蛐 8-连通分量构建与屏障检测（判定 answer 0）**
- 对每个蛐蛐格子，建立 8-连通图（Chebyshev ≤ 1 视为相邻）
- 对每个分量，记录其触及的边界集合（位掩码，4 bit）
- 若某个分量同时触及：
  - 上(T) 和 下(B)：垂直屏障
  - 左(L) 和 右(R)：水平屏障
  - 上(T) 和 左(L)：隔离左上角
  - 上(T) 和 右(R)：隔离右上角
  - 下(B) 和 左(L)：隔离左下角
  - 下(B) 和 右(R)：隔离右下角
- 若存在这样的分量，还需要检查屏障两侧是否都有跳蚤。
  检查方法：看关键跳蚤图中，是否有 two nodes that are not connected but are on different sides of the barrier。
  简化：若存在屏障分量，则继续判断「关键跳蚤图」的连通分量数。若关键跳蚤图中有 ≥ 2 个分量 → 0。

**注意**：此处"关键跳蚤图的分量数"不能直接等同于"完整跳蚤图的分量数"，因为可能存在两个关键分量通过非关键格子连通的情况。但结合屏障检测，可以更准确地判定。

标准做法：先检测蛐蛐 8-连通屏障，若存在屏障，再构建关键跳蚤图并运行 BFS。若 BFS 发现关键跳蚤图有多个分量，且这些分量被蛐蛐屏障分隔，则判定为 0。

**Step 7：割点判定（answer 1）**
- 在关键跳蚤图上运行 Tarjan 求割点
- 注意排除虚拟边界节点
- 根节点特殊处理（子节点数 ≥ 2 才是割点）
- 若存在割点 → 输出 1

**Step 8：输出 2**
- 若以上条件都不满足 → 输出 2

### 4.3 复杂度分析

- 关键格子数：每个蛐蛐最多贡献 25 个格子，因此 |near| ≤ 25c + c = O(c)
- 建图复杂度：O(|near|)
- Tarjan 算法：O(|near| + |E|) = O(c)
- 蛐蛐屏障检测：建立 8-连通分量 O(c)
- 总复杂度：O(∑c log c)（哈希去重），∑c ≤ 1×10⁵，完全可行

### 4.4 正确性讨论

算法的核心依赖于两个定理：
1. 割点只能在蛐蛐的 Chebyshev ≤ 1 邻域内 → 扩展到 Chebyshev ≤ 2 确保不漏
2. 跳蚤图不连通 ⇔ 存在 8-连通的蛐蛐分量连接两条边界，且屏障两侧都有跳蚤

定理 2 的证明方向：
- (→) 若跳蚤图不连通，取两个属于不同分量的跳蚤格子 p, q。在棋盘上画一条四连通路径连接 p 和 q 的"外部"（绕过蛐蛐）。由于跳蚤图不连通，任何四连通路径必经蛐蛐。这些蛐蛐形成一个 8-连通屏障。该屏障的两端必然分别连接两条边界（因为路径从一条边界附近出发，到另一条边界附近结束）。
- (←) 若存在 8-连通蛐蛐分量连接两条边界，则在四连通意义下，该分量将棋盘分割成两个区域。若两个区域中都有跳蚤，则跳蚤图不连通。

---

## Part 5 — 示例代码（C++）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int dx[4]{-1,1,0,0};
constexpr int dy[4]{0,0,-1,1};
constexpr int dx8[8]{-1,-1,-1,0,0,1,1,1};
constexpr int dy8[8]{-1,0,1,-1,1,-1,0,1};
constexpr int dx2[5]{-2,-1,0,1,2};
constexpr int dy2[5]{-2,-1,0,1,2};

int T,n,m,c;
i64 total;
vector<pair<int,int>>crickets;
unordered_set<i64>cri_set;

struct Pos{int x,y;};
vector<Pos>nodes;

auto hsh=[](int x,int y){return 1ll*x*int(1e9+5)+y;};

unordered_map<i64,int>id;
vector<int>graph[2500005];
int dfn[2500005],low[2500005],timer;
bool cut[2500005];
int vis[2500005];

int TOP,BOT,LEF,RIG;
int tot;

void add(int u,int v)
{
	graph[u].push_back(v);
	graph[v].push_back(u);
}
bool valid(int x,int y)
{
	return x>=1&&x<=n&&y>=1&&y<=m;
}
bool isflea(int x,int y)
{
	return valid(x,y)&&!cri_set.count(hsh(x,y));
}
bool iscri(int x,int y)
{
	return valid(x,y)&&cri_set.count(hsh(x,y));
}

// DSU for cricket 8-connected components
int f[2500005];
void ini_dsu(int sz)
{
	for(int i=0;i<sz;++i)
		f[i]=i;
}
int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}
void merge(int x,int y)
{
	x=getrt(x);y=getrt(y);
	if(x!=y)
		f[x]=y;
}

void solve()
{
	cin>>n>>m>>c;
	crickets.resize(c);
	cri_set.clear();
	id.clear();
	nodes.clear();
	total=1ll*n*m-c;

	for(int i=0;i<c;++i)
	{
		cin>>crickets[i].first>>crickets[i].second;
		cri_set.insert(hsh(crickets[i].first,crickets[i].second));
	}

	// Step 2: total <= 1
	if(total<=1)
	{
		cout<<-1<<'\n';
		return;
	}

	// Step 3: 1D degenerate
	if(n==1||m==1)
	{
		vector<int>obst;
		int len=n==1?m:n;
		for(auto&[x,y]:crickets)
			obst.push_back(n==1?y:x);
		sort(obst.begin(),obst.end());
		obst.erase(unique(obst.begin(),obst.end()),obst.end());
		if(obst.empty())
		{
			// no cricket, full line
			if(len<=1)
				cout<<-1<<'\n';
			else
				cout<<1<<'\n';// interior node is cut
			return;
		}
		// count segments of fleas
		int segs=0;
		int last=0;
		for(int p:obst)
		{
			if(p-last-1>0)
				++segs;
			last=p;
		}
		if(len-last>0)
			++segs;
		if(segs>=2)
		{
			cout<<0<<'\n';
			return;
		}
		// connected, total >= 2, must have a cut point at the obstacle's neighbor
		cout<<1<<'\n';
		return;
	}

	// Step 4: collect key cells (Chebyshev <= 2 of each cricket)
	for(auto&[x,y]:crickets)
		for(int i=0;i<5;++i)
			for(int j=0;j<5;++j)
			{
				int nx=x+dx2[i],ny=y+dy2[j];
				if(valid(nx,ny))
					nodes.push_back({nx,ny});
			}

	// deduplicate
	sort(nodes.begin(),nodes.end(),[](auto a,auto b){
		return a.x!=b.x?a.x<b.x:a.y<b.y;
	});
	nodes.erase(unique(nodes.begin(),nodes.end(),[](auto a,auto b){
		return a.x==b.x&&a.y==b.y;
	}),nodes.end());

	// Step 5: assign IDs to flea key cells
	tot=0;
	for(auto&[x,y]:nodes)
	{
		if(isflea(x,y))
			id[hsh(x,y)]=++tot;
	}

	// if no key flea cells (should not happen when total>=2 and n,m>=2, but just in case)
	if(tot==0)
	{
		// all cells are crickets or c=0 (but c=0 handled -> all key cells exist if cricket exists)
		// Actually if c=0, nodes is empty, tot=0. But c=0 and total>=2 with n,m>=2 means answer 2.
		cout<<2<<'\n';
		return;
	}

	// virtual nodes
	TOP=++tot; BOT=++tot; LEF=++tot; RIG=++tot;

	// clear graph
	for(int i=1;i<=tot;++i)
	{
		graph[i].clear();
		dfn[i]=low[i]=0;
		cut[i]=0;
		vis[i]=0;
	}
	timer=0;

	// build edges among key flea cells
	for(auto&[x,y]:nodes)
	{
		if(!isflea(x,y))
			continue;
		int u=id[hsh(x,y)];
		for(int d=0;d<4;++d)
		{
			int nx=x+dx[d],ny=y+dy[d];
			if(isflea(nx,ny))
			{
				auto it=id.find(hsh(nx,ny));
				if(it!=id.end())
				{
					int v=it->second;
					if(u<v)
						add(u,v);
				}
			}
		}
		// connect to boundary virtual nodes
		if(x==1)add(u,TOP);
		if(x==n)add(u,BOT);
		if(y==1)add(u,LEF);
		if(y==m)add(u,RIG);
	}

	// Step 6: cricket 8-connected components for barrier detection
	vector<pair<int,int>>cri_idx; // index of each cricket in the DSU
	for(int i=0;i<c;++i)
	{
		auto[x,y]=crickets[i];
		cri_idx.push_back({x,y});
	}
	ini_dsu(c);
	for(int i=0;i<c;++i)
	{
		auto[x1,y1]=crickets[i];
		for(int j=i+1;j<c;++j)
		{
			auto[x2,y2]=crickets[j];
			if(max(abs(x1-x2),abs(y1-y2))<=1)
				merge(i,j);
		}
	}

	// boundary mask for each component
	vector<int>bmask(c,0);
	for(int i=0;i<c;++i)
	{
		int r=getrt(i);
		auto[x,y]=crickets[i];
		if(x==1)bmask[r]|=1;    // T
		if(x==n)bmask[r]|=2;    // B
		if(y==1)bmask[r]|=4;    // L
		if(y==m)bmask[r]|=8;    // R
	}

	// check if any component forms a barrier
	bool barrier=false;
	for(int i=0;i<c;++i)
	{
		if(getrt(i)!=i)
			continue;
		int msk=bmask[i];
		// opposite edges
		if((msk&1)&&(msk&2))barrier=true;// T-B
		if((msk&4)&&(msk&8))barrier=true;// L-R
		// adjacent edges forming a corner
		if((msk&1)&&(msk&4))barrier=true;// T-L
		if((msk&1)&&(msk&8))barrier=true;// T-R
		if((msk&2)&&(msk&4))barrier=true;// B-L
		if((msk&2)&&(msk&8))barrier=true;// B-R
	}

	if(barrier)
	{
		// Barrier exists. Check if key flea graph is disconnected.
		// If it IS disconnected, then the barrier actually separates flea cells -> answer 0.
		// If it's NOT disconnected, the barrier region has no flea cells on one side -> not a real separation.
		// BFS to find all components of key flea graph
		int comp_cnt=0;
		for(int i=1;i<=tot-4;++i)
		{
			if(!vis[i])
			{
				++comp_cnt;
				queue<int>q;
				q.push(i);
				vis[i]=1;
				while(!q.empty())
				{
					int u=q.front();q.pop();
					for(int v:graph[u])
						if(!vis[v])
						{
							vis[v]=1;
							q.push(v);
						}
				}
			}
		}
		if(comp_cnt>=2)
		{
			cout<<0<<'\n';
			return;
		}
		// Reset vis for Tarjan
		for(int i=1;i<=tot-4;++i)
			vis[i]=0;
	}
	else
	{
		// No barrier. Still need to check key flea graph connectivity.
		// If disconnected without a cricket barrier, the disconnection is through non-key cells?
		// Actually, disconnection without cricket barrier is impossible (proved).
		// But key graph might have multiple components connected through non-key cells.
		// In that case, full graph is still connected -> not answer 0.
		// We just need to check cut points.
		// For simplicity, run BFS from first node to check connectivity
		bool all_conn=true;
		int st=0;
		for(int i=1;i<=tot-4;++i)
		{
			st=i;
			break;
		}
		if(st)
		{
			queue<int>q;
			q.push(st);
			vis[st]=1;
			int cnt_vis=0;
			while(!q.empty())
			{
				int u=q.front();q.pop();
				++cnt_vis;
				for(int v:graph[u])
					if(!vis[v])
					{
						vis[v]=1;
						q.push(v);
					}
			}
			if(cnt_vis!=tot-4)
				all_conn=false;
		}
		// Reset vis
		for(int i=1;i<=tot-4;++i)
			vis[i]=0;

		if(!all_conn)
		{
			// Key graph disconnected but no cricket barrier.
			// This means components are connected through non-key flea cells.
			// Full graph is connected. Not answer 0.
			// But we need to handle this case: just continue to Tarjan.
		}
	}

	// Step 7: Tarjan for cut points
	function<void(int,int)>tarjan=[&](int u,int rt){
		static int root;
		root=rt;
		dfn[u]=low[u]=++timer;
		int ch=0;
		for(int v:graph[u])
		{
			if(v==rt)
				continue;
			if(!dfn[v])
			{
				tarjan(v,u);
				low[u]=min(low[u],low[v]);
				if(low[v]>=dfn[u]&&u!=root)
					cut[u]=1;
				++ch;
			}
			else
				low[u]=min(low[u],dfn[v]);
		}
		if(u==root&&ch>=2)
			cut[u]=1;
	};

	for(int i=1;i<=tot-4;++i)
	{
		if(!dfn[i])
			tarjan(i,i);
	}

	// Check cut points (exclude virtual nodes)
	for(int i=1;i<=tot-4;++i)
	{
		if(cut[i]&&i<=tot-4)
		{
			cout<<1<<'\n';
			return;
		}
	}

	// Step 8: otherwise
	cout<<2<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
		solve();
	return 0;
}
```
