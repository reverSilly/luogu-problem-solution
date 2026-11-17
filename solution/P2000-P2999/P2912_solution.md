# P2912 [USACO08OCT] Pasture Walking G — 题解

## 一、解题过程：人类思考叙事

**题意重述（剥离背景）。** 给一棵 $N$ 个点的无根树（$2\le N\le 1000$），边带正权 $L_i\in[1,10^4]$。$Q$ 次询问（$1<Q<1000$），每次给两点 $p_1,p_2$，求树上两点间**唯一路径**的边权和。点编号 $1..N$，询问可以 $p_1=p_2$（答案 $0$）。

**第一步：先估暴力，别急着上高级算法。** 每次询问从 $p_1$ 出发 DFS/BFS 到 $p_2$，单次 $O(N)$，总量 $O(NQ)\le 10^6$——**在本题约束下其实能过**。这一点必须诚实承认：如果只想拿分，每询问搜一遍就完了。但题目标签写着 LCA，且 $N,Q$ 再大一个数量级（$10^5$）时暴力就是 $10^{10}$ 立刻爆炸。值得把可推广的经典工具练一遍：**把"路径长度询问"归约成"LCA 询问"**。顺带排除另一个选项：Floyd 全源最短路 $O(N^3)=10^9$，在 $1$ 秒时限边缘疯狂试探，而且树上路径根本不需要"最短路"——路径唯一，没有选择余地，Floyd 是杀鸡用牛刀还杀不利索。

**第二步：定根，把路径拆成两条祖先链。** 任选根（取 $1$ 号，内部 $0$ 号）。记 $\mathrm{dis}[u]$ 为 $u$ 到根的带权距离，$\mathrm{dep}[u]$ 为边数深度。树上路径唯一，而"$u$ 沿父亲链上爬到某公共祖先、再沿父亲链下降到 $v$"是一条合法路径；取**最深**的公共祖先 $w=\mathrm{lca}(u,v)$，这条路径就是唯一路径（Part 2 引理 2 严格证）。它的长度：上行段 $\mathrm{dis}[u]-\mathrm{dis}[w]$，下行段 $\mathrm{dis}[v]-\mathrm{dis}[w]$，合计

$$\mathrm{dist}(u,v)=\mathrm{dis}[u]+\mathrm{dis}[v]-2\,\mathrm{dis}[\mathrm{lca}(u,v)].$$

一次 DFS 预处理出 $\mathrm{dis},\mathrm{dep}$，之后每个询问只差一个 LCA。

**第三步：LCA 怎么求？** 朴素做法是两点一步步上爬，最坏 $O(N)$ 每询问，总量又回到 $10^6$——和暴力同阶，白忙。要加速就得**一次跳多步**：倍增。预处理 $\mathrm{gf}[u][k]=u$ 的 $2^k$ 级祖先，DFS 时顺带 $O(N\log N)$ 建表。求 LCA 分两段：

1. **对齐深度**：把深的点用二进制拆分抬到浅点的深度（从大到小试跳 $2^k$）；
2. **同步上跳**：若两点已相同，它就是 LCA；否则从大到小试 $k$，只要 $\mathrm{gf}[u][k]\ne\mathrm{gf}[v][k]$ 就一起跳。跳完停在 LCA 的**正下方一对儿子**上，返回父亲。

第 2 段的精髓是"跳过头的判据"：$2^k$ 级祖先**不同**，说明还没跳到公共祖先那么高，可以放心跳；**相同**则可能已经越过 LCA，不跳。为什么"不同"就一定没越过？因为一旦跳到 LCA 或其上方，两点落在同一条根链的同一深度，祖先唯一，必然相同（引理 5 严格证）。

**第四步：两个实现细节。**

