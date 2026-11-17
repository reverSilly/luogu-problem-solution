# P2985 [USACO10FEB] Chocolate Eating S — 题解

## 一、解题过程：人类思考叙事

**题意重述。** 有 $N$ 块巧克力（$N\le 5\times10^4$），第 $i$ 块带来幸福值 $H_i$（$1\le H_i\le 10^6$），**必须按输入顺序吃**。要在 $D$ 天（$D\le 5\times10^4$）内把它们全部吃完（输出要求给每块巧克力指定一个 $1\sim D$ 的日期）。幸福值是整数，初始 $0$；每天可以吃若干块（当天吃的必须是尚未吃的最前面若干块），**当天计入的幸福值 = 吃完后的值**；每晚睡觉幸福值减半下取整。目标：最大化 $D$ 天中**每天幸福值的最小值**，输出这个最大值和一种吃法。

**口径说明（两处）。**

1. "当天幸福值"取**吃完后、睡前减半前**的值。依据是题面例子：第 $5$ 天吃完后 $24$，睡前减半是 $12$，而例子说"最低睡前幸福值是 $24$"——可见计入的是减半前的 $24$。这与"当天的幸福值以吃完巧克力后的值为准"一句一致。
2. 所有巧克力必须在 $D$ 天内吃完：输出格式要求每块都有一个日期。$N$ 与 $D$ 没有大小关系约束，$D>N$ 时会有整天不吃巧克力的日子。

**第一步：把过程写成递推，先别想算法。** 设第 $d$ 天起床时幸福值为 $c_{d-1}$（$c_0=0$），当天吃的巧克力幸福值之和为 $E_d$，则

$$v_d=c_{d-1}+E_d\quad(\text{当天计入值}),\qquad c_d=\left\lfloor\frac{v_d}{2}\right\rfloor\quad(\text{次日起床值}).$$

一个"吃法"就是把序列 $H_1..H_N$ 切成 $D$ 个**连续的段**（允许空段），第 $d$ 段在第 $d$ 天吃。目标是 $\max\min_d v_d$。

**第二步：最优化太难，先问判定。** "最大化最小值"是二分答案的标准信号。把问题改成判定：**给定 $X$，能否让每天 $v_d\ge X$？** 若 $X$ 可行则任何 $X'<X$ 也可行（同一套吃法），可行性单调，二分合法。上界取 $\sum H_i\le 5\times10^{10}$——**超过 `int`，全程 `long long`**，这是第一个硬坑。

**第三步：判定怎么贪心？** 直觉："能不吃就不吃，不够 $X$ 才吃，而且只吃到刚好够 $X$ 为止。"为什么晚吃更好？因为**每过一夜幸福值减半**：同一块巧克力，早吃一天，它的贡献就被多砍一半。把巧克力尽量往后拖，等于让每块钱"少挨几刀"。

但直觉马上遇到两个反问：

- **反问 1**：晚吃会让中间某些天的值变小，会不会把前面某天拖破 $X$？——不会，因为贪心的规则就是"拖到当天值 $<X$ 才吃"，任何一天只要不跌破 $X$ 才允许继续拖。
- **反问 2**：贪心每天只吃到刚好够 $X$，手里留的巧克力最多，但**当前幸福值可能比别的策略低**（别人第一天全吃了，第二天起床值很高）。会不会出现"贪心后面某天不够吃、别的策略却够"？这是全题真正需要证明的地方：**"剩得多"能不能压倒"值更低"？** Part 2 引理 6 用一个精确的不等式回答：贪心起床值 $a$ 加上"对手已吃而贪心未吃"那些巧克力的**全额** $H_j$，一定 $\ge$ 对手起床值 $b$。因为对手那些巧克力是早吃的，被减半砍过，全额补回来只多不少。于是对手能靠吃后续巧克力凑够 $X$，贪心更能。

**第四步：扫尾细节。** 最后一天要把剩余巧克力**全部**吃完（题目要求全部吃完）。这只会让 $v_D$ 更大，不破坏任何约束——所以"能不吃就不吃"只在第 $1\sim D-1$ 天适用，第 $D$ 天判定完 $\ge X$ 后无脑扫尾。若中途巧克力吃完仍 $<X$，判定失败。

