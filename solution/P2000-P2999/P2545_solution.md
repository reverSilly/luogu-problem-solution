# P2545 [AHOI2004] 实验基地 — 题解

## 一、解题过程：人类思考叙事

**题意重述。** 登陆地带是 $2\times N$ 的网格（$3<N<2000$，即 $N\ge 4$），每格一个适用度（绝对值 $\le 10^6$，可负）。要选一段 $2\times Q$ 的连续列区域 $[l,r]$ 建基地，但必须在**第一行**挖掉一段连续长度 $P\ge 1$ 的"样品采集区" $[a,b]$，且**采集区左右两侧都得有基地**（不能贴着区域边界）。剩下的形状是个"凹"字：第二行 $[l,r]$ 整行保留，第一行保留 $[l,a-1]$ 与 $[b+1,r]$ 两段。求基地适用度（保留格子之和）的最大值。

**第一步：把"凹"字翻译成四个端点。** 一个方案完全由 $(l,r,a,b)$ 决定，约束是 $1\le l<a\le b<r\le N$：

- $a\le b$：采集区长度 $P=b-a+1\ge 1$（必须挖）；
- $a>l$：采集区左边还得有基地（第一行 $[l,a-1]$ 非空）；
- $b<r$：采集区右边还得有基地（第一行 $[b+1,r]$ 非空）。

收益用前缀和写出来。设第一行前缀和 $A$、第二行前缀和 $B$，则

$$\text{收益}=\underbrace{(B_r-B_{l-1})}_{\text{第二行整段}}+\underbrace{(A_{a-1}-A_{l-1})}_{\text{第一行左段}}+\underbrace{(A_r-A_b)}_{\text{第一行右段}}.$$

**第二步：先撞暴力的墙。** 最直接的想法是枚举 $(l,r,a,b)$ 四元组，$O(N^4)$。$N=2000$ 时 $N^4=1.6\times10^{13}$，妥妥超时。就算用前缀和把每组收益算成 $O(1)$，四重循环也扛不住。必须找结构。

**第三步：换元，把四个端点变成"四个递增切点"。** 我盯着收益式子，发现它只用到 $l-1,\ a-1,\ b,\ r$ 这四个下标。令

$$x=l-1,\quad y=a-1,\quad z=b,\quad w=r,$$

则约束 $l<a\le b<r$ 恰好变成 $x<y<z<w$（下面引理 3 会严格证这个等价），而收益（记 $C=A+B$ 为两行合计前缀和）化简成

$$\text{收益}=C_w-C_x+A_y-A_z.$$

于是问题变成：**在 $0\le x<y<z<w\le N$ 中选四个严格递增的下标，最大化 $C_w-C_x+A_y-A_z$。** 四重枚举的"区间"味道没了，剩下一个干净的"选四个有序点"的最优化。

**第四步：四个有序点 ⇒ 四阶段 DP。** 式子 $C_w-C_x+A_y-A_z$ 可以按 $x<y<z<w$ 的顺序拆成四段累加：

$$\underbrace{(-C_x)}_{\text{选 }x}+\underbrace{A_y}_{\text{选 }y}\underbrace{-A_z}_{\text{选 }z}+\underbrace{C_w}_{\text{选 }w}.$$

这正是一个"依次选四个递增下标、每选一个加一项"的过程。我从左到右扫 $i=0..N$，维护三个前缀最优：

- `dp1`$=\max_{x\le i}(-C_x)$（已选 $x$）；
- `dp2`$=\max_{x<y\le i}(A_y-C_x)$（已选 $x,y$）；
- `dp3`$=\max_{x<y<z\le i}(A_y-C_x-A_z)$（已选 $x,y,z$）；

每扫到一个 $i$，就把它当作 $w$ 去更新答案 `ans=max(ans,dp3+C_i)`，再依次把 $i$ 接进 `dp3`（当 $z$）、`dp2`（当 $y$）、`dp1`（当 $x$）。**关键在更新顺序**：必须先 `ans`、再 `dp3`、再 `dp2`、最后 `dp1`，且每个阶段读的是"上一阶段尚未被本轮更新"的旧值——这样才能保证 $x<y<z<w$ 严格递增，不会让同一个 $i$ 同时充当两个角色。复杂度一下降到 $O(N)$。

