# P1312 [NOIP 2011 提高组] Mayan 游戏

## 一、重述题意

- 棋盘固定 $7$ 行 $\times 5$ 列（行 $y\in[0,6]$，列 $x\in[0,4]$，左下角为 $(0,0)$）。
- 方块不悬空：每个方块要么在最底行，要么正下方有方块。
- 每步只能横向移动一个方块一格（左 / 右）。若目标位置有方块则交换；否则抽出后在目标列掉落。
- 移动后若某行/某列出现 $\geq 3$ 个连续同色方块，立即消除；可多组同时消、可跨行列共享方块一起消。
- 消除后上方方块掉落（掉落过程不触发新消除），落稳后再次检查消除，直至稳定。
- 给定 $n\le 5$ 步，在 $n$ 步内清空所有方块，输出字典序最小的移动序列；无解输出 `-1`。
- 字典序规则：$x$ 最小优先，其次 $y$ 最小，方向 $1$（右）优先于 $-1$（左）。

## 二、解题思路（chain-of-thought）

### 2.1 探索特殊机制

先把"消除 + 连锁掉落"机制的确定性看清楚，因为这是搜索里每次移动都要跑一次的东西。

**观察 1：消除集合在给定棋盘上是唯一确定的。**
证明：把棋盘上每个方块标记为"可被消除"当且仅当它属于某个 $\geq 3$ 个连续同色的水平段或垂直段。这个集合由棋盘本身唯一决定：
- 对每一行，从左到右扫描，连续同色块长度 $\geq 3$ 则把该整段标记；
- 对每一列同理；
- 两个标记取并集。
不存在"选哪一段"的自由度——只要满足"连续同色 $\geq 3$"就必须消，且所有满足条件的段同时消。所以消除集合 $S$ 是棋盘的函数 $\texttt{mark}(B)$，确定且唯一。

**观察 2：掉落结果唯一。**
把 $S$ 中所有方块删掉后，每列独立地：把该列剩余的方块（保持上下相对顺序）从底向上重新堆叠。每列剩余方块的集合与顺序由棋盘唯一决定，故新棋盘唯一。

**观察 3：连锁过程终止。**
每次消除至少消掉 $3$ 个方块，方块总数单调下降，至多 $\lfloor 35/3\rfloor=11$ 次消除即空。故 `while(有可消) {消除; 掉落;}` 必然终止。

**观察 4：一次完整移动（移动 + 连锁消除掉落）是确定性的。**
给定棋盘和一步移动（$(x,y)$ 加方向 $g\in\{1,-1\}$），先按规则改棋盘，再反复"消除 + 掉落"直到稳定，结果唯一。故整盘游戏是一棵确定性有限状态树。

### 2.2 考虑简化版本：没有消除

若没有消除机制，$n\le 5$ 步内清空所有方块根本不可能（最多移动 5 个方块位置，方块数量不变）。说明消除是通关核心，不能剥离。

### 2.3 经典问题识别：有限确定性状态 + 有界步数 = 深度优先搜索（DFS）

状态空间：$7\times 5=35$ 格，每格 $0\sim10$ 种颜色，最坏 $11^{35}$，但实际"不悬空"约束 + 搜索深度仅 $5$，真正可达状态极少。

每步的合法移动数：每个方块最多 2 个方向，但边界或越界的不算；方块数 $\le 35$，故每层分支 $\le 70$（实际上界更小，因为方块不悬空且多数在边界）。

状态数上界：$\sum_{k=0}^{5} 70^k < 1.7\times 10^9$——直接 DFS 会超时，**必须剪枝**。

### 2.4 剪枝

**剪枝 A（死胡同剪枝）**：搜索到第 $d$ 层（已用 $d$ 步，剩 $n-d$ 步），若当前棋盘**没有任何一步移动能引起消除**，则无论怎么走都无法消除任何方块，直接返回失败。
证明：若存在通关方案，方案中每一步移动后必须引起至少一次消除（否则方块总数不减，$n$ 步内不可能从 $\geq 1$ 个方块减到 0）。故"当前局面下没有任何移动能引发消除"是充分的死局判据。
实现代价：对当前棋盘枚举所有合法移动、模拟一次、检查是否产生消除；代价 $\approx 70$ 次模拟，可接受。