- **根的父亲设为自己**。这样"跳越过根"自动钳在根上：$\mathrm{gf}[u][k]$ 的深度是 $\max(\mathrm{dep}[u]-2^k,0)$，对齐循环的条件 `dep[gf[u][k]]>=dep[v]` 天然防越界，不需要 INF 哨兵，也不需要特判。
- **范围**：$\mathrm{dis}\le (N-1)\times10^4=9.99\times10^6$，答案同阶，`int` 绰绰有余；$2\,\mathrm{dis}[\mathrm{lca}]\le 2\times10^7$ 也安全。$\texttt{mxk}=11$（$2^{10}=1024>999$ 最大深度）。

**样例核对**（题面只给样例解释，按其反推：边 $1\text{-}2$ 长 $2$、$1\text{-}4$ 长 $3$、$4\text{-}3$ 长 $2$；询问 $(1,2)$ 与 $(3,2)$）：

```
输入：
4 2
1 2 2
1 4 3
4 3 2
1 2
3 2
```

以 $1$ 为根：$\mathrm{dis}=(0,2,7,3)$（点 $1,2,3,4$），$\mathrm{dep}=(0,1,2,1)$。询问 $(1,2)$：$\mathrm{lca}=1$，$0+2-0=2$ ✓；询问 $(3,2)$：$\mathrm{lca}=1$，$7+2-0=9$？不对——等等，$\mathrm{dis}[3]=\mathrm{dis}[4]+2=3+2=5$，不是 $7$！重算：$7$ 是"路径 $3\to4\to1\to2$ 的长度 $2+3+2$"，而 $\mathrm{dis}[3]=5$。答案 $5+2-0=7$ ✓。这个手算小插曲正好提醒：$\mathrm{dis}$ 是**到根**的距离，不是到询问另一端点的距离，两者别混。

**另造三组手算数据。**

链 $1\text{-}2\text{-}3\text{-}4\text{-}5$，边权 $1,2,3,4$：询问 $(1,5)\to10$、$(2,4)\to5$、$(3,3)\to0$。专打"深度差大 + 同点"。

星形：中心 $1$ 连 $2,3,4$，权 $5,7,9$：$(2,3)\to12$、$(3,4)\to16$、$(2,2)\to0$。专打"LCA 恒为根"。

非根内部 LCA：边 $1\text{-}2(1),2\text{-}3(2),2\text{-}4(3),4\text{-}5(4)$，询问 $(3,5)$：$\mathrm{lca}=2$（深度 $1$，非根），$\mathrm{dis}[3]=3,\mathrm{dis}[5]=8,\mathrm{dis}[2]=1$，答案 $3+8-2=9$；交换询问顺序 $(5,3)$ 仍 $9$。专打"同步上跳停在 LCA 儿子"与对称性。

再加一组极限：$1000$ 点链、边权全 $10^4$，询问 $(1,1000)\to 999\times10^4=9\,990\,000$，验证 $\texttt{mxk}$ 边界（深度 $999$ 需要 $2^9$ 级跳）与 `int` 范围。

---

## 二、思维链：完整证明

**记号。** 树 $T=(V,E)$，$|V|=N$，边权 $w>0$。定根 $r=0$（内部编号）。$\mathrm{dep}[u]$：$u$ 到根的边数；$\mathrm{dis}[u]$：$u$ 到根的边权和；$\mathrm{fa}[u]$：父亲（根的父亲定义为根自己）。$u$ 的**祖先**：$u$ 沿 $\mathrm{fa}$ 链可达的点（含 $u$ 自身与根）。$\mathrm{lca}(u,v)$：$u,v$ 的公共祖先中 $\mathrm{dep}$ 最大者。$\mathrm{gf}[u][k]$：倍增表。

---

### 引理 1（树上路径唯一）

任意两点 $u,v$ 之间存在唯一简单路径。

