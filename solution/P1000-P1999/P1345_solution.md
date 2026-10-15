# P1345 [USACO5.4] 奶牛的电信 Telecowmunication

## Part 1 — 重新读题

$N$ 台电脑（编号 $1\sim N$），$M$ 条**双向**链路，链路是电脑之间的直接连接；题面保证：**连接没有重复**、**两台电脑之间至多有一条连接**、并且 **$c_1$ 与 $c_2$ 不会直接相连**。

奶牛踩坏一台电脑后，这台电脑不能收发邮件，**与它相关的所有连接同时失效**（等价于图中删掉这个顶点及其所有邻边）。求最少的"被踩坏电脑数"，使 $c_1$ 与 $c_2$ 之间不存在任何通路；并且 **$c_1,c_2$ 自己不能被踩坏**。

数据范围：$1\le N\le 100$，$1\le M\le 600$。

输出格式（逐字确认本地题面）：**"一行，一个整数，表示使电脑 $c_1$ 和 $c_2$ 不能互相通信需要坏掉的电脑数目的最小值。"** 即洛谷只要最小值一个数，**不要求输出具体方案**；USACO 原始版本还要求第二行按升序输出被删电脑编号，本地题面里已删去（处理方式见 Part 4/5，代码里保留可 uncomment 的方案输出块）。本地题面**没有样例**，只有描述中那张 3 点 2 边的示意图（$1\!-\!3\!-\!2$，答案 $1$）——**无法对标官方样例**，本文的验证全部是手算（Part 2 §2.8）。

一句话本质：这是**无向图上的最小 $s$-$t$ 点割（顶点分离集）**，源汇本身不可删。

题面逐条线索（照抄原文中每句"保证"，并标出它在解法里被用在哪一步）：

| 题面原文线索 | 在解法中的用途 |
| --- | --- |
| "$c_1,c_2$ 不能被破坏" | $s,t$ 的内部边容量取 $\infty$（§2.4，否则答案退化为 $1$） |
| "电脑 $c_1$ 和 $c_2$ 不会直接相连" | 保证 $k^\*\le\deg(s)\le N-1<\mathsf{inf}$，最小割不含 $\infty$ 边（引理 C）、还原出的 $C$ 大小恰等于割值（引理 E）；也是"必有无解之外的合法答案"的根据（例 1 演示违反后果） |
| "连接没有重复…两台电脑之间至多有一条连接" | 无重边 ⇒ 删邻居集 $N(s)$ 的容量恰为 $\deg(s)$，不必讨论平行边；也排除自环（自环即便出现也只是 $\mathit{out}(v)\to\mathit{in}(v)$，永不帮助连通性，无害） |
| "均为双向的" | 每条原边要拆成**两条**有向 $\infty$ 弧（§2.4 建图第 2 步 + 坑点 ①） |
| "一行，一个整数…" | 输出只需 $k^\*$；具体方案在 §2.6 完整证明后以注释块保留（USACO 原版第二行） |
| "$1\le N\le100,\,1\le M\le600$" | 拆点后 $2N\le 200$ 点、$N+2M\le 1300$ 条有向弧 ⇒ Dinic 通界 $O(V^2E)\approx10^8$ 但实际远小于此（复杂度见 Part 4） |


## Part 2 — 思维链（含完整证明）

### 2.1 先把机制翻译成图论语言

"通路 $a_1,\dots,a_c$ 逐边相连"就是路径；"踩坏电脑"就是删点。目标 = 删最少的点（不许碰 $s=c_1,t=c_2$）使 $s,t$ 分处不同连通块 = **最小 $s$-$t$ 点割**。

暴力不行：$N\le 100$，枚举删除集合是 $2^{100}$；即便枚举"只删 $k$ 个"，$k$ 上界是 $\deg(s)\le 99$，组合数仍然天文数字。必须找结构。

### 2.2 朴素贪心为什么错（找失败模式）

自然的贪心："找一条 $s$-$t$ 路径，删掉它中间某个点，重复直到不连通"。反例（$s=1,t=8$）：边集
$$\{1\!-\!4,4\!-\!5,5\!-\!2,2\!-\!8,\;1\!-\!6,6\!-\!7,7\!-\!3,3\!-\!8\}$$
即两条长链 $1\!-\!4\!-\!5\!-\!2\!-\!8$ 与 $1\!-\!6\!-\!7\!-\!3\!-\!8$。最优答案是 $2$（删 $\{2,3\}$，也就是 $t$ 的两个邻居）。而"每次删路径中点"的贪心：第一条路径删 $5$，第二条 $1\!-\!4\!-\!2\!-\!8$ 删 $4$，第三条 $1\!-\!6\!-\!7\!-\!3\!-\!8$ 删 $7$，再删 $6$——共 $4$ 次。

**失败原因**：贪心每次只看一条路径，删掉的点对"其他路径"可能毫无贡献，它无法感知"全局瓶颈层"。正确的下界来源应该是**若干条互不相交的路径**：若存在 $k$ 条内部点两两不交的 $s$-$t$ 路径，任何合法删除方案必须**每条至少删一个不同的点**，故 $\ge k$。这正是 **Menger 定理**：最小点割大小 $=$ 内部点不相交 $s$-$t$ 路径的最大条数。于是问题变成"最多能塞几条点不相交的路径"——这就是**流**！（积分流分解成路径 + 容量 $1$ 的内部边保证"一个点只被一条路径用"。）

### 2.3 先解掉被剥弱的版本：删边

若代价在**边**上（删最少条链路），就是标准**最小 $s$-$t$ 边割**：无向图每条边按两个方向入流网络、容量 $1$，跑最大流即可。本题差距在于**代价挂在点上**，而最大流的容量只挂在边上。这个差距要用"拆点"补齐。

### 2.4 拆点归约：点容量 $\to$ 边容量（本题核心）

**建图**（内部编号一律 $0$-based，$v\in[0,N)$；约定 $\mathit{in}(v)=2v$、$\mathit{out}(v)=2v+1$）：

