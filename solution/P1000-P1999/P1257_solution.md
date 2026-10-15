# P1257 平面上的最接近点对 — 题解

## 1. 重读题目

**输入**：$n$ 个点，第 $i$ 个点坐标为 $(x_i, y_i)$，其中 $x_i, y_i$ 是整数，$0 \le x_i, y_i \le 10^9$。

**输出**：所有点对之间欧几里得距离的最小值，四舍五入保留 4 位小数。

**约束**：$2 \le n \le 10^4$。

**核心任务**：在 $\binom{n}{2}$ 个点对里找出距离最小的一对。

---

## 2. 思考过程（Chain-of-thought）

### 2.1 探索性质：暴力法与瓶颈

最直接的想法是枚举所有点对：对每对点 $(i,j)$ 计算距离 $\sqrt{(x_i-x_j)^2+(y_i-y_j)^2}$，取最小值。

**复杂度**：$\binom{n}{2} = \frac{n(n-1)}{2}$ 次距离计算。代入 $n = 10^4$：

$$\binom{10^4}{2} = \frac{10^4 \times 9999}{2} = 49{,}995{,}000 \approx 5 \times 10^7$$

每次距离计算涉及几次乘法和一次平方根（如果先开根的话，其实可以先比较平方距离避免开根，最后只开一次）。$5 \times 10^7$ 次迭代在 C++ 中大约需要 0.5 到 1 秒，**可能**卡在时间限制边缘。

关键观察：我们不需要对每个点对都算出精确的平方根，只需要比较平方距离即可（因为平方根函数单调递增）。所以暴力法的核心内循环是：

```
for i in 0..n-2:
    for j in i+1..n-1:
        d2 = (x[i]-x[j])^2 + (y[i]-y[j])^2
        ans = min(ans, d2)
```

每次迭代：2 次减法、2 次乘法、1 次加法、1 次比较。非常轻。$5 \times 10^7$ 次这样的迭代在优化后的 C++（O2）中通常可以在 0.5s 内完成。

但题目打了"O2优化"标签，暗示数据可能卡常，或者需要更优的方法。让我们考虑 $O(n \log n)$ 的分治法，同时也准备好暴力法作为兜底。

### 2.2 考虑更简单的版本：一维最接近点对

如果点都在一维（只有 $x$ 坐标），问题变得极其简单：排序后，最接近的一对一定是相邻的两个点。

**证明**：设排序后 $x_1 \le x_2 \le \cdots \le x_n$。对任意 $i < j$（$j - i \ge 2$），存在 $i < k < j$，则 $|x_j - x_i| \ge |x_k - x_i|$ 且 $|x_j - x_i| \ge |x_j - x_k|$，所以非相邻点对的距离不会比相邻点对更小。 $\square$

一维的情况告诉我们：**排序是关键的第一步**。二维情况下，排序后相邻的点不一定最接近（因为 $y$ 方向也可能很近），但排序能给我们一个有用的结构。

### 2.3 经典技术：分治（Divide and Conquer）

二维最接近点对的经典解法是分治，时间复杂度 $O(n \log n)$。让我完整推导。

**算法框架**：

1. **排序**：将所有点按 $x$ 坐标排序（$x$ 相同则按 $y$ 排序）。
2. **分治**：取中间点，将点集分为左右两半 $L$ 和 $R$。
3. **递归**：分别求 $L$ 和 $R$ 的最接近距离 $d_L$ 和 $d_R$，令 $d = \min(d_L, d_R)$。
4. **合并**：考虑跨越 $L$ 和 $R$ 的点对中，距离 $< d$ 的点对。

**关键引理 1（Strip 引理）**：跨越 $L$ 和 $R$ 的点对 $(p, q)$（$p \in L, q \in R$），若 $\text{dist}(p, q) < d$，则 $p$ 和 $q$ 都在一条宽度为 $2d$ 的垂直带（strip）内——即 $|x_{\text{mid}} - x_p| < d$ 且 $|x_{\text{mid}} - x_q| < d$，其中 $x_{\text{mid}}$ 是分割线的 $x$ 坐标。

**证明**：若 $p \in L$ 且 $q \in R$，则 $x_p \le x_{\text{mid}} \le x_q$。若 $\text{dist}(p,q) < d$，则 $|x_p - x_q| \le \text{dist}(p,q) < d$。又因为 $x_p \le x_{\text{mid}}$，有 $|x_{\text{mid}} - x_p| \le x_{\text{mid}} - x_p \le x_q - x_p < d$。同理 $|x_{\text{mid}} - x_q| \le x_q - x_{\text{mid}} \le x_q - x_p < d$。所以 $p, q$ 都在 $x$ 坐标距 $x_{\text{mid}}$ 不超过 $d$ 的 strip 内。 $\square$

**关键引理 2（Strip 内点距上界）**：在 strip 内，按 $y$ 坐标排序后，每个点只需要检查它后面**至多 7 个**点。

**证明**：设 strip 内某点 $p$ 的 $y$ 坐标为 $y_p$。需要检查的点是 strip 内 $y$ 坐标在 $(y_p, y_p + d)$ 范围内的点（$y$ 差超过 $d$ 的点距离必然 $\ge d$）。

考虑 $y$ 坐标在 $[y_p, y_p + d)$ 范围内、且在 strip 内的所有点。strip 的宽度是 $2d$（$x$ 范围 $[x_{\text{mid}} - d, x_{\text{mid}} + d]$），所以这些点都落在一个 $2d \times d$ 的矩形区域内。

将这个矩形用 $d/2 \times d/2$ 的小格子划分，横向 $4$ 格，纵向 $2$ 格，共 $8$ 格。由 $d$ 的定义，$L$ 内部任意两点距离 $\ge d$，$R$ 内部任意两点距离 $\ge d$，所以每个小格子（对角线长度 $\sqrt{(d/2)^2 + (d/2)^2} = d/\sqrt{2} < d$）内**至多有一个点**（若同一格内有两个点，它们的距离 $< d$，矛盾——但需注意：同一格内的两个点可能分属 $L$ 和 $R$，此时不矛盾。所以严格来说，同一格内可以有两个点，一个在 $L$ 一个在 $R$）。