*证明*：题面已直接保证"任意两个不同牧场之间有且仅有一条通行路径"，这里仍给出图论证明以明确结构。存在性：树连通，任取一条 $u\to v$ 行走，删去其中的环（重复点之间的子段）得到简单路径。唯一性：若有两条不同的简单路径 $P_1,P_2$，取它们的并：从 $u$ 出发沿 $P_1$ 走，在第一个"分岔点" $x$（$P_1,P_2$ 在此之后走向不同）分开，沿 $P_2$ 反向回到第一个重新汇合的点 $y$（$v$ 是公共点，故汇合点存在），$x\to y$ 的两段内部点不相交（否则有更早的汇合点），拼出一个环——与树无环矛盾。证毕。

**推论**：树上"路径长度"就是"最短路"，无需任何最短路算法；问题只剩"快速描述这条唯一路径"。

---

### 引理 2（路径过 LCA 与距离公式）

设 $w=\mathrm{lca}(u,v)$。则唯一路径恰为"$u$ 沿父亲链上行到 $w$，再沿 $v$ 的父亲链下行到 $v$"，且

$$\mathrm{dist}(u,v)=\mathrm{dis}[u]+\mathrm{dis}[v]-2\,\mathrm{dis}[w].$$

*证明*：分四小步。

**(a) 公共祖先存在且最深者唯一。** 根是所有点的祖先，公共祖先集非空。任一祖先链上的点深度互异（每步 $\mathrm{dep}-1$），两个公共祖先若深度相同则必是同一点（它们都在 $u$ 的祖先链上，而链上每个深度恰一个点）。故最深公共祖先唯一，$\mathrm{lca}$ 良定义。

**(b) 两条上行链只交于 $w$。** $u\to w$ 链与 $v\to w$ 链上的点都是公共祖先（$w$ 及其上方）或各自私有（$w$ 下方）。若某点 $x\ne w$ 同时在两条链上，则 $x$ 是公共祖先且 $\mathrm{dep}[x]>\mathrm{dep}[w]$（$x$ 在 $w$ 下方），与 $w$ 最深矛盾。故两链除 $w$ 外点集不交。

**(c) 拼接是简单路径。** $u$ 上行到 $w$ 的点序列深度严格递减，无重复；$w$ 下行到 $v$ 是 $v$ 上行链的逆序，也无重复；由 (b) 两段除 $w$ 外不交，$w$ 只出现一次。拼接是简单路径，由引理 1 它就是**唯一**路径。

**(d) 长度望远镜。** 上行段长度 $=\sum$ 链上边权 $=\mathrm{dis}[u]-\mathrm{dis}[w]$（$\mathrm{dis}$ 沿父亲链可加：$\mathrm{dis}[u]=\mathrm{dis}[\mathrm{fa}[u]]+w(u,\mathrm{fa}[u])$，逐层展开到 $w$ 即得差）。下行段同理 $=\mathrm{dis}[v]-\mathrm{dis}[w]$。相加即公式。证毕。

---

### 引理 3（倍增表的语义）

DFS 预处理后，$\mathrm{gf}[u][k]$ 是 $u$ 沿父亲链走 $\min(2^k,\mathrm{dep}[u])$ 步到达的点；特别地 $\mathrm{dep}[\mathrm{gf}[u][k]]=\max(\mathrm{dep}[u]-2^k,\,0)$，且 $\mathrm{gf}[u][k]$ 仍是 $u$ 的祖先。

*证明*：对 $k$ 归纳（$u$ 任意）。

$k=0$：代码 $\mathrm{gf}[u][0]=\mathrm{fa}[u]$。根的 $\mathrm{fa}$ 是自己，走 $\min(1,0)=0$ 步停在根 ✓；非根走 $1$ 步到父亲 ✓。深度式：非根 $\mathrm{dep}-1=\max(\mathrm{dep}-1,0)$（$\mathrm{dep}\ge1$）；根 $0=\max(0-1,0)$ ✓。

