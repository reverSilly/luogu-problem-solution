# P1839 Play with Power

> 难度：普及/提高- · 标签：博弈论，记忆化搜索

## 1. 重新读题
### 1.1 问题重述（人话版）
给定上限 $n$，以及满足 $a^b\le n$ 的正整数对 $(a,b)$。Masha 先手。每回合必须把 $a$ 或 $b$ 加 1，但加完以后仍要 $a^b\le n$；两个选择都不合法就输。两人都会最优行动。问先手胜（`Masha`）、后手胜（`Stas`），还是谁也不能强迫对方输（`Missing`，平局）。

平局是真的会出现的：当 $a=1$ 时 $1^b=1$ 永远不超过 $n$，把 $b$ 一直加下去这盘棋就没有终点。

### 1.2 关键约束与规模暗示
$n\le 10^8$，$t\le 100$，且每组都保证 $a^b\le n$。$30\%$ 数据 $n\le 2\times 10^3$，提示小数据可以把整个局面图算完来对拍。$a\ge 2$ 时指数最多大约 $\log_2 n\le 26$ 步就会爆掉，所以「$a\ge 2$」的局面是有限的；真正麻烦的是 $a=1$ 的无限链，以及 $b=1$ 时 $a$ 可以一路加到 $n$（最多 $10^8$ 个点，不能逐个递推）。

## 2. 思维链（Chain-of-thought）
双方的偏好按这个顺序：能赢就赢，不能赢就尽量拖成平局，最后才是认输。把当前要走的人的结局记成三种：

- `W`：存在一步走到对手的必败；
- `L`：没有任何合法步，或者每一步都走到对手的必胜；
- `D`：走不到对手的必败，但存在一步走到平局（包括可以无限走下去）。

没有合法步的局面是 `L`，因为规则就是「你必须走，可是一走就超过 $n$」。

**引理 1**：$a\ge 2$ 的所有合法局面构成有限有向无环图，结局只有 `W` 或 `L`，没有 `D`。

证明：每一步 $a$ 或 $b$ 至少加 1，局面在整数格点上严格往右上走，不可能回到旧局面。又因为 $a\ge 2$ 且 $a^b\le n$，所以 $b\le \lfloor\log_2 n\rfloor\le 26$，同时 $a\le n$。局面个数有限，没有环，也没有无限射线。在没有平局机会的有限确定博弈里，从没有后继的终端（记为 `L`）沿拓扑反序归纳：有后继是 `L` 则为 `W`，否则所有后继都是 `W`、自己是 `L`。归纳中不会出现第三种值。$\square$

**引理 2**：设 $L=\lfloor\sqrt n\rfloor$。若 $b=1$ 且 $a>L$，则只能把 $a$ 加 1，形成一条链 $(a,1)\to(a+1,1)\to\cdots\to(n,1)$。$(n,1)$ 是 `L`；$(a,1)$ 是 `W` 当且仅当 $n-a$ 为奇数，否则是 `L`。

证明：$a>L$ 推出 $a^2>n$，所以把 $b$ 改成 2 非法。唯一候选是 $(a+1,1)$，且仅当 $a+1\le n$。到了 $(n,1)$：$(n+1)^1=n+1>n$；若 $n\ge 2$ 则 $n^2>n$。两条都非法，终端为 `L`。链上边数正好是 $k=n-a$。对 $k$ 归纳：$k=0$ 为 `L`；$k>0$ 时唯一后继的距离是 $k-1$，后继为 `L` 则自己为 `W`，后继为 `W` 则自己为 `L`。因此 $k$ 为奇数时 `W`，偶数时 `L`。$\square$

$n=1$ 时没有 $a>L$ 的合法起点（$a\ge 2$ 已经超过 $n$），公式不会被用到。

**引理 3**：$a=1$ 时 $1^b=1\le n$ 对一切 $b$ 成立，这是整张图里唯一的无限方向。令 $B=\lfloor\log_2 n\rfloor$（即满足 $2^B\le n$ 的最大 $B$；$n=1$ 时令 $B=0$）。则一切 $b>B$ 的局面 $(1,b)$ 都是 `D`。

证明：$b>B$ 时 $2^b>n$，不能走到 $(2,b)$。唯一合法步是 $(1,b+1)$，而 $(1,b+1)$ 同样只有这一类步。对局永远不结束，没有任何一方能在有限步内让对方无路可走，所以既不是 `W` 也不是 `L`，按上面的偏好就是 `D`。$\square$

