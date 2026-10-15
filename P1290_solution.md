# P1290 欧几里德的游戏 — 题解

> 难度：提高 / 省选− ｜ 标签：数学、博弈论、递归 ｜ 数据：$1\le C\le 6$，$M,N<2^{31}$

---

## Part 1 — 重述问题

两个人玩一个由两个正整数开始的游戏，Stan 先手，Ollie 后手。

- 当前局面是一对非负整数 $(a,b)$（至少一个非零）。
- 轮到你的时候：取**较大的那个数**，从中减去**较小那个数的一个正整数倍**，使得结果**不小于 $0$**。
- **谁在某一步制造出 $0$，谁立刻获胜**，游戏结束。
- 例如 $(25,7)$：Stan $(25,7)\to(11,7)$（$25-2\cdot7$），Ollie $(11,7)\to(4,7)$（$11-1\cdot7$），Stan $(4,7)\to(4,3)$（$7-1\cdot4$），Ollie $(4,3)\to(1,3)$（$4-1\cdot3$），Stan $(1,3)\to(1,0)$（$3-3\cdot1$）—— Stan 赢。

输入 $C$ 组，每组两个正整数 $M,N$，两人**都最优操作**，判断谁赢：Stan 赢输出 `Stan wins`，否则输出 `Ollie wins`。

约束很小（$C\le 6$），所以单次复杂度几乎没有上限，关键只在"想清楚判定规则"。

---

## Part 2 — 思路发现（Chain-of-thought）

### 第 0 步：先把"一步能走成什么样"刻精确

把局面看成无序对，令 $a\ge b\ge 0$。轮到你在 $a\ge b$ 时操作，只能动大数 $a$：

$$
a\longrightarrow a-k\,b,\qquad k\in\mathbb{Z}^{+},\quad a-k\,b\ge 0.
$$

于是 $k$ 的范围是 $1\le k\le\left\lfloor a/b\right\rfloor$，走完后得到 $\big(a-k\,b,\;b\big)$。

注意 $a-k\,b\ge a-\lfloor a/b\rfloor b=a\bmod b$，且当 $b\mid a$ 时取 $k=a/b$ 可以直接取到 $0$。

> **急刹车**：只要轮到你时大数是小数的整数倍（$b\mid a$），你取 $k=a/b$ 一步就得到 $(0,b)$，**你直接赢**。这是后面所有论证里反复用到的"逃生门"。

### 第 1 步：定胜负的递归结构（博弈题标准套路）

定义 $\text{win}(a,b)=\text{true}$ 表示"轮到该玩家、面对 $\{a,b\}$（约定 $a\ge b$）时，该玩家必胜"。

- **终止**：$\text{win}(x,0)=\text{false}$。因为 $(x,0)$ 意味着**上一步**已有人造出 $0$ 而赢了，轮到的人已经输了。
- **一般**（$b>0$）：
  $$
  \text{win}(a,b)=\text{true}\iff\exists\,k\in[1,\lfloor a/b\rfloor]\ \text{使}\ \text{win}(a-k\,b,\,b)=\text{false}.
  $$
  存在一步走到"对手输"的局面就赢；所有走向都"对手赢"就输。

> **反直觉点**：终局 $(x,0)$ 是**轮到的人输**而非赢——造 $0$ 的胜利属于上一个人。先把这条钉死，后面才不会错。

### 第 2 步：核心观察 —— 什么时候"有得选"，什么时候"被迫"

这是本题真正的突破口。看合法 $k$ 的个数 $\lfloor a/b\rfloor$：

- **$a\ge 2b$（即 $\lfloor a/b\rfloor\ge 2$）**：你有**至少两个**选择（比如 $k=1$ 和 $k=\lfloor a/b\rfloor$）。你有"挑选权"。
- **$b<a<2b$（即 $\lfloor a/b\rfloor=1$）**：$k$ 只能取 $1$，你**没得选**，被迫走 $\{a-b,\,b\}$（且此时 $a-b<b$，状态变小）。

直觉上，"有选择"的一方通常更强。但"有选择就一定赢"在博弈里**不一定**成立（可能你每个选择都让对手赢）。所以下面要**严格证明**：$a\ge 2b$ 时当前玩家确实必胜。

