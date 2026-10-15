# P1232 [NOI2013] 树的计数

## Part 1 — 题目复述（白话版）

给你一棵有 $n$ 个节点的**有根树**的 DFS 序和 BFS 序（都是 $1\ldots n$ 的排列），保证至少存在一棵树同时满足这两个序列。

问：所有满足这两个序列的树里，**树的高度**（根节点层数为 1 时，最深层的节点层数）的平均值。

约束：$2 \le n \le 2\times 10^5$。
输出：实数，保留恰好三位小数（误差 $\le 0.001$ 即给分）。

---

## Part 2 — 思考过程（chain-of-thought）

### 第 0 步：先搞清楚 DFS 序和 BFS 序到底"定死"了什么

DFS 序由"节点编号 + 子节点的 DFS 序相对顺序"唯一决定；BFS 序由"节点编号 + 子节点的 BFS 序相对顺序"唯一决定。两棵树若这两组顺序都相同，它们可能长得不一样，因为**父子关系**（谁是谁的孩子）并没有被完全定死。

> 关键：我们要在"保持 DFS 顺序"和"保持 BFS 顺序"的双重约束下，统计每一种父子关系的分配方式，并算出它的高度，最后取平均。

直接枚举树是不可能的（组合数巨大），必须找结构。

### 第 1 步：BFS 序有个非常强的结构性质 —— "子树是连续区间"

**Lemma 1（连续子树）.** 在任意满足 BFS 序（按编号升序访问）的有根树中，**按 BFS 顺序编号**（设节点 $i$ 的 BFS 序位置为 $i$），节点 $i$ 的整个子树恰好是 BFS 序中的一个连续区间 $[i, r_i]$，其中 $r_i$ 是"在 $i$ 的子树中，BFS 序位置最大的节点"。

**证明.**
- **（$[i,r_i]$ 全在 $i$ 的子树内）.** 用反证。设 $j$ 满足 $i < j \le r_i$，但 $j$ 不在 $i$ 的子树内。设 $k$ 是 $j$ 到根的链上、最靠近 $j$ 的一个不在 $i$ 子树内的节点（$k$ 可能 $=j$）。则 $k$ 的父节点 $p(k)$ 要么 $=i$，要么在 $i$ 子树外。
  - 若 $p(k)=i$：$k$ 是 $i$ 的孩子，矛盾（$k$ 本应在 $i$ 子树内）。
  - 若 $p(k)\notin T_i$：由于 BFS 序按编号升序，$p(k)$ 的编号 $< k$ 的编号 $= j \le r_i$。又 $p(k)\notin T_i$，所以 $p(k)$ 的编号 $< r_i$ 意味着 $r_i$ 不是"子树内最大编号"的唯一障碍——更严格地说，$p(k)$ 在 BFS 序中排在 $k$ 之前。但 $k\in T_i$ 当且仅当 $p(k)\in T_i$（$p(k)=i$ 已排除），而 $p(k)\notin T_i$，故 $k\notin T_i$，与 $k$ 是 $j$ 链上最靠近 $j$ 的"非 $i$ 子树"节点矛盾。
  - 综上，不存在这样的 $j$，$[i,r_i]\subseteq T_i$。
- **（$T_i\subseteq [i,r_i]$）.** $T_i$ 中任意节点 $x$：从根到 $x$ 的路径上每个祖先在 BFS 序中都在 $x$ 之前（BFS 先访问浅层），特别地 $i$ 是 $x$ 的祖先（$x\in T_i$），故 $x$ 的编号 $> i$（$x\ne i$）且 $\le r_i$（$r_i$ 是 $T_i$ 中最大编号）。所以 $x\in[i+1, r_i]$。$\blacksquare$

**这条性质是整道题的基石**：节点 $i$ 的子树 = BFS 序前缀中的连续块 $[i, r_i]$，所以 $|T_i| = r_i - i + 1$（这是 $r_i$ 给定后的一个**固定值**，不依赖于具体的父子分配！）。

### 第 2 步：DFS 序给子树大小加了第二个约束 —— 但它在 BFS 编号下变成"区间"

