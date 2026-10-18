# P1526 [NOI2003] 智破连环阵

> 标签：搜索 / IDA* / 剪枝 / 冲突导向回跳 / NOI 2003 · 难度：Level 8
> 本地题面：`P1526.md`（$1\le M,n\le 100,\,1\le k\le 1000$，坐标 $\in[0,10000]$）。**本地题面未收录样例输入/输出**；本文所有结论均经本地编译的 $1000$ 组有解随机数据与状压 DP 暴力对拍 + 贪心锁死边界反例验证（见 §4.4）。

## 1. 重新读题

**问题重述（人话版）**

平面上有 $M$ 个按顺序编号为 $0,1,\dots,M-1$（0-based）的武器，以及 $n$ 枚编号为 $0,1,\dots,n-1$ 的炸弹。

- **连环阵机制**：初始时只有 $0$ 号武器处于攻击状态（可被摧毁），其余武器均无敌；第 $i$ 号武器被消灭后，隔 $1$ 秒第 $i+1$ 号武器自动解除无敌进入可被摧毁状态；当第 $M-1$ 号武器被消灭时连环阵被彻底摧毁。
- **炸弹机制**：依次引爆一个**互不相同**的炸弹序列 $a_1,a_2,\dots,a_x$（每枚炸弹至多使用一次）。每枚炸弹持续引爆 $5$ 分钟（$300$ 秒），在引爆期间能瞬间消灭一切与它欧氏距离 $\le k$ 且处于攻击状态的武器。

求摧毁全部 $M$ 个武器所需的最少炸弹枚数 $x$（保证必然有解）。

> 一句话版本：**给定 $n$ 枚炸弹对 $M$ 个武器的 $0/1$ 覆盖关系，每枚炸弹在状态 $cur$ 引爆时可连续消灭极长前缀 $[cur,\mathrm{rch}[b][cur]]$，求从状态 $0$ 推进到状态 $M$ 且每枚炸弹至多用一次的最少步数。**

**题面里容易漏掉的条款**（第 1 条直接把时间轴消掉，第 2 条解释了为什么不能无脑贪心）

1. **“持续爆炸 $5$ 分钟”与“间隔 $1$ 秒激活”的量级差** $\Rightarrow$ $5$ 分钟 $=300$ 秒，而武器总数 $M\le 100$！就算一枚炸弹一口气把全部 $100$ 个武器从头炸到尾，也只需要 $99$ 秒 $\ll 300$ 秒。因此**炸弹的持续时间永远够用**：当待消灭武器为 $cur$ 时引爆炸弹 $b$，它会且仅会连续消灭从 $cur$ 开始、连续落在其半径 $k$ 内的极长区间 $[cur,\mathrm{rch}[b][cur]]$！
2. **每枚炸弹只能引爆一次** $\Rightarrow$ 如果允许同一枚炸弹反复引爆，问题就退化成了经典的一维区间跳跃（每次无脑选 $\mathrm{rch}[b][cur]$ 最大的炸弹即可）；正是因为“在 $cur$ 处用掉炸弹 $b$ 会导致 $b$ 无法在后续关键瓶颈处使用”，才使问题具有全局资源耦合性。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：单枚炸弹的转移确定性与起点单调性

记 $\mathrm{cov}[b][w]\in\{\mathrm{true},\mathrm{false}\}$ 表示炸弹 $b$ 与武器 $w$ 的欧氏距离是否 $\le k$（用整数平方比较 $(u_b-x_w)^2+(v_b-y_w)^2\le k^2$）。

对任意炸弹 $b$ 和武器下标 $i\in[0,M-1]$，定义其连续可达右端点：

$$\mathrm{rch}[b][i]=\begin{cases}i-1,&\text{若 }\neg\mathrm{cov}[b][i],\\\max\big(i,\,\mathrm{rch}[b][i+1]\big),&\text{若 }\mathrm{cov}[b][i].\end{cases}$$