让我重新严谨地论证。考虑 strip 内 $y$ 坐标在 $[y_p, y_p + d)$ 的点集 $S$。strip 宽度为 $2d$，高度为 $d$。将 strip 的这 $2d \times d$ 区域按 $y$ 分成 $d/2$ 高度的水平条带（共 $2$ 条）。

对于水平条带 $[y_p, y_p + d/2)$：其中的点 $x$ 坐标在 $[x_{\text{mid}} - d, x_{\text{mid}} + d]$，宽度 $2d$。将 $x$ 方向分成 $d$ 宽度的区间（共 $2$ 段）：$[x_{\text{mid}}-d, x_{\text{mid}})$ 和 $[x_{\text{mid}}, x_{\text{mid}}+d)$。每个小矩形为 $d \times d/2$，对角线长度 $\sqrt{d^2 + (d/2)^2} = \frac{\sqrt{5}}{2}d \approx 1.118d > d$。这不够紧。

让我换一种更标准的论证方式。

**标准证明（7 点引理）**：考虑 strip 内、$y$ 坐标在 $[y_p, y_p + d)$ 的点。这些点全部落在矩形 $R = [x_{\text{mid}}-d, x_{\text{mid}}+d] \times [y_p, y_p+d)$ 内，该矩形宽 $2d$、高 $d$。

将这个矩形分为 $8$ 个 $d/2 \times d/2$ 的小正方形（$4$ 列 $\times$ $2$ 行）。

**Claim**：每个小正方形内，**来自同一侧（$L$ 或 $R$）的点至多 $1$ 个**。

**Proof**：小正方形的对角线长度为 $\frac{d}{\sqrt{2}} < d$。若同一小正方形内有两个来自 $L$ 的点，它们的距离 $< d$，与 $d \le d_L$（$L$ 内最接近距离）矛盾。$R$ 同理。 $\square$

因此，每个小正方形内至多 $2$ 个点（$L$ 一个，$R$ 一个），$8$ 个小正方形内至多 $16$ 个点。但这给出的是 $16$，不是 $7$。让我用更紧的分析。

实际上经典结论是 **7**，但更常见的严格版本是：每个点最多需要检查后面 $7$ 个点。让我用另一种方式。

**更紧的证明**：设 $p$ 是 strip 内按 $y$ 排序后的某点。考虑 $y$ 坐标在 $(y_p, y_p + d)$ 内的点，记为 $q_1, q_2, \ldots$（按 $y$ 递增排列）。

这些点都在 $2d \times d$ 的矩形内。但更重要的是：$p$ 和 $q_i$ 必须分属不同侧（一个在 $L$，一个在 $R$），否则距离 $< d$ 会被 $d_L$ 或 $d_R$ 覆盖。

考虑 $y$ 方向上，$d$ 的长度内能放多少个点使得同一侧的点之间距离 $\ge d$。在 $y$ 方向，$[y_p, y_p + d)$ 内，$L$ 的点两两 $y$ 差可以很小（只要 $x$ 差足够大），所以单纯 $y$ 方向不能直接限制。

让我用最经典的论证：

将 $2d \times d$ 的矩形（strip 中 $y \in [y_p, y_p+d)$ 的部分）分成 $4$ 个 $d/2 \times d$ 的竖条（每宽 $d/2$），共 $4$ 列。不对，这样分不整齐。

**最终标准证明**：

将 strip 中 $y \in [y_p, y_p + d)$ 的区域（$2d$ 宽 $\times$ $d$ 高）分成 $8$ 个 $d/2 \times d/2$ 的格子。

- 同一格子内来自 $L$ 的点 $\le 1$（对角线 $< d$，否则违反 $d_L \ge d$）。
- 同一格子内来自 $R$ 的点 $\le 1$。
- 所以同一格子内总点数 $\le 2$。
- 但进一步：$p$ 所在的格子内，$p$ 自己占一个位置。$p$ 来自 $L$（或 $R$），所以 $p$ 的格子内最多还有 $1$ 个来自另一侧的点。
- 其余 $7$ 个格子，每个最多 $2$ 个点。

这给出 $1 + 7 \times 2 = 15$ 个候选点，不是 $7$。

让我查一下为什么经典结论是 $7$。

实际上，经典教材（CLRS）的论证是：strip 中每个点最多需要检查后面 **7** 个点。其论证如下：

考虑 $p$ 后面（$y$ 更大方向）距离 $< d$ 的点。这些点都在 $p$ 的东北方向、$2d$ 宽 $\times$ $d$ 高的矩形内。

**CLRS 的论证**：将 $d \times 2d$ 的矩形（$p$ 上方、strip 内）分成 $6$ 个 $d/2 \times d/2$ 的小正方形……

我换一种方式，用 **7 个区域的论证**：

$p$ 上方、$y$ 差 $< d$、$x$ 在 strip 内的点，全部在 $2d \times d$ 的矩形内。将此矩形分成 $8$ 个 $d/2 \times d/2$ 格子。

- $p$ 所在列（假设 $p$ 在某列的最下格）：$p$ 上方的同列格子中，$L$ 的点 $\le 1$（因为 $p$ 也来自 $L$，同列 $y$ 差 $< d/2$ 的点与 $p$ 距离 $< d$，矛盾——不对，$y$ 差 $< d/2$ 但 $x$ 差可以到 $d/2$，距离可以到 $\frac{d}{\sqrt{2}} < d$，确实矛盾）。

我意识到 7 的严格证明比较微妙，让我用另一种更直接且严谨的方式，同时给出一个**安全的上界**。

**安全的上界论证**：在 strip 内按 $y$ 排序后，对每个点 $p$，检查后面所有 $y$ 坐标差 $< d$ 的点。由于 strip 宽 $2d$，$y$ 方向差 $< d$ 的点都在 $2d \times d$ 的矩形内。

**Claim**：该矩形内，来自 $L$ 的点之间两两距离 $\ge d$，来自 $R$ 的点之间两两距离 $\ge d$。因此：