1. 每个点 $v$ 拆成 $\mathit{in}(v)\to\mathit{out}(v)$，容量
$$c(\mathit{in}(v),\mathit{out}(v))=\begin{cases}\infty, & v=s\ \text{或}\ v=t\\ 1, & \text{否则}\end{cases}$$
2. 每条无向原边 $(u,v)$ 换成两条有向边 $\mathit{out}(u)\to\mathit{in}(v)$、$\mathit{out}(v)\to\mathit{in}(u)$，容量都是 $\infty$。
3. 源 $S=\mathit{in}(s)$，汇 $T=\mathit{out}(t)$。跑最大流（Dinic），流值即答案。

**拆点后到底长什么样**（就拿题面描述里那张 $1\!-\!3\!-\!2$ 的图：电脑 $1$ 是 $s$（$v=0$）、电脑 $2$ 是 $t$（$v=1$）、电脑 $3$ 是 $v=2$；下文 $\mathit{in}(v)/\mathit{out}(v)$ 的自变量一律用 $0$-based 编号，电脑编号 $x$ 对应 $v=x-1$）：

```plain
内部边   v=0(电脑1=s): in=0 --inf--> out=1
         v=1(电脑2=t): in=2 --inf--> out=3
         v=2(电脑3)  : in=4 --  1 --> out=5      容量 1 = "这台电脑最多坏一次"
原边弧   1-3: out(0)=1 --inf--> in(2)=4 ,  out(2)=5 --inf--> in(0)=0
         2-3: out(1)=3 --inf--> in(2)=4 ,  out(2)=5 --inf--> in(1)=2
源汇     S = in(0) = 0 ,  T = out(1) = 3
唯一的 S->T 路径 0->1->4->[买票:容量1]->5->2->3，故答案 1（电脑3）
```

一条 $S\to T$ 的路径必须形如 $S\to\mathit{out}(s)\to\mathit{in}(u_1)\to\mathit{out}(u_1)\to\cdots\to\mathit{in}(t)\to T$：**每进入一个点就必须买它的"门票"**（容量 $1$ 的内部边），这就是"点数"被翻译成"边容量"的关键；门票只能买一次（一条边只被割一次），所以同一个点不会被重复计价。注意 $\mathit{out}(u)\to\mathit{in}(v)$ 是**有向**的，一条无向边要写两次 `addedge`；若只写一次（上图只剩 $\mathit{out}(0)\!\to\!\mathit{in}(2)$ 与 $\mathit{out}(1)\!\to\!\mathit{in}(2)$，两个反边容量都是 $0$），则 $S$ 一侧根本走不到 $T$，最大流 $=0$，答案错成 $0$（正确是 $1$）。

记拆点后的图为 $G'$，原图为 $G$。下证"最小割值 $=$ 最小点数"。先用一个引理把 $\infty$ 钉死。

**引理 C（$\infty$ 边进不了最小割）.** 取 $\mathsf{inf}>N-1$（代码里 $\mathsf{inf}=1145141919$）。设 $s,t$ 不相邻（题面保证），则 $G'$ 的最小割值 $k^\*\le\deg(s)\le N-1<\mathsf{inf}$。

*证明.* 取 $C=N_G(s)$（$s$ 的全部邻居）。因 $s,t$ 不相邻，$t\notin C$；且 $s\notin C$（简单图无自环），故 $C$ 是**合法**删除集。删掉 $C$ 后 $s$ 无任何邻边，与 $t$ 不连通，所以"最小删除数"$\le\deg(s)$。下面在 $G'$ 中造一个容量恰为 $\deg(s)$ 的割：此时 $G-C$ 中从 $s$ 可达的点集就是 $R=\{s\}$，取源侧 $A=\{\mathit{in}(s),\mathit{out}(s)\}\cup\{\mathit{in}(v):v\in C\}$。逐类数跨界弧：(i) 内部边——$\mathit{in}(s)\to\mathit{out}(s)$ 两端都在 $A$ 不跨界；$v\in C$ 时 $\mathit{in}(v)\in A$ 而 $\mathit{out}(v)\notin A$（$C$ 中的点不在 $R$ 里），各贡献容量 $1$，共 $\deg(s)$ 条；其余点 $\mathit{in}\notin A$ 不跨界。(ii) 原边弧 $\mathit{out}(u)\to\mathit{in}(w)$——尾点在 $A$ 的只有 $u=s$，而其终点 $\mathit{in}(w)$ 满足 $w\in C$，仍在 $A$ 内，不跨界。于是 $\mathrm{cap}(A)=\deg(s)$，故 $k^\*\le\deg(s)\le N-1<\mathsf{inf}$；而任何含 $\infty$ 跨界边的割容量 $\ge\mathsf{inf}>k^\*$，所以**最小割必不含 $\infty$ 边**。$\square$

**引理 A（删除集 $\Rightarrow$ 割，$\mathrm{cap}=|C|$ 恰好）.** 设 $C\subseteq V\setminus\{s,t\}$ 使 $G-C$ 中 $s,t$ 不连通，则 $G'$ 存在容量**恰为** $|C|$ 的 $S$-$T$ 割。

*证明.* 令 $R=\{v\in V:$ 在 $G-C$ 中 $s$ 到 $v$ 可达$\}$，则 $s\in R$、$t\notin R$。取源侧
$$A=\{\mathit{in}(v),\mathit{out}(v):v\in R\}\cup\{\mathit{in}(v):v\in C\}.$$
逐类检查跨界的有向边：

- 内部边 $\mathit{in}(v)\to\mathit{out}(v)$：$v\in R$ 时两端都在 $A$；$v\in C$ 时 $\mathit{in}(v)\in A$，而 $\mathit{out}(v)\notin A$（$R$ 是 $G-C$ 的点集，故 $R\cap C=\varnothing$，$\mathit{out}(v)$ 只可能随 $R$ 入 $A$），**恰好跨界**、容量 $1$；其余 $v\notin R\cup C$ 的 $\mathit{in}(v)\notin A$，弧尾点不在 $A$，不跨界。
- 原边映成的 $\mathit{out}(u)\to\mathit{in}(w)$（$u,w$ 在原图相邻）：若 $\mathit{out}(u)\in A$，则只有 $u\in R$ 一种可能（$C$ 中的 $\mathit{out}$ 不在 $A$）；而 $R$ 在 $G-C$ 中对邻边封闭，故 $w\in R$ 或 $w\in C$，两种情形都有 $\mathit{in}(w)\in A$，边不跨界。

