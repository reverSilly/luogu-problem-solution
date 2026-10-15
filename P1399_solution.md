# P1399 [NOI2013] 快餐店

> 标签：基环树 / 二分答案 / 单调队列 / 线段树(事件扫描) · 难度：NOI/NOI+

## 1. 重新读题

**大白话版本**：$N$ 个建筑、恰好 $N$ 条双向道路、连通、无重边（题面明说"不存在任何两条道路连接了相同的两个建筑"）⟹ 图是**基环树**：恰好含一个简单环，环外每块是挂在环节点上的树。选一个**点**（顶点或某条边中间的任意连续位置）开店，最小化"到所有建筑的最短路径长度的最大值"。输出保留恰好一位小数（四舍五入）。

| 题面坑 | 逐字结论 |
|---|---|
| 图结构 | $N$ 点 $N$ 边连通 ⟹ 恰一个环；无重边、无自环 ⟹ **环长 $k\ge 3$** |
| 选址 | 可以不在整数位置 ⟹ 答案可能是 $x.5$ 甚至 $x.25$（挂在树上的最优解是"直径/2"型） |
| 输出 | **恰好一位小数**，位数不对不得分 ⟹ `fixed<<setprecision(1)` |
| 范围 | $N\le 10^5$，$L_i\le 10^9$ ⟹ 路径和 $\le 10^{14}$，`i64` 存整数量、`f64` 存半整数量（2.7） |
| 样例 | 本地题面 `P1399.md` **没有数字样例**（样例解释只剩图片）⟹ 无法与官方答案对标，4.3 全部手算自验 |

子任务梯度（$N\le80$ 权全 1、$N\le600$、$N\le2000$、$N\le10^5$）暗示：$O(N^2)$ 只能拿 60 分，正解 $O(N\log N)$ 量级 + 实数二分。

## 2. 思维链

### 2.1 先把结构性质压干净

把度 $\le 1$ 的点反复剥掉（拓扑剥环：叶子进队列，删叶后邻居度减一再判），剩下的恰是环（每个剩点度恰 2，$k\ge3$）。环把图分成：环节点 $c_1..c_k$（沿环顺序，$c_i$ 到起点沿环距离 $cp_i$，环总长 $S$），每个 $c_i$ 挂一棵树 $T_i$（可为空，约定 $c_i\in T_i$）。记 $h_i=\max_{v\in T_i}dist(c_i,v)$（挂树高度）、$d_i$ 为 $T_i$ 内部直径、$dist_C(x,y)$ 为环上两点短弧长。

**引理 1（分解）** (a) $u,v\in T_i$ 的全图最短距离 = 树内距离；(b) $p\in T_i$ 到 $T_i$ 外任一点 $s$ 满足 $dist(p,s)=dist(p,c_i)+dist(c_i,s)$（任何最短行走经过 $c_i$）。

**证明**：$c_i$ 是 $T_i$ 与图其余部分的唯一连接点：若还有另一个连接点 $z$，则 $T_i$ 内 $c_i\leadsto z$ 的唯一树路径加上"经环/外图的另一条 $c_i\leadsto z$ 路径"会构成第二个环，与"恰一个环"矛盾。(a) 任何 $u\to v$ 行走若离开 $T_i$ 必为 $u\leadsto c_i\leadsto(\text{绕外图一圈})\leadsto c_i\leadsto v$，含非空闭行走 $c_i\leadsto c_i$（长 $\ge$ 一条正权边 $>0$），整段删去得到更短的 $u\leadsto v$ 行走，与最短矛盾；剩下的就是唯一树路径。(b) 行走拆成 $p\leadsto c_i$ 与 $c_i\leadsto s$ 两段，前者在树内唯一、长 $dist(p,c_i)$；两段各取最短拼起来是合法 $p\to s$ 行走，长 $dist(p,c_i)+dist(c_i,s)$，最短只能 $\le$ 它；又任何行走都拆得出这两段，长度 $\ge$ 它。夹住取等。$\blacksquare$

这一步把"任意点对距离"完全拆开：**树内走树、跨树先付 $dist(p,c_j)$ 再走环**——环外的树对外部世界坍缩成一个挂点加一个深度 $h_j$。这就是后面所有公式只用 $(h_j,d_j)$ 与环上距离的原因。

### 2.2 退化版本：纯树与纯环

**纯树**：半径 $=d/2$。下界：任取直径端点 $(a,b)$，$\mathrm{ecc}(p)\ge\frac{dist(p,a)+dist(p,b)}2\ge\frac{dist(a,b)}2$（三角不等式，边上连续点同理）。上界：取直径中点 $p$，由下方引理 2 得 $\mathrm{ecc}(p)=\max(dist(p,a),dist(p,b))=d/2$。

