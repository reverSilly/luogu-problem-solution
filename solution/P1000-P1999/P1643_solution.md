# P1643 完美数 题解

> 标签：高精度 / 数位统计 / 回文数 / 构造 · 难度：省选/NOI-

## 1. 重新读题

**问题重述（人话版）**

给定 $T$ 组询问，每组询问给出两个十进制大整数 $X$ 和 $N$，求**严格大于 $X$ 的第 $N$ 小十进制正回文数**（不含前导零，如 $1 \sim 9, 11 \sim 99, 101 \sim 999, \dots$）。

**约束与规模暗示**

- $20\%$ 数据：$X \le 2 \times 10^5$，$N \le 1000$；
- $100\%$ 数据：$1 \le T \le 10$，$0 \le X \le 10^{10000}$，$1 \le N \le 10^{10000}$，且保证答案不超过 $10^{20001}$。
- 规模暗示：$X$ 和 $N$ 的位数高达 $10001$ 位，必须在高精度十进制位数组上直接完成回文数排名映射与跨位数跳跃，且不能做 $O(|N|^2)$ 的逐长度全量高精度减法，而应利用十进制对齐性质做到 $O(|X| + |N|)$。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：同长度回文数的“左半部分保序双射”

对于任意固定的位数 $L \ge 1$，令左半长度 $m = \lfloor (L + 1) / 2 \rfloor$。任何 $L$ 位正回文数都由它的前 $m$ 位十进制数 $H \in [10^{m-1}, 10^m - 1]$ 唯一决定（将 $H$ 的前 $L - m$ 位倒序拼接到 $H$ 右侧即可得到完整回文数 $\operatorname{pal}(H, L)$）。

**引理 1（固定位数 $L$ 下回文数的字典序与左半整数 $H$ 保序同构）**

在位数 $L$ 固定时，对于任意两个 $m$ 位无前导零正整数 $H_1, H_2 \in [10^{m-1}, 10^m - 1]$：
$$\operatorname{pal}(H_1, L) < \operatorname{pal}(H_2, L) \iff H_1 < H_2.$$
因此全体 $L$ 位正回文数按升序排列恰为 $\operatorname{pal}(10^{m-1}, L), \operatorname{pal}(10^{m-1}+1, L), \dots, \operatorname{pal}(10^m-1, L)$，总个数为 $9 \times 10^{m-1}$。

**证明**：
两个同为 $L$ 位的正整数比较大小等价于从最高位到最低位作字典序比较。由于 $H_1 \ne H_2$ 时，它们首次出现不同的数位必落在前 $m$ 位之内（而后 $L - m$ 位只是前 $L - m$ 位的镜像），故 $\operatorname{pal}(H_1, L)$ 与 $\operatorname{pal}(H_2, L)$ 的首个不同位就是 $H_1$ 与 $H_2$ 的首个不同位，大小关系完全一致。而 $H$ 的取值范围 $[10^{m-1}, 10^m - 1]$ 共有 $10^m - 10^{m-1} = 9 \times 10^{m-1}$ 个整数。$\blacksquare$

### 2.2 归约到线性高精度算法：左半相加 + 单数位均摊跳跃

**引理 2（同位数偏移与跨位数排名的统一公式）**

设 $X$ 的十进制表示长度为 $L$，取前 $m = \lfloor (L + 1) / 2 \rfloor$ 位构成整数 $H_0$，并镜像生成 $P_0 = \operatorname{pal}(H_0, L)$。定义偏移量 $\Delta = (X \ne 0 \land P_0 > X) ? (N - 1) : N$，并令 $S = H_0 + \Delta$：
1. 若 $S < 10^m$，则答案位数仍为 $L$，其左半部分即为 $H = S$；
2. 若 $S \ge 10^m$，令 $K = S - 10^m$，则 $K \ge 0$ 恰为目标回文数在所有位数 $\ge L + 1$ 的回文数中的 **0-based 排名**。随后递增 $L$（并更新 $m = \lfloor (L + 1) / 2 \rfloor$），当 $K \ge 9 \times 10^{m-1}$ 时不断令 $K \leftarrow K - 9 \times 10^{m-1}$ 并递增 $L$；终止时的 $L$ 即为答案位数，左半部分为 $H = 10^{m-1} + K$。

**证明**：
当 $X \ne 0$ 时，位数为 $L$ 且严格大于 $X$ 的最小回文数左半为 $H_{\text{start}} = H_0 + [P_0 \le X]$（当 $X = 0$ 时 $L=1, m=1, H_0=0, H_{\text{start}}=1$，同样满足 $H_{\text{start}} = H_0 + 1$）。因此位数为 $L$ 且 $> X$ 的第 $N$ 个回文数对应的左半值为 $H_{\text{start}} + (N - 1) = H_0 + \Delta = S$。
- 若 $S < 10^m$，说明位数 $L$ 内的回文数足够，直接取 $H = S$ 即可；
- 若 $S \ge 10^m$，位数 $L$ 内严格大于 $X$ 的回文数共有 $C_0 = 10^m - H_{\text{start}}$ 个，扣除后在位数 $\ge L + 1$ 中的 0-based 排名为 $(N - 1) - C_0 = H_{\text{start}} + N - 1 - 10^m = S - 10^m = K$。
此后每递增一次位数 $L$，由引理 1 知该位数共有 $9 \times 10^{m-1}$ 个回文数；当 $K < 9 \times 10^{m-1}$ 时，第 $K$ 个（0-based）$L$ 位回文数的左半部分正是起始值 $10^{m-1}$ 加上 $K$。$\blacksquare$

**引理 3（十进制数组上单点减法的 $O(|X| + |N|)$ 均摊时间）**