于是跨界边**恰为** $\{\mathit{in}(v)\to\mathit{out}(v):v\in C\}$，共 $|C|$ 条、每条容量 $1$，故 $\mathrm{cap}(A)=|C|$（不多不少）。又 $S=\mathit{in}(s)\in A$（$s\in R$）、$T=\mathit{out}(t)\notin A$（$t\notin R$，且 $t\notin C$ 而 $\mathit{out}(\cdot)$ 只随 $R$ 进 $A$）。故这是合法的 $S$-$T$ 割，并且容量恰等于删除集大小。$\square$

**引理 B（割 $\Rightarrow$ 删除集，等号）.** 设 $A$ 是 $G'$ 的**最小**割（$S\in A$，$T\notin A$），由引理 C 它不含 $\infty$ 跨界边。令
$$C=\{v\in V\setminus\{s,t\}:\mathit{in}(v)\in A,\ \mathit{out}(v)\notin A\}.$$
则 $C$ 是 $G$ 中合法的 $s$-$t$ 分离集，且 $|C|=\mathrm{cap}(A)$。

*证明.* 先定端点归属：$\mathit{in}(s)=S\in A$，而 $\mathit{in}(s)\to\mathit{out}(s)$ 容量 $\infty$，若 $\mathit{out}(s)\notin A$ 则该 $\infty$ 边跨界，矛盾，故 $\mathit{out}(s)\in A$。同理 $T=\mathit{out}(t)\notin A$。

再定容量：跨界边只能是容量 $1$ 的内部边（引理 C），容量 $1$ 的内部边正是 $v\notin\{s,t\}$ 的那些，跨界当且仅当 $\mathit{in}(v)\in A\wedge\mathit{out}(v)\notin A$，即 $v\in C$。注意 $\mathit{in}(t)\to\mathit{out}(t)$ 不可能"从 $A$ 指向外"造成额外计数——若 $\mathit{in}(t)\in A$ 则它是 $\infty$ 边跨界（因为 $\mathit{out}(t)\notin A$），已被排除，所以 $\mathit{in}(t)\notin A$。故
$$\mathrm{cap}(A)=\sum_{v\in C}1=|C|.$$
最后证 $C$ 真的断开 $s,t$。反设 $G-C$ 中有路径 $x_0=s,x_1,\dots,x_\ell=t$。归纳：$\mathit{out}(x_0)=\mathit{out}(s)\in A$；若 $\mathit{out}(x_i)\in A$，则 $\infty$ 边 $\mathit{out}(x_i)\to\mathit{in}(x_{i+1})$ 不能跨界，故 $\mathit{in}(x_{i+1})\in A$；又 $x_{i+1}\notin C$（路径避开 $C$），"$\mathit{in}\in A$ 而 $\mathit{out}\notin A$"不成立，故 $\mathit{out}(x_{i+1})\in A$。递推到 $i=\ell-1$ 得 $\mathit{in}(t)=\mathit{in}(x_\ell)\in A$，与前面 $\mathit{in}(t)\notin A$ 矛盾。故 $C$ 是合法删除集，且 $|C|=\mathrm{cap}(A)=k^\*$。$\square$

**推论（值相等）.** 记最小删除集大小为 $\kappa$，$G'$ 的最小割值为 $k^\*$。引理 A 说明每个删除集都给出容量同为 $|C|$ 的割，故 $k^\*\le\kappa$；引理 B 说明最小割给出大小同为 $\mathrm{cap}(A)$ 的删除集，故 $\kappa\le k^\*$。于是 $\kappa=k^\*=$ 最大流值（最大流最小割定理）。这同时给出更强的结论：两个方向都是**等号**（容量与点数一一对应，没有"多算/少算"的余地）。$\square$

**容量 $1$ 对应"至多删一次".** 一个点 $v$ 在任意割里至多贡献它自己那一条内部边 $\mathit{in}(v)\to\mathit{out}(v)$ 一次，容量 $1$ 恰好等于"这台电脑要么坏、要么不坏，代价一次"；不可能出现"删半个点"或"删两次"，因为割是按**边集**定义的、每条边只计一次容量。反过来，删一台电脑也只需付 $1$。双向对齐，故流网络里的"割容量"与题意"坏掉的电脑数"完全等量。

**$s,t$ 的内部边必须是 $\infty$.** 若给它们也设容量 $1$，则割 $\{\mathit{in}(s)\to\mathit{out}(s)\}$ 容量只有 $1$；只要 $s,t$ 原本连通，最小割就被压成 $1$（等价于"把 $c_1$ 自己踩坏"），同时任何一条 $S$-$T$ 路径都过 $\mathit{in}(s)\to\mathit{out}(s)$，最大流也恒为 $1$——输出 $1$，违反题面 $c_1,c_2$ 不可破坏。具体反例（此处电脑编号按题面 $1$-based 叙述，$s$ 即电脑 $1$）：$N=4$，边 $1\!-\!3,3\!-\!2,1\!-\!4,4\!-\!2$，$s=1,t=2$，正确答案 $2$（删 $\{3,4\}$，两条点不交路径 $1\!-\!3\!-\!2$、$1\!-\!4\!-\!2$ 说明 $<2$ 不可行）；若 $s$ 的内部容量取 $1$，输出会是 $1$，错。取 $\infty$ 后 $s,t$ 自身永不被割（引理 B 首段已严格说明），而每条 $S$-$T$ 路径仍必须穿过某个容量 $1$ 的内部边（因为 $s,t$ 不相邻，见 §2.5），故流值仍有限。

### 2.5 最大流的实际含义（Menger 的另一面）+ 为什么不会爆

Dinic 每轮增广的瓶颈是某条容量 $1$ 的内部边，推的是整数流（容量全整数 ⇒ 算法保持积分性），故最终流可分解成 $k^\*$ 条**内部点不重**的 $S$-$T$ 路径，对应原图 $k^\*$ 条内部点不相交的 $s$-$t$ 路径——这就是 Menger 定理的构造性证明。同时说明**答案 $k^\*\le N-1$ 一定存在**：$s$ 的邻居集就是合法删除集（引理 C），因此"无解"在本数据约定下不会发生。