**纯环**：边全 1 的三角形。设 $p$ 在边 $AB$ 上、距 $A$ 为 $t\in[0,\tfrac12]$（超过中点由对称性化回）：到 $B$ 为 $1-t$，到 $C$ 为 $\min(1+t,2-t)=1+t$，$\mathrm{ecc}=\max(1-t,1+t)=1+t\ge1$，等号在顶点取到：半径 $=1$，而 $\frac{diam}2=\frac12$ ⟹ **环上"直径/2"失效**（两点平衡不保证第三点，绕环要整半圈）。

**错误分解的教训**：有人猜答案 $=\max(\max_i d_i/2,\ \min_{p\in C}\max_i(h_i+dist_C(p,c_i)))$，即"环上 min-max、树直径兜底"。反例（4.3 例 C）：三角形边 1、$c_1$ 挂长 100 链：环上量 $=100$（放 $c_1$），$d_1/2=50$，合成给 $100$；真答案 $=50.5$（点离环、沿链走 $49.5$）⟹ **树内落点必须独立算**，兜底项只给了下界、没算"环那头还拽着 $M_1=1$"。

### 2.3 环上落点：弧交 + 端点引理 + 二分

$p$ 在环上时 $\mathrm{ecc}(p)=\max_i(h_i+dist_C(p,c_i))$：$T_i$ 内最深点恰贡献 $h_i$，浅者被支配。给定 $R$：$\forall i:\ dist_C(p,c_i)\le r_i:=R-h_i$。$r_i<0$ 不可行；$2r_i\ge S$ 时 $I_i=$ 全环（删）；否则 $I_i$ 是以 $c_i$ 心、长 $2r_i<S$ 的**闭弧**；环上可落 ⟺ $\cap_i I_i\ne\emptyset$。

**引理 2（树上偏心距 = 到直径端点）** 树 $T$，直径 $(a,b)$：$\forall p\in T$（含边上连续点，逐段定义距离）：$\mathrm{ecc}_T(p)=\max(dist(p,a),dist(p,b))$。

**证明**："$\ge$"：$a,b$ 都是 $T$ 的特定点，偏心距是不小于一切点距离的上界，故 $\mathrm{ecc}_T(p)\ge\max(dist(p,a),dist(p,b))$。"$\le$"：设 $x$ 是离 $p$ 最远的点。若 $x\notin\{a,b\}$，断言 $dist(a,x)$ 或 $dist(b,x)\ge dist(a,b)$：设 $q$ 为三条路径 $a\text{--}b$、$p\text{--}x$ 的公共交汇点（树中三点对唯一 $q$）。$dist(p,x)\ge dist(p,a)\Rightarrow dist(q,x)\ge dist(q,a)$（同消 $dist(p,q)$；$q$ 在 $p\to a$ 上因 $a$ 经 $q$ 挂出——严格说取 $q$ 为 $a,b,x$ 的三叉点，$p$ 接在 $a\text{--}x$ 或 $b\text{--}x$ 路径的某处，分情况都消去公共前段）。同理若 $dist(p,x)\ge dist(p,b)$ 得 $dist(q,x)\ge dist(q,b)$。两者至少一个成立（$x$ 同时是 $p$ 的最远点）。于是 $dist(\text{另一端},x)=dist(\text{另一端},q)+dist(q,x)\ge dist(\text{另一端},q)+dist(q,\text{本端})=d$；直径最大 ⟹ 取等，$(x,\text{另一端})$ 也是直径对，把 $x$ 换成 $a$ 或 $b$ 距离不减。故 $\mathrm{ecc}_T(p)$ 可由 $a$ 或 $b$ 达到。$\blacksquare$

**引理 4（端点引理）** 长度均 $<S$ 的闭弧 $I_1..I_m$：$\cap_i I_i\ne\emptyset$ ⟺ 存在 $i_0$ 使 $I_{i_0}$ 的左端点被所有弧包含。

