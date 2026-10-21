# P1606 [USACO07FEB] Lilypad Pond G

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

$M\times N$ 的池塘，每格是水（`0`）、已有荷叶（`1`）、岩石（`2`）、起点荷叶（`3`）或目标荷叶（`4`）。
贝西站在起点上，每次按"马"的走法跳（共 $8$ 个方向），**只能落在荷叶上**（已有、新加、起点、终点都算）。
约翰可以在水格补种荷叶（不能种在岩石上），使贝西能从起点跳到终点。
求最少补种数；并在补种数最少的前提下求**不同补种方案数**（不同的水格集合）。无解输出 `-1` 且不输出第二行。

### 输入输出格式

- **输入格式**：第一行 $M,N$；随后 $M$ 行每行 $N$ 个整数（`0/1/2/3/4`）。
- **输出格式**：第一行最少补种数；第二行方案数（$64$ 位有符号整数内）。第一行为 $-1$ 时不输出第二行。
- **数据规模**：$1\le M,N\le30$，格子数与水格数都 $\le900$。

### 关键约束分析

1. **两问耦合**：第一问是"最短路的长度"，第二问是"最短路条数"，但第二问数的是**不同补种方案**而不是不同跳法，这是本题最大的坑。
2. **代价不对称**：落到已有荷叶不花钱，落到水格要花 $1$ 片，边权 $0/1$，必须用 0-1 BFS。
3. **起点与终点都是荷叶**：它们不花代价，并且必须参与"已有荷叶的连通性"计算。
4. **无解与零花费**：$d=\infty$ 时输出 `-1`；$d=0$ 时输出 `0` 与 `1`。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：把"补种"翻译成路径代价

固定补种集合 $X$。若存在一条 $S\to T$ 的马步路径，路径上落在水格的部分必须全属于 $X$，其余落点是已有荷叶。
于是"补种数"就是"路径上水格的个数"；反过来，把一条路径经过的水格全补上就能走通。

**引理 1（最小补种数 = 最短路径的水格数）**：设 $d(v)$ 为从 $S$ 到 $v$ 的马步路径中"经过水格个数"的最小值（落水格计 $1$，落荷叶计 $0$），
则最少补种数 $=d(T)$。

*证明*：任取可行集合 $X$ 及对应路径 $P$，$P$ 上每个水格都在 $X$ 中，故 $d(T)\le$（$P$ 的水格数）$\le|X|$，
所以 $d(T)$ 是不超过任何可行解的下界。另一方面，取一条达到 $d(T)$ 的路径，把它的水格全补上即得大小为 $d(T)$ 的可行集合。
下界可达，故最小值 $=d(T)$。$\square$

第一问因此是 $0/1$ 最短路：结点是格子，边是 $8$ 种马步（目标非岩石），边权为目标格是否为水格。

### 第二步：第二问难在哪

若问"最少跳法的条数"，直接在 0-1 BFS 的 DAG 上数路径即可。但本题问的是水格集合的个数，
同一个集合可能对应多条路径（同一片荷叶可以由不同荷叶路线接上），直接数路径会偏大。所以必须先把"路径"与"集合"的关系理清。

### 第三步：最短路径的结构——水格按距离分层

设 $d=d(T)$，一条最短路径为 $S=p_0,\dots,p_m=T$，其中水格依次为 $w_1,\dots,w_d$。

**引理 2（水格距离等于序号）**：对每个 $j$，$d(w_j)=j$。

*证明*：前缀 $S\to w_j$ 是这条路径的一段，用了 $j$ 个水格，故 $d(w_j)\le j$；
后缀 $w_j\to T$ 用了 $d-j$ 个水格，故 $d(T)\le d(w_j)+(d-j)$，即 $d(w_j)\ge j$。合并得 $d(w_j)=j$。$\square$

**引理 3（集合决定顺序）**：设最优集合 $|X|=d$，则 $X$ 中元素的距离两两不同且恰为 $1,2,\dots,d$，
因此 $X$ 上的合法顺序唯一（按距离升序）。

