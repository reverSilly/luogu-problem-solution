# P3094 [USACO13DEC] Vacation Planning S — 题解

## 一、解题过程：人类思考叙事

**题意重述（剥离背景）。** $N\le 200$ 个点、$M\le 10^4$ 条**有向**带权边（边权 $1\le d\le 10^6$），前 $K\le 100$ 个点（编号 $1\dots K$）是枢纽。$Q\le 10^4$ 个询问 $(a,b)$：求从 $a$ 到 $b$、**途中至少经过一个枢纽**（起点或终点本身是枢纽也算）的最小费用；航线允许重复经过点。输出两行：可行询问数、所有可行询问的最小费用之和。

**口径声明。** 题面明说"可能会多次经过某些农场"，所以合法路线是**游走（walk）**而非简单路径——这一点是解法成立的命门，下面会看到。询问未禁止 $a=b$：若 $a$ 是枢纽，长度为 $0$ 的平凡游走就"经过"了枢纽 $a$，费用 $0$；若 $a$ 不是枢纽，则需要一条经过某枢纽再回到 $a$ 的回路。我按此口径实现（这也与"可以是起点或者终点农场"的字面意思一致）。

**手造数据核对。** 题面 Hint 给了完整例子，整理成输入：

```
3 3 1 3
3 1 10
1 3 10
1 2 7
3 2
2 3
1 2
```

枢纽只有 $1$。$3\to2$：必须经 $1$，$3\to1\to2=10+7=17$；$2\to3$：$2$ 没有出边，不可行；$1\to2$：起点即枢纽，直飞 $7$。输出 `2` 与 `24`，与 Hint 一致。

再造四组专打边界：

- 重边：`2 2 1 1 / 1 2 5 / 1 2 3 / 1 2` → 应取便宜的重边，输出 `1`、`3`。
- 起/终点是枢纽：`3 2 2 3 / 2 3 4 / 3 1 6 / 2 3 / 3 1 / 1 2` → 枢纽 $\{1,2\}$；$2\to3$ 起点即枢纽费用 $4$，$3\to1$ 终点即枢纽费用 $6$，$1\to2$ 无出边不可行。输出 `2`、`10`。
- $a=b$ 且是枢纽：`1 1 1 1 / 1 1 5 / 1 1` → 平凡游走费用 $0$，输出 `1`、`0`（自环 $5$ 不该被选中）。
- $a=b$ 非枢纽：`2 2 1 1 / 2 1 3 / 1 2 4 / 2 2` → 必须绕枢纽 $1$ 一圈，$3+4=7$，输出 `1`、`7`。

**第一步：暴力为什么不行。** 每个询问跑一遍"带状态的最短路"（状态 = 是否已访问枢纽，拆点分层图 Dijkstra），单次 $O(M\log N)$，总量 $Q\cdot M\log N\approx 10^4\times10^4\times8=8\times10^8$，超时。瓶颈在于**每个询问都从头搜图**，而图只有 $200$ 个点——这个规模在暗示预处理**全源最短路**。

**第二步：抓结构性质——"经过枢纽"可以拆成两段。** 一条合法游走 $a\rightsquigarrow b$ 必定在某时刻踩到某个枢纽 $h$。把它在 $h$ 处剪开：前段是 $a\to h$ 的游走，后段是 $h\to b$ 的游走，两段费用各自 $\ge$ 对应最短路 $\mathrm{dis}[a][h]$、$\mathrm{dis}[h][b]$。反过来，把两条最短路**首尾拼接**就得到一条经过 $h$ 的合法游走——这里"允许重复经过点"是决定性条件：两段最短路可能共享中间点，拼接后不是简单路径，但题目不在乎！于是

$$\mathrm{ans}(a,b)=\min_{h\in\text{枢纽}}\bigl(\mathrm{dis}[a][h]+\mathrm{dis}[h][b]\bigr).$$

若题目要求简单路径，这个分解就**错了**（两段可能相交），必须另想办法；本题的"可重复"把难点直接抹平。

**第三步：归约到经典问题。** 上式只需要全源最短路。$N\le200$，Floyd 三重循环 $N^3=8\times10^6$，比 $N$ 次 Dijkstra 还好写；询问端 $Q\cdot K=10^6$ 次加法取 min。总量 $\approx 9\times10^6$，轻松。

**第四步：四个会静默出错的坑。**

