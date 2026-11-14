# P2658 汽车拉力比赛 — 题解

## 一、解题过程：人类思考叙事

**题意重述。** 一个 $M\times N$ 的网格（$1\le M,N\le500$），每格海拔 $h\in[0,10^9]$。某些格子是路标。要找一个最小的难度系数 $D$，使得**任意两个路标**之间都存在一条路径，路径上**相邻两格的海拔差绝对值都 $\le D$**（上下左右四连通）。输出这个最小的 $D$。

**先抠一个容易读反的输入格式。** 题面写的是"第 $1$ 行两个整数 $M$ 和 $N$，第 $2$ 到 $M+1$ 行每行 $N$ 个整数"。所以 **$M$ 是行数、$N$ 是列数**，先读 `m` 再读 `n`，网格是 `h[m][n]`。很多人习惯"n 行 m 列"，这里反了，读反会直接 WA。

**第一步：把"任意两个路标可达"化简。** 字面要求是"所有路标两两可达"，听起来要检查 $\binom{k}{2}$ 对。但固定 $D$ 后，把"相邻且海拔差 $\le D$"的格子连边，得到一张无向图 $G_D$。在这张图里"可达"是一个**等价关系**：

- 自反：每个格子到自己有平凡路径；
- 对称：海拔差是绝对值，$|h_u-h_v|=|h_v-h_u|$，所以边是无向的，$u$ 能到 $v$ 则 $v$ 能到 $u$；
- 传递：$u\to v$ 的路径接上 $v\to w$ 的路径就是 $u\to w$ 的路径。

于是"所有路标两两可达"**等价于**"所有路标落在 $G_D$ 的**同一个连通块**里"。这就把 $\binom{k}{2}$ 对检查塌缩成**一次 BFS**：从任意一个路标出发，只走差 $\le D$ 的边，看能不能把所有路标都访问到。

**第二步：发现单调性 ⇒ 二分答案。** 设 $\mathrm{ok}(D)$ 表示"难度 $D$ 能让所有路标连通"。如果 $D$ 可行，那么 $D+1$ 一定也可行——因为放宽阈值只会**增加**可用边（$|diff|\le D\Rightarrow|diff|\le D+1$），连通块只会合并不会分裂。所以 $\mathrm{ok}$ 关于 $D$ 单调：存在一个临界值 $D^\*$，$D<D^\*$ 全 false，$D\ge D^\*$ 全 true。这正是**二分答案**的标准形态：在 $[0,10^9]$ 上二分最小的可行 $D$。

为什么上界取 $10^9$？海拔都在 $[0,10^9]$，任意相邻差 $\le10^9$，所以 $D=10^9$ 时**所有边**都可用，网格本身四连通，必然连通所有路标，$\mathrm{ok}(10^9)=\text{true}$。下界 $0$：若所有路标本就同海拔或只有一个路标，$D=0$ 即可。

**第三步：$\mathrm{check}(D)$ 怎么写。** 从第一个路标 $(sx,sy)$ 出发 BFS，扩展时只接受 $|h_{nx,ny}-h_{x,y}|\le D$ 的邻居；每访问到一个路标就计数。最后 `cnt==tot` 即可行。特判 `tot<=1`（零个或一个路标）直接返回 true，答案是 $0$。

**第四步：换个视角——这其实是"瓶颈路 / 最小生成树"。** 二分 + BFS 是对的，但还有一个更漂亮的等价刻画：把所有相邻边按海拔差**从小到大排序**，用并查集依次加入，直到所有路标连通，**最后加入那条边的权值就是 $D^\*$**。理由：$D^\*$ 是"让路标连通所需的最小瓶颈"，而 Kruskal 加边到连通的那一刻，恰好是用最小可能的最大边把路标串起来——这就是最小生成树上路标间路径的最大边权（minimax 路径）。这个视角把"二分 + 每次 BFS"换成"一次排序 + 并查集"，复杂度 $O(E\log E)$，$E\approx2MN$。两种做法都对，本文实现更直观的二分 + BFS，并在第二部分证明二者等价。