设 $k-1$ 成立。代码 $\mathrm{gf}[u][k]=\mathrm{gf}[\mathrm{gf}[u][k-1]][k-1]$：先走 $\min(2^{k-1},\mathrm{dep}[u])$ 步到 $x$，再走 $\min(2^{k-1},\mathrm{dep}[x])$ 步。若 $2^{k-1}\ge\mathrm{dep}[u]$：第一段已到根，$\mathrm{dep}[x]=0$，第二段走 $0$ 步，共 $\mathrm{dep}[u]=\min(2^k,\mathrm{dep}[u])$ 步 ✓。若 $2^{k-1}<\mathrm{dep}[u]$：$\mathrm{dep}[x]=\mathrm{dep}[u]-2^{k-1}$；第二段走 $\min(2^{k-1},\mathrm{dep}[u]-2^{k-1})$ 步，合计 $2^{k-1}+\min(2^{k-1},\mathrm{dep}[u]-2^{k-1})=\min(2^k,\mathrm{dep}[u])$ ✓。深度式与祖先性随归纳保持（祖先的祖先仍是祖先）。

**计算顺序合法性**：DFS 先填 $\mathrm{gf}[u][\cdot]$ 再递归儿子；填 $\mathrm{gf}[u][k]$ 引用的 $\mathrm{gf}[x][k-1]$ 中 $x=\mathrm{gf}[u][k-1]$ 是 $u$ 的祖先，其整行在更早的 DFS 层已填好（根行在 $\mathrm{dfs}(r,r)$ 开头自洽填完：$\mathrm{gf}[r][k]=\mathrm{gf}[r][k-1]=r$）。无未初始化读取。证毕。

---

### 引理 4（对齐深度循环）

设进入 $\mathrm{lca}$ 时 $\mathrm{dep}[u]\ge\mathrm{dep}[v]$（代码先 swap 保证）。执行 `for k=mxk-1..0: if(dep[gf[u][k]]>=dep[v]) u=gf[u][k];` 后：$\mathrm{dep}[u]=\mathrm{dep}[v]$，且 $u$ 是原 $u$ 的祖先（或自身），$\mathrm{lca}(u,v)$ 不变。

*证明*：记 $\Delta=\mathrm{dep}[u]-\mathrm{dep}[v]\ge0$。由引理 3，条件 $\mathrm{dep}[\mathrm{gf}[u][k]]\ge\mathrm{dep}[v]$ 即 $\max(\mathrm{dep}[u]-2^k,0)\ge\mathrm{dep}[v]$。

- 若 $\mathrm{dep}[v]>0$：等价于 $\mathrm{dep}[u]-2^k\ge\mathrm{dep}[v]$，即 $2^k\le$ 当前剩余差 $\delta$。循环从 $k=10$ 到 $0$ 贪心：能减 $2^k$ 就减（含等号，$2^k=\delta$ 时一步减到 $0$）。下证终态 $\delta=0$：

  假设终态 $\delta>0$，取 $k_0=\lfloor\log_2\delta\rfloor\le10$，则 $2^{k_0}\le\delta$。考察层 $k_0$ 执行时的剩余差 $\delta'\ge\delta$（差只减不增）。由于所有 $>k_0$ 的层都"能跳就跳"，进入层 $k_0$ 时必有 $\delta'<2^{k_0+1}$（否则层 $k_0+1$ 会跳掉 $2^{k_0+1}$）。于是层 $k_0$ 的条件 $2^{k_0}\le\delta'$ 成立，必跳，跳后剩余 $\delta'-2^{k_0}<2^{k_0}$；后续层只能再减，终态 $\delta<2^{k_0}\le\delta$，矛盾。故 $\delta=0$。

- 若 $\mathrm{dep}[v]=0$（$v$ 是根）：条件恒真（深度 $\ge0$），每层都跳；跳 $\mathrm{dep}[u]$ 步内到根后钳住，终态 $u=r=v$，$\mathrm{dep}$ 相等 ✓。

