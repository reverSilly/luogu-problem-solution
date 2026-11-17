# P2816 宋荣子搭积木 — 题解

## 一、解题过程：人类思考叙事

**题意重述。** $n$ 块积木（$1\le n\le5000$），第 $i$ 块有情绪值 $x_i$（$0\le x_i\le n$）。摞成若干竖列，要求**每块都用上**，且每块积木**正上方**的积木总数不超过它的 $x_i$（否则它不高兴）。求**最少列数**。

**第一步：把"一列"的约束写清楚。** 一列高 $h$，从**顶**往下数，第 $d$ 层（$d=0$ 是顶）的积木正上方有 $d$ 块。所以第 $d$ 层的积木必须满足 $x\ge d$。换句话说：一列从顶到底，各块的 $x$ 必须分别 $\ge 0,1,2,\dots,h-1$。

于是"哪些积木能摞成一列"就有了判据：把这一列的 $x$ **升序**排成 $a_{(0)}\le a_{(1)}\le\cdots\le a_{(h-1)}$，最优摆法是**小的放上面**（要求低），即第 $j$ 小的放第 $j$ 层。可行当且仅当

$$a_{(j)}\ge j\quad(\forall j=0..h-1).$$

为什么"小的放上面"最优？因为约束是 $x\ge\text{层号}$，层号从上往下递增；把最小的 $x$ 配最小的层号，是经典的"排序匹配"——若存在任何合法摆法，升序对升序也合法（交换论证，下面证）。

**第二步：找下界——关键性质。** 直接构造最少列不好想，先问"**至少**要几列"。盯住情绪值小的积木：一块 $x\le v$ 的积木只能放在层号 $d\le x\le v$ 的位置，也就是**每列的最上面 $v+1$ 层**（层号 $0..v$）。一列只有 $v+1$ 个这样的"浅位"。

设 $x\le v$ 的积木共 $\mathrm{cnt}(v)$ 块，它们全要塞进各列的浅位，每列最多塞 $v+1$ 块，所以

$$\text{列数 } C\ \ge\ \left\lceil\frac{\mathrm{cnt}(v)}{v+1}\right\rceil\quad(\forall v\ge0).$$

把积木**升序**排成 $a_0\le a_1\le\cdots\le a_{n-1}$。取 $v=a_i$，则 $\mathrm{cnt}(a_i)\ge i+1$（前 $i+1$ 块都 $\le a_i$），于是

$$C\ \ge\ \left\lceil\frac{i+1}{a_i+1}\right\rceil\quad(\forall i).$$

取最大，得下界 $C\ge \max_i\lceil(i+1)/(a_i+1)\rceil$。

**第三步：猜这个下界就是答案，并构造证明。** 下界往往松，但这里我猜它**紧**。怎么证？给出一个恰好用 $C=\max_i\lceil(i+1)/(a_i+1)\rceil$ 列的摆法。

自然的构造：**轮转发牌**。把升序排好的积木依次发给 $C$ 列：第 $i$ 块发给第 $i\bmod C$ 列，且在该列里从顶往下摞。于是第 $c$ 列拿到下标 $c,c+C,c+2C,\dots$，第 $j$ 个（层号 $j$）拿到的是 $a_{c+jC}$。要合法，需

$$a_{c+jC}\ge j.$$

而 $C$ 的定义恰好能推出这个！由 $C\ge\lceil(i+1)/(a_i+1)\rceil$ 对所有 $i$ 成立，即 $i+1\le C(a_i+1)$，整理得 $a_i\ge \frac{i+1}{C}-1$，取整即

$$a_i\ \ge\ \left\lfloor\frac{i}{C}\right\rfloor.$$

（因为 $a_i$ 是整数，$a_i\ge\frac{i+1}{C}-1\iff a_i+1>\frac{i}{C}\iff a_i\ge\lfloor i/C\rfloor$。）

代入 $i=c+jC$（$0\le c<C$）：$\lfloor(c+jC)/C\rfloor=j+\lfloor c/C\rfloor=j$。所以 $a_{c+jC}\ge j$，正是所需！**构造合法，下界可达，答案就是 $C$。**