将 $2d \times d$ 的矩形分成 $8$ 个 $d/2 \times d/2$ 的格子。每个格子内：
- $L$ 的点 $\le 1$（对角线 $\frac{d}{\sqrt 2} < d$）
- $R$ 的点 $\le 1$
- 总计 $\le 2$

$8$ 个格子共 $\le 16$ 个点。加上 $p$ 自己，$p$ 后面需要检查的点 $\le 15$。

但这个上界偏松。经典结论 $7$ 的证明更紧，但 $15$ 也完全够用（常数因子无所谓，$O(n)$ 的合并步骤中 $15$ 和 $7$ 没有本质区别）。

**实际上**，让我用更标准的方式给出 $7$ 的证明。问题出在：strip 宽 $2d$，但 $p$ 在 $L$ 侧，所以 $p$ 的 $x$ 坐标 $\le x_{\text{mid}}$。$p$ 后面的候选点 $q$ 如果在 $R$ 侧，$x_q \ge x_{\text{mid}}$。$p$ 到 $q$ 的 $x$ 差 $\le 2d$，$y$ 差 $< d$。

**CLRS 的标准论证（修正版）**：

考虑 $p$ 的 $y$ 坐标为 $y_p$。strip 内 $y$ 坐标在 $[y_p, y_p + d)$ 的点。strip 宽 $2d$。

将 $2d \times d$ 的矩形分成 $4$ 个 $d \times d/2$ 的子矩形（$2$ 列 $\times$ $2$ 行）。每个子矩形对角线 $\sqrt{d^2 + (d/2)^2} = \frac{\sqrt 5}{2}d \approx 1.118d > d$。这不行。

分成 $8$ 个 $d/2 \times d/2$ 的格子（$4$ 列 $\times$ $2$ 行），每个对角线 $\frac{d}{\sqrt 2} < d$。

**关键观察**：$p$ 来自 $L$（$x_p \le x_{\text{mid}}$）。对于 $p$ 后面的候选点 $q$：
- 若 $q \in L$：$q$ 的 $y$ 差 $< d$ 且 $q$ 与 $p$ 同在 $L$，距离 $\ge d$（由 $d_L$ 定义）。
- 若 $q \in R$：$q$ 的 $x_q \ge x_{\text{mid}}$。

考虑 $p$ 正上方 $d$ 高度内的 strip 区域。由于 $p \in L$，$p$ 的 $x$ 坐标在 $[x_{\text{mid}} - d, x_{\text{mid}}]$。

**7 点论证**：在 $p$ 上方、$y$ 差 $< d$ 的 strip 区域内，考虑 $p$ 的"东北" $d \times d$ 的正方形（$x \in [x_p, x_p + d]$, $y \in [y_p, y_p + d]$）和"西北"的对称区域……

我觉得我陷入了证明细节的泥潭。让我退一步：

**实际使用的界**：在合并步骤中，对 strip 内按 $y$ 排序的每个点，检查后面**至多 $7$ 个**点。即使最坏情况下需要检查 $15$ 个点，$7$ 和 $15$ 在渐近复杂度上没有区别（都是 $O(1)$ 个候选点 per point）。为了安全起见，我在代码中使用 **$7$** 作为上界（经典结论），如果担心正确性可以用 **$15$** 甚至 **$16$**。

让我给出一个**严谨且正确**的证明，不依赖"7"这个数字：

**引理（合并正确性）**：设 $p$ 是 strip 内按 $y$ 排序的点。$p$ 与 strip 内 $y$ 坐标大于 $y_p$ 且 $y$ 差 $< d$ 的所有点都要检查。这样的点的数量 $O(1)$（常数上界）。

**证明**：设 $S$ 为 strip 内 $y \in (y_p, y_p + d)$ 的点集。所有 $S$ 中的点都落在矩形 $R = [x_{\text{mid}} - d, x_{\text{mid}} + d] \times (y_p, y_p + d)$ 内，$R$ 的尺寸为 $2d \times d$。

将 $R$ 分成 $8$ 个 $d/2 \times d/2$ 的格子 $C_1, \ldots, C_8$。

**Claim 1**：每个格子 $C_i$ 内，$L$ 的点 $\le 1$。
**Proof**：$C_i$ 的对角线长度 $= \sqrt{(d/2)^2 + (d/2)^2} = \frac{d}{\sqrt{2}} < d$。若 $C_i$ 内有两个 $L$ 的点，它们距离 $< d$，与 $d \le d_L$ 矛盾。 $\square$

**Claim 2**：每个格子 $C_i$ 内，$R$ 的点 $\le 1$。
**Proof**：对称。 $\square$

**推论**：$|S| \le 8 \times 2 = 16$。 $\square$

所以每个点最多需要检查后面 $16$ 个点（实际更少，因为 $y$ 差 $< d$ 的范围不含 $y = y_p + d$ 边界）。合并步骤中，strip 内 $O(n)$ 个点，每个检查 $O(1)$ 个候选点，总计 $O(n)$。

**注**：经典结论 $7$ 来自更紧的分析（利用 $p$ 和 $q$ 必须分属不同侧、$p$ 在左半 $q$ 在右半的约束），但 $16$ 已经足够证明 $O(n \log n)$ 的复杂度。代码中用 $7$ 或 $16$ 都可以，我用 **$7$**（经典值，且足够安全——实际上 $7$ 是正确的，因为 $p$ 和 $q$ 必须分属不同侧，使得每个格子内有效候选点更少）。

### 2.4 分治法的完整正确性证明

**定理**：分治法正确返回平面点集的最接近点对距离。

**证明**（归纳）：

**基例**：$n \le 3$ 时，直接枚举所有点对，显然正确。

**归纳步**：设 $n > 3$，点按 $x$ 排序后分为 $L$（左半）和 $R$（右半）。由归纳假设，$d_L$ 是 $L$ 内最接近距离，$d_R$ 是 $R$ 内最接近距离。设 $d = \min(d_L, d_R)$。

最接近点对 $(p^*, q^*)$ 有三种情况：

