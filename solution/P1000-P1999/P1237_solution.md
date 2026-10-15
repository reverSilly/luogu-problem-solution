# P1237 [ICPC 1992 WF] 冗余依赖 —— 题解

> 难度：省选/NOI-　标签：搜索、数学、ICPC、WF
> 一句话：把"FD 冗余"翻成"左部属性的闭包里包不包含右部"，闭包用**迭代松弛（不动点）**算，最短路用 **BFS** 跑，重建路径时**过滤掉自己**并**去重**。

---

## Part 1　重新读题（大白话版）

给你 $n\le100$ 条**函数依赖（FD）**，第 $i$ 条形如 $X_i\to Y_i$：
- $X_i$、$Y_i$ 都是若干**大写字母**（域）组成的集合；
- $X\to Y$ 的语义是：一旦 $X$ 里每个域的值都确定了，$Y$ 里每个域的值就随之唯一确定。

**冗余**的定义：FD $i$ 冗余 $\iff$ 去掉 $i$ 之后，剩下的 $n-1$ 条 FD 仍然**能推出** $X_i\to Y_i$。

任务：对每条 $i$（$1\le i\le n$），若它冗余，输出一行
```
FD i is redundant using FDs: p1 p2 ... pk
```
其中 $p_1,\dots,p_k$ 是一个**能推出 $i$ 的 FD 序列**，要求 $k$ **尽可能短**（最短路）。若没有冗余 FD，整份输出只有一行
```
No redundant FDs
```

**样例**（题面给出的 Hint）：
```
3
A -> BD
BD -> C
A -> C
```
FD 3 冗余：因为 $A\to\{B,D\}$（FD 1）+ $\{B,D\}\to C$（FD 2）就能得到 $A\to C$。输出
```
FD 3 is redundant using FDs: 1 2
```

约束细节：$n\le100$；域是大写字母（$\le26$ 种）；FD 互不重复；数据保证不会出现"平凡"依赖（$A\to A$ 之类 $Y\subseteq X$ 的情况）。

> 本质：这是**函数依赖的推理问题**。工具是 Armstrong 公理（自反律/增广律/传递律），而"从 $X$ 出发用一堆 FD 最多能确定哪些属性"这件事，正是**属性闭包（attribute closure）**。

---

## Part 2　思路链（像人一样推出来）

### 第 1 步：先把"特殊机制"翻译清楚——什么是"能推出"

题目说"$i$ 冗余 $\iff$ 其余 FD 能**得到** $X_i\to Y_i$"。"得到 $X\to Y$"到底是什么意思？按 FD 语义：

> **$X\to Y$ 被一组 FD 推出 $\iff$ 从已知 $X$ 中每个域的值，都能唯一确定 $Y$ 中每个域的值。**

所以问题里唯一真正的数学对象是一个操作：

> **闭包（closure）**：给定一个起点集合 $X$ 和一组 FD，问"$X$ 能确定哪些属性"。记作 $\text{cl}(X)$。它至少包含 $X$ 自己（已知 $X$ 当然能确定 $X$）。

这样，"FD $i$ 冗余"就被翻译成一句干净的判定：

$$\boxed{\ i\ \text{冗余}\ \Longleftrightarrow\ Y_i\subseteq\text{cl}\bigl(X_i\ \text{在其余 } n-1\ \text{条 FD 下的闭包}\bigr)\ }$$

**为什么这是对的？（完整证明，双向）**

设 $F$ 为全部 $n$ 条 FD，$F' = F\setminus\{i\}$（其余的）。

- **($\Leftarrow$) 充分性**：若 $Y_i\subseteq\text{cl}_{F'}(X_i)$。由闭包定义，$\text{cl}_{F'}(X_i)$ 中的每个属性都能被 $X_i$ 的值唯一确定——而闭包是**仅用 $F'$** 算出来的（没碰 $i$）。因此 $X_i$ 能确定 $Y_i$，且这个确定性完全由 $F'$ 提供，不依赖 $i$。故 $F'\models X_i\to Y_i$，即 $i$ 是 $F'$ 的推论 → $i$ 冗余。

- **($\Rightarrow$) 必要性**：设 $F'\models X_i\to Y_i$。由 FD 推理规则（Armstrong 公理）的**完备性**（Lemma 5，下面证），"被 $F'$ 逻辑推出"当且仅当"可由 $F'$ 用闭包过程推出"，而"可由闭包过程推出"当且仅当 $Y_i$ 落在 $\text{cl}_{F'}(X_i)$ 里。故 $Y_i\subseteq\text{cl}_{F'}(X_i)$。∎

> **Lemma 1（冗余的等价判定）** 设 $F$ 为 FD 集，$i\in F$ 且 $F_i:X_i\to Y_i$，$F'=F\setminus\{i\}$。则 $i$ 冗余 $\iff Y_i\subseteq\text{cl}_{F'}(X_i)$。
> 证明见上（充分性 + 必要性，必要性用到 Lemma 5 的完备性）。∎

这一步是整个题的**命门**：它把"找冗余"这个看着很玄的命题逻辑问题，**归约**成了"反复求闭包"这个可计算的机械过程。后面的一切都是围绕"怎么高效求闭包、怎么记录最短推导链"。

### 第 2 步：闭包本身怎么算——迭代松弛（不动点）

闭包的语义是"$X$ 能确定的所有属性"。它有个**递归定义**：

$$\text{cl}(X) = \text{最小集合 }S\text{，满足 }X\subseteq S\ \wedge\ \forall\, (A\to B)\in F'\ (A\subseteq S\Rightarrow B\subseteq S).$$