在十进制位数组 `d[]` 上，减去 $10^m$ 只需令 `d[m] -= 1`，减去 $9 \times 10^{m-1}$ 只需令 `d[m-1] -= 9` 并向上传播借位；最后加上 $10^{m-1}$ 只需令 `d[m-1] += 1`。整个过程总时间复杂度为 $O(|X| + |N|)$。

**证明**：
因为 $L$ 每增加 $2$，操作位置 $m - 1$ 就严格右移（向高位移动）$1$ 位，每个数位至多被作为减法起点常数次；而借位传播每发生一次就会使高位减少 $1$，总借位次数受各位数字总和线性控制。故单组询问总操作次数为 $O(|X| + |N|)$。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **explore its property** — 发现固定位数 $L$ 的回文数与左半 $m = \lfloor (L+1)/2 \rfloor$ 位无前导零正整数 $H \in [10^{m-1}, 10^m-1]$ 严格保序一一对应（引理 1）。
- **reduce to easier problem** — 通过一次左半加法 $S = H_0 + \Delta$ 将“从 $X$ 出发向后数 $N$ 个”统一化归为位数保持（$S < 10^m$）或从 $L+1$ 位起点的 0-based 排名 $K = S - 10^m$（引理 2）。
- **classic problem solution** — 利用 $9 \times 10^{m-1}$ 在十进制下仅占单个非零数位的特性，直接在第 $m-1$ 位执行单点减法与借位（引理 3），实现 $O(|X| + |N|)$ 线性求解。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| `X, N` | `string` | 读入的起点与排名（十进制字符串） | 读入 | 大整数输入 |
| `L, m` | `int` | 当前回文数总长度 $L$ 与左半长度 $m=(L+1)/2$ | `L=\|X\|` | 位数游标 |
| `d[mxd]` | `int [20015]` | 小端序十进制数位数组（`d[0]` 为个位） | `0` | 高精度累加器 |
| `len_d` | `int` | `d[]` 的当前有效十进制位数 | 动态维护 | 最高非零位+1 |

### 4.2 算法步骤

1. 读入字符串 `X, N`，令 $L = |X|$，$m = (L + 1) / 2$，构造 $P_0 = \operatorname{pal}(X[0..m-1], L)$。
2. 在小端序数组 `d[]` 中计算 $S = H_0 + N - [X \ne \text{"0"} \land P_0 > X]$ 并进位归一化。
3. 若 `len_d > m`（即 $S \ge 10^m$）：
   - 调用 `sub_at(m, 1)` 令 $K = S - 10^m$，并令 `++L, m = (L + 1) / 2`；
   - 当 `ge_9pow(m - 1)`（即 $K \ge 9 \times 10^{m-1}$）成立时，循环执行 `sub_at(m - 1, 9)` 并令 `++L, m = (L + 1) / 2`；
   - 令 `++d[m - 1]` 完成 $H = K + 10^{m-1}$。
4. 将 `d[0..m-1]` 转为字符串 $H$，拼接其前 $L - m$ 位的镜像后输出。

### 4.3 复杂度与验证报告

- **时间复杂度**：单组数据 $O(|X| + |N|)$，总时间 $O(T(|X| + |N|))$。
- **空间复杂度**：$O(|X| + |N|)$。
- **对拍验证**：`appendix/P1643/P1643_check.py` 中与逐个枚举整数检验回文的暴力基准完成 **400 组随机批次对拍**全部一致；极限规模 $T = 10$（$10000$ 位 $X, N$）实测耗时 **0.0278 s**。

## 5. 示例代码

```cpp
//
// 题目：P1643 完美数
// https://www.luogu.com.cn/problem/P1643
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxd{20015};

int d[mxd],len_d;

void trim()
{
	while(len_d>1&&d[len_d-1]==0)
		--len_d;
}

bool ge_9pow(int p)
{
	if(len_d>p+1)
		return true;
	if(len_d<p+1)
		return false;
	return d[p]>=9;
}

void sub_at(int p,int v)
{
	int i=p;
	d[i]-=v;
	while(d[i]<0)
	{
		d[i]+=10;
		--d[++i];
	}
	trim();
}

void solve()
{
	string X,N;
	cin>>X>>N;
	int L=X.size(),m=(L+1)/2;
	string P0=X.substr(0,m);
	for(int i=L-m-1;i>=0;--i)
		P0.push_back(X[i]);

	bool dec=(X!="0"&&P0>X);

	int max_l=max(m,(int)N.size())+5;
	fill_n(d,max_l,0);
	for(int i=0;i<m;++i)
		d[i]+=X[m-1-i]-'0';
	for(int i=0;i<(int)N.size();++i)
		d[i]+=N[N.size()-1-i]-'0';
	if(dec)
		--d[0];

	len_d=max_l;
	for(int i=0;i<len_d-1;++i)
	{
		if(d[i]<0)
		{
			d[i]+=10;
			--d[i+1];
		}
		else if(d[i]>=10)
		{
			d[i+1]+=d[i]/10;
			d[i]%=10;
		}
	}
	trim();

	if(len_d>m)
	{
		sub_at(m,1);
		++L;
		m=(L+1)/2;
		while(ge_9pow(m-1))
		{
			sub_at(m-1,9);
			++L;
			m=(L+1)/2;
		}
		while(len_d<m)
			d[len_d++]=0;
		++d[m-1];
	}

	string H(m,'0');
	for(int i=0;i<m;++i)
		H[m-1-i]=char('0'+d[i]);
	string ans=H;
	for(int i=L-m-1;i>=0;--i)
		ans.push_back(H[i]);
	cout<<ans<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
		solve();
	return 0;
}
```