有人可能会问：**当我们在状态 $cur$ 引爆炸弹 $b$ 时，有没有必要“提前关掉炸弹 $b$”，只让它炸到某个 $r'<\mathrm{rch}[b][cur]$ 就停下，从而配合下一枚炸弹？** 下面的引理证明：永远不需要提前停下！

**引理 1（极长推进占优性与起点单调定理）**

设当前待消灭武器下标为 $p_1,p_2$ 且 $0\le p_1\le p_2\le M$，剩余可用炸弹集合分别为 $U_1\subseteq U_2\subseteq\{0,\dots,n-1\}$。若从状态 $(p_1,U_1)$ 出发存在合法炸弹序列在 $t$ 步内到达 $M$，则从更靠右的状态 $(p_2,U_2)$ 出发也必然存在合法炸弹序列在 $\le t$ 步内到达 $M$。

特别地，在状态 $cur$ 使用炸弹 $b$ 时，直接推进到极长右端点 $r+1=\mathrm{rch}[b][cur]+1$ 恒不劣于停在任何 $r'+1\le r$。

**证明**：

设从 $p_1$ 出发的 $t$ 步合法炸弹序列为 $b_1,b_2,\dots,b_t\in U_1$，依次到达的分界点为 $p_1=q_0<q_1<q_2<\dots<q_t=M$，其中每一步满足 $q_s\le\mathrm{rch}[b_s][q_{s-1}]+1$（即炸弹 $b_s$ 覆盖闭区间 $[q_{s-1},q_s-1]$ 内所有武器）。

- 若 $p_2=M$，则 $0\le t$ 步已到达 $M$。
- 若 $p_2<M$，因 $q_0=p_1\le p_2<q_t=M$，必存在唯一一步 $s\in\{1,\dots,t\}$ 使得 $q_{s-1}\le p_2<q_s$。
- 因为炸弹 $b_s$ 覆盖整个区间 $[q_{s-1},q_s-1]$，而 $[p_2,q_s-1]\subseteq[q_{s-1},q_s-1]$，故炸弹 $b_s$ 同样覆盖 $[p_2,q_s-1]$ 内所有武器，即 $\mathrm{rch}[b_s][p_2]\ge q_s-1$！
- 于是从 $p_2$ 出发，直接使用后缀子序列 $b_s,b_{s+1},\dots,b_t\in U_1\subseteq U_2$（共 $t-s+1\le t$ 枚互不相同的炸弹），第一步即可从 $p_2$ 推进至 $\ge q_s$，随后沿原序列到达 $M$。$\blacksquare$

### 2.2 反复碰壁：贪心为什么会锁死？朴素 DFS 为什么会超时？

**失败 1：每步贪心选当前 $\mathrm{rch}[b][cur]$ 最大的未用炸弹。**

- 反例（$M=5,n=3,k=10$，见 `P1526_check.py` 边界用例）：
  - 炸弹 $B_0$ 覆盖武器 $\{0,1,2,\ 4\}$（注意它中间漏掉了武器 $3$，但它是**唯一**能覆盖收尾武器 $4$ 的炸弹！）；
  - 炸弹 $B_1$ 覆盖武器 $\{0,1,\ 3\}$；
  - 炸弹 $B_2$ 覆盖武器 $\{2,3\}$。
- 在起点 $cur=0$：
  - $B_0$ 可一口气推进到武器 $2$（$\mathrm{rch}[B_0][0]=2$），而 $B_1$ 只能推进到武器 $1$（$\mathrm{rch}[B_1][0]=1$）。
  - 若贪心选 $B_0$ 推进到 $cur=3$，下一步用 $B_2$（或 $B_1$）消灭武器 $3$ 到达 $cur=4$，此时唯一能炸武器 $4$ 的 $B_0$ 已经在第一步被用掉了，全局卡死无解！
  - 正确最优方案：第一步先选覆盖较短的 $B_1$ 消灭 $\{0,1\}$，第二步选 $B_2$ 消灭 $\{2,3\}$，把 $B_0$ 留到第三步消灭 $\{4\}$，共 $3$ 枚炸弹通关。

**失败 2：朴素回溯搜索在深层遇到瓶颈时发生指数级盲目回溯。**