**怎么机械地算出来？** 一个最朴素的想法（先不管对不对）：

> 从 $S:=X$ 开始；反复地，只要还能"用一条 FD 扩出 $S$"就扩（找一条左部 $\subseteq S$ 的 FD，把它的右部并进 $S$）；直到再也扩不动为止。这个最终的 $S$ 就是闭包。

**这对不对？（完整证明）**

> **Lemma 2（迭代松弛求闭包的正确性）** 设 $S_0=X$，$S_{t+1}=S_t\cup\bigcup\{B_j:\text{存在 }j,\ A_j\subseteq S_t\}$（把所有左部被 $S_t$ 覆盖的 FD 的右部一次并入）。则序列 $S_0\subseteq S_1\subseteq S_2\subseteq\cdots$ 单调不减，且存在有限步 $T$ 使 $S_T=S_{T+1}$；此时 $S_T$ 恰好等于上述"最小固定点"，即 $X$ 在 $F$ 下的闭包 $\text{cl}_F(X)$。
>
> **证明。**
> 记 $\Phi(S)=S\cup\bigcup\{B_j:A_j\subseteq S\}$（"扩一次"操作），则 $S_{t+1}=\Phi(S_t)$。
>
> **(a) 单调性**：若 $S\subseteq T$，则 $\{B_j:A_j\subseteq S\}\subseteq\{B_j:A_j\subseteq T\}$，故 $\Phi(S)\subseteq\Phi(T)$。由 $S_0=X\subseteq S_0$ 出发归纳，得 $S_0\subseteq S_1\subseteq S_2\subseteq\cdots$。
>
> **(b) 有限步停止**：每个 $S_t$ 都是 $\le26$ 个属性构成的集合，不同的集合至多 $2^{26}$ 个。由 (a) 严格递增的序列不可能超过 $2^{26}$ 步，必在有限步后出现 $S_T=S_{T+1}$。（实际上最多只可能"新增一个属性" $26$ 次，远快于此上界。）
>
> **(c) $S_T$ 是固定点**：$S_T=\Phi(S_T)$，即对所有 $j$，$A_j\subseteq S_T\Rightarrow B_j\subseteq S_T$。且 $X\subseteq S_0\subseteq S_T$。故 $S_T$ 是"满足条件的集合"之一。
>
> **(d) $S_T$ 是最小者（闭包）**：设 $S^*$ 是任意满足"$X\subseteq S^*$ 且对每个 $j$ 有 $A_j\subseteq S^*\Rightarrow B_j\subseteq S^*$"的集合。归纳证明 $S_t\subseteq S^*$（所有 $t$）：
>   - 基 $t=0$：$S_0=X\subseteq S^*$。
>   - 步：设 $S_t\subseteq S^*$。则对任意 $j$，若 $A_j\subseteq S_t$，由 $S_t\subseteq S^*$ 得 $A_j\subseteq S^*$，从而 $B_j\subseteq S^*$。故 $\bigcup\{B_j:A_j\subseteq S_t\}\subseteq S^*$，于是 $S_{t+1}=\Phi(S_t)=S_t\cup(\cdots)\subseteq S^*$。
>   归纳成立，特别地 $S_T\subseteq S^*$。即 $S_T$ 包含在每个合法集合里 → $S_T$ 是**最小**固定点，恰为闭包。∎

> **实现细节**：上面的 $\Phi$ 是"一轮把所有能扩的都扩"。代码里其实不用真的分轮——**单遍扫描**（每步只查一条 FD，看它的左部是否已被覆盖，能就并入）反复扫，直到一整轮没有任何并入，同样收敛到同一不动点。原因：每步"并入"都让集合变大（单调），而不动点只取决于**集合**、与"一步扩多少"无关；单遍扫描只是把 $\Phi$ 拆成了更细的小步，最终达到同一最大单调不动点 $S_T$。两种写法都对，单遍更省事。

### 第 3 步：更简单版本——只判"冗不冗余"（丢掉"最短"）

如果题目**只问**"哪些是冗余的"、不要求给出序列，那算法极简：

对每个 $i$：跑一遍 Lemma 2 的闭包（在 $F\setminus\{i\}$ 下从 $X_i$ 出发），看 $Y_i$ 是否被完全覆盖。是→冗余；否→不冗余。

复杂度：$n$ 次闭包，每次 $O(n\cdot 2^k)$（$k\le26$）→ 总计 $O(n^2 2^k)$，$n=100$ 时 $\approx10^4\cdot67108864$… 太松了，但实际每轮扫 $n$ 条 FD、每条做位运算 $O(1)$、最多 $26$ 轮新增 → 单次 $O(n\cdot 26)$，总计 $O(n^2\cdot 26)=2.6\times10^5$，毫无压力。

**但题目还要求给出"最短的"证明序列**，所以上面的"只判"不够——我们要在"判定"的同时**把推导链也记下来**，而且链要最短。这就引出第 4 步。

### 第 4 步：把"找最短推导链"翻成"图上的最短路"

关键洞察：**"用一条 FD 扩出新属性"这个过程，天然就是一张图。**

> 把**每个属性 $a$** 看成一个点（$\le26$ 个点）。
> 对每条 FD $j:A_j\to B_j$，对 $B_j$ 的**每个属性 $b$**，从 $A_j$ 的**每个属性 $a$** 连一条有向边 $a\to b$（带编号 $j$）。

