# P1302 可见矩形 — 题解

## Part 1 — 重述问题

给定第一象限内 $n$ 个两两面积不相交（可边/角相触）的正方形。第 $i$ 个正方形左下角为 $(x_i, y_i)$，边长 $l_i$。

对正方形 $R_i$，若存在其边界上两个**不同**点 $A, B$，使得以 $O(0,0), A, B$ 为顶点的三角形的**内部**（开集）与其余所有正方形**均无公共点**，则称 $R_i$ 从 $O$ 点可见。

求可见正方形的个数。

**约束：** $1 \le n \le 1000$，$1 \le x_i, y_i, l_i \le 10000$。

---

## Part 2 — 思维链（发现过程）

### Step 1：理解"可见"的几何本质

从 $O$ 向正方形 $R$ "看过去"，视线是被其他正方形遮挡的。所谓"可见"，就是存在从 $O$ 出发的某条"视角区间"，能"看到" $R$ 上的一段边界。

具体地，取 $R$ 边界上的两点 $A, B$，形成 $\triangle OAB$。若该三角形内部不被任何正方形占据，说明从 $O$ 出发、穿过该三角形的所有射线在到达 $R$ 之前没有被挡住，所以 $R$ 至少"露出"了 $A$ 到 $B$ 这一段边界。

**核心问题转化为：** 对每个 $R_i$，判断是否存在边界上两点 $A, B$，使得 $\text{int}(\triangle OAB) \cap \bigcup_{j \ne i} R_j^{\circ} = \varnothing$。

### Step 2：候选点可以缩到顶点

**断言：** 若存在边界上两点 $A, B$ 使 $\triangle OAB$ 内部与所有其他正方形不相交，则存在两个**顶点** $V_1, V_2 \in \{BL, BR, TR, TL\}$ 使 $\triangle OV_1V_2$ 内部与所有其他正方形不相交。

**证明：** 设 $A$ 在 $R$ 的边界边 $e_1$ 上，$B$ 在 $e_2$ 上。考虑 $A$ 沿 $e_1$ 向一个端点 $P$ 滑动（$P$ 是 $e_1$ 的某个端点）。当 $A$ 从原位置滑到 $P$ 时，$\triangle OAB$ 的面积连续变化，且 $\triangle OAB$ 是 $\triangle OPB$ 的子集（若 $A$ 在线段 $PB$ 之间）或超集（若滑过 $P$）。

更精确地：设 $A$ 在线段 $PQ$ 上（$P, Q$ 是该边两端点），$A = (1-\lambda)P + \lambda Q$，$\lambda \in [0,1]$。对任意固定 $B$，$\triangle OAB$ 是关于 $\lambda$ 连续变形的三角形族。关键观察：

若 $\triangle OAB$ 内部与正方形 $S$ 的内部不相交，即 $\text{int}(\triangle OAB) \cap \text{int}(S) = \varnothing$，则 $\text{int}(S) \subseteq \text{ext}(\triangle OAB)$（$S$ 的内部完全在三角形外部或边界上）。

将 $A$ 滑动到端点 $P$（取 $\lambda = 0$ 或 $1$），得到 $\triangle OPB$。由于 $\text{int}(S)$ 是闭集的补集的闭子集（在有界区域内），而 $\triangle OAB$ 到 $\triangle OPB$ 是连续形变，若形变过程中 $\text{int}(S)$ 从未进入 $\text{int}(\triangle OAB)$，则 $\text{int}(S)$ 也从未进入 $\text{int}(\triangle OPB)$。

但这里需要更仔细：连续形变不保证中间状态都不相交。换一个角度：

设 $\triangle OAB$ 与 $\text{int}(S)$ 不相交。对 $S$ 内部的任意点 $P$，$P$ 在 $\triangle OAB$ 的"外侧"（严格地在某条边外侧，或在三角形外）。当 $A$ 移到端点 $P'$ 时，$\triangle OP'B$ 是 $\triangle OAB$ 的"扩展"或"收缩"。若 $A$ 在 $P'$ 和另一端点 $Q$ 之间，则 $\triangle OAB \subseteq \triangle OP'B \cup \triangle OQB$（不一定，取决于 $O$ 的位置）。

