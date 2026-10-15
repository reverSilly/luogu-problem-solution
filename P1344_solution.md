# P1344 [USACO4.4] 追查坏牛奶 Pollutant Control — 题解

## Part 1 · 重读题目（白话版）

- 有向图：$N$ 个仓库（$2\le N\le 32$），$M$ 辆卡车（$0\le M\le 10^3$），第 $i$ 辆把牛奶从 $S_i$ 单向运到 $E_i$，停掉它的损失是 $C_i$（$0\le C_i\le 2\times 10^6$，**可以为 $0$**）。
- $1$ 号是发货工厂（源点 $s$），$N$ 号是零售商（汇点 $t$）。坏牛奶能沿**任何**一条 $s\to t$ 的有向路径流过去，所以要停掉一批车，使图中不再存在 $s\to t$ 路径 —— 这正是"有向图 $s$-$t$ 边割"。
- 输出（**本地题面**）：两个整数 $C,T$。$C$ = 最小的总损失；$T$ = **在损失最小的那些方案里**，最少要停的车数。也就是字典序最小化 $(\text{代价},\text{车数})$，**代价优先**。
- 关于"输出边的编号集合"：那是 USACO 原版（`ditch`）的要求，**本地洛谷题面只要求两个整数**，不输出编号。所以本解法主输出是 `C T`，但把方案还原完整做出来并保留在代码里（含"属于某个最优方案"的判据），改判原版只需放开注释。编号顺序：按输入顺序编号 $1\dots M$，输出时升序（我们的枚举顺序天然是升序）。
- 文件 I/O：本地题面未提文件读写，按 stdin/stdout 处理；若投 USACO 训练站需 `ditch.in/ditch.out`，代码中以注释保留这一对 `freopen`。
- **题面没有样例**（本文件里没有 Sample 段），因此无法对标官方样例；下面用自造小例逐步手算验证。

---

## Part 2 · 思维链（发现过程，含完整证明）

### Step 1 探索结构：方案 = 边割

**引理 1（只考虑形如 $\delta(S)$ 的割不丢最优解）** 设 $F$ 是任一方案（删掉 $F$ 后 $s$ 到 $t$ 不可达）。令 $S=\{x:\ x\text{ 在 }G-F\text{ 中由 }s\text{ 可达}\}$，则 $\delta(S)\subseteq F$。

*证明* 若存在边 $e=(u,v)\in\delta(S)$ 且 $e\notin F$：$u\in S$ 说明 $G-F$ 中有 $s\leadsto u$，加上未被删的 $e$ 得到 $s\leadsto v$，即 $v\in S$，与 $v\notin S$ 矛盾。故 $\delta(S)\subseteq F$。又所有 $C_i\ge 0$，所以 $\text{cost}(\delta(S))\le \text{cost}(F)$、$|\delta(S)|\le |F|$，用 $\delta(S)$ 替换 $F$ 两个目标都不变差。证毕。

于是问题变成：**在 $s$-$t$ 割上做带次级目标的最小割**。直接枚举源侧 $S$ 是 $2^{N-2}\le2^{30}\approx10^9$ 个子集，每个还要判连通性，显然不可行；必须靠最大流。单看代价就是教科书的最小割（最大流最小割定理），麻烦在于第二目标。

### Step 2 更简单版本

- 只有代价：直接最小割。
- 只有车数：容量全取 $1$ 的最小割（就是"最少割几刀"）。
- 两个目标 $\Rightarrow$ 想把它们压进**一个**整数里，让最大流只跑一次就同时优化两者。这就是权重放大。

### Step 3 权重放大：$c'_e=K\cdot C_e+1$，取 $K=m+1$

对割 $X$ 记 $\text{cost}(X)=\sum_{e\in X}C_e$，$|X|$ 为车数，新容量和
$$V(X)=\sum_{e\in X}(K\cdot C_e+1)=K\cdot \text{cost}(X)+|X|.$$

**引理 2（放大方向与 $K$ 的显式下界）** 若 $K>m$（即取 $K=m+1$），则对任意两个割 $A,B$：
$$\big(\text{cost}(A),|A|\big)<_{\text{lex}}\big(\text{cost}(B),|B|\big)\ \Longrightarrow\ V(A)<V(B),$$
即 $V$ 的最小值恰好落在字典序最小的割上。