*证明*：取一条使用 $X$ 的最短路径。它的水格数为 $d=d(T)$，且都取自 $X$；若某个 $x\in X$ 未被用到，
则 $X\setminus\{x\}$ 仍然是可行集合且更小，与 $d$ 的最小性矛盾，故路径水格恰为 $X$。
由引理 2 这些格子的距离为 $1..d$ 且互不相同，于是"按距离排序"就是路径顺序，唯一。$\square$

引理 3 是计数正确性的核心：**同一个集合只有一种排列方式**，所以数"合法水格序列"就等于数"集合"。

### 第四步：相邻水格之间的连接条件

最短路径中相邻两个水格之间只可能夹着已有荷叶（否则水格序列就不是 $w_1..w_d$）。$S$ 与 $w_1$、$w_d$ 与 $T$ 同理。
把"只踩已有荷叶就能互通"记作 $\sim$，并用并查集把已有荷叶收缩成连通块。

**引理 4（$\sim$ 的刻画）**：对两个水格 $u,v$，$u\sim v$ 当且仅当：$u,v$ 直接马步相邻，或存在同一连通块 $C$ 使 $u,v$ 都与 $C$ 中某格马步相邻。

*证明*：充分性：直接相邻时一步即达；共享连通块时，在块内沿马步路径从 $u$ 的邻格走到 $v$ 的邻格，全程落在已有荷叶上。
必要性：设 $u\to l_1\to\cdots\to l_k\to v$ 的中间点全是已有荷叶，则 $l_1,\dots,l_k$ 同属一个连通块，且 $u$ 邻 $l_1$、$v$ 邻 $l_k$。$\square$

注意 $S,T$ 也是荷叶，所以"$S\sim w_1$"就是"$w_1$ 的邻域连通块集合包含 $S$ 的连通块"。

### 第五步：为什么不能直接数最短路条数

```
0 1 2 0 1
1 0 2 1 0
0 0 0 1 0
2 3 0 0 4
```

正确答案是"补 $1$ 片、$1$ 种方案"：只有唯一一个水格同时与起点的连通块、终点的连通块相邻。
但在 0-1 最短路 DAG 上数路径会得到 $2$——同一片荷叶被两条不同的荷叶绕行路线接上了，集合相同、路径不同。
所以计数必须在集合层面进行，而引理 3 正好提供了这个桥梁。

### 第六步：分层 DP

由引理 2、3，最优解等价于序列 $w_1,\dots,w_d$，满足 $d(w_j)=j$、$w_1\sim S$、$w_j\sim w_{j-1}$、$w_d\sim T$。
按距离分层 DP：$f(w)$ 表示"以 $w$ 结尾、恰好用 $d(w)$ 片"的方案数。

$$f(w)=\begin{cases}1,& d(w)=1,\ w\sim S\\ \sum_{u:\,d(u)=d(w)-1,\ u\sim w}f(u),& d(w)\ge2\end{cases}
\qquad \text{ans}=\sum_{w:\,d(w)=d,\ w\sim T}f(w)$$

**引理 5（DP 不重不漏）**：上式对每个最优集合恰好计数一次。

*证明*：由引理 3，每个最优集合 $X$ 唯一对应序列 $(w_1,\dots,w_d)$，其末元素 $w_d$ 是 $X$ 中距离最大者，唯一。
对 $k$ 归纳：$f(w_k)$ 统计的是"前 $k$ 个元素构成合法前缀"的方案数；扩展时不同前驱 $u$ 是集合中不同的元素，故对应不同集合；
同一个 $u$ 只通过"$\sim$"这一布尔判定贡献一次，不会把同一集合重复计入。逐层加完后每个集合恰被计一次。$\square$

---

## Part 3 — 模式总结 (Pattern Summary)