**第四步：化简成整数算式。** $\lceil(i+1)/(a_i+1)\rceil=(i+1+a_i)/(a_i+1)$（整数除法 $\lceil p/q\rceil=(p+q-1)/q$，这里 $p=i+1,q=a_i+1$）。所以

$$\text{ans}=\max_{i=0}^{n-1}\frac{i+a_i+1}{a_i+1}\quad(\text{整数除法}).$$

排序后一遍扫即可。$n\le5000$，$O(n\log n)$ 绰绰有余。

**手造数据核对**：

| $x$（升序后） | 逐项 $\lceil(i+1)/(a_i+1)\rceil$ | ans | 验证 |
|---|---|---|---|
| `[0]` | $1$ | $1$ | 单块，上方 $0$ 块 ✓ |
| `[0,0,0]` | $1,2,3$ | $3$ | $x=0$ 不能垫底，只能各占一列顶 ✓ |
| `[1,1,1]` | $1,1,2$ | $2$ | 列高 $2,1$：底 $x=1$ 上方 $1$ 块 ✓ |
| `[0,1,2]` | $1,1,1$ | $1$ | 顶→底 $0,1,2$，层号 $0,1,2$ ✓ |
| `[0,0,1,1]` | $1,2,2,2$ | $2$ | 两列各 `0(顶),1(底)` ✓ |
| `[2,0,2,0]`→`[0,0,2,2]` | $1,2,1,2$ | $2$ | 轮转：列 $0$ 拿 $a_0=0,a_2=2$；列 $1$ 拿 $a_1=0,a_3=2$ ✓ |

特别看 `[0,0,0]`：三块 $x=0$ 谁也不能在谁下面，必须 $3$ 列，公式给 $3$，吻合。`[0,1,2]` 能摞成一列（顶 $0$、中 $1$、底 $2$），公式给 $1$，吻合。

---

## 二、思维链：完整证明

**记号。** 升序排序后 $a_0\le a_1\le\cdots\le a_{n-1}$。一列从顶往下层号 $0,1,\dots$；层号 $d$ 的积木上方有 $d$ 块，需 $x\ge d$。$C$ 为列数。

---

### 引理 1（单列可行性判据）

一组积木能摞成一列，当且仅当其 $x$ 升序排列 $a_{(0)}\le\cdots\le a_{(h-1)}$ 满足 $a_{(j)}\ge j$ 对所有 $j$。且此时"升序对层号"（第 $j$ 小放层号 $j$）是一个合法摆法。

*证明*：

**充分性**：若 $a_{(j)}\ge j$，把第 $j$ 小的积木放层号 $j$，则该积木 $x=a_{(j)}\ge j=$ 上方块数，合法。故可摞成一列。

**必要性**（交换论证）：设存在某合法摆法，即一个双射 $\pi$ 把积木配到层号 $0..h-1$，满足每块 $x\ge\pi(\text{块})$。要证升序排列也满足 $a_{(j)}\ge j$。

反设不然：存在最小的 $j$ 使 $a_{(j)}<j$，即 $a_{(j)}\le j-1$。则前 $j+1$ 小的积木 $a_{(0)},\dots,a_{(j)}$ 都 $\le j-1$。在合法摆法 $\pi$ 中，这 $j+1$ 块积木各自被配到一个层号，且每块的层号 $\le$ 其 $x\le j-1$，即它们的层号都落在 $\{0,1,\dots,j-1\}$（只有 $j$ 个位置）。但 $j+1$ 块积木要占 $j+1$ 个**不同**层号（$\pi$ 是双射），鸽笼原理矛盾。故对所有 $j$ 有 $a_{(j)}\ge j$。证毕。

---

### 引理 2（列数下界）

对任意 $v\ge0$，设 $\mathrm{cnt}(v)=|\{i:x_i\le v\}|$。则任何合法方案的列数 $C\ge\lceil\mathrm{cnt}(v)/(v+1)\rceil$。特别地，升序排序后 $C\ge\max_i\lceil(i+1)/(a_i+1)\rceil$。