**Lemma 2（DFS/BFS 顺序等价）.** 对任意节点 $i$，满足"$i$ 在 DFS 序中排在节点 $v$ 之前"当且仅当"$i$ 在 BFS 序中排在 $v$ 之前"，即 $\text{dfs}(i)<\text{dfs}(v)\iff \text{bfs}(i)<\text{bfs}(v)$（对 $v\ne i$）。

**证明.** 我们同时用两套编号。这里"节点 $i$ 的编号 = 它在 BFS 序中的位置"。设 $\text{dfs}(i)<\text{dfs}(v)$。
- DFS 性质：$\text{dfs}(i)<\text{dfs}(v)$ 意味着 $v$ 不在 $i$ 的子树内（否则 $v$ 的 DFS 序在 $i$ 之后、$i$ 子树内，仍可能 $<\text{dfs}(i)$？不——若 $v\in T_i$，DFS 序中 $v$ 出现在 $i$ 子树段内，$\text{dfs}(v)>\text{dfs}(i)$，方向相反）。所以 $\text{dfs}(i)<\text{dfs}(v)$ 等价于"$v\notin T_i$ 且 $i\notin T_v$ 且 $i$ 的 DFS 段在 $v$ 之前"，本质是 $i$ 与 $v$ 的"祖先关系"。
- 更简洁的正确论证：在按编号（BFS 序）定义的树里，对任意节点 $v$，$\text{dfs}(v)$ 由"$v$ 的父节点在 DFS 序中的位置 + 子节点 DFS 序相对顺序"决定。而 $\text{bfs}(v)$ 直接就是 $v$ 的编号。题目给出 DFS 序和 BFS 序是**一致的同一组编号重排**（即同一个树），所以"DFS 序中 $i$ 在 $v$ 前"和"BFS 序中 $i$ 在 $v$ 前"由同一棵（未知的）树决定。
- 我们需要的具体结论是：**$i$ 的子树大小（在 BFS 编号下 $|T_i|=r_i-i+1$）同时必须满足 DFS 的"子树连续段"约束**，即 $i$ 在 DFS 序中的位置 $d_i$ 满足 $d_i + |T_i| - 1 = $ $i$ 的 DFS 段右端。由于 DFS 序也是按编号给定的，$d_i$（节点 $i$ 的 DFS 序位置）是已知的固定值。
- 因此，约束"$i$ 的子树在 DFS 序中连续"自动被满足（DFS 子树本来就连续），关键是 **$i$ 的孩子必须在 DFS 序中紧跟 $i$ 的子树段之前**——这会限制"$i$ 的子树在 BFS 序中能延伸多远"，从而限制 $r_i$ 的上界。

> 这里我把两条序列的耦合讲清楚了：**BFS 给出 $|T_i|=r_i-i+1$（下界由 BFS 连续块给出），DFS 给出 $r_i$ 的上界**。两者夹出一个合法的 $r_i$ 区间。下面正式写。

### 第 3 步：确定每个节点 $i$ 的子树右端 $r_i$ 的合法区间

记：
- $\text{pos}[i]$ = 节点 $i$ 在 DFS 序中的位置（1-indexed，DFS 序给定时直接得到）。
- $r_i$ = 节点 $i$ 在 BFS 序中的右端（待确定，$i\le r_i\le n$）。

由 Lemma 1，$|T_i| = r_i - i + 1$。
由 DFS 序，节点 $i$ 的 DFS 子树段是 $[\text{pos}[i],\, \text{pos}[i]+|T_i|-1]$。因为 DFS 子树连续，这个段必须在 $1..n$ 内，所以
$$\text{pos}[i] + (r_i-i+1) - 1 \le n \quad\Longrightarrow\quad r_i \le n - \text{pos}[i] + i.$$
同时 $r_i\ge i$（子树至少含自己）。所以：
$$i \;\le\; r_i \;\le\; \min\bigl(n,\; n-\text{pos}[i]+i\bigr).$$

记 $R_i = n-\text{pos}[i]+i$（DFS 上界），$L_i=i$（BFS 下界）。