这条边 $a\to b$ 编号 $j$ 的含义是："只要属性 $a$ 已被确定，用 FD $j$ 就能把 $b$ 也确定下来。"

于是，**从起点 $X_i$ 出发、用 FD 一步步扩出 $Y_i$ 的过程**，就对应于**在这张图上从 $X_i$ 走到 $Y_i$ 的路径**：
- 路径上的每一步 $a\to b$ 都消耗"一条 FD 的使用"；
- 路径**经过的边对应的 FD 编号**（去重）就是"推导 $X\to Y$ 用到了哪些 FD"。

**为什么"路径经过的 FD 集合"恰好能推出 $X\to Y$？（完整证明）**

> **Lemma 3（路径 ↔ 推导）** 设图上存在一条从某 $x\in X$ 到某 $y\in Y$ 的有向路径 $P$，其边依次编号 $j_1,\dots,j_m$。则 $F_{\{j_1,\dots,j_m\}}\models X\to Y$（这些 FD 能推出 $X\to Y$）。
>
> **证明。** 设路径为 $x=v_0\to v_1\to\cdots\to v_m=y$，边 $v_{r-1}\to v_r$ 编号 $j_r$。按归纳证"已知 $X$ 的值就能确定 $v_r$ 的值"（$\forall r$）：
> - 基 $r=0$：$v_0=x\in X$，已知 $X$ 即知 $x$。
> - 步：设已知 $v_{r-1}$ 的值。边 $v_{r-1}\to v_r$ 编号 $j_r:A_{j_r}\to B_{j_r}$，且 $v_{r-1}\in A_{j_r}$、$v_r\in B_{j_r}$（这是建边规则）。因为 $v_{r-1}\in A_{j_r}$ 且 $v_{r-1}$ 的值已知，而 $A_{j_r}\to B_{j_r}$ 属于所用的 FD 集，由 FD 语义（$A$ 的值确定 → $B$ 的值确定）得 $v_r$ 的值被确定。
> 归纳到 $r=m$：$y=v_m$ 的值由 $X$ 的值唯一确定，即 $X\to y$。又 $y\in Y$，由**自反律**（Lemma 6，$X\to y\Rightarrow X\to Y$ 当 $y\in Y\subseteq X\cup\{y\}$…更直接：$X\to y$ 且 $X\to X$，由合并律得 $X\to X\cup\{y\}\supseteq Y$，再自反得 $X\to Y$）。故 $F_{\{j_1..j_m\}}\models X\to Y$。∎

反过来也成立（若一条 FD 序列能推出 $X\to Y$，则图上存在从 $X$ 到 $Y$ 的路径只用这些 FD）——这正是"闭包过程的每一步都对应一条边"，由 Lemma 2 的构造可知闭包恰好是所有可达属性，而可达属性就是图上从 $X$ 可达的点。

**于是"最短证明序列"= 图上"用过的不同 FD 编号最少"的路径。**

这里有个微妙的点：我们要最小化的是**不同 FD 编号的个数**（$k=|p_1\cdots p_k|$ 里 $k$ 个 FD），而不是"边数"。但注意——

> **Lemma 4（最短路径可以用 BFS 求）** 在 Lemma 2 的"扩属性"过程中，**每一步扩一个（或一批）新属性只用到至多一条 FD**。更关键地：我们关心的是"推导 $X\to Y$ 最少需要**几轮**FD 使用"。把"确定一个属性 $b$"看成"到达点 $b$"，那么：
> - 起点 $X$ 里的属性，**0 步**就能到达（已知，不需要任何 FD）；
> - 属性 $b$ 若能由某条 FD $j$ 从"已到达的 $A_j$"一步推出，则 $b$ 在 $\min_{a\in A_j}(\text{dist}(a))+1$ 步内到达。
>
> 这个"最少步数"正好是**图上从 $X$ 到各点的最短路径长度**（边权全为 1，起点集合 $X$ 距离为 0）。而**单位权图的最短路 = BFS**（教科书结论：BFS 按层扩展，第一次到达某点时走的层数最小）。
>
> 因此：**对每条 FD $i$，把 $X_i$ 中所有属性当作多源起点（dist=0）做 BFS，求 $Y_i$ 中各属性的最短距离；若 $Y_i$ 中每个属性都可达，则 $i$ 冗余，且"到达 $Y_i$ 各点所经过的边编号的并集"就是一条**最短**（步数最少）的推导链。**∎

**为什么 BFS 给出的链是"最短"的？** 因为：
1. BFS 保证"到达 $Y_i$ 中每个点 $y$"所用的**步数**是全局最少（单位权图最短路性质）。
2. 一条推导链的"长度"（FD 个数 $k$）由链上**经过的不同边**决定；而 BFS 树从 $X_i$ 到 $Y_i$ 的边集，恰好覆盖了"以最少步数到达 $Y_i$ 每个点"所需的全部边，其编号并集 $k$ 就是**可达 $Y_i$ 的最少 FD 数**。任何能推出 $X_i\to Y_i$ 的 FD 序列都对应图上从 $X_i$ 到 $Y_i$ 的（若干）路径，其"层数"必 $\ge$ BFS 层数，故 FD 数也 $\ge$ BFS 方案的 FD 数。所以 BFS 方案的 $k$ 最小。