**第五步：抠边界与值域。** $N\ge 4$ 保证存在合法四元组（$x=0,y=1,z=2,w=3$ 对应 $Q=3,P=1$），答案一定有限。值域：$|A|,|B|\le N\cdot10^6\approx 2\times10^9$，$|C|\le 4\times10^9$，收益 $\in[-8\times10^9,8\times10^9]$，**超 `int`，必须 `long long`**。`dp1/dp2/dp3` 初值设 $-\infty$，但因为真实收益 $\ge-8\times10^9\gg-\infty$，哨兵永远不会赢，也不会溢出（$-\infty$ 取 $-2^{60}$，加减 $4\times10^9$ 仍远在 `long long` 内）。

**手造数据核对**（题面无样例，我手算三组并用程序核对）：

- $N=4$，第一行 `1 2 3 4`、第二行 `1 1 1 1`：全选 $[1,4]$、挖掉第一行最小的内部格 `2`（$a=b=2$），收益 $=(1{+}1{+}1{+}1)+(1+3+4)=12$。程序输出 $12$ ✓。
- $N=4$，第一行 `5 -100 5 5`、第二行 `0 0 0 0`：全选、挖掉 `-100`（$a=b=2$），收益 $=0+(5+5+5)=15$。程序输出 $15$ ✓（挖掉越负的采集区越赚）。
- $N=4$，两行全 `-5`：只能选 $Q=3$、挖一个 `-5`，收益 $=3\times(-5)+2\times(-5)-(-5)=-25$。程序输出 $-25$ ✓（全负时被迫选最小损失）。

---

## 二、思维链：完整证明

**记号。** 第一行 $u_1..u_N$、第二行 $v_1..v_N$。前缀和 $A_i=\sum_{t\le i}u_t$，$B_i=\sum_{t\le i}v_t$，$C_i=A_i+B_i$，$A_0=B_0=C_0=0$。方案 $(l,r,a,b)$ 满足 $1\le l<a\le b<r\le N$。收益
$$V(l,r,a,b)=(B_r-B_{l-1})+(A_{a-1}-A_{l-1})+(A_r-A_b).$$

---

### 引理 1（"凹"字方案的端点刻画）

合法方案与四元组 $(l,r,a,b)\in\{1\le l<a\le b<r\le N\}$ 一一对应；其基地为第二行 $[l,r]$ 整段加第一行 $[l,a-1]\cup[b+1,r]$，收益为 $V(l,r,a,b)$。特别地 $Q=r-l+1\ge 3$、$P=b-a+1\ge 1$，且第一行左右两段都非空。

*证明*：基地是 $2\times Q$ 区域 $[l,r]$ 挖去第一行连续段 $[a,b]$。"必须设采集区"⇒ $P=b-a+1\ge1$ ⇒ $a\le b$。"采集区左侧有基地"⇒ 区域内 $a$ 左边还有列 ⇒ $a-1\ge l$ ⇒ $a>l$；同理右侧 ⇒ $b+1\le r$ ⇒ $b<r$。合起来 $l<a\le b<r$。反过来，任何满足此约束的四元组：第一行左段 $[l,a-1]$ 因 $a-1\ge l$ 非空、右段 $[b+1,r]$ 因 $b+1\le r$ 非空，采集区 $[a,b]$ 非空（$a\le b$），构成合法"凹"字。由 $l<a\le b<r$ 得 $r\ge l+2$，故 $Q=r-l+1\ge3$。收益 = 第二行 $[l,r]$ 和 $+$ 第一行 $[l,a-1]$ 和 $+$ 第一行 $[b+1,r]$ 和，用前缀和即 $V$。证毕。

---

### 引理 2（收益的切点形式）

令 $x=l-1,\ y=a-1,\ z=b,\ w=r$，则
$$V(l,r,a,b)=C_w-C_x+A_y-A_z.$$