**手造数据核对**（题面无样例，我按定义手算了几组）：

- $2\times2$，海拔 $\begin{smallmatrix}1&3\\5&9\end{smallmatrix}$，路标在 $(0,0)$ 与 $(1,1)$。两条路径：$1\to3\to9$ 的差是 $2,6$，瓶颈 $6$；$1\to5\to9$ 的差是 $4,4$，瓶颈 $4$。取较小瓶颈 $D=4$。
- $2\times3$，海拔 $\begin{smallmatrix}1&2&3\\4&5&6\end{smallmatrix}$，路标 $(0,0),(1,2)$：沿上边再下到底差为 $1,1,3$，瓶颈 $3$；$D=3$。
- $1\times2$，海拔 $1,8$，两格都是路标：唯一边差 $7$，$D=7$。
- 单格单路标 / 单格无路标：$D=0$。

程序输出与手算逐条吻合；另用 Floyd 求所有点对 minimax 路径的独立暴力在 $400$ 组随机小网格（$\le4\times4$，海拔 $0\sim20$，路标随机）上对拍，$0$ mismatch。

---

## 二、思维链：完整证明

**记号。** 网格格子集 $V=\{(i,j):0\le i<M,0\le j<N\}$，$|V|=MN$。海拔 $h:V\to[0,10^9]$。路标集 $L\subseteq V$，$k=|L|$。对阈值 $D\ge0$，定义无向图 $G_D=(V,E_D)$，其中 $\{u,v\}\in E_D$ 当且仅当 $u,v$ 四连通相邻且 $|h_u-h_v|\le D$。记 $u\sim_D v$ 为 $u,v$ 在 $G_D$ 中连通（存在路径）。$\mathrm{ok}(D)\iff\forall p,q\in L,\ p\sim_D q$。目标 $D^\*=\min\{D\ge0:\mathrm{ok}(D)\}$。

---

### 引理 1（$\sim_D$ 是等价关系）

对固定 $D$，关系 $\sim_D$ 在 $V$ 上自反、对称、传递。

*证明*：
- **自反**：对任意 $u$，长度 $0$ 的平凡路径给出 $u\sim_D u$。
- **对称**：$G_D$ 的边定义为无向（$|h_u-h_v|=|h_v-h_u|$，条件对称），故若存在路径 $u=v_0,v_1,\dots,v_t=v$ 每步是 $E_D$ 的边，则反向 $v=v_t,\dots,v_0=u$ 也是合法路径，$v\sim_D u$。
- **传递**：若 $u\sim_D v$、$v\sim_D w$，取路径 $P:u\to v$、$Q:v\to w$，拼接 $P\cdot Q$ 是 $u\to w$ 的走道，每步仍是 $E_D$ 的边（走道可含重复点，但连通性只要求存在走道，等价于存在简单路径），故 $u\sim_D w$。

因此 $\sim_D$ 把 $V$ 划分成若干连通块（等价类）。证毕。

---

### 引理 2（两两可达 ⟺ 单连通块 ⟺ 一次 BFS 全覆盖）

设 $L\ne\varnothing$，任取 $s\in L$。则
$$\mathrm{ok}(D)\iff L\text{ 全部落在 }s\text{ 所在的 }G_D\text{ 连通块}\iff \text{从 }s\text{ 出发 BFS 访问到的路标数}=k.$$

*证明*：
($\Rightarrow$) 若 $\mathrm{ok}(D)$，则对任意 $p\in L$ 有 $p\sim_D s$，即 $p$ 与 $s$ 同连通块，故 $L$ 全在 $s$ 的块里。
($\Leftarrow$) 若 $L$ 全在 $s$ 的块里，则任意 $p,q\in L$ 都与 $s$ 连通，由引理 1 的对称与传递，$p\sim_D q$，故 $\mathrm{ok}(D)$。

第二个等价：BFS 从 $s$ 出发恰好遍历 $s$ 所在的整个连通块（引理 6 保证 BFS 正确），访问到的路标数 $=|L\cap(\text{s 的块})|$。它等于 $k=|L|$ 当且仅当 $L\subseteq s$ 的块。证毕。