*证明* 分两种情况。
1) $\text{cost}(A)<\text{cost}(B)$：代价是整数，故 $\text{cost}(B)-\text{cost}(A)\ge 1$。于是
$$V(A)-V(B)=K\big(\text{cost}(A)-\text{cost}(B)\big)+\big(|A|-|B|\big)\le -K+(m-0)=m-K<0.$$
关键不等式 $|A|-|B|\le m$ 用到"$|\cdot|\le m$"（割是边集，至多 $m$ 条）与 $K> m$。**这就是 $K$ 的下界：必须严格大于边数的最大可能差 $m$**（代价差最坏只有 $1$）。
2) $\text{cost}(A)=\text{cost}(B)$ 且 $|A|<|B|$：$V(A)-V(B)=|A|-|B|<0$，此时 $K$ 不参与。
关于"反过来是否也成立"（$V$ 小 $\Rightarrow$ 字典序小）：设 $X$ 是 $V$ 最小的割，若它不是字典序最小，取字典序最小的割 $Y$，由已证方向 $V(Y)<V(X)$，矛盾；故 $V$ 最小者必字典序最小。反过来字典序最小的割也一定 $V$ 最小：任一 $V$ 最小割 $X$ 与它比较，$X$ 也字典序最小，于是它们 $(\text{cost},|X|)$ 数值相同。这就说明 $(C^*,T^*)$ 定义良好（所有最优方案的代价、车数都分别相等），且"最小化 $V$"与"字典序最小化"是同一件事。$K>m$ 这条下界只用来给"排序"提供保障，取 $K=m$ 时排序在绝大多数数据上仍正确（唯一可能翻盘的是 $|B|=0$ 的空割，而那只在已不连通时出现），**但下一段说明 $K\ge m+1$ 是反解不可省的要求**。证毕。

**$K\le m$ 为什么不行（编码不再单射 + 余数回卷）** 编码要把二元组 $(\text{cost},|X|)$ 注入整数 $V=K\cdot\text{cost}+|X|$。因为 $0\le |X|\le m$，当 $K\le m$ 时 $(c,\,K)$ 与 $(c+1,\,0)$ 都映到同一个整数 $cK+K=(c+1)K$，编码**不是单射**：反解 $T=V\bmod K$ 必然 $<K$，于是"$|X|=K$"这种割的车数会被写回成 $0$ 并把代价多算 $1$。举例（纯算术）$m=3$，若最优割 $(\text{cost},|X|)=(1,3)$：取 $K=3$ 得 $V=6$，反解成 $(6/3,\,6\bmod3)=(2,0)$，两个数都错；取 $K=4$ 得 $V=7$，反解 $(7/4,\,7\bmod4)=(1,3)$ ✓。所以 $K\ge m+1$ 是**反解**的硬要求，而不只是引理 2 里排序的需要；引理 3 中"$T\le m<K$"这条正是靠它成立。取 $K=m+1$ 同时满足两处，且是满足 $K>m$ 的最小整数（再大只会增大溢出风险，不改善正确性）。

**引理 3（反解方向）** 设最小新容量为 $V_{\min}$，对应最优方案 $(C^*,T^*)$。由 $0\le T^*\le m<K$ 与 $V_{\min}=K\cdot C^*+T^*$，带余除法的唯一性给出
$$C^*=\left\lfloor V_{\min}/K\right\rfloor,\qquad T^*=V_{\min}\bmod K.$$
*证明* 带余除法说：对 $V_{\min},K>0$ 满足 $V_{\min}=Kq+r,\ 0\le r<K$ 的 $(q,r)$ 唯一；$(C^*,T^*)$ 已满足该区间条件，故 $q=C^*,r=T^*$。若把方向弄反（用 $V/K$ 当车数、余数当代价）数值毫无意义。证毕。

**溢出核算（必须 i64）**
- 单边：$c'_e=C_iK+1\le 2\cdot10^6\cdot 1001+1=2.002001\times10^9<2^{31}-1=2.147483647\times10^9$（单条刚好不炸 `int`，是巧合，不要依赖）。
- 总流量/割值：$V_{\min}\le \sum_{e}c'_e\le 10^3\times 2.002001\times10^9=2.002001\times10^{12}>2^{31}$，**必须 `i64`**；反边容量累加也 $\le c'_e$，累加器 `flow` 上限 $2.1\times10^{12}\ll 9.2\times10^{18}$（`i64` 安全）。
- 若换"车数优先"的方向（见 Step 5），要 $K'>\sum C_i\le 2\times10^{9}$，$K'+C_i\approx2.0\times10^9$ 仍勉强进 `int`，但总和同样 $\sim2\times10^{12}$ 溢出 `int`。

**零代价边的坑**：$C_i=0$ 时新容量是 $0\cdot K+1=1>0$。这个 $+1$ 的第二重作用就是**让零代价边在流网络里仍然可见**，否则它们容量为 $0$、既不进割也无法被计数，$T$ 会算错。

### Step 4 方案还原：残量网络上的三个判据

记最大流后的**残量图** $G_f$（只保留 $E[\cdot].w>0$ 的弧），
$$R=\{x:\ s\leadsto x\ \text{在}\ G_f\},\qquad Q=\{x:\ x\leadsto t\ \text{在}\ G_f\}.$$
这里"$\leadsto$"按**自反**理解（允许空路径），故 $s\in R$、$t\in Q$，代码里 `rst[x]` 也总把 $x$ 自己置位；这一约定是判据 $u\not\leadsto v$ 能自动排除自环的原因。

**引理 4（Picard–Queyranne：最小割源侧 $\leftrightarrow$ 残量图闭集）** $S$ 是某个最小割的源侧 $\iff$ $s\in S,\ t\notin S$ 且 $S$ 对 $G_f$ 的弧封闭（$u\in S,(u,v)\in G_f\Rightarrow v\in S$）。

