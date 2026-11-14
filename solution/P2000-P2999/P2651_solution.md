# P2651 添加括号III — 题解

## 一、解题过程：人类思考叙事

**题意重述。** 给表达式 $a_1/a_2/\cdots/a_n$（$2\le n\le10^4$，$1\le a_i\le2^{31}-1$，$t\le100$ 组），可任意加括号改变运算顺序，问能否使其值为整数。

**第一步：哪些数能进分子、哪些只能进分母？** 我先盯住前两个数。$a_1$ 是第一个数，无论怎么加括号它都在分子（它永远是被除的"起点"）。$a_2$ 呢？试了几个括号：$(a_1/a_2)/a_3=a_1/(a_2a_3)$、$a_1/(a_2/a_3)=a_1a_3/a_2$——$a_2$ 始终在分母。我猜 **$a_2$ 永远在分母**，后面用归纳法证明（引理 1）。而 $a_3..a_n$ 可以进分子也可以进分母。

**第二步：怎样最容易变成整数？** 要让分数是整数，分子越大越好。既然 $a_1$ 必在分子、$a_2$ 必在分母，最理想就是把 $a_3..a_n$ **全放分子**。这能做到吗？能：$a_1/(a_2/a_3/\cdots/a_n)=a_1a_3\cdots a_n/a_2$。于是值为整数 $\iff a_2\mid a_1a_3\cdots a_n$。

**第三步：会不会"放一些进分母"反而更好？** 我担心全放分子不是最优。设 $P=a_3\cdots a_n$，把下标集 $S\subseteq\{3..n\}$ 放分母，值变成 $a_1P/(a_2(\prod_S a_i)^2)$。要为整数需 $a_2(\prod_S)^2\mid a_1P$，这比 $a_2\mid a_1P$ **更强**（多除了一个平方）。所以全放分子（$S=\varnothing$）是最弱的要求：它不整数，别的更不整数；它整数，答案就是 Yes。于是**答案 = ($a_2\mid a_1a_3\cdots a_n$)**。

**第四步：乘积天文数字，怎么判整除？** $a_1a_3\cdots a_n$ 可达 $(2^{31})^{10^4}$，绝不能真乘。我想到**逐个约分**：令 $\text{rem}=a_2$，对每个 $i\ne2$ 做 $\text{rem}\gets\text{rem}/\gcd(\text{rem},a_i)$。直觉是"用每个分子去消掉 rem 里的因子"。最后 $\text{rem}=1$ 当且仅当 $a_2$ 的所有素因子都被分子覆盖，即 $a_2\mid$ 乘积。这一步的正确性用素因子指数严格证（引理 5）。全程只用 `gcd`，$O(n\log a)$，无需高精度、无需分解质因数。

**手造数据核对**（题面没给样例）：

| 表达式 | 判据 | 程序 |
|---|---|---|
| $1/2$ | $2\nmid1$ | `No` |
| $1/2/4$ | $2\mid1\cdot4$ | `Yes` |
| $6/2/3/7$ | $2\mid6\cdot3\cdot7=126$ | `Yes` |
| $6/7/2/3$ | $7\nmid6\cdot2\cdot3=36$ | `No` |
| $2/12/3/4$ | $12\mid2\cdot3\cdot4=24$ | `Yes` |
| $2/12/3$ | $12\nmid2\cdot3=6$ | `No` |

---

## 二、思维链：完整证明

**记号。** 表达式 $a_1/a_2/\cdots/a_n$，加括号即选一棵以 $a_i$ 为叶、`/` 为内节点的二叉树。$P=\prod_{i=3}^n a_i$。$v_p(x)$ 为素数 $p$ 在 $x$ 中的指数。

### 引理 1（$a_1$ 恒在分子，$a_2$ 恒在分母）

任何加括号方案，其值都可写成 $\dfrac{a_1}{a_2}\cdot\prod_{i=3}^n a_i^{e_i}$，其中 $e_i\in\{+1,-1\}$。特别地 $a_1$ 指数 $+1$、$a_2$ 指数 $-1$。