1. **重边**：$u\to v$ 可能给多条，读入时取 min，否则后读的贵边覆盖便宜边。
2. **`int` 的 INF 加法溢出**：Floyd 里 `dis[i][t]+dis[t][j]` 可能是 INF+INF。若 INF 取 `1e9`（`int`），和 $2\times10^9$ 已经贴着 `INT_MAX`；取 `1145141919` 则和 $\approx2.29\times10^9>$ `INT_MAX`，**溢出成负数**，Floyd 会凭空造出"负权最短路"。干脆全程 `i64`，哨兵 `1145141919810`，和 $\approx2.3\times10^{12}$ 毫无压力。
3. **总费用必须 `i64`**：单询问费用 $\le 2(N-1)\cdot10^6\approx4\times10^8$，$Q$ 个加起来 $\approx4\times10^{12}$，`int` 装不下。
4. **全局数组默认 $0$ 不是 INF**：`dis` 必须显式 `fill` 成 INF，再置 `dis[i][i]=0`；否则所有点对初始"距离 0"，答案全错。

顺带：`dis[i][i]=0` 让"起点/终点本身是枢纽"自动被覆盖——取 $h=a$ 得 $0+\mathrm{dis}[a][b]$，不需要特判；正权自环永远不如不走，读入时 `min` 也不会破坏 $0$。

---

## 二、思维链：完整证明

**记号。** 点集 $V=\{0,\dots,N-1\}$（内部 0-based），枢纽集 $\mathcal H=\{0,\dots,K-1\}$。边权 $w(e)\in[1,10^6]$。$\mathrm{dis}[u][v]$ 为 $u\to v$ 的最短游走费用（不可达为 $+\infty$，实现中为哨兵 `inf`）。$\mathrm{ans}(a,b)$ 为"经过至少一个枢纽的 $a\to b$ 游走"的最小费用。

---

### 引理 1（正权 ⇒ 最优游走是简单路径，最短路良定义）

对任意 $u,v$，若存在 $u\to v$ 的游走，则存在费用最小的游走，且其中费用最小者可取为**简单路径**（点不重复）；从而 $\mathrm{dis}[u][v]$ 等于简单路径意义下的最短路，且 $\mathrm{dis}[u][v]\le(N-1)\cdot10^6$。$\mathrm{dis}[u][u]=0$。

*证明*：任取一条 $u\to v$ 游走 $W$。若 $W$ 重复经过某点 $x$，则 $W$ 含一个从 $x$ 回到 $x$ 的闭子游走 $C$；$C$ 至少含一条边，边权 $\ge1$，故 $w(C)\ge1>0$。从 $W$ 中删去 $C$ 得到仍合法的 $u\to v$ 游走，费用严格减少。每次删环点数不减、边数严格减少，有限步后得到简单路径 $P$，$w(P)\le w(W)$。故最小值在简单路径中取得（简单路径只有有限条），$\mathrm{dis}$ 良定义。简单路径至多 $N-1$ 条边，费用和 $\le(N-1)\cdot10^6<2\times10^8$。$u=u$ 时长度 $0$ 的平凡游走费用 $0$，且任何非平凡游走费用 $\ge1>0$，故 $\mathrm{dis}[u][u]=0$。证毕。

---

### 引理 2（Floyd 不变量）

设外层循环变量 $t$ 从 $0$ 到 $N-1$。第 $t$ 轮结束后，对所有 $i,j$：

$$\mathrm{dis}[i][j]=\min\{w(P):P\text{ 是 }i\to j\text{ 的游走，内部点}\subseteq\{0,\dots,t\}\}\quad(\text{无则 }+\infty).$$

（"内部点"指除端点 $i,j$ 外经过的点。）

*证明*：对 $t$ 归纳。初始（$t=-1$，未进循环）：$\mathrm{dis}[i][j]$ 为直接边最小权（重边取 min，引理 4），$i=j$ 为 $0$，恰是"内部点集为空"的游走（单条边或平凡游走）的最小费用 ✓。

设第 $t-1$ 轮后不变量成立，考察第 $t$ 轮 `dis[i][j]=min(dis[i][j],dis[i][t]+dis[t][j])`。记 $S_t$ 为内部点 $\subseteq\{0,\dots,t\}$ 的 $i\to j$ 游走集合，$S_{t-1}$ 类似。任一 $P\in S_t$：