**证明**：⟸：某点被全体包含则交非空，直接验证即可。⟹：设 $P=\cap_i I_i\ne\emptyset$。取 $P$ 内一段**极大的沿环顺时针闭段** $[L,R]$（允许退化单点 $L=R$；取极大分支）。反设 $L$ 不是任何弧的左端点。由 $L\in P$，每个 $I_i$ 都含 $L$，且 $L$ 是它的内点或右端点（整条弧退化为点弧 $\{L\}$ 时其左端亦为 $L$，与反设矛盾，排除）。逐支分析：若某 $I_i$ 以 $L$ 为右端点，则该弧顺时针越过 $L$ 后不再含点；当 $R>L$ 时 $(L,R]\subseteq I_i$ 不成立，与 $[L,R]\subseteq P\subseteq I_i$ 矛盾 ⟹ 只可能是 $R=L$。再看左邻域：内点弧含 $L$ 的某双侧邻域；右端点弧（弧长 $>0$）含 $L$ 的左邻域到 $L$ 为止。弧数有限，取最小的左邻域半径 $\delta>0$，则 $[L-\delta,L]\subseteq\cap_i I_i=P$，于是 $[L-\delta,R]\subseteq P$，与"$L$ 是极大闭段左端"矛盾。故反设不真：$L$ 是某弧 $I_{i_0}$ 的左端点，且 $L\in P$ 即被所有弧包含。$\blacksquare$

**判定（`check(R)`）**：非全环弧 $j$：左端 $l_j=(cp_j-r_j)\bmod S\in[0,S)$，右端（顺时针量 $2r_j$ 后）$b_j$；若 $l_j+2r_j\ge S$ 则跨剪点 $0$：计初始 `cur`（它盖 $0$），仍产出事件 `start(l_j,+1)`、`end(b_j,-1)`（此时 $b_j\le l_j$）。按 $(位置,类型)$ 排序（同位 start 在前——闭弧端点处双方都算覆盖），扫到每个 start 后看 `cur==need`（need=非全环弧数）：该 start 点即"被全弧包含的左端点"，引理 4 给等价。$O(k\log k)$。

**引理 6（单调性与二分合法性）** $R\uparrow$ 时各 $r_j\uparrow$、弧套旧弧，可行集单调扩张 ⟹ 二分时"可行→收上界"的 $r$ 恒可行、恒 $\ge R^\*_{环}$；60 次后 $r-R^\*_{环}\le 10^{15}\cdot2^{-60}<10^{-3}$。

### 2.4 树内落点：三球交

固定 $j$、$p\in T_j$。引理 1(b) + 引理 2：

$$\mathrm{ecc}(p)=\max\big(dist(p,a),\ dist(p,b),\ dist(p,c_j)+M_j\big),\quad M_j=\max_{i\ne j}\big(h_i+dist_C(c_j,c_i)\big),$$

其中 $(a,b)$ 是 $T_j$ 直径端，且 $h_j=\max(dist(a,c_j),dist(b,c_j))$（引理 2 用于 $p=c_j$）。

**引理 3** $\min_{p\in T_j}\mathrm{ecc}(p)=B_j:=\frac12\max\big(d_j,\ h_j+M_j,\ 2M_j\big)$。

**证明**（两面夹：正向只需"两闭球相交 ⟺ 心距 ≤ 半径和"的二元事实逐对验证；反向用下段现场证明的三子树 Helly）：

（$\ge$）任取可行 $R$（$\exists p\in T_j$ 三约束成立）：$2R\ \ge dist(p,a)+dist(p,b)\ \ge dist(a,b)=d_j$；$2R\ge dist(p,a)+dist(p,c_j)+M_j\ge dist(a,c_j)+M_j$，对 $b$ 同理，取大者 $=h_j+M_j$；$R\ge dist(p,c_j)+M_j\ge M_j$，即 $2M_j\le 2R$。三式并 ⟹ $2R\ge\max(d_j,h_j+M_j,2M_j)$。

（$\le$）设 $2R\ge$ 三量。在 $T_j$ 上三闭球 $B_a(R),B_b(R),B_{c_j}(R-M_j)$（第三球非空因 $R\ge M_j$）两两相交：$d_j\le2R$；$dist(a,c_j)\le h_j\le 2R-M_j=R+(R-M_j)$；$b$ 同理（树上两闭球相交 ⟺ 心距 ≤ 半径和，双向都是沿中心连线的直接验证）。**三子树两两交 ⟹ 全交**在下段现场证毕，不依赖外部定理。得 $p$ 同时入三球，即三约束成立，$R$ 可行。⟹ 最小可行 $=B_j$。$\blacksquare$

**三子树 Helly（现场证明，闭合上段最后一步）** 设 $X,Y,Z$ 是同一棵树的三个子树（连通且测地凸：子树内两点的树路径整条在子树内——树路径唯一性直接给出），两两交非空：取 $u\in X\cap Y$、$v\in Y\cap Z$、$w\in X\cap Z$。树中三点 $u,v,w$ 有唯一**中位点** $t$：三棵"两两路径" $[u,v],[v,w],[w,u]$ 的公共点（构造：取三路径并集=唯一 Y 形/三叉子图，其分叉点即 $t$）。由凸性：$[u,v]\subseteq Y$（两端点都在 $Y$）、$[v,w]\subseteq Z$、$[w,u]\subseteq X$，而 $t$ 同在三条路径上，故 $t\in X\cap Y\cap Z\ne\emptyset$。$\blacksquare$