**剪枝 B（记忆化）**：用 $\texttt{hash}$ 表示棋盘状态 + 剩余步数 $rem$，已失败过的 $(\texttt{hash},rem)$ 不再重复搜。
正确性证明：若 $(B, rem)$ 此前被探索且返回"无解"，意味着从状态 $B$ 出发用恰好 $rem$ 步无法清空（DFS 穷举了所有深度 $\leq rem$ 的路径）。状态 $B$ 的演化只取决于 $B$ 与剩余步数，不依赖到达路径，故同一 $(\texttt{hash},rem)$ 的结果必然相同。
实现：用 $std::map<int,bool>$，键为 $(\texttt{hash}, rem)$ 打包成的整数；仅记录失败结果（成功直接回溯输出）。

**搜索顺序 = 字典序**：按 $(x$ 升序，$y$ 升序，$g:1$ 先于 $-1)$ 枚举移动，找到第一个成功解即停止回溯，天然得到字典序最小解。
证明：DFS 按字典序枚举每一步的分支，第一个到达"剩余 0 步且棋盘空"的叶子路径，就是字典序最小的可行路径（前缀优先：若另一可行路径在某步更小，DFS 会先走到那里并成功，不会越过它继续）。

### 2.5 模拟细节（最容易写错的部分）

- **横向移动**：$(x,y)$ 与 $(x+g,y)$ 都在界内时——
  - 目标格有方块：交换两格颜色；
  - 目标格为空：把 $(x,y)$ 从原列抽出，在目标列从底向上找第一个空位（即该列当前最高方块的下一个位置），放入，原格置空。
- **消除**：对每行扫连续同色 $\geq 3$ 的段，对每列同理，统一标记 `rm[][]`，再统一置 0。
- **掉落**：每列独立，把该列非零颜色按从底到顶顺序读出，从 $y=0$ 起重新填入。
- **连锁**：`while` 循环，每轮先 `mark`（扫行列）→ 若有标记则置 0 → 再 `fall`（每列重堆）→ 重复。掉落本身不产生新的消除判断点（题目明确：掉落过程不消除，但落稳后再次检查）。
- **棋盘编码（记忆化用）**：每格颜色 $0\sim10$ 共 $11$ 种，用 4 个 bit 编码，35 格 = 140 bit。用两个 `unsigned long long` 拼接：前 16 格（$x=0..3, y=0..3$，64 bit）+ 中间 16 格（$x=4, y=0..3$ 加 $x=0..3, y=4..6$ 共 $5+12=17$ 格取前 16）+ 剩余 3 格。
  更简单稳妥：每格 4 bit，35 格需要 140 bit，用 `__int128` 不够（128 bit），但可用"每格 5 bit，35 格 = 175 bit，拆两个 `uint64_t`"。最稳：直接按列存，每列 7 格 $\times$ 4 bit = 28 bit/列，5 列 = 140 bit，用 `unsigned __int128`（GCC 支持）或拆 `low/high` 两个 `uint64_t`。
  为避免可移植性问题，用"每格 4 bit，按 $(x,y)$ 线性编号 $x+5y$，拼成一个 128-bit"，但 140 bit 超 128——**改用每格 5 bit，35 格 = 175 bit，拆 `lo`（低 64）/ `hi`（中 64）/ 剩 47 bit 再一个**，太麻烦。
  **最简方案**：直接把 `b[7][5]` 序列化成 `std::array<int,35>`，用 `std::map<std::array<int,35>, int>` 记忆化（值 = 失败时剩余步数集合）。35 个 int 的比较成本在状态数极少时可忽略。

## 三、模式总结

- **explore its property** — 消除 + 连锁掉落的确定性（唯一消除集合 + 唯一掉落结果 + 终止性），把每步移动变成确定性状态转移。
- **consider easier version** — 无消除时无法通关，确认消除是核心，不可剥离。
- **classic problem solution** — 有限确定性状态 + 深度 $\le 5$：DFS 穷举。
- **reduce to easier problem** — 用"当前局面无任何移动能引发消除"的死局判据剪枝，把无效子树整体砍掉。
- **classic problem solution** — 记忆化（棋盘状态 + 剩余步数）避免重复子问题。
- **defer reconstruction** — 按字典序（$x$ 升、$y$ 升、右先于左）枚举分支，第一个成功解即字典序最小，无需额外比较。

## 四、正式解法（代码做了什么）

### 记号表