关于"唯一会出 $\infty$ 的情形"：若 $s,t$ **直接相邻**（题面禁止，但值得知道机制），$G'$ 里存在只含 $\infty$ 边的路径 $S\to\mathit{out}(s)\to\mathit{in}(t)\to T$，其瓶颈 $=\mathsf{inf}$，最大流 $\ge\mathsf{inf}$，割值也 $\ge\mathsf{inf}$——这正是 USACO 原版输出 `IMPOSSIBLE` 的情形（删任何中间电脑都断不掉直连，例 1 手算演示）。反过来，除这条直连弧之外，任何**简单** $S$-$T$ 路径都至少穿过一条容量 $1$ 的内部边：路径形如 $S,\mathit{out}(s),\mathit{in}(u_1),\mathit{out}(u_1),\dots,\mathit{in}(t),T$，从 $\mathit{in}(\cdot)$ 走到 $\mathit{out}(\cdot)$ 只能用内部边，而容量 $\infty$ 的内部边只有 $s,t$ 两条；简单路径不会重复经过 $s$（它的 $\mathit{in}\to\mathit{out}$ 已在起点用掉），一旦到达 $t$ 就是终点方向，所以中途必然踩到某个 $v\notin\{s,t\}$ 的容量 $1$ 内部边。于是合法数据下**每次增广的瓶颈恰为 $1$**，流值 $=k^\*\le N-1$，所有 $\mathsf{int}$ 量的逐项溢出核算见 §2.7。

### 2.6 方案还原：残量网络上的可达集（完整证明）

求完最大流后，在**残量网络**上从 $S$ 做遍历得可达集 $R'$，令
$$C=\{v\in V\setminus\{s,t\}:\mathit{in}(v)\in R',\ \mathit{out}(v)\notin R'\}.$$
这就是一个具体的最小点割（编号升序输出即可）。逐条证明它满足要求。

**引理 D（可达集给出最小割）.** 流最大 $\iff$ 残量网络无 $S\to T$ 路径，此时 $T\notin R'$，且
$$\text{流值}\;=\;\mathrm{cap}(R',\overline{R'}).$$
*证明.* 流值 $=\sum_{\mathit{from}\in R',\,\mathit{to}\notin R'}f(e)-\sum_{\mathit{from}\notin R',\,\mathit{to}\in R'}f(e)$（对任意 $S$-$T$ 划分，净跨界流量都等于流值，由流守恒逐点求和、内部贡献成对抵消）。若 $a\in R',b\notin R'$ 且 $\mathit{res}(a\to b)>0$，则 $b$ 从 $S$ 可达，矛盾——故所有 $R'\to\overline{R'}$ 的弧都饱和，$f(e)=\mathrm{cap}(e)$。若 $a\notin R',b\in R'$ 且 $f(a\to b)>0$，则反向残量弧 $b\to a$ 容量 $>0$，于是 $a$ 可达，矛盾——故这部分 $f=0$。代入得流值 $=\sum_{R'\to\overline{R'}}\mathrm{cap}(e)=\mathrm{cap}(R',\overline{R'})$。又"割容量 $\ge$ 流值"恒成立（任何割都界住流），故 $R'$ 是**最小割**。$\square$

**引理 E（$R'$ 的跨界边全是容量 $1$ 的内部边）.** 合法数据（$s,t$ 不相邻）下流值 $k\le N-1$，于是任意弧上的流量 $\le k<\mathsf{inf}$，故每条 $\infty$ 弧的**残量** $\ge\mathsf{inf}-k>0$（正向残量 $=\mathsf{inf}-f>0$）。因此：

- 若 $\mathit{out}(u)\in R'$ 且 $(u,v)$ 是原边，则弧 $\mathit{out}(u)\to\mathit{in}(v)$ 残量 $>0$，故 $\mathit{in}(v)\in R'$。
- $S=\mathit{in}(s)\in R'$，内部边 $\mathit{in}(s)\to\mathit{out}(s)$ 残量 $>0$ $\Rightarrow\mathit{out}(s)\in R'$；即 $s\notin C$。
- $\mathit{in}(t)\notin R'$：否则 $S\to\cdots\to\mathit{in}(t)$ 是增广路（且 $\mathit{in}(t)\to\mathit{out}(t)=T$ 残量 $>0$ 使 $T\in R'$），与"流最大即 $T\notin R'$"矛盾。

所以 $R'\to\overline{R'}$ 的跨界弧只能是 $v\notin\{s,t\}$ 的内部边 $\mathit{in}(v)\to\mathit{out}(v)$，且它们饱和（残量 $0$、容量 $1$）。$\square$