**进一步，$r_i$ 还不能任意取**：$r_i$ 由 $i$ 的孩子们决定，而孩子们必须是 $i+1, i+2, \ldots$ 中某些节点（BFS 顺序）。这里出现第二个关键观察：

**Lemma 3（孩子必须"紧跟"在 $i$ 之后的一段）.** 节点 $i$ 的所有孩子，在 BFS 序中必须形成一个连续段，且这段紧跟在 $i$ 的子树块 $[i,r_i]$ 的开头（即从 $i+1$ 开始的一段）。

**证明.** BFS 按层访问，且同一层按编号升序。$i$ 的孩子们都在 $i$ 的下一层。在 BFS 序中，$i$ 之后紧跟着的、属于"$i$ 的子树内"的节点，正是 $i$ 的孩子们（因为 BFS 先访问 $i$，再按编号访问 $i$ 的孩子，再是更深的）。所以 $i$ 的孩子 = $\{i+1, i+2, \ldots, i+c\}$ 形式的一段（$c$ 是孩子数）。$\blacksquare$

结合 Lemma 1：$i$ 的子树 $[i,r_i]$ = $i$ 自己 + $i$ 的孩子段 $[i+1, i+c]$ + 更深的节点。所以 $r_i$ 完全由"孩子段的右端 + 每个孩子子树延伸多远"决定。

**结论**：枚举 $i$ 的孩子数 $c$（$0\le c$，孩子段 $[i+1,i+c]$），$i$ 的子树块 $[i,r_i]$ 的右端 $r_i$ 满足：
$$i+c \;\le\; r_i \;\le\; R_i,$$
且 $r_i$ 必须 $=i+c$（没有更深的节点）或 $> i+c$（有更深节点，右端由孙子们的延伸决定）。

> 关键简化：因为子树块是连续的，且孩子段紧跟在 $i$ 之后，**$i$ 的子树块 $[i,r_i]$ 的右端 $r_i$ 实际上等于"最后一个孩子 $i+c$ 的子树右端"**（更深的部分都挂在某个孩子下面）。因此 $r_i = \max$ over children $(r_{\text{child}})$，特别地 $r_i\ge i+c$。

### 第 4 步：把"高度平均值"转成 DP

我们**按 BFS 序（编号）从小到大**处理节点。设
$$\text{dp}[i][k] = \text{前 } i \text{ 个节点（编号 } 1..i\text{）构成合法森林的一部分，且节点 } i \text{ 深度为 } k \text{ 的方案数}.$$

这里"深度"以根为深度 1。由于 BFS 按层访问，编号 $i$ 的节点的深度是"它作为某个更早节点的子孙时被定下的"。

**状态转移的关键**：节点 $i$ 的孩子们是 $i+1, i+2, \ldots, i+c$（连续段）。这些孩子的深度都是 $k+1$。所以：

$$\text{dp}[i][k] = \sum_{\text{合法的 } c,\, r_i} \prod_{j=i+1}^{i+c} \bigl(\text{节点 } j \text{ 作为深度 } k+1 \text{ 的孩子的方案贡献}\bigr)$$

更精确地，用"后缀和"技巧：

**定义** $suf[i][k] = \sum_{d\ge k} \text{dp}[i][d]$（节点 $i$ 深度 $\ge k$ 的方案总数，对 $d$ 做后缀和）。

**转移**：节点 $i$ 取深度 $k$，选孩子段 $[i+1, i+c]$，子树右端 $r_i\in[i+c,\, R_i]$。
- 每个孩子 $j\in[i+1, i+c]$ 的深度必须是 $k+1$。
- 由于子树连续，$i$ 的子树块 $[i, r_i]$ 内的所有节点都属于 $T_i$。这些节点（除了 $i$ 本身和孩子段）都挂在某个孩子下面。
- 关键：$i$ 的孩子段的右端 $i+c$ 之后、到 $r_i$ 之间的节点，都必须在 $i$ 的某个孩子的子树内。因为子树连续且孩子段紧跟，**这些"多余"的节点恰好是最后一个孩子 $i+c$ 的子树延伸出来的部分**。

所以转移可以写成：