*证明* 记 $f(S,T)=\sum_{e\in S\to T}f_e$，$c(S,T)=\sum_{e\in S\to T}c_e$；恒等式 $|f|=f(S,T)-f(T,S)$ 对任何割成立（净流量等于流量值）。
($\Leftarrow$) 闭集意味着没有残量弧离开 $S$。于是每条原弧 $(u,v)$（$u\in S,v\notin S$）必**饱和**（否则它的残量弧 $u\to v$ 离开 $S$），即 $f(S,T)=c(S,T)$；每条原弧 $(u,v)$（$u\notin S,v\in S$）上流量必为 $0$（若有流量 $x>0$，其反弧 $v\to u$ 残量 $x>0$，正是离开 $S$ 的残量弧），即 $f(T,S)=0$。代入恒等式得 $|f|=f(S,T)=c(S,T)$，即该割容量等于最大流值，故它是最小割。
($\Rightarrow$) 设 $(S,T)$ 是最小割，$f$ 是最大流，则 $c(S,T)=\lambda=\max|f|=|f|$（最大流最小割定理）。而 $|f|=f(S,T)-f(T,S)\le f(S,T)\le c(S,T)=|f|$，首尾相等，故两个不等号都取等：$f(T,S)=0$（每条 $T\to S$ 原弧零流，因为逐项 $\ge0$）且 $f(S,T)=c(S,T)$（每条 $S\to T$ 原弧饱和，同理）。这两条恰好排除了离开 $S$ 的两类残量弧（$S\to T$ 原弧的正残量、$T\to S$ 原弧的反残量），故 $S$ 闭。证毕。

**推论 4.1** $R$ 本身是闭集（可达集按定义闭），且被任何闭集包含；$V\setminus Q$ 也是闭集（若 $u\notin Q$ 且 $u\to v$，则 $v$ 若能到 $t$ 就使 $u$ 也能到 $t$，矛盾），且包含任何闭集（闭集 $S$ 若与 $Q$ 相交，路径会把 $t$ 拖进 $S$）。故每个最小割源侧满足 $R\subseteq S\subseteq V\setminus Q$；特别地 $(R,V\setminus R)$ 与 $(V\setminus Q,Q)$ 都是最小割。

**结论 A（要一套可行方案，只要 $1$ 次最大流）** 取 $\delta(R)$：由推论 4.1 它是最小割，$V(\delta(R))=V_{\min}=KC^*+T^*$；又 $|\delta(R)|\le m<K$，由引理 3 唯一性得 $|\delta(R)|=T^*$、$\text{cost}=C^*$。即"从 $s$ 在残量图里能到的点"划一刀，就是**代价最小且车数也最小**的方案。

**结论 B（哪些车可能出现在某个最优方案里）** 对原图边 $e=(u,v)$（新容量 $\ge1>0$）：
$$e\in\text{某个最小割}\ \iff\ u\notin Q\ \wedge\ v\notin R\ \wedge\ u\not\leadsto v\ (\text{在}\ G_f).$$
*证明* $e$ 被闭集 $S$ 割到 $\iff u\in S,\ v\notin S$。含 $\{s,u\}$ 的**最小**闭集是 $\uparrow\{s,u\}=\{x:\ s\leadsto x\ \text{或}\ u\leadsto x\}$；避开 $\{v,t\}$ 的**最大**闭集是 $V\setminus\downarrow\{v,t\}=\{x:\ x\not\leadsto v\ \text{且}\ x\not\leadsto t\}$（两者闭性：前者显然，后者若 $x$ 在其中且 $x\to y$，$y$ 若能到 $v$ 或 $t$ 则 $x$ 也能，矛盾）。于是存在满足条件的闭集 $\iff$ $\uparrow\{s,u\}\subseteq V\setminus\downarrow\{v,t\}$ $\iff$ 四个不可达关系 $s\not\leadsto v,\ s\not\leadsto t,\ u\not\leadsto v,\ u\not\leadsto t$ 全成立。其中 $s\not\leadsto t$ 由最大流（无增广路）自动成立；剩下三条翻译成 $v\notin R$、$u\notin Q$、$u\not\leadsto v$。
充分性：条件成立时取 $S=\uparrow\{s,u\}$，它闭、含 $s,u$、不含 $v,t$，由引理 4 是最小割源侧且割到 $e$。必要性：任何割到 $e$ 的最小割源侧 $S$ 闭且含 $u$ 不含 $v$，故 $u\not\leadsto v$（否则闭性逼 $v\in S$）；同理 $s\not\leadsto v$、$u\not\leadsto t$。**不重**（每个条件都必要）**不漏**（构造出具体的 $S$）。证毕。

注意 $u\not\leadsto v$ 蕴含 $u,v$ 不同强连通分量，所以同分量内两条点之间的边永远不可能被任何最小割割到（缩图视角：闭集是 SCC 缩图 DAG 上的上闭集，必须整体包含或整体避开一个分量）。

