# P1315 [NOIP 2011 提高组] 观光公交

> 标签：贪心 / 递推 · 难度：省选/NOI-

## 1. 重新读题

**问题重述（大白话版）**

一条直线上有 $n$ 个景点（站台），只有一辆公交车。车在第 $0$ 分钟出现在 $1$ 号站，只会往前开或者在站上停车等人，依次经过 $2,3,\dots,n$ 号站。第 $i$ 段路（$i$ 号站开到 $i+1$ 号站）要 $D_i$ 分钟。

有 $m$ 名乘客，第 $p$ 名乘客在 $T_p$ 分钟出现在 $A_p$ 号站，要坐到 $B_p$ 号站（保证 $A_p<B_p$，只坐一次车，中途不下车）。

**关键规则**：车每到一站，必须等到**所有以该站为出发站的乘客都到齐**才能发车。也就是说车的到站时刻可能早于某个乘客的到达时刻，此时车要空等。上下车瞬间完成。

乘客的旅行时间 = 到达目的站的时刻 $-$ 他出现在出发站的时刻。要求所有人的旅行时间之和最小。

手里有 $k$ 个加速器，每用一个，就把某一段 $D_i$ 减 $1$（**同一段可以重复用**，只要求减完之后 $D_i\ge 0$）。求最小总旅行时间。

**我修正过的三处理解偏差**（这些是很容易"想当然"的地方，也是本题的坑）

1. **本题没有车身容量限制**，也只有一班车：所有乘客最终都坐同一班车，车会等齐才走。所以"装不下要等下一班"这种机制在这里不存在。真正"截断"影响的是**等待点**（车到得比乘客早的那些站）。
2. **同一段路可以用多个加速器**（题面明说"对于同一个 $D_i$ 可以重复使用加速器"），只要求 $D_i\ge 0$。因此"每段路只能修一次"也是错的。
3. 目标函数不是"最后一批乘客到达终点的时间"，而是**所有乘客旅行时间之和**，所以要区分"在每一站下车的人数"。

**约束与子任务（提示复杂度）**

| 数据范围 | 规模 | 暗示 |
|---|---|---|
| 10% | $k=0$ | 只要会写"到站/离站"递推 |
| 20% | $k=1$ | 只需分析**一次**修路的收益 |
| 40% | $n\le50,\;m\le10^3,\;k\le20,\;D_i\le10,\;T_i\le500$ | 允许每次修路后重算，$O(k\cdot n\cdot m)$ 级模拟 |
| 60% | $n\le100,\;m\le10^3,\;k\le100,\;D_i\le100$ | $O(k\cdot n)$ 递推可过 |
| 100% | $n\le10^3,\;m\le10^4,\;k\le10^5,\;D_i\le100,\;T_i\le10^5$ | $k$ 太大，**必须把每一轮压成 $O(n)$ 且轮数为 $O(n)$**（批处理），即 $O(n^2)$ |

**值域与溢出**：$\mathrm{arr}[n]\le\max T+\sum D_i\le10^5+10^3\cdot100=2\times10^5$，单人旅行时间 $\le2\times10^5$，$m\le10^4$ ⇒ 总和可到 $2\times10^9$，**超过 $\mathrm{int}$ 上限 $2.147\times10^9$ 的量级**，总量必须用 `i64`；时刻、人数用 `int` 足够。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索机制 —— 车的时间表完全由"到站/离站"递推决定

记（1-based，与题面一致）：

- $\mathrm{arr}[i]$：车**到达** $i$ 号站的时刻，$\mathrm{arr}[1]=0$；
- $\mathrm{lev}[i]$：车**离开** $i$ 号站的时刻；
- $\mathrm{laji}[i]$：以 $i$ 站为出发站的乘客中最晚到达的时刻（该站没人上车则记 $0$）；
- $\mathrm{cnt}[i]$：在 $i$ 站**下车**的人数。

$$\mathrm{lev}[i]=\max(\mathrm{arr}[i],\mathrm{laji}[i]),\qquad \mathrm{arr}[i+1]=\mathrm{lev}[i]+D_i$$

目标函数：乘客 $p$ 到达目的站的时刻就是 $\mathrm{arr}[B_p]$，所以

$$\text{Total}=\sum_{p}(\mathrm{arr}[B_p]-T_p)=\sum_{i=2}^{n}\mathrm{cnt}[i]\cdot\mathrm{arr}[i]-\sum_{p}T_p$$

$\sum T_p$ 与修路方案无关 ⇒ **最小化 Total $\iff$ 最小化 $\sum_i\mathrm{cnt}[i]\cdot\mathrm{arr}[i]$**。这一步把"旅行时间之和"换成了"到站时刻的加权和"，后面全部围绕 $\mathrm{arr}[\cdot]$ 做文章。

**把递推展开（引理 1：max-plus 闭式）**

$$\mathrm{arr}[B]=\max_{1\le j\le B-1}\Big(\mathrm{laji}[j]+\sum_{r=j}^{B-1}D_r\Big)$$

**证明**：对 $B$ 归纳。$B=2$：$\mathrm{arr}[2]=\mathrm{lev}[1]+D_1=\max(\mathrm{arr}[1],\mathrm{laji}[1])+D_1=\max(0,\mathrm{laji}[1])+D_1$。因为 $\mathrm{laji}[1]\ge 0$（所有 $T_p\ge0$），故 $\mathrm{arr}[2]=\mathrm{laji}[1]+D_1$，恰为 $j=1$ 的项 ✓。
归纳步：设对 $B$ 成立。$\mathrm{arr}[B+1]=\mathrm{lev}[B]+D_B=\max(\mathrm{arr}[B],\mathrm{laji}[B])+D_B$。把 $\mathrm{arr}[B]$ 用归纳假设代入：

- $\mathrm{arr}[B]+D_B=\max_{j\le B-1}\big(\mathrm{laji}[j]+\sum_{r=j}^{B-1}D_r+D_B\big)=\max_{j\le B-1}\big(\mathrm{laji}[j]+\sum_{r=j}^{B}D_r\big)$；
- $\mathrm{laji}[B]+D_B=\mathrm{laji}[B]+\sum_{r=B}^{B}D_r$，正是 $j=B$ 的项。

两者取 $\max$ 就是 $j\le B$ 全部项的 $\max$。$\blacksquare$