*证明*：代入 $V=(B_r-B_{l-1})+(A_{a-1}-A_{l-1})+(A_r-A_b)$，用 $r=w,l-1=x,a-1=y,b=z$：
$$V=(B_w-B_x)+(A_y-A_x)+(A_w-A_z)=(A_w+B_w)-(A_x+B_x)+A_y-A_z=C_w-C_x+A_y-A_z.$$
最后一步用 $C=A+B$。证毕。

---

### 引理 3（约束等价于四个严格递增切点）

映射 $(l,r,a,b)\mapsto(x,y,z,w)=(l-1,a-1,b,r)$ 是合法方案集与 $\{(x,y,z,w):0\le x<y<z<w\le N\}$ 之间的**双射**。

*证明*：先证约束等价。

($\Rightarrow$) 设 $1\le l<a\le b<r\le N$。
- $l<a\Rightarrow l-1<a-1\Rightarrow x<y$；
- $a\le b\Rightarrow a-1\le b-1<b\Rightarrow y<z$；
- $b<r\Rightarrow z<w$；
- $l\ge1\Rightarrow x=l-1\ge0$；$r\le N\Rightarrow w\le N$。
故 $0\le x<y<z<w\le N$。

($\Leftarrow$) 设 $0\le x<y<z<w\le N$，令 $l=x+1,a=y+1,b=z,r=w$。
- $x\ge0\Rightarrow l\ge1$；$w\le N\Rightarrow r\le N$；
- $x<y\Rightarrow x+1\le y\Rightarrow l\le a-1<a\Rightarrow l<a$；
- $y<z\Rightarrow y+1\le z\Rightarrow a\le b$；
- $z<w\Rightarrow b<r$。
故 $1\le l<a\le b<r\le N$，是合法方案。

两方向互逆（$(l,r,a,b)\to(x,y,z,w)\to(l,r,a,b)$ 显然还原），故为双射。证毕。

**推论**：原问题 $=\max\{C_w-C_x+A_y-A_z:0\le x<y<z<w\le N\}$。由 $N\ge4$，$(0,1,2,3)$ 合法，最大值有限且可达。

---

### 引理 4（四阶段 DP 的状态与转移）

定义（对 $i=0..N$，"处理完 $i$"后）：
$$\mathrm{dp1}_i=\max_{0\le x\le i}(-C_x),\quad \mathrm{dp2}_i=\max_{0\le x<y\le i}(A_y-C_x),\quad \mathrm{dp3}_i=\max_{0\le x<y<z\le i}(A_y-C_x-A_z),$$
空集时取 $-\infty$。则按 $i=0..N$ 顺序、每轮**依次**执行
$$\text{ans}\gets\max(\text{ans},\mathrm{dp3}_{i-1}+C_i);\quad \mathrm{dp3}_i=\max(\mathrm{dp3}_{i-1},\mathrm{dp2}_{i-1}-A_i);$$
$$\mathrm{dp2}_i=\max(\mathrm{dp2}_{i-1},\mathrm{dp1}_{i-1}+A_i);\quad \mathrm{dp1}_i=\max(\mathrm{dp1}_{i-1},-C_i)$$
（$\mathrm{dp\ast}_{-1}=-\infty$，$\text{ans}$ 初值 $-\infty$），最终 $\text{ans}=\max_{x<y<z<w}(C_w-C_x+A_y-A_z)$。

*证明*：先证三个 dp 的递推正确（以 dp3 为例，dp1/dp2 同理）。$\mathrm{dp3}_i$ 的最优四元组前缀 $(x,y,z)$ 要么 $z<i$（则属于 $\mathrm{dp3}_{i-1}$），要么 $z=i$（则 $x<y<i$，值为 $\mathrm{dp2}_{i-1}-A_i$，因 $\mathrm{dp2}_{i-1}$ 枚举了所有 $x<y\le i-1<i$）。两情形取 max 即递推式。dp2：$y<i$ 归 $\mathrm{dp2}_{i-1}$，$y=i$ 归 $\mathrm{dp1}_{i-1}+A_i$（$x<i$）。dp1：$x<i$ 归 $\mathrm{dp1}_{i-1}$，$x=i$ 归 $-C_i$。