**判据的额外前提（务必写明，否则命题不成立）**
1. $R,Q,\leadsto$ 全部取自**最大流之后**的残量图 $G_f$：引理 4 的两个方向都用了"$|f|=c(S,T)$"，而 $s\not\leadsto t$ 正是最大流的定义。随便一个可行流上跑出来的 $R$ 不是最小割源侧（例如零流时 $R$ 是整个可达闭包，割容量可以任意大）。
2. 边必须**正容量** $c'_e>0$：结论 B 只保证"$e$ 是某个最小割的跨边"，容量为 $0$ 的边即使不被割也不影响容量，会破坏"不重"的方向。本题因 $+1$ 有 $c'_e\ge1$，此前提自动满足（这也是不用"$c'=KC$"的理由之一）。
3. 判据说的是"**存在某个**最优方案含它"，不保证任何两个方案可以任意合并；要拿到一套真正可行的方案仍须用结论 A 的 $\delta(R)$（或 $\delta(V\setminus Q)$），把 `sm` 里的边全停掉一般会超出 $T$ 辆。
4. 结论 A/B/C 都不要求"最小割唯一"；唯一性只是使三者重合的特例（例 1 即 $|$`al`$|=|$`pl`$|=|$`sm`$|$，例 3 则严格不等）。

**结论 C（哪些车在所有最优方案里）** $e=(u,v)\in$ 每个最小割 $\iff u\in R\wedge v\in Q$。
*证明* $\Leftarrow$：推论 4.1 给出 $R\subseteq S\subseteq V\setminus Q$，故 $u\in S$ 且 $v\notin S$ 对一切最小割成立。$\Rightarrow$：若 $u\notin R$，割 $\delta(R)$ 不含 $e$；若 $v\notin Q$，割 $\delta(V\setminus Q)$ 不含 $e$。证毕。三者给出自检链 $|\text{所有}|\le|\delta(R)|=T^*\le|\text{某个}|$。

**引理 4.2（$R\cap Q=\varnothing$ 及三判据相容）** 若 $x\in R\cap Q$，则 $G_f$ 中有 $s\leadsto x\leadsto t$，即存在增广路，与"流已最大"矛盾，故 $R\cap Q=\varnothing$。由此三判据互相自洽：$\delta(R)$ 的任一边 $(u,v)$ 满足 $u\in R\Rightarrow u\notin Q$，且 $u\leadsto v$ 不成立（否则 $v\in R$），故它必被结论 B 标上 —— 即 `al`$\subseteq$`pl`$\subseteq$`sm`（代码里的 `assert` 就是这条）。另外 $|\delta(R)|=T^*$ 与 $T^*\le|\text{sm}|$ 也再次说明结论 A 给出的方案车数确实达到最优。
*证明* 上面三段分别由 $R\cap Q=\varnothing$、闭包单调性、以及引理 3 的唯一性直接得到；包含关系的严格性见例 3（`sm` 有 $3$ 条而 `pl` 只有 $2$ 条）与例 1（`al`$=$`pl`$=$`sm`，最小割唯一）。证毕。

**坑（本题最容易错的地方）** "饱和边就是割边"是错的：$\delta(R)$ 由"$u\in R,\ v\notin R$"决定，饱和但不跨 $R$ 的边（例如从 $R$ 外一点指向汇路的边）不算，例 5 给出可手算的反例。反过来，只用"$u\in R$"会把 $R$ 内部的边误计入；只用"$v\notin R$"会把跟本割无关的下层边误计入。要枚举"可能出现在最优解里的卡车"，必须用结论 B 的三条合取，其中 $u\not\leadsto v$ 需要**全源可达闭包**（代码里对每个点跑一次 `closure`），只从 $s$ 跑一遍 BFS 是拿不到这条信息的。

### Step 5 反向放大（为什么不能先最小化车数）

若题目改成"先最少停车、再最小损失"，编码应是 $c'_e=K'+C_i$，则 $V=K'\cdot|X|+\text{cost}(X)$；要让车数优先需要 $K'>\max(\text{cost差})$，即 $K'>U=\sum C_i\le m\cdot 2\cdot10^6=2\times10^9$（取 $K'=U+1$），此时反解成 $|X|=V/K'$、$\text{cost}=V\bmod K'$。两个方向的 $K$ 下界来源不同：本方向 $K>m$（车数上界），那方向 $K'>m\cdot U_{\max}$（代价上界）。**本地题面是代价优先，用 $K=m+1$。**

### Step 6 兜底：可证明但慢的逐边试探

若不愿依赖结论 B，可用这条精确判据：**$e$ 属于某个最小割 $\iff$ 把 $c'_e$ 减少 $1$ 后最小割值严格变小**。
*证明* 若 $e$ 在某最小割 $X$ 中，则新值 $\le V(X)-1=V_{\min}-1<V_{\min}$。若 $e$ 不属于任何最小割，则含 $e$ 的割容量 $\ge V_{\min}+1$（整数且严格大于），减 $1$ 后 $\ge V_{\min}$；不含 $e$ 的割不变，故新值仍为 $V_{\min}$。证毕。
代价：$M$ 次最大流，单次 Dinic 上界 $O(V^2E)\approx32^2\cdot2000\approx2\times10^6$，总计 $\approx2\times10^9$，在 $N=32,M=1000$ 下偏慢（洛谷可能 TLE）。优化空间：容量只降 $1$，可在旧的最大流上修补——若 $e$ 上流量恰好等于旧容量（新割的弧必饱和，故这种情况出现），沿"流网络中的 $s\leadsto u$ 与 $v\leadsto t$"退回 $1$ 单位，再补跑一次增广即可判定，均摊远小于重跑；但实现繁琐、易错。本解法采用结论 B：$1$ 次最大流 + $O(n)$ 次闭包 DFS，约 $6.5\times10^4$ 步；Step 6 只作为"判据不放心时的对拍/自检思路"保留（环境无编译器，本次未实际对拍）。