**定理（还原出的 $C$ 恰为最小点割）.** 由引理 E，$\mathrm{cap}(R')=|C|\cdot 1$；由引理 D，$|C|=\text{流值}=k$（**大小恰等于割值**，因为每条跨界弧容量恰为 $1$，没有 $\infty$ 弧混入，也没有"一条弧计多次"）。由引理 B（把 $A$ 换成 $R'$，其前提"无 $\infty$ 跨界弧"由引理 E 保证，且 $\mathit{out}(t)\notin R'$：进入 $\mathit{out}(t)$ 的弧只有 $\mathit{in}(t)\to\mathit{out}(t)$，而 $\mathit{in}(t)\notin R'$），$C$ 删除后 $s,t$ 确实不连通，且 $C$ 不含 $s,t$（$\mathit{out}(s)\in R'$ 使 $s$ 不满足"$\mathit{in}\in R',\mathit{out}\notin R'$"；$t$ 由定义排除，并且上面已证 $\mathit{in}(t),\mathit{out}(t)\notin R'$）。又 $|C|=k$ 等于最小值，故 $C$ 是一个**最小点割**。$\square$

**引理 F（$R'$ 是最小割里源侧最小者 ⇒ 方案确定）.** 设 $(A,\overline{A})$ 是任一最小割（$S\in A$，$T\notin A$），则 $R'\subseteq A$。

*证明.* 记流值为 $\mathsf{val}$。对任意 $S$-$T$ 割 $A$，由流守恒逐点求和（内部贡献成对抵消）得
$$\mathsf{val}=\sum_{A\to\overline{A}}f-\sum_{\overline{A}\to A}f\le\sum_{A\to\overline{A}}\mathrm{cap}-0=\mathrm{cap}(A).$$
若 $A$ 还是最小割，则 $\mathrm{cap}(A)=\mathsf{val}$（最大流最小割定理），上面每一步都必须是等号：$\sum_{\overline{A}\to A}f=0$（故每条 $\overline{A}\to A$ 的弧流量为 $0$），且每条 $A\to\overline{A}$ 的弧满足 $f=\mathrm{cap}$（饱和、残量 $0$）。于是残量网络里不存在"从 $A$ 指向 $\overline{A}$"的正残量弧——$A\to\overline{A}$ 的弧残量 $0$，而 $\overline{A}\to A$ 的弧其反向残量也因 $f=0$ 而不构成离开 $A$ 的弧。故从 $S\in A$ 出发的任意残量路径始终留在 $A$ 内，即 $R'\subseteq A$。$\square$

推论：$R'$ 自己是最小割（引理 D）且包含于每个最小割的源侧，故它是**唯一确定**的源侧最小割，于是代码还原出的 $C$ 不依赖增广顺序（Dinic 的弧序只会影响 `cur` 的推进，不影响最终残量可达集），输出按 $v=0\dots N-1$ 扫描天然升序。同时因为 $\mathit{in}(t),\mathit{out}(t)$ 属于每个最小割的汇侧，$t\notin C$；$s$ 由 $\mathit{out}(s)\in R'$ 亦不会被计入。

直觉：$R'$ 是"从 $s$ 一侧推到底"的那版最小割，所以它总是贴着 $s$ 的那一层（例 2 选电脑 $2$ 而不选电脑 $3$，例 3 选左层 $\{2,3\}$ 而不选右层 $\{4,5\}$）。


### 2.7 $\mathsf{inf}$ 的取值与溢出核算

需要 $\mathsf{inf}>\deg(s)$，即 $\mathsf{inf}\ge N$ 就够（引理 C）。取 $\mathsf{inf}=1145141919$（114514 家族哨兵），显式不等式：
$$k^\*\le\deg(s)\le N-1\le 99\ll 1145141919=\mathsf{inf}.$$
溢出核查（全部 $\mathsf{int}$）：

- 单边容量 $\mathsf{inf}=1145141919<2^{31}-1=2147483647$。✓
- 正向残量 $\mathsf{inf}-f$：$f\le\mathsf{inf}$，$\ge 0$。✓
- 反向边 `E[i^1].w+=pushed`：单条弧上的流量 $\le$ 该弧容量 $\mathsf{inf}$，累加不会超过 $\mathsf{inf}$。✓
- 流值累加 `flow+=pushed`：**任何** $S$-$T$ 路径都必须经过 $\mathit{in}(s)\to\mathit{out}(s)$（容量 $\mathsf{inf}$）与 $\mathit{in}(t)\to\mathit{out}(t)$（容量 $\mathsf{inf}$），这两条边各自界住了总流量，故恒有 $\mathit{flow}\le\mathsf{inf}=1145141919<2^{31}-1=2147483647$；合法数据（$s,t$ 不相邻）下更强：$\mathit{flow}=k^\*\le N-1=99$。✓
- `dfs(S,inf)` 的 $\min(\mathit{cap},\text{剩余})$ 全程只减不增。✓

### 2.8 手算验证

**例 0（题面描述的 3 点图；本地无官方样例，只能用它对标语义）**：`3 2 1 2`，边 $1\!-\!3$、$2\!-\!3$。$0$-based：$s=0,t=1$，原边 $(0,2),(1,2)$。$G'$：内部边 $0\!\to\!1(\infty),\,2\!\to\!3(\infty),\,4\!\to\!5(1)$；原边弧 $1\!\to\!4(\infty),5\!\to\!0(\infty),3\!\to\!4(\infty),5\!\to\!2(\infty)$。$S=0,T=3$。
第 1 轮 BFS 层：$0{:}0,1{:}1,4{:}2,5{:}3,2{:}4,3{:}5$；增广路 $0\!\to\!1\!\to\!4\!\to\!5\!\to\!2\!\to\!3$，瓶颈 $=\min(\infty,\infty,1,\infty,\infty,\infty)=1$，推 $1$，$4\!\to\!5$ 饱和。流 $=1$。
第 2 轮 BFS：$0,1,4$ 可达，$4\to 5$ 残量 $0$，到不了 $T$，停。
可达集 $R'=\{0,1,4\}$，跨界内部边只有 $4\!\to\!5$ ⇒ $C=\{v\!=2\}=\{$电脑 $3\}$，$|C|=1=$ 流值 ✓。删电脑 $3$ 后 $1,2$ 确实断开（$1$ 的邻居只剩 $3$）。与题面"如果电脑 3 坏了，电脑 1 与 2 便不能互发信息"一致。

**例 1（$s,t$ 直连：检验机制与"为什么题面禁止"）**：`3 3 1 2`，边 $1\!-\!2,1\!-\!3,3\!-\!2$。$G'$：$s=0$（$\infty$）、$t=1$（$\infty$）、$v_2=2$（容量 $1$，即 $4\!\to\!5$）；弧 $1\!\to\!2$（直连边，$\infty$）、$3\!\to\!0$、$1\!\to\!4$、$5\!\to\!0$、$5\!\to\!2$、$3\!\to\!4$。$S=0,T=3$。
第 1 轮层图含路径 $0\!\to\!1\!\to\!2\!\to\!3$（全是 $\infty$ 边），瓶颈 $\mathsf{inf}$，一次推满 $\Rightarrow\mathit{flow}=\mathsf{inf}$；随后 $\mathit{out}(s)\!\to\!\mathit{in}(t)$ 与 $\mathit{in}(t)\!\to\!\mathit{out}(t)$ 均饱和，任何绕 $v_2$ 的路径都进不了 $t$，无更多增广。$\mathit{flow}\ge\mathsf{inf}$ 就是"点删不开"的信号（USACO 原版的 `IMPOSSIBLE`）。现实中删电脑 $3$ 后直连仍在，确实无解 ✓——题面"c1 和 c2 不会直接相连"这条保证让答案恒 $\le N-1$，代码不必特判。

**例 2（链，割不唯一但还原唯一）**：`4 3 1 4`，边 $1\!-\!2,2\!-\!3,3\!-\!4$。$G'$：内部 $0\!\to\!1(\infty),2\!\to\!3(1),4\!\to\!5(1),6\!\to\!7(\infty)$；弧 $1\!\to\!2,3\!\to\!0,3\!\to\!4,5\!\to\!2,5\!\to\!6,7\!\to\!4$。$S=0,T=7$。
第 1 轮唯一路径 $0\!\to\!1\!\to\!2\!\to\!3\!\to\!4\!\to\!5\!\to\!6\!\to\!7$，瓶颈 $1$，推 $1$ ⇒ $2\!\to\!3$ 与 $4\!\to\!5$ 都饱和。第 2 轮：从 $0$ 只到 $\{0,1,2\}$，$2\!\to\!3$ 残量 $0$ ⇒ 不可达 $T$，$\mathit{flow}=1$。
$R'=\{0,1,2\}$ ⇒ 跨界内部边 $2\!\to\!3$ ⇒ $C=\{$电脑 $2\}$。本题最优单点割有 $\{2\}$ 与 $\{3\}$ 两个（不能删 $1,4$ 自身），算法**确定性地选出贴 $s$ 的那个**（引理 F）；删电脑 $2$ 后 $1$ 与 $4$ 断开 ✓，$|C|=1=\mathit{flow}$ ✓。

**例 3（两层完全二部：两个平行候选割）**：`6 8 1 6`，边 $1\!-\!2,1\!-\!3,2\!-\!4,3\!-\!4,2\!-\!5,3\!-\!5,4\!-\!6,5\!-\!6$。左割 $\{2,3\}$、右割 $\{4,5\}$ 都是答案 $2$；且 $1\!-\!2\!-\!4\!-\!6$ 与 $1\!-\!3\!-\!5\!-\!6$ 是两条点不交路径，故 $k\ge2$、$k=2$。
$0$-based 弧表（$S=0,T=11$）：内部 $0\!\to\!1(\infty),2\!\to\!3(1),4\!\to\!5(1),6\!\to\!7(1),8\!\to\!9(1),10\!\to\!11(\infty)$；原边弧 $1\!\to\!2,3\!\to\!0,1\!\to\!4,5\!\to\!0,3\!\to\!6,7\!\to\!2,5\!\to\!6,7\!\to\!4,3\!\to\!8,9\!\to\!2,5\!\to\!8,9\!\to\!4,7\!\to\!10,11\!\to\!6,9\!\to\!10,11\!\to\!8$。
第 1 轮 BFS 层：$0{:}0,1{:}1,2/4{:}2,3/5{:}3,6/8{:}4,7/9{:}5,10{:}6,11{:}7$。DFS 沿当前弧：
- 路径 1 $0\!\to\!1\!\to\!2\!\to\!3\!\to\!6\!\to\!7\!\to\!10\!\to\!11$，瓶颈 $1$（$2\!\to\!3$ 与 $6\!\to\!7$），饱和这两条；
- 路径 2 $0\!\to\!1\!\to\!4\!\to\!5\!\to\!8\!\to\!9\!\to\!10\!\to\!11$（注意 $5\!\to\!6$ 已因 $6\!\to\!7$ 饱和而枯竭，DFS 回溯后改走 $5\!\to\!8$），瓶颈 $1$，饱和 $4\!\to\!5$、$8\!\to\!9$；
- 再次从 $S$ 出发：$1\!\to\!2$ 后 $2\!\to\!3$ 残量 $0$；$1\!\to\!4$ 后 $4\!\to\!5$ 残量 $0$ ⇒ 阻塞流 $=2$。
（第 1 轮里 DFS 的具体分支顺序依赖表头插入序（LIFO），上面按"先试 $5\!\to\!6$ 失败再退到 $5\!\to\!8$"的顺序推演；换任何顺序，两条容量 $1$ 内部边集 $\{2\!\to\!3,4\!\to\!5\}$ 与 $\{6\!\to\!7,8\!\to\!9\}$ 的配对只会是这两种之一，流值仍为 $2$。）
第 2 轮 BFS：$\{0,1,2,4\}$ 可达，$T$ 不可达 ⇒ $\mathit{flow}=2$ ✓（与 Menger 下界吻合）。
$R'=\{0,1,2,4\}$：跨界内部边 $2\!\to\!3$（$v=1$，电脑 $2$）与 $4\!\to\!5$（$v=2$，电脑 $3$）⇒ $C=\{2,3\}$，恰为**左层**那个割，$|C|=2=\mathit{flow}$ ✓。删电脑 $2,3$ 后电脑 $1$ 无邻居，$1,6$ 断开 ✓；升序输出 `2 3`。（若沿另一方向把流推到右层，$R'$ 也一样是源侧最小的 $\{0,1,2,4\}$，这正是引理 F 保证的顺序无关性。）

**例 4（$s,t$ 本就不连通：答案 $0$）**：`4 2 1 4`，边 $1\!-\!2,3\!-\!4$。第 1 轮 BFS 就到不了 $T$，$\mathit{flow}=0$；$R'=\{0,1,2,3\}$（电脑 $1,2$ 的 $\mathit{in},\mathit{out}$ 全在 $R'$ 内），$R'$ 向外出发的弧（如 $3\!\to\!0$）都指回 $R'$，故 $\mathrm{cap}(R')=0$ ⇒ $C=\varnothing$，$|C|=0=\mathit{flow}$ ✓。电脑 $3$ 的内部边 $4\!\to\!5$ 残量仍是 $1$（未饱和），且 $\mathit{in}=4\notin R'$，两个条件都不满足 ⇒ 不会被误计入 $C$——这体现还原判据"只认 $R'\to\overline{R'}$ 的饱和弧"（引理 D/E）而不是"看有没有流量"。

以上 5 组（题面描述图、非法直连、链、平行割、$0$ 割）全部为**逐轮手算**，核对三件事：$\mathit{flow}=$ 手推的最小删除数、$|C|=\mathit{flow}$、$s,t\notin C$ 且删 $C$ 后 $s,t$ 真断开；**未做随机对拍与测速**（环境无编译器，按要求不运行任何代码）。

**退化情形清单**（题面已排除或代码自然处理）：

- $s,t$ 已不连通 / $s$ 孤立：$\mathit{flow}=0$，$C=\varnothing$，输出 $0$（例 4）。
- $s,t$ 直接相邻：题面禁止；若真出现，$\mathit{flow}\ge\mathsf{inf}$，对应 USACO 原版的 `IMPOSSIBLE`（例 1），代码可加 `if(flow>=inf)` 特判。
- $c_1=c_2$（含 $N=1$）：$S=\mathit{in}(s),T=\mathit{out}(s)$ 之间是容量 $\mathsf{inf}$ 的内部边，$\mathit{flow}=\mathsf{inf}$；题面"两头奶牛""$c_1,c_2$ 不会直接相连"已隐含 $c_1\ne c_2$，故不出现。
- 自环（题面未逐字排除但"两台电脑之间的连接"不会自环）：映射成 $\mathit{out}(v)\to\mathit{in}(v)$，容量 $\mathsf{inf}$，它让路径往回走，对可达性无帮助，无害。

### 2.9 其它可行思路（对比用，不写代码）

- **Menger + 反复找点不交路径**：容量 $1$ 的内部边使每次增广瓶颈恰为 $1$，Ford–Fulkerson（BFS/DFS 找路）$\le k\le 99$ 轮即终止，$O(k\cdot E')\approx 99\times 2600\approx 2.6\times10^5$，其实也够；Dinic 是自家模板，层图 + 当前弧更稳，直接沿用。
- **枚举答案集大小 + 网络流判定**：无必要，最大流已给最小值。
- **删点转删边的另一种做法**（对每条原边拆成两条、点权转边权）与拆点等价，只是常数更大。

## Part 3 — 模式总结（按真实思考顺序）

- **explore its property** — "踩坏电脑 $\equiv$ 删点并连带删其所有边"，目标是分离指定的 $s,t$，$s,t$ 本身不可删 ⇒ 最小 $s$-$t$ **点割**。
- **find the failure pattern of greedy** — "每找一条路径删一个点"可能删 $4$ 个而最优只要 $2$ 个：贪心看不见全局瓶颈层；下界必须来自"点不交路径条数"（Menger）。
- **consider easier version** — 把点权换成边权（删边）就是最小边割 $=$ 最大流，直接可解；差距在于容量只能挂在边上。
- **classic problem solution** — **拆点归约**：$v\to\mathit{in}(v),\mathit{out}(v)$，内部边容量 $1$（$s,t$ 取 $\infty$），原边 $\mathit{out}(u)\to\mathit{in}(v)$ 容量 $\infty$；引理 A/B 双向证明"删除集 ⟺ 割"，容量 $1$ 恰好实现"每个点至多删一次"。
- **bound the sentinel** — 用"$s$ 的邻居集是合法割"得 $k^\*\le\deg(s)\le N-1$，据此定 $\mathsf{inf}>N-1$，从而 $\infty$ 边进不了最小割（引理 C），并顺手做完溢出核算。
- **defer reconstruction** — 方案不靠额外搜索：最大流后在残量网络上取 $S$ 的可达集 $R'$，$C=\{v:\mathit{in}(v)\in R',\mathit{out}(v)\notin R'\}$（引理 D/E + 定理），$|C|$ 恰等于割值、不含 $s,t$、删后必断。
- **classic problem solution (2nd technique)** — **Dinic**（BFS 层图 + `cur[]` 当前弧 + `E[i^1]` 反边），$2N\le 200$ 点、$N+2M\le1300$ 弧，$O(V^2E)$ 上界约 $10^8$，实际流值 $\le 99$。

## Part 4 — 真实解法（含记号表）

**记号表**

| 符号 | 含义 | 初始化 / 范围 |
| --- | --- | --- |
| `n,m` | 电脑数、链路数 | 读入，$n\le100,m\le600$ |
| `s,t` | $c_1,c_2$，**$0$-based**（读入即 `--s;--t`） | $s\ne t$，题面保证不相邻 |
| `S,T` | 流网络源汇：$S=\mathit{in}(s)=2s$，$T=\mathit{out}(t)=2t+1$ | — |
| `nin(v)/nout(v)` | $2v$ / $2v+1$：拆点后 $v$ 的入点/出点编号 | $0\le v<n$，共 $2n$ 个点 |
| `E[]` | 边数组，`{v,w,nx}`＝终点/残量容量/同链下一条 | `addedge` 成对写，反边 `i^1` |
| `H[u]` | 点 $u$ 的链式前向星表头，$-1$ 为空 | `fill_n(H,2*n,-1)` |
| `cnt` | 下一条边的下标 | $0$ 起，上界 $2(n+2m)\le 2600$ |
| `dep[u]` | Dinic 层号 | 每轮 `fill_n(dep,2*n,-1)`，$-1$＝不在层图 |
| `cur[u]` | 当前弧，记录 $u$ 已扫到的边号 | 每轮 `memcpy(cur,H,…)` |
| `flow` | 最大流值 $=$ 最小点数 $=$ 答案 | $0$ 累加 |
| `vis[u]` | 残量网络上 $u\in R'$（$S$ 可达）＝$1$ | 全局默认 $0$，`reach(S)` 置位 |
| `ans` | 由 $R'$ 还原出的 $|C|$（可校验 $=\mathit{flow}$） | $0$ 累加 |
| `inf` | $\infty$ 容量哨兵 $1145141919$，$>N-1$ | `constexpr` |

**算法步骤**

1. 读 $n,m,s,t$ 并转 $0$-based；$S=2s$，$T=2t+1$；`fill_n(H,2*n,-1)`，`cnt=0`。
2. 建内部边：`for(i=0;i<n;++i) addedge(nin(i),nout(i),(i==s||i==t)?inf:1);`
3. 建原边弧：`for` 每条 $(u,v)$：`addedge(nout(u),nin(v),inf); addedge(nout(v),nin(u),inf);`（两条**独立**的有向 $\infty$ 弧，各自带反边；不能用一次 `addedge` 代替，见 §2.4 末段的反例与 Part 5 末尾坑点 ①）。
4. `dinic()`：`while(bfs())` 建层图并 `memcpy` 出 `cur`，`while(int p=dfs(S,inf)) flow+=p;`。
5. `reach(S)`：在残量网络（只看 `E[e].w>0`）上 DFS 标 `vis`，得 $R'$。
6. 还原并计数：`for(i=0;i<n;++i) if(vis[nin(i)]&&!vis[nout(i)]) ++ans;`（$s,t$ 结构上不会被计入；理论上 `ans==flow`）。
7. 输出 `flow`（洛谷格式只要这一个整数）。若需 USACO 原始的第二行，把步骤 6 的判定改成按 $i$ 升序输出 `i+1`，代码里已给注释块。
8. 文件 I/O：本地题面**未**要求 `freopen`，用标准输入输出；USACO 训练题的 `telecow.in/telecow.out` 以注释形式保留在代码里，需要时打开。

**复杂度**：拆点后 $V'=2n\le 200$ 个点；`addedge` 调用 $n+2m\le 1300$ 次（即 $2(n+2m)\le 2600$ 条弧记录）。Dinic 通界 $O(V'^2E')=200^2\times 2600\approx 1.04\times10^8$（最坏），本题流值 $\le N-1=99$ 且每次成功增广至少饱和一条容量 $1$ 的内部边，故实际 $O(k\cdot E')\approx 2.6\times10^5$ 级，远低于时限；空间 $O(V'+E')$，数组开销 $\approx 2600\times 12\ \text{B}\approx 31\ \text{KB}$。

**验证状态**：5 组逐轮手算（题面描述图 $=1$、非法直连 $=\infty$ 信号、链 $=1$ 且还原唯一、两层平行割 $=2$ 且还原为左层、不连通 $=0$），并核对 $|C|=\mathit{flow}$、$s,t\notin C$、删除后确实断开；**本地题面无样例，无法对标官方样例**；未运行代码、未做对拍与计时。

## Part 5 — 示例代码（C++，Louis Li 风格）

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};              // 原图点数 N<=100，+5 松弛
constexpr int mxv{2*mxn};            // 拆点后点数翻倍：2N<=200（这里 210，宁虚高不越界）
constexpr int mxm{1305};             // addedge 次数上界 N+2M<=1300（内部边 n 次 + 每条无向边 2 次）
constexpr int inf{1145141919};       // 容量 inf：必须 > N-1（见题解 §2.7）

int n,m,s,t,S,T;
struct{int v,w,nx;}E[mxm*2];         // 每条 addedge 写 2 个槽位，反边下标 i^1
int H[mxv],cnt=0;
int dep[mxv],cur[mxv],vis[mxv];

int nin(int v){return v*2;}          // v_in = 2v
int nout(int v){return v*2+1;}       // v_out = 2v+1

void addedge(int u,int v,int w)
{
	E[cnt]={v,w,H[u]};H[u]=cnt++;
	E[cnt]={u,0,H[v]};H[v]=cnt++;
}

bool bfs()
{
	fill_n(dep,2*n,-1);
	dep[S]=0;
	queue<int>Q;
	Q.push(S);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int e=H[u];~e;e=E[e].nx)
		{
			int v=E[e].v;
			if(E[e].w>0&&dep[v]<0)
			{
				dep[v]=dep[u]+1;
				Q.push(v);
			}
		}
	}
	return dep[T]>=0;
}