- `reduce to easier problem` — 把"补种数量"化为路径上落在水格的次数，第一问变成 0/1 最短路。
- `classic problem solution` — deque 的 0-1 BFS，$\mathcal{O}(MN)$ 求出每格的最小补种数。
- `explore its property` — 最短路径上第 $j$ 个水格的距离恰为 $j$（前后夹逼），水格因此按距离分层。
- `reduce to easier problem` — 用引理 3 把"数集合"降成"数按距离排序的序列"。
- `classic problem solution` — 并查集收缩已有荷叶，把"只踩荷叶能否互通"降成一次连通块判定。
- `defer counting` — 最后按距离分层做线性 DP，避免在同一层内重复枚举路径。
- `discuss by category` — 分 $d=\infty$、$d=0$、$d\ge1$ 三种情形输出。

---

## Part 4 — 真正的解法 (The Real Solution)

### 符号表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $M,N$ | `M`, `N` | 行数、列数 |
| — | `g[i][j]` | 格子类型：`0` 水、`1` 荷叶、`2` 岩石、`3` 起点、`4` 终点 |
| $S,T$ | `sx,sy` / `tx,ty` | 起点、终点坐标 |
| $d(v)$ | `dis[i][j]` | 到该格的最少"落在水格"次数，初值 `inf`，由 0-1 BFS 求出 |
| — | `fa[]` | 并查集父指针，只对荷叶（`1/3/4`）初始化，按马步合并 |
| — | `getrt(x)` | 并查集查找（路径压缩） |
| — | `lily(v)` | 格子类型是否为荷叶 |
| — | `kn(x1,y1,x2,y2)` | 两点是否构成马步 |
| $C(w)$ | `adjc[i][j][0..adjcn-1]` | 水格邻域中荷叶连通块的编号（去重），`adjcn` 为个数 |
| — | `hasc(i,j,c)` | 判定 $c\in C(w)$，即 $w\sim$（连通块 $c$） |
| — | `compat(x1,y1,x2,y2)` | 判定两水格是否满足 $\sim$ |
| $f(w)$ | `dp[i][j]` | 以该水格结尾、恰好用 $d(w)$ 片荷叶的方案数 |
| $d$ | `d` | `dis[tx][ty]`，最少补种数 |
| $C(S),C(T)$ | `cs`, `ct` | 起点、终点所在连通块编号 |
| — | `dx[8]`,`dy[8]` | 八个马步方向 |
| — | `Q` | 0-1 BFS 用的 `deque`，权 $0$ 的边从队头入、权 $1$ 的边从队尾入 |
| — | `w` | 本次跳跃的边权：落水格 $1$、落荷叶 $0$ |
| — | `c`, `dup` | 邻格所属连通块编号、以及"是否已在 `adjc` 中"的去重标记 |
| $ans$ | `ans` | 最少补种数下的方案总数 |

### 算法步骤

1. 读入网格（同时把 `dis` 置 `inf`），记录 $S,T$。
2. 对荷叶建并查集，按马步合并成连通块。
3. 以 $S$ 为源做 0-1 BFS（落水格权 $1$、落荷叶权 $0$、岩石不走）。$d=\infty$ 输出 `-1`；$d=0$ 输出 `0` 与 `1`。
4. 预处理每个水格的 `adjc/adjcn`（邻域荷叶连通块，去重）。
5. 按 $k=1..d$ 分层：对 `dis=k` 的水格 $w$，$k=1$ 时以 `hasc(w,cs)` 为判据（起点块相当于第 $0$ 层、方案数 $1$），
   $k\ge2$ 时累加所有 `dis=k-1` 且与其 `compat` 的水格的 $f$。
6. 答案 $=\sum_{w:\,dis(w)=d,\ hasc(w,ct)}f(w)$，与 $d$ 一起输出。

### 复杂度分析

- **时间复杂度**：并查集 $\mathcal{O}(MN\alpha)$；0-1 BFS $\mathcal{O}(MN)$；分层 DP 的枚举量 $\sum_k|\{w:dis=k\}|\cdot MN\cdot 64$，最坏约 $5\times10^7$ 次比较，$MN\le900$ 时实测毫秒级。
- **空间复杂度**：$\mathcal{O}(MN)$，几个 $35\times35$ 数组加一个并查集。