> **闭式的价值**：它把"谁决定 $\mathrm{arr}[B]$"说清楚了 —— 是某个上车站 $j$ 的"乘客到齐时刻 $\mathrm{laji}[j]$"加上从 $j$ 到 $B$ 的行驶时间。**想让 $\mathrm{arr}[B]$ 变小，只能减小那些"取到最大值"的项，也就是减小位于 $j$ 与 $B$ 之间的那些 $D_r$；而如果某个 $\mathrm{laji}[j']$（$j'>x$）自己就足够大，那么修 $x$ 及以左的路一点用都没有 —— 车到 $j'$ 站照样要等。** 这就是本题真正的"截断"机制。

### 2.2 简化版（先只考虑"一次"）—— 修一段路到底帮到了谁

先定义**等待点**（含"刚好赶上"的平局）：

$$\mathcal{W}=\{i:\mathrm{arr}[i]\le\mathrm{laji}[i]\}$$

平局也要算进 $\mathcal{W}$，原因见下面引理 2 的推导（$\mathrm{arr}[i]=\mathrm{laji}[i]$ 时，$\mathrm{arr}[i]$ 减小 $1$ 后 $\mathrm{lev}[i]$ 仍然不变）。注意 $1\in\mathcal{W}$ 恒成立（$\mathrm{arr}[1]=0\le\mathrm{laji}[1]$）。

**引理 2（单段修路的影响范围）**：设当前方案下各量为 $\mathrm{arr},\mathrm{lev}$。若把路段 $x$ 减 $1$（要求 $D_x\ge1$），记 $g(x)=\min\{i\ge x+1:\mathrm{arr}[i]\le\mathrm{laji}[i]\}$，若集合为空则 $g(x)=n$。则

$$\mathrm{arr}'[i]=\mathrm{arr}[i]-1\ (x+1\le i\le g(x)),\qquad \mathrm{arr}'[i]=\mathrm{arr}[i]\ (i>x\ \text{且}\ i>g(x)),\qquad \mathrm{arr}'[i]=\mathrm{arr}[i]\ (i\le x)$$

即**收益恰好覆盖 $(x,g(x)]$ 这一段站**，因此总旅行时间减少

$$V(x)=\sum_{B=x+1}^{g(x)}\mathrm{cnt}[B]$$

**证明**：先说明非负性：把 $D_x$ 换成 $D_x-1$ 后新数组 $D'\le D$ 逐坐标成立，而对 $\mathrm{arr}[i+1]=\max(\mathrm{arr}[i],\mathrm{laji}[i])+D_i$ 做归纳即得 $\mathrm{arr}'[i]\le\mathrm{arr}[i]$、$\mathrm{lev}'[i]\le\mathrm{lev}[i]$（$\max$ 对两个变量都单调）。记 $\varepsilon_i=\mathrm{arr}[i]-\mathrm{arr}'[i]$，$\delta_i=\mathrm{lev}[i]-\mathrm{lev}'[i]$，均为非负整数。
由 $\mathrm{lev}[i]=\max(\mathrm{arr}[i],\mathrm{laji}[i])$、$\mathrm{lev}'[i]=\max(\mathrm{arr}[i]-\varepsilon_i,\mathrm{laji}[i])$ 相减得

$$\delta_i=\min\big(\varepsilon_i,\ \max(0,\ \mathrm{arr}[i]-\mathrm{laji}[i])\big)\tag{$*$}$$

即 $\mathrm{arr}[i]>\mathrm{laji}[i]$ 时 $\delta_i=\varepsilon_i$；$\mathrm{arr}[i]\le\mathrm{laji}[i]$ 时 $\delta_i=0$（平局也算 —— 这正是必须把 $\mathrm{arr}=\mathrm{laji}$ 计入 $\mathcal{W}$ 的原因）。
$x$ 之前一切不变：$\varepsilon_i=\delta_i=0\ (i\le x)$。把 $\mathrm{arr}[i+1]=\mathrm{lev}[i]+D_i$ 与 $\mathrm{arr}'[i+1]=\mathrm{lev}'[i]+D_i-[i=x]$ 相减：
$$\varepsilon_{i+1}=\delta_i+[i=x]$$
于是 $\varepsilon_{x+1}=\delta_x+1=1$。而当 $i>x$ 时 $\varepsilon_{i+1}=\delta_i\le\varepsilon_i$（由 $(*)$），所以 $\{\varepsilon_i\}_{i\ge x+1}$ 单调不增且只取 $0/1$，一旦为 $0$ 就永远为 $0$。
再用 $(*)$：若 $\mathrm{arr}[i]>\mathrm{laji}[i]$ 则 $\delta_i=\varepsilon_i$，$1$ 继续往右传播；遇到第一个 $i\ge x+1$ 使 $\mathrm{arr}[i]\le\mathrm{laji}[i]$，该站自身仍有 $\varepsilon_i=1$（它还在连续为 $1$ 的区间内），但 $\delta_i=0\Rightarrow\varepsilon_{i+1}=0$，此后全为 $0$。这个"第一个 $i$"按定义就是 $g(x)$；若它不存在则 $\varepsilon$ 一路为 $1$ 到 $n$，即取 $g(x)=n$。$\blacksquare$

> **这条引理是全题核心**：收益不是"车上有多少人"，也不是"这段路右边还有多少人要坐车"，而是**"在 $(x,g(x)]$ 这段站内下车的人数"**，$g(x)$ 是右边第一个"车要等（哪怕刚好赶上）"的站。修路的收益会被**等待点**吃掉。

### 2.3 用引理 2 检查三个候选贪心 —— 两个是错的（反例手算）

**候选 A：每次修"全局最左"的可行路段（$D_x\ge1$）** —— 错。
**候选 B：先找最早的等待点 $p$，再修 $p$ 之后最左的可行路段** —— 错。
**候选 C：修"能影响到最早那批乘客"路径上的第一段可行路段** —— 错（这就是 @Transhumanist 的 hack 打掉的思路）。

**反例 1（手算，打 A/B/C）**：$n=4$，$D=[10,10,10]$，$k=1$。乘客：$1$ 人 $T=0,A=1,B=2$；$1000$ 人 $T=10,A=2,B=3$。
$\mathrm{laji}=[0,10,0,0]$，$\mathrm{cnt}[2]=1,\mathrm{cnt}[3]=1000$，$\sum T=10000$。

| 站 | 1 | 2 | 3 | 4 |
|---|---|---|---|---|
| $\mathrm{arr}$ | 0 | 10 | 20 | 30 |
| $\mathrm{laji}$ | 0 | 10 | 0 | 0 |

$\mathcal{W}=\{1,2\}$（$\mathrm{arr}[2]=10\le\mathrm{laji}[2]=10$，平局）。不修路：$\text{Total}=1\cdot10+1000\cdot20-10000=10010$（验算：$1$ 人坐 $10$ 分钟，$1000$ 人各坐 $10$ 分钟 ⇒ $10+10000$ ✓）。
$V(1)=\sum_{B=2}^{g(1)=2}\mathrm{cnt}[B]=\mathrm{cnt}[2]=1$；$V(2)=\sum_{B=3}^{g(2)=4}\mathrm{cnt}[B]=1000+0=1000$；$V(3)=\mathrm{cnt}[4]=0$。
最左方案（候选 A/B/C 都会去修路段 1）只省 $1$，得 $10009$；正确答案 $=\text{Total}-1000=9010$。手算核对：修路段 2 ⇒ $\mathrm{arr}[3]=19$ ⇒ $1000$ 人各坐 $9$ 分钟 ⇒ $10+9000=9010$ ✓。
**失败形态**：最早那批乘客（在 1 号站上车的 $1$ 人）下车太早，修左边的路只能帮到 $1$ 个人；而"车在 2 号站等齐 $1000$ 人"这个等待点把左边所有修路的收益全部截断，只有它右边的路段才有效。**结论：必须在所有"段"里取 $V$ 最大的那一段（即受益人数最多的一批），而不能按位置先后取。**

**反例 2（手算，打掉"只能修 $D_x>1$ 的路段"）**：$n=4$，$D=[1,10,1]$，$k=1$。乘客：$1$ 人 $T=1000,A=1,B=2$；$100$ 人 $T=0,A=1,B=3$。
$\mathrm{laji}=[1000,0,0,0]$，$\mathrm{cnt}[2]=1,\mathrm{cnt}[3]=100$，$\sum T=1000$。

| 站 | 1 | 2 | 3 | 4 |
|---|---|---|---|---|
| $\mathrm{arr}$ | 0 | 1001 | 1011 | 1012 |
| $\mathrm{lev}$ | 1000 | 1001 | 1011 | 1012 |

$\mathcal{W}=\{1\}$（$0\le1000$；后面各站 $\mathrm{laji}=0$ 都远小于 $\mathrm{arr}$）。不修路 $\text{Total}=1\cdot1001+100\cdot1011-1000=101101$。
$g(1)=n=4\Rightarrow V(1)=\mathrm{cnt}[2]+\mathrm{cnt}[3]+\mathrm{cnt}[4]=1+100+0=101$。若强制 $D_x>1$ 就只能选路段 2：$V(2)=\mathrm{cnt}[3]+\mathrm{cnt}[4]=100$，答案 $101001$。
而直接手算修路段 1（$1\to0$）：$\mathrm{arr}[2]=1000,\mathrm{arr}[3]=1010,\mathrm{arr}[4]=1011$，$\text{Total}=1000+101000-1000=101000$ ✓ 正好省 $101$。
**为什么修成 $0$ 不会"自断收益"**：修路段 $x$ 后车到 $x+1$ 站更早，只有当 $\mathrm{laji}[x+1]\ge$ **新的** $\mathrm{arr}[x+1]$ 时收益才被截断。设修之前 $\mathrm{arr}[x+1]=\mathrm{lev}[x]+1$；若 $\mathrm{lev}[x]>\mathrm{laji}[x+1]$，修完 $\mathrm{arr}'[x+1]=\mathrm{lev}[x]>\mathrm{laji}[x+1]$，$x+1$ 站仍然不等 ⇒ 收益继续传播。所以**禁止 $D_x=1$ 的路段是错的**；唯一的限制是 $D_x\ge1$（否则不能用加速器）。引理 2 里的平局判据 $\mathrm{arr}\le\mathrm{laji}$ 已经把所有截断情形都算准了。

**再补一条**：也不能"修光一条就换下一条、但把这段的收益记成车上人数"。例如 $\mathrm{pax}_1:1\to4$，$\mathrm{pax}_2:2\to3$，路段 1 上的在车人数只有 $1$，但 $V(1)$ 还包含 $\mathrm{cnt}[3]$（中途上车的 $\mathrm{pax}_2$ 也会因左边修路而提前到站）。**收益按"下车人数在 $(x,g(x)]$ 内求和"算，与在哪上车无关。**

### 2.4 更简单的版本：完全不等（$\mathrm{laji}\equiv0$）

若没有任何等待点（$\mathrm{arr}[i]>\mathrm{laji}[i]$ 对所有 $i\ge2$），则 $g(x)\equiv n$，$V(x)=\sum_{B=x+1}^{n}\mathrm{cnt}[B]$ **关于 $x$ 单调不增**，于是"一直修最左边能修的路段"就是最优的，而且一次能连修 $\min(k,D_x)$ 次、每次收益都是 $V(x)$。
**这一步的作用**：它告诉我们"左移支配"这件事是真的，只不过**支配范围只在"同一个无等待的段"内成立**。加上等待点之后，段与段之间必须比较 $V$ 的大小（反例 1）。

### 2.5 归约：分段独立 + 递推 + 前缀和 + 批处理

**定义段**：把 $\mathcal{W}=\{w_1<w_2<\dots\}$（$w_1=1$）当作分割点，每段 $[w_j,\,w_{j+1}]$ 内部的站都不等待。由引理 2，路段 $x\in[w_j,w_{j+1}-1]$ 的 $g(x)=w_{j+1}$（若存在，否则 $n$），所以

$$V(x)=\sum_{B=x+1}^{w_{j+1}}\mathrm{cnt}[B]\quad\text{在段内关于 }x\text{ 单调不增}$$

**引理 3（段内最左支配）**：同一时刻，段 $[w,r]$ 内所有满足 $D\ge1$ 的路段中，**最左边那一条**的 $V$ 最大。
**证明**：$x<x'$ 同属本段（$w\le x<x'\le r-1$）⇒ 本段内部（除了右端 $r$）没有等待点 ⇒ $g(x)=g(x')=r$（$r$ 是下一个等待点，或 $r=n$）⇒ 求和区间 $(x,r]\supseteq(x',r]$，且 $\mathrm{cnt}\ge0$ ⇒ $V(x)=\sum_{B=x+1}^{r}\mathrm{cnt}[B]\ge\sum_{B=x'+1}^{r}\mathrm{cnt}[B]=V(x')$。$\blacksquare$

**引理 4（段间独立）**：修段 $[w,r]$ 内的路段 $x$，只会改变 $(x,r]$ 上各站的 $\mathrm{arr}$；若 $r<n$，$r$ 之后的站的 $\mathrm{arr}$ 一律不变（$r=n$ 时右边没有站，结论平凡成立）。于是别的段的 $V$ 与收益完全不受影响。
**证明**：由引理 2，$\varepsilon_i=0$ 对 $i>r=g(x)$ 成立（$r$ 是 $x$ 右边第一个等待点），即 $(x,r]$ 之外 $\mathrm{arr}$ 不变。再看离站时刻：修前 $\mathrm{lev}[r]=\max(\mathrm{arr}[r],\mathrm{laji}[r])=\mathrm{laji}[r]$，修后 $\mathrm{arr}'[r]=\mathrm{arr}[r]-1\le\mathrm{laji}[r]-1<\mathrm{laji}[r]$，故 $\mathrm{lev}'[r]=\mathrm{laji}[r]=\mathrm{lev}[r]$ —— **等待点把左边的提前量整个"吸收"掉了**；于是 $\mathrm{arr}[r+1]=\mathrm{lev}[r]+D_r$ 也逐项不变，归纳可知 $r$ 之后 $\mathrm{lev}$ 全部不变。$\blacksquare$

**引理 4'（水位形式：平局的本质、record 永久存在、左右精确可加）**

先把引理 1 的闭式改写成一个更好用的形式，本节后面全靠它。记 $S_i=\sum_{s\le i}D_s$（原始前缀），$A_i=\sum_{s\le i}a_s$，其中 $a_s$ 是路段 $s$ 上实际使用的加速器数（$0\le a_s\le D_s$，$\sum a_s\le k$）。令

$$b_j=\mathrm{laji}[j]-S_{j-1}+A_{j-1},\qquad m_B=\max_{1\le j\le B-1}b_j,\qquad \mathrm{arr}[B]=S_{B-1}-A_{B-1}+m_B\tag{$\star$}$$

（由引理 1：$\mathrm{arr}[B]=\max_j\big(\mathrm{laji}[j]+\sum_{r=j}^{B-1}(D_r-a_r)\big)=\max_j\big(\mathrm{laji}[j]+(S_{B-1}-A_{B-1})-(S_{j-1}-A_{j-1})\big)=(S_{B-1}-A_{B-1})+\max_j b_j$ ✓）

$(\star)$ 把 $\mathrm{arr}$ 拆成"纯行驶量 $S-A$"加"水位 $m$"，而 $b_j$ 正是"$j$ 站上的人若从 0 时刻算起的相对水位"。

**(i) 平局为什么必须算等待点**：$\mathrm{arr}[i]\le\mathrm{laji}[i]\iff S_{i-1}-A_{i-1}+m_i\le\mathrm{laji}[i]\iff m_i\le b_i$。即 **$i$ 是等待点 $\iff b_i$ 是序列 $b$ 的新高点（record）**。"$\le$"（含平局）不是打补丁，而是"新高"的定义里天然的等号。

**(ii) record 永久存在**：若 $i$ 在某状态下是 record（$b_i\ge m_i$），则此后无论再加哪些加速器，$i$ 仍是 record。
**证明**：往路段 $s$ 加 $1$ 个单位使 $A_u$ 对所有 $u\ge s$ 增 $1$，于是 $b_j$ 对所有 $j\ge s+1$ 增 $1$，而 $j\le s$ 的 $b_j$ 不变。分两种情况：

- $s\ge i$：$b_i$ 依赖 $A_{i-1}$，$i-1<s$，故不变；$m_i=\max_{j<i}b_j$ 里每个 $j<i\le s$ 也不变。⟹ 关系 $b_i\ge m_i$ 不变 ✓
- $s<i$：$b_i'=b_i+1$；$m_i'=\max_{j<i}b_j'\le\max_{j<i}(b_j+1)=m_i+1$。⟹ $b_i'\ge m_i+1\ge m_i'$ ✓

两类都保住了 $b_i\ge m_i$，且归纳可叠加任意多个单位。$\blacksquare$

⟹ $\mathcal W$ 随修路**只增不减**（§2.6 轮数 $\le2n$ 用的就是这条，之前只是断言）；更重要的是：**分裂点一旦产生就永不再消失**，所以"分段"是真正的树形递归，而不是临时状态。

**(iii) 左右精确可加（本节的核心等式）**：设 $i$ 是当前状态的 record，把路段分成左 $[1,i-1]$、右 $[i,n-1]$，左半所用单位总数记 $\Lambda=\sum_{s\le i-1}a_s$，右半的前缀记 $A^{R}_u=\sum_{i\le s\le u}a_s$。则

- $B\le i$：$(\star)$ 右端只用到 $a_s(s<B)$ 与 $b_j(j<B)$，全是左半的量 ⟹ 与右半无关 ✓
- $B>i$：先把 $m_B$ 劈成两截。由 (ii) 用的 record 性质，$\max_{j\le i}b_j=b_i=\mathrm{laji}[i]-S_{i-1}+\Lambda$（$A_{i-1}$ 恰好就是 $\Lambda$，因为左半路段全 $\le i-1$）。对 $j>i$ 有 $A_{j-1}=\Lambda+A^{R}_{j-1}$。定义右半的水位

$$m^{R}_B=\max\Big(\underbrace{\mathrm{laji}[i]-S_{i-1}}_{\text{虚拟起点 }i,\ A^{R}_{i-1}=0},\ \max_{i<j<B}\big(\mathrm{laji}[j]-S_{j-1}+A^{R}_{j-1}\big)\Big)$$

则 $\max_{i<j<B}b_j=\Lambda+\big(m^{R}_B-(\text{可能的第一项})\big)\le\Lambda+m^{R}_B$，且 $\max_{j\le i}b_j=\mathrm{laji}[i]-S_{i-1}+\Lambda\le m^{R}_B+\Lambda$，两者取 $\max$ 得 $m_B=\Lambda+m^{R}_B$。代回 $(\star)$：

$$\mathrm{arr}[B]=S_{B-1}-\big(\Lambda+A^{R}_{B-1}\big)+\big(\Lambda+m^{R}_B\big)=S_{B-1}-A^{R}_{B-1}+m^{R}_B$$

**$\Lambda$ 一负一正完全抵消** —— 左半怎么用加速器对右半的 $\mathrm{arr}$ **没有任何影响**。而方括号里那个式子恰好是子问题"从 $i$ 站出发、离站时刻钉死在 $\mathrm{laji}[i]$、只有右半路段可修"的同一个闭式 $(\star)$。

合并两半：$\text{Total}+\sum_pT_p=\sum_{B\le i}\mathrm{cnt}[B]\mathrm{arr}[B]+\sum_{B>i}\mathrm{cnt}[B]\mathrm{arr}[B]$，第一项只含左半单位、第二项只含右半单位 ⟹ **总收益在 record 处精确可加**，全局问题 $=$ 左子问题 $\oplus$ 右子问题，两者唯一的耦合是"加速器总数 $k$"这一份共享资源。$\blacksquare$

> 这条把上面引理 4 那句"等待点把左边的提前量整个吸收掉"从口头论证升级成了代数恒等式：吸收 $=$ $\Lambda$ 在 $-A_{B-1}$ 与 $+m_B$ 两处正好抵消。下面引理 7 的归纳证明只依赖 (ii)(iii)。

**引理 5（收益只减不增）**：修过路之后，任意路段的 $V$ 都不会变大。
**证明**：修路只让 $\mathrm{arr}$ 变小 ⇒ $\mathcal{W}$ 只会变大 ⇒ $g(x)=\min\{i\ge x+1:i\in\mathcal{W}\}$ 只会变小（或不变）⇒ 求和区间 $(x,g(x)]$ 缩小 ⇒ $V$ 不增。$\blacksquare$

**引理 6（每段的"报价表"结构 + 整体最优）**：固定一段 $[w,r]$，设 $x$ 是其最左可行路段（$D_x\ge1$），$V=V(x)$，瓶颈容量
$$q=\min\Big(D_x,\ \min_{i\in(x,r)}\big(\mathrm{arr}[i]-\mathrm{laji}[i]\big)\Big)$$
（内层区间为空时取 $+\infty$；段内各站 $\mathrm{arr}[i]>\mathrm{laji}[i]$，故内层最小值 $\ge1$，于是 $q\ge1$）。则：

1. 在这段里，**连修 $x$ 共 $q$ 次**，每次收益都恰为 $V$；第 $q+1$ 次修 $x$ 时要么 $D_x=0$（不可用），要么某个内站变成平局等待点。
   **证明**：第 $j$ 次修 $x$（修前该段已修 $j-1$ 次）要求"传播经过"每个内站 $i\in(x,r)$，即当时的 $\mathrm{arr}[i]-\mathrm{laji}[i]\ge1$，即 $j\le\mathrm{arr}_0[i]-\mathrm{laji}[i]$；故 $j\le\min_{i}(sl_i)$。加上 $j\le D_x$。此时 $g(x)$ 仍是 $r$、$V$ 不变，收益 $=\sum_{B=x+1}^{r}\mathrm{cnt}[B]$ ✓（$r$ 站的 $\mathrm{arr}$ 也照样减 $1$，$r$ 站的下客数计入）。$\blacksquare$
2. 若 $q=\min_i sl_i\le D_x$，则修完第 $q$ 次后，第一个 slack 被耗光的站 $s$ 成为新的等待点，本段**分裂**成 $[w,s]$ 与 $[s,r]$ 两个子段；若 $q=D_x$，本段继续，只是最左可行路段右移到下一条 $x'$，收益变成 $V(x')\le V$。
3. 无论哪种后继，**子问题的报价都不超过 $V$**：分裂后左子段的最优报价 $\le\sum_{B=x+1}^{s}\mathrm{cnt}[B]\le V$，右子段最左可行路段 $\ge s>x$，其收益是 $(x,r]$ 的子区间和 $\le V$；不分裂的右移情形由引理 3 直接得 $\le V$。