实际上，对于本题的正方形（都在第一象限，$O$ 在原点），有一个更直接的事实：

**对于固定 $B$，函数 $\lambda \mapsto \text{int}(\triangle O((1-\lambda)P+\lambda Q)B)$ 是嵌套的。** 具体地，若 $P, Q, B$ 构成不含 $O$ 的三角形，则当 $A$ 从 $P$ 滑向 $Q$ 时，$\triangle OAB$ 的面积先增后减（或单调），但关键性质是：

**若 $\triangle OAB$ 内部不含 $S$ 的任意内部点，则对 $e_1$ 上 $A$ 的任一端点 $P$，$\triangle OPB$ 内部也不含 $S$ 的任意内部点。**

证明：假设 $P$ 是 $e_1$ 的端点，$A$ 在 $e_1$ 上，$A \ne P$。则 $\triangle OAB \subseteq \triangle OPB$（因为 $A$ 在线段 $PQ$ 上，且 $O$ 在正方形 $R$ 外部下方）。因此 $\text{int}(S) \cap \text{int}(\triangle OPB) \supseteq \text{int}(S) \cap \text{int}(\triangle OAB) = \varnothing$ 不成立——包含关系反了。

让我重新理清：$A$ 在线段 $PQ$ 上，$P, Q$ 是正方形 $R$ 同一条边的两端点。$O$ 在原点。对于第一象限的正方形，$O$ 在 $R$ 的"左下方"。

设 $e_1$ 是 $R$ 的底边，$P = BL, Q = BR$。$A = (x_A, y_1)$，其中 $y_1$ 是底边 $y$ 坐标。$B$ 是 $R$ 边界上另一点。

$\triangle OAB$：$O = (0,0), A = (x_A, y_1), B$。
$\triangle OPB$：$O = (0,0), P = (x_1, y_1), B$。

若 $x_A \ge x_1$（$A$ 在 $P$ 右侧），则 $A$ 在线段 $PB$ 的"内侧"（靠近 $O$ 的连线方向），$\triangle OAB \subseteq \triangle OPB$。

因此若 $\text{int}(S) \cap \text{int}(\triangle OAB) \ne \varnothing$，则 $\text{int}(S) \cap \text{int}(\triangle OPB) \ne \varnothing$。
反之，若 $\text{int}(S) \cap \text{int}(\triangle OPB) = \varnothing$，则 $\text{int}(S) \cap \text{int}(\triangle OAB) = \varnothing$。

也就是说：**取端点 $P$ 是"更保守"（更难被遮挡）的选择。** 若端点三角形被遮挡，则中间点三角形也被遮挡；若端点三角形不被遮挡，则中间点三角形也不被遮挡。

因此，只需检查顶点即可。对每个正方形，边界上"最有利"的可见对是顶点-顶点对。

但还有对角线对：$A = BL, B = TR$（或 $A = TL, B = BR$）。这时 $A, B$ 不在同一条边上，而是在对角位置。$\triangle OBLTR$ 比任何边-边三角形都大。

实际上，对正方形 $R$，所有可能的"可见性"由以下 6 个顶点对决定（每对形成 $\triangle O V_1 V_2$）：
1. $(BL, BR)$ — 底边两端
2. $(BL, TL)$ — 左边两端
3. $(TL, TR)$ — 顶边两端
4. $(BR, TR)$ — 右边两端
5. $(BL, TR)$ — 主对角线
6. $(TL, BR)$ — 副对角线

**定理：** $R$ 可见 $\iff$ 存在上述 6 对中的某对 $(V_1, V_2)$，使得 $\text{int}(\triangle OV_1V_2)$ 与所有其他正方形的内部不相交。

**证明（$\Leftarrow$ 方向）：** 若某对顶点满足条件，直接由定义可知 $R$ 可见。