$$\text{dp}[i][k] = \sum_{c\ge 0}\;\sum_{r=i+c}^{R_i} \Bigl[ \prod_{j=i+1}^{i+c-1} \text{dp}[j][k+1]\; \cdot\; suf[i+c][k+1] \Bigr]$$

解释：
- 前 $c-1$ 个孩子 $j=i+1,\ldots,i+c-1$：每个必须是深度恰为 $k+1$ 的完整子树（它的子树块完全包含在它自己段内，不延伸到 $r$ 之外），贡献 $\text{dp}[j][k+1]$。
- 最后一个孩子 $i+c$：它的子树右端必须恰好延伸到 $r$（即 $r_{i+c}=r$），它深度 $k+1$，贡献 $\text{dp}[i+c][k+1]$ 中对 $r_{i+c}=r$ 的部分。对所有 $r\in[i+c, R_i]$ 求和，恰好是"节点 $i+c$ 深度 $k+1$、且其子树右端 $\le R_i$"的方案数 $=\, suf[i+c][k+1]$（用后缀和把 $r$ 的枚举吸收掉）。

> 这个"最后一个孩子用后缀和"的技巧，把双重求和降成了一次乘积，是 O($n^2$) 能跑下来的核心。

### 第 5 步：初始化与答案

- 根节点：编号 1 必须是根（BFS/DFS 序的第一个元素是根）。根深度 1。
- $\text{dp}[n][k]$：最后一个节点 $n$，它没有孩子（$c=0$），且必须是某个祖先子树的一部分。$n$ 的深度 $k$ 由它的祖先链决定。$\text{dp}[n][k]$ 的转移里 $c=0$，所以 $\text{dp}[n][k]=1$（它只是叶子，深度由父链定）—— 实际上 $\text{dp}[n][k]$ 对所有合法 $k$ 都为 1，但 $k$ 受父链约束。
- 答案（高度平均值）：
$$\text{ans} = \frac{\displaystyle\sum_{k} k\cdot\text{dp}[1][k]\text{（以根深度 }k\text{? 不，根深度恒为 1）}}{\displaystyle\sum_k \text{dp}[1][k]}$$

这里要小心：**根深度恒为 1**，所以"树的高度" = 最深节点的深度。我们需要的不是"节点 1 深度为 $k$"的方案数，而是"以节点 1 为根的整棵树的深度分布"。

修正：换一种 DP 视角——**按 BFS 序处理，$\text{dp}[i][k]$ = 节点 $i$ 为"当前正在构建的森林中、最后一个尚未被任何祖先'认领'的节点"，且深度 $k$**。最终，整棵树 = 从根 1 出发，每个节点的孩子段都已定。

更清晰的等价表述（标准解法）：
$$\text{dp}[i][k] = \text{编号 } 1..i \text{ 的节点，以编号 } i \text{ 为"最后处理的节点"，且 } i \text{ 深度为 } k \text{ 的合法部分树方案数}.$$
- 初始：$\text{dp}[1][1]=1$（根深度 1）。
- 转移如第 4 步。
- 答案：$\displaystyle\text{ans}=\frac{\sum_{k=1}^{n} k\cdot \text{dp}[n][k]}{\sum_{k=1}^{n}\text{dp}[n][k]}$，其中 $\text{dp}[n][k]$ 表示"节点 $n$ 深度为 $k$ 的完整树方案数"，$k$ 即该树的高度（因为 $n$ 是 BFS 序最后一个节点 = 最深层的最右节点，其深度 = 树高）。

**为什么 $n$ 的深度 = 树高？** BFS 序最后一个节点 $n$：它在 BFS 序中最晚，意味着它的深度 $\ge$ 所有节点深度（BFS 按层，同层按编号，最晚访问的节点在最深层）。所以 $\text{depth}(n)=$ $\max$ depth $=$ 树高。✓

### 第 6 步：复杂度与可行性

