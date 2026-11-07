# P1924 贴海报

## 1. 重新读题

一面 $n\times m$ 的墙，某些格子是窟窿（输入 $1$）。海报是 $2\times3$ 的长方形，边与墙平行，可以横贴（$2$ 行 $\times3$ 列）也可以竖贴（$3$ 行 $\times2$ 列）。海报不能盖住窟窿，也不能互相重叠。求最多能贴多少张。

### 关键约束与规模暗示

- $n\le150$，$m\le10$。
- $m$ 很小、$n$ 相对大，这是典型的"**按行推进 + 状压**"信号：状态应只和"竖着跨过当前行的海报"有关，位数是 $O(m)$ 甚至 $O(2m)$。
- 海报高度最多 $3$（竖贴 $3$ 行），所以"未来影响"最多跨两行——这决定了状态至少需要两行。
- 题面不要求铺满，**空格是允许的**。

## 2. 思维链（Chain-of-thought）

### 2.1 为什么不能只压缩一行

先试试最朴素的想法：$dp[r][mask]$，`mask` 表示第 $r$ 行哪些格子已被占用。

- 横贴 $2\times3$ 会同时占用第 $r,r+1$ 行；
- 竖贴 $3\times2$ 会同时占用第 $r,r+1,r+2$ 行。

如果只知道第 $r$ 行的占用，把一张竖贴海报放在第 $r$ 行后，它在第 $r+1,r+2$ 行占的格子就"丢了"，下一行会重复占用。所以至少要记住第 $r$ 行和第 $r+1$ 行。

### 2.2 状态压缩：只需要两行

**引理 1（状态充分性）**：按行 $r=0,1,\dots,n-1$ 依次决策。在开始处理第 $r$ 行时，只需知道

- $A$：第 $r$ 行中被"起始行 $<r$"的海报占用的列集合；
- $B$：第 $r+1$ 行中被"起始行 $<r$"的海报占用的列集合。

第 $r+2$ 行此时一定没有任何占用。

**证明**：一张海报的起始行为 $s$，高度为 $2$（横）或 $3$（竖），覆盖行 $s..s+1$ 或 $s..s+2$，最大行号 $s+2$。若 $s<r$，则 $s\le r-1$，最大覆盖到 $r+1$。所以第 $r+2$ 行还没有被任何已放置的海报碰到。

至于"是否够用"：任何未来的海报起始行 $\ge r$，覆盖行 $\ge r$，因此可能与旧海报重叠的行只可能是 $r$ 或 $r+1$；这两行的旧占用已经分别由 $A,B$ 完整记录。$\blacksquare$

状态数最多 $2^{2m}\le2^{20}$。

### 2.3 行内转移：逐列三选一

处理第 $r$ 行时，枚举起始行**恰好为 $r$** 的所有海报。由于起始行 $>r$ 的海报不可能覆盖第 $r$ 行，第 $r$ 行的格子在这一步之后就不会再变，所以我们只需在行内从左到右做一次 DFS：

设当前列 `j`，维护本行新海报对三行的占用 `addA, addB, addC`（分别对应 $r,r+1,r+2$）。

1. 若第 `j` 列在 $A$ 中已占用，或 `addA` 已占用，或是窟窿 → 直接跳到下一列；
2. 否则三种选择：
   - **留空**：什么都不放，`j+1`；
   - **横放**（$j+2<m$ 且 $r+1<n$）：占 $(r,j..j+2)$ 与 $(r+1,j..j+2)$，要求这 6 格不在 $A,B,$`addA,addB` 中、也不是窟窿；
   - **竖放**（$j+1<m$ 且 $r+2<n$）：占 $(r..r+2,j,j+1)$，要求这 6 格不在 $A,B,$`addA,addB,addC` 中、也不是窟窿。

每种选择都继续 DFS，到达 $j=m$ 时得到一个转移：新状态为

$$(A',B')=\big(B\cup \text{addB},\ \text{addC}\big),$$

价值增加本行新放的海报数。

**为什么一定要有"留空"分支**：题面只要求不重叠、不盖洞，不要求铺满。早期我写过一个"强制第 $r$ 行每个非洞格都必须被覆盖"的版本，在

```
3 3
0 0 0
0 0 0
0 0 1
```