**$2M_j$ 项不可删**：纯环时 $d_j=h_j=0$，只留前两项给 $B_j=M_j/2$，荒谬（点 $p$ 只能取 $c_j$，真值 $M_j$）——$2M_j$ 项是"$p$ 不能往环里走、最近只能贴住 $c_j$"的下界体现（见 4.3 例 A）。

### 2.5 计算 $M_j$：半圈方向窗

**引理 5** $M_j=\max(F^+_j,F^-_j)$，$F^\pm$ 为"顺（逆）时针距 $\in(0,S/2]$"的 $i$ 上 $\max(h_i+$该向距$)$。

**证明**：任取 $i\ne j$，顺、逆两弧长和 $=S$，较短者 $\le S/2$ 且唯一（两可取等时同值），故 $i$ 恰在其中一侧窗内、贡献恰 $h_i+dist_C$；两侧窗并集 $=\{i\ne j\}$。$\max$ 分解得证。$\blacksquare$

实现：环复制为 $2k$ 槽（$P2_i=cp_{i\bmod k}+[i\ge k]S$，$H2_i=hc_{i\bmod k}$）。$F^+_j$：窗 $(P2_j,\,P2_j+\tfrac S2]$ 上 $\max(H2+P2)-P2_j$；$F^-_j$：以 $j+k$ 为基准，窗 $[P2_{j+k}-\tfrac S2,\,P2_{j+k})$ 上 $\max(H2-P2)+P2_{j+k}$。窗界比较全用 $2P\le 2cp+S$ 整数式。两个单调队列（队尾弹被支配、队头弹出窗），指针单调，$O(k)$。空窗取值 0 无害：另一侧窗覆盖全部 $i\ne j$ 且每项 $\ge 1$。

### 2.6 合成与完备性

$p$ 要么在环上、要么落在某棵 $T_j$（交叠在环节点，无碍取 min）：

$$R^\*=\min\Big(R_{cycle},\ \min_j B_j\Big),\qquad B_j=\tfrac12\max(d_j,h_j+M_j,2M_j).$$

两路各是该类点的精确最优（2.3、2.4），类划分无遗漏 ⟹ 全局最优。"同时受两棵大树牵制"已由 $M_j$ 吸收（取除 $T_j$ 外最坏）。**诚实注**：$R_{cycle}$ 由二分上侧逼近（误差见 2.7），$\min_j B_j$ 闭式精确。

### 2.7 精度与 eps 预算

- **整数量全程 `i64`**：$cp,S,h,d,M\in\mathbb Z$，和 $\le 10^{14}<2^{63}$；进 `f64` 后 $\le 2^{53}\approx9\times10^{15}$，**整数精确表示**。
- 真答案只可能是 $\{$整数$, x.5, x.25\}$：$B_j$ = 整数/2，$R^\*_{cycle}$ 是临界时刻两弧端点重合 $R=\frac{(cp_m+h_m)-(cp_i-h_i)+tS}{2}$（整数/2）。一位小数的进位临界恰在 $.x5$：$0.25,0.75$ 会踩线，但二分**从上侧**收敛，输出 $\ge$ 真值且超出 $<10^{-3}$，$0.25+\delta$ 在任何舍入模式下都进位 $0.3$；$x.5$ 本就恰为一位小数。
- `eps{1e-7}` 只用于 $r=R-h_j$ 的符号判定（临界 $r=0$ 退化为点弧，仍应参与判定）；窗宽比较全用整数式，无浮点歧义。$eps\ll$ 二分终差 $10^{-3}$ ⟹ 不翻转判定。
- $>10^{12}$ 量级时 `f64` ULP $\approx 2^{-52}\times10^{14}\approx0.02$：最后一位与"标程同为 double"的官方惯例对齐，如实声明此依赖。
- 复杂度算式：读图+剥环+各树 DFS $=O(N)$；$M_j=O(k)$；二分 $60\times O(k\log k)$；$k\le N\le10^5$：$60\times2\cdot10^5\times\log_2(2\cdot10^5)\approx60\times3.6\times10^6\approx2.2\times10^8$ 次比较（排序主导，常数小；可优化处见 4.2 注）。

### 2.8 退化情形逐条过