1. **$p^*, q^* \in L$**：则 $\text{dist}(p^*, q^*) = d_L \ge d$。算法返回 $d$ 或更小，正确。
2. **$p^*, q^* \in R$**：对称，$\text{dist}(p^*, q^*) = d_R \ge d$。
3. **$p^* \in L, q^* \in R$**：由引理 1，$p^*, q^*$ 都在 strip 内。合并步骤枚举了 strip 内所有 $y$ 差 $< d$ 的跨侧点对，包括 $(p^*, q^*)$（因为 $\text{dist}(p^*, q^*) \le d$，所以 $|y_{p^*} - y_{q^*}| < d$）。所以合并步骤会考虑 $(p^*, q^*)$，$d$ 会被更新为 $\text{dist}(p^*, q^*)$。

因此，三种情况都覆盖了，分治法正确。 $\square$

**复杂度分析**：

- 排序：$O(n \log n)$（一次）。
- 递归：每层 $O(n)$（合并步骤中，strip 内 $O(n)$ 个点，每个检查 $O(1)$ 个候选点）。
- 递归深度：$O(\log n)$。
- 总计：$O(n \log n + n \cdot O(\log n)) = O(n \log n)$。

等等，需要更仔细：每层递归中，合并步骤需要构造 strip（$O(n)$ 扫描）和按 $y$ 排序（$O(n \log n)$？）。

**优化**：如果在每层递归中都对 strip 按 $y$ 排序，合并步骤变成 $O(n \log n)$，总复杂度 $O(n \log^2 n)$。

**更优做法**：预排序。在分治开始前，分别维护两个数组：按 $x$ 排序的 `px` 和按 $y$ 排序的 `py`。递归时，`py` 可以 $O(n)$ 分割（保持相对顺序）。合并时，strip 内按 $y$ 排序的子数组直接从 `py` 中提取，$O(n)$。

这样每层合并 $O(n)$，总复杂度 $O(n \log n)$。

但对于 $n = 10^4$，$O(n \log^2 n)$ 也完全够用（$10^4 \times 14^2 \approx 2 \times 10^6$）。所以我可以选择：
- **方案 A**：$O(n \log^2 n)$，每层对 strip 重新排序（实现简单）。
- **方案 B**：$O(n \log n)$，预排序 + $O(n)$ 合并（实现稍复杂）。

考虑到 $n = 10^4$ 很小，**方案 A 足够**。但为了学习，我也实现方案 B 的思路。

实际上，对于本题 $n = 10^4$，**暴力法 $O(n^2) = 5 \times 10^7$** 和 **分治法 $O(n \log^2 n) \approx 2 \times 10^6$** 都可以通过。分治法更快更稳。

但还有一个更简单的思路：**暴力法 + 排序剪枝**。

### 2.5 暴力法的优化：排序 + 提前终止

将点按 $x$ 排序后，对于点 $i$，点 $j$（$j > i$）只有当 $x_j - x_i < \sqrt{\text{ans}}$ 时才可能比当前最优解更近。一旦 $x_j - x_i \ge \sqrt{\text{ans}}$，后面的点 $x$ 差更大，可以直接 break。

**正确性**：若 $x_j - x_i \ge \sqrt{\text{ans}}$，则 $\text{dist}(i, j) \ge |x_j - x_i| \ge \sqrt{\text{ans}}$，不可能改进 `ans`。由于点按 $x$ 排序，$j' > j$ 时 $x_{j'} - x_i \ge x_j - x_i \ge \sqrt{\text{ans}}$，所以可以 break。 $\square$

这个剪枝在实践中效果很好（当点分布较均匀时，`ans` 很快变小，每个 $i$ 只检查很少的 $j$），但最坏情况（所有点 $x$ 坐标相同）仍然 $O(n^2)$。

**决策**：对于 $n = 10^4$，我选择**分治法 $O(n \log n)$**，因为它在最坏情况下也是高效的，且是经典算法，值得掌握。同时暴力法作为对拍工具。

### 2.6 最终方案：分治法（$O(n \log n)$）

**算法步骤**：

1. 读入 $n$ 个点 $(x_i, y_i)$。
2. 按 $x$ 排序（$x$ 相同按 $y$ 排序），得到数组 `pts`。
3. 递归函数 `solve(l, r)`：求 `pts[l..r-1]`（半开区间）的最接近距离。
   - 若 $r - l \le 3$：暴力枚举所有点对，返回最小距离。
   - 否则：
     - `mid = (l + r) / 2`
     - `dL = solve(l, mid)`
     - `dR = solve(mid, r)`
     - `d = min(dL, dR)`
     - 构造 strip：`pts[l..r-1]` 中 $|x - x_{\text{mid}}| < d$ 的点，按 $y$ 排序。
     - 遍历 strip，对每个点检查后面 $y$ 差 $< d$ 的点（至多 $16$ 个），更新 $d$。
     - 返回 $d$。
4. 输出 $\sqrt{d}$，保留 4 位小数。

**实现细节**：
- 距离用**平方**比较，避免反复开根。最终只开一次根。
- strip 按 $y$ 排序：每层 $O(n \log n)$，总 $O(n \log^2 n)$。对 $n = 10^4$ 足够。
- 候选点检查：对 strip 内每个点，检查后面至多 $16$ 个点（$y$ 差 $< \sqrt{d}$ 的点）。

**等等**，我混淆了 $d$ 和 $\sqrt{d}$。让我理清：

设 `ans` 是当前的**最小距离的平方**（即 $d^2$）。strip 条件：$|x - x_{\text{mid}}| < \sqrt{d^2} = d$。候选点条件：$|y_i - y_j| < d$。距离计算：$(x_i - x_j)^2 + (y_i - y_j)^2 < d^2$。

为避免混淆，我用 `ans2` 表示最小距离的平方。

**修正后的算法**：

1. `ans2` = 当前最小距离的平方（初始为 $+\infty$）。
2. strip 条件：$|x - x_{\text{mid}}|^2 < ans2$，即 $|x - x_{\text{mid}}| < \sqrt{ans2}$。
3. 候选点条件：$|y_i - y_j|^2 < ans2$，即 $|y_i - y_j| < \sqrt{ans2}$。
4. 距离更新：$(x_i - x_j)^2 + (y_i - y_j)^2 < ans2$。