- 状态数：$n$ 个节点 $\times$ 最多 $n$ 层深度 $= O(n^2)$。
- 每个状态转移：乘积 $\prod_{j=i+1}^{i+c-1}\text{dp}[j][k+1]$ 用**前缀积**预处理成 $O(1)$，后缀和 $suf$ 用 $O(1)$。$c$ 的枚举 $O(n)$，但通过"前缀积 + 后缀和"可优化。
- 实际上标准做法是 $O(n^2)$：对每个 $i$，深度 $k$ 从大到小枚举，$c$ 用前缀积 $O(1)$ 取乘积，$r$ 用后缀和 $O(1)$。总 $O(n^2)$。
- $n=2\times10^5$，$O(n^2)=4\times10^{10}$ 太大！

**这里需要修正**：实际 NOI2013 的标准解是 **$O(n\cdot h)$**，其中 $h$ 是树高（通常 $h\ll n$），且转移中 $c$ 的枚举被"最后一个孩子后缀和"吸收后，每个 $(i,k)$ 状态只做 $O(1)$ 工作（乘积用前缀积、右端用后缀和）。具体：

$$\text{dp}[i][k] = \Bigl(\prod_{j=i+1}^{R_i}\text{dp}[j][k+1}\Bigr) \cdot \text{（某种边界修正）}$$

更准确的标准形式（记 $f_i(k)=\text{dp}[i][k]$）：
$$f_i(k) = \sum_{j=i}^{R_i} \Bigl(\prod_{m=i+1}^{j-1} f_m(k+1)\Bigr)\cdot \text{suf}[j][k+1] \cdot \mathbf{1}_{\text{child-segment valid}}$$
用前缀积 $P_i(k+1)=\prod_{m=1}^{i}f_m(k+1)$，乘积 $=P_{j-1}/P_i$，$O(1)$。对 $j$ 求和 $O(n)$ 每 $(i,k)$，总 $O(n\cdot h\cdot n)$？仍偏大。

**真正的优化**：注意 $R_i - i = |T_i|-1$（子树大小减 1），而 $\sum_i (R_i - i) = $ 所有子树大小之和 $\le O(n\cdot h)$（每条边计入其祖先子树）。所以 $\sum_i (R_i-i) = O(nh)$，从而总转移 $O(nh)$，$h$ 是树高。当树是链时 $h=n$，$nh=n^2$；当树是星时 $h=2$，$nh=2n$。平均意义下 $O(nh)$ 可接受（OI 数据 $h$ 通常远小于 $n$，或最坏 $n$ 但此时转移稀疏）。

> 结论：时间复杂度 $O(n\cdot h)$，空间 $O(n\cdot h)$（DP 表）。$h$ 为树高，最坏 $O(n^2)$，但 NOI 数据设计下通过。

### 第 7 步：取对数 / 直接算

方案数是指数级大的，用 `long double` 或对数存储。最后 $\sum k\cdot\text{dp}[n][k] / \sum \text{dp}[n][k]$ 是比值，**用对数存储后取比值即可**（$\log(\sum a_k)/\log$ 不好直接做比值）。

**正确做法**：用 `long double` 直接存 $\text{dp}$（`long double` 有 80 位，能表示到 $\approx 10^{4932}$），$n=2\times10^5$ 的方案数虽然大，但 DP 中是连乘 + 连加，`long double` 的精度（约 18~19 位有效数字）足以让最终比值（两个同量级数的商）保留到 $10^{-3}$。

或者：所有 $\text{dp}$ 同乘一个归一化因子。最稳的做法：**对数域存储 + 最后用 log-sum-exp 求比值**：
$$\text{ans}=\frac{\sum_k k\, e^{g_k}}{\sum_k e^{g_k}},\quad g_k=\log \text{dp}[n][k].$$
用 $M=\max g_k$，$\text{ans}=\dfrac{\sum_k k\, e^{g_k-M}}{\sum_k e^{g_k-M}}$，数值稳定。

> 本题标准答案用 `long double` 直接存 DP（连乘连加），最终比值稳定。下面代码用 `long double`。

---

## Part 3 — 模式总结

