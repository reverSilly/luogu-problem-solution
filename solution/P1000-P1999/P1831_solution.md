# P1831 [ICPC 2010 Chengdu R] 杠杆数

> 难度：NOI/NOI+ · 标签：数位 DP、枚举

## 1. 重新读题

### 1.1 问题重述（人话版）

称一个正整数是「杠杆数」，如果存在它的某一位作为支点，使得

$$
\sum_{\text{支点左边的数字 }d_i}d_i\cdot(\text{该位到支点的距离})
=\sum_{\text{支点右边的数字 }d_i}d_i\cdot(\text{该位到支点的距离}).
$$

给定 $[x,y]$，求其中杠杆数的个数。例如 $4139$ 以 $3$ 为支点：左边 $4\times2+1\times1=9$，右边 $9\times1=9$，所以它是杠杆数。

### 1.2 关键约束与规模暗示

- $1\le x\le y\le 10^{18}$。$10^{18}$ 是 19 位数，因此数位长度 $L\le 19$。
- 40% 的数据 $y\le x+10^5$，可以逐个枚举再 $O(L^2)$ 检验；但满分必须做到 $O(L^3\cdot\text{力矩范围})$ 级别的数位 DP。
- 力矩总和的绝对值上限：$9\sum_{i}|p-i|\le 9\cdot\frac{18\cdot19}{2}=1539$，状态空间很小。
- 答案可达 $10^{16}$ 量级（$1\sim10^{18}$ 中约 $1.26\times10^{16}$ 个杠杆数），必须用 64 位整数。

## 2. 思维链（Chain-of-thought）

### 2.1 先写最直接的判定，确认它只能拿部分分

对一个数，枚举支点 $p$，把左边每位的「数字 × 距离」加上、右边同样加上，相等即为杠杆数。单次判定 $O(L^2)$（$L\le19$），用来做 $y-x\le10^5$ 的子任务是够的。但满分区间长度 $10^{18}$，逐个判定完全不可能。

此时自然的想法是：**把「枚举数」换成「枚举数位」**。支点位置只有 19 种，可以先固定支点，再数「满足该支点平衡」的数有多少个。

### 2.2 固定支点后的判定：把它写成一个带符号的力矩和

设把 $x$ 写成恰好 $L=19$ 位（不足补前导零），第 $i$ 位数字为 $d_i$（$i=0$ 是最高位，$i=18$ 是个位）。固定支点 $p$，定义

$$
T_p(x)=\sum_{i<p}d_i\,(p-i)\;-\;\sum_{i>p}d_i\,(i-p).
$$

即：左边的贡献取正，右边的贡献取负，支点这一位系数为 $0$。显然

$$
x\text{ 以 }p\text{ 为支点时平衡}\iff T_p(x)=0.
$$

补前导零不改变 $T_p$ 的值（零位贡献为 $0$），也不改变非零数字之间的相对距离，所以「19 位补零表示」与「按实际位数计算」是等价的。于是可以放心地在固定长度的数字串上做数位 DP。

### 2.3 关键观察：一个数至多只有一个平衡支点

**引理 1（相邻支点的力矩差等于数位和）**

设 $S=\sum_{i=0}^{18}d_i$ 为各位数字之和，则对任意 $0\le p<18$，

$$
T_{p+1}(x)-T_p(x)=S .
$$

**证明**：直接展开。

$$
\begin{aligned}
T_{p+1}(x)
&=\sum_{i<p+1}d_i(p+1-i)-\sum_{i>p+1}d_i(i-p-1)\\
&=\sum_{i<p}d_i(p-i)+\sum_{i\le p}d_i-\sum_{i>p+1}d_i(i-p)+\sum_{i>p+1}d_i,\\
T_{p}(x)
&=\sum_{i<p}d_i(p-i)-\sum_{i>p}d_i(i-p)\\
&=\sum_{i<p}d_i(p-i)-d_{p+1}-\sum_{i>p+1}d_i(i-p).
\end{aligned}
$$

两式相减，中间两对求和项抵消，剩下