---

## Part 3 · 模式总结（按实际思考顺序）

- 探索结构 — 删车方案 $\Rightarrow$ 用 $\delta(S)$ 割代替（引理 1），双目标最小割
- 考虑更简单版本 — 单目标时就是普通最小割 / 单位容量最小割
- 经典算法 — 最大流最小割定理（Dinic）
- 权重放大 — $c'_e=K C_e+1,\ K>m$ 把 $(\text{代价},\text{车数})$ 折成一个整数序
- 带余除法反解 — $C=\lfloor V/K\rfloor,\ T=V\bmod K$（要求 $T<K$）
- 残量图闭集（Picard–Queyranne）— 一次最大流给出"一套方案/属于某个/属于所有"三种判据
- 全源可达闭包（位掩码）— 补上 $u\not\leadsto v$ 这条容易漏的条件
- 兜底暴力 — 逐边试探重跑最大流，可证明正确但慢 $M$ 倍

---

## Part 4 · 正式解法

### 符号表

| 符号 | 含义 | 类型 / 初始化 | 助记 |
|---|---|---|---|
| $n,m$ | 仓库数、卡车数 | `int`，读入 | — |
| $K$ | 放大基数 $=m+1$ | `i64`，建图前 | 必须严格 $>m$ |
| `ip[i]` | 第 $i$ 条输入边 $\{u,v,c\}$，0-based 端点 | 全局结构数组 | 输出编号 $=i+1$ |
| `E[]`,`H[]`,`cnt` | 链式前向星：`{v,w,nx}`、点表头、边指针 | `fill_n(H,mxn,-1)` | `E[i^1]` 是反边；`ip[i]` 的正弧 $=2i$ |
| `dep[]`,`cur[]` | Dinic 分层 / 当前弧 | 每轮 BFS 重置；`copy(H,H+mxn,cur)` | — |
| $V$ | 最小割的新容量和 $=KC+T$ | `i64`，累加最大流 | 反解被除数 |
| `rst[x]` | 残量图中 $x$ 的可达点集（含自身） | `unsigned` 位掩码，$n\le32$ | 传递闭包行 |
| `tim`,`tag[]` | DFS 计时戳，免清空 | `0` / 隐式 $0$ | epoch 技巧 |
| $R$ | $s$ 侧最小集 $=\,$`rst[s]` | `unsigned` | Reachable |
| `pl/pcnt` | 典范割 $\delta(R)$ 的边编号（一套最优方案） | `int[]`,`0` | plan |
| `sm/scnt` | 属于**某个**最优方案的边（结论 B） | `int[]`,`0` | some |
| `al/acnt` | 属于**所有**最优方案的边（结论 C） | `int[]`,`0` | all |

### 算法步骤

1. 读 $n,m$；`K=m+1`；`fill_n(H,mxn,-1)`。
2. 逐条读 $S_i,E_i,C_i$，转 0-based 存入 `ip[i]`，`addedge(u,v,c*K+1)`（`i64` 乘法）。
3. Dinic 求最大流，累加到 `i64 V`（BFS 分层 + 当前弧 DFS，直到无增广路）。
4. 输出 `V/K` 与 `V%K` —— 即题面要的 $C$ 与 $T$。
5. 对每个点 $x$ 跑 `closure(x)` 得 `rst[x]`（只在 $E[i].w>0$ 的残量弧上走）。
6. 逐条原边按三个判据分类：$u\in R,v\notin R\Rightarrow$ `pl`；$u\notin Q,v\notin R,u\not\leadsto v\Rightarrow$ `sm`；$u\in R,v\in Q\Rightarrow$ `al`。
7. `assert` 自检：`pcnt==V%K`、`acnt<=pcnt<=scnt`。
8. 若改判 USACO 原版：放开注释，按升序输出 `pl[1..pcnt]`（枚举顺序即编号升序）。

**正确性小结** 引理 1 把"停车方案"限定为 $s$-$t$ 割；引理 2 保证放大后的最小割就是 $(\text{代价},\text{车数})$ 字典序最小的割；引理 3 说明从 $V$ 用带余除法反解 $(C,T)$ 的方向与合法性；最大流最小割定理给出 $V=\max|f|$（代码里的 `V`），从而完全不枚举 $2^{N-2}$ 个源侧。步骤 5–6 的三个判据由引理 4（闭集 $\leftrightarrow$ 最小割）、推论 4.1 与引理 4.2 保证既不重也不漏；步骤 7 的 `assert` 是这些命题的可执行自检（$|$`pl`$|=V\bmod K$ 与 $|$`al`$|\le|$`pl`$|\le|$`sm`$|$），本地打开即验。