由 1–3：对每一段，"能拿到的收益"构成一棵树/一串层级，父层收益 $\ge$ 子层收益，且要拿到子层的报价必须先取完父层的 $q$ 份。把每一段的这些报价汇总成一串**有序表** $G_\sigma$（层序 + 每层 $q$ 份相同值），下面用归纳把它变成真正的定理。

**引理 7（段内报价表 $G_\sigma$ 的最优性 —— 归纳证明）**

固定一段 $\sigma=[w,r]$（$w$ 是 record，$r$ 是它右边第一个 record，或 $r=n$），段内无 record，即内点 $i\in(w,r)$ 都有 $\mathrm{sl}_i=\mathrm{arr}[i]-\mathrm{laji}[i]\ge1$。$x$ = 本段最左可行路段（$D_x\ge1$），$V=V(x)=\sum_{B=x+1}^{r}\mathrm{cnt}[B]$，

$$q=\min\Big(D_x,\ \min_{i\in(x,r)}\mathrm{sl}_i\Big)\quad(\text{内点集为空时第二项}=+\infty)$$

$s^*$ = 内点中 $\mathrm{sl}$ 的**最左**最小点（无内点则视为不存在）。递归定义表 $G_\sigma$：

- **路段先耗尽**（$q=D_x\le\min\mathrm{sl}$）：$G_\sigma=[\,\underbrace{V,\dots,V}_{q}\,]\uplus G_{\sigma\setminus x}$，$\sigma\setminus x$ 表示同一段但把 $D_x$ 置 $0$（下一条最左可行路段右移）。
- **先出现新 record**（$q=\mathrm{sl}_{s^*}<D_x$）：$G_\sigma=[\,V\times q\,]\uplus G_{[w,s^*]}\uplus G_{[s^*,r]}$。