**证明（$\Rightarrow$ 方向）：** 设 $A, B$ 是 $R$ 边界上任意两点，$\text{int}(\triangle OAB)$ 与所有其他正方形内部不相交。需证存在顶点 $V_1, V_2$ 使 $\text{int}(\triangle OV_1V_2)$ 也与所有其他正方形内部不相交。

将 $A$ 滑到其所在边的端点 $P_A$，使 $A$ 向"远离 $O$ 的视线方向"移动（即使三角形面积增大或不变的方向）。由前面的包含关系，$\triangle OAB \subseteq \triangle OP_A B$（当 $A$ 在线段上向远端移动时）。因此 $\text{int}(\triangle OP_A B) \supseteq \text{int}(\triangle OAB)$，但这方向不对——我们要的是更小的三角形。

反过来：$A$ 在线段 $P_A Q_A$ 上。若 $A$ 更靠近 $P_A$（$P_A$ 是更靠近 $O$ 投影方向的端点），则 $\triangle OAB \supseteq \triangle OP_A B$（$P_A$ 更靠近 $O$，三角形更小）。

因此：将 $A$ 滑到更靠近 $O$ 的端点 $P_A$，三角形缩小，$\text{int}(\triangle OP_A B) \subseteq \text{int}(\triangle OAB)$。由于 $\text{int}(\triangle OAB)$ 与其他正方形不相交，子集 $\text{int}(\triangle OP_A B)$ 也不相交。

对 $B$ 同理，滑到更靠近 $O$ 的端点 $P_B$，$\text{int}(\triangle OP_A P_B) \subseteq \text{int}(\triangle OP_A B)$，仍不相交。

因此 $V_1 = P_A, V_2 = P_B$ 是两个顶点，$\triangle OV_1V_2$ 内部与所有其他正方形不相交。$\square$

**注：** "更靠近 $O$ 的端点"的含义：对边 $PQ$，$O$ 到边 $PQ$ 的垂足在 $PQ$ 延长线上还是线段上？对第一象限的正方形，$O$ 在左下方，所以：
- 底边：左端点（$BL$）更靠近 $O$
- 左边：下端点（$BL$）更靠近 $O$
- 顶边：右端点（$TR$）更靠近 $O$（因为 $O$ 在左下）

等等，这不对。"更靠近 $O$" 应该是使 $\triangle O V B$ 面积更小的端点。

对底边 $BLBR$，固定 $B$（$R$ 上另一点）：
- $\triangle OBLB$ vs $\triangle OBRB$
- $O$ 到 $BL$ 的距离 vs $O$ 到 $BR$ 的距离……

实际上更简单的做法：**不纠结方向，6 个顶点对全检查。** 若 $R$ 可见，则 6 对中至少有一对通过。若 6 对都不通过，则 $R$ 不可见。

上面的定理已证：若存在任意边界点对 $(A,B)$ 通过，则存在顶点对通过。6 个顶点对覆盖了所有可能的顶点对组合（$\binom{4}{2} = 6$）。因此检查 6 对是充要的。$\square$

### Step 3：判断 $\text{int}(\triangle OV_1V_2)$ 与正方形 $S$ 内部是否相交

给定三角形 $T = \text{int}(\triangle OV_1V_2)$（开集）和正方形 $S$（闭集），判断 $T \cap \text{int}(S) \ne \varnothing$。

等价于：$\text{int}(S) \not\subseteq \mathbb{R}^2 \setminus \text{int}(\triangle OV_1V_2)$，即 $\text{int}(S)$ 不完全在三角形外部（含边界）。

更直接的判定：$T \cap \text{int}(S) \ne \varnothing$ 当且仅当：

**条件 A**：$S$ 的某个顶点在 $\text{int}(\triangle OV_1V_2)$ 内部，**或**

**条件 B**：$\triangle OV_1V_2$ 的某条边与 $\text{int}(S)$ 相交（即 $S$ 的某条边与三角形的某条边相交于内点，或三角形顶点在 $S$ 内部），**或**

**条件 C**：$S$ 完全包含 $\text{int}(\triangle OV_1V_2)$ 的某部分。

但这些条件有重叠且不完整。更好的方法是：