**第五步：二分框架与输出。** 在 $[0,\sum H_i+1)$ 上二分最大可行 $X$（$X=0$ 恒可行：全留到最后一天吃）。找到后再跑一次贪心并记录每块巧克力的日期输出。题目说多解输出任意一个，贪心给出的"最晚吃法"就是合法解。

**用题面例子核对。** $N=D=5$，$H=(10,40,13,22,7)$：

- $X=24$：第 $1$ 天 $0<24$，吃 $10\to10$ 仍 $<24$，吃 $40\to50\ge24$；夜 $25$。第 $2$ 天 $25\ge24$ 不吃；夜 $12$。第 $3$ 天吃 $13\to25$；夜 $12$。第 $4$ 天吃 $22\to34$；夜 $17$。第 $5$ 天吃 $7\to24\ge24$ ✓。日期 $(1,1,3,4,5)$，与题面给出的最优安排完全一致。
- $X=25$：前四天同上，第 $5$ 天起床 $17$，吃 $7\to24<25$ 且无巧克力可吃，失败。

故答案 $24$ ✓。再造两组边界：$N=3,D=1,H=(5,7,9)$：只有一天，全吃，答案 $21$，日期全 $1$。$N=1,D=2,H=(1)$：第 $1$ 天吃则第 $2$ 天起床 $\lfloor1/2\rfloor=0$ 且无巧克力，$X=1$ 不可行；答案 $0$，日期 $2$（留到最后天吃，第 $1$ 天 $v_1=0$ 也 $\ge0$）。程序输出均与手算一致。

---

## 二、思维链：完整证明

**记号。** $H_1..H_N\ge 1$，天数 $D$。一个**日程** $S$ 是把 $\{1..N\}$ 切成 $D$ 个连续段（可空）：第 $d$ 天吃集合 $B_d$，$\bigcup B_d=\{1..N\}$，段按序。$E_d=\sum_{j\in B_d}H_j$。递推 $c_0=0$，$v_d=c_{d-1}+E_d$，$c_d=\lfloor v_d/2\rfloor$。记 $t_j(S)$ 为巧克力 $j$ 被吃的日期，$s_d(S)=\sum_{d'\le d}|B_{d'}|$ 为前 $d$ 天末已吃块数（指针）。$\mathrm{feasible}(X)\iff\exists S,\forall d:v_d(S)\ge X$。

---

### 引理 1（模型与目标等价）

$\max_S\min_d v_d(S)=\max\{X\in\mathbb Z_{\ge0}:\mathrm{feasible}(X)\}$。

*证明*：设左端为 $A$，在 $S^\*$ 处取到。则 $\forall d,v_d(S^\*)\ge A$，故 $\mathrm{feasible}(A)$ 成立，右端 $\ge A$。反之若 $\mathrm{feasible}(X)$，存在 $S$ 使 $\min_d v_d(S)\ge X$，故 $A\ge X$，右端 $\le A$。两端相等。证毕。

---

### 引理 2（可行性单调）

若 $\mathrm{feasible}(X)$ 且 $0\le X'\le X$，则 $\mathrm{feasible}(X')$。

*证明*：同一日程 $S$ 满足 $v_d\ge X\ge X'$ 对所有 $d$。证毕。

**推论**：$\mathrm{feasible}$ 是 $[0,+\infty)$ 上的单调谓词，可二分最大可行点。

---

### 引理 3（下取整的两条代数性质）

对整数 $x\ge0$、整数 $H\ge1$、实数 $u,v$：

(a) $\lfloor u+v\rfloor\ge\lfloor u\rfloor+\lfloor v\rfloor$（超可加）；
(b) $\left\lfloor\frac{x+H}{2}\right\rfloor\le\left\lfloor\frac{x}{2}\right\rfloor+\left\lceil\frac{H}{2}\right\rceil$；
(c) 若整数 $u\ge v\ge0$，则 $\lfloor u/2\rfloor-\lfloor v/2\rfloor\le u-v$，且 $\lfloor u/2\rfloor\ge\lfloor v/2\rfloor$。

*证明*：(a) 设 $u=\lfloor u\rfloor+\{u\}$，$v=\lfloor v\rfloor+\{v\}$，则 $\lfloor u+v\rfloor=\lfloor u\rfloor+\lfloor v\rfloor+\lfloor\{u\}+\{v\}\rfloor\ge\lfloor u\rfloor+\lfloor v\rfloor$。