$$
T_{p+1}-T_p=\sum_{i\le p}d_i+d_{p+1}+\sum_{i>p+1}d_i=S .
$$

$\blacksquare$

**推论 1（支点唯一）**：对 $x>0$，$S>0$，故 $T_0<T_1<\cdots<T_{18}$ 严格递增。因此 $T_p=0$ 的 $p$ 至多一个：一个正数至多有一个平衡支点。

推论 1 非常关键。它意味着：

- 我们可以**分别对 19 个支点计数，然后直接把结果相加**，不需要处理「同一个数被多个支点重复统计」的容斥问题。
- 唯一被重复统计的数是 $0$：它每一位都是 $0$，对任何 $p$ 都有 $T_p=0$，会被数 19 次。但在计算区间答案 $F(y)-F(x-1)$ 时这 19 次会精确抵消，故无需特殊处理。

> **失败尝试的极简反例**：如果没有推论 1，就必须处理去重。比如若某个数有两个平衡支点，直接相加会把它算两次。我们花时间验证了「不会发生」——证明就是上面的严格单调性，而不是靠样例猜。

### 2.4 数位 DP：固定支点，统计 $T_p=0$ 的数的个数

现在问题变成：对每个 $p$，统计 $0\le x\le N$ 中满足 $T_p(x)=0$ 的 $x$ 的个数。这是标准的「数位 DP / 逐位确定」：

- 从左到右填第 $0,1,\dots,18$ 位；
- 状态：当前位 `pos`、当前力矩和 `sum`、是否贴着上界 `tight`；
- 转移：枚举这一位填 $d\in[0,\text{up}]$，其中 `up=9`（不贴紧）或 `up=dig[pos]`（贴紧）；权重

$$
w(i,p)=\begin{cases}
p-i,& i<p,\\
0,& i=p,\\
-(i-p),& i>p,
\end{cases}
\qquad \text{新状态 }sum'=sum+d\cdot w(i,p).
$$

- 边界：填完 19 位后，`sum==0` 则记 $1$。

正确性：每个 $0\le x\le N$ 都对应唯一的 19 位补零数字串，而 DP 枚举了所有满足字典序不超过 $N$ 的数字串，恰好一次。$sum$ 的取值被限制在 $[-1539,1539]$，加偏移量后用数组记忆化即可。`tight=false` 的状态与上界无关，可以记忆化复用；`tight=true` 的状态只有 $O(L)$ 个，不必记忆。

### 2.5 合并答案

设 $F(N)=\sum_{p=0}^{18}\#\{0\le x\le N:T_p(x)=0\}$。由推论 1，$F(N)$ 中每个 $[1,N]$ 内的杠杆数恰好出现一次，$0$ 出现 19 次；于是

$$
\#\{\text{杠杆数}\in[x,y]\}=F(y)-F(x-1),
$$

$0$ 的 19 次贡献在相减时抵消。当 $x=1$ 时 $F(0)=19$，公式依然成立（`calc(0)` 会返回 19）。

### 2.6 复杂度

位数 $L=19$，支点 19 个，力矩状态约 $2\times1539+1\approx3079$ 个。每个状态枚举 10 个数字：

$$
O(L\cdot L\cdot 3079\cdot 10)\approx1.2\times10^7
$$

次转移，实际运行在毫秒级。记忆化数组 $19\times3405$ 个 `long long`，约 $517\text{ KB}$。

### 2.7 一个隐蔽的溢出坑

记忆化里存的是「方案数」，量级可以达到 $10^{18}$（例如 $N=10^{18}$ 时前缀计数约 $1.26\times10^{16}$）。如果 `memo` 用 `int`，在 $N$ 很大时会被截断，得到完全错误的结果；小数据对拍（$N\le2\times10^5$）却查不出来。所以 `memo` 必须用 `long long`（$19\times3405\times8\text{ B}\approx517\text{ KB}$，仍然很小）。

## 3. 模式总结（Pattern Summary）