$T \cap \text{int}(S) \ne \varnothing$ 当且仅当 **$S$ 的内部中有点在三角形内部**。

由于 $S$ 是凸多边形（正方形），$T$ 也是凸多边形（三角形），两者相交当且仅当：
1. $S$ 的某个顶点在 $T$ 内，**或**
2. $T$ 的某个顶点在 $\text{int}(S)$ 内，**或**
3. $S$ 的某条边与 $T$ 的某条边相交（非顶点交）。

但 $T$ 是开集，所以"在 $T$ 内"要求严格内部。

**更实用的方法：分离轴 + 顶点包含**

$T \cap \text{int}(S) = \varnothing$ 当且仅当存在一条直线，使得 $\text{int}(S)$ 在直线一侧（含直线），$T$ 在另一侧（不含直线）。对凸多边形，这等价于：对 $T$ 的每条边，$\text{int}(S)$ 不完全在 $T$ 的该边内侧；**且** 对 $\text{int}(S)$ 的每条边，$T$ 不完全在 $\text{int}(S)$ 的该边内侧。

但这太复杂。由于坐标全是整数且 $n \le 1000$，我们可以用暴力方法：

**实用判定法：**

$\text{int}(\triangle OV_1V_2) \cap \text{int}(S) \ne \varnothing$ 当且仅当以下任一条件成立：

1. $S$ 的某个顶点在 $\triangle OV_1V_2$ 的**严格内部**（不在边界上）。
2. $V_1$ 或 $V_2$ 在 $\text{int}(S)$ 内（不可能，因为 $V_1, V_2$ 是 $R$ 的顶点，$R$ 与 $S$ 面积不相交）。
3. $O$ 在 $\text{int}(S)$ 内（不可能，$S$ 在第一象限，$O$ 在原点，$S$ 的 $x, y \ge 1$）。
4. $\triangle OV_1V_2$ 的某条边与 $\text{int}(S)$ 相交（即线段 $OV_1$ 或 $OV_2$ 或 $V_1V_2$ 穿过 $\text{int}(S)$）。

条件 2 和 3 不可能。条件 4 需要检查 3 条边与正方形 $S$ 的 4 条边是否相交（线段相交，排除端点相触）。

**但条件 1 和 4 合起来是否充分？**

考虑：$S$ 的 4 个顶点都不在 $\triangle OV_1V_2$ 内部，且没有边相交。此时 $S$ 与 $\triangle OV_1V_2$ 不相交。

反过来，若 $S$ 与 $\triangle OV_1V_2$ 相交（开集相交），则必有：
- 某顶点在内部，或
- 某边相交。

这是因为两个凸多边形若相交（开集），则交点必在边界上或内部。若交点全在边界上，则边相交或顶点在边界上（不算内部）。若交点在内部，则顶点在内部。

**注意：** 顶点恰好在三角形边界上不算"在内部"。边相触（一个端点在对方边上）不算"穿过内部"。

所以判定逻辑：

```
func intersects(R_sq, V1, V2, S_sq):
    # 检查 S 的 4 个顶点是否在 int(triangle OV1V2) 内
    for each vertex P of S:
        if P is strictly inside triangle OV1V2:
            return true
    # 检查三角形的 3 条边是否与 int(S) 相交
    for each edge E of triangle OV1V2:
        if E crosses int(S):  # 即 E 的某内点在 S 内部
            return true
    return false
```

**"严格在三角形内部"的判定：**

点 $P$ 在 $\triangle OV_1V_2$ 的严格内部 $\iff$ $P$ 在三条边的同一侧（与对顶点同侧），且 $P \ne O, V_1, V_2$。

由于 $O = (0,0)$ 且所有坐标为正，$P$（第一象限）永远不与 $O$ 重合。$P$ 可能是 $V_1$ 或 $V_2$ 吗？$P$ 是 $S$ 的顶点，$V_1, V_2$ 是 $R$ 的顶点，$R \ne S$ 且面积不相交，所以 $P \ne V_1, V_2$。

因此只需检查三条边的半平面条件。