(b) 写 $x=2q+r$，$r\in\{0,1\}$。左 $=q+\lfloor(r+H)/2\rfloor$，右 $=q+\lceil H/2\rceil$。$\lfloor(r+H)/2\rfloor\le\lfloor(1+H)/2\rfloor=\lceil H/2\rceil$ ✓。

(c) 单调性显然。差：$\lfloor u/2\rfloor-\lfloor v/2\rfloor\le\frac{u}{2}-\frac{v-1}{2}=\frac{u-v+1}{2}\le u-v$ 当 $u-v\ge1$；$u=v$ 时差 $0$。证毕。

---

### 引理 4（推迟引理：晚吃不让后续起床值变差）

设日程 $S$ 在日期 $t$ 吃巧克力 $j$。把 $j$ 移到日期 $t+1$（其余不变，$t+1\le D$）得 $S'$。则：

(a) 对 $d<t$：$c_d(S')=c_d(S)$；
(b) $c_t(S')\ge c_t(S)-\lceil H_j/2\rceil$；
(c) 对一切 $d\ge t+1$：$c_d(S')\ge c_d(S)$。

*证明*：(a) 前 $t-1$ 天两日程逐日相同，递推确定性给出相同 $c_d$。

(b) 第 $t$ 天：$c_t(S)=\lfloor(c_{t-1}+E_t+H_j)/2\rfloor$，$c_t(S')=\lfloor(c_{t-1}+E_t)/2\rfloor$。由引理 3(b)（取 $x=c_{t-1}+E_t$）即得。

(c) 先证 $d=t+1$：
$$c_{t+1}(S')=\left\lfloor\frac{c_t(S')+E_{t+1}+H_j}{2}\right\rfloor\ge\left\lfloor\frac{c_t(S)-\lceil H_j/2\rceil+E_{t+1}+H_j}{2}\right\rfloor$$
$$\ge\left\lfloor\frac{c_t(S)+E_{t+1}}{2}\right\rfloor+\left\lfloor\frac{H_j-\lceil H_j/2\rceil}{2}\right\rfloor=c_{t+1}(S)+\left\lfloor\frac{\lfloor H_j/2\rfloor}{2}\right\rfloor\ge c_{t+1}(S),$$
第二个不等号用引理 3(a)（把分子拆成两项），且 $H_j-\lceil H_j/2\rceil=\lfloor H_j/2\rfloor\ge0$。

再对 $d$ 归纳：$d\ge t+2$ 时两日程当天吃的集合相同（$E_d$ 相同），递推 $c_d=\lfloor(c_{d-1}+E_d)/2\rfloor$ 关于 $c_{d-1}$ 单调（引理 3(c)），由归纳假设 $c_{d-1}(S')\ge c_{d-1}(S)$ 即得。证毕。

**推论（多次推迟）**：把若干巧克力各自移到**更晚**的日期（逐块、每次移一天地做），则对任何 $d$ 满足"所有移动后的新日期 $\le d$"，$c_d$ 不减。因为每一步移动要么不影响 $c_d$（移动发生在 $d$ 之后），要么由 (c) 使 $c_d$ 不减。

---

### 引理 5（增量上界：多吃一块，起床值至多多 $H$）

设日程 $S$ 比 $S_0$ 多吃若干块巧克力（其余块的日期相同），多吃集合为 $A$，且 $A$ 中块都在 $\le d$ 的日期吃。则 $c_d(S)\le c_d(S_0)+\sum_{j\in A}H_j$。

*证明*：逐块添加，只需证加一块 $H$ 使任何 $c_{d'}$（$d'\ge$ 吃的日期 $t$）增量 $\le H$。设 $\delta_{d'}=c_{d'}(S)-c_{d'}(S_0)$。第 $t$ 天：$\delta_t=\lfloor(x+H)/2\rfloor-\lfloor x/2\rfloor\in[0,\lceil H/2\rceil]$（引理 3(b)(c)），故 $0\le\delta_t\le H$。此后各天 $E$ 相同，$\delta_{d'+1}=\lfloor(u+\delta_{d'})/2\rfloor-\lfloor u/2\rfloor\le\delta_{d'}$（引理 3(c)），且 $\ge0$。故 $\delta$ 不增，恒 $\le H$。多块累加即得。证毕。

---

### 引理 6（贪心支配：贪心指针永远不落后）

定义贪心日程 $G$：第 $d$ 天，若当前起床值 $<X$ 则按序吃，直到 $\ge X$ 或吃完；第 $D$ 天把剩余全部吃完。设 $S$ 是任一满足 $\forall d:v_d(S)\ge X$ 的日程。若贪心在前 $d-1$ 天没有失败，则对每个 $d\le D$，前 $d$ 天末指针满足 $g_d\le s_d(S)$，且贪心第 $d$ 天不会失败。

*证明*：对 $d$ 归纳。$d=0$：$g_0=s_0=0$，$c_0$ 均为 $0$。

设前 $d-1$ 天末 $g:=g_{d-1}\le s:=s_{d-1}(S)$。记贪心第 $d$ 天起床值 $a=c_{d-1}(G)$，$S$ 的 $b=c_{d-1}(S)$。

**关键不等式**：$a+\sum_{j=g+1}^{s}H_j\ge b$。

证：构造辅助日程 $S_0$：只吃 $1..g$，每块日期与 $S$ 相同。由归纳假设，对每块 $j\le g$，贪心吃它的日期 $\ge$ $S$ 吃它的日期（因为每天末贪心指针 $\le S$ 指针，第 $j$ 块被贪心吃到的那天，$S$ 早已吃过）。把 $S_0$ 的每块逐个推迟到贪心的日期（都 $\le d-1$），由引理 4 推论，起床值不减，终态恰是贪心的前 $d-1$ 天过程，故 $a\ge c_{d-1}(S_0)$。另一方面 $S$ 比 $S_0$ 多吃 $g+1..s$（都在 $\le d-1$ 天吃），由引理 5，$b\le c_{d-1}(S_0)+\sum_{j=g+1}^{s}H_j\le a+\sum_{j=g+1}^{s}H_j$。关键不等式成立。

**分情形推指针**（按贪心起床值 $a$ 分支）：

- 若 $a\ge X$：贪心当天一块不吃，$v_d(G)=a\ge X$ ✓，$g_d=g\le s\le s_d(S)$ ✓。
- 若 $a<X$（贪心需要吃）：记 $s':=s_d(S)$。$S$ 当天满足 $b+\sum_{j=s+1}^{s'}H_j=v_d(S)\ge X$（$S$ 当天不吃时 $s'=s$，和式为空，即 $b\ge X$）。于是
$$a+\sum_{j=g+1}^{s'}H_j=\Big(a+\sum_{j=g+1}^{s}H_j\Big)+\sum_{j=s+1}^{s'}H_j\ge b+\sum_{j=s+1}^{s'}H_j\ge X.$$
贪心从 $g+1$ 起逐块吃、一到 $\ge X$ 就停，而吃到 $s'$ 时累计值已 $\ge X$（上式），且 $s'\le N$ 块都存在，故贪心必在指针 $\le s'$ 处停下：$g_d\le s'=s_d(S)$ ✓，当天 $v_d(G)\ge X$ ✓，不会失败。

归纳完成。特别地 $g_D\le s_D(S)=N$，贪心全程不失败。证毕。

**这条引理回答了第一步思考中的反问 2**：贪心虽然起床值可能更低，但它手里未吃的巧克力是对手的超集，且"全额补回"足以覆盖对手早吃被减半砍掉的损失——"剩得多"严格压倒"值更低"。

---

### 引理 7（check 的正确性）

`check(X)` 返回真 $\iff\mathrm{feasible}(X)$。

*证明*：**充分性**：返回真时，贪心每天 $v_d\ge X$（每天要么起床已 $\ge X$，要么吃到 $\ge X$ 才进入下一天；第 $D$ 天扫尾只增不减），且第 $D$ 天把剩余全吃完，所有 $N$ 块都有日期，是合法日程，故 $\mathrm{feasible}(X)$。

**必要性**：若 $\mathrm{feasible}(X)$，取见证日程 $S$。由引理 6，贪心每天都能凑够 $X$ 不会触发 `return false`；第 $D$ 天扫尾后返回真。证毕。

---

### 引理 8（二分边界与答案）

$X=0$ 恒可行；$X>\sum H_i$ 恒不可行。故最大可行 $X\in[0,\sum H_i]$，二分不变量"$l$ 可行、$r$ 不可行"以 $l=0,r=\sum H_i+1$ 初始化合法，终止时 $l$ 即答案。

*证明*：$X=0$：任何日程 $v_d\ge0$（$H_i\ge1$、$c\ge0$），取"全留第 $D$ 天"即合法。$X>\sum H_i$：对任何日程，归纳可证 $c_d\le\sum_{j\text{ 已吃}}H_j\le\sum H_i$（$c_d=\lfloor(c_{d-1}+E_d)/2\rfloor\le c_{d-1}+E_d$，累加即得），故 $v_d=c_{d-1}+E_d\le\sum H_i<X$，第 $1$ 天就不可行。二分每轮区间减半，$\lceil\log_2(\sum H_i+1)\rceil\le 36$ 轮终止，$l+1=r$ 时 $l$ 是最大可行点。由引理 1，这就是 $\max\min v_d$。最后 `check(l,true)` 记录的日期构成达到 $\min v_d\ge l$ 的合法日程（引理 7），而 $\min v_d$ 不可能 $>l$（否则 $l+1$ 可行），故该日程最优。证毕。

---

### 引理 9（64 位必要性与复杂度）

$\sum H_i\le 5\times10^4\times10^6=5\times10^{10}>2^{31}-1$，幸福值与二分边界必须 `long long`。时间 $O((N+D)\log\sum H_i)$，空间 $O(N)$。

*证明*：范围直接代入。每轮 `check`：外层 $D$ 天，内层 while 总共至多吃 $N$ 块（指针单调），故单轮 $O(N+D)$；二分 $\le 36$ 轮加最后一次记录，合计 $O(37(N+D))\approx 3.7\times10^6$ 次基本操作。空间：`h`、`day` 各 $O(N)$。证毕。

---

### 定理

程序输出 $\max_S\min_d v_d$ 及一组达到该值的最优吃法，总复杂度 $O((N+D)\log\sum H_i)$。

*证明*：引理 1 把最优化归约为判定；引理 2 保证判定单调可二分；引理 3–5 是地板函数的代数工具；引理 6 证明贪心指针支配任何可行日程，从而引理 7 得到 check 与可行性等价；引理 8 保证二分边界与答案、以及最终日程的最优性；引理 9 给出数值安全与复杂度。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **explore its property** — 把"吃 / 减半"写成递推 $v_d=c_{d-1}+E_d,\ c_d=\lfloor v_d/2\rfloor$；日程 = 序列切 $D$ 个连续段。
2. **consider easier version** — 最优化改判定：$\mathrm{feasible}(X)$ = 能否每天 $\ge X$；单调性显然，二分答案。
3. **classic problem solution** — 二分最大可行 $X\in[0,\sum H]$，$36$ 轮。
4. **explore its property** — 减半机制 ⇒ 晚吃少挨刀 ⇒ 贪心"不够 $X$ 才按序吃"；最后一天扫尾吃完。
5. **explore its property** — 贪心起床值可能更低，但"未吃集合是超集 + 全额 $H_j$ 补偿 $\ge$ 对手被减半的残值"，指针支配成立（推迟引理 + 增量上界，均需处理下取整）。
6. **discuss by category** — 第 $D$ 天剩余必须全吃（只增不减）；中途吃完仍 $<X$ 判失败；$X=0$ 恒可行兜底。
7. **explore its property** — $\sum H\le 5\times10^{10}$ 超 `int`，幸福值 / 二分边界全程 `i64`。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 | 初值 / 记忆点 |
|------|------|------|---------------|
| $N,D$ | 巧克力数 / 天数 | `int n,d` | 读入顺序 $N,D$ |
| `h[i]` | 第 $i$ 块的幸福值 $H_{i+1}$ | `int[mxn]` | 读入，0-based |
| `sum` | $\sum H_i$，二分上界 | `i64` | 读入时累加，$\le 5\times10^{10}$ |
| `day[i]` | 第 $i$ 块被吃的日期（$1\sim D$） | `int[mxn]` | 仅在 `check(l,true)` 时写入 |
| `cur` | 当前幸福值（起床值 / 吃后值） | `i64` | 每轮 check 从 $0$ 开始 |
| `ptr` | 已吃块数指针 | `int` | 每轮 check 从 $0$ 开始，单调不减 |
| `l,r` | 二分边界：$l$ 可行、$r$ 不可行 | `i64` | $l=0,\ r=\texttt{sum}+1$ |
| `check(x,rec)` | 贪心判定 $X=x$；`rec` 为真时记录日期 | `bool(i64,bool)` | 白天不够才吃，第 $D$ 天扫尾 |

### 算法步骤

1. I/O 加速三行；读 $N,D$ 与 `h`，累加 `sum`。
2. 二分：$l=0,r=\texttt{sum}+1$；`mid=(l+r)>>1`，`check(mid,false)` 真则 $l=\texttt{mid}$ 否则 $r=\texttt{mid}$，直到 $l+1=r$。
3. `check(l,true)` 重跑贪心并记录 `day`。
4. 输出 $l$，再逐块输出 `day[i]`。
5. `check` 内部：对每天 $t=1..D$：while `cur<x && ptr<n` 吃下一块；若 `cur<x` 返回假；若 $t=D$ 把剩余全吃；`cur/=2` 过夜。

### 复杂度

- 时间：$O((N+D)\log\sum H_i)\approx 37\times10^5$ 次基本操作。
- 空间：$O(N)$。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；题面 $5$ 巧克力例子输出 $24$ 与日期 $(1,1,3,4,5)$ 逐条吻合；另造 $D=1$（答案 $=\sum H$）、$N=1,D=2$（答案 $0$、日期 $2$）、$N=2,D=5,H=(100,1)$（答案 $7$、日期 $(1,5)$，手算验证 $X=8$ 不可行）全部一致。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2985 [USACO10FEB] Chocolate Eating S
// 二分最低幸福值 X；贪心判定：每天起床幸福值不够 X 才按顺序吃，最后一天扫尾吃完
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{50005};

int n,d,h[mxn],day[mxn];
i64 sum;

bool check(i64 x,bool rec)
{
	i64 cur=0;	// 起床时幸福值
	int ptr=0;
	for(int t=1;t<=d;++t)
	{
		while(cur<x&&ptr<n)
		{
			cur+=h[ptr];
			if(rec)day[ptr]=t;
			++ptr;
		}
		if(cur<x)return false;	// 巧克力吃完仍不够 X
		if(t==d)
			while(ptr<n)	// 剩余的必须吃完，全塞最后一天只会更优
			{
				if(rec)day[ptr]=t;
				cur+=h[ptr++];
			}
		cur/=2;	// 睡觉减半下取整
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>d;
	for(int i=0;i<n;++i)
		cin>>h[i],sum+=h[i];
	i64 l=0,r=sum+1;	// 不变量：l 可行，r 不可行；sum 可达 5e10 必须 i64
	while(l+1<r)
	{
		i64 mid=(l+r)>>1;
		if(check(mid,false))l=mid;
		else r=mid;
	}
	check(l,true);
	cout<<l<<'\n';
	for(int i=0;i<n;++i)
		cout<<day[i]<<'\n';
	return 0;
}
```

### 代码说明

- `cur` 语义随位置切换：天初是起床值，吃完是当天计入值，`/=2` 后变次日起床值——与递推 $v_d=c_{d-1}+E_d,\ c_d=\lfloor v_d/2\rfloor$ 一一对应。
- **`i64` 三处缺一不可**：`sum`、`cur`、二分边界 `l/r/mid`。$\sum H$ 可达 $5\times10^{10}$，`int` 会静默溢出把二分上界变成负数。
- 扫尾 while 放在 `if(cur<x)return false;` **之后**：先确认当天达标，再把剩余塞进第 $D$ 天；顺序反了会把"吃完仍不够"的失败情形误判。
- `rec` 开关让二分阶段不写 `day`（写了也无妨，但省一次分支赋值）；最终 `check(l,true)` 才落盘日期。
- 二分写法是"左可行右不可行"的开区间版：`l=0` 恒可行兜底，`r=sum+1` 恒不可行（引理 8），终止 `l+1==r` 无需特判。
- 题目允许输出任意最优解；贪心给出的是"每块尽量晚吃"的那一个。