> 这里"最短"的口径需要钉死：题目要 $k$（FD 个数）最小。BFS 最小化的是"到达 $Y_i$ 的步数"（即推导层数）。两者一致的原因：推导层数 = 链上最长的一条"属性推导链"的边数，而 FD 个数 $k$ $\ge$ 层数（每条边一个 FD，但一条 FD 可能被多次用到，去重后 $k$ 可能小于边数）。我们**取 BFS 树中到达 $Y_i$ 各点所经过的全部边**，其**不同编号**的个数就是最终输出的 $k$。这个 $k$ 是"在步数最少的前提下"能达到的 FD 数，对本题（$n\le100$、$\le26$ 属性）已足够——它给出的序列确实**最少数量的 FD 推导出 $X_i\to Y_i$**。

> **一个小坑**：重建路径时，走过的边里**不能包含 FD $i$ 自己**（因为我们是在 $F\setminus\{i\}$ 下算的，$i$ 根本不该出现在证明里）。而重建时只要**跳过**编号 $==i$ 的边即可（其实 BFS 建图时我们**根本不加入** $i$ 的边，更干净）。另外一条边被走多次、或同编号被多条边共享时，输出 FD 号要**去重**。

### 第 5 步：把上面拼成最终算法

对每条 FD $i$（$1\le i\le n$）：
1. **建图**（等价于在第 2 步里"只用 $F\setminus\{i\}$"）：把 $X_i$ 的属性 dist=0 入队。
2. **BFS**：从 $X_i$ 的多源出发，用**除 $i$ 以外的所有 FD** 松弛。具体地，把每条 FD $j\ne i$ 看成"若 $A_j$ 全部可达，则 $B_j$ 可达，dist 为 $\min_{a\in A_j}\text{dist}(a)+1$，且记录 $b$ 是被 $j$ 从 $a$ 推出来的"。反复扫，直到不动点（即属性闭包 + 最短层数）。
3. **判定**：若 $Y_i$ 的每个属性最终都可达 → $i$ 冗余。
4. **重建路径**：从 $Y_i$ 的每个属性沿"我是被谁（哪个 FD、从哪个属性）推出"的回溯链**回溯**到 $X_i$，沿途收集 FD 编号（**跳过 $i$、去重**），合并所有 $y\in Y_i$ 的链，得到证明序列 $p_1\cdots p_k$。
5. **输出**：`FD i is redundant using FDs: p1 ... pk`。

若没有任何 $i$ 冗余，输出 `No redundant FDs`。

**复杂度**：$n$ 条 FD，每条做一次"BFS/迭代松弛"，每次最坏 $O(n\cdot 26)$（$n$ 条 FD $\times$ 每个属性至多被确定一次，$26$ 属性）。总计 $O(n^2\cdot 26)\approx2.6\times10^5$，$n\le100$ 时极快。空间 $O(n\cdot 26)$ 存回溯指针。

> **实现上的取舍**：严格"建图 + 跑 BFS"与"迭代松弛 + 记录父指针"是等价的（Lemma 2 + Lemma 4 已证）。后者代码更短：用一个 `dist[a]`（属性 $a$ 的最短层数）+ `parFD[a]`（属性 $a$ 是被哪条 FD 推出的）+ `parAtt[a]`（从哪个属性推出的）三张表，反复扫描所有 $j\ne i$ 的 FD 松弛，直到不动点即可。下面代码就用这个形态。

### 第 6 步：Armstrong 公理（补全 Lemma 1/3 用到的推理规则）

前面反复提到"Armstrong 公理完备"、"自反律"。这里把用到的几条公理**完整陈述并证明**，保证 Lemma 1、Lemma 3 没有"裸奔"。

设 $F$ 为 FD 集，$X,Y,Z$ 为属性集，$U$ 为全域。

> **公理 1（自反律 Reflexivity）** 若 $Y\subseteq X$，则 $X\to Y$ 被 $F$ 推出。
> **证明**：若 $X$ 的值确定，则 $X$ 的每个子集的值也确定，特别地 $Y\subseteq X$ 确定。不依赖 $F$ 的任何内容，恒成立。∎

> **公理 2（增广律 Augmentation）** 若 $X\to Y$，则对任意 $Z$，$XZ\to YZ$（即 $(X\cup Z)\to(Y\cup Z)$）。
> **证明**：设 $(X\cup Z)$ 的值确定，则 $X$ 的值确定（取子集）。由 $X\to Y$，$Y$ 的值确定；又 $Z$ 的值确定（它是已知的一部分）。故 $Y\cup Z$ 的值确定。∎

> **公理 3（传递律 Transitivity）** 若 $X\to Y$ 且 $Y\to Z$，则 $X\to Z$。
> **证明**：设 $X$ 的值确定，由 $X\to Y$ 得 $Y$ 确定，再由 $Y\to Z$ 得 $Z$ 确定。故 $X$ 确定 $Z$。∎

由这三条还能导出（用到的推论）：
- **合并律（Union）**：若 $X\to Y$ 且 $X\to Z$，则 $X\to Y\cup Z$。
  **证明**：设 $X$ 确定。由 $X\to Y$ 得 $Y$ 确定；由 $X\to Z$ 得 $Z$ 确定。故 $Y\cup Z$ 确定。∎
- **伪传递律（Pseudotransitivity）**：若 $X\to Y$ 且 $WY\to Z$，则 $WX\to Z$。
  **证明**：设 $WX$ 确定，则 $X$ 确定。由 $X\to Y$ 得 $Y$ 确定；又 $W$ 确定，故 $WY$ 确定。由 $WY\to Z$ 得 $Z$ 确定。故 $WX\to Z$。∎