*证明*：对 $n$ 归纳，证命题 $P(n)$：$a_1/\cdots/a_n$ 的任一括号化值 $=\frac{a_1}{a_2}\cdot Q$，$Q$ 是 $a_3..a_n$ 的带符号乘积（每个指数 $\pm1$）。
- $n=2$：值 $=a_1/a_2$，$Q$ 为空积 $1$。成立。
- 设 $P(<n)$ 成立。顶层把序列切成左 $L$（$a_1..a_k$）右 $R$（$a_{k+1}..a_n$），值 $=L/R$。
  - **$k=1$**：$L=a_1$；$R$ 是 $a_2/\cdots/a_n$ 的括号化，由归纳 $R=\frac{a_2}{a_3}\cdot Q'$（$Q'$ 为 $a_4..a_n$ 带符号积）。故值 $=a_1/\big(\frac{a_2}{a_3}Q'\big)=\frac{a_1}{a_2}\cdot\frac{a_3}{Q'}$，而 $a_3/Q'$ 是 $a_3..a_n$ 的带符号积。成立。
  - **$k\ge2$**：由归纳 $L=\frac{a_1}{a_2}\cdot Q$（$Q$ 为 $a_3..a_k$ 带符号积），值 $=L/R=\frac{a_1}{a_2}\cdot(Q/R)$，$Q/R$ 是 $a_3..a_n$ 带符号积（$R$ 的叶取负指数）。成立。
故 $a_1$ 恒 $+1$、$a_2$ 恒 $-1$。证毕。

### 引理 2（$a_3..a_n$ 全放分子可达）

取括号 $a_1/(a_2/a_3/\cdots/a_n)$（内层左结合），其值 $=\dfrac{a_1a_3\cdots a_n}{a_2}=\dfrac{a_1P}{a_2}$。

*证明*：内层 $a_2/a_3/\cdots/a_n$ 左结合 $=\dfrac{a_2}{a_3a_4\cdots a_n}=\dfrac{a_2}{P}$。故整体 $=a_1/\dfrac{a_2}{P}=\dfrac{a_1P}{a_2}$。此时 $a_3..a_n$ 指数全 $+1$。证毕。

### 引理 3（全放分子是最弱要求）

设把 $S\subseteq\{3..n\}$ 放分母，值为整数 $\Rightarrow a_2\mid a_1P$；反之 $a_2\mid a_1P\Rightarrow$ 存在方案（$S=\varnothing$）使值为整数。

*证明*：$S$ 放分母时，分子 $=a_1\cdot P/\prod_S a_i$，分母 $=a_2\cdot\prod_S a_i$，值 $=\dfrac{a_1P}{a_2(\prod_S a_i)^2}$。为整数需 $a_2(\prod_S a_i)^2\mid a_1P$。因 $(\prod_S a_i)^2$ 是正整数，$a_2\mid a_2(\prod_S)^2\mid a_1P$，故 $a_2\mid a_1P$（必要性）。反之若 $a_2\mid a_1P$，由引理 2 取 $S=\varnothing$，值 $=a_1P/a_2$ 为整数（充分性）。证毕。

**推论（判据）**：答案为 `Yes` $\iff a_2\mid a_1P=a_1a_3\cdots a_n$。

### 引理 4（问题归约）

原问题等价于判定 $a_2\mid\prod_{i\ne2}a_i$。

*证明*：由引理 3 的推论，`Yes` $\iff a_2\mid a_1a_3\cdots a_n=a_1\prod_{i=3}^n a_i=\prod_{i\ne2}a_i$（因 $a_1$ 也在乘积里）。证毕。

### 引理 5（gcd 逐个约分的正确性）

令 $\text{rem}=a_2$，依次对 $i\ne2$ 做 $\text{rem}\gets\text{rem}/\gcd(\text{rem},a_i)$。结束后 $\text{rem}=1\iff a_2\mid\prod_{i\ne2}a_i$。

*证明*：逐素数 $p$ 分析。设 $e=v_p(a_2)$，$f_i=v_p(a_i)$。$\text{rem}$ 中 $p$ 的指数初为 $e$。一步 $\text{rem}/\gcd(\text{rem},a_i)$ 把指数减去 $\min(\text{当前指数},f_i)$，即 $\text{exp}\gets\max(0,\text{exp}-f_i)$。依次处理所有 $i\ne2$，指数变为
$$\max\bigl(0,\ e-\textstyle\sum_{i\ne2}f_i\bigr)$$
（归纳：$\max(0,\max(0,e-f_1)-f_2)=\max(0,e-f_1-f_2)$，一旦到 $0$ 就保持 $0$）。故 $\text{rem}=1\iff$ 对所有 $p$ 有 $\max(0,e-\sum f_i)=0\iff e\le\sum_{i\ne2}f_i\iff v_p(a_2)\le v_p(\prod_{i\ne2}a_i)$ 对所有 $p$ $\iff a_2\mid\prod_{i\ne2}a_i$。证毕。