**复杂度** 一次 Dinic：$O(V^2E)=32^2\times2002\approx2\times10^6$；闭包：$n$ 次 DFS $=O(n(n+2m))\approx6.5\times10^4$；总时间 $\approx2\times10^6$，最大流只跑**一次**（不是 $M$ 次）。空间 $O(V+E)$：`E[mxm*2]` 的 `struct{int v;i64 w,nx;}` 因对齐占 $24$ B $\Rightarrow2010\times24\approx48$ KB，其余数组 $\le 8$ KB。

**验证状态** 题面无样例，无法对标官方样例；下 $5$ 组自造小例逐步手算全部通过（覆盖"边数相同代价不同""代价相同边数不同""两目标冲突""边界 $M=0$/不连通/零代价边""饱和边陷阱"，含 $K$ 取值、割值反解、还原边集与自检链 $|\text{al}|\le|\text{pl}|=T\le|\text{sm}|$）；未做随机对拍与测速（环境无编译器，全程未运行代码）。

### 手算验证

**例 1（边数相同、代价不同：验证代价优先）** $n=4,m=4,K=5$；$e_1:1\to2(1),\,e_2:2\to4(5),\,e_3:1\to3(1),\,e_4:3\to4(5)$，新容量 $6,26,6,26$。
所有源侧：$\{1\}\Rightarrow\{e_1,e_3\}=12$；$\{1,2\}\Rightarrow\{e_2,e_3\}=32$；$\{1,3\}\Rightarrow\{e_1,e_4\}=32$；$\{1,2,3\}\Rightarrow\{e_2,e_4\}=52$。
$V_{\min}=12\Rightarrow C=12/5=2,\ T=12\bmod5=2$ ✓（四个割边数都是 $2$，只能由代价定胜负）。
最大流：出 $s$ 的 $e_1,e_3$ 均饱和，$V=6+6=12$ ✓。残量：$R=\{1\}$，$Q=\{2,3,4\}$。还原：`pl`$=\{e_1,e_3\}$，$2$ 条、代价 $2$ ✓；`sm` 同（$e_2,e_4$ 的尾 $\in Q$ 被排除）；`al`$=\{e_1,e_3\}$（最小割唯一）✓。自检 `acnt=pcnt=scnt=2`，且 $2=V\bmod K$ ✓。

**例 2（代价相同、边数不同：验证 $+1$ 的破平作用）** $n=4,m=4,K=5$；$e_1:1\to2(6),\,e_2:2\to4(3),\,e_3:2\to3(3),\,e_4:3\to4(3)$，新容量 $31,16,16,16$。
源侧 $\{1\}\Rightarrow\{e_1\}=31=(\text{cost}\,6,\text{cnt}\,1)$；$\{1,2\}\Rightarrow\{e_2,e_3\}=32\,(6,2)$；$\{1,2,3\}\Rightarrow\{e_2,e_4\}=32\,(6,2)$。
$V_{\min}=31\Rightarrow C=31/5=6,\ T=31\bmod5=1$ ✓。最大流 $=$ 出边 $31$、入汇 $32$，实际 $16+15=31$ ✓（$e_4$ 剩 $1$）。残量：$R=\{1\}$，$Q=\{2,3,4\}$。还原：`pl`$=\{e_1\}$，$1$ 条、代价 $6$ ✓；`sm`$=\{e_1\}$（$e_2,e_3,e_4$ 的尾都能到 $t$）；`al`$=\{e_1\}$ ✓。

**例 3（两目标冲突：验证放大方向，反方向会错）** $n=4,m=4,K=5$；$e_1:1\to2(20),\,e_2:1\to3(5),\,e_3:3\to2(5),\,e_4:2\to4(1000)$，新容量 $101,26,26,5001$。
源侧 $\{1\}\Rightarrow\{e_1,e_2\}=127\,(25,2)$；$\{1,3\}\Rightarrow\{e_1,e_3\}=127\,(25,2)$；$\{1,2\}\Rightarrow\{e_2,e_4\}=5027\,(1005,2)$；$\{1,2,3\}\Rightarrow\{e_4\}=5001\,(1000,1)$。
$V_{\min}=127\Rightarrow C=25,\ T=2$ ✓。若误用"车数优先"($c'_e=K'+C_i$) 会选中只割 $e_4$ 的方案，答 $(1000,1)$，与题面"损失最小优先"矛盾 ✗。
最大流 $=101+26=127$（$e_1,e_2,e_3$ 饱和，$e_4$ 剩 $4874$）。残量：$R=\{1\}$；$2\to4$ 有残量 $\Rightarrow2,4\in Q$；$3$ 只有反弧 $3\to1$ $\Rightarrow rst[3]=\{3,1\}$，$3\notin Q$。
还原：`pl`$=\{e_1,e_2\}$（$2$ 条、代价 $25$）✓；`sm`：$e_1(1\notin Q,2\notin R,1\not\leadsto2)$ ✓、$e_2(3\notin R,1\not\leadsto3)$ ✓、$e_3(3\notin Q,2\notin R,3\not\leadsto2)$ ✓（它属于另一个最小割 $\delta(\{1,3\})$）、$e_4(2\in Q)$ ✗ → `sm`$=\{e_1,e_2,e_3\}$，$3$ 条；`al`$=\{e_1\}$（$1\in R,2\in Q$）✓ 两个最小割都含 $e_1$。自检 $1\le2\le3$ ✓。