每次跳都跳到祖先，故 $u$ 始终是原 $u$ 的祖先。$\mathrm{lca}$ 不变：新 $u$ 是原 $u$ 的祖先且 $\mathrm{dep}[\text{新}u]=\mathrm{dep}[v]\ge\mathrm{dep}[\mathrm{lca}]$（LCA 深度 $\le$ 两端点深度），故新 $u$ 在原 $u\to\mathrm{lca}$ 链上（含 $\mathrm{lca}$），它与 $v$ 的最深公共祖先仍是 $\mathrm{lca}$（公共祖先集是原公共祖先集的子集且仍含 $\mathrm{lca}$：新 $u$ 的祖先链 $\subseteq$ 原 $u$ 的祖先链，而 $\mathrm{lca}$ 在新 $u$ 链上）。证毕。

---

### 引理 5（同步上跳循环）

对齐后设 $\mathrm{dep}[u]=\mathrm{dep}[v]=d$，$u\ne v$，$L=\mathrm{lca}(u,v)$，$g=d-\mathrm{dep}[L]\ge1$。执行 `for k=mxk-1..0: if(gf[u][k]!=gf[v][k]) u=gf[u][k],v=gf[v][k];` 后，$u,v$ 恰为 $L$ 的两个（不同）儿子方向上的点且 $\mathrm{dep}= \mathrm{dep}[L]+1$；返回 $\mathrm{gf}[u][0]=L$。

*证明*：核心断言：在同深度 $d$、同为 $L$ 严格后代的前提下，

$$\mathrm{gf}[u][k]\ne\mathrm{gf}[v][k]\iff 2^k<g\quad(\text{当前 }g=d-\mathrm{dep}[L]).$$

**（$\Leftarrow$）** 若 $2^k<g$：落点深度 $d-2^k>\mathrm{dep}[L]$。$u$ 的祖先链在 $L$ 下方的部分与 $v$ 的在 $L$ 下方的部分**点集不交**（引理 2(b)：交点会是更深的公共祖先）。两个落点都在各自链的 $L$ 下方，故不同。（$2^k<g\le d$ 保证不触发根钳位。）

**（$\Rightarrow$）** 若 $2^k\ge g$：落点深度 $\le\mathrm{dep}[L]$（或被钳到根，深度 $0\le\mathrm{dep}[L]$）。$u,v$ 的祖先链在深度 $\le\mathrm{dep}[L]$ 的部分**完全重合**（都是 $L$ 及其上方，祖先链在公共祖先之上唯一），同一深度的祖先唯一，故两落点相同。

于是循环等价于"从大到小，凡 $2^k<g$ 就把 $g$ 减 $2^k$"。与引理 4 同样的贪心二进制论证（条件含严格不等号版本）：不变量"处理完层 $k$ 后 $g\le2^k$"——初始 $g\le999\le2^{10}$；层 $k$ 若 $2^k<g$ 则减（$g\le2^{k+1}\Rightarrow g-2^k\le2^k$），否则 $g\le2^k$ 保持。终态 $g\le1$；又每次跳都保持"$u,v$ 是 $L$ 的严格后代"（落点深度 $>\mathrm{dep}[L]$），故 $g\ge1$。得 $g=1$。

此时 $u,v$ 深度 $\mathrm{dep}[L]+1$，它们的父亲是深度 $\mathrm{dep}[L]$ 的公共祖先；$L$ 是最深公共祖先，故该父亲就是 $L$（循环在 $k=0$ 时 $\mathrm{gf}[u][0]=\mathrm{gf}[v][0]=L$ 相同，不跳，恰好停住）。返回 $\mathrm{gf}[u][0]=L$ ✓。

另：若对齐后 $u=v$，则 $u$ 是 $v$ 的祖先且同深度，$u=v$ 即公共祖先中最深者（任何更深公共祖先须是 $u$ 的真祖先，深度更小），直接返回 $u$ 正确。证毕。

---

### 引理 6（范围与复杂度）

$\mathrm{dis}[u]\le9.99\times10^6$，答案 $\le9.99\times10^6$，`int` 安全；时间 $O(N\log N+Q\log N)$，空间 $O(N\log N)$。