### 第 3 步：严格证明核心引理

**引理.** 对所有 $a\ge b\ge 0$，

$$
\text{win}(a,b)=
\begin{cases}
\text{false}, & b=0,\\[2pt]
\text{true}, & a\ge 2b,\\[2pt]
\lnot\,\text{win}(b,\,a-b), & b<a<2b.
\end{cases}
$$

**证明（对大数 $a$ 作强归纳）。** 记"公式值"为右侧分段给出的布尔值，"定义值"为按第 1 步定义的布尔值。归纳论断：对所有 $b\le a$，公式值 $=$ 定义值。

**基础 $a=0$。** 只能 $b=0$。定义值：$b=0$ 终止 $\to\text{false}$。公式值：第一项 $b=0\to\text{false}$。一致。✓

**归纳步。** 设 $a\ge 1$，且对所有 $a'<a$ 论断成立。取任意 $b$，$b\le a$，分三情况。

**情况 A：$b=0$。** 定义值 $=\text{false}$（终止）；公式值第一项 $=\text{false}$。一致。✓

**情况 B：$b<a<2b$。** 此时 $\lfloor a/b\rfloor=1$，合法 $k$ 只有 $k=1$（$k\ge2\Rightarrow a-2b<0$ 非法）。定义值 $=\lnot\,\text{win}(a-b,b)$。因 $a<2b$ 有 $a-b<b$，故 $\{a-b,b\}$ 大数为 $b$，即 $\text{win}(a-b,b)=\text{win}(b,a-b)$。又 $b\le a-1<a$，归纳假设适用，$\text{win}(b,a-b)$ 的值等于公式值。于是定义值 $=\lnot\,\text{win}(b,a-b)$，与公式第三项一致。✓

**情况 C：$a\ge 2b$。** 要证定义值 $=\text{true}$，即**存在** $k$ 使 $\text{win}(a-k\,b,b)=\text{false}$。设 $a=qb+r$，$q=\lfloor a/b\rfloor\ge 2$，$0\le r<b$。

- **若 $r=0$**：取 $k=q$，得 $\{0,b\}$，其值 $\text{win}(0,b)=\text{win}(b,0)=\text{false}$（终止）。故存在 $k$，定义值 $=\text{true}$。✓

- **若 $r>0$**：考察候选 $k=q$，其后继为 $\{r,b\}$（大数 $b$，因 $r<b$），定义值记为 $W:=\text{win}(b,r)$。由公式按 $b$ 与 $2r$ 的大小：

  - **子情况 i：$b\ge 2r$。** 则 $W=\text{true}$（公式第二项）。当前玩家改取 $k=q-1$，后继 $\{b+r,b\}$（大数 $b+r$，小数 $b$）。因 $r<b$ 有 $b+r<2b$，走公式第三项：
    $$
    \text{win}(b+r,b)=\lnot\,\text{win}(b,(b+r)-b)=\lnot\,\text{win}(b,r)=\lnot\,\text{true}=\text{false}.
    $$
    故取 $k=q-1$ 即让对手输，定义值 $=\text{true}$。✓

  - **子情况 ii：$b<2r$。** 此时 $\{b,r\}$ 走公式第三项，$W=\lnot\,\text{win}(r,b-r)$（注意 $b-r<r$，$\{r,b-r\}$ 大数为 $r<b<a$，归纳假设适用）。
    - 若 $\text{win}(r,b-r)=\text{true}$，则 $W=\text{false}$，直接取 $k=q$（后继 $\{r,b\}$ 让对手输）即赢，定义值 $=\text{true}$。✓
    - 若 $\text{win}(r,b-r)=\text{false}$，则 $W=\text{true}$。当前玩家取 $k=q-1$，后继 $\{b+r,b\}$。因 $r<b$ 有 $b+r<2b$，走公式第三项：$\text{win}(b+r,b)=\lnot\,\text{win}(b,r)=\lnot\,W=\text{false}$，故取 $k=q-1$ 即赢，定义值 $=\text{true}$。✓

  两个子情况都给出至少一个让对手输的 $k$，故定义值 $=\text{true}$，与公式第二项一致。✓