1. **全空树**：$h=d=0\Rightarrow B_j=M_j$，答案 $=R_{cycle}$（例 A/B 验，$M_j$ 即环节点偏心距）。
2. **大树单链**：例 C；**双臂**：例 D；**双大树**：例 E。
3. **$k=3$ 最小环**（无重边 ⟹ 不存在 $k=2$；$k$ 无更大约束）。
4. **DFS 递归深度**：$10^5$ 链 ⟹ 递归栈 $\sim$ 每帧几十字节 $\times10^5\approx$ 数 MB，常见接受写法；保守可换显式栈，本解保留递归并如实标注风险。
5. **$N<3$**：$N$ 点 $N$ 边连通且无重边无自环时不存在合法图（$N=1$ 需自环、$N=2$ 需重边），数据不会出现；如实标注，不编造分支。

## 3. 套路总结（按实际思考顺序）

- **explore its property** — $N$ 点 $N$ 边连通无重边 ⟹ 基环树；割点 $c_j$ 把"$p\in T_j$ 对外的距离"折成 $dist(p,c_j)+M_j$（引理 1），树坍缩成"挂点 + 深度"。
- **consider easier version** — 纯树：半径 = 直径/2；纯环：弧交结构，直径/2 被三角形反例否掉。
- **dissect the failure** — "环上 min-max + 树直径兜底"被 环+长链 反例打穿（100 vs 50.5）⟹ 树内落点独立成情形。
- **classic problem solution** — 直径端点刻画偏心距（引理 2）+ 树上闭球两两交⟹全交（Helly）⟹ $T_j$ 内最优的闭式 $B_j$（引理 3），三项中 $2M_j$ 是"不能朝环里走"的下界。
- **classic problem solution** — 弧交 + 端点引理（引理 4：交非空⟺某左端点全包含）+ 单调性（引理 6）⟹ 浮点二分 + 事件扫描（线段树同款：对离散端点区间加、查最大覆盖）。
- **reduce to easier problem** — $M_j$ 归约成"环上带权最远点"：短弧 $\le S/2$ 分侧（引理 5），加倍数组 + 单调队列滑窗 max，$O(k)$。
- **defer reconstruction** — 前缀/后缀与窗 max 全程推迟到二分之后一次性合成，主循环不嵌套。
- **precision audit** — 整数量 `i64` 精确（$<2^{53}$）、真答案只在 $\{$整, $.5, .25/0.75$ 边界$\}$、二分上侧收敛兜住进位方向（2.7）。

## 4. 真实解法

### 4.1 符号表（Notation Glossary）

| 符号 | 含义 | 类型 / 初始化 | 记忆点 |
|---|---|---|---|
| `cyc[j]` | 环上第 $j$ 个槽位的原节点号 | `vector<int>`，走环时按序 push | 0-based 槽位，沿环顺时针 |
| `cp[j]` | $c_j$ 沿环到起点 $c_0$ 的距离 | `i64`，`cp[0]=0`，累加边权 | "c umulated pos" |
| `hc[j]` | 挂树高度 $h_j$（=0 表示空树） | `i64`，`dfs` 返回 | 环节点自深 0 |
| `dm[j]` | 挂树直径 $d_j$（=0 空树） | `i64`，`dfs` 的引用出参 | 两最长子支和取 max |
| `S` | 环总长 | `i64`，走环闭合时 `cp[k-1]+w(last→st)` | 整数 |
| `deg[]` | 剥叶过程中的当前度 | 读入后 `graph[i].size()` | 剩 $\ge2$ 者在环上 |
| `incyc[]` | 是否环节点 | `fill_n` 后按 `deg>=2` 写 | dfs 里当"环边屏蔽板" |
| `M[j]` | $\max_{i\ne j}(h_i+dist_C(c_j,c_i))$ | `i64`，双单调队列 | $j$ 自己不算（自己树走 $d,h$） |
| `P2,H2` | 环复制两圈：$cp+(i\ge k)S$、$hc$ | `i64[2k]` | 半圈窗用 |
| `check(R)` | 环上是否存在被所有 $I_j$ 覆盖的点 | `bool` | 事件 + 端点引理 |
| `eps{1e-7}` | 仅 $r=R-hc_j$ 符号容差 | `constexpr double` | 窗宽比较纯整数 |

### 4.2 算法步骤