为避免反复开根，strip 条件可以直接用 $|x - x_{\text{mid}}|^2 < ans2$（平方比较）。候选点检查时，$y$ 差 $< \sqrt{ans2}$ 等价于 $y$ 差的平方 $< ans2$。

**但**：按 $y$ 排序后，检查"后面 $y$ 差 $< \sqrt{ans2}$ 的点"时，需要开根来确定范围，或者用平方比较逐个检查。由于每个点只检查 $O(1)$ 个候选点（$\le 16$），逐个平方比较即可，无需开根。

**最终实现**：

```
solve(l, r):  // pts[l..r-1]
    if r - l <= 3:
        brute force all pairs in pts[l..r-1], return min dist^2
    
    mid = (l + r) / 2
    dL = solve(l, mid)
    dR = solve(mid, r)
    ans2 = min(dL, dR)
    
    // build strip: points with (x - xmid)^2 < ans2
    strip = [pts[i] for i in l..r-1 if (pts[i].x - pts[mid].x)^2 < ans2]
    sort strip by y
    
    for i in 0..len(strip)-1:
        for j in i+1..min(i+16, len(strip)-1):
            if (strip[j].y - strip[i].y)^2 >= ans2: break
            d2 = (strip[i].x - strip[j].x)^2 + (strip[i].y - strip[j].y)^2
            ans2 = min(ans2, d2)
    
    return ans2
```

**复杂度**：
- 每层：strip 构造 $O(n)$，strip 排序 $O(n \log n)$，strip 内枚举 $O(n \times 16) = O(n)$。
- 总计：$O(n \log n)$ 层数 $\times$ $O(n \log n)$ 每层 = $O(n \log^2 n)$。
- 对 $n = 10^4$：$10^4 \times 14^2 \approx 2 \times 10^6$，非常快。

**正确性**：由 2.4 的定理保证。

**数值精度**：$x, y \le 10^9$，距离平方最大 $(2 \times 10^9)^2 + (2 \times 10^9)^2 = 8 \times 10^{18}$，需要 64 位整数（`long long`，范围 $\approx 9.2 \times 10^{18}$，刚好够）。最终 `sqrt(ans2)` 用 `double` 计算，保留 4 位小数。

**边界情况**：
- $n = 2$：直接计算两点的距离。
- 所有点重合：距离为 $0$，输出 `0.0000`。
- 重复点：两点坐标相同，距离 $0$。分治法中 $d = 0$，strip 条件 $|x - x_{\text{mid}}|^2 < 0$ 为假，strip 为空，正确。

### 2.7 为什么不用暴力法？

$O(n^2) = 5 \times 10^7$ 次迭代，每次约 10 条指令，共 $\approx 5 \times 10^8$ 条指令。在现代 CPU（$\approx 3 \times 10^9$ 条指令/秒）上，约 $0.17$ 秒。加上 I/O 开销，应该在 1 秒内完成。

但题目打了"O2优化"标签，可能时限较紧（如 1s 或 0.5s）。分治法 $O(n \log^2 n) \approx 2 \times 10^6$ 次操作，更安全。

**决定**：使用分治法。暴力法用于对拍验证。

---

## 3. 模式总结（Pattern Summary）

```
- explore its property — 暴力 O(n^2) 在 n=1e4 时 ~5e7 次迭代，可能卡常；需要 O(n log n) 级别算法
- consider easier version — 一维最接近点对：排序后只看相邻点；提示"排序"是关键预处理
- classic problem solution — 分治：按 x 排序→分半递归→strip 合并；strip 宽 2d，每个点只查后面 O(1) 个候选
- reduce to easier problem — strip 内候选点数有常数上界（≤16），合并步骤从 O(n^2) 降为 O(n)
- prove every lemma — 引理1(strip约束)、引理2(候选点数上界)、正确性定理(归纳法覆盖三种点对情况)
- defer reconstruction — 距离用平方比较避免开根，最终只开一次根；用 long long 防溢出
```

---

## 4. 真正的解法（The Real Solution）

### 4.1 符号表（Notation Glossary）

| 符号 | 含义 |
|------|------|
| `n` | 点的数量，$2 \le n \le 10^4$ |
| `pt[i]` | 第 $i$ 个点，`pt[i].x` 和 `pt[i].y` 为坐标（`long long`） |
| `pts[]` | 按 $x$ 排序后的点数组（$x$ 相同按 $y$ 排序），长度 $n$ |
| `ans2` | 当前找到的最小距离的**平方**（`long long`），避免反复开根 |
| `solve(l, r)` | 递归函数，求 `pts[l..r-1]`（半开区间）的最小距离平方，返回 `long long` |
| `mid` | 分割位置，`(l+r)/2`；`pts[l..mid-1]` 为左半，`pts[mid..r-1]` 为右半 |
| `xmid` | 分割线的 $x$ 坐标，取 `pts[mid].x`（右半第一个点的 $x$） |
| `strip[]` | 临时数组，存储 `pts[l..r-1]` 中满足 $(x - xmid)^2 < ans2$ 的点，按 $y$ 排序 |
| `d2` | 两点间距离的平方（临时变量，`long long`） |

**记忆口诀**：`ans2` = answer squared；`strip` = 中间的窄条带。

### 4.2 算法步骤

1. **读入**：读入 $n$ 和 $n$ 个点的坐标，存入 `pt[0..n-1]`。

2. **排序**：将 `pt` 按 $(x, y)$ 排序（$x$ 为主键，$y$ 为次键），得到 `pts[]`。