*证明*：根到任一点的路径至多 $N-1\le999$ 条边，每权 $\le10^4$，故 $\mathrm{dis}\le9.99\times10^6$；$\mathrm{dist}(u,v)\le\mathrm{dis}[u]+\mathrm{dis}[v]\le1.998\times10^7<2^{31}$（实际上 $\mathrm{dist}\le9.99\times10^6$，因唯一路径边数 $\le999$）。中间量 $2\,\mathrm{dis}[\mathrm{lca}]\le2\times10^7$ 安全。DFS 每点填 $\texttt{mxk}=11$ 个倍增格，$O(N\log N)$；每询问两次 $11$ 层循环，$O(\log N)$。空间 $\mathrm{gf}$ 为 $1005\times11$ 个 `int` $\approx44$ KB。证毕。

---

### 定理

程序对每个询问输出树上两点唯一路径的边权和，总时间 $O(N\log N+Q\log N)$。

*证明*：引理 1 保证路径唯一、无需最短路算法；引理 2 把路径长度归约为 $\mathrm{dis}[u]+\mathrm{dis}[v]-2\,\mathrm{dis}[\mathrm{lca}]$；引理 3 保证倍增表语义与计算顺序；引理 4、5 保证 $\mathrm{lca}$ 函数两段循环正确返回 LCA；引理 6 给出范围与复杂度。串联即得。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **explore its property** — 树：路径唯一，"路径长度"不是最短路问题；边权正、可加。
2. **consider easier version** — 每询问 DFS 一遍 $O(NQ)=10^6$ 在本题其实能过；Floyd $O(N^3)=10^9$ 反而危险，排除。
3. **reduce to easier problem** — 定根后路径必过 LCA：$\mathrm{dist}(u,v)=\mathrm{dis}[u]+\mathrm{dis}[v]-2\,\mathrm{dis}[\mathrm{lca}]$，路径询问归约为 LCA 询问。
4. **classic problem solution** — 倍增 $\mathrm{gf}[u][k]=2^k$ 级祖先，DFS 顺带 $O(N\log N)$ 建表；根的父亲设自己，跳越根自动钳位。
5. **classic problem solution** — LCA 两段式：先二进制拆分对齐深度，再"祖先不同就同跳"停在 LCA 儿子层；判据的充分必要性用"祖先链在 LCA 上方重合、下方不交"证明。
6. **explore its property** — 范围核算 $\mathrm{dis}\le9.99\times10^6$ 用 `int`；$\texttt{mxk}=11$ 覆盖深度 $999$；$\mathrm{dis},\mathrm{dep}$ 在根处零初始化恰好正确。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 | 初值 / 记忆点 |
|------|------|------|---------------|
| `n,q` | 点数 $\le1000$、询问数 $<1000$ | `int` | 读入 |
| `graph[u]` | 邻接表，元素 `{v,w}` | `vector<pair<int,int>>[mxn]` | 双向加边 |
| `dep[u]` | $u$ 到根的**边数**深度 | `int[mxn]` | 根 $0$（全局默认），DFS 赋值 |
| `dis[u]` | $u$ 到根的**带权**距离 | `int[mxn]` | 根 $0$，DFS 累加；$\le9.99\times10^6$ |
| `gf[u][k]` | $u$ 的 $2^k$ 级祖先（越根钳在根） | `int[mxn][mxk]` | DFS 先序填；根行全为根 |
| `mxn,mxk` | $1005$、$11$（$2^{10}>999$） | `constexpr int` | 数组尺寸 |
| `lca(u,v)` | 最深公共祖先 | `int(int,int)` | 对齐 + 同跳两段 |

### 算法步骤