**这条引理把 $\binom{k}{2}$ 对检查降为一次 BFS**，是本题第一个关键化简。$L=\varnothing$（$k=0$）时 $\mathrm{ok}(D)$ 空真，约定 $D^\*=0$；$k=1$ 时单点自反连通，$D^\*=0$。代码用 `tot<=1` 特判。

---

### 引理 3（单调性）

若 $0\le D\le D'$，则 $E_D\subseteq E_{D'}$，从而 $\mathrm{ok}(D)\Rightarrow\mathrm{ok}(D')$。

*证明*：$\{u,v\}\in E_D\Rightarrow|h_u-h_v|\le D\le D'\Rightarrow\{u,v\}\in E_{D'}$，故 $E_D\subseteq E_{D'}$。$G_D$ 的任意路径也是 $G_{D'}$ 的路径（边都在），所以 $u\sim_D v\Rightarrow u\sim_{D'} v$。于是 $\forall p,q\in L,p\sim_D q\Rightarrow\forall p,q\in L,p\sim_{D'} q$，即 $\mathrm{ok}(D)\Rightarrow\mathrm{ok}(D')$。证毕。

**推论**：$\mathrm{ok}$ 是 $[0,\infty)$ 上的单调布尔函数，存在临界 $D^\*$ 使 $D<D^\*$ 时 false、$D\ge D^\*$ 时 true（$D^\*$ 可能为 $0$）。这保证二分答案合法。

---

### 引理 4（临界值存在且落在 $[0,10^9]$）

$D^\*$ 存在、唯一，且 $0\le D^\*\le10^9$；$\mathrm{ok}(10^9)=\text{true}$。

*证明*：海拔 $h\in[0,10^9]$，故任意相邻 $u,v$ 有 $|h_u-h_v|\le10^9$，即 $E_{10^9}$ 包含**所有**四连通相邻边。网格图在全部相邻边下连通（任意两格可走折线到达），故所有路标连通，$\mathrm{ok}(10^9)=\text{true}$。集合 $\{D\ge0:\mathrm{ok}(D)\}$ 非空（含 $10^9$）且有下界 $0$，由单调性（引理 3）它是一个形如 $[D^\*,\infty)$ 的区间，$D^\*=\min$ 存在唯一。又 $\mathrm{ok}(10^9)$ 真故 $D^\*\le10^9$；$D^\*\ge0$ 显然。

$D^\*$ 一定是某条相邻边的海拔差（或 $0$）：因为 $\mathrm{ok}$ 只在"某个 $|h_u-h_v|$ 被跨过"时才可能从 false 变 true，而这些差是有限个整数，故 $D^\*$ 取在其中之一（$k\le1$ 时 $D^\*=0$）。这保证二分整数区间能精确命中。证毕。

---

### 引理 5（二分答案的正确性）

代码 `l=0,r=1e9; while(l<r){mid=(l+r)>>1; if(check(mid))r=mid; else l=mid+1;}` 结束时 `l=r=D*`。

*证明*：维护不变量"$D^\*\in[l,r]$"。初始 $l=0,r=10^9$，由引理 4 成立。每轮 $mid=\lfloor(l+r)/2\rfloor$，$l\le mid<r$（因 $l<r$）：
- 若 $\mathrm{ok}(mid)$ 真：由单调性 $D^\*\le mid$，结合不变量 $D^\*\ge l$，得 $D^\*\in[l,mid]$，令 $r=mid$ 保持。
- 若 $\mathrm{ok}(mid)$ 假：则 $D^\*>mid$（否则 $D^\*\le mid$ 会由单调性推出 $\mathrm{ok}(mid)$ 真），结合 $D^\*\le r$，得 $D^\*\in[mid+1,r]$，令 $l=mid+1$ 保持。

每轮区间长度 $r-l$ 严格减小（$mid<r$ 保证 $r=mid$ 缩短；$mid\ge l$ 保证 $l=mid+1>l$ 缩短），故有限步终止于 $l=r$。由不变量 $D^\*\in[l,l]$，即 $l=r=D^\*$。证毕。

---

### 引理 6（BFS 正确性与队列规模）

`check(D)` 中的 BFS 从 $s$ 出发，恰好访问 $s$ 在 $G_D$ 的连通块，每个格子至多入队一次，队列长度 $\le MN$。

*证明*：标准 BFS 不变量：`vis[u]=true` 当且仅当 $u$ 已被发现并入队。初始只 $s$ 入队。每次取出队首 $u$，枚举其四邻居 $v$，若 $v$ 未访问且 $\{u,v\}\in E_D$（即 $|h_v-h_u|\le D$ 且在界内）则标记并入队。

- **只访问块内**：归纳可证每个入队的点都通过 $E_D$ 边与 $s$ 相连，故都在 $s$ 的块内。
- **块内全访问**：设 $v$ 在 $s$ 的块内，取最短路径 $s=v_0,\dots,v_t=v$。对 $t$ 归纳：$v_0=s$ 入队；若 $v_{i}$ 入队，则它出队时会检查邻居 $v_{i+1}$（$\{v_i,v_{i+1}\}\in E_D$），若未访问则入队。故 $v_t=v$ 终会入队。
- **至多一次**：`vis` 守卫使每点只在首次发现时入队。
- **队列规模**：每点至多入队一次，总入队 $\le MN$，`qx/qy` 开到 `mxn*mxn` 足够。

计数 `cnt` 在每发现一个路标时 $+1$，故 BFS 结束 `cnt`$=|L\cap s\text{ 的块}|$，由引理 2 返回 `cnt==tot` 即 $\mathrm{ok}(D)$。证毕。

---

### 引理 7（Kruskal / 瓶颈路等价刻画）

设把所有相邻边按权 $w=|h_u-h_v|$ 升序排列，用并查集依次加入，$D_{\mathrm{kruskal}}$ 为"所有路标首次连通"时最后加入边的权（$k\le1$ 时为 $0$）。则 $D_{\mathrm{kruskal}}=D^\*$。

*证明*：
($D^\*\le D_{\mathrm{kruskal}}$)：加入权 $\le D_{\mathrm{kruskal}}$ 的所有边后路标连通，而这些边都属于 $E_{D_{\mathrm{kruskal}}}$，故 $\mathrm{ok}(D_{\mathrm{kruskal}})$ 真，由 $D^\*$ 的最小性 $D^\*\le D_{\mathrm{kruskal}}$。

($D_{\mathrm{kruskal}}\le D^\*$)：反证。设 $D_{\mathrm{kruskal}}>D^\*$。考虑只用权 $\le D^\*$ 的边（即 $E_{D^\*}$），由 $\mathrm{ok}(D^\*)$ 真，路标在 $E_{D^\*}$ 下连通。Kruskal 按升序加边，加完所有权 $\le D^\*$ 的边时，路标已连通（因为这些边就是 $E_{D^\*}$，足以连通），故"首次连通"发生在权 $\le D^\*$ 处，即 $D_{\mathrm{kruskal}}\le D^\*$，矛盾。

故相等。证毕。

**意义**：$D^\*$ 就是最小生成树上连接各路标的路径的最大边权（minimax 瓶颈）。这给出第二种实现：排序 $O(E\log E)$ + 并查集 $O(E\alpha)$，$E\approx2MN$。与二分 + BFS 的 $O(\log(10^9)\cdot MN)$ 同量级，二者皆可。

---

### 引理 8（复杂度与值域）

二分 + BFS：时间 $O(\log(10^9)\cdot MN)$，空间 $O(MN)$。所有量用 `int` 安全。

*证明*：二分 $O(\log_2 10^9)\approx30$ 轮，每轮一次 BFS $O(MN)$（每格至多入队一次、检查 $4$ 邻居），故时间 $O(30\cdot MN)$。$M,N\le500$，$MN\le2.5\times10^5$，约 $7.5\times10^6$ 次出队、$3\times10^7$ 次邻居检查，C++ 下远低于时限。空间：`h,mk,vis` 各 $505^2$，队列 `qx,qy` 各 $505^2$，合计 $O(MN)$，约几 MB。

值域：$h\in[0,10^9]$，差 $h_u-h_v\in[-10^9,10^9]$，`abs` 后 $\le10^9<2^{31}$，`int` 安全；$D\in[0,10^9]$ 亦 `int` 安全。证毕。

---

### 定理

程序输出的 `l` 等于 $D^\*$，即让所有路标相互可达的最小难度系数；复杂度 $O(\log(10^9)\cdot MN)$。

*证明*：引理 1、2 把"两两可达"化为"一次 BFS 全覆盖"；引理 3、4 保证单调性与临界值落在 $[0,10^9]$；引理 5 保证二分命中 $D^\*$；引理 6 保证 `check` 的 BFS 正确；引理 8 给出复杂度与值域。引理 7 另证 Kruskal 视角等价。串联即得。证毕。

---

## 三、模式总结（按实际思考顺序）

- **explore its property** — 固定 $D$ 后"差 $\le D$ 相邻连边"，可达是等价关系（绝对差对称 + 路径传递）。
- **reduce to easier problem** — "所有路标两两可达" ⟺ "全在同一连通块" ⟺ 从任一路标一次 BFS 数够 `tot`，把 $\binom{k}{2}$ 对降为一次遍历。
- **explore its property** — 放宽 $D$ 只增边不删边 ⇒ $\mathrm{ok}(D)$ 单调，存在临界 $D^\*$。
- **classic problem solution** — 二分答案：$[0,10^9]$ 上二分最小可行 $D$，`check` 用 BFS 判连通。
- **reduce to easier problem** — 另一视角：$D^\*$ = 最小生成树上路标间路径的最大边权（瓶颈路），Kruskal 排序加边到连通即得。
- **discuss by category** — 边界：`tot<=1` 答案 $0$；输入是 $M$ 行 $N$ 列（先 `m` 后 `n`）；上界 $10^9$ 必然可行。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|------|------|-------------|--------|
| `m,n` | 行数 $M$、列数 $N$（$\le500$） | `int`，读入 | **先 m 后 n**，`h[m][n]` |
| `h[i][j]` | 格子海拔 $\in[0,10^9]$ | `int[mxn][mxn]`，读入 | 差用 `abs` |
| `mk[i][j]` | 是否路标（$0/1$） | `bool[mxn][mxn]`，读入 | 第二个 $M\times N$ 矩阵 |
| `tot` | 路标总数 $k$ | `int`，初值 $0$ | `tot<=1` ⇒ 答案 $0$ |
| `sx,sy` | 第一个路标坐标（BFS 源） | `int` | 读入时记录 |
| `vis[i][j]` | BFS 访问标记 | `bool[mxn][mxn]` | 每次 `check` 前 `memset` 清零 |
| `qx,qy` | BFS 队列（坐标） | `int[mxn*mxn]` | 每点至多入队一次 |
| `dx,dy` | 四方向偏移 | `int[4]` | 上下左右 |
| `D` | 当前二分的难度阈值 | `int` | `check(D)` 的边过滤条件 |
| `l,r` | 二分区间 $[0,10^9]$ | `int` | 收敛到 $D^\*$ |
| `mxn` | $500+5=505$ | `constexpr int` | 网格尺寸 $+5$ 松弛 |

### 算法步骤

1. I/O 加速三行；读 `m,n`（先 $M$ 后 $N$）。
2. 读海拔 `h[0..m-1][0..n-1]`；读路标 `mk`，统计 `tot`，记录第一个路标 `(sx,sy)`。
3. 二分 `l=0,r=1000000000`：`mid=(l+r)>>1`，`check(mid)` 真则 `r=mid` 否则 `l=mid+1`，直到 `l==r`。
4. `check(D)`：`tot<=1` 返回 true；`memset(vis,0)`；从 `(sx,sy)` BFS，只走 `abs(h差)<=D` 的界内未访问邻居，发现路标 `++cnt`；返回 `cnt==tot`。
5. 输出 `l` 与 `'\n'`。

### 复杂度

- 时间：$O(\log(10^9)\cdot MN)\approx30\times2.5\times10^5=7.5\times10^6$ 次出队（约 $3\times10^7$ 次邻居检查）。
- 空间：$O(MN)$，`h/mk/vis/qx/qy` 各 $505^2$，约几 MB。
- 值域：海拔差与 $D$ 均 $\le10^9$，`int` 安全。

### 验证状态

`g++ -std=c++20 -O2` 编译通过；题面无样例，手算 $2\times2$（瓶颈 $4$）、$2\times3$（瓶颈 $3$）、$1\times2$（差 $7$）、单路标 / 无路标（$0$）全部吻合；另与 Floyd 求所有点对 minimax 路径的独立暴力在 $400$ 组随机小网格（$\le4\times4$，海拔 $0\sim20$，路标随机含 $0/1$ 个）对拍，$0$ mismatch。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2658 汽车拉力比赛
// 思路：固定 D 后"差<=D 相邻连边"，可达是等价关系 ⇒ 所有路标两两可达 ⟺ 同一连通块。
//       ok(D) 单调（放宽只增边）⇒ 二分最小可行 D，check 用一次 BFS 数路标。
//       等价视角：D* = 最小生成树上路标间路径的最大边权（Kruskal 瓶颈）。
//       注意输入是 M 行 N 列（先 m 后 n）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};

int n,m,h[mxn][mxn],tot,sx,sy;
bool mk[mxn][mxn],vis[mxn][mxn];
int qx[mxn*mxn],qy[mxn*mxn];
int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};