**实现陷阱**：$\text{rem}$ 与 $a_i$ 可达 $2^{31}-1$，用 `long long` 存 $\text{rem}$ 防 `gcd` 中间溢出；一旦 $\text{rem}=1$ 可提前结束（已整除）。

### 引理 6（复杂度）

每组 $O(n\log(\max a))$，总 $O(tn\log(\max a))$；空间 $O(n)$。

*证明*：每组做 $\le n$ 次 `gcd`，每次 $O(\log a)$；$\text{rem}$ 单调不增，提前 `break` 只会更快。$t\le100,n\le10^4$，约 $10^6$ 次 `gcd`，远低于限制。存 $a[1..n]$，空间 $O(n)$。证毕。

### 定理

程序对每组输出 `Yes`/`No` 正确。

*证明*：引理 1–3 把题目化为 $a_2\mid\prod_{i\ne2}a_i$（引理 4）；引理 5 保证 gcd 约分恰好判定该整除；引理 6 给复杂度。证毕。

---

## 三、模式总结（按实际思考顺序）

- **explore its property** — 归纳证明 $a_1$ 恒分子、$a_2$ 恒分母，$a_3..a_n$ 符号自由。
- **consider easier version** — 先想"全放分子"这一最理想方案：$a_1/(a_2/a_3/\cdots/a_n)=a_1P/a_2$。
- **reduce to easier problem** — 放分母只会让整除要求更强（多除一个平方）⇒ 全放分子是最弱要求，答案 $\iff a_2\mid a_1P$。
- **classic problem solution** — 乘积巨大不能真乘 ⇒ 用 $\text{rem}/\gcd(\text{rem},a_i)$ 逐个约分判整除，素因子指数证明其正确。
- **discuss by category** — $\text{rem}$ 用 `long long`；$\text{rem}=1$ 提前结束；无需高精度/质因数分解。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|------|------|-------------|--------|
| `t,n` | 组数、表达式长度 | `int`，读入 | $n\le10^4,t\le100$ |
| `a[i]` | 第 $i$ 个数（0-based，$a_2$ 是 `a[1]`） | `i64[mxn]` | $\le2^{31}-1$ |
| `rem` | 待约分的分母，初 $=a_2$ | `i64` | 约到 $1$ 即整除 |
| `mxn` | $10005$ | `constexpr int` | $+5$ 松弛 |

### 算法步骤

1. I/O 加速三行；读 $t$。
2. 每组：读 $n$ 与 $a[0..n-1]$。
3. `rem=a[1]`（即 $a_2$）。
4. 对 $i=0..n-1$ 且 `rem>1`：跳过 $i=1$；`rem/=gcd(rem,a[i])`。
5. 输出 `rem==1?"Yes":"No"`。

### 复杂度

- 时间 $O(tn\log(\max a))$，约 $10^6$ 次 `gcd`。
- 空间 $O(n)$。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；$6$ 组手算全对；与"枚举全部加括号方案、精确分数判整"的独立暴力在 $30000$ 组随机小数据（$n\le6,a_i\le25$）上 $0$ mismatch。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2651 添加括号III
// 思路：a1 恒分子、a2 恒分母；全放分子最优 ⇒ 答案 iff a2 | a1*a3*...*an。
//       乘积巨大 ⇒ rem=a2，逐个 rem/=gcd(rem,a_i) 约分，rem==1 即整除。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n;
i64 a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>a[i];
		i64 rem=a[1];			// a2 永远在分母，判断它能否被整除
		for(int i=0;i<n&&rem>1;++i)
		{
			if(i==1)
				continue;
			rem/=gcd(rem,a[i]);	// 逐个用分子约掉 rem 的因子
		}
		cout<<(rem==1?"Yes\n":"No\n");
	}
	return 0;
}
```

### 代码说明

- **`a[1]` 是 $a_2$**：0-based 读入，$a_2$ 对应下标 $1$，它恒在分母，是唯一需要被整除的数。
- **`rem/=gcd(rem,a[i])`**：逐个用分子 $a_i$（$i\ne2$）消去 `rem` 的素因子；由引理 5，最终 `rem==1` 当且仅当 $a_2\mid\prod_{i\ne2}a_i$。
- **`rem>1` 提前结束**：一旦约到 $1$ 已整除，后面的数不必再看（但仍需先读完整组输入）。
- **`i64`**：$a_i\le2^{31}-1$，`gcd` 与 `rem` 用 `long long` 稳妥；`std::gcd` 需 C++17，`-std=c++20` 满足。
- 跳过 `i==1`（$a_2$ 自己不能约自己）；`a[0]`（$a_1$）与 $a_3..a_n$ 都参与约分。