1. I/O 加速三行；读 $N,Q$；读 $N-1$ 条边（`--u;--v`）双向建图。
2. `dfs(0,0)`：填 $\mathrm{gf}[u][0]=\mathrm{fa}$ 与整行倍增；对每个儿子赋 $\mathrm{dep},\mathrm{dis}$ 后递归。
3. 每询问读 $u,v$（转 0-based），输出 `dis[u]+dis[v]-2*dis[lca(u,v)]`。

### 复杂度

- 时间 $O(N\log N+Q\log N)\approx 2\times10^4$ 次基本操作；空间 $O(N\log N)\approx44$ KB（倍增表）。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；样例解释反推数据输出 $2,7$ 吻合；链（$10,5,0$）、星形（$12,16,0$）、非根 LCA（$9,9$）、$1000$ 点满权链（$9\,990\,000$ 等）共五组手算数据全部吻合。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2912 [USACO08OCT] Pasture Walking G
// 树上两点路径必过 LCA：dist(u,v)=dis[u]+dis[v]-2*dis[lca]。
// dis = 到根的带权距离；倍增 gf[u][k] = u 的 2^k 级祖先（越过根停在根）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxk{11};	// 2^10=1024>1000

int n,q,dep[mxn],dis[mxn],gf[mxn][mxk];
vector<pair<int,int>>graph[mxn];

void dfs(int u,int fa)
{
	gf[u][0]=fa;
	for(int k=1;k<mxk;++k)
		gf[u][k]=gf[gf[u][k-1]][k-1];
	for(auto[v,w]:graph[u])
		if(v!=fa)
		{
			dep[v]=dep[u]+1;
			dis[v]=dis[u]+w;
			dfs(v,u);
		}
}

int lca(int u,int v)
{
	if(dep[u]<dep[v])
		swap(u,v);
	for(int k=mxk-1;k>=0;--k)	// 先把深的抬到同深度
		if(dep[gf[u][k]]>=dep[v])
			u=gf[u][k];
	if(u==v)
		return u;
	for(int k=mxk-1;k>=0;--k)	// 一起上跳，停在 LCA 的正下方
		if(gf[u][k]!=gf[v][k])
			u=gf[u][k],v=gf[v][k];
	return gf[u][0];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	for(int i=1,u,v,w;i<n;++i)
	{
		cin>>u>>v>>w;--u;--v;
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}
	dfs(0,0);	// 根的父亲设为自己：跳越根时钳在根，无需 INF 哨兵
	for(int i=0,u,v;i<q;++i)
	{
		cin>>u>>v;--u;--v;
		cout<<dis[u]+dis[v]-2*dis[lca(u,v)]<<'\n';
	}
	return 0;
}
```

### 代码说明

- **根的父亲是自己**（`dfs(0,0)`）：倍增跳越根时钳在根，深度式 $\max(\mathrm{dep}-2^k,0)$ 自动成立，对齐条件 `dep[gf[u][k]]>=dep[v]` 因此永不越界——省掉 INF 哨兵与一切特判。
- `dfs` 里**先填本点倍增行、再递归儿子**：填 `gf[u][k]` 引用的祖先行必然已填好（引理 3），顺序反了会读到全局默认 $0$ 而静默出错。
- 第二段循环的判据是 `!=` 才跳：相同说明可能已越过 LCA（引理 5 的 $\Rightarrow$ 方向），跳了就越过头；循环结束返回 `gf[u][0]` 而不是 `u`——此时 $u$ 停在 LCA 的**儿子**层。
- `dis` 是到**根**的距离，公式里减两倍 LCA 的 `dis` 恰好消去重复计入的根→LCA 段；手算时别把 `dis[3]` 误当成"到另一端点的距离"（Part 1 的插曲）。
- $N\le1000$ 递归深度安全；$\mathrm{dis}\le9.99\times10^6$，`int` 足够，无需 `long long`。
- 本题约束下每询问 DFS 的 $O(NQ)$ 暴力也能过，但倍增 LCA 把单询问压到 $O(\log N)$，约束放大到 $10^5$ 依然成立——这是练这道题的真正收益。