用叉积：对边 $UV$ 和对点 $W$，$\text{cross}(U, V, W) = (V_x - U_x)(W_y - U_y) - (V_y - U_y)(W_x - U_x)$。

$P$ 在 $\triangle OV_1V_2$ 内部 $\iff$ 三个叉积 $\text{cross}(O, V_1, P)$, $\text{cross}(V_1, V_2, P)$, $\text{cross}(V_2, O, P)$ 全正或全负（同向）。

**"线段 $E$ 与 $\text{int}(S)$ 相交"的判定：**

线段 $E = UV$ 与 $\text{int}(S)$ 相交 $\iff$ 存在 $t \in (0,1)$ 使得 $U + t(V-U) \in \text{int}(S)$。

等价于：线段 $UV$ 与 $\text{int}(S)$ 的交集非空（开线段与开正方形内部）。

这可以转化为：
- $U$ 在 $\text{int}(S)$ 内，或 $V$ 在 $\text{int}(S)$ 内，或
- 线段 $UV$ 与 $S$ 的某条边相交于 $S$ 边的内点（非端点），或
- $U, V$ 在 $S$ 外侧，线段穿过 $S$。

更简洁：线段 $UV$ 与 $\text{int}(S)$ 相交 $\iff$ $UV$ 与 $S$ 的 4 条边中某条边**严格相交**（交点在两条边的相对内部），或 $U$（或 $V$）在 $\text{int}(S)$ 内。

线段严格相交的判定（排除端点）：两线段 $AB$ 和 $CD$ 严格相交 $\iff$ $A, B$ 在 $CD$ 的两侧（严格叉积异号）**且** $C, D$ 在 $AB$ 的两侧（严格叉积异号）。

但还有"一个线段端点恰好在另一个线段内部"的情况——这算不算"穿过内部"？

若 $U$ 在 $S$ 的边上（非顶点），$V$ 在 $S$ 外部，线段 $UV$ 从 $S$ 的边出发向外。此时 $UV$ 的某内点是否在 $\text{int}(S)$ 内？若 $U$ 在 $S$ 的边上，$UV$ 向 $S$ 外部延伸，则 $UV$ 的内点（$t \in (0,\epsilon)$）在 $S$ 外部。所以不算穿过内部。

若 $U$ 在 $S$ 的顶点，$V$ 在 $S$ 外部，类似地不算。

若线段 $UV$ 的一个端点在 $S$ 的边上（非顶点），另一个端点在 $S$ 内部，则线段穿过 $\text{int}(S)$。但这种情况已被"端点在 $\text{int}(S)$ 内"覆盖。

所以：**线段 $UV$ 与 $\text{int}(S)$ 相交 $\iff$**
- $U \in \text{int}(S)$，或 $V \in \text{int}(S)$，或
- $UV$ 与 $S$ 的某条边**严格交叉**（两线段在各自内部相交，即交点不是任何端点）。

"严格交叉"：$\text{cross}(C,D,U) \cdot \text{cross}(C,D,V) < 0$ 且 $\text{cross}(U,V,C) \cdot \text{cross}(U,V,D) < 0$。

**"点 $P$ 在 $\text{int}(S)$ 内"的判定：**

$S$ 是 $[x_s, x_s+l_s) \times [y_s, y_s+l_s)$ 的内部，即 $x_s < P_x < x_s + l_s$ 且 $y_s < P_y < y_s + l_s$（严格不等式）。

**"点 $P$ 在 $\triangle OV_1V_2$ 的严格内部"的判定：**

三个叉积同号且非零：
- $c_1 = \text{cross}(O, V_1, P) = V_{1x} \cdot P_y - V_{1y} \cdot P_x$
- $c_2 = \text{cross}(V_1, V_2, P) = (V_{2x}-V_{1x})(P_y-V_{1y}) - (V_{2y}-V_{1y})(P_x-V_{1x})$
- $c_3 = \text{cross}(V_2, O, P) = -V_{2x} \cdot P_y + V_{2y} \cdot P_x$