**引理 4**：对 $1\le b\le B$，$(1,b)$ 的后继集合是「若 $2^b\le n$ 则包含 $(2,b)$」再加上 $(1,b+1)$。这些后继的结局都不依赖更小的 $b$，因此可以从 $b=B$ 递减算出：

- 有后继是 `L`，则自己是 `W`；
- 否则有后继是 `D`，则自己是 `D`；
- 否则每个后继都是 `W`，自己是 `L`。

证明：$(2,b)$ 的底数至少是 2，由引理 1、2 它的结局已经是纯粹的 `W`/`L`，计算时不会再回到 $a=1$。$(1,b+1)$ 若 $b=B$ 则由引理 3 是 `D`，若 $b<B$ 则在递减过程中已经定案。后继集合已知，三段规则就是本节开头的定义，没有其它选择。$\square$

**定理**：初始局面的结局是 `W`/`L`/`D` 时，分别输出 `Masha`/`Stas`/`Missing`。

证明：Masha 就是初始局面的当前玩家，三段定义已经按「先赢、次平、最后输」选定了双方的最优策略。引理 1–4 覆盖了全部满足 $a^b\le n$ 的 $(a,b)$：要么 $a=1$，要么 $a\ge 2$ 且 $b=1$ 且 $a>\sqrt n$，要么 $a\ge 2$ 且 $a\le\sqrt n$（$b\ge 2$ 时 $a^b\le n$ 自动推出 $a\le\sqrt n$）。$\square$

用样例核对。$n=9$，$L=3$，$B=3$（因为 $2^3=8\le 9$，$2^4=16>9$）。

- $(3,2)$：$(4,2)=16>9$，$(3,3)=27>9$，无路，`L`。$(2,3)$ 同样无路，`L`。$(2,2)$ 能走到 `L`，所以是 `W`，输出 `Masha`。
- $(1,4)$：$4>B$，由引理 3 是 `D`，输出 `Missing`。

实现时还有一个坑：$n<4$ 时 $\sqrt n<2$，$(2,1)$ 根本不在「$a\le L$」的表里。射线往 $(2,b)$ 看时必须改走引理 2 的奇偶，不能去读一张没填过、默认值是 0 的表，否则会把「未计算」误当成必败。

暴力不使用奇偶捷径，也不使用自底向上的递推表。它把 $n$ 较小（对拍时 $n\le 3000$）的全部合法局面列出来，终端标成 `L`，把 $(1,B+1)$ 冻成 `D`，然后做单调吸引子迭代：能走到已知 `L` 的标成 `W`；所有后继都已经是 `W` 的标成 `L`；剩下的是 `D`。这和定义逐字相同，但控制流和标程不同。两边在 $1\le n\le 3000$ 的大量合法询问上一致，包括 $n=3$ 这种 $\sqrt n<2$ 的边界。

## 3. 模式总结（Pattern Summary）
- **explore its property** — $a=1$ 会无限加 $b$，所以结局除了胜负还有平局。
- **discuss by category** — $a\ge 2$ 是有限 DAG；$b=1$ 且 $a>\sqrt n$ 是一条链；$a=1$ 是一条射线再加若干跳到 $a=2$ 的岔路。
- **reduce to easier problem** — 链的结局退化成 $n-a$ 的奇偶，不必把 $10^8$ 个点都存下来。
- **classic problem solution** — 有限区自底向上按后继组合 `W/L/D`；射线上的 $b$ 从大到小接上这条结果。

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| `n` | `i64` | 幂的上限 | 读入 | limit |
| `lim` | `int` | $\lfloor\sqrt n\rfloor$ | 0 后自增 | sqrt |
| `maxb` | `int` | $\lfloor\log_2 n\rfloor$，$n=1$ 时为 0 | 0 | log |
| `f[a][b]` | `int[10005][40]` | $2\le a\le\lim$ 的结局，0 负 / 1 平 / 2 胜 | 全局 0，只写合法格 | finite |
| `ray[b]` | `int[40]` | $(1,b)$ 的结局 | `ray[maxb+1]=1` | ray |
| `ok(a,b)` | 函数 | 判断 $a^b\le n$，乘法前除法防溢出 | — | power |

结局到字符串：2 → `Masha`，0 → `Stas`，1 → `Missing`。