int dfs(int u,int f)
{
	if(u==T)
		return f;
	for(int&i=cur[u];~i;i=E[i].nx)
	{
		int v=E[i].v;
		if(E[i].w>0&&dep[v]==dep[u]+1)
		{
			int res=dfs(v,min(f,E[i].w));
			if(res)
			{
				E[i].w-=res;E[i^1].w+=res;
				return res;
			}
		}
	}
	return 0;
}

int dinic()
{
	int flow=0;
	while(bfs())
	{
		memcpy(cur,H,sizeof(int)*2*n);
		while(int pushed=dfs(S,inf))
			flow+=pushed;
	}
	return flow;
}

void reach(int u)                            // 残量网络上从 S 的可达集 R'
{
	vis[u]=1;
	for(int e=H[u];~e;e=E[e].nx)
	{
		int v=E[e].v;
		if(E[e].w>0&&!vis[v])
			reach(v);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	// freopen("telecow.in","r",stdin);freopen("telecow.out","w",stdout);   // USACO 原版文件 I/O

	cin>>n>>m>>s>>t;--s;--t;
	S=nin(s);T=nout(t);                       // 源 = s_in，汇 = t_out
	fill_n(H,2*n,-1);
	cnt=0;
	for(int i=0;i<n;++i)
		addedge(nin(i),nout(i),(i==s||i==t)?inf:1);  // s,t 不可破坏 -> 容量 inf
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		addedge(nout(u),nin(v),inf);         // 原边不可破坏 -> 容量 inf
		addedge(nout(v),nin(u),inf);
	}

	int flow=dinic();
	reach(S);
	int ans=0;
	for(int i=0;i<n;++i)
		if(vis[nin(i)]&&!vis[nout(i)])
			++ans;                            // 割集 C，升序；理论上恒有 ans==flow
	// assert(ans==flow);
	cout<<flow<<'\n';                         // 洛谷只要这一个整数
	// USACO 原版第二行（升序方案），需要时打开：
	// for(int i=0;i<n;++i)
	// 	if(vis[nin(i)]&&!vis[nout(i)])
	// 		cout<<i+1<<' ';
	// cout<<'\n';
	return 0;
}
```

坑点对照：① 无向边必须是**两条独立的有向 $\infty$ 弧**（各带自己的 $0$ 容量反边）；只调一次 `addedge(nout(u),nin(v),inf)` 时，$\mathit{out}(v)\to\mathit{in}(u)$ 方向只剩容量 $0$ 的反边，图被当成有向图，流值会偏小（§2.4 末段的反例：正确答案 $1$ 会算成 $0$）；② 全局 `int` 数组默认 $0$，`H` 必须 `fill_n(H,2*n,-1)` 才能配合 `~e` 判空，`vis` 默认 $0$ 正好表示"不可达"，`dep` 每轮重置为 $-1$；③ $s,t$ 的内部边若忘记取 `inf`，答案会退化成 $1$（§2.4 反例）；④ `dfs` 一次只回传一条路，`while(int pushed=dfs(S,inf))` 形式必须有，否则阻塞流算不完。