- 若 $P$ 不经过 $t$（作为内部点），则 $P\in S_{t-1}$，$w(P)\ge$ 旧 $\mathrm{dis}[i][j]$。
- 若 $P$ 经过 $t$：由引理 1 的删环论证，可设 $P$ 费用最小且**不重复经过 $t$**（重复则删去两次经过 $t$ 之间的闭子游走，费用严格降且仍属 $S_t$）。在 $t$ 处剪开：$P_1:i\to t$、$P_2:t\to j$，二者内部点 $\subseteq\{0,\dots,t-1\}$（$t$ 只出现一次且是端点；$i$ 或 $j$ 等于 $t$ 时对应段为平凡游走，费用 $0$，同样成立）。由归纳假设 $w(P_1)\ge\mathrm{dis}[i][t]$、$w(P_2)\ge\mathrm{dis}[t][j]$（注意本轮更新 $\mathrm{dis}[i][j]$ 时读的 $\mathrm{dis}[i][t]$、$\mathrm{dis}[t][j]$：$\mathrm{dis}[i][t]$ 在本轮至多被 `min(dis[i][t],dis[i][t]+dis[t][t])` 更新，而 $\mathrm{dis}[t][t]=0$，值不变；$\mathrm{dis}[t][j]$ 同理不变——故读到的仍是第 $t-1$ 轮的值，归纳假设可用）。于是 $w(P)\ge\mathrm{dis}[i][t]+\mathrm{dis}[t][j]$。

反过来，旧 $\mathrm{dis}[i][j]$ 对应的游走属于 $S_t$；$\mathrm{dis}[i][t]+\mathrm{dis}[t][j]$ 对应两条游走拼接，内部点 $\subseteq\{0,\dots,t\}$，属于 $S_t$（拼接允许重复点，游走定义允许）。故更新后的 $\mathrm{dis}[i][j]$ 恰为 $\min_{P\in S_t}w(P)$。$t=N-1$ 时内部点无限制，由引理 1 即全源最短路。证毕。

---

### 引理 3（询问分解公式）

$$\mathrm{ans}(a,b)=\min_{h\in\mathcal H}\bigl(\mathrm{dis}[a][h]+\mathrm{dis}[h][b]\bigr),$$

其中 $+\infty$ 参与取 min 的规则是"任何有限值优先"；右边为 $+\infty$ 当且仅当询问不可行。特别地，$a\in\mathcal H$ 时取 $h=a$ 得 $\mathrm{dis}[a][b]$，$b\in\mathcal H$ 时取 $h=b$ 同样，端点为枢纽无需特判。

*证明*：**下界**（$\mathrm{ans}\ge$ 右式）：设 $W$ 是任一合法游走，它经过某枢纽 $h$。在 $W$ **第一次**到达 $h$ 处剪开为 $W_1:a\to h$ 与 $W_2:h\to b$（$h$ 首次出现，$W_1$ 内部不含 $h$，但这对下界无影响）。$w(W)=w(W_1)+w(W_2)\ge\mathrm{dis}[a][h]+\mathrm{dis}[h][b]\ge$ 右式。对所有合法 $W$ 取 min 得 $\mathrm{ans}\ge$ 右式。

**上界**（$\mathrm{ans}\le$ 右式）：对任一 $h\in\mathcal H$ 使两段均可达，取最短游走 $P_1:a\to h$、$P_2:h\to b$，拼接 $P_1P_2$ 是 $a\to b$ 的游走（**允许重复点**，故即使 $P_1,P_2$ 共享中间点也合法），且经过枢纽 $h$，费用恰为 $\mathrm{dis}[a][h]+\mathrm{dis}[h][b]$。故 $\mathrm{ans}\le$ 每一项有限值，$\mathrm{ans}\le$ 右式。

两边夹得等式。不可行性：左式不可行 $\iff$ 不存在经过枢纽的游走 $\iff$ 每个 $h$ 至少一段不可达（否则上界构造给出合法游走）$\iff$ 右式每项含 $+\infty$ $\iff$ 右式 $+\infty$。端点情形：$a\in\mathcal H$ 时 $h=a$ 项为 $\mathrm{dis}[a][a]+\mathrm{dis}[a][b]=0+\mathrm{dis}[a][b]$（引理 1），正是"起点即枢纽直接走"；$b$ 同理。$a=b$ 时公式给出 $\min_h(\mathrm{dis}[a][h]+\mathrm{dis}[h][a])$：$a$ 是枢纽则含 $0$ 项，否则是过枢纽的最小回路，与 Part 1 口径一致。证毕。

**注**：上界构造是"允许重复经过农场"这一题面条件的**唯一**用武之地，也是本题与"必须简单路径"版本的本质区别。

---

### 引理 4（重边与自环的预处理）