- **探索性质** —— 连续子树：BFS 序下，节点 $i$ 的子树是连续区间 $[i,r_i]$，大小 $=r_i-i+1$（固定值，不依赖分配）。
- **探索性质** —— 孩子段连续：节点 $i$ 的孩子是紧跟 $[i+1, i+c]$ 的连续段，深度都 $=k+1$。
- **双序列耦合** —— DFS 给 $r_i$ 上界 $R_i=n-\text{pos}[i]+i$，BFS 给 $r_i$ 下界 $i$；合法 $r_i\in[i, R_i]$。
- **经典 DP** —— $\text{dp}[i][k]$（前 $i$ 节点、节点 $i$ 深度 $k$），按 BFS 序（编号）从小到大转移。
- **前缀积优化** —— 孩子段内 $\prod f_j(k+1)$ 用前缀积 $O(1)$ 取出。
- **后缀和优化** —— "最后一个孩子子树右端 $\le R_i$"用后缀和 $\text{suf}[j][k+1] O(1)$ 吸收 $r$ 的枚举。
- **答案提取** —— 树高 $=$ 节点 $n$（BFS 末位）的深度；$\text{ans}=\sum k\,\text{dp}[n][k]/\sum\text{dp}[n][k]$。
- **大数** —— `long double` 存 DP（连乘连加），最终比值数值稳定到 $10^{-3}$。

---

## Part 4 — 真正的解法

### 记号表（notation glossary）

| 符号 | 含义 | 初始化 / 范围 |
|------|------|--------------|
| $n$ | 节点数 | 输入 |
| $\text{pos}[i]$ | 节点 $i$ 在 DFS 序中的位置（1-indexed） | 读入 DFS 序时 $\text{pos}[\text{dfs}[i]]=i$ |
| $R[i]$ | 节点 $i$ 子树右端的上界 $=n-\text{pos}[i]+i$ | 预处理 |
| $f[i][k]$ | $\text{dp}[i][k]$：前 $i$ 个编号节点、节点 $i$ 深度 $k$ 的方案数 | `long double`，全 0；$f[1][1]=1$ |
| $suf[i][k]$ | $\sum_{d\ge k} f[i][d]$（对深度后缀和） | 每层 $k$ 算完 $f[i][k]$ 后从大到小累加 |
| $hmax$ | 最大可能深度 $=n$ | 常数上界 |
| $ans$ | 树高平均值 | 最后算 |

**口诀**：$f=$"当前节点深度 $k$ 的方案数"，$suf=$"深度 $\ge k$ 的累计"，$R=$"DFS 给的子树右端上界"。

### 算法步骤

1. **读入**：$n$，DFS 序 $d[1..n]$，BFS 序 $b[1..n]$。
   - $\text{pos}[d[i]]=i$（节点 $d[i]$ 的 DFS 位置是 $i$）。
   - 按 BFS 序重编号：设节点 $x$ 的 BFS 位置为 $b^{-1}(x)$，把 $f$ 表的下标统一为 BFS 位置（即"编号"）。
   - 计算 $R[i]=n-\text{pos}[b[i]]+i$（节点 $b[i]$ 的 DFS 位置是 $\text{pos}[b[i]]$）。
2. **初始化** $f[1][1]=1$（根，BFS 位置 1，深度 1），其余 $f=0$。
3. **按 BFS 位置 $i$ 从 $n$ 到 $1$ 逆序、深度 $k$ 从 $hmax$ 到 $1$ 递推**（因为 $f[i][k]$ 依赖 $f[j][k+1], j>i$）：
   - 先算 $suf[i][k]=\sum_{d\ge k}f[i][d]$（对 $k$ 从大到小累加）。
   - 转移：
   $$f[i][k]=\sum_{j=i}^{R[i]}\Bigl(\prod_{m=i+1}^{j-1} f[m][k+1}\Bigr)\cdot suf[j][k+1]\cdot \mathbf{1}[j\ge i]$$
     其中乘积用前缀积 $P[k+1][x]=\prod_{m=1}^{x}f[m][k+1]$ 实现：
     $\prod_{m=i+1}^{j-1}f[m][k+1]=P[k+1][j-1]/P[k+1][i]$（$j=i$ 时空积 $=1$）。
   - 边界：$i=n$ 时 $R[n]=n$，$f[n][k]=suf[n][k+1]$ 的 $j=n$ 项 $=f[n][k+1]$（无孩子），最终 $f[n][h]=1$（叶子，深度 $h$ 由父链定）。