1. 读 $N$ 与 $N$ 条边（0-based）。
2. 拓扑剥叶得 `incyc`；从任一环节点出发走环一圈，定序 `cyc`、累 `cp`、闭合算 `S`（无重边 ⟹ 环上每点恰两个环邻居，走法唯一到回起点）。
3. 对每个环节点 `dfs`（遇 `incyc` 邻居跳过）：返回高度 `hc[j]`，引用带出直径 `dm[j]`（合并"过当前点的两最长支"）。
4. 加倍数组 `P2/H2`；两个单调队列分别扫"顺半圈窗 $\max(H2+P2)$"与"逆半圈窗 $\max(H2-P2)$"，合成 `M[j]`（引理 5）。
5. 浮点二分 60 次 `check(mid)`：弧端点事件排序扫描（引理 4+6），得上界 `r` $\approx R_{cycle}$。
6. `ans = r`；对每个 $j$ 取 $\min$ 进 $B_j=\max(dm_j,hc_j+M_j,2M_j)/2$（引理 3）。输出 `fixed<<setprecision(1)`。

**复杂度**：$O(N)$（步骤 2–4）$+\,60\cdot O(k\log k)$（步骤 5）$=O(N\log N\log V)$；最大项 $60\times2\cdot10^5\times18\approx2.2\times10^8$ 次比较（排序主导）。**提速注**：左端点循环序 $=(cp_j+hc_j)\bmod S$ 的循环平移、右端点 $=(cp_j-hc_j)\bmod S$ 同理（都只差一个全弧公共的 $-R$ 或 $+R$ 平移），预先各排一次即可把 `check` 降到 $O(k)$（两定序表按 R 旋转合并），总代价 $O(N\log N+60k)$；为保持代码直读，本解未实现。

**验证状态**：无编译器环境，全部结论来自 4.3 手算与引理证明，**未运行任何代码、未对拍**。

### 4.3 验证（全部手算；本地题面无数字样例 ⟹ 无法对标官方，只能自验）

- **例 A（纯环最小退化：单位三角形）** `3 / 1 2 1 / 2 3 1 / 3 1 1`。$k=3,S=3,h=d=0$，$M_j=1$（任一环节点偏心距 1），$B_j=\max(0,1,2)/2=1$。环侧：2.2 已逐段算过 $\min_{p\in C}\mathrm{ecc}=1$（顶点取到），即 $R_{cycle}=1$。与事件扫描对账：$R=1$ 时 $r_j=1$、$2r=2<S$；$I_1$（心 $cp_1=0$）：$l_1=2,b_1=1$ 跨剪点 ⟹ 初始 `cur=1`；$I_2$（心 1）：$l_2=0,b_2=2$ 不跨；$I_3$（心 2）：$l_3=1,b_3=0$ 跨 ⟹ `cur=2`。事件排序后首个为 start$(0)$：cur $2\to3=$ need ⟹ true，可行点 $t=0=c_1$（到三环节点距离 $\{0,1,1\}\le 1$ ✔）。$R=1-\epsilon$：$I_2=[\epsilon,2-\epsilon]$ 不含 $c_1$，逐一查三个左端点均被某弧排除 ⟹ false。答 $\min(1,1)=1.0$ ✔（$2M_j$ 项在此撑起 $B_j$，缺项则得 $0.5$ 错）。
- **例 B（纯环偶对称：单位正方形）** `4` 边全 1：$S=4$。$R_{cycle}$：中点 $t=0.5$：$\{0.5,0.5,1.5,1.5\}\to1.5$；顶点 $=2$ ⟹ $R_{cycle}=1.5$。$B_j=\max(0,2,4)/2=2$。答 $1.5$ ✔（验证"两路必须取 min"：环上更优）。
- **例 C（环上落点被否：三角形 + 链 100）**：$h_1=100,d_1=100,M_1=1$：$B_1=\max(100,101,2)/2=50.5$。直验：点沿链离 $c_1$ 距 $s$：$\mathrm{ecc}=\max(100-s,\ s+1)$，交点 $s=49.5$ 值 $50.5$ ✔；$R_{cycle}=100$（$r_1=R-100$ 必须 $\ge0$，且 $R=100$ 时 $I_1=\{c_1\}$、其余弧盖全环，$c_1$ 可行）取 min 后 $50.5$。答 $50.5$ ✔（打脸 2.2 错误分解）。
- **例 D（树内两臂平衡 + 空臂）**：三角形边 1；$c_1$–$u$：50，$c_1$–$v$：60：$d_1=110,h_1=60,M_1=1$：$B_1=\max(110,61,2)/2=55$。直验：$p$ 在 $c_1 v$ 臂离 $c_1$ 距 $s$：$\max(60-s,50+s,s+1)=\max(60-s,50+s)\ (s+1\le50+s$ 恒)，交点 $s=5$ 值 $55$ ✔。答 $\min(R_{cycle}=60,\,55,\,B_2,B_3)=55.0$。
- **例 E（双臂 + 双大树，$M$ 由对侧大树决定）**：三角形边 1；$c_1$–A 长 4；$c_2$–B 长 1000。$M_2=\max(4+1,0+1)=5$，$B_2=\max(1000,1005,10)/2=502.5$；$M_1=1001$，$B_1=\max(4,1005,2002)/2=1001$（$2M$ 项主导！）；$R_{cycle}$：$r_1=R-4,\ r_2=R-1000,\ r_3=R$：$R=1000$ 时 $r_2=0$ 点弧 $\{c_2\}$、其余全环 ⟹ 可行且不能再小（$r_1\ge0$ 要 $R\ge4$、点弧钉死 $c_2$）⟹ $R_{cycle}=1000$。直验 $B_2$：$p$ 沿 $c_2 B$ 离 $c_2$ 距 $s$：$\max(1000-s,\ s+5)=502.5$（$s=497.5$）：到 A $=497.5+1+4=502.5$ ✔。答 $502.5$ ✔。
- **反事实核对**：删掉 $2M_j$ 项 ⟹ 例 A 得 $\min(1, M_j/2)=0.5$ 错；删掉 $h_j+M_j$ 项 ⟹ 例 C 得 $\max(100,\dots)/2$ 仍对但例 D 型"s 不能太深"失守（$d/2$ 单独不约束 $s+M_j$ 与臂长和）；删掉 $d_j$ 项 ⟹ 例 D 得 $\max(61,2)/2=30.5$ 错（点贴 $c_1$ 时 $u,v$ 距离 110 压不住）⟹ 三项各自至少被一个手算例锁定。
- **单调队列小核对（引理 5）**：例 E 环 $cp=\{0,1,2\},S=3$，$hc=\{4,1000,0\}$：$j=0$：顺窗 $(0,1.5]\ni c_2$：$F^+=1000+1=1001$；逆窗 $[1.5,3)\ni c_3$：$F^-=0+(3-2)=1$；$M_0=1001$ ✔。$j=1$：顺窗 $(1,2.5]\ni c_3$：1；逆窗 $[2.5,4)$：含 $c_1$ 复制位 $3$：$4+ (4-3)=5$ ✔（$P2$ 里 $c_1$ 复制在 $0+3=3$，$H2-P2+P2_{jb}=4-3+4=5$ ✔）。