- **consider easier version** — 先写 $O(\Delta\cdot L^2)$ 的逐个判定，拿到短区间子任务，并确认「枚举数」不可行。
- **explore its property** — 固定支点，把判定写成带符号力矩和 $T_p(x)=0$。
- **explore its property** — 证明 $T_{p+1}-T_p$ 恒等于数位和 $S>0$，得到「正数至多一个平衡支点」，从而各支点计数可直接相加。
- **classic problem solution** — 对每个支点做数位 DP：状态 `(pos, 力矩和, tight)`，统计终点力矩为 $0$ 的方案数。
- **reduce to easier problem** — 区间答案化为前缀 $F(y)-F(x-1)$，$0$ 的重复计数自动抵消。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `x, y` | `i64` | 询问区间端点 | 读入 | $\le10^{18}$ |
| `maxL` | `constexpr int` | 固定数位长度 $19$ | 常量 | $10^{18}$ 是 19 位 |
| `dig[19]` | `int [19]` | $N$ 的 19 位补零数字串，`dig[0]` 最高位 | 由 `N` 逐位取出 | digits |
| `pv` | `int` | 当前枚举的支点位置 | $0\dots18$ | pivot |
| `sum` | `int` | 当前累计力矩（带符号） | $0$，范围 $[-1539,1539]$ | torque sum |
| `off, mxw` | `constexpr int` | 偏移量 $1700$、数组宽度 $3405$ | 常量 | 把负下标平移到 $[0,3405)$ |
| `memo[pos][sum+off]` | `i64 [19][3405]` | 不贴紧状态的方案数记忆 | 每个支点前重置为 $-1$ | memoization |
| `calc(N)` | 函数返回 `i64` | $F(N)=\sum_p\#\{0\le x\le N:T_p=0\}$ | — | 前缀计数 |

### 4.2 算法步骤

1. 读入 `x,y`。
2. 实现 `dfs(pos,sum,tight,pv)`：
   - `pos==19` 时返回 `sum==0`；
   - 若 `!tight` 且 `memo[pos][sum+off]` 已算过则直接返回；
   - `up = tight ? dig[pos] : 9`，枚举 `d=0..up`，按 `w(pos,pv)` 更新 `sum`，递归；
   - 对 `!tight` 的结果写回 `memo`。
3. 实现 `calc(N)`：若 `N<0` 返回 $0$；否则把 `N` 拆成 19 位存入 `dig`；对 `pv=0..18`，清空 `memo`，累加 `dfs(0,0,1,pv)`；返回总和。
4. 输出 `calc(y)-calc(x-1)`。

### 4.3 复杂度

- 时间：$O(19\cdot19\cdot3405\cdot10)\approx1.2\times10^7$ 次转移。
- 空间：`memo` 约 $517\text{ KB}$。

## 5. 示例代码

```cpp
//
// 题目：P1831 [ICPC 2010 Chengdu R] 杠杆数
// https://www.luogu.com.cn/problem/P1831
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int maxL{19};
constexpr int off{1700};
constexpr int mxw{3405};

i64 x,y;
int dig[maxL];
i64 memo[maxL][mxw];

i64 dfs(int pos,int sum,int tight,int pv)
{
	if(pos==maxL)
		return sum==0;
	if(!tight&&memo[pos][sum+off]>=0)
		return memo[pos][sum+off];
	int up=tight?dig[pos]:9;
	i64 res{0};
	for(int d=0;d<=up;++d)
	{
		int ns=sum;
		if(pos<pv)
			ns+=d*(pv-pos);
		else if(pos>pv)
			ns-=d*(pos-pv);
		res+=dfs(pos+1,ns,tight&&(d==up),pv);
	}
	if(!tight)
		memo[pos][sum+off]=res;
	return res;
}

i64 calc(i64 N)
{
	if(N<0)
		return 0;
	for(int i=maxL-1;i>=0;--i)
	{
		dig[i]=(int)(N%10);
		N/=10;
	}
	i64 res{0};
	for(int pv=0;pv<maxL;++pv)
	{
		fill_n(&memo[0][0],maxL*mxw,-1);
		res+=dfs(0,0,1,pv);
	}
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>x>>y;
	cout<<calc(y)-calc(x-1)<<'\n';
	return 0;
}
```