**例 4（边界：$M=0$、已不连通、零代价边）**
- $m=0\Rightarrow K=1$，无边可流，$V=0$，输出 `0 0` ✓（$K=1>0=m$ 仍满足 $K>m$，且不会除零）。
- $n=3,m=1$，$e_1:2\to3(7)$：$s=1$ 本就到不了 $t$，$V=0\Rightarrow$ `0 0`；判据下 $rst[2]=\{2,3\}\ni t$，即 $2\in Q$，故 $e_1$ 不被标记，`pl` 为空 ✓。
- $n=3,m=3,K=4$：$e_1:1\to2(0),\,e_2:2\to3(1),\,e_3:1\to3(1)$，新容量 $1,5,5$。源侧 $\{1\}\Rightarrow\{e_1,e_3\}=6=(1,2)$；$\{1,2\}\Rightarrow\{e_2,e_3\}=10=(2,2)$。$V_{\min}=6\Rightarrow C=1,T=2$ ✓，零代价边 $e_1$ 因 $+1$ 而留在割里并被计数（若容量取 $0$ 就会漏计，$T$ 变 $1$ 错）✓。最大流 $=1+5=6$ ✓，$R=\{1\}$，`pl`$=\{e_1,e_3\}$ ✓。

**例 5（饱和边陷阱：为什么判据不能用 $E[i].w==0$）** $n=4,m=4,K=5$；$e_1:1\to2(1),\,e_2:2\to4(1),\,e_3:2\to3(5),\,e_4:3\to4(5)$，新容量 $6,6,26,26$。$e_1$ 是桥，所有割都含它。
源侧 $\{1\}\Rightarrow\{e_1\}=6\,(1,1)$；$\{1,2\}\Rightarrow\{e_2,e_3\}=32\,(6,2)$；$\{1,2,3\}\Rightarrow\{e_2,e_4\}=32\,(6,2)$。$V_{\min}=6\Rightarrow C=1,\ T=1$ ✓（答案 `1 1`）。
两种可能的增广顺序：**①（本代码的实际行为）** 第一轮 BFS 把 $3$ 与 $4$ 都定在第 $2$ 层，弧 $3\to4$ 不在分层图里，于是 $6$ 单位流量全走 $1\to2\to4$，$e_1,e_2$ **同时饱和**；**②** 若手工先走 $1\to2\to3\to4$，则只有 $e_1$ 饱和，$e_2$ 满容量未用，$e_3,e_4$ 各剩 $20$。
两种顺序下 $R=\{1\}$、$Q=\{2,3,4\}$ 都一样 $\Rightarrow$ `pl`$=\{e_1\}$、`sm`$=\{e_1\}$、`al`$=\{e_1\}$，$1=V\bmod K$ ✓。但"饱和边集合"在 ①② 之间不同（① 多出 $e_2$）：按"$E[i].w==0$ 就是割边"来还原，① 会报 $2$ 辆车、$T$ 直接错，② 反而碰巧对 —— 说明该判据既不正确、也不增广顺序无关。用结论 B：$e_2=(2,4)$ 的尾 $2\in Q$ 且 $2\leadsto4$（走 $2\to3\to4$）被双重排除 ✓；$e_3,e_4$ 的尾同样 $\in Q$ 被排除 ✓。
这个例子还说明了"为什么需要 $u\not\leadsto v$ 这条额外条件"：$e_2$ 的 $u=2\notin R$、$v=4\notin R$ 前两条都满足，只有 $u\not\leadsto v$ 把它拦下来。

---

## Part 5 · 示例代码（C++，Louis Li 风格）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{35};              // N<=32,+5 松弛
constexpr int mxm{1005};            // M<=1000,+5 松弛
constexpr i64 inf{1145141919191919};// 增广上界 1.15e15 > 割值上界 2.002e12(注意 {1e18} 会因 double->i64 收窄而编译失败)

int n,m;
i64 K;                              // 放大基数,K=m+1 须严格 > m
struct{int u,v; i64 c;}ip[mxm];     // 原始边,ip[i] = 输入第 i+1 辆卡车
struct{int v; i64 w,nx;}E[mxm*2];   // 放大后容量必须 i64
int H[mxn],cnt=0,dep[mxn],cur[mxn];
unsigned rst[mxn];                  // rst[x]:残量网络中 x 的可达点集(含自身)
int tag[mxn],tim=0;
int pl[mxm],pcnt,sm[mxm],scnt,al[mxm],acnt;

void addedge(int u,int v,i64 w)
{
	E[cnt]={v,w,H[u]};H[u]=cnt++;
	E[cnt]={u,0,H[v]};H[v]=cnt++;
	//不变量:cnt 从 0 起、按输入顺序建图,故 ip[i] 的正弧就是 E[2*i](调试残量时可直接看)
}

bool bfs(int s,int t)
{
	fill_n(dep,mxn,-1);
	queue<int>q;
	dep[s]=0;q.push(s);
	while(!q.empty())
	{
		int u=q.front();q.pop();
		for(int i=H[u];i!=-1;i=E[i].nx)
			if(E[i].w>0&&dep[E[i].v]==-1)
			{
				dep[E[i].v]=dep[u]+1;
				q.push(E[i].v);
			}
	}
	return dep[t]!=-1;
}