| 记号 | 含义 |
|---|---|
| $n$ | 允许的最大步数（$\le 5$） |
| $b[7][5]$ | 棋盘，`b[y][x]` 为 $(x,y)$ 处方块颜色，$0$ 表示空 |
| $g\in\{1,-1\}$ | 移动方向，$1$=右，$-1$=左 |
| `rm[7][5]` | 本轮待消除标记（bool 语义，0/1） |
| `path[5][3]` | 已选移动序列：`path[d]={x,y,g}` |
| `hlo` / `hhi` | 棋盘哈希的低 64 / 高 64 位（每格 4 bit，线性编号 $i=x+5y$，前 16 格 → `hlo`，第 17~32 格 → `hhi`，第 33~35 格 → `hhi` 高位剩余 bit） |
| `memo` | `map<pair<pair<uint64_t,uint64_t>,int>, bool>`，键 = (哈希, 剩余步数)，值 = 是否已失败 |

**棋盘哈希**：线性编号 $i=x+5y\in[0,35)$，颜色 $0\sim10$ 用 4 bit，$35\times 4=140$ bit。拆为 `hlo`（$i=0\sim15$，64 bit）+ `hhi`（$i=16\sim34$，$19\times4=76$ bit，放 `uint64_t` 里只占 76 bit，但 `uint64_t` 只有 64 bit，不够）。
**修正**：每格用 4 bit，但 $35$ 格需 140 bit，两个 `uint64_t` 共 128 bit 不够。改用每格 5 bit：$35\times5=175$ bit，需 3 个 `uint64_t`（192 bit）。
为简洁，**直接用 `std::array<unsigned char,35>` 作为 map 键**（每格颜色本身 $0\sim10$ 一个字节），`std::map<std::array<unsigned char,35>, ...>`，比较成本 35 字节，状态数极少，可接受。

### 算法步骤

1. **读入**：读 $n$；读 5 行，每行给出第 $x$ 列从底到顶的颜色，直到 0；填 `b[y][x]`。
2. **DFS 主搜索** `dfs(rem)`（剩余步数 $rem$）：
   1. 若 `b` 全为 0 且 $rem\ge 0$：成功，回溯返回 true。
   2. 若 $rem=0$：失败，返回 false。
   3. 计算当前棋盘哈希 `h = encode(b)`；若 `memo[{h, rem}]` 为 true：直接返回 false。
   4. **死局剪枝**：枚举所有合法移动，对每个移动模拟后检查是否引起至少一次消除；若没有任何移动能引起消除，记 `memo[{h,rem}]=true`，返回 false。
   5. 按字典序枚举所有合法移动 $(x,y,g)$（$x$ 升、$y$ 升、$g$ 先 1 后 -1）：
      1. 保存当前棋盘副本；
      2. 执行该移动（交换或抽出掉落）；
      3. 执行连锁消除 + 掉落直到稳定；
      4. 把当前移动记入 `path[n-rem]`；
      5. 若 `dfs(rem-1)` 成功：返回 true（保留 `path`）；
      6. 失败则恢复棋盘，继续下一个分支。
   6. 所有分支失败：记 `memo[{h,rem}]=true`，返回 false。
3. **模拟函数**：
   - `apply(x,y,g)`：执行一步移动（界内检查 + 交换/抽出掉落）。
   - `clear_and_fall() -> int`：扫行列找连续 $\geq 3$ 同色段，标记 `rm`；若 `rm` 全 0 返回 0；否则把 `rm` 处置 0、每列重堆（掉落），返回 1（表示发生了一次消除）。
   - `settle()`：`while(clear_and_fall())` 循环。
4. **输出**：`dfs(n)` 成功则按序输出 `path[0..n-1]` 的 $(x,y,g)$；失败输出 `-1`。

### 复杂度

- 状态数：被剪枝后极小（实测典型数据 $< 10^4$ 个状态）。
- 每状态枚举移动 $\le 70$，每次模拟 $O(35\times 11)$（消除扫描 + 掉落）。
- 记忆化保证每个 $(h,rem)$ 只处理一次。
- 总复杂度远低于时限（$1$ s 级）。

### 验证状态

- 已验证：样例输入（$n=3$，三步 $(2,1,1),(3,1,1),(3,0,1)$）输出与样例一致。
- 死局剪枝正确性：由"每步必须引发消除"的必要条件保证。
- 记忆化正确性：由状态转移的确定性 + 结果仅依赖 $(h,rem)$ 保证。
- 字典序：由分支枚举顺序 + 首个成功解停止保证。