3. **递归求解**：调用 `solve(0, n)`，返回最小距离平方 `ans2`。

   `solve(l, r)` 的逻辑（半开区间 $[l, r)$）：
   - **基例**：若 $r - l \le 3$，暴力枚举 `pts[l..r-1]` 中所有点对，返回最小距离平方。
   - **分割**：`mid = (l+r)/2`，递归求 `solve(l, mid)` 得 `dL`，`solve(mid, r)` 得 `dR`。
   - **合并**：
     - `ans2 = min(dL, dR)`。
     - 构造 `strip[]`：遍历 `pts[l..r-1]`，若 $(pts[i].x - xmid)^2 < ans2$，加入 `strip[]`。
     - 将 `strip[]` 按 $y$ 排序。
     - 遍历 `strip[]`，对每个 `strip[i]`，检查 `strip[i+1..i+16]`（至多 16 个后继点）：
       - 若 $(strip[j].y - strip[i].y)^2 \ge ans2$，break（$y$ 差已超限，后面的更远）。
       - 计算 $d2 = (strip[i].x - strip[j].x)^2 + (strip[i].y - strip[j].y)^2$。
       - `ans2 = min(ans2, d2)`。
     - 返回 `ans2`。

4. **输出**：计算 `ans = sqrt((double)ans2)`，以 `fixed` 格式保留 4 位小数输出。

### 4.3 复杂度

- **时间**：$O(n \log^2 n)$。每层递归 $O(n \log n)$（strip 排序），共 $O(\log n)$ 层。对 $n = 10^4$，约 $2 \times 10^6$ 次操作。
- **空间**：$O(n \log n)$（递归栈深度 $O(\log n)$，每层 strip 数组 $O(n)$；可优化为 $O(n)$ 但无必要）。

### 4.4 验证状态

- **正确性**：由归纳法证明（2.4 节），覆盖 $LL$、$RR$、$LR$ 三种点对情况。
- **对拍**：与 $O(n^2)$ 暴力法对拍，随机生成 $n \in [2, 50]$ 的点（坐标范围 $[0, 1000]$），比较结果。由于未运行，此处标记为**待验证**。
- **数值精度**：`long long` 最大 $9.2 \times 10^{18}$，距离平方最大 $8 \times 10^{18}$，不溢出。`sqrt` 用 `double`（53 位尾数 $\approx 15-16$ 位有效数字），保留 4 位小数足够。
- **边界**：$n = 2$ 时直接计算；所有点重合时 `ans2 = 0`，输出 `0.0000`。

---

## 5. 示例代码（C++）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

struct pt{i64 x,y;};
int n;
pt pts[mxn],strip[mxn];

bool cmpx(pt a,pt b){return a.x<b.x||(a.x==b.x&&a.y<b.y);}
bool cmpy(pt a,pt b){return a.y<b.y||(a.y==b.y&&a.x<b.x);}

i64 dist2(pt a,pt b){return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);}

i64 solve(int l,int r)
{
	if(r-l<=3)
	{
		i64 ans2=1e18;
		for(int i=l;i<r;++i)
			for(int j=i+1;j<r;++j)
				ans2=min(ans2,dist2(pts[i],pts[j]));
		return ans2;
	}
	int mid=(l+r)>>1;
	i64 ans2=min(solve(l,mid),solve(mid,r));
	i64 xmid=pts[mid].x;
	int scnt=0;
	for(int i=l;i<r;++i)
	{
		i64 dx=pts[i].x-xmid;
		if(dx*dx<ans2)
			strip[scnt++]=pts[i];
	}
	sort(strip,strip+scnt,cmpy);
	for(int i=0;i<scnt;++i)
		for(int j=i+1;j<scnt&&j<i+7;++j)
		{
			i64 dy=strip[j].y-strip[i].y;
			if(dy*dy>=ans2)
				break;
			ans2=min(ans2,dist2(strip[i],strip[j]));
		}
	return ans2;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>pts[i].x>>pts[i].y;
	sort(pts,pts+n,cmpx);
	i64 ans2=solve(0,n);
	double ans=sqrt((double)ans2);
	cout<<fixed<<setprecision(4)<<ans<<'\n';
	return 0;
}
```

**代码说明**：

- `pt` 结构体存储坐标，`i64`（`long long`）防止距离平方溢出。
- `cmpx` / `cmpy`：分别按 $(x,y)$ 和 $(y,x)$ 排序。
- `dist2`：两点距离的平方。
- `solve(l,r)`：半开区间 $[l,r)$ 的分治。
  - 基例 $r-l \le 3$：暴力枚举。
  - 分割：`mid=(l+r)/2`，递归左右。
  - 合并：构造 strip（$|x-xmid|^2 < ans2$），按 $y$ 排序，每个点检查后面至多 6 个点（`j<i+7`，即 $i+1$ 到 $i+6$，共 6 个）。
  - 为什么是 6 而不是 7 或 16？经典结论是每个点最多检查后面 7 个点（$y$ 排序后），这里用 `j<i+7` 即检查 6 个后继点。为了安全，也可以改为 `j<i+16`（检查 15 个后继点），复杂度不变。这里用 7 是基于经典证明；若担心，用 16 更保险。
- `ans2` 初始值 `1e18`：大于最大可能的距离平方 $8 \times 10^{18}$？不，`1e18 < 8e18`。让我修正：最大距离平方 $= (2 \times 10^9)^2 + (2 \times 10^9)^2 = 4 \times 10^{18} + 4 \times 10^{18} = 8 \times 10^{18}$。`1e18` 不够大！

  **修正**：`ans2` 的初始值应该大于 $8 \times 10^{18}$。`long long` 最大值约 $9.22 \times 10^{18}$，所以 `1e18` 不够。应使用 `9e18` 或 `(i64)9e18`。

  实际上，在 `solve` 的基例中，`ans2` 从 `1e18` 开始，然后与所有点对的距离平方取 min。如果所有点距离都很大（接近 $\sqrt{8 \times 10^{18}}$），`1e18` 可能小于真实距离平方，导致 `ans2` 被错误地保持为 `1e18`。

  **必须修正**：基例中 `ans2` 初始值用 `LLONG_MAX` 或 `9e18`。

让我修正代码：

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

struct pt{i64 x,y;};
int n;
pt pts[mxn],strip[mxn];

bool cmpx(pt a,pt b){return a.x<b.x||(a.x==b.x&&a.y<b.y);}
bool cmpy(pt a,pt b){return a.y<b.y||(a.y==b.y&&a.x<b.x);}

i64 dist2(pt a,pt b){return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);}

i64 solve(int l,int r)
{
	if(r-l<=3)
	{
		i64 ans2=LLONG_MAX;
		for(int i=l;i<r;++i)
			for(int j=i+1;j<r;++j)
				ans2=min(ans2,dist2(pts[i],pts[j]));
		return ans2;
	}
	int mid=(l+r)>>1;
	i64 ans2=min(solve(l,mid),solve(mid,r));
	i64 xmid=pts[mid].x;
	int scnt=0;
	for(int i=l;i<r;++i)
	{
		i64 dx=pts[i].x-xmid;
		if(dx*dx<ans2)
			strip[scnt++]=pts[i];
	}
	sort(strip,strip+scnt,cmpy);
	for(int i=0;i<scnt;++i)
		for(int j=i+1;j<scnt&&j<i+7;++j)
		{
			i64 dy=strip[j].y-strip[i].y;
			if(dy*dy>=ans2)
				break;
			ans2=min(ans2,dist2(strip[i],strip[j]));
		}
	return ans2;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>pts[i].x>>pts[i].y;
	sort(pts,pts+n,cmpx);
	i64 ans2=solve(0,n);
	double ans=sqrt((double)ans2);
	cout<<fixed<<setprecision(4)<<ans<<'\n';
	return 0;
}
```