读入时 `dis[u][v]=min(dis[u][v],w)` 正确；正权自环不影响任何 $\mathrm{dis}$；$\mathrm{dis}[i][i]=0$ 不被自环破坏。

*证明*：平行边 $u\to v$ 中，任何游走若使用其中一条，换成最便宜的那条费用不增，故只保留最小权不改变任何点对的最短路。自环 $u\to u$ 权 $w\ge1$：含它的游走删环后费用严格减（引理 1），故最优游走不用自环；代码里 `min(dis[u][u],w)=min(0,w)=0`，$\mathrm{dis}[u][u]$ 保持 $0$ ✓。证毕。

---

### 引理 5（哨兵运算与数值范围）

取 `inf=1145141919810`（`i64`）。则：(a) Floyd 与询问中的加法不溢出；(b) 含 `inf` 的和不会伪装成有限距离；(c) `best<inf` 恰好判可行；(d) 总费用 `ans` 需 `i64`。

*证明*：(a) 任意两个 $\le$ `inf` 的值之和 $\le2.29\times10^{12}\ll2^{63}-1\approx9.2\times10^{18}$ ✓。(b) `inf`$+x\ge$ `inf`（$x\ge0$），故只要有一段不可达，和 $\ge$ `inf`，`min` 不会把它当成有限值；反之两段均可达时和 $\le2(N-1)\cdot10^6<4\times10^8<$ `inf`。(c) 由 (b)，`best<inf` $\iff$ 存在枢纽使两段均可达 $\iff$ 可行（引理 3）。(d) 单询问费用 $<4\times10^8$，$Q\le10^4$ 个求和 $<4\times10^{12}>2^{31}-1\approx2.1\times10^9$，`int` 必溢出，`i64` 安全。另：真实距离 $<2\times10^8$，若用 `int` 存距离本身够，但 INF+INF 在 `int` 下（`1145141919`$\times2\approx2.29\times10^9>$ `INT_MAX`）溢出为负，会伪造"更短路"——这正是全程 `i64` 的理由。证毕。

---

### 引理 6（复杂度）

时间 $O(N^3+M+QK)$，空间 $O(N^2)$。

*证明*：初始化 `fill` 为 $O(\text{mxn}^2)$ 常数级（$\approx4\times10^4$）；读边 $O(M)$；Floyd 三重循环恰 $N^3\le8\times10^6$ 次 `min` 与加法；每询问枚举 $K$ 个枢纽，共 $QK\le10^6$ 次。合计 $\approx9\times10^6$ 次基本操作。空间：`dis` 为 $205\times205$ 个 `i64` $\approx336$ KB。证毕。

---

### 定理

程序第一行输出可行询问数，第二行输出所有可行询问的最小费用之和，复杂度 $O(N^3+M+QK)$。

*证明*：引理 4 保证建图正确；引理 1、2 保证 Floyd 结束后 `dis` 是全源最短路；引理 3 保证每询问答案等于 $\min_h(\mathrm{dis}[a][h]+\mathrm{dis}[h][b])$ 且可行性判据正确；引理 5 保证哨兵运算不溢出、不伪造、总和用 `i64`；引理 6 给出复杂度。计数器 `cnt` 与累加器 `ans` 按判据逐询问累计，即所求。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **consider easier version** — 每询问分层图 Dijkstra 是 $Q\cdot M\log N\approx8\times10^8$ 的暴力；$N\le200$ 暗示预处理全源最短路。
2. **explore its property** — "至少经过一个枢纽" + **允许重复经过点** ⇒ 在枢纽处剪开/拼接，两段独立：$\mathrm{ans}=\min_h\mathrm{dis}[a][h]+\mathrm{dis}[h][b]$。
3. **reduce to easier problem** — 询问归约为全源最短路上的 $K$ 项取 min；端点是枢纽被 $\mathrm{dis}[x][x]=0$ 自动覆盖。
4. **classic problem solution** — Floyd $O(N^3)=8\times10^6$，$N\le200$ 下最省心的全源最短路。
5. **discuss by category** — 重边取 min；正权自环无用；$a=b$ 分"是枢纽（费用 0）/非枢纽（过枢纽回路）"；不可达用哨兵判。
6. **explore its property** — 数值三坑：总费用 $\approx4\times10^{12}$ 必须 `i64`；`int` INF+INF 溢出伪造负路 ⇒ 距离与哨兵全 `i64`；全局数组默认 $0$ 不是 INF，必须显式 `fill`。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|---|---|---|---|
| `n,m,k,q` | 点数 / 边数 / 枢纽数 / 询问数 | `int`，读入 | 枢纽 = 前 $k$ 个点（0-based $0\dots k-1$） |
| `mxn` | $205$ | `constexpr int` | 约束 $+5$ |
| `inf` | 不可达哨兵 | `constexpr i64{1145141919810}` | **i64 哨兵**，INF+INF 不溢出 |
| `dis[u][v]` | $u\to v$ 最短路费用 | `i64[mxn][mxn]`，先 `fill` 成 `inf` 再 `dis[i][i]=0` | 全局默认 $0$ 不是 INF！ |
| `cnt` | 可行询问数 | `int`，全局默认 $0$ | 第一行输出 |
| `ans` | 可行询问最小费用之和 | `i64`，全局默认 $0$ | 可达 $4\times10^{12}$，必须 `i64` |
| `best` | 当前询问的 $\min_h$ 值 | `i64`，每询问重置 `inf` | `best<inf` 即可行 |
| `h` | 枚举的枢纽（0-based） | `int` | $0\dots k-1$ |