三情况均一致，归纳完成，引理成立。$\blacksquare$

> **一句话记忆**：
> - $a\ge 2b$：**有选择权**，且总能挑到 $k$ 把对手送进"输"的局面 → 当前玩家**必赢**。
> - $b<a<2b$：**没得选**，只能 $a\to a-b$，胜负翻转交给 $\text{win}(b,a-b)$。
> - $b=0$：轮到的人**已输**。

### 第 4 步：跑起来（递归即可）

引理给出的判定规则本身就是递归，且每走一步"$b<a<2b$"分支后，大数从 $a$ 变成 $b<a$（严格变小）；走"$a\ge 2b$"分支则立即终止。所以**递归深度 $= O(\log\max(a,b))$**（类似欧几里得算法），$\max<2^{31}$ 时不超过约 $64$ 层，直接递归完全安全，无需额外数据结构。

> 一个等价的小优化：在"$b<a<2b$"分支里 $(a,b)\to(b,a-b)$，连续多次"$a<2b$"其实等价于"取模 $a\bmod b$"。但直接写 $a-b$ 更不容易出错，且层数已经够少，**本文代码就用直接递归**。

### 第 5 步：验证（手工 + 小数据）

**样例 $(25,7)$，Stan 先手**，求 $\text{win}(25,7)$：
- $25\ge 2\cdot7=14$ ✓（情况 C）→ $\text{win}(25,7)=\text{true}$ → **Stan 赢** ✓（与题意一致）。

**$(3,2)$**：
- $3<4=2\cdot2$（情况 B）→ $\lnot\,\text{win}(2,1)$。
- $\text{win}(2,1)$：$2\ge 2$（情况 C）→ $\text{true}$。
- 回代：$\text{win}(3,2)=\lnot\,\text{true}=\text{false}$ → **Ollie 赢**。
- 验证：Stan 面对 $(3,2)$ 被迫 $3-2=1\to(1,2)$，Ollie 面对 $(2,1)$ 取 $k=2$ 归零赢 ✓。

**$(1,1)$**：$1<2$（情况 B）→ $\lnot\,\text{win}(1,0)=\lnot\,\text{false}=\text{true}$ → Stan 取 $k=1$ 归零赢 ✓。
**$(2,1)$**：$2\ge2$（情况 C）→ true → Stan 取 $k=2$ 归零赢 ✓。
**$(11,7)$（Ollie 那步）**：$11<14$（B）→ $\lnot\text{win}(7,4)$；$\text{win}(7,4)$：$7<8$（B）→ $\lnot\text{win}(4,3)$；$\text{win}(4,3)$：$4<6$（B）→ $\lnot\text{win}(3,1)$；$\text{win}(3,1)$：$3\ge2$（C）→ true。回代：$\text{win}(4,3)=\text{false}$，$\text{win}(7,4)=\text{true}$，$\text{win}(11,7)=\text{false}$。即 Ollie 面对 $(11,7)$ 时"轮动者输"——但样例里 Ollie 走完后是 Stan 赢，自洽（Ollie 面对的是 $(11,7)$，他输这个子博弈，意味着他走完后 Stan 在 $(4,7)$ 赢，而 $\text{win}(4,7)=\text{win}(7,4)=\text{true}$ ✓）。

所有抽查与规则、与"欧几里得算法 + $a\ge 2b$ 强制赢"的经典结论一致。

---

## Part 3 — 模式总结（Pattern Summary）

- **explore its property — 刻画一步能走到哪**：$a\ge b$ 时只能把 $a$ 变成 $a-kb$（$1\le k\le\lfloor a/b\rfloor$）；$b\mid a$ 时一步归零直接赢。
- **define win-state — 博弈标准递归**：$\text{win}(a,b)$=存在一步走到对手输；终局 $(x,0)$=轮到者输（胜利归上一人）。
- **split by choice — 按"有没有选择"分类**：$\lfloor a/b\rfloor\ge2$（$a\ge2b$）有选择权；$\lfloor a/b\rfloor=1$（$a<2b$）被迫。
- **classic problem solution — 强制赢（$a\ge 2b$）**：有选择权时用 $k=q$ 与 $k=q-1$ 两个候选，证明必有一个让对手输 → 当前玩家必胜。
- **reduce to easier problem — 收缩成欧几里得**：$a<2b$ 唯一后继 $(b,a-b)$，整条路径就是欧几里得算法，深度 $O(\log)$。