### 正确性验证

引理 1 给出第一问最优值；引理 2 给出水格的距离分层；引理 3 保证"数集合 = 数序列"；引理 4 把"只踩荷叶互通"判成连通块关系；
引理 5 证明分层 DP 对每个集合恰好计一次；最后按 $d=\infty/0/\ge1$ 分类覆盖退化情形。
方案数用 `i64`（题目保证在 $64$ 位有符号整数内），距离最大 $900$，无溢出风险。

---

## Part 5 — 示例代码 (Example Code)

```cpp
//
// 题目：P1606 [USACO07FEB] Lilypad Pond G
// https://www.luogu.com.cn/problem/P1606
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{35};                    // M,N<=30 (+5 松弛)
constexpr int inf{1145141919};
constexpr int dx[8]{1,1,2,2,-1,-1,-2,-2},dy[8]{2,-2,1,-1,2,-2,1,-1};

int M,N,g[mxn][mxn],dis[mxn][mxn],sx,sy,tx,ty;
int fa[mxn*mxn];                          // 已有荷叶的并查集
int adjc[mxn][mxn][8],adjcn[mxn][mxn];    // 水格邻域中荷叶连通块(去重)
i64 dp[mxn][mxn];                         // 恰好用 dis 片荷叶到达该水格的方案数

int getrt(int x){return fa[x]==x?x:fa[x]=getrt(fa[x]);}

bool lily(int v){return v==1||v==3||v==4;}

bool kn(int x1,int y1,int x2,int y2)
{
	int a=abs(x1-x2),b=abs(y1-y2);
	return (a==1&&b==2)||(a==2&&b==1);
}

bool hasc(int x,int y,int c)              // 水格能否只踩已有荷叶走到连通块 c
{
	for(int i=0;i<adjcn[x][y];++i)
		if(adjc[x][y][i]==c)
			return true;
	return false;
}

bool compat(int x1,int y1,int x2,int y2)  // 两水格之间能否不加水直接连通
{
	if(kn(x1,y1,x2,y2))
		return true;
	for(int i=0;i<adjcn[x1][y1];++i)
		for(int j=0;j<adjcn[x2][y2];++j)
			if(adjc[x1][y1][i]==adjc[x2][y2][j])
				return true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>M>>N;
	for(int i=1;i<=M;++i)
		for(int j=1;j<=N;++j)
		{
			cin>>g[i][j];
			if(g[i][j]==3)sx=i,sy=j;
			if(g[i][j]==4)tx=i,ty=j;
			dis[i][j]=inf;
		}

	for(int i=1;i<=M;++i)                 // 并查集只含已有荷叶(1/3/4)
		for(int j=1;j<=N;++j)
			if(lily(g[i][j]))
				fa[i*N+j]=i*N+j;
	for(int i=1;i<=M;++i)                 // 按马步合并连通块
		for(int j=1;j<=N;++j)
		{
			if(!lily(g[i][j]))
				continue;
			for(int k=0;k<8;++k)
			{
				int x=i+dx[k],y=j+dy[k];
				if(x<1||x>M||y<1||y>N||!lily(g[x][y]))
					continue;
				int u=getrt(i*N+j),v=getrt(x*N+y);
				if(u!=v)
					fa[u]=v;
			}
		}

	deque<pair<int,int>>Q;                // 0-1 BFS: 落水格代价 1, 落荷叶代价 0
	dis[sx][sy]=0;
	Q.push_front({sx,sy});
	while(!Q.empty())
	{
		auto[x,y]=Q.front();
		Q.pop_front();
		for(int k=0;k<8;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<1||nx>M||ny<1||ny>N||g[nx][ny]==2)
				continue;
			int w=g[nx][ny]==0?1:0;
			if(dis[x][y]+w<dis[nx][ny])
			{
				dis[nx][ny]=dis[x][y]+w;
				if(w==0)
					Q.push_front({nx,ny});
				else
					Q.push_back({nx,ny});
			}
		}
	}

	int d=dis[tx][ty];
	if(d==inf)
	{
		cout<<-1<<'\n';
		return 0;
	}
	if(d==0)                              // 全靠已有荷叶可达, 唯一方案是不加
	{
		cout<<0<<'\n'<<1<<'\n';
		return 0;
	}

	for(int i=1;i<=M;++i)                 // 每个水格"免费"能触及的荷叶连通块
		for(int j=1;j<=N;++j)
		{
			if(g[i][j]!=0)
				continue;
			for(int k=0;k<8;++k)
			{
				int x=i+dx[k],y=j+dy[k];
				if(x<1||x>M||y<1||y>N||!lily(g[x][y]))
					continue;
				int c=getrt(x*N+y),dup=0;
				for(int q=0;q<adjcn[i][j];++q)
					if(adjc[i][j][q]==c)
						dup=1;
				if(!dup)
					adjc[i][j][adjcn[i][j]++]=c;
			}
		}

	int cs=getrt(sx*N+sy),ct=getrt(tx*N+ty);
	for(int k=1;k<=d;++k)                 // 按 dis 分层, 第 k 层恰好用 k 片新荷叶
		for(int i=1;i<=M;++i)
			for(int j=1;j<=N;++j)
			{
				if(g[i][j]!=0||dis[i][j]!=k)
					continue;
				i64 s=0;
				if(k==1)
					s=hasc(i,j,cs)?1:0;
				else
					for(int x=1;x<=M;++x)
						for(int y=1;y<=N;++y)
							if(g[x][y]==0&&dis[x][y]==k-1&&dp[x][y]&&compat(x,y,i,j))
								s+=dp[x][y];
				dp[i][j]=s;
			}

	i64 ans=0;
	for(int i=1;i<=M;++i)
		for(int j=1;j<=N;++j)
			if(g[i][j]==0&&dis[i][j]==d&&dp[i][j]&&hasc(i,j,ct))
				ans+=dp[i][j];
	cout<<d<<'\n'<<ans<<'\n';
	return 0;
}
```