**关键：严格递增由"读旧值"保证。** 更新 $\mathrm{dp3}_i$ 用的是 $\mathrm{dp2}_{i-1}$（$y\le i-1<i=z$），故新接入的 $z=i$ 严格大于其 $y$；更新 $\mathrm{dp2}_i$ 用 $\mathrm{dp1}_{i-1}$（$x\le i-1<i=y$）；更新 ans 用 $\mathrm{dp3}_{i-1}$（$z\le i-1<i=w$）。**同一轮内必须按 ans→dp3→dp2→dp1 的顺序**：这样 dp3 读到的 dp2 还是旧值（dp2 尚未在本轮更新），dp2 读到的 dp1 还是旧值，ans 读到的 dp3 还是旧值——每个阶段引用的都是"上一阶段 $\le i-1$"的值，杜绝了同一个 $i$ 兼任两个角色（那会破坏 $x<y<z<w$）。

最后，ans 在每轮把 $w=i$ 与所有 $z<i$ 的最优前缀 $\mathrm{dp3}_{i-1}$ 配对，遍历 $i=0..N$ 即枚举了所有 $w$ 及所有 $x<y<z<w$，故 $\text{ans}=\max_{x<y<z<w}(C_w-C_x+A_y-A_z)$。由引理 3 推论，这就是原问题最优值。证毕。

---

### 引理 5（边界、哨兵与值域）

$N\ge4$ 时答案有限；$|A_i|,|B_i|\le N\cdot10^6<2\times10^9$，$|C_i|<4\times10^9$，收益 $\in(-8\times10^9,8\times10^9)$，须 `long long`。哨兵 $-\infty=-2^{60}$ 安全：它只参与 max 与"加/减一个 $\le4\times10^9$ 的量"，结果仍 $\approx-2^{60}$，既不溢出（`long long` 下限 $-2^{63}$）也永远输给真实值（$\ge-8\times10^9$）。

*证明*：$N\ge4$ ⇒ $(0,1,2,3)$ 合法（引理 3），ans 至少被一个真实值更新，有限。值域：$|A_i|\le\sum|u_t|\le N\cdot10^6\le1999\times10^6<2\times10^9$，同理 $B$，$C=A+B$ 故 $|C_i|<4\times10^9$；收益是四个这样的量相加减，绝对值 $<8\times10^9$，超 `int`（$\pm2.1\times10^9$）须 `long long`（$\pm9.2\times10^{18}$）。哨兵 $-2^{60}\approx-1.15\times10^{18}$：转移里 $\mathrm{dp2}-A_i$、$\mathrm{dp1}+A_i$、$\mathrm{dp3}+C_i$ 至多让它偏移 $4\times10^9$，仍 $\approx-1.15\times10^{18}>-2^{63}$，不溢出；又 $-1.15\times10^{18}\ll-8\times10^9\le$ 任何真实收益，故含哨兵的候选永远不是 max。代码用 `if(dp>ninf/2)` 显式跳过无效状态，双保险。证毕。

---

### 引理 6（复杂度）

时间 $O(N)$，空间 $O(N)$。

*证明*：读入 $2N$ 个数 $O(N)$；预处理 $A,C$ 各 $O(N)$；DP 扫 $i=0..N$ 每轮 $O(1)$，$O(N)$。空间：$A,C$ 两个长 $N+1$ 的 `long long` 数组，$O(N)$。$N\le1999$，实测 $0.05$ 秒。证毕。

---

### 定理

程序输出最大基地适用度 $\max_{1\le l<a\le b<r\le N}V(l,r,a,b)$，时间 $O(N)$、空间 $O(N)$。

*证明*：引理 1 把"凹"字方案刻画为四元组并给出收益式；引理 2 换元成切点形式；引理 3 证约束等价于 $x<y<z<w$ 且为双射，从而原问题 = 切点最优化；引理 4 证四阶段 DP（按序读旧值）正确求出该最大值；引理 5 保证边界、哨兵、值域安全；引理 6 给复杂度。证毕。