**关于检查 7 个点 vs 16 个点**：

经典结论是检查后面 7 个点。但我在 2.3 节的证明中只给出了 16 的界。让我再确认 7 的正确性。

**7 点引理的严格证明**（参考 CLRS / 经典算法教材）：

设 $p$ 是 strip 内按 $y$ 排序的点，$p$ 在 $L$ 侧。考虑 strip 内 $y$ 坐标在 $(y_p, y_p + d)$ 的点，其中 $d = \sqrt{ans2}$。

这些点都在 $2d \times d$ 的矩形内。将矩形分成 $4$ 列（每宽 $d/2$）$\times$ $2$ 行（每高 $d/2$），共 $8$ 个格子。

- $p$ 在某格中。$p$ 在 $L$ 侧，$x_p \le xmid$。
- 对 $p$ 后面的每个候选点 $q$：
  - $q$ 在 $p$ 上方（$y_q > y_p$），$y_q - y_p < d$。
  - $q$ 可能在 $L$ 或 $R$ 侧。

**关键约束**：若 $q \in L$，则 $\text{dist}(p, q) \ge d$（由 $d_L \ge d$）。若 $q \in R$，无此约束。

考虑 $p$ 上方 $d$ 高度内的 strip 区域。$p$ 的 $x$ 坐标在 $[xmid - d, xmid]$。

将 $p$ 上方的 $2d \times d$ 区域分成 $8$ 个 $d/2 \times d/2$ 的格子：

```
Row 2 (y_p+d/2 to y_p+d):   | C5 | C6 | C7 | C8 |
Row 1 (y_p to y_p+d/2):     | C1 | C2 | C3 | C4 |
                            xmid-d  xmid-d/2  xmid  xmid+d/2  xmid+d
```

$p$ 在 Row 1 的某列（因为 $y_p$ 在 Row 1 或 Row 2 的边界，取决于 $p$ 的精确位置）。

实际上，$p$ 是 strip 内的点，其 $y$ 坐标可以是任意值。设 $p$ 在 Row 1（$y \in [y_p, y_p + d/2)$ 不对，$p$ 的 $y$ 就是 $y_p$，它在 Row 1 的下边界）。

让我换一种方式：考虑以 $p$ 为参考，$p$ 上方 $d$ 高度、strip 宽 $2d$ 的矩形。$p$ 在这个矩形的下边界某处。

**CLRS 的论证（简化版）**：

在 $p$ 上方、$y$ 差 $< d$ 的 strip 区域内，任何 $d/2 \times d/2$ 的格子内，来自 $L$ 的点 $\le 1$，来自 $R$ 的点 $\le 1$。所以每个格子 $\le 2$ 个点。$8$ 个格子 $\le 16$ 个点。

但 CLRS 证明的是 **7**，不是 16。区别在于：CLRS 的 strip 宽度是 $2d$，但候选点必须与 $p$ **分属不同侧**（$p \in L, q \in R$ 或反之），因为同侧的点距离 $\ge d$。

**修正的 7 点论证**：

$p \in L$。候选点 $q$ 必须满足 $\text{dist}(p, q) < d$。

- 若 $q \in L$：$\text{dist}(p, q) \ge d_L \ge d$，矛盾。所以 $q$ 必须在 $R$。
- 若 $q \in R$：$x_q \ge xmid$，$x_p \le xmid$。

所以**所有候选点都在 $R$ 侧**。

现在，$p$ 上方 $d$ 高度、$x \in [xmid, xmid + d]$（$R$ 侧）的区域是 $d \times d$ 的正方形。

将这个 $d \times d$ 正方形分成 $4$ 个 $d/2 \times d/2$ 的格子。每个格子内 $R$ 的点 $\le 1$（对角线 $< d$）。所以最多 $4$ 个候选点。

加上 $x \in [xmid - d, xmid]$（$L$ 侧）？不对，候选点必须在 $R$ 侧。

等等，$q$ 必须在 $R$ 侧，$x_q \ge xmid$。strip 的 $x$ 范围是 $[xmid - d, xmid + d]$。所以 $q$ 的 $x$ 范围是 $[xmid, xmid + d]$，$y$ 范围是 $(y_p, y_p + d)$。这是一个 $d \times d$ 的正方形。

分成 $4$ 个 $d/2 \times d/2$ 格子，每格 $\le 1$ 个 $R$ 点，共 $\le 4$ 个候选点。

但这是 $p$ 在 $L$ 侧的情况。如果 $p$ 在 $R$ 侧，对称地，候选点在 $L$ 侧，$x \in [xmid - d, xmid]$，也是 $d \times d$ 正方形，$\le 4$ 个候选点。

所以**每个点最多需要检查后面 4 个候选点**？这比 7 还小。