## 5. 示例代码

**代码导读**（对应 4.1/4.2；写码硬规则逐项落位）：

- 结构四块：拓扑剥叶找环（`deg` 队列）→ 走环定序（`cyc/cp/S`）→ 挂树 `dfs`（引用出参带直径，`incyc` 兼作"环边屏蔽板"）→ `M[j]` 双单调队列 + 二分 `check`。
- 整数量全程 `i64`（和 $\le10^{14}<2^{63}$，进 `f64` 仍精确），只有 `check` 的弧端点与最终 `/2.0` 是浮点；窗界比较写成 `2*P<=2*cp+S` 纯整数式，浮点歧义零。
- `check` 的事件 `pair<f64,int>`：同位置 start(0) 排在 end(1) 前——闭弧端点"双方都盖"，配合端点引理只需在 start 处查 `cur==need`；跨剪点弧先计 `cur`（它盖 0），其 end 事件照常产出。
- 全局静态数组 + `constexpr int mxn{100005}`（$+5$）；`fill_n(incyc,mxn,false)` 后再按 `deg>=2` 写真（剥叶阶段 `deg` 已把外树削成 $\le1$，无需重置）；`cin.tie/cout.tie/ios::sync_with_stdio` 固定顺序；`'\n'` 收尾；输出 `fixed<<setprecision(1)` 满足"恰好一位小数"。
- Tab 缩进、运算符无空格、小写电报命名（`hc/dm/cp/cyc/st`）。
- **未运行声明**：环境无编译器，按要求未编译未运行；正确性依据 = 第 2 部分引理 + 4.3 手算（含反事实）。已知风险如实列：递归 `dfs` 深度可达 $N=10^5$（2.8-4）；`check` 内 `sort` 使二分档总运算约 $2.2\times10^8$（4.2 给出 $O(k)$ 化路线未实施）；$>10^{12}$ 量级输出的末位小数依赖官方同为 double（2.7）。