### 算法步骤

1. I/O 加速三行；读 $N,M,K,Q$。
2. `dis` 全表 `fill` 成 `inf`；`dis[i][i]=0`；读 $M$ 条边（`--u;--v`），重边取 min。
3. Floyd：外层 $t$、内层 $i,j$，`dis[i][j]=min(dis[i][j],dis[i][t]+dis[t][j])`。
4. 每询问读 $a,b$（`--a;--b`），`best=min_h(dis[a][h]+dis[h][b])`；若 `best<inf` 则 `++cnt,ans+=best`。
5. 输出 `cnt`、`ans` 各一行。

### 复杂度

- 时间：$O(N^3+M+QK)\approx9\times10^6$。
- 空间：$O(N^2)\approx336$ KB。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；Hint 样例输出 `2/24` 吻合；重边、端点枢纽、$a=b$ 是枢纽（费用 $0$）、$a=b$ 非枢纽（回路 $7$）四组手造数据均与手算一致。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P3094 [USACO13DEC] Vacation Planning S
// N<=200 ⇒ Floyd 全源最短路；请求 (a,b) 必须经过枢纽 ⇒ 答案=min_h dis[a][h]+dis[h][b]。
// 正权 ⇒ 最优游走可拆成两段最短路；拼接合法因为允许重复经过农场。
// 总费用可达 1e4*2e8=2e12 ⇒ i64；inf 用 i64 哨兵，避免 int INF+INF 溢出。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};
constexpr i64 inf{1145141919810};

int n,m,k,q,cnt;
i64 dis[mxn][mxn],ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k>>q;
	fill_n(&dis[0][0],mxn*mxn,inf);	// 全局默认 0 不是 INF！
	for(int i=0;i<n;++i)
		dis[i][i]=0;
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;--u;--v;
		dis[u][v]=min(dis[u][v],(i64)w);	// 重边取最小
	}
	for(int t=0;t<n;++t)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				dis[i][j]=min(dis[i][j],dis[i][t]+dis[t][j]);
	for(int i=0,a,b;i<q;++i)
	{
		cin>>a>>b;--a;--b;
		i64 best=inf;
		for(int h=0;h<k;++h)
			best=min(best,dis[a][h]+dis[h][b]);
		if(best<inf)
			++cnt,ans+=best;
	}
	cout<<cnt<<'\n'<<ans<<'\n';
	return 0;
}
```

### 代码说明

- **先 `fill` 再置对角线**：全局 `i64` 数组默认 $0$，不 `fill` 的话所有点对初始"距离 0"，Floyd 与询问全部静默出错——这是本风格最经典的陷阱。
- `dis` 与 `inf` 都用 `i64`：`int` 下 `1145141919*2>INT_MAX`，Floyd 的 INF+INF 会溢出成负数、伪造最短路；`i64` 哨兵相加 $\approx2.3\times10^{12}$ 安全。`ans` 同理必须 `i64`。
- `dis[u][v]=min(...)` 处理重边；`dis[i][i]=0` 在前、自环权 $\ge1$，`min` 不会破坏对角线。
- 枢纽就是读入编号的前 $K$ 个点，`--` 后为 $0\dots k-1$，询问循环 `h<k` 直接枚举，无需额外存储。
- `best<inf` 判可行：含 `inf` 的和 $\ge$ `inf`（引理 5），不会误判；两段均可达时和 $<4\times10^8\ll$ `inf`，不会漏判。
- Floyd 外层必须是中转点 $t$（不变量按"允许的内部点集"归纳，引理 2），写错循环顺序就错。