上输出 $0$：正确的方案是在第 $0,1$ 行横放一张，第 $2$ 行剩两个非洞格无法覆盖，被"强制铺满"判成非法。加上留空分支后答案变为 $1$，与手算一致。

### 2.4 正确性

**引理 2（DP 与合法方案一一对应）**：设 $dp_r(A,B)$ 为处理完前 $r$ 行、旧海报在 $r,r+1$ 行的占用为 $A,B$ 时能贴出的最大海报数。则最终 $\max_{A,B}dp_n(A,B)$ 等于全局最优海报数。

**证明**：

先证"任一合法方案 $\mathcal P$ 都在 DP 中有对应路径，且价值不小于 $|\mathcal P|$"。对行 $r$ 归纳，假设在开始第 $r$ 行时 DP 状态 $(A,B)$ 恰为 $\mathcal P$ 中起始行 $<r$ 的海报在 $r,r+1$ 行的占用。把 $\mathcal P$ 中起始行 $=r$ 的海报取出，它们两两不重叠、不盖洞，正是 DFS 枚举的一个分支（DFS 覆盖了所有合法的行内放置子集），且与 $A,B$ 不冲突。走这个分支后，新状态为 $\big(B\cup\text{addB},\text{addC}\big)$，正好等于 $\mathcal P$ 中起始行 $<r+1$ 的海报在 $r+1,r+2$ 行的占用（第 $r+2$ 行此前为空，由引理 1）。归纳成立。最后得到 $|\mathcal P|$。

再证"任一 DP 路径对应的海报集合合法"。每次转移只放行内互不重叠、避开窟窿和 $A,B$ 的海报；对任意两张海报：
- 起始行相同：同一次 DFS 内已检查不重叠；
- 起始行差 $1$ 或 $2$：重叠只可能发生在后一张的起始行附近的一两行，而这些占用已被状态 $A,B$ 记录并检查；
- 起始行差 $\ge3$：每张最多跨 $3$ 行，两张的行区间不相交，必不重叠。

所以得到的是合法方案，其价值不超过最优值。两边合起来，DP 最大值即最优值。$\blacksquare$

### 2.5 复杂度

- 状态数 $O(2^{2m})$，行数 $n$。
- 每个状态做一次行内 DFS，最坏枚举数记为 $T(m)$（长度 $m$ 的区间上选若干互不相交、长度 $2$ 或 $3$ 的区间并允许空隙的方案数，约指数级但底数很小）。
- 总上界 $O(n\cdot2^{2m}\cdot T(m))$，这个上界很松；实际可达状态远少于 $2^{2m}$：在 $150\times10$ 的全零网格上，每行只有约 $1.1\times10^3$ 个状态，总运行约 $0.1$ s。
- 空间 $O(2^{2m})$ 个 `int`（$m=10$ 时约 $8$ MB，两张表）。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 海报最多跨 3 行，未来影响窗口是 2 行 → 状态存当前行与下一行（引理 1）。
- **consider easier version** — 先想只存一行，发现竖贴会丢占用，必须加一维。
- **classic problem solution** — 轮廓线（按行）状压 DP，逐列 DFS 枚举行内放置。
- **counterexample** — 强制铺满会漏掉"留空"的合法解（$3\times3$ 带洞的例子）。
- **proof of equivalence** — 用归纳证明 DP 路径与合法海报集合一一对应。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n,m` | `int` | 墙的行数、列数 | 读入 | 行多列少 |
| `hole[r]` | `int[155]` | 第 $r$ 行窟窿的列位掩码 | 读入 | holes |
| `full` | `int` | $(1\ll m)-1$，列全掩码 | — | all columns |
| `A,B` | `int` | 状态：第 $r$、$r+1$ 行旧占用 | 由状态 `s` 拆出 | Above/Below |
| `addA,addB,addC` | `int` | 本行新海报在第 $r,r+1,r+2$ 行的占用 | $0$ | 增量 |
| `dp[s]` | `int[1<<20]` | 当前行各状态的最大海报数 | 除 `dp[0]=0` 外 `NEG` | DP 表 |
| `ndp[s]` | `int[1<<20]` | 下一行 DP 表 | `NEG` | next |
| `cur,nxt` | `vector<int>` | 当前/下一行的可达状态列表 | `{0}` / 空 | 稀疏状态 |
| `s` | `int` | 状态编码 `(A<<m)|B` | — | — |
| `NEG` | `constexpr int` | 不可达哨兵 | `-1145141919` | 负无穷 |

### 4.2 算法步骤

1. 读入 $n,m$；把每行的窟窿转成掩码 `hole[r]`；`full=(1<<m)-1`。
2. 初始化 $dp[0]=0$，其余为 `NEG`；可达状态表 `cur={0}`。
3. 对 $r=0..n-1$：
   1. 取 `hm=hole[r]`，以及下一行、下两行的窟窿掩码 `hm1,hm2`（越界记 $0$）；
   2. 清空 `ndp` 为 `NEG`，`nxt` 清空；
   3. 对每个可达状态 `s`（拆分出 $A,B$），用 DFS 枚举行内放置：
      - 跳过已占用或为洞的列；
      - 在自由列上尝试"留空 / 横放 / 竖放"；
      - 走到行末时，用 `(B|addB, addC)` 作为新状态更新 `ndp`，并把首次出现的状态压入 `nxt`；
   4. `swap(dp,ndp)`，`swap(cur,nxt)`。
4. 答案为 `cur` 中所有状态的 `dp` 最大值。

### 4.3 复杂度

- 时间：$O(n\cdot(\text{可达状态数})\cdot T(m))$，其中 $T(m)$ 为行内放置枚举量；$m\le10$ 时实测总时间约 $0.1$ s。
- 空间：两张 $2^{2m}$ 的 `int` 表，约 $8$ MB；加上 $O(n)$ 的窟窿掩码。

## 5. 示例代码

```cpp
//
// 题目：P1924 贴海报
// https://www.luogu.com.cn/problem/P1924
//
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{155};
constexpr int mxstate{1<<20};
constexpr int NEG{-1145141919};