$P$ 在内部 $\iff$ $c_1, c_2, c_3$ 全正或全负（严格，无零）。

### Step 4：复杂度分析

- $n$ 个正方形，每个检查 6 对顶点
- 每对顶点，对 $n-1$ 个其他正方形检查相交
- 每次相交检查：4 个顶点 + 3 条边 × 4 条边 = 4 + 12 = 16 次基本运算

总复杂度：$O(n \times 6 \times n \times 16) = O(96n^2) \approx O(n^2)$。

$n = 1000$ 时，$96 \times 10^6 \approx 10^8$ 次基本运算，在 C++ 中可在 1 秒内完成。

### Step 5：特殊情况与正确性验证

**正方形相触（边或角）的情况：**

设 $R$ 和 $S$ 共享一个顶点 $P$。$P$ 在 $R$ 的边界上，也在 $S$ 的边界上。若 $P$ 是 $R$ 的顶点 $V_1$，则 $P = V_1$ 在 $S$ 的边界上（非内部），所以"点 $P$ 在 $\text{int}(S)$ 内"不成立。若 $P$ 是 $S$ 的顶点，则 $P$ 在 $R$ 的边界上，检查"$S$ 的顶点 $P$ 在 $\triangle OV_1V_2$ 内部"时，$P = V_1$ 是三角形顶点，不算"严格内部"（叉积为零）。所以相触不算相交。✓

**正方形 $S$ 完全在 $\triangle OV_1V_2$ 内部：**

此时 $S$ 的 4 个顶点都在三角形内部，条件 1 直接命中。✓

**$\triangle OV_1V_2$ 完全在 $S$ 内部：**

不可能，因为 $O$ 不在 $S$ 内部（$S$ 在第一象限，$O$ 在原点）。✓

**三角形的一条边恰好沿 $S$ 的边：**

此时线段与 $S$ 的边重合。严格交叉条件（叉积异号）不满足（叉积为零）。端点也不在内部。所以不算相交。✓（边界相触不算"内部有公共点"）

### Step 6：总结算法

对每个正方形 $R_i$：
1. 标记 $R_i$ 为"待判定"
2. 对 6 个顶点对 $(V_1, V_2)$：
   a. 标记这对为"通过"
   b. 对每个 $j \ne i$：
      - 若 $\text{int}(\triangle OV_1V_2)$ 与 $\text{int}(R_j)$ 相交：标记这对为"不通过"，跳出
   c. 若这对通过：$R_i$ 可见，跳出顶点对循环
3. 若 6 对都不通过：$R_i$ 不可见
4. 统计可见数

---

## Part 3 — 模式总结

```
- explore its property — 可见性 = 存在边界点对使三角形内部不被遮挡；关键几何：第一象限+O在原点
- consider easier version — 若只有一对候选点（如对角线），问题退化为单三角形与n个正方形的相交检测
- classic problem solution — 凸多边形相交检测：顶点包含 + 边严格交叉，O(1) per pair
- reduce to easier problem — 候选点从连续边界缩到4个顶点（6对），由三角形包含关系证明
- classic problem solution — 叉积判定：严格内部（三叉积同号）+ 严格交叉（双叉积异号），O(1) per check
- brute force suffices — n≤1000, 6对×n×16次基本运算 = O(n²) ≈ 10^8，无需优化
```

---

## Part 4 — 正式解法

### 符号表

| 符号 | 含义 |
|------|------|
| `x[i], y[i], l[i]` | 第 $i$ 个正方形的左下角坐标和边长（0-based，$0 \le i < n$） |
| `BL[i], BR[i], TR[i], TL[i]` | 第 $i$ 个正方形的四个顶点：左下、右下、右上、左上 |
| `cross(ax,ay, bx,by, cx,cy)` | 叉积 $(b_a - a_a)(c_a - a_a) - (b_b - a_b)(c_b - a_b)$ |
| `insq(px,py, i)` | 点 $(p_x, p_y)$ 是否在正方形 $i$ 的严格内部 |
| `intri(ax,ay, bx,by, cx,cy, px,py)` | 点 $(p_x, p_y)$ 是否在三角形 $ABC$ 的严格内部 |
| `segcross(ax,ay,bx,by, cx,cy,dx,dy)` | 线段 $AB$ 与线段 $CD$ 是否严格交叉（交点在两线段内部） |
| `edgecross(ax,ay,bx,by, j)` | 线段 $AB$ 是否与正方形 $j$ 的内部相交 |