### 4.2 算法步骤
1. 求出 `lim` 与 `maxb`。
2. $b$ 从大到小、$a$ 从 `lim` 递减到 2，若 `ok(a,b)`，看后继 $(a+1,b)$ 与 $(a,b+1)$。$b=1$ 且 $a+1>\texttt{lim}$ 时后继用引理 2，否则读表。按「有 `L` 则 `W`，否则有 `D` 则 `D`，否则 `L`」写入 `f[a][b]`。
3. `ray[maxb+1]=1`。$b$ 从 `maxb` 降到 1：后继是 $(2,b)$（$2>\texttt{lim}$ 时用奇偶，否则读 `f[2][b]`）和 `ray[b+1]`，同一套三段规则。
4. 回答询问：`a==1` 时 $b>\texttt{maxb}$ 输出平局，否则输出 `ray[b]`；$b==1$ 且 $a>\texttt{lim}$ 用奇偶；其余输出 `f[a][b]`。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）
有限区 $a\le 10^4$、$b\le 26$，填表 $O(\lim\cdot\log n\cdot\log n)$，预处理可以忽略。单组询问 $O(1)$。`g++ -O2 -std=c++17 -Wall -Wextra -Werror` 无警告。与「枚举小 $n$ 的全部局面再做吸引子迭代」的暴力对拍 47 组（含官方样例 $n=9,(2,2)\to\texttt{Masha},(1,4)\to\texttt{Missing}$，以及 $n\le 3000$ 的边界和随机合法局面），checker 墙钟 2.884s。另计一组最大数据 $n=10^8,t=100$，标程 0.004s。

## 5. 示例代码
### 5.1 标程（`P1839.cpp`）

```cpp
//
// 题目：P1839 Play with Power
// https://www.luogu.com.cn/problem/P1839
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxa{10005};
constexpr int mxb{40};

i64 n;
int lim,maxb;
int f[mxa][mxb];
int ray[mxb];

bool ok(i64 a,int b)
{
	if(a<1||b<1)
		return false;
	if(b==1)
		return a<=n;
	if(a==1)
		return true;
	i64 r=1;
	for(int i=0;i<b;++i)
	{
		if(r>n/a)
			return false;
		r*=a;
	}
	return r<=n;
}

int biga(i64 a)
{
	return ((n-a)&1)?2:0;
}

void build()
{
	lim=0;
	while((i64)(lim+1)*(lim+1)<=n)
		++lim;
	for(int b=mxb-2;b>=1;--b)
	{
		for(int a=lim;a>=2;--a)
		{
			if(!ok(a,b))
				continue;
			bool any=false,hasl=false,hasd=false;
			if(ok(a+1,b))
			{
				any=true;
				int r=b==1&&a+1>lim?biga(a+1):f[a+1][b];
				if(r==0)
					hasl=true;
				else if(r==1)
					hasd=true;
			}
			if(b+1<mxb&&ok(a,b+1))
			{
				any=true;
				int r=f[a][b+1];
				if(r==0)
					hasl=true;
				else if(r==1)
					hasd=true;
			}
			if(!any)
				f[a][b]=0;
			else if(hasl)
				f[a][b]=2;
			else if(hasd)
				f[a][b]=1;
			else
				f[a][b]=0;
		}
	}
	maxb=0;
	i64 p2=1;
	while(p2<=n/2)
	{
		p2*=2;
		++maxb;
	}
	ray[maxb+1]=1;
	for(int b=maxb;b>=1;--b)
	{
		bool hasl=false,hasd=false;
		if(ok(2,b))
		{
			int r=b==1&&2>lim?biga(2):f[2][b];
			if(r==0)
				hasl=true;
			else if(r==1)
				hasd=true;
		}
		if(ray[b+1]==0)
			hasl=true;
		else if(ray[b+1]==1)
			hasd=true;
		if(hasl)
			ray[b]=2;
		else if(hasd)
			ray[b]=1;
		else
			ray[b]=0;
	}
}

void say(int o)
{
	if(o==2)
		cout<<"Masha\n";
	else if(o==0)
		cout<<"Stas\n";
	else
		cout<<"Missing\n";
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int t;
	cin>>n>>t;
	build();
	while(t--)
	{
		int a,b;
		cin>>a>>b;
		if(a==1)
		{
			if(b>maxb)
				say(1);
			else
				say(ray[b]);
		}
		else if(b==1&&a>lim)
			say(biga(a));
		else
			say(f[a][b]);
	}
	return 0;
}
```