---

## Part 4 — 真正解法（Real Solution）

### 记号表（Notation Glossary）

| 符号 | 含义 | 初始化 / 范围 |
| --- | --- | --- |
| $a,b$ | 当前局面两数，维护 $a\ge b\ge 0$ | 输入 $M,N$ 后 $a=\max(M,N),\,b=\min(M,N)$ |
| $\text{win}(a,b)$ | 布尔：轮到该玩家面对 $\{a,b\}$（$a\ge b$）时是否必胜 | 见下递归 |
| $a\bmod b$ | $a$ 除以 $b$ 余数（$b>0$） | — |
| $C$ | 测试组数 | $1\le C\le 6$ |
| $M,N$ | 一组输入 | $1\le M,N<2^{31}$ |

**判定规则（代码核心）：**

$$
\text{win}(a,b)\ (a\ge b)=
\begin{cases}
\text{false}, & b=0,\\
\text{true}, & a\ge 2b,\\
\lnot\,\text{win}(b,\,a-b), & b<a<2b.
\end{cases}
$$

### 算法步骤

1. 读入 $C$；对每组读 $M,N$，令 $a=\max(M,N)$，$b=\min(M,N)$。
2. 计算 $\text{win}(a,b)$：
   - $b=0$ → 返回 `false`；
   - $a\ge 2b$ → 返回 `true`；
   - 否则（$b<a<2b$）→ 返回 $\lnot\,\text{win}(b,a-b)$。
3. $\text{win}(a,b)=\text{true}$ 则 Stan（先手，即面对初始局面的轮动者）赢，输出 `Stan wins`；否则输出 `Ollie wins`。

> 初始局面轮到 Stan，故 $\text{win}(M,N)$ 的返回值**直接**就是"Stan 是否赢"，无需额外换人标记。

### 复杂度

- 时间：每组 $O(\log\max(M,N))$（每步大数严格变小），$\max<2^{31}$ 时 $<64$ 层；$C\le 6$，总量可忽略。
- 空间：递归栈 $O(\log\max(M,N))$。

### 验证状态

- 手工验证样例 $(25,7)\to$ Stan 赢 ✓；$(3,2)\to$ Ollie 赢 ✓；$(1,1),(2,1)\to$ Stan 赢 ✓。
- 与"欧几里得算法 + $a\ge 2b$ 强制赢"经典结论一致。
- 数值 $<2^{31}$，用 `long long` 保证 $2b$ 不溢出 `int`。

---

## Part 5 — 示例代码（C++）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

//win(a,b): 轮到你面对 {a,b}(a>=b) 是否必胜
//b==0: 上一步已有人造 0, 轮到的人输
//a>=2b: 强制赢(可选 k=q 或 k=q-1 把对手送进输局)
//b<a<2b: 只能 a-=b, 胜负交给 ¬win(b,a-b)
bool win(i64 a,i64 b)
{
	if(b==0)
		return false;
	if(a>=2*b)
		return true;
	return !win(b,a-b);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int C;
	cin>>C;
	while(C--)
	{
		i64 M,N;
		cin>>M>>N;
		i64 a=max(M,N),b=min(M,N);
		//初始轮到 Stan, win 返回值即 Stan 是否赢
		cout<<(win(a,b)?"Stan wins\n":"Ollie wins\n");
	}
	return 0;
}
```

> **实现要点**：
> - `a>=2*b` 用 `long long` 比较，避免 $b$ 接近 $2^{31}$ 时 $2b$ 溢出 `int`。
> - 递归深度 $O(\log)$，安全；追求绝对保险可改成 while 迭代（逻辑完全等价）。
> - `win` 返回"面对该局面的轮动者"是否赢；初始轮到 Stan，直接映射到输出。