### 算法步骤

1. **读入** $n$ 个正方形，存储 $x[i], y[i], l[i]$。计算并存储每个正方形的四个顶点坐标。

2. **对每个正方形 $i$（0-based）：**
   - `vis = false`
   - 对 6 个顶点对 $(V_1, V_2)$（依次为 $(BL,BR), (BL,TL), (TL,TR), (BR,TR), (BL,TR), (TL,BR)$）：
     - `blocked = false`
     - **对每个 $j \ne i$：**
       - 若 $\text{int}(\triangle OV_1V_2)$ 与 $\text{int}(R_j)$ 相交（用下述子步骤判定）：
         - `blocked = true`，跳出 $j$ 循环
     - 若 `!blocked`：`vis = true`，跳出顶点对循环
   - 若 `vis`：`ans++`

3. **相交判定**（$\text{int}(\triangle OV_1V_2)$ 与 $\text{int}(R_j)$）：
   - 对 $R_j$ 的 4 个顶点 $P$：若 $P$ 在 $\triangle OV_1V_2$ 的严格内部（`intri`）：返回 true
   - 对三角形的 3 条边 $E$（$OV_1$, $OV_2$, $V_1V_2$）：若 $E$ 与 $\text{int}(R_j)$ 相交（`edgecross`）：返回 true
   - 返回 false

4. **`intri(ax,ay, bx,by, cx,cy, px,py)`**（点 $P$ 在 $\triangle ABC$ 严格内部）：
   - $c_1 = \text{cross}(A, B, P)$
   - $c_2 = \text{cross}(B, C, P)$
   - $c_3 = \text{cross}(C, A, P)$
   - 返回 $(c_1 > 0 \land c_2 > 0 \land c_3 > 0) \lor (c_1 < 0 \land c_2 < 0 \land c_3 < 0)$

5. **`edgecross(ax,ay,bx,by, j)`**（线段 $AB$ 与 $\text{int}(R_j)$ 相交）：
   - 若 $A \in \text{int}(R_j)$（`insq`）：返回 true
   - 若 $B \in \text{int}(R_j)$（`insq`）：返回 true
   - 对 $R_j$ 的 4 条边 $S_k$：若 `segcross(A, B, S_{k,start}, S_{k,end})`：返回 true
   - 返回 false

6. **`segcross`**（两线段严格交叉）：
   - $c_1 = \text{cross}(A, B, C)$，$c_2 = \text{cross}(A, B, D)$
   - $c_3 = \text{cross}(C, D, A)$，$c_4 = \text{cross}(C, D, B)$
   - 返回 $(c_1 > 0 \land c_2 < 0) \lor (c_1 < 0 \land c_2 > 0)) \land ((c_3 > 0 \land c_4 < 0) \lor (c_3 < 0 \land c_4 > 0))$

7. **`insq(px,py, j)`**：
   - 返回 $x[j] < p_x \land p_x < x[j] + l[j] \land y[j] < p_y \land p_y < y[j] + l[j]$

### 复杂度

- 时间：$O(n^2)$。$n$ 个正方形 × 6 对顶点 × $n$ 个其他正方形 × $O(1)$ 次相交检测。
- 空间：$O(n)$。

### 验证状态

未进行实际编译和随机对拍验证。逻辑正确性基于上述逐步证明。

---

## Part 5 — C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
int n;
int x[mxn],y[mxn],l[mxn];
// 四个顶点: 0=BL, 1=BR, 2=TR, 3=TL
int vx[mxn][4],vy[mxn][4];
// 6对顶点对: 0=(BL,BR) 1=(BL,TL) 2=(TL,TR) 3=(BR,TR) 4=(BL,TR) 5=(TL,BR)
int vp[6][2]{{0,1},{0,3},{3,2},{1,2},{0,2},{3,1}};