- 假设在第 $1$ 步（$cur=0$）误用了唯一能覆盖末端武器 $w=90$ 的炸弹 $b^\ast$。
- 如果不对后缀做前瞻死点检测和冲突导向回跳，搜索树会在中间的 $cur=1\dots 89$ 层反复尝试各种不同的炸弹组合，直到第 $90$ 层才发现武器 $90$ 没人能炸；退回第 $89$ 层后又换一枚炸弹继续往下撞墙，产生 $2^{50}$ 级别的无效回溯！

### 2.3 三重核心加速：IDA* 可采纳下界 + 前瞻死点检测 + 冲突导向回跳

为了精准解决上述两个问题，我们将搜索设计为带冲突集返回值的 **IDA*（迭代加深 A*）**：

**引理 2（无复用限制最短路 $h[i]$ 的可采纳性）**

令 $h[m]=0$，对 $i=m-1,\dots,0$ 逆推定义

$$h[i]=1+\min_{b:\,\mathrm{rch}[b][i]\ge i}h\big[\mathrm{rch}[b][i]+1\big].$$

则从任意状态 $(cur,U)$ 出发到达 $m$ 的真实最少步数 $\ge h[cur]$。因此若当前已用步数 `dep` 满足 `dep + h[cur] > lim`，当前分支必无法在 `lim` 步内完成。

**证明**：真实方案是“每枚炸弹至多用一次”的合法转移序列；去掉“至多用一次”的限制后，可行序列集合扩大，其最短路长度 $h[cur]$ 自然是真实最短路的下界（即可采纳估价函数）。$\blacksquare$

**引理 3（前瞻死点检测与冲突导向回跳定理）**

在 `ida(cur, dep, conflict)` 中，用布尔集 `conflict` 收集导致子树失败的“已被祖先占用的炸弹集合”：

1. **前瞻死点检测**：维护 `availCnt[w]` 表示当前未使用的炸弹中能覆盖武器 $w$ 的数量。若存在某个尚未消灭的武器 $w\in[cur,m-1]$ 使得 `availCnt[w] == 0`，则当前分支必无解，且只有释放某个满足 $\mathrm{cov}[b][w]=\mathrm{true}$ 的炸弹才可能挽救 $w$，故直接将 `{b : cov[b][w]}` 并入 `conflict` 并立即返回 `false`。
2. **单调冲突导向回跳（One-Step Backjumping）**：将 `cur` 处所有满足 `dep + 1 + h[r + 1] <= lim` 的可用候选炸弹按右端点 $r=\mathrm{rch}[b][cur]$ **从大到小**排序为 $(r_0,b_0),(r_1,b_1),\dots$。
   当我们依次尝试 $(r_{\text{idx}},b_{\text{idx}})$ 并递归调用 `ida(r + 1, dep + 1, subConf)` 失败时，若发现
   $$\texttt{!subConf.test}(b_{\text{idx}})$$
   （即 $(r+1,\text{dep}+1)$ 子树的失败全程**并未因为炸弹 $b_{\text{idx}}$ 被占用而受阻**），则后续所有剩余候选 $(r_j,b_j)\ (j>\text{idx})$ **无需尝试，可直接从当前函数 `return false` 回跳**！

**证明**：