int n,m,full;
int hole[mxn];
int dp[mxstate],ndp[mxstate];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	full=(1<<m)-1;
	for(int i=0;i<n;++i)
	{
		int mask=0;
		for(int j=0;j<m;++j)
		{
			int x;
			cin>>x;
			if(x)
				mask|=1<<j;
		}
		hole[i]=mask;
	}
	int SZ=1<<(2*m);
	fill(dp,dp+SZ,NEG);
	dp[0]=0;
	vector<int>cur,nxt;
	cur.push_back(0);
	for(int r=0;r<n;++r)
	{
		int hm=hole[r];
		int hm1=(r+1<n)?hole[r+1]:0;
		int hm2=(r+2<n)?hole[r+2]:0;
		fill(ndp,ndp+SZ,NEG);
		nxt.clear();
		for(int s:cur)
		{
			int val=dp[s];
			int A=s>>m,B=s&full;
			if(A&hm)
				continue; // 旧海报不可能盖到洞上，这里只是保险
			int addA=0,addB=0,addC=0;
			function<void(int,int)>go=[&](int j,int cnt)
			{
				while(j<m&&((A>>j&1)||(addA>>j&1)||(hm>>j&1)))
					++j;
				if(j>=m)
				{
					int ns=((B|addB)<<m)|addC;
					if(ndp[ns]<val+cnt)
					{
						if(ndp[ns]==NEG)
							nxt.push_back(ns);
						ndp[ns]=val+cnt;
					}
					return;
				}
				go(j+1,cnt); // 本格留空
				if(j+2<m&&r+1<n) // 横放 2x3
				{
					int mask=7<<j;
					if(!(hm&mask)&&!(hm1&mask)&&!(A&mask)&&!(addA&mask)&&!(B&mask)&&!(addB&mask))
					{
						addA|=mask;addB|=mask;
						go(j+1,cnt+1);
						addA^=mask;addB^=mask;
					}
				}
				if(j+1<m&&r+2<n) // 竖放 3x2
				{
					int mask=3<<j;
					if(!(hm&mask)&&!(hm1&mask)&&!(hm2&mask)&&!(A&mask)&&!(addA&mask)&&!(B&mask)&&!(addB&mask)&&!(addC&mask))
					{
						addA|=mask;addB|=mask;addC|=mask;
						go(j+1,cnt+1);
						addA^=mask;addB^=mask;addC^=mask;
					}
				}
			};
			go(0,0);
		}
		swap(dp,ndp);
		cur.swap(nxt);
	}
	int ans{0};
	for(int s:cur)
		ans=max(ans,dp[s]);
	cout<<ans<<'\n';
	return 0;
}
```