### 易错点

1. **直接数"最短路条数"**：同一片荷叶可由多条荷叶路线接入，会重复计数（反例见 Part 2 第五步：正确答案 $1$，数路径得 $2$）。
2. **用普通 BFS 求最小补种数**：边权有 $0$ 有 $1$，必须 0-1 BFS。
3. **忘记 $d=0$ 的分支**：只靠已有荷叶可达时输出 `0` 与 `1`，否则会输出 `0` 与 `0`。
4. **$d=\infty$ 时多输出一行**：`-1` 之后不能输出第二行。
5. **水格邻域连通块不去重**：同一块可能经多个邻格出现，不去重会把布尔判定变成计数，答案偏大。
6. **把 $S/T$ 当水格**：它们是荷叶，必须进并查集，否则 `cs/ct` 无意义。

### 本地验证结果

题面归档无样例段，用题面 Hint 的示例（$4\times5$，最少 $2$ 片、$3$ 种方案）作基准，并与"枚举水格子集"的暴力（`P1606_brute.cpp`）对拍。

| 测试项 | 覆盖范围 | 结果 |
| :--- | :--- | :--- |
| 题面 Hint 示例 | $M=4,N=5$，应为 $2$ 与 $3$ | 输出 `2 / 3`，通过 |
| 重复计数反例 | 上文 $4\times5$ 网格（数路径得 $2$、正确答案 $1$） | 输出 `1`，通过 |
| 退化情形 | $1\times2$ 无合法马步输出 `-1`；$2\times3$ 起终点马步相邻（$d=0$）输出 `0 / 1` | 通过 |
| `P1606_check.py` 随机对拍 | $204$ 组：$160$ 组小网格（$M,N\le4$）+ $40$ 组 $5\times5$（水格 $\le17$）+ $4$ 组手工用例，与子集枚举暴力比对两行输出 | $204$ 组全部一致 |
| 满规模性能 | $30\times30$ 全水（$d=19$，方案数 $2720952$） | $0.002$s |