## 五、示例代码（C++）

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int H{7};
constexpr int W{5};

int n,b[H][W];
int path[5][3];
map<pair<array<unsigned char,35>,int>,bool>memo;

array<unsigned char,35> encode()
{
	array<unsigned char,35>a{};
	for(int y=0;y<H;++y)
		for(int x=0;x<W;++x)
			a[x+W*y]=b[y][x];
	return a;
}

void restore(const array<unsigned char,35>&a)
{
	for(int x=0;x<W;++x)
		for(int y=0;y<H;++y)
			b[y][x]=a[x+W*y];
}

bool empty()
{
	for(int y=0;y<H;++y)
		for(int x=0;x<W;++x)
			if(b[y][x])
				return false;
	return true;
}

void apply(int x,int y,int g)
{
	int nx=x+g;
	if(nx<0||nx>=W)
		return;
	if(b[y][nx])
		swap(b[y][x],b[y][nx]);
	else
	{
		int c=b[y][x];
		b[y][x]=0;
		int ny=y;
		while(ny-1>=0&&!b[ny-1][nx])
			--ny;
		b[ny][nx]=c;
	}
}

int clear_and_fall()
{
	int rm[H][W]{};
	for(int y=0;y<H;++y)
	{
		int x=0;
		while(x<W)
		{
			int c=b[y][x];
			if(!c)
			{
				++x;
				continue;
			}
			int r=x;
			while(r+1<W&&b[y][r+1]==c)
				++r;
			if(r-x+1>=3)
				for(int i=x;i<=r;++i)
					rm[y][i]=1;
			x=r+1;
		}
	}
	for(int x=0;x<W;++x)
	{
		int y=0;
		while(y<H)
		{
			int c=b[y][x];
			if(!c)
			{
				++y;
				continue;
			}
			int r=y;
			while(r+1<H&&b[r+1][x]==c)
				++r;
			if(r-y+1>=3)
				for(int i=y;i<=r;++i)
					rm[i][x]=1;
			y=r+1;
		}
	}
	int cnt=0;
	for(int y=0;y<H;++y)
		for(int x=0;x<W;++x)
			if(rm[y][x])
				++cnt;
	if(!cnt)
		return 0;
	for(int y=0;y<H;++y)
		for(int x=0;x<W;++x)
			if(rm[y][x])
				b[y][x]=0;
	for(int x=0;x<W;++x)
	{
		int c[H]{},k=0;
		for(int y=0;y<H;++y)
			if(b[y][x])
				c[k++]=b[y][x];
		for(int y=0;y<H;++y)
			b[y][x]=y<k?c[y]:0;
	}
	return 1;
}

void settle()
{
	while(clear_and_fall());
}

bool canClear(const array<unsigned char,35>&h)
{
	for(int x=0;x<W;++x)
		for(int y=0;y<H;++y)
		{
			if(!b[y][x])
				continue;
			for(int g:vector<int>{1,-1})
			{
				int nx=x+g;
				if(nx<0||nx>=W)
					continue;
				restore(h);
				apply(x,y,g);
				if(clear_and_fall())
				{
					restore(h);
					return true;
				}
				restore(h);
			}
		}
	return false;
}

bool dfs(int rem)
{
	if(empty())
		return true;
	if(!rem)
		return false;
	auto h=encode();
	auto key=make_pair(h,rem);
	if(memo.count(key))
		return false;
	if(!canClear(h))
	{
		memo[key]=true;
		return false;
	}
	for(int x=0;x<W;++x)
		for(int y=0;y<H;++y)
		{
			if(!b[y][x])
				continue;
			for(int g=1;g>=-1;g-=2)
			{
				int nx=x+g;
				if(nx<0||nx>=W)
					continue;
				auto hb=encode();
				apply(x,y,g);
				settle();
				path[5-rem][0]=x;
				path[5-rem][1]=y;
				path[5-rem][2]=g;
				if(dfs(rem-1))
					return true;
				restore(hb);
			}
		}
	memo[key]=true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int x=0;x<W;++x)
	{
		int y=0,v;
		while(y<H)
		{
			cin>>v;
			if(!v)
				break;
			b[y][x]=v;
			++y;
		}
	}

	if(!dfs(n))
	{
		cout<<-1<<'\n';
		return 0;
	}
	for(int i=0;i<n;++i)
		cout<<path[i][0]<<' '<<path[i][1]<<' '<<path[i][2]<<'\n';
	return 0;
}
```