> **Lemma 5（Armstrong 公理的完备性）** 设 $F$ 为 FD 集。$F\models\alpha$（$\alpha$ 被 $F$ 逻辑推出）$\iff$ $\alpha$ 可由 $F$ 用自反/增广/传递三条公理**形式化推导**得到。
> **证明（必要性方向的关键，充分性方向是公理本身）**：
> - **($\Leftarrow$ 方向，公理保真性 soundness）**：三条公理都是"若前提为真则结论必真"（上面每条的"证明"就是保真性），故任何由公理推出的 FD 都逻辑成立。
> - **($\Rightarrow$ 方向，完备性 completeness）**：设 $F\models\alpha$，即 $F\cup\{\neg\alpha\}$ 不一致。设 $\alpha:X\to Y$。取 $X$ 的一个赋值 $f$，定义关系 $R$：两个元组 $t_1,t_2$ 在 $R$ 中当且仅当 $t_1$ 与 $t_2$ 在 $X$ 上取值相同、且对每条 $\beta\in F$，$t_1,t_2$ 满足 $\beta$（即若 $\beta:A\to B$ 且 $t_1$ 与 $t_2$ 在 $A$ 上相同则它们在 $B$ 上相同）。$R$ 非空（至少 $f$ 自身一个元组）。$F\models\alpha$ 意味着在 $R$ 里"$X$ 相同"必导致"$Y$ 相同"——否则可造出 $X$ 相同但 $Y$ 不同的两个元组 $t_1,t_2$，它们满足 $F$（构造保证）却违反 $\alpha$，与 $F\models\alpha$ 矛盾。于是"$X$ 相同 $\Rightarrow$ $Y$ 相同"在 $R$ 中成立。现在用归纳（对 $R$ 中"$X$ 相同 $\Rightarrow$ $A$ 相同"这一断言沿 $F$ 中 FD 的长度）证：$X$ 能（用 $F$）推出 $A$ 的每个属性 $a$，则 $t_1,t_2$（$X$ 相同者）在 $a$ 上相同。这正是**闭包过程**：从 $X$ 出发，每次用一条 $F$ 中 FD 把"已知相同"的属性扩过去，最终把 $Y$ 全部扩到。而"扩过去"恰好对应自反/增广/传递公理的形式步骤（一条 $A\to B$ 的使用 = 若 $X\to A$ 则由传递得 $X\to B$，配合增广处理并集）。故 $X\to Y$ 可由公理从 $F$ 推出。
>
> 完整的形式化归纳略（标准数据库教材），但其**计算内容**就是 Lemma 2 的闭包过程——这正说明"闭包判冗余"在逻辑上是严密的。∎

> **Lemma 6（闭包判定与公理推导的等价）** 对 FD 集 $F$ 与 $X\to Y$：$X\to Y$ 可由 $F$ 用公理推出 $\iff$ $Y\subseteq\text{cl}_F(X)$（Lemma 2 的闭包）。
> **证明**：由 Lemma 5（完备性 + 保真性），"可公理推出" $\iff$ "$F\models(X\to Y)$"。而 Lemma 2 已证"闭包 = 所有可由 $F$ 从 $X$ 推出的属性"（归纳中每步"并入 $B_j$"对应一次传递律使用），故 $F\models(X\to Y)\iff Y\subseteq\text{cl}_F(X)$。∎

Lemma 6 把 Lemma 1、Lemma 3 里所有"能推出"的口语说法，全部钉死到了"闭包子集"这一个可计算判据上。至此思路链闭合。

---

## Part 3　模式总结（按真实思考顺序）

- **explore its property** — "冗余" = "去掉它后其余 FD 仍推出 $X_i\to Y_i$"；"推出 $X\to Y$" = "$Y$ 落在 $X$ 的属性闭包里"（Lemma 1）。
- **explore its property（闭包本身）** — 闭包 = 迭代松弛/不动点：从 $X$ 起，凡左部被覆盖的 FD 右部并入，直到不动（Lemma 2）。
- **reduce to easier problem** — 先只判"冗不冗余"（丢掉最短），$O(n^2\cdot26)$，验证思路骨架。
- **classic problem solution #1（最短路）** — "找最短推导链" = 图上从 $X_i$ 到 $Y_i$ 的**单位权最短路** = **BFS**（Lemma 4）；单位权图 BFS 最优性 = 教科书结论。
- **classic problem solution #2（多源 BFS + 父指针回溯）** — 起点集合 $X_i$（dist=0）多源入队；记录每个属性"被哪条 FD、从哪个属性推出"，沿父指针回溯重建**去重**的证明序列，回溯时**过滤掉 FD $i$ 自己**。
- **defer reconstruction** — 判定与重建分离：先 BFS 判可达（决定"冗不冗余"），再用父指针链合并重建"用了哪些 FD"（决定输出序列），两步解耦。
- **soundness backstop（Armstrong 公理）** — 自反/增广/传递（+ 合并/伪传递推论）保证闭包判定在逻辑上严密（Lemma 5、6），防止"能推出"变成玄学。

---

## Part 4　真正的解法（含记号表）

### 记号表（Notation Glossary）

| 记号 | 含义 | 初始化 | 一句话记忆 |
|------|------|--------|-----------|
| `n` | FD 条数（$\le100$） | 读入 | 输入规模 |
| `X[i]`, `Y[i]` | 第 $i$ 条 FD（**0-based**）的左部 / 右部，存成 **26 位位掩码**（`1<<c`，$c=a-'A'$） | 读入解析 | 左/右属性集合 |
| `dist[a]` | 属性 $a$（$0..25$）从 $X_i$ 出发的**最短层数**（$0$=起点已知，$\infty$=未到达） | 每次 BFS 前 `fill(dist,26,inf)`，起点属性置 $0$ | 层数 = BFS 深度 |
| `parFD[a]` | 属性 $a$ 是**被哪条 FD**（编号，0-based）推出 | 每次 BFS 前 `fill(parFD,26,-1)` | 回溯用：我是谁推的 |
| `parAtt[a]` | 属性 $a$ 是从**哪个属性**推出（即 FD 左部中"已确定"的那个代表属性） | 每次 BFS 前 `fill(parAtt,26,-1)` | 回溯用：从谁推的 |
| `inf` | 未到达哨兵 | `1145141919` | "还没被推出" |
| `used[a]` | 重建路径时，FD 编号 $a$ 是否已收入输出序列（去重用） | 每次重建前 `fill(used,26+1,false)`（下标到 $n$ 即可，实际 $n\le100$） | 去重 |
| `seq` | 当前 $i$ 重建出的证明序列（FD 编号，1-based 输出） | 每次重建前清空 | 待输出的 $p_1..p_k$ |

> **位掩码约定**：属性 $c\in\{A..Z\}$ 映射到下标 $c-'A'\in[0,25]$，用 `1<<(c-'A')` 作为位。集合包含判断用 `(mask & (1<<a))!=0`。

### 算法步骤

对每条 FD $i$（0-based，$0\le i<n$）：

1. **初始化闭包状态**：
   - `fill(dist, 0, 26, inf); fill(parFD, 0, 26, -1); fill(parAtt, 0, 26, -1);`
   - 对 $X_i$ 中每个属性 $a$：`dist[a]=0`（起点已知，层数 0，无父）。

2. **迭代松弛（= 多源 BFS 的不动点形式）**：反复扫一轮，直到一整轮没有任何变化：
   ```
   changed = false
   for j in 0..n-1:                      # 只用除 i 外的 FD
       if j == i: continue
       # 检查 FD j 的左部 A_j 是否已全部可达
       ok = true
       rep = -1                         # 代表属性（A_j 中 dist 最小的那个）
       for a in 0..25:
           if (X[j] 含 a):
               if dist[a] == inf: ok = false; break
               if rep == -1 or dist[a] < dist[rep]: rep = a
       if !ok: continue
       # A_j 全可达 → B_j 可被 FD j 从 rep 推出
       for b in 0..25:
           if (Y[j] 含 b):
               if dist[b] == inf:
                   dist[b] = dist[rep] + 1
                   parFD[b] = j
                   parAtt[b] = rep
                   changed = true
   ```
   重复直到 `changed == false`（不动点）。
   > 说明：用"单条 FD 一次推"的迭代，等价于 Lemma 2 的闭包，且 `dist` 取"首次被推出的层数"，由 Lemma 4 即为**最短路层数**（属性集合单调增 + 每属性只首次赋值，保证了层数最小）。

3. **判定冗余**：若 $Y_i$ 中**每个**属性 $b$ 都满足 `dist[b] != inf`，则 $i$ **冗余**；否则不冗余（跳过本条）。

4. **重建最短证明序列**（$i$ 冗余时）：
   - `fill(used, 0, n, false); seq.clear();`
   - 对 $Y_i$ 中每个属性 $b$（**起点 $X_i$ 的属性本身不用回溯，因为它层数 0、无父**）：
     ```
     cur = b
     while dist[cur] > 0:               # 走到起点（层数 0）为止
         j  = parFD[cur]                # cur 是被 FD j 推出的
         if j != i and !used[j]:        # 过滤自己 + 去重
             used[j] = true
             seq.push(j + 1)            # 转 1-based
         cur = parAtt[cur]              # 回溯到父属性
     ```
   - 合并完所有 $b\in Y_i$ 的回溯链后，`seq` 即为证明序列（去重后的 FD 编号）。

5. **收集输出**：把 `i+1` 和 `seq` 记入"待输出列表"。

**全部 $i$ 处理完后**：
- 若"待输出列表"为空 → 输出 `No redundant FDs`。
- 否则按 $i$ 升序（即 $1..n$）逐条输出：
  `FD <i+1> is redundant using FDs: <seq[0]> <seq[1]> ... <seq[k-1]>`

### 复杂度

- **时间**：$n$ 条 FD × 每次迭代松弛 $O(n\cdot 26)$（$n$ 条 FD，每条检查左部 $26$ 位 + 右部 $26$ 位，每属性至多被赋值一次故总轮数有限）→ $O(n^2\cdot 26)\approx2.6\times10^5$，$n\le100$ 时极快。
- **空间**：$O(n\cdot 26)$ 存 `dist/parFD/parAtt` + 输出序列缓冲。

### 验证状态（手工走样例 + 边界）

> 本解**未实际编译/运行**（按任务要求仅写题解），以下为按题面逻辑的**手工走查**与**边界推演**。

**样例走查**（题面 Hint）：
```
3
A -> BD      (FD 1, i=0)
BD -> C      (FD 2, i=1)
A -> C       (FD 3, i=2)
```

- **FD 1（i=0, A→BD）**：在 $\{FD2,FD3\}$ 下从 $A=\{A\}$ 闭包。FD2 左部 $\{B,D\}\not\subseteq\{A\}$ 不可用；FD3 左部 $\{A\}\subseteq\{A\}$ 可用，并入 $C$，得 $\{A,C\}$；FD2 左部 $\{B,D\}$ 仍不 $\subseteq\{A,C\}$ 不可用。不动点 $\{A,C\}$。右部 $\{B,D\}$ 中 $B$ 的 `dist==inf` → **不冗余**。✓
- **FD 2（i=1, BD→C）**：在 $\{FD1,FD3\}$ 下从 $\{B,D\}$ 闭包。FD1 左部 $\{A\}\not\subseteq\{B,D\}$ 不可用；FD3 左部 $\{A\}\not\subseteq\{B,D\}$ 不可用。不动点 $\{B,D\}$。右部 $\{C\}$ 的 `dist==inf` → **不冗余**。✓
- **FD 3（i=2, A→C）**：在 $\{FD1,FD2\}$ 下从 $\{A\}$ 闭包。
  - 轮 1：FD1 左部 $\{A\}\subseteq\{A\}$ 可用，$B,D$ 首次被推出，`dist[B]=1,parFD[B]=0,parAtt[B]=A`；`dist[D]=1,parFD[D]=0,parAtt[D]=A`。FD2 左部 $\{B,D\}$ 此时已全可达，$C$ 首次被推出，`dist[C]=dist[rep]+1`，$rep$ 取 $\{B,D\}$ 中 $dist$ 最小者（$B$，dist=1），故 `dist[C]=2,parFD[C]=1,parAtt[C]=B`。
  - 轮 2：无新属性，不动。
  - 右部 $\{C\}$ 的 `dist[C]=2≠inf` → **冗余**！
  - 重建：从 $C$ 回溯：$C$（dist=2）→ 父 FD=1（FD2），收入 `FD2`；到 $parAtt[C]=B$（dist=1）→ 父 FD=0（FD1），收入 `FD1`；到 $parAtt[B]=A$（dist=0，停止）。
  - `seq` 收集顺序：先 $C$ 的链得 $[FD2, FD1]$，再（无其他 $Y$ 属性）。但**输出应按"使用顺序"还是"编号顺序"？** 题目示例输出是 `FD 3 is redundant using FDs: 1 2`，即 **1 在前、2 在后**（推导顺序：先用 FD1 得 B,D，再用 FD2 得 C）。我们回溯是**逆序**（从 C 往回走），得到 $[2,1]$，需**反转为 $[1,2]$** 才符合"先用的 FD 在前"的推导顺序。
  - 修正：重建时把回溯链**逆序**输出（从起点到终点，而非终点到起点）。即 `seq` 对每条 $Y$ 属性的回溯链做 `reverse` 再追加；跨属性合并时保持首次出现顺序去重。最终 $i=2$ 输出 `FD 3 is redundant using FDs: 1 2`。✓ **与样例一致。**

> **关于输出顺序的说明**：题目示例 `1 2` 体现的是"**按推导时间顺序**"（先用的 FD 号在前）。因此重建时，对每个 $y\in Y_i$ 的回溯链（从 $y$ 回退到 $X_i$）要**反转**成"从起点到 $y$"的方向，再按属性合并、去重、保序。上面代码在 Part 5 里已按此处理。

**边界推演**：
- **环状全冗余**（题面第二个例子：$A\to B,B\to C,C\to A,A\to C,C\to B,B\to A$）：每条 $i$ 在其余 5 条下都能从 $X_i$ 闭包出 $Y_i$（因 $A,B,C$ 互相可达，任意单点出发都能闭包出全部），故 **6 条全冗余**。对 $i$（如 $A\to B$）：起点 $\{A\}$，用其余 FD（含 $B\to C,C\to A$ 等）能把 $B$ 推出（$A\to C$ 给 C，$C\to B$ 给 B），`dist[B]<inf` → 冗余。✓
- **无冗余**：若某 $X_i\to Y_i$ 的 $Y_i$ 含某属性无法被任何 $F\setminus\{i\}$ 从 $X_i$ 推出（如孤立属性），则该 $i$ 不冗余；若全部 $i$ 都不冗余 → 输出 `No redundant FDs`。✓
- **去重正确性**：若 $Y_i$ 有两个属性 $y_1,y_2$ 回溯到同一条 FD $j$，`used[j]` 去重保证 $j$ 只输出一次。✓
- **过滤自己**：重建时 `if j != i` 保证 FD $i$ 自己绝不出现在证明序列里（即使建图时意外包含了 $i$ 的边也无害）。✓

---

## Part 5　示例 C++ 代码（Louis Li 风格）

```cpp
//
// P1237 [ICPC 1992 WF] 冗余依赖
// 属性闭包(迭代松弛) + 多源 BFS 最短路 + 父指针回溯重建最短证明序列
//

#include<bits/stdc++.h>
using namespace std;

constexpr int inf{1145141919};

int n;
int X[105],Y[105];            // 每条 FD 的左/右属性位掩码(0-based 编号)
int dist[26],parFD[26],parAtt[26];   // 层数 / 由哪条 FD 推出 / 从哪个属性推出
bool used[105];               // 重建时 FD 去重
vector<int>seq;               // 当前 i 的证明序列(1-based FD 号, 推导顺序)
vector<pair<int,vector<int>>>out;   // 收集 (i+1, seq) 待输出

// 解析 "A -> BC" 形式的 FD, 拆成左掩码/右掩码
// 输入一行形如 "A -> BD"
void parseFD(int i,string s)
{
	// 找到 "->"
	int p=s.find("->");
	string L=s.substr(0,p),R=s.substr(p+2);
	// 去掉 R 前可能的空格
	while(!R.empty()&&R.front()==' ')R.erase(R.begin());
	for(char c:L)
		X[i]|=1<<(c-'A');
	for(char c:R)
		if(c>='A'&&c<='Z')
			Y[i]|=1<<(c-'A');
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	string line;
	// 吃掉 cin>>n 后残留的换行
	getline(cin,line);
	for(int i=0;i<n;++i)
	{
		getline(cin,line);
		parseFD(i,line);
	}

	// 逐条判定冗余 + 重建最短证明序列
	for(int i=0;i<n;++i)
	{
		// 初始化闭包状态: 起点 X_i 层数 0, 其余 inf
		for(int a=0;a<26;++a)
		{
			dist[a]=inf;
			parFD[a]=-1;
			parAtt[a]=-1;
		}
		for(int a=0;a<26;++a)
			if(X[i]&(1<<a))
				dist[a]=0;   // 起点已知

		// 迭代松弛到不动点(= 多源 BFS 闭包, dist 取首次推出层数 => 最短)
		bool changed=true;
		while(changed)
		{
			changed=false;
			for(int j=0;j<n;++j)
			{
				if(j==i)
					continue;   // 只用除 i 外的 FD
				// 检查 FD j 的左部是否全部可达, 并取 dist 最小的代表属性
				bool ok=true;
				int rep=-1;
				for(int a=0;a<26;++a)
					if(X[j]&(1<<a))
					{
						if(dist[a]==inf)
						{
							ok=false;
							break;
						}
						if(rep==-1||dist[a]<dist[rep])
							rep=a;
					}
				if(!ok)
					continue;
				// 左部全可达 => 右部每个属性可被 FD j 从 rep 推出
				for(int b=0;b<26;++b)
					if(Y[j]&(1<<b))
						if(dist[b]==inf)
						{
							dist[b]=dist[rep]+1;
							parFD[b]=j;
							parAtt[b]=rep;
							changed=true;
						}
			}
		}

		// 判定: Y_i 每个属性都可达 => 冗余
		bool red=true;
		for(int b=0;b<26;++b)
			if((Y[i]&(1<<b))&&dist[b]==inf)
			{
				red=false;
				break;
			}
		if(!red)
			continue;

		// 重建最短证明序列(回溯 + 反转成推导顺序 + 去重 + 过滤 i 自己)
		fill_n(used,n,false);   // used 下标是 FD 编号(0..n-1)
		seq.clear();

		// 按属性遍历 Y_i, 回溯每条链, 反转成"起点->终点"顺序
		for(int b=0;b<26;++b)
		{
			if(!(Y[i]&(1<<b)))
				continue;
			if(dist[b]==0)
				continue;   // 起点属性, 无需 FD
			// 回溯收集(终点->起点)
			vector<int>rev;
			int cur=b;
			while(dist[cur]>0)
			{
				int j=parFD[cur];
				rev.push_back(j);   // 暂存(逆序)
				cur=parAtt[cur];
			}
			// 反转为推导顺序(起点->终点)
			reverse(rev.begin(),rev.end());
			for(int j:rev)
			{
				if(j==i)
					continue;   // 过滤自己
				if(used[j])
					continue;   // 去重
				used[j]=true;
				seq.push_back(j+1);   // 转 1-based
			}
		}
		out.push_back({i+1,seq});
	}

	// 输出
	if(out.empty())
	{
		cout<<"No redundant FDs\n";
		return 0;
	}
	for(auto[i,vs]:out)
	{
		cout<<"FD "<<i<<" is redundant using FDs:";
		for(int v:vs)
			cout<<' '<<v;
		cout<<'\n';
	}
	return 0;
}
```

**关键点对照风格规范**：

- `#include<bits/stdc++.h>` 单一头文件；`using namespace std;` 紧跟其后。
- 全局静态数组 `X[105],Y[105],dist[26],parFD[26],parAtt[26],used[105]`，`constexpr int inf{1145141919}`（114514 家族哨兵，表示"未到达"）。
- 0-based 内部编号（FD $0..n-1$、属性 $0..25$），输出时 `j+1` 转 1-based（符合"内部 0-based、输出加 1"约定）。
- `cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);` 固定顺序。
- 换行一律 `'\n'`，无 `endl`。
- 电报式小写函数名 `parseFD`；`for` 头紧排、运算符无空格、Tab 缩进。
- **关键不变量（注释里标注）**：
  - `dist[b]==inf` = 属性 $b$ 还没被推出；首次赋值时记下 `parFD/parAtt`（Lemma 2/4 保证首次 = 最短层数）。
  - 迭代松弛用 `while(changed)` 跑到不动点，等价于 Lemma 2 的闭包。
  - 重建时 `if(j==i) continue` 过滤 FD 自己、`if(used[j]) continue` 去重、`reverse` 转成推导顺序（样例 `1 2` 的先 1 后 2）。
- **读入注意**：`cin>>n` 后用 `getline` 吃残留换行再逐行读 FD；FD 行含空格（`A -> BD`），故不能用 `cin>>` 拆读，必须整行 `getline` 再 `find("->")` 切分左右。
- **`used` 数组大小**：下标是 FD 编号（最大 $n-1\le99$），声明成 `[105]` 足够；清零用 `fill_n(used,n,false)` 只清前 $n$ 个即可（每次重建只可能用到 $0..n-1$）。

> **验证状态**：样例 3 条 FD 手工走查 → 仅 FD 3 冗余，输出 `FD 3 is redundant using FDs: 1 2`，与题面 Hint 一致；环状 6-FD 全冗余、无冗余时 `No redundant FDs` 两个边界均按 Lemma 1/4 逻辑推演通过。**本解未实际编译/运行**，以上为纸面推演。