由引理 6 第 3 点，子表的每一项 $\le V$，故 $G_\sigma$ **自身不增**。

**命题**：用 $t$ 个单位、全部投进 $\sigma$，能拿到的最大收益 $=G_\sigma$ 前 $t$ 大之和。

**证明**（对测度"$(|\sigma|,\ \sigma\text{ 内可行路段数})$ 按字典序"归纳；两种后继都严格变小：$\sigma\setminus x$ 少一条可行路段，分裂后两子段长度都更短，故归纳良基）。取任意方案 $a$（$\sum a=t$）与它的任一执行顺序，把单位分两堆：$P$ = 投在路段 $<s^*$ 的单位，$R$ = 投在路段 $\ge s^*$ 的单位（无 $s^*$ 时 $R=\varnothing$）。

1. **$R$ 堆的收益 $=$ 它作为右子段 $[s^*,r]$ 的方案时的收益**。对 $y\ge s^*$，只要 $\sigma$ 未分裂，$g(y)=r$，故 $V^{(\sigma)}(y)=\sum_{B=y+1}^{r}\mathrm{cnt}[B]$；而右子段的边界同样是 $r$，故逐次报价**字面相同**。又引理 2 说 $R$ 单位的收益区间 $(y,g(y)]$ 整体落在 $s^*$ 右侧，$s^*$ 左边一切不变 ⟹ $R$ 单位不会提前触发分裂。于是 $R$ 堆正是右子段的一个合法方案，由归纳假设其收益 $\le G_{[s^*,r]}$ 的前 $|R|$ 大之和。
2. **$P$ 堆**。未分裂时每个 $P$ 单位收益 $\le V$（引理 3：$V(y)=\sum_{B=y+1}^{r}\mathrm{cnt}[B]$ 随 $y$ 不增，$\le V(x)=V$）。
   - $|P|\le q$：$P$ 堆总收益 $\le|P|\cdot V$。
   - $|P|>q$：先定位**分裂的确切时刻**。每个 $P$ 单位（未分裂时）都把 $\mathrm{arr}[s^*]$ 减 $1$ 而 $\mathrm{laji}[s^*]$ 不变，故第 $\mathrm{sl}_{s^*}$ 个 $P$ 单位之后 $s^*$ 成为 record。其它内点 $s'\ne s^*$ 要更早变 record，需 $\mathrm{sl}_{s'}\ge\mathrm{sl}_{s^*}$ 个投在 $<s'$ 的单位；$s'<s^*$ 时这些也是 $P$ 单位、个数 $\ge\mathrm{sl}_{s'}\ge q$，$s'>s^*$ 时 $s^*$ 已在第 $q$ 个之前先裂 ⟹ **第一个新 record 恰是 $s^*$，恰在第 $q$ 个 $P$ 单位之后出现**（若 $q=D_x<\mathrm{sl}_{s^*}$ 则改成"$x$ 先饱和"）。
     - 情形 $q=D_x$：投在 $x$ 上的单位至多 $D_x=q$ 个，每个收益 $\le V$；$P$ 中其余单位与 $R$ 中全部单位都落在 $\sigma\setminus x$ 内，用归纳假设。
     - 情形 $q=\mathrm{sl}_{s^*}$：前 $q$ 个 $P$ 单位每个 $\le V$；后 $|P|-q$ 个单位在**已分裂**状态下执行，由引理 4'(iii)（$\Lambda$ 抵消恒等式）它们的逐次收益**恰好等于**在左子段 $[w,s^*]$ 中执行同一串单位的收益，用归纳假设 $\le G_{[w,s^*]}$ 的前 $|P|-q$ 大之和。
3. **合并成上界**。要用到的唯一事实是：两串降序表 $A,B$ 的并集，其前 $a+b$ 大之和 $\ge$ ($A$ 的前 $a$ 大之和) $+$ ($B$ 的前 $b$ 大之和)（右边选出的 $a+b$ 个数都在并集里）。
   - $|P|\le q$ 且发生分裂：$\text{gain}\le|P|V+\text{top}_{|R|}(G_{[s^*,r]})$。$G_\sigma$ 含 $q\ge|P|$ 个 $V$，取前 $t$ 大时先拿掉 $|P|$ 个 $V$，还剩 $t-|P|=|R|$ 个名额，从 $G_{[w,s^*]}\uplus G_{[s^*,r]}$ 里挑，$\ge$ 右子表单独的前 $|R|$ 大 ✓
   - $|P|>q$（分裂）：$\text{gain}\le qV+\text{top}_{|P|-q}(G_{[w,s^*]})+\text{top}_{|R|}(G_{[s^*,r]})\le qV+\text{top}_{t-q}(G_{[w,s^*]}\uplus G_{[s^*,r]})=$ 前 $t$ 大之和（$t-q=(|P|-q)+|R|$）✓
   - $q=D_x$（饱和）：$\text{gain}\le qV+\text{top}_{t-q}(G_{\sigma\setminus x})=$ 前 $t$ 大之和 ✓
4. **可达性**。按 $G_\sigma$ 的层序真的去执行（父层 $q$ 份取满再进子层，同值先浅后深）：由第 2 步，父层这 $q$ 份之内既不会提前分裂、也不会提前饱和，每份都确实拿到 $V$；第 $q$ 份之后状态恰为后继状态，递归下去逐份兑现 $G_\sigma$。故上界可达，等号成立。$\blacksquare$

**全局拼装**：把引理 4'(iii) 沿所有 record 递归下去，整条线被唯一拆成若干段，总收益 $=$ 各段收益之和，段之间唯一的耦合只有"加速器总数 $k$"这一份共享资源。每段的值函数 $t\mapsto G_\sigma$ 前 $t$ 大之和是**凹的**（表不增），"把 $k$ 份不可分资源分给若干凹函数"的最优值 $=$ **所有段表并集的前 $k$ 大之和**（上界由各段凹性逐项相加得到，可达由"每次从各段当前层取最大"的归并过程得到）。而"每轮全局取最大 $V$、一次批处理 $q$ 份"的贪心，正是在线地按降序消耗这个并集（同一批 $q$ 份同值，先取谁都不改变总和）⟹ **贪心 $=$ 最优**。$\blacksquare$

> **与旧稿的差别**：原来这一步写的是"一份 slack 被消耗，最多让以后某个右移的报价少拿 $1$ 份，而那份的值 $\le V(x)$，所以换不过去只会更差"——那是记账式直觉，不是证明。现在分叉的合法性由引理 4'(iii) 的**恒等式**（$\Lambda$ 在 $-A$ 与 $+m$ 两处正好抵消，故左右互不影响）保证，"父层恰好能拿 $q$ 份"由第 2 步"第一个新 record 必是最左的 slack 最小点"保证，跨子问题的合并由第 3 步的并集不等式保证。三处都是等式或归纳，没有模糊记账。

### 2.6 分类讨论 / 复杂度收口

- $k=0$：只做一次递推 + 加权求和，$O(n+m)$（这正是 10% 分）。
- 若每轮只修 $1$ 次：轮数 $\le k=10^5$，每轮 $O(n)$ 递推 ⇒ $10^8$，只有 60% 的数据（$k\le100$）能过。**所以必须批处理。**
- 批处理后：每轮结束必然发生下面之一：(a) $k$ 被用完；(b) 某条路段 $D_x$ 变成 $0$；(c) 出现一个新的等待点。事件 (b) 至多 $n-1$ 次，事件 (c) 至多 $n-1$ 次（$\mathcal W$ 单调增由引理 4'(ii) 保证：record 一旦产生就永久存在），所以**轮数 $\le 2n$**，总复杂度 $O(n^2+m)=O(10^6)$ ✓。
- 若某段最左可行路段的 $V=0$：该段所有更右路段 $V$ 也只可能 $\le 0$（引理 3），整段直接跳过，不影响轮数上界。
- $D_x=0$ 的路段不能用加速器，但它仍在段内 ⇒ 要**继续往右**找本段第一条 $D\ge1$ 的路段（不是换段）。

---

## 3. 模式总结（Pattern Summary）

按实际思考顺序：

- **explore its property** — 把"总旅行时间"改写成"到站时刻的加权和" $\sum_i\mathrm{cnt}[i]\mathrm{arr}[i]-\mathrm{const}$，只与 $\mathrm{arr}$ 有关。
- **explore its property（闭式）** — 展开 $\mathrm{lev}[i]=\max(\mathrm{arr}[i],\mathrm{laji}[i])$ 得 $\mathrm{arr}[B]=\max_j(\mathrm{laji}[j]+\sum D_r)$，看清"谁决定到站时刻"。
- **consider easier version** — 只做一次修路：推出引理 2，收益 = $(x,g(x)]$ 内下车人数，$g(x)$ 为右边第一个"等待点（含平局）"。
- **consider easier version（不等任何人）** — 没有等待点时 $V(x)$ 关于 $x$ 单调不增 ⇒ 最左支配；从而知道"最左"这件事只在**同一段内**成立。
- **dissect the greedy failure** — 反例 1（最早那批乘客只值 1，另一批值 1000）：证明"最早一批/全局最左"必错，必须跨段比大小。
- **dissect the greedy failure** — 反例 2（$D=[1,10,1]$）：证明"$D_x>1$ 才修"必错，正确限制只是 $D_x\ge1$；平局判据 $\mathrm{arr}\le\mathrm{laji}$ 已经把"修成 0 是否自断收益"处理干净。
- **classic problem solution** — 贪心 + 递推：每轮重算 $\mathrm{arr}/\mathrm{lev}$（$O(n)$），用等待点分段，段内取最左可行路段（引理 3），跨段取 $V$ 最大者。
- **reduce to easier problem（水位恒等式）** — 把 $\mathrm{arr}[B]$ 写成 $S_{B-1}-A_{B-1}+m_B$，等待点 $=$ $b$ 序列的新高点；于是"record 永久存在"和"$\Lambda$ 一负一正正好抵消"给出**段间精确可加**（引理 4'），全局问题变成段的树形递归。
- **reduce to easier problem（段表 + 归纳）** — 每段一张不增报价表 $G_\sigma$（父层 $q$ 份 $V$ ⊕ 子段表），命题"投 $t$ 份的最优值 $=G_\sigma$ 前 $t$ 大之和"用良基归纳 + 并集前 $a+b$ 大不等式闭合（引理 7）；跨段再按凹函数归并取前 $k$ 大 ⟹ 贪心最优。
- **classic problem solution（第二招：批处理）** — 用"段内 slack 最小值"算出同一位置能连修几次 $q=\min(D_x,\min sl_i)$，一次扣 $q\cdot V$；轮数被 $\le 2n$ 卡住，$O(n^2)$。
- **reduce to easier problem（前缀/后缀）** — 段内收益 $\sum_{B=x+1}^{r}\mathrm{cnt}[B]$ 用下车人数后缀和 $O(1)$ 查；slack 最小值用"段内后缀最小" $O(1)$ 查。
- **overflow guard** — 答案可达 $2\times10^9$，$\mathrm{arr},\mathrm{cnt}$ 用 `int`，总量与累加用 `i64`。

---

## 4. 真实解法（Real Solution）

### 4.1 符号表（Notation Glossary）

| 符号 | 含义 | 类型 / 初始化 | 记忆点 |
|---|---|---|---|
| `n,m,k` | 站数 / 乘客数 / 加速器数 | `int`，输入 | $n\le10^3,m\le10^4,k\le10^5$ |
| `D[i]` | $i\to i+1$ 的行驶时间 | `int D[mxn]`，输入；会随贪心被 $-q$ | 可重复修，须 $\ge0$ |
| `laji[i]` | $i$ 站上车的最后一名乘客到达时刻 | `int laji[mxn]`，全局默认 0，取 $\max T$ | "垃圾时间"＝车要空等的底线 |
| `cnt[i]` | 在 $i$ 站下车的人数 | `int cnt[mxn]`，默认 0，`++cnt[B]` | 收益就靠它求和 |
| `sumT` | $\sum_p T_p$ | `i64`，累加 | 与方案无关的常数 |
| `arr[i]` | 车**到站**时刻 | `int arr[mxn]`，`calc()` 每轮重算 | $\mathrm{arr}[1]=0$ |
| `lev[i]` | 车**离站**时刻 | `int lev[mxn]`，同上 | $\mathrm{lev}[i]=\max(\mathrm{arr}[i],\mathrm{laji}[i])$ |
| `nx[i]` | 从 $i$ 起（含 $i$）第一个等待点的站号 | `int nx[mxn]`，`nx[n+1]=n`，右往左推 | $g(x)=\mathrm{nx}[x+1]$，收益右界 |
| `lim[i]` | 站 $i$ 到它的等待点**之前**，$\mathrm{arr}-\mathrm{laji}$ 的最小值 | `int lim[mxn]`，`lim[n+1]=inf`，右往左推 | "这里最多能连修几次不被截断" |
| `sc[i]` | 下车人数后缀和 $\sum_{B\ge i}\mathrm{cnt}[B]$ | `int sc[mxn]`，`sc[n+1]=0` | 段内收益 = 两段后缀和之差 |
| `bx,bv,bq` | 本轮选中路段 / 其收益 / 批处理次数 | `int`，每轮重扫 | $x^*,V^*,q^*$ |
| `ans` | 当前总旅行时间 | `i64`，先算基准值，再 `ans-=bv*bq` | 输出它 |
| `inf` | "右边卡不住"的哨兵 | `constexpr int inf{1145141919}` | 与 $D_x$、$k$ 一起取 $\min$ |

> **下标约定**：全文与代码一律 **1-based 站号**，与题面 $D_i$ 一致（前缀/后缀与递推边界最不容易错）；$D_i$ 表示路段 $i\to i+1$，只存在 $i=1\dots n-1$。全局 `int` 数组默认值为 $0$，本题正好利用它表示"该站没人上车/没人下车"；唯一需要显式初始化的是"表示无穷"的 `lim[n+1]=inf`（否则 0 会被当成"这里立刻卡住"）。

### 4.2 算法步骤（Numbered Steps）

1. **读入**：$n,m,k$；$D_1\dots D_{n-1}$；每名乘客 $(T,A,B)$：$\mathrm{laji}[A]=\max(\mathrm{laji}[A],T)$，$\mathrm{cnt}[B]++$，$\mathrm{sumT}+=T$。
2. **基准解 `calc()`**：$\mathrm{arr}[1]=0$；$i=1..n-1$：$\mathrm{lev}[i]=\max(\mathrm{arr}[i],\mathrm{laji}[i])$，$\mathrm{arr}[i+1]=\mathrm{lev}[i]+D_i$；最后 $\mathrm{lev}[n]=\max(\mathrm{arr}[n],\mathrm{laji}[n])$。$O(n)$。
3. **初始答案**：$\text{ans}=\sum_{i=2}^{n}\mathrm{cnt}[i]\cdot\mathrm{arr}[i]-\mathrm{sumT}$（`i64`）。
4. **贪心主循环**（`while k>0`，轮数 $\le 2n$）：
   1. `calc()` 重算时间表。
   2. 后缀量，一次右往左扫描：$\mathrm{sc}[n+1]=0$，$\mathrm{sc}[i]=\mathrm{sc}[i+1]+\mathrm{cnt}[i]$；$\mathrm{nx}[n+1]=n,\mathrm{lim}[n+1]=\mathrm{inf}$；若 $\mathrm{arr}[i]\le\mathrm{laji}[i]$（等待点，含平局）则 $\mathrm{nx}[i]=i,\mathrm{lim}[i]=\mathrm{inf}$，否则 $\mathrm{nx}[i]=\mathrm{nx}[i+1],\mathrm{lim}[i]=\min(\mathrm{lim}[i+1],\mathrm{arr}[i]-\mathrm{laji}[i])$。
   3. 左往右扫路段 $x=1..n-1$，用布尔 `ok` 标记"本段还没出现可行路段"：遇 $\mathrm{arr}[x]\le\mathrm{laji}[x]$ 就 `ok=true`（新段开始）；$D_x\le0$ 跳过（继续往右找）；`!ok` 跳过；否则 `ok=false`，令 $g=\mathrm{nx}[x+1]$，$V=\mathrm{sc}[x+1]-\mathrm{sc}[g+1]$；若 $V\le0$ 跳过（本段作废）；$q=\min(k,\min(D_x,\mathrm{lim}[x+1]))$；记录 $V$ 最大的 $(x,V,q)$ 到 $(\mathrm{bx},\mathrm{bv},\mathrm{bq})$。
   4. 若 $\mathrm{bx}<0$：无处可修，`break`。否则 $D_{\mathrm{bx}}-=\mathrm{bq}$，$\text{ans}-=\mathrm{bv}\cdot\mathrm{bq}$，$k-=\mathrm{bq}$。
5. **输出** `ans`。

**复杂度**：时间 $O(n^2+m)$（轮数 $\le 2n$，每轮 $O(n)$；$2\times10^3\times10^3\approx2\times10^6$）；空间 $O(n)$。

### 4.3 验证状态（全部为手工推演，未运行任何代码）

1. **题目样例（唯一官方样例）**：由样例说明反推输入 $n=3,m=3,k=2$，$D=[1,4]$，乘客 $(0,1,3),(1,1,2),(5,2,3)$；$\mathrm{laji}=[1,5,0]$，$\mathrm{cnt}[2]=1,\mathrm{cnt}[3]=2$，$\mathrm{sumT}=6$。
   - 递推：$\mathrm{arr}=[0,2,9]$，$\mathrm{lev}=[1,5,9]$；基准 $\text{ans}=1\cdot2+2\cdot9-6=14$。
   - $\mathcal{W}=\{1,2\}$；$\mathrm{nx}[2]=2,\mathrm{lim}[2]=\mathrm{inf}$；$\mathrm{nx}[3]=3,\mathrm{lim}[3]=9$；$\mathrm{sc}=[3,3,2,0]$。
   - 第 1 轮：候选 路段 1（$g=2,V=\mathrm{sc}[2]-\mathrm{sc}[3]=1,q=1$）；候选 路段 2（$g=3,V=\mathrm{sc}[3]-\mathrm{sc}[4]=2,q=\min(2,4,9)=2$）⇒ 取 $x=2,V=2,q=2$。
   - 更新：$D_2=2$，$\text{ans}=14-2\times2=10$，$k=0$ ⇒ 结束。**输出 10 ✓ 与官方输出一致**，且方案"对 $D_2$ 用 2 个加速器"与题目说明一致。
2. **反例 1 走一遍代码逻辑**：$\mathrm{nx}[2]=2$（平局）⇒ $V(1)=\mathrm{sc}[2]-\mathrm{sc}[3]=1$；$V(2)=\mathrm{sc}[3]-\mathrm{sc}[4]=1000$，$q=\min(1,10,\mathrm{lim}[3]=20)=1$ ⇒ 选路段 2，$\text{ans}=10010-1000=9010$ ✓ 与手算最优一致（错误贪心给 10009）。
3. **反例 2 走一遍**：$\mathcal{W}=\{1\}$，单段 $[1,4]$；$x=1$：$g=\mathrm{nx}[2]=4$，$V=\mathrm{sc}[2]-\mathrm{sc}[5]=101$，$q=\min(1,1,\mathrm{lim}[2]=1001)=1$；$x=2,3$ 被 `ok=false` 屏蔽 ⇒ 选路段 1，$\text{ans}=101101-101=101000$ ✓ 与手算最优一致（$D>1$ 限制给 101001）。
4. **分裂 + 多轮**：$n=3$，$D=[5,5]$，$k=10$，$100$ 人 $T=0,A=1,B=3$，$\mathrm{laji}[2]=3$。基准 $\mathrm{arr}=[0,5,10]$，$\text{ans}=1000$。
   - 第 1 轮：$V(1)=100$，$\mathrm{lim}[2]=5-3=2$，$q=\min(10,5,2)=2$ ⇒ $D_1=3$，$\text{ans}=800$，$k=8$。
   - 第 2 轮：新 $\mathrm{arr}=[0,3,8]$，$\mathcal{W}=\{1,2\}$；段 $[1,2]$ 的 $V=\mathrm{cnt}[2]=0$ ⇒ 作废；$V(2)=\mathrm{sc}[3]-\mathrm{sc}[4]=100$，$\mathrm{lim}[3]=\mathrm{inf}$ ⇒ $q=\min(8,5)=5$ ⇒ $D_2=0$，$\text{ans}=300$，$k=3$。
   - 第 3 轮：$D_1=3$ 但 $V(1)=0$（被 2 号站截断），$D_2=0$ ⇒ $\mathrm{bx}=-1$，退出。**输出 300**。独立验证最优性：$\mathrm{arr}[3]=\max(\mathrm{laji}[1]+D_1'+D_2',\ \mathrm{laji}[2]+D_2')=\max(D_1'+D_2',\,3+D_2')$，取 $D_2'=0,D_1'=3$ 得 $\mathrm{arr}[3]=3$，无法更小 ⇒ 最优 ✓。
5. **专治贪心的对抗算例 A（同值 $V$ 的平局取舍 + 平局等待点）**：$n=3$，$D=[5,5]$，$k=2$，$100$ 人 $T=0,A=1,B=3$，$\mathrm{laji}[2]=3$（第 4 组同样数据但 $k=2$）。基准 $\mathrm{arr}=[0,5,10]$，$\text{ans}=1000$，$\mathcal{W}=\{1\}$，$V(1)=V(2)=100$。
   - 代码用 `v>bv`（严格大于）⇒ 平局保留最左，选 $x=1$，$q=\min(2,D_1=5,\mathrm{lim}[2]=5-3=2)=2$ ⇒ $\text{ans}=1000-200=800$，落在 $D=[3,5]$：$\mathrm{arr}=[0,3,8]$，$100\cdot8=800$ ✓ 与"每轮减 $1$ 次"的逐步模拟一致。
   - 其它顺序核对：$(1$ 号路段 $1$ 次 $+\,2$ 号路段 $1$ 次$)\to D=[4,4]\to\mathrm{arr}[3]=8$；$(2$ 号路段 $2$ 次$)\to D=[5,3]\to\mathrm{arr}[3]=8$。三种都得 $800$；上界：每次加速器至多让 $\mathrm{arr}[3]$ 减 $1$，$2$ 次至多省 $200$ ✓ 贪心取到上界。平局怎么破都不影响最优值（这正是引理 4 段独立 + 引理 6 同层同价的体现）。
6. **专治贪心的对抗算例 B（消耗 slack 是否亏）**：$n=4$，$D=[2,100,100]$，$k=6$；$\mathrm{laji}=[0,1,99,0]$；$1000$ 人 $T=0,A=1,B=4$（$\mathrm{cnt}[4]=1000$），$\mathrm{cnt}[2]=\mathrm{cnt}[3]=0$。基准 $\mathrm{arr}=[0,2,102,202]$，$\text{ans}=202000$，$\mathcal{W}=\{1\}$，单段 $[1,4]$，$V(1)=V(2)=V(3)=1000$。
   - 贪心：轮 1 $x=1$，$q=\min(6,2,\min(sl_2=1,sl_3=3))=1$ ⇒ 收益 $1000$，$D=[1,100,100]$；轮 2 新 $\mathrm{arr}=[0,1,101,201]$，$\mathcal{W}=\{1,2\}$（平局），段 $[1,2]$ 的 $V=\mathrm{cnt}[2]=0$ 作废，段 $[2,4]$：$x=2$，$q=\min(5,100,sl_3=101-99=2)=2$ ⇒ 收益 $2000$；轮 3 新 $\mathrm{arr}=[0,1,99,199]$，$\mathcal{W}=\{1,2,3\}$，段 $[3,4]$：$x=3$，$q=\min(3,100,\mathrm{inf})=3$ ⇒ 收益 $3000$。合计 $6000$，$\text{ans}=196000$。
   - 上界核对：每名乘客的旅行时间 $=\mathrm{arr}[4]-0$，而每次加速器至多让 $\mathrm{arr}[4]$ 减 $1$ ⇒ $6$ 个加速器至多省 $6000$ ✓ 贪心达到理论上界，最优。同时验证"修 $1$ 号路段消耗 $2$ 号站 slack 导致分裂"确实没有让总收益变少（若完全不碰路段 1，也只得 $6\times1000=6000$）。
7. **边界自查**：$n=1$ 时题面 $A_i<B_i$ 无解，数据不会出现（$m\ge1\Rightarrow n\ge2$）；代码里读 $D$ 的循环 $i<n$、路段枚举 $x<n$ 均不越界；`sc/lim/nx` 只访问到 $n+1\le1001<$ `mxn`。
8. **未做的验证（缺口如实报告）**：本环境没有编译器，我**没有**编译或运行任何代码，也**没有**随机数据对拍、没有计时；上面 6 组算例全部是逐站手算表格得到的。此外，引理 4'(ii)（record 永久存在）与 4'(iii)（$\Lambda$ 抵消 ⟹ 左右精确可加）是纯代数核对，我按 $(\star)$ 对 $s\ge i$、$s<i$ 两种情况逐一验证过；引理 7 第 2 步"第一个新 record 恰是最左的 slack 最小点"用第 4、5、6 组算例逐份对齐：第 6 组内点 $\mathrm{sl}_2=1$ 最左最小 ⟹ $q=1$、分裂成 $[1,2]$（$V=\mathrm{cnt}[2]=0$ 作废）与 $[2,4]$（$\mathrm{sl}_3=2\Rightarrow q=2$），与文中 $1000/2000/3000$ 三份报价一一对应 ✓；第 3 步"并集前 $a+b$ 大 $\ge$ 两表各自前 $a,b$ 大之和"在多段并列同值（第 5 组）处也确认平局不亏。若要彻底放心，应补一个 $n\le6,D_i\le3$ 的暴力枚举（枚举 $\sum a_i\le k$ 的所有分配，用闭式算 $\mathrm{arr}$）与该贪心的对拍。

---

## 5. 示例代码（C++）

```cpp
// P1315 [NOIP 2011 提高组] 观光公交
// 贪心 + 递推：按"等待点"分段，段内修最左可行路段，批处理到 O(n^2)
// https://www.luogu.com.cn/problem/P1315
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};
constexpr int inf{1145141919};   // 本段右边卡不住时的哨兵

int n,m,k;
int D[mxn];          // D[i]: i -> i+1 的时间，可反复 -1，须保持 >=0
int laji[mxn];       // i 站上车的最后一名乘客到达时刻（没人上车则为 0）
int cnt[mxn];        // i 站下车人数
int arr[mxn],lev[mxn];   // 到站时刻 / 离站时刻
int nx[mxn];         // 从 i 起（含 i）第一个等待点，arr[j]<=laji[j]
int lim[mxn];        // 到该等待点之前 min(arr-laji)，即同一点能连修几次
int sc[mxn];         // 下车人数后缀和

void calc()
{
	arr[1]=0;
	for(int i=1;i<n;++i)
	{
		lev[i]=max(arr[i],laji[i]);
		arr[i+1]=lev[i]+D[i];
	}
	lev[n]=max(arr[n],laji[n]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	for(int i=1;i<n;++i)
		cin>>D[i];
	i64 sumT=0;
	for(int i=1,T,A,B;i<=m;++i)
	{
		cin>>T>>A>>B;
		laji[A]=max(laji[A],T);
		++cnt[B];
		sumT+=T;
	}

	calc();
	i64 ans=0;
	for(int i=2;i<=n;++i)
		ans+=(i64)cnt[i]*arr[i];
	ans-=sumT;

	while(k>0)
	{
		calc();
		sc[n+1]=0;
		for(int i=n;i>=1;--i)
			sc[i]=sc[i+1]+cnt[i];
		nx[n+1]=n;lim[n+1]=inf;   // 全局数组默认 0，这里必须显式填
		for(int i=n;i>=1;--i)
		{
			if(arr[i]<=laji[i])   // 等待点（平局也算）：收益在这里被截断
				nx[i]=i,lim[i]=inf;
			else
				nx[i]=nx[i+1],lim[i]=min(lim[i+1],arr[i]-laji[i]);
		}

		int bx=-1,bv=0,bq=0;
		bool ok=true;   // 本段里还没出现可修路段
		for(int x=1;x<n;++x)
		{
			if(arr[x]<=laji[x])
				ok=true;          // 进入新的一段
			if(D[x]<=0)
				continue;         // 这条路已修平，同段继续往右找
			if(!ok)
				continue;         // 只修本段第一条可行路段，段内左移支配
			ok=false;
			int g=nx[x+1];
			int v=sc[x+1]-sc[g+1];    // 收益 = (x,g] 内下车人数
			if(v<=0)
				continue;             // 本段没人受益，整段作废
			int q=min(k,min(D[x],lim[x+1]));
			if(v>bv)
				bv=v,bq=q,bx=x;
		}
		if(bx<0)
			break;                // 正收益的修法已经用尽
		D[bx]-=bq;
		ans-=(i64)bv*bq;
		k-=bq;
	}

	cout<<ans<<'\n';
	return 0;
}
```

### 代码说明（关键点与坑）

- **1-based 站号**：与题面 $D_i$、站点编号完全一致，前缀/后缀和递推边界不用换算，全文（含第 2 节公式）都用同一套编号。
- **平局算等待点**：判定一律写 `arr[i]<=laji[i]`。写成 `<` 会漏掉"刚好赶上"的站，从而把收益算大（引理 2 里 $\delta_i=\min(\varepsilon_i,\mathrm{arr}[i]-\mathrm{laji}[i])$ 在 $\mathrm{arr}[i]=\mathrm{laji}[i]$ 时为 $0$，传播正是停在这里）。
- **不限定 $D_x>1$**：只要求 `D[x]>0`，允许修成 $0$；反例 2 说明加 $D_x>1$ 会丢分。
- **`D[x]<=0` 是 `continue` 而不是换段**：本段最左"可行"路段可能要跳过若干条已经修平的路段。
- **`lim[n+1]=inf` 必须显式填**：全局 `int` 数组默认 $0$，不填就等于"下一站立刻卡住"，批处理次数会被错算成 $0$。
- **`q` 三重上限**：$\min(k,D_x,\mathrm{lim}[x+1])$ 保证这一批 $q$ 次每次收益都真是 `bv`；批处理是轮数 $O(n)$、复杂度 $O(n^2)$ 的来源。
- **`i64`**：`ans` 与 `sumT` 用 `i64`（总量可达 $2\times10^9$，`int` 会炸）；时刻、人数、$D$ 用 `int` 足够。
- 时间用 `'\n'` 输出，缩进用 Tab，运算符两侧不留空格。