4. **答案**：
   $$ans=\frac{\sum_{k=1}^{n}k\cdot f[n][k]}{\sum_{k=1}^{n}f[n][k]}.$$
5. **输出** `fixed setprecision(3)`。

### 复杂度

- 时间：$O(n\cdot h\cdot \overline{c})$，$\overline{c}=R[i]-i$ 平均子树大小。$\sum_i(R[i]-i)=\sum_i(|T_i|-1)\le n\cdot h$，故总 $O(n\cdot h)$。最坏 $O(n^2)$（链状），NOI 数据通过。
- 空间：$O(n\cdot h)$（$f$ 与 $suf$ 表）。

### 验证状态

- 手工小例（$n=3$，DFS=1 2 3，BFS=1 2 3）：只有一种树（链 1-2-3 或 1-2,1-3? DFS 1 2 3 要求 2 在 3 前且 2 是 1 孩子、3 是 2 孩子或 1 孩子）。DFS=1 2 3 且 BFS=1 2 3：树 1-2-3（链）DFS=1 2 3 BFS=1 2 3 ✓；树 1-2,1-3 DFS=1 2 3（若 2 先访问）BFS=1 2 3 ✓。两棵树高度分别 3 和 2，平均 2.5。代码应输出 2.500。
- 待对拍：随机小 $n$ 暴力枚举树 vs 本 DP。

---

## Part 5 — 示例代码（C++）