bool check(int D)
{
	if(tot<=1)
		return true;	// 零个或一个路标，D=0 即可
	memset(vis,0,sizeof vis);
	int head=0,tail=0,cnt=0;
	qx[tail]=sx;qy[tail++]=sy;
	vis[sx][sy]=true;
	if(mk[sx][sy])
		++cnt;
	while(head<tail)
	{
		int x=qx[head],y=qy[head++];
		for(int d=0;d<4;++d)
		{
			int nx=x+dx[d],ny=y+dy[d];
			if(nx<0||nx>=m||ny<0||ny>=n||vis[nx][ny])
				continue;
			if(abs(h[nx][ny]-h[x][y])>D)
				continue;	// 海拔差超过阈值，这条边不可用
			vis[nx][ny]=true;
			if(mk[nx][ny])
				++cnt;
			qx[tail]=nx;qy[tail++]=ny;
		}
	}
	return cnt==tot;	// 所有路标都在源点连通块里
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>m>>n;	// 题面：第一行 M N，M 行 N 列
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
			cin>>h[i][j];
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			cin>>mk[i][j];
			if(mk[i][j])
			{
				++tot;
				if(tot==1)
					sx=i,sy=j;
			}
		}
	int l=0,r=1000000000;	// 海拔差上界 1e9，check(1e9) 必真
	while(l<r)
	{
		int mid=(l+r)>>1;
		if(check(mid))
			r=mid;
		else
			l=mid+1;
	}
	cout<<l<<'\n';
	return 0;
}
```

### 代码说明

- **先 `m` 后 `n`**：题面第一行是 $M$（行）$N$（列），`h[m][n]`，BFS 边界 `nx<m`、`ny<n`。读反会 WA。
- **`tot<=1` 特判**：零个路标空真、一个路标自反连通，答案都是 $0$，避免无源点 BFS。
- **`check` 里 `memset(vis,0,sizeof vis)`**：每轮二分都要重置访问标记；`vis` 是 `bool` 全局数组，`sizeof` 即 $505^2$ 字节，$30$ 轮共约 $7.6$ MB 写入，可忽略。
- **`abs(h[nx][ny]-h[x][y])>D` 过滤边**：差在 $[-10^9,10^9]$，`int` 安全；这是 $G_D$ 的边条件。
- **二分 `l=0,r=1e9`**：`check(1e9)` 必真（全边可用、网格连通），不变量 $D^\*\in[l,r]$ 收敛到答案（引理 5）。
- 队列 `qx/qy` 开 `mxn*mxn`，每点至多入队一次，不会溢出；`'\n'` 换行；下标内部 $0$-based。