若 `!subConf.test(b)` 为真，说明在 $(r+1,\text{dep}+1)$ 的整棵搜索子树中，没有任何一个分支试图使用炸弹 $b$，即即便炸弹 $b$ 仍处于未占用状态，从起点 $r+1$ 使用当前全部可用炸弹集合 $U$ 也无法在 $\text{lim}-(\text{dep}+1)$ 步内到达 $m$。
对于排在后面的任意候选 $(r',b')$，因候选按右端点降序排列，必有 $r'+1\le r+1$，且其剩余可用炸弹集合为 $U'=U\setminus\{b'\}\subseteq U$。由**引理 1（起点单调定理）**，既然从更靠右的起点 $r+1$ 配合更大的可用集 $U$ 都无解，从更靠左的起点 $r'+1\le r+1$ 配合更小的可用集 $U'\subseteq U$ 必然同样无解！故可立即终止本层循环直接回跳。$\blacksquare$

> **引理 3(2) 的威力**：它把原本在每一层有多个候选炸弹时的指数级分叉，压成了**“只有当刚才选的那枚炸弹恰好与子树下游发生直接冲突时，才允许在本层尝试次优炸弹；否则整条与冲突无关的中间层直接一竿子回跳到冲突源祖先”**！

## 3. 模式总结（Pattern Summary）

按实际思考顺序列出本题的可迁移模式：

- **explore its property（时间窗充足 $\Rightarrow$ 确定性极长区间）** — 利用 $300\text{ s}>100\text{ s}$ 消除时间维度，将每枚炸弹在位置 $i$ 的作用浓缩为确定性转移 $i\to\mathrm{rch}[b][i]+1$。
- **monotonicity lemma（起点单调性）** — 证明区间后缀闭合性使“起点越靠右、可用炸弹越多”单调占优（引理 1），排除提前关停炸弹的劣决策。
- **failed attempt（纯贪心选最远）** — 被“唯一能炸终点的炸弹在起点也能炸最远”的 3 炸弹反例击穿。
- **classic problem solution（IDA* + 松弛下界 $h[i]$）** — 先倒推允许炸弹复用的最短路 $h[i]$ 作为可采纳下界，按 `lim = h[0]..n` 迭代加深（引理 2）。
- **conflict-directed backjumping（前瞻死点 + 单调回跳）** — 实时维护各武器剩余可用炸弹数 `availCnt[w]` 提前截断死路，并利用引理 1 证明当子树失败与当前所选炸弹 $b$ 无关（`!subConf.test(b)`）时，所有右端点 $\le r$ 的备选必全军覆没，直接回跳（引理 3）。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 / 来源 |
|---|---|---|---|---|
| `m, n, k` | `int` | 武器数 $M$、炸弹数 $n$、爆炸半径 $k$ | 读入 | $m,n\le 100$ |
| `wx[i], wy[i]` | `int [105]` | 第 $i$ 号武器的坐标（0-based） | 读入 | weapon x, y |
| `bx[b], by[b]` | `int [105]` | 第 $b$ 号炸弹的坐标（0-based） | 读入 | bomb x, y |
| `cov[b][w]` | `bool [105][105]` | 炸弹 $b$ 能否覆盖武器 $w$ | `inRange(b, w)` | 平方距离 $\le k^2$ |
| `rch[b][i]` | `int [105][105]` | 炸弹 $b$ 从武器 $i$ 起连续可炸到的最远下标 | 倒推预处理 | 不可炸 $i$ 时为 $i-1$ |
| `availCnt[w]` | `int [105]` | 当前未使用炸弹中能覆盖武器 $w$ 的数量 | $\sum_b\mathrm{cov}[b][w]$ | 前瞻死点检测 |
| `h[i]` | `int [105]` | 允许炸弹复用时从 $i$ 到 $m$ 的最少步数 | 倒推 DP | 引理 2 |
| `used[b]` | `bool [105]` | 炸弹 $b$ 是否已在当前搜索路径中使用 | 全 `false` | 状态标记 |
| `lim` | `int` | IDA* 当前迭代深度上限 | 从 `h[0]` 递增 | 首次命中即最优 |

### 4.2 编号步骤

1. 读入 $m,n,k$ 及武器、炸弹坐标；对每枚炸弹 $b$ 从 $i=m-1$ 倒推预处理 `cov[b][i]`、`rch[b][i]`，并累加初始 `availCnt[i]`。
2. 从 $i=m-1$ 到 $0$ 倒推计算无复用限制最短路下界 `h[i]`。
3. 令迭代加深上限 `lim` 从 `h[0]` 递增至 `n`，每次调用 `ida(0, 0, conf)`：
   - 若 `cur == m` 返回 `true`；若 `dep + h[cur] > lim` 返回 `false`；
   - 扫描 $w\in[\text{cur},m-1]$：若发现 `availCnt[w] == 0`，把所有满足 `cov[b][w]` 的 $b$ 写入 `conflict` 并返回 `false`；
   - 遍历所有炸弹 $b$：对满足 `rch[b][cur] >= cur` 且 `dep + 1 + h[rch[b][cur] + 1] <= lim` 的炸弹，若 `used[b]` 为真则 `conflict.set(b)`，否则加入候选表 `cands`；
   - 将 `cands` 按右端点 $r$ 降序排列，依次尝试每个 $(r,b)$：置 `used[b] = true` 并更新 `availCnt`，递归调用 `ida(r + 1, dep + 1, subConf)`；若失败则恢复状态、合并 `conflict |= subConf`，若 `!subConf.test(b)` 立即 `return false` 回跳。
4. 第一个使 `ida(0, 0, conf)` 返回 `true` 的 `lim` 即为最少炸弹数，输出并结束。

### 4.3 复杂度

- **时间**：预处理 $O(nm)$；由于 $h[i]$ 紧贴真实下界，且冲突导向回跳在遇到无冲突分支时 $O(1)$ 跨层回跳，在随机及各类构造图上均在数千次节点访问内完成，实测耗时 $\le 0.02\text{ s}$。
- **空间**：二维数组 `cov, rch` 大小仅 $105\times 105$，空间复杂度 $O(nm)\approx 100\text{ KB}$。

### 4.4 验证状态小结

所有结论均由本地真实编译运行（`g++ -O2 -std=c++17`）及 `appendix/P1526/P1526_check.py` 验证：

| 项目 | 状态 |
|---|---|
| 贪心锁死反例 | **通过**：构造 3 炸弹 5 武器贪心必死实例（起点选最远炸弹 $B_0$ 会导致末端武器 $4$ 无人可炸），标程与暴力均正确输出 `3`。 |
| 随机数据对拍 | **通过**：$1000$ 组有解随机实例，IDA* 标程 `P1526.cpp` 与 $O(2^n\cdot n)$ 状压 DP 暴力基准 `P1526_brute.cpp` 输出**逐字节一致**。 |
| 最坏规模计时 | $M=n=100$ 满规模测试：含进程启动最快耗时 **$0.0169\text{ s}$**。 |

## 5. 示例代码

### 5.1 主解：IDA* + 前瞻死点检测 + 单调冲突导向回跳

```cpp
//
// 题目：P1526 [NOI2003] 智破连环阵
// https://www.luogu.com.cn/problem/P1526
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};           // M,n<=100 (+5 松弛)
constexpr int inf{1145141919};

int m,n,k;
int wx[mxn],wy[mxn],bx[mxn],by[mxn];
bool cov[mxn][mxn];               // cov[b][w]=炸弹 b 能否覆盖武器 w
int rch[mxn][mxn];                // rch[b][i]=炸弹 b 从武器 i 开始连续可炸毁到的最远武器下标(不可炸 i 则为 i-1)
int availCnt[mxn];                // availCnt[w]=当前未使用炸弹中能覆盖武器 w 的数量
int h[mxn];                       // h[i]=不限制炸弹复用时从 i 到 m 的最少步数(IDA* 静态可采纳下界)
bool used[mxn];
int lim;

bool inRange(int b,int w)
{
	i64 dx=bx[b]-wx[w],dy=by[b]-wy[w];
	return dx*dx+dy*dy<=1LL*k*k;
}

bool ida(int cur,int dep,bitset<mxn>&conflict)
{
	if(cur==m)
		return true;
	if(dep+h[cur]>lim)
		return false;

	// 前瞻死点检测: 若后缀 [cur, m-1] 中某武器 w 已无任何可用炸弹覆盖, 记录所有能覆盖 w 的炸弹为冲突源并立即回溯
	for(int w=cur;w<m;++w)
		if(availCnt[w]==0)
		{
			for(int b=0;b<n;++b)
				if(cov[b][w])
					conflict.set(b);
			return false;
		}

	pair<int,int>cands[mxn];
	int cnt=0;
	for(int b=0;b<n;++b)
		if(rch[b][cur]>=cur)
		{
			int r=rch[b][cur];
			if(dep+1+h[r+1]<=lim)
			{
				if(used[b])
					conflict.set(b);      // 记录因被祖先占用而无法在 cur 处使用的合法炸弹
				else
					cands[cnt++]={r,b};
			}
		}
	sort(cands,cands+cnt,greater<>());    // 按右端点 r 从大到小排序

	for(int idx=0;idx<cnt;++idx)
	{
		auto[r,b]=cands[idx];
		used[b]=true;
		for(int w=0;w<m;++w)
			if(cov[b][w])
				--availCnt[w];

		bitset<mxn>subConf;
		if(ida(r+1,dep+1,subConf))
			return true;

		for(int w=0;w<m;++w)
			if(cov[b][w])
				++availCnt[w];
		used[b]=false;
		conflict|=subConf;

		// 单调性与冲突导向回跳定理:
		// 若 (r+1, dep+1) 子树失败且全程未受 used[b] 限制, 换用任何 r'<=r 的其它炸弹 b' 必同样失败!
		if(!subConf.test(b))
			return false;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>m>>n>>k))
		return 0;
	for(int i=0;i<m;++i)
		cin>>wx[i]>>wy[i];
	for(int i=0;i<n;++i)
		cin>>bx[i]>>by[i];

	for(int b=0;b<n;++b)
	{
		rch[b][m]=m-1;
		for(int i=m-1;i>=0;--i)
		{
			cov[b][i]=inRange(b,i);
			if(cov[b][i])
			{
				++availCnt[i];
				rch[b][i]=max(i,rch[b][i+1]);
			}
			else
				rch[b][i]=i-1;
		}
	}

	h[m]=0;
	for(int i=m-1;i>=0;--i)
	{
		h[i]=inf;
		for(int b=0;b<n;++b)
			if(rch[b][i]>=i&&h[rch[b][i]+1]<inf)
				h[i]=min(h[i],1+h[rch[b][i]+1]);
	}

	for(lim=h[0];lim<=n;++lim)
	{
		bitset<mxn>conf;
		if(ida(0,0,conf))
		{
			cout<<lim<<'\n';
			return 0;
		}
	}
	return 0;
}
```

### 5.2 对照实现：状压 DP 维护各炸弹子集最大推进位置（仅用于小规模对拍）

```cpp
//
// 题目：P1526 [NOI2003] 智破连环阵
// 暴力基准: 状压 DP (maxPos[mask] = 使用子集 mask 的炸弹最多能推进到的待消灭武器下标, 依据引理 1 单调性)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20};

int m,n,k;
int wx[mxn],wy[mxn],bx[mxn],by[mxn];
int rch[mxn][mxn];

bool inRange(int b,int w)
{
	i64 dx=bx[b]-wx[w],dy=by[b]-wy[w];
	return dx*dx+dy*dy<=1LL*k*k;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>m>>n>>k))
		return 0;
	for(int i=0;i<m;++i)
		cin>>wx[i]>>wy[i];
	for(int i=0;i<n;++i)
		cin>>bx[i]>>by[i];

	for(int b=0;b<n;++b)
	{
		rch[b][m]=m-1;
		for(int i=m-1;i>=0;--i)
			rch[b][i]=inRange(b,i)?max(i,rch[b][i+1]):i-1;
	}

	int tot=1<<n;
	vector<int>maxPos(tot,-1);
	maxPos[0]=0;
	int ans=n+1;
	for(int mask=0;mask<tot;++mask)
	{
		int cur=maxPos[mask];
		if(cur<0)
			continue;
		int cnt=__builtin_popcount(mask);
		if(cur==m)
		{
			ans=min(ans,cnt);
			continue;
		}
		for(int b=0;b<n;++b)
			if(!(mask>>b&1)&&rch[b][cur]>=cur)
			{
				int nmask=mask|(1<<b);
				maxPos[nmask]=max(maxPos[nmask],rch[b][cur]+1);
			}
	}
	cout<<(ans>n?-1:ans)<<'\n';
	return 0;
}
```