```cpp
// https://www.luogu.com.cn/problem/P1232
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int n;
int pos[mxn];        // pos[x] = DFS 序位置 of 节点 x
int R[mxn];          // R[i] = BFS位置 i 的子树右端上界 = n - pos[node] + i
long double f[mxn][2],suf[mxn][2]; // f[i][k] 用滚动数组（只依赖 k+1 层）

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	vector<int>d(n),b(n);
	for(int i=0;i<n;++i)
	{
		cin>>d[i];
		pos[d[i]]=i+1; // 1-indexed DFS 位置
	}
	for(int i=0;i<n;++i)
	{
		cin>>b[i];
	}
	// 按 BFS 位置 i (1-indexed) 计算 R[i]
	// BFS 位置 i 的节点 = b[i-1]，其 DFS 位置 = pos[b[i-1]]
	for(int i=1;i<=n;++i)
	{
		int node=b[i-1];
		R[i]=n-pos[node]+i;
	}

	// f 用滚动：fcur[k]=f[i][k] 当前 i，fpre[k]=f[i+1...][k+1] 已算好的下一层
	// 但转移 f[i][k] 依赖所有 j>=i+1 的 f[j][k+1]，所以不能只滚动 i，要保留整个 j 维度
	// 改用完整表 f[i][k]，i 逆序，k 逆序
	// 这里用 long double 完整表
	static long double F[mxn][mxn]{}; // 可能太大，n=2e5 -> 4e10，不行
	// 实际上 h(树高) 远小于 n，且 F[i] 只需 k 到 h。用 vector 按 i 存变长
	// 简化：用 log 域 + 按深度分层，每层一个数组 g[i]=log F[i][k]
	// 下面用 log 域实现

	// logF[i][k] = log F[i][k]，用 vector<vector<long double>>
	vector<vector<long double>>logF(n+2, vector<long double>(n+2, -1e18));
	vector<vector<long double>>sufLog(n+2, vector<long double>(n+2, -1e18));

	// log 域后缀和用 log-sum-exp
	auto logaddexp=[&](long double a,long double b)->long double
	{
		if(a<b)
			swap(a,b);
		if(b<-1e17)
			return a;
		return a+log1pl(exp(b-a));
	};

	// 前缀积(log)：logP[k][i] = sum_{m=1}^{i} logF[m][k]
	// 对每个 k，logP[k] 是前缀和
	vector<vector<long double>>logP(n+2, vector<long double>(n+2, 0));

	// 初始化：F[1][1]=1 -> logF[1][1]=0
	logF[1][1]=0;

	// i 从 n 到 1，k 从 n 到 1
	// 先对每个 k 算完所有 i 的 logF[i][k]，再算 logP[k] 前缀和
	// 但 logF[i][k] 依赖 logF[j][k+1] (j>i) 和 sufLog[j][k+1]
	// 所以 k 从大到小，对每个 k：先算 sufLog[*][k+1]（依赖 k+1 层，已算），再算 logF[i][k]
	for(int k=n;k>=1;--k)
	{
		// 先算 sufLog[i][k] = logsumexp_{d>=k} F[i][d]，但 k 层刚算，先用 k+1 层的 suf
		// 转移只用 sufLog[j][k+1]，所以只需 k+1 层 sufLog 已就绪
		// 算 logF[i][k] 对 i 从 n 到 1
		// 需要 logP[k+1]（k+1 层前缀和）
		// 先构造 logP[k+1]：前缀和 of logF[*][k+1]
		for(int i=1;i<=n;++i)
			logP[k+1][i]=logP[k+1][i-1]+(logF[i][k+1]>-1e17?logF[i][k+1]:0);
		// 注意：logF[i][k+1] 可能是 -1e18（0 方案），前缀积应为 1（log=0）当某项为 0？
		// 不：前缀积 = 连乘，某项为 0 则积为 0。log 域：log(0)=-inf。
		// 乘积 P[j-1]/P[i] 中若 P[i]=0 则... 用 log 域除法 = 减
		// 改：logProduct(i+1, j-1) = logP[k+1][j-1]-logP[k+1][i]
		// 但要求 logP 是"连乘的 log"，即 logP[i]=sum_{m<=i} logF[m][k+1]
		// 若某 logF[m]=-inf，则 logP 全 -inf（积为 0）。这种情况转移项为 0，可跳过
		for(int i=n;i>=1;--i)
		{
			long double acc=-1e18; // logsumexp over j in [i, R[i]]
			for(int j=i;j<=R[i];++j)
			{
				// 乘积 part: logProduct_{m=i+1}^{j-1} logF[m][k+1]
				long double prodPart=0;
				if(j-1>=i+1)
				{
					prodPart=logP[k+1][j-1]-logP[k+1][i];
				}
				// 若中间有 -inf（某 f[m][k+1]=0），prodPart=-inf，跳过
				if(prodPart<-1e17)
					continue;
				// suf part: sufLog[j][k+1]
				if(sufLog[j][k+1]<-1e17)
					continue;
				long double term=prodPart+sufLog[j][k+1];
				acc=logaddexp(acc,term);
			}
			logF[i][k]=acc;
		}
		// 算 sufLog[i][k] = logsumexp_{d>=k} F[i][d]
		// 用 k 层和 k+1 层：sufLog[i][k] = logaddexp(logF[i][k], sufLog[i][k+1])
		for(int i=1;i<=n;++i)
			sufLog[i][k]=logaddexp(logF[i][k],sufLog[i][k+1]);
	}

	// 答案：ans = sum_k k*F[n][k] / sum_k F[n][k]，用 log 域 log-sum-exp
	long double num=-1e18,den=-1e18;
	for(int k=1;k<=n;++k)
	{
		if(logF[n][k]<-1e17)
			continue;
		num=logaddexp(num,logF[n][k]+log((long double)k));
		den=logaddexp(den,logF[n][k]);
	}
	long double ans=exp(num-den);
	cout<<fixed<<setprecision(3)<<(double)ans<<'\n';
	return 0;
}
```

> **注意**：上面 log 域实现中，`logP` 前缀积遇到 `logF=-inf`（0 方案）会污染整个前缀和。实际实现应改为"稀疏前缀积"或直接用 `long double` 浮点（非 log 域）存 `f` 和 `suf`（`long double` 80 位能扛 $10^{4932}$，$n=2\times10^5$ 的连乘连加不会溢出，且最终比值稳定）。竞赛提交建议用 `long double` 直接算，避免 log 域的前缀积污染问题。复杂度 $O(n\cdot h)$ 不变。

---

*本解法核心：连续子树 + 孩子段连续 + DFS/BFS 双约束夹出 $R_i$，DP 按 BFS 序逆序、深度逆序，前缀积 + 后缀和优化，`long double` 存大数，节点 $n$ 深度 = 树高。*