*证明*：一块 $x\le v$ 的积木若放在层号 $d$，需 $x\ge d$，故 $d\le x\le v$，即只能占层号 $0..v$ 之一，共 $v+1$ 个位置。每列至多容纳 $v+1$ 块 $x\le v$ 的积木。$\mathrm{cnt}(v)$ 块这样的积木分到 $C$ 列，由鸽笼原理 $C\cdot(v+1)\ge\mathrm{cnt}(v)$，即 $C\ge\lceil\mathrm{cnt}(v)/(v+1)\rceil$。

取 $v=a_i$：因 $a_0\le\cdots\le a_i$ 都 $\le a_i$，故 $\mathrm{cnt}(a_i)\ge i+1$，得 $C\ge\lceil(i+1)/(a_i+1)\rceil$。对所有 $i$ 取最大即得。证毕。

---

### 引理 3（下界的等价整数形式）

令 $C=\max_{i}\lceil(i+1)/(a_i+1)\rceil$。则

(a) $C=\max_i\big\lfloor(i+a_i+1)/(a_i+1)\big\rfloor$（整数除法）；

(b) 对所有 $i$，$a_i\ge\lfloor i/C\rfloor$；

(c) $1\le C\le n$。

*证明*：

(a) 对正整数 $p,q$，$\lceil p/q\rceil=\lfloor(p+q-1)/q\rfloor$。取 $p=i+1,q=a_i+1\ge1$：$\lceil(i+1)/(a_i+1)\rceil=\lfloor(i+1+a_i+1-1)/(a_i+1)\rfloor=\lfloor(i+a_i+1)/(a_i+1)\rfloor$。取最大即 (a)。

(b) 由 $C\ge\lceil(i+1)/(a_i+1)\rceil$ 得 $C\ge(i+1)/(a_i+1)$，即 $C(a_i+1)\ge i+1$，即 $a_i+1\ge(i+1)/C$，即 $a_i\ge(i+1)/C-1=(i+1-C)/C$。因 $a_i$ 为整数，$a_i\ge\lceil(i+1-C)/C\rceil$。而 $\lceil(i+1-C)/C\rceil=\lceil(i+1)/C\rceil-1=\lfloor i/C\rfloor$（最后一步：对整数 $i,C>0$，$\lceil(i+1)/C\rceil-1=\lfloor i/C\rfloor$，因为 $\lceil(i+1)/C\rceil=\lfloor(i+C)/C\rfloor=\lfloor i/C\rfloor+1$）。故 $a_i\ge\lfloor i/C\rfloor$。

(c) $C\ge\lceil(0+1)/(a_0+1)\rceil\ge1$（$a_0\ge0$）。又 $\lceil(i+1)/(a_i+1)\rceil\le i+1\le n$（$a_i\ge0$），故 $C\le n$。证毕。

---

### 引理 4（轮转构造达到下界）

把升序积木按"第 $i$ 块发给第 $i\bmod C$ 列、在该列从顶往下摞"的方式分配（$C$ 由引理 3）。则每列合法，且恰用 $C$ 列（每列非空），从而最少列数 $=C$。

*证明*：第 $c$ 列（$0\le c<C$）拿到下标 $c,c+C,c+2C,\dots$。设它拿到第 $j$ 个（层号 $j$）的积木下标为 $i=c+jC$。由引理 3(b)，$a_i\ge\lfloor i/C\rfloor=\lfloor(c+jC)/C\rfloor=j+\lfloor c/C\rfloor=j$（因 $0\le c<C$）。故该积木 $x=a_i\ge j=$ 层号 $=$ 上方块数，合法。每列都合法。

非空：$C\le n$（引理 3(c)），故对每个 $c<C\le n$，下标 $c$ 存在且发给第 $c$ 列，每列至少一块。共用 $C$ 列。

结合引理 2 的下界 $C\ge\max_i\lceil(i+1)/(a_i+1)\rceil=C$，知最少列数恰为 $C$。证毕。

---

### 引理 5（复杂度）

时间 $O(n\log n)$，空间 $O(n)$。