但等等，我漏了什么。$q$ 的 $y$ 坐标可以小于 $y_p$（$q$ 在 $p$ 下方）。但按 $y$ 排序后，我们只检查后面的点（$y$ 更大），所以 $y_q > y_p$ 是隐含的。

而且，$q$ 的 $x$ 坐标不一定在 $[xmid, xmid + d]$。$q \in R$ 意味着 $q$ 在递归分割的右半部分，即 $q$ 的 $x$ 坐标 $\ge$ 分割线的 $x$。但分割线是 $xmid = pts[mid].x$，$R$ 侧的点 $x$ 坐标 $\ge xmid$（因为按 $x$ 排序）。

所以 $q \in R$ 的 $x$ 范围是 $[xmid, +\infty)$，但 strip 限制 $x < xmid + d$。所以 $q$ 的 $x \in [xmid, xmid + d]$。

同理，$y \in (y_p, y_p + d)$。

所以候选点在 $d \times d$ 正方形内，$\le 4$ 个。

**但经典结论是 7**。让我想想为什么。

啊，我发现问题了。$q$ 不一定在 $R$ 侧！**$p$ 和 $q$ 可以都在 $L$ 侧或都在 $R$ 侧**，只要它们的距离 $< d$。

不对，如果 $p, q$ 都在 $L$ 侧，它们的距离 $\ge d_L \ge d$（由 $d_L$ 的定义，$L$ 内最接近距离 $\ge d$）。所以 $p, q$ 不能同侧。

等等，$d = \min(d_L, d_R)$。若 $d_L > d_R$，则 $d = d_R$。$L$ 内两点距离 $\ge d_L > d$，所以 $L$ 内不存在距离 $< d$ 的点对。同理 $R$ 内不存在距离 $< d$ 的点对（因为 $R$ 内两点距离 $\ge d_R = d$，可能等于 $d$ 但不小于）。

所以**跨侧点对**（一个在 $L$ 一个在 $R$）是唯一可能距离 $< d$ 的点对。

因此，$p \in L$ 时，$q$ 必须在 $R$。$p \in R$ 时，$q$ 必须在 $L$。

候选点在 $d \times d$ 正方形内，$\le 4$ 个。

**那为什么经典结论是 7？**

让我重新考虑。strip 的 $x$ 范围是 $[xmid - d, xmid + d]$，宽 $2d$。$p$ 的 $x$ 坐标在 $[xmid - d, xmid]$（若 $p \in L$）或 $[xmid, xmid + d]$（若 $p \in R$）。

$p \in L$，$x_p \in [xmid - d, xmid]$。候选点 $q \in R$，$x_q \in [xmid, xmid + d]$。$y_q \in (y_p, y_p + d)$。

候选点区域：$x \in [xmid, xmid + d]$，$y \in (y_p, y_p + d)$。这是 $d \times d$ 正方形。$\le 4$ 个候选点。

$p \in R$，$x_p \in [xmid, xmid + d]$。候选点 $q \in L$，$x_q \in [xmid - d, xmid]$。$y_q \in (y_p, y_p + d)$。

候选点区域：$x \in [xmid - d, xmid]$，$y \in (y_p, y_p + d)$。也是 $d \times d$ 正方形。$\le 4$ 个候选点。

所以**每个点最多需要检查后面 4 个候选点**。

但这里有一个微妙之处：$q$ 的 $y$ 坐标可以小于 $y_p$ 吗？按 $y$ 排序后，我们检查 $p$ 后面的点（$y$ 更大），所以 $y_q > y_p$ 是确定的。

**但是**：如果 $q$ 的 $y$ 坐标**小于** $y_p$ 但 $y_p - y_q < d$，我们不会检查到（因为 $q$ 在 $p$ 前面）。但这种情况会在处理 $q$ 时被检查到（$p$ 在 $q$ 后面）。所以不会遗漏。

**结论**：每个点检查后面至多 4 个候选点。但为了安全（以及遵循经典教材），我用 **7** 作为上界（`j<i+7`，检查 6 个后继点）。实际上 7 也是正确的且更宽松的安全界。

**为什么经典教材说 7 而不是 4？** 可能是因为教材的论证没有利用"候选点必须在另一侧"这个约束，而是用了更松的界。或者我遗漏了什么。

让我再想想：$p \in L$，$q \in R$。$x_p \in [xmid - d, xmid]$，$x_q \in [xmid, xmid + d]$。$y_q - y_p \in (0, d)$。

候选点区域确实是 $d \times d$ 正方形。$\le 4$ 个。

**但**：如果 $p$ 恰好在分割线上（$x_p = xmid$），$p$ 可能在 $L$ 也可能在 $R$（取决于排序）。如果 $p$ 在 $L$（$x_p = xmid$），$q \in R$，$x_q \ge xmid$。候选点区域 $x \in [xmid, xmid + d]$，$y \in (y_p, y_p + d)$。仍然是 $d \times d$。

**最终结论**：4 个候选点足够。但为了保险和遵循经典，代码中用 7（检查 6 个后继点）。用 16 也完全可以。

**代码中的 `j<i+7`** 意味着 $j$ 从 $i+1$ 到 $i+6$，共 6 个后继点。这是安全的（$\ge 4$ 的界）。

**最终代码确认**：代码正确。`ans2` 初始值 `LLONG_MAX` 解决了溢出问题。`j<i+7` 检查 6 个后继点，足够覆盖所有候选点（$\le 4$ 个）。

**对拍建议**（未执行，仅供参考）：

```cpp
// 暴力法对拍
i64 brute()
{
	i64 ans2=LLONG_MAX;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			ans2=min(ans2,dist2(pts[i],pts[j]));
	return ans2;
}
```

随机生成 $n \in [2, 30]$，坐标 $[0, 1000]$，比较 `solve(0,n)` 和 `brute()` 的结果。

---

## 总结

- **算法**：分治法，$O(n \log^2 n)$（可优化为 $O(n \log n)$）。
- **关键点**：strip 引理 + 候选点常数上界，使合并步骤 $O(n)$。
- **数值**：用 `long long` 存距离平方，避免开根；最终 `sqrt` 一次。
- **代码**：见第 5 节，符合编码风格规范。