i64 dfs(int u,int t,i64 f)
{
	if(u==t)
		return f;
	for(int &i=cur[u];i!=-1;i=E[i].nx)
	{
		int v=E[i].v;
		if(E[i].w>0&&dep[v]==dep[u]+1)
		{
			i64 res=dfs(v,t,min(f,E[i].w));
			if(res)
			{
				E[i].w-=res;
				E[i^1].w+=res;
				return res;
			}
		}
	}
	return 0;                       // 死路:当前弧已扫完
}

void closure(int s)                 // 只在残量弧上走,位掩码记可达集
{
	static int stk[mxn];
	int tp=0;
	unsigned mk=1u<<s;
	tag[s]=++tim;stk[tp++]=s;
	while(tp)
	{
		int u=stk[--tp];
		for(int i=H[u];i!=-1;i=E[i].nx)
		{
			int v=E[i].v;
			if(E[i].w>0&&tag[v]!=tim)
			{
				tag[v]=tim;mk|=1u<<v;stk[tp++]=v;
			}
		}
	}
	rst[s]=mk;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	//freopen("ditch.in","r",stdin); // USACO 训练站原版才需要
	//freopen("ditch.out","w",stdout);

	cin>>n>>m;
	K=m+1;                            // c'=K*C+1:先最小化代价,再最小化车数
	fill_n(H,mxn,-1);                 // 前向星头指针必须填 -1,不能留 0
	for(int i=0;i<m;++i)
	{
		int u,v;
		i64 c;
		cin>>u>>v>>c;--u;--v;
		ip[i]={u,v,c};
		addedge(u,v,c*K+1);           // 2e6*1001+1 fits int,但累计会爆 -> 全程 i64
	}

	int s=0,t=n-1;
	i64 V=0;                          // 最小割新容量和 = K*C+T
	while(bfs(s,t))
	{
		copy(H,H+mxn,cur);
		while(i64 res=dfs(s,t,inf))
			V+=res;
	}

	cout<<V/K<<' '<<V%K<<'\n';        // 带余除法反解:商=代价,余数=车数

	for(int x=0;x<n;++x)              // 方案还原:1 次最大流 + n 次闭包
		closure(x);
	unsigned R=rst[s];                // s 侧最小闭集
	for(int i=0;i<m;++i)
	{
		int u=ip[i].u,v=ip[i].v;
		bool uinR=(R>>u&1),vinR=(R>>v&1),uToT=(rst[u]>>t&1),vToT=(rst[v]>>t&1);
		if(uinR&&!vinR)
			pl[pcnt++]=i+1;           // 典范最小割 delta(R)
		if(!uToT&&!vinR&&!(rst[u]>>v&1))
			sm[scnt++]=i+1;           // 属于某个最优方案(结论 B)
		if(uinR&&vToT)
			al[acnt++]=i+1;           // 属于所有最优方案(结论 C)
	}
	assert((i64)pcnt==V%K);           // 典范割边数 == T
	assert(acnt<=pcnt&&pcnt<=scnt);   // 所有 ⊆ 典范 ⊆ 某个
	// cerr<<scnt<<' ';for(int i=0;i<scnt;++i)cerr<<sm[i]<<' ';cerr<<'\n';
	// USACO 原版输出方案(编号升序,枚举顺序天然升序):
	// cout<<V/K<<' '<<pcnt<<'\n';
	// for(int i=0;i<pcnt;++i)
	// 	cout<<pl[i]<<'\n';
	return 0;
}
```

**实现坑与要点**

- `fill_n(H,mxn,-1)`：全局 `int` 默认 $0$，而 $0$ 是合法边下标，不填会把"没有下一条边"当成"指向第 $0$ 条边"。
- `dep` 每轮 BFS 用 `fill_n(dep,mxn,-1)` 重置，`cur` 每轮用 `copy(H,H+mxn,cur)` 重新起弧。
- 容量数组是**原地消耗**的：最大流跑完后的 `E[].w` 就是残量，正好给 `closure` 用，不要再重建一遍图（否则判据全废）。
- 自环 $S_i=E_i$：`addedge` 两条弧同链、`i^1` 配对仍正确；BFS/DFS 因 `dep[v]==dep[u]+1` 永假而不走；闭包里 $u\leadsto u$ 恒真 $\Rightarrow$ 判据 $u\not\leadsto v$ 失败，自环永不被误报为割边（它确实不可能被任何割分离）✓。
- $C_i=0$ 的边容量为 $1$（$+1$ 的第二重作用），保证能被割、能被计数。
- $m=0\Rightarrow K=1$：无除零风险，$V=0$ 直接反解出 `0 0`。

---

*一句话总结：把 $(\text{代价},\text{车数})$ 折成 $K\cdot\text{代价}+\text{车数}$（$K=m+1$）跑一次最大流，商与余数就是答案的两个数；残量图上的闭集理论（$R$、$Q$、全源可达闭包）再用 $O(n\cdot m)$ 给出"这套方案是谁、谁可能进方案、谁必进方案"，无需为每条边重跑最大流。*