*证明*：排序 $O(n\log n)$；扫描 $n$ 项每项 $O(1)$ 整数运算。数组 $O(n)$。$n\le5000$ 远低于时限。证毕。

---

### 定理

最少列数 $=\max_{i=0}^{n-1}\lfloor(i+a_i+1)/(a_i+1)\rfloor$（$a$ 为 $x$ 升序排列），程序输出该值。

*证明*：引理 1 给出单列判据；引理 2 给出下界；引理 3 把下界化成整数算式并推出 $a_i\ge\lfloor i/C\rfloor$；引理 4 用轮转构造达到下界，证明下界紧；引理 5 给出复杂度。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **explore its property** — 一列从顶层号 $d$ 的积木需 $x\ge d$；单列可行 ⟺ 升序 $a_{(j)}\ge j$（排序匹配 / 鸽笼）。
2. **consider easier version** — 先求下界：$x\le v$ 的积木只能占每列最上 $v+1$ 层 ⇒ $C\ge\lceil\mathrm{cnt}(v)/(v+1)\rceil$。
3. **reduce to easier problem** — 升序排序后下界写成 $C\ge\max_i\lceil(i+1)/(a_i+1)\rceil$，猜它就是答案。
4. **classic problem solution** — 轮转发牌构造：第 $i$ 块给第 $i\bmod C$ 列；由 $C$ 的定义推出 $a_i\ge\lfloor i/C\rfloor$，恰好保证每列合法 ⇒ 下界可达。
5. **discuss by category** — 化 $\lceil p/q\rceil$ 为整数除法 $(p+q-1)/q$，一遍扫描取最大；$x=0$ 的积木各占一列顶是极端情形。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 | 初值 / 记忆点 |
|------|------|------|---------------|
| `mxn` | $n$ 上界 $5000+5$ | `constexpr int{5005}` | 数组尺寸 |
| `n` | 积木数 | `int` | 读入 |
| `a[i]` | 升序排序后的情绪值 | `int[mxn]` | 读入后 `sort` |
| `ans` | 最少列数 $C$ | `int` | 全局默认 $0$，扫描取 max（$n\ge1$ 保证 $\ge1$） |
| `i` | 扫描下标 | `int` | 局部 |

### 算法步骤

1. I/O 加速三行；读 `n` 与 `a[0..n-1]`。
2. `sort(a,a+n)` 升序。
3. 对 $i=0..n-1$：`ans=max(ans,(i+a[i]+1)/(a[i]+1))`（整数除法即 $\lceil(i+1)/(a_i+1)\rceil$）。
4. 输出 `ans`。

### 复杂度

- 时间：$O(n\log n)$，$n\le5000$。
- 空间：$O(n)$。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；$6$ 组手造数据（含全 $0$、可单列、混合）与公式逐项吻合。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2816 宋荣子搭积木
// 一列从顶层号 d 的积木需 x>=d。下界：x<=v 的积木只能占每列最上 v+1 层。
// 升序后 C=max ceil((i+1)/(a[i]+1))，轮转发牌构造恰好达到下界 ⇒ 这就是答案。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};

int n,a[mxn],ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	for(int i=0;i<n;++i)
		ans=max(ans,(i+a[i]+1)/(a[i]+1));	// ceil((i+1)/(a[i]+1)) 的整数写法
	cout<<ans<<'\n';
	return 0;
}
```

### 代码说明

- `(i+a[i]+1)/(a[i]+1)` 是 $\lceil(i+1)/(a_i+1)\rceil$ 的整数除法形式（$\lceil p/q\rceil=(p+q-1)/q$，$p=i+1,q=a_i+1$）。分子 $\le n+n+1\approx10^4$，`int` 不溢出。
- 必须先 `sort`：公式依赖 $a$ 升序（下界取 $v=a_i$ 时用到"前 $i+1$ 块都 $\le a_i$"）。
- `ans` 全局默认 $0$，但 $n\ge1$ 时第一项 $\ge1$，扫描后必 $\ge1$，无需额外初始化。
- 整个算法只有"排序 + 一遍扫描"，构造性证明（引理 4）保证它给出的就是最少列数，无需真的去摆积木。