```cpp
// P1399 [NOI2013] 快餐店
// 基环树半径：剥叶找环 + 挂树(高,直径) + 半圈单调队列求M_j + 弧交端点引理二分 + 三球交闭式取min
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using f64=double;

constexpr int mxn{100005};			//N<=1e5 +5
constexpr double eps{1e-7};			//仅r符号判定；窗界全整数比较

struct edge{int v;int w;};
int n,k;
vector<edge>graph[mxn];
int deg[mxn];
bool incyc[mxn];
vector<int>cyc;
i64 cp[mxn],hc[mxn],dm[mxn],M[mxn];
i64 P2[mxn*2],H2[mxn*2];
i64 S;

i64 dfs(int x,int f,i64&d)
{
	i64 res=0;
	for(auto e:graph[x])
	{
		if(e.v==f||incyc[e.v])
			continue;
		i64 d2{0};
		i64 h2=dfs(e.v,x,d2)+e.w;
		d=max(d,max(d2,res+h2));
		res=max(res,h2);
	}
	return res;
}

bool check(f64 R)
{
	vector<pair<f64,int>>ev;
	ev.reserve(2*k);
	int need=0,cur=0;
	for(int j=0;j<k;++j)
	{
		f64 r=R-(f64)hc[j];
		if(r<-eps)
			return false;
		if(r<0)
			r=0;
		if(2*r>=S)
			continue;
		++need;
		f64 l=fmod(cp[j]-r,S);
		if(l<0)
			l+=S;
		f64 b=l+2*r;
		if(b>=S)
		{
			b-=S;
			++cur;
		}
		ev.push_back({l,0});
		ev.push_back({b,1});
	}
	if(need==0)
		return true;
	sort(ev.begin(),ev.end());
	for(auto&t:ev)
	{
		if(!t.second)
		{
			++cur;
			if(cur==need)
				return true;
		}
		else
			--cur;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,u,v,w;i<n;++i)
	{
		cin>>u>>v>>w;--u;--v;
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}

	deque<int>Q;
	for(int i=0;i<n;++i)
	{
		deg[i]=graph[i].size();
		if(deg[i]<=1)
			Q.push_back(i);
	}
	while(!Q.empty())
	{
		int x=Q.front();Q.pop_front();
		for(auto e:graph[x])
			if(--deg[e.v]==1)
				Q.push_back(e.v);
	}
	fill_n(incyc,mxn,false);
	for(int i=0;i<n;++i)
		incyc[i]=deg[i]>=2;

	int st=0;
	while(!incyc[st])
		++st;
	cyc.push_back(st);
	cp[0]=0;k=1;
	int nx=-1,w=0;
	for(auto e:graph[st])
		if(incyc[e.v]){nx=e.v;w=e.w;break;}
	int pre=st,u=nx;
	while(u!=st)
	{
		cyc.push_back(u);
		cp[k]=cp[k-1]+w;++k;
		nx=-1;w=0;
		for(auto e:graph[u])
			if(incyc[e.v]&&e.v!=pre){nx=e.v;w=e.w;break;}
		if(nx==st)
			S=cp[k-1]+w;
		pre=u;u=nx;
	}

	for(int j=0;j<k;++j)
	{
		i64 d{0};
		hc[j]=dfs(cyc[j],cyc[j],d);
		dm[j]=d;
	}

	for(int i=0;i<2*k;++i)
	{
		P2[i]=cp[i%k]+(i>=k?S:0);
		H2[i]=hc[i%k];
	}
	deque<int>q1,q2;
	int r1=0,r2=0;
	for(int j=0;j<k;++j)
	{
		while(r1<2*k&&2*P2[r1]<=2*P2[j]+S)
		{
			while(!q1.empty()&&H2[q1.back()]+P2[q1.back()]<=H2[r1]+P2[r1])
				q1.pop_back();
			q1.push_back(r1++);
		}
		while(!q1.empty()&&P2[q1.front()]<=P2[j])
			q1.pop_front();
		i64 cw=q1.empty()?0:H2[q1.front()]+P2[q1.front()]-P2[j];
		i64 jb=j+k;
		while(r2<2*k&&2*P2[r2]<2*P2[jb])
		{
			while(!q2.empty()&&H2[q2.back()]-P2[q2.back()]<=H2[r2]-P2[r2])
				q2.pop_back();
			q2.push_back(r2++);
		}
		while(!q2.empty()&&2*P2[q2.front()]<2*P2[jb]-S)
			q2.pop_front();
		i64 ccw=q2.empty()?0:H2[q2.front()]-P2[q2.front()]+P2[jb];
		M[j]=max(cw,ccw);
	}

	f64 l=0,r=1e15;
	for(int it=0;it<60;++it)
	{
		f64 mid=(l+r)/2;
		if(check(mid))
			r=mid;
		else
			l=mid;
	}
	f64 ans=r;
	for(int j=0;j<k;++j)
		ans=min(ans,max({dm[j],hc[j]+M[j],2*M[j]})/2.0);

	cout<<fixed<<setprecision(1)<<ans<<'\n';
	return 0;
}
```