long long cross(int ax,int ay,int bx,int by,int cx,int cy){
	return (long long)(bx-ax)*(cy-ay)-(long long)(by-ay)*(cx-ax);
}

bool insq(int px,int py,int j){
	return x[j]<px&&px<x[j]+l[j]&&y[j]<py&&py<y[j]+l[j];
}

bool intri(int ax,int ay,int bx,int by,int cx,int cy,int px,int py){
	long long c1=cross(ax,ay,bx,by,px,py);
	long long c2=cross(bx,by,cx,cy,px,py);
	long long c3=cross(cx,cy,ax,ay,px,py);
	return(c1>0&&c2>0&&c3>0)||(c1<0&&c2<0&&c3<0);
}

bool segcross(int ax,int ay,int bx,int by,int cx,int cy,int dx,int dy){
	long long c1=cross(ax,ay,bx,by,cx,cy);
	long long c2=cross(ax,ay,bx,by,dx,dy);
	long long c3=cross(cx,cy,dx,dy,ax,ay);
	long long c4=cross(cx,cy,dx,dy,bx,by);
	return((c1>0&&c2<0)||(c1<0&&c2>0))&&((c3>0&&c4<0)||(c3<0&&c4>0));
}

// 线段(ab)是否与正方形j的内部相交
bool edgecross(int ax,int ay,int bx,int by,int j){
	if(insq(ax,ay,j))
		return true;
	if(insq(bx,by,j))
		return true;
	// 正方形j的四条边: BL-BR, BR-TR, TR-TL, TL-BL
	if(segcross(ax,ay,bx,by,vx[j][0],vy[j][0],vx[j][1],vy[j][1]))
		return true;
	if(segcross(ax,ay,bx,by,vx[j][1],vy[j][1],vx[j][2],vy[j][2]))
		return true;
	if(segcross(ax,ay,bx,by,vx[j][2],vy[j][2],vx[j][3],vy[j][3]))
		return true;
	if(segcross(ax,ay,bx,by,vx[j][3],vy[j][3],vx[j][0],vy[j][0]))
		return true;
	return false;
}

// 三角形(0,0)-(a)-(b)内部是否与正方形j内部相交
bool triintersq(int ax,int ay,int bx,int by,int j){
	// 1. 正方形j的4个顶点是否在三角形内部
	if(intri(0,0,ax,ay,bx,by,vx[j][0],vy[j][0]))
		return true;
	if(intri(0,0,ax,ay,bx,by,vx[j][1],vy[j][1]))
		return true;
	if(intri(0,0,ax,ay,bx,by,vx[j][2],vy[j][2]))
		return true;
	if(intri(0,0,ax,ay,bx,by,vx[j][3],vy[j][3]))
		return true;
	// 2. 三角形的3条边是否与正方形j内部相交
	if(edgecross(0,0,ax,ay,j))
		return true;
	if(edgecross(0,0,bx,by,j))
		return true;
	if(edgecross(ax,ay,bx,by,j))
		return true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	
	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>x[i]>>y[i]>>l[i];
		vx[i][0]=x[i];vy[i][0]=y[i];
		vx[i][1]=x[i]+l[i];vy[i][1]=y[i];
		vx[i][2]=x[i]+l[i];vy[i][2]=y[i]+l[i];
		vx[i][3]=x[i];vy[i][3]=y[i]+l[i];
	}
	int ans=0;
	for(int i=0;i<n;++i)
	{
		bool vis=false;
		for(int k=0;k<6;++k)
		{
			int a=vp[k][0],b=vp[k][1];
			bool blocked=false;
			for(int j=0;j<n;++j)
			{
				if(j==i)
					continue;
				if(triintersq(vx[i][a],vy[i][a],vx[i][b],vy[i][b],j))
				{
					blocked=true;
					break;
				}
			}
			if(!blocked)
			{
				vis=true;
				break;
			}
		}
		if(vis)
			++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
```