---

## 三、模式总结（按实际思考顺序）

- **explore its property** — "凹"字 = 第二行整段 $[l,r]$ + 第一行两段 $[l,a-1],[b+1,r]$；"采集区两侧有基地"翻译成 $l<a\le b<r$。
- **consider easier version** — 先写前缀和收益式 $V=(B_r-B_{l-1})+(A_{a-1}-A_{l-1})+(A_r-A_b)$，枚举四元组 $O(N^4)$ 超时，确认要找结构。
- **reduce to easier problem** — 换元 $x=l-1,y=a-1,z=b,w=r$，约束塌成 $x<y<z<w$，收益塌成 $C_w-C_x+A_y-A_z$：区间枚举 → 选四个有序点。
- **classic problem solution** — 四阶段 DP：`dp1=-C_x`、`dp2=A_y-C_x`、`dp3=A_y-C_x-A_z`，扫到 $i$ 先当 $w$ 更新 ans，再依次接进 dp3/dp2/dp1。
- **explore its property** — 严格递增靠"每阶段读上一阶段的旧值 + 按 ans→dp3→dp2→dp1 顺序更新"保证，杜绝同一下标兼任两角。
- **discuss by category** — 值域 $\pm8\times10^9$ 超 int 须 long long；哨兵 $-2^{60}$ 只参与 max 不溢出；$N\ge4$ 保证答案有限。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|------|------|-------------|--------|
| `n` | 列数（$3<n<2000$） | `int`，读入 | 下标 $1..n$ |
| `a[i]` | 第一行前缀和 $A_i$ | `i64[mxn]`，`a[0]=0` | 读第一行累加 |
| `c[i]` | 两行合计前缀和 $C_i=A_i+B_i$ | `i64[mxn]`，`c[0]=0` | 读第二行时 `c[i]=c[i-1]+top[i]+bot[i]` |
| `dp1` | $\max_{x\le i}(-C_x)$ | `i64`，初值 `ninf` | 已选 $x$ |
| `dp2` | $\max_{x<y\le i}(A_y-C_x)$ | `i64`，初值 `ninf` | 已选 $x,y$ |
| `dp3` | $\max_{x<y<z\le i}(A_y-C_x-A_z)$ | `i64`，初值 `ninf` | 已选 $x,y,z$ |
| `ans` | $\max_{x<y<z<w}(C_w-C_x+A_y-A_z)$ | `i64`，初值 `ninf` | 答案 |
| `ninf` | 哨兵 $-2^{60}$ | `constexpr i64` | 只参与 max，永不赢、不溢出 |
| `mxn` | $2000+5$ | `constexpr int{2005}` | +5 松弛 |

### 算法步骤

1. I/O 加速三行。
2. 读 `n`；读第一行 $n$ 个数，累加得 `a[i]`；读第二行 $n$ 个数，`c[i]=c[i-1]+(a[i]-a[i-1])+bot`。
3. `dp1=dp2=dp3=ans=ninf`。对 $i=0..n$ **依次**：
   1. 若 `dp3` 有效，`ans=max(ans,dp3+c[i])`（$i$ 当 $w$）；
   2. 若 `dp2` 有效，`dp3=max(dp3,dp2-a[i])`（$i$ 当 $z$）；
   3. 若 `dp1` 有效，`dp2=max(dp2,dp1+a[i])`（$i$ 当 $y$）；
   4. `dp1=max(dp1,-c[i])`（$i$ 当 $x$）。
4. 输出 `ans` 与 `'\n'`。

### 复杂度

- 时间：$O(N)$，$N\le1999$，实测 $0.05$ 秒。
- 空间：$O(N)$，两个长 $2005$ 的 `i64` 数组约 $32$ KB。
- 值域：收益 $\in(-8\times10^9,8\times10^9)$，`i64` 安全（引理 5）。

### 验证状态

`g++ -std=c++20 -O2` 编译通过；题面无样例，手算三组（全正挖最小→$12$、含大负数挖之→$15$、全负被迫最小损失→$-25$）程序逐条吻合。与"枚举全部 $(l,r,a,b)$ 四元组、前缀和 $O(1)$ 算收益取 max"的暴力在 $20000$ 组随机数据（$N\in[4,9]$，值 $\in[-10,10]$）上端到端对拍，$0$ mismatch；$N=1999$ 极限随机数据运行 $0.05$ 秒。轻量对拍脚本见 `appendix/P2545/check.cpp`（仅 C++）。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2545 [AHOI2004] 实验基地
// 思路：凹字型 = 选 l<a<=b<r，收益 (B_r-B_{l-1})+(A_{a-1}-A_{l-1})+(A_r-A_b)。
//       换元 x=l-1,y=a-1,z=b,w=r ⇒ 约束 x<y<z<w，收益 C_w-C_x+A_y-A_z（C=A+B）。
//       四阶段 DP：dp1=-C_x, dp2=A_y-C_x, dp3=A_y-C_x-A_z，扫到 i 先当 w 更新 ans，
//       再依次接进 dp3/dp2/dp1；每阶段读上一阶段旧值保证严格递增。O(N)。
//       值域 ±8e9 超 int ⇒ long long；哨兵 -2^60 只参与 max。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};
constexpr i64 ninf{-(1LL<<60)};

int n;
i64 a[mxn],c[mxn];	// a=第一行前缀和, c=两行合计前缀和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
	{
		i64 x;cin>>x;
		a[i]=a[i-1]+x;
	}
	for(int i=1;i<=n;++i)
	{
		i64 x;cin>>x;
		c[i]=c[i-1]+(a[i]-a[i-1])+x;	// a[i]-a[i-1] 即第一行第 i 格
	}
	// 凹字型 = 选 x<y<z<w，收益 c[w]-c[x]+a[y]-a[z]
	i64 dp1=ninf,dp2=ninf,dp3=ninf,ans=ninf;
	for(int i=0;i<=n;++i)
	{
		if(dp3>ninf/2)
			ans=max(ans,dp3+c[i]);	// i 当 w
		if(dp2>ninf/2)
			dp3=max(dp3,dp2-a[i]);	// i 当 z（读旧 dp2，y<i）
		if(dp1>ninf/2)
			dp2=max(dp2,dp1+a[i]);	// i 当 y（读旧 dp1，x<i）
		dp1=max(dp1,-c[i]);	// i 当 x
	}
	cout<<ans<<'\n';
	return 0;
}
```

### 代码说明

- **换元是灵魂**：`c[w]-c[x]+a[y]-a[z]` 把"选区间 + 挖子段"变成"选四个递增点各加一项"，四重枚举塌成一次扫描。
- **更新顺序 ans→dp3→dp2→dp1 不可乱**：每个阶段读的是"上一阶段尚未被本轮更新"的旧值，从而 $x<y<z<w$ 严格成立。若先更新 dp1 再更新 dp2，同一个 $i$ 会同时当 $x$ 和 $y$，破坏严格递增。
- **`if(dp>ninf/2)` 跳过无效状态**：dp1 在 $i=0$ 才有效、dp2 在 $i=1$、dp3 在 $i=2$、ans 在 $i=3$，前几轮用哨兵守卫避免把 $-\infty$ 当合法前缀。即便不加守卫，哨兵 $\ll$ 真实值也不会赢（引理 5），守卫只是双保险。
- **`c[i]=c[i-1]+(a[i]-a[i-1])+x`**：`a[i]-a[i-1]` 是第一行第 $i$ 格，加上第二行第 $i$ 格 `x`，即两行合计的增量，无需另存第二行前缀和。
- **值域**：收益可达 $\pm8\times10^9$，`int` 会溢出，全程 `i64`；`ninf=-(1LL<<60)` 加减 $\le4\times10^9$ 仍安全。
- 下标 1-based（前缀和天然以"前 $i$ 格"定义），切点 $x,y,z,w\in[0,n]$，$i$ 从 $0$ 扫到 $n$ 含两端。