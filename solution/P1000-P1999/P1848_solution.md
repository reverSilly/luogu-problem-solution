# P1848 [USACO12OPEN] Bookshelf G

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $N$ 本书要**按原顺序**放进若干层书架。第 $i$ 本书宽 $W_i$、高 $H_i$。

- 每层书架装的是原序列里连续的一段书；
- 每层书架的**总宽度**不超过 $L$；
- 一层书架的高度等于该层最高的书的高度；
- 整组书架的高度是各层高度之和。

求最小的总高度。

### 1.2 关键约束与规模暗示

- $N\le 10^5$，$L\le 10^9$，$W_i\le L$（单本一定放得下），$H_i\le 10^6$。
- 答案最大约 $10^5\times10^6=10^{11}$，必须用 64 位。
- $N=10^5$ 排除 $O(N^2)$ 区间 DP（$10^{10}$），需要 $O(N\log N)$ 一类的数据结构优化。
- "必须连续、按顺序"是本题的救命条件：它让"最后一层"可以只用两个下标描述。

---

## 2. 思维链（Chain-of-thought）

### 2.1 最直白的 DP

设 $dp[i]$ 为前 $i$ 本书的最小总高度。最后一层装的是 $j+1..i$（$0\le j<i$），于是

$$dp[i]=\min_{j}\Big(dp[j]+\max_{k=j+1}^{i}H_k\Big),$$

其中 $j$ 需满足 $\sum_{k=j+1}^{i}W_k\le L$。

把下标改成 0-based：最后一层是 $j..i-1$，

$$dp[i]=\min_{j\in[lo_i,\,i-1]}\Big(dp[j]+M(j,i-1)\Big),\qquad M(j,i-1)=\max_{k=j}^{i-1}H_k,$$

$lo_i$ 是最小的合法起点：

$$lo_i=\min\Big\{j\ \Big|\ \sum_{k=j}^{i-1}W_k\le L\Big\}.$$

**引理 1（上式是正确的最小值）**

$dp[i]$ 由上式给出，其中 $dp[0]=0$。

**证明**：任何方案的最后一行覆盖某个后缀 $j..i-1$；其前面部分是对前 $j$ 本书的合法方案，高度至少 $dp[j]$，最后一行高度恰为 $M(j,i-1)$，故方案高度 $\ge dp[j]+M(j,i-1)\ge$ 上式。反过来，任取合法 $j$，把"前 $j$ 本的最优方案"接上"一行 $j..i-1$"就是一个合法方案，高度恰为 $dp[j]+M$。两边夹逼即得。$\blacksquare$

直接算是 $O(N^2)$。

### 2.2 观察：$i$ 增加一位时，最大值函数只在一段后缀上改变

固定 $i$，把 $f[j]=dp[j]+M(j,i-1)$ 看成关于 $j$ 的数组。当 $i\to i+1$（新书 $i$，高 $h=H_i$）时：

$$M(j,i)=\max\big(M(j,i-1),\,h\big).$$

所以只有那些**"原来的最大值 $<h$"** 的 $j$ 会变成 $h$；其余不变。

关键问题是：哪些 $j$ 满足 $M(j,i-1)<h$？

**引理 2（满足条件的 $j$ 是连续的一段后缀）**

存在 $l$，使得 $M(j,i-1)<h$ 当且仅当 $j\ge l$。并且这些 $j$ 的旧最大值构成若干段"同一高度"的连续区间。

**证明**：$M(j,i-1)$ 随 $j$ 增大而单调不增（从更靠后的位置开始取 max，集合变小）。因此 $\{j:M(j,i-1)<h\}$ 是一个后缀。进一步，把 $j$ 从 $i-1$ 往左扫，$M$ 是一个**阶梯函数**：每当遇到一本比以前都高的书，$M$ 就跳一次。用单调栈正好维护这些"阶梯段"。$\blacksquare$

### 2.3 单调栈的不变量

维护一个栈（栈底到栈顶），每个元素是 `(高度 h, 左端点 l)`，表示

$$M(j,i-1)=h\quad\text{对所有 } j\in[l,\ \text{下一个栈元素的 }l).$$

最顶上那一段的右端点是 $i-1$。

**引理 3（栈的不变量）**

- 栈中高度从栈底到栈顶**严格递减**；
- 每段 $[l,\text{next}_l)$ 内所有 $j$ 的当前最大值都等于该段记录的 $h$；
- 这些段恰好划分 $[0,i-1]$。

**证明**（对 $i$ 归纳）：

- $i=1$：只有 $j=0$，$M=H_0$，栈为 `(H_0,0)`，成立。
- 设 $i$ 时成立，加入 $h=H_i$。从栈顶开始，所有高度 $\le h$ 的段，其 $M$ 都会被更新为 $h$（因为 $h$ 比它们都高/相等）；高度 $>h$ 的段不受影响（它们的最大值仍更大，且更靠左的段覆盖的区间里含有那本更高的书）。把这些被弹出的段合并，加上新的 $j=i$，构成一段 `(h, 最左 l)`。合并后剩下的段高度都 $>h$，仍严格递减。$\blacksquare$

### 2.4 用线段树维护 $f[j]$

我们要支持三件事：

1. 对一段 $j$ 区间，把 $M$ 升高到 $h$，即 $f[j]\mathrel{+}=(h-h_{\text{old}})$；
2. 在位置 $i-1$ 插入新的 $j$：$f[i-1]=dp[i-1]+H_{i-1}$；
3. 查询 $\min_{j\in[lo_i,\,i-1]}f[j]$。

这正好是"区间加 + 单点赋值 + 区间最小值"。线段树可以胜任。

**引理 4（操作 1 的正确性）**

把某个阶梯段 $[l,r)$ 的旧高度 $h_{\text{old}}$ 统一改成 $h$ 时，对整段执行 $f[j]\mathrel{+}=(h-h_{\text{old}})$ 是合法的。

**证明**：在该段内，对每个 $j$ 都有 $M(j,i-1)=h_{\text{old}}$（引理 3），而 $f[j]=dp[j]+M$，所以每个 $j$ 的增量都是同一个常数 $h-h_{\text{old}}$。$\blacksquare$

**引理 5（操作 2 是"纯赋值"）**

位置 $i-1$ 从未被之前的任何区间加影响。

**证明**：第 $i'<i$ 步的所有区间加，其右端点都不超过 $i'-1<i-1$；而位置 $i-1$ 直到第 $i$ 步才第一次作为 $j$ 出现。因此对它直接赋值即可。$\blacksquare$

### 2.5 合法左端点 $lo_i$ 用双指针

**引理 6（$lo_i$ 单调不降）**

若 $j$ 对 $i+1$ 合法，则 $j$ 对 $i$ 合法，因此 $lo_{i+1}\ge lo_i$。

**证明**：$\sum_{k=j}^{i}W_k\ge\sum_{k=j}^{i-1}W_k$。若前者 $\le L$，则后者 $\le L$。所以 $i+1$ 的合法集合 $\subseteq$ $i$ 的合法集合 $\cup\{i\}$，最小元不可能变小。$\blacksquare$

于是用一个游标 $lo$ 和窗口宽度 $sw$ 即可：每加一本书就 `sw+=W`，当 `sw>L` 时不停 `sw-=W[lo]`、`++lo`。

### 2.6 算法串起来

第 $i$ 步（$1\le i\le N$）：

1. 更新窗口，得到 $lo_i$；
2. 令 $l=i-1$，弹出栈顶所有高度 $\le h$ 的段，对每个被弹段 $[sl,l)$ 做区间加 $h-sh$，并令 $l=sl$；
3. 在位置 $i-1$ 赋值 $dp[i-1]+h$；
4. 压栈 `(h,l)`；
5. $dp[i]=\min_{[lo_i,\,i)}f$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 最后一行是后缀，$dp[i]=\min_j dp[j]+\max H[j..i-1]$，附带宽度窗口约束。
- **consider easier version** — 若不算最大值，只是一个前缀 min；最大值让每个 $j$ 的代价随 $i$ 动态变化。
- **explore its property** — 加入 $H_i$ 时，只有"旧最大值 $<H_i$"的那段后缀会改变，且它是一个阶梯函数的一段后缀。
- **classic problem solution** — 单调栈维护阶梯段，每个段内 $f[j]$ 同步增加同一个常数。
- **classic problem solution** — 线段树（区间加 + 单点赋值 + 区间 min）承载所有 $f[j]$。
- **two pointers** — 宽度约束给出单调不降的合法左端点 $lo_i$，查询区间是 $[lo_i,i)$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 书的数量 | 读入 | $\le 10^5$ |
| `L` | `i64` | 每层最大宽度 | 读入 | — |
| `H[i], W[i]` | `i64 [100005]` | 第 $i$ 本（0-based）的高、宽 | 读入 | height/width |
| `dp[i]` | `i64 [100005]` | 前 $i$ 本的最小总高 | `dp[0]=0` | DP |
| `lo` | `int` | 当前 $i$ 的最左合法起点 | $0$ | left ok |
| `sw` | `i64` | 窗口 $[lo,i-1]$ 的宽度和 | $0$ | sum width |
| `sh[], sl[]` | `i64/int [100005]` | 单调栈：段高度、段左端点 | 空 | stack h / l |
| `tr[i]` | `i64 [400020]` | 线段树节点最小值 | 全 `inf` | tree |
| `lz[i]` | `i64 [400020]` | 区间加懒标记 | 全 $0$ | lazy |

注：$f[j]=dp[j]+M(j,i-1)$ 不单独存数组，直接存在线段树里；第 $j$ 个叶子就是 $f[j]$。

### 4.2 算法步骤

1. 读入 $N,L$ 与所有 $(H_i,W_i)$。
2. 线段树初始全为 `inf`（表示该位置还不是合法前缀）；`dp[0]=0`，栈空，`lo=0`，`sw=0`。
3. 对 $i=1..N$：
   1. `sw+=W[i-1]`；`while(sw>L){sw-=W[lo];++lo;}`；
   2. `h=H[i-1]`，`l=i-1`；
   3. `while(栈非空且 栈顶高度<=h)`：弹出 `(hh,ll)`，对 `[ll,l)` 区间加 `h-hh`，令 `l=ll`；
   4. 单点赋值 `pos=i-1` 为 `dp[i-1]+h`；
   5. 压入 `(h,l)`；
   6. `dp[i]=query_min(lo, i)`（半开区间）。
4. 输出 `dp[N]`。

### 4.3 复杂度

- 每本书最多入栈/出栈一次，总区间加次数 $O(N)$。
- 每次区间加/单点赋值/区间查询 $O(\log N)$。
- 总时间 $O(N\log N)$；空间 $O(N)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1848 [USACO12OPEN] Bookshelf G
// https://www.luogu.com.cn/problem/P1848
// 本地自测通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr i64 inf{1'000'000'000'000'000'000LL};

int n;
i64 L;
i64 H[mxn],W[mxn],dp[mxn];

i64 tr[mxn*4],lz[mxn*4];

void apply(int i,i64 v)
{
	tr[i]+=v;
	lz[i]+=v;
}
void push(int i)
{
	if(lz[i])
	{
		apply(i*2+1,lz[i]);
		apply(i*2+2,lz[i]);
		lz[i]=0;
	}
}
void add(int ql,int qr,i64 v,int l,int r,int i)
{
	if(qr<=l||r<=ql)
		return;
	if(ql<=l&&r<=qr)
	{
		apply(i,v);
		return;
	}
	push(i);
	int mid=(l+r)>>1;
	add(ql,qr,v,l,mid,i*2+1);
	add(ql,qr,v,mid,r,i*2+2);
	tr[i]=min(tr[i*2+1],tr[i*2+2]);
}
void setp(int pos,i64 v,int l,int r,int i)
{
	if(r-l==1)
	{
		tr[i]=v;
		lz[i]=0;
		return;
	}
	push(i);
	int mid=(l+r)>>1;
	if(pos<mid)
		setp(pos,v,l,mid,i*2+1);
	else
		setp(pos,v,mid,r,i*2+2);
	tr[i]=min(tr[i*2+1],tr[i*2+2]);
}
i64 qmin(int ql,int qr,int l,int r,int i)
{
	if(qr<=l||r<=ql)
		return inf;
	if(ql<=l&&r<=qr)
		return tr[i];
	push(i);
	int mid=(l+r)>>1;
	return min(qmin(ql,qr,l,mid,i*2+1),qmin(ql,qr,mid,r,i*2+2));
}

int sl[mxn];
i64 sh[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>L;
	for(int i=0;i<n;++i)
		cin>>H[i]>>W[i];

	fill_n(tr,mxn*4,inf);
	dp[0]=0;
	int top=0;
	i64 sw=0;
	int lo=0;
	for(int i=1;i<=n;++i)
	{
		i64 h=H[i-1];
		sw+=W[i-1];
		while(sw>L)
		{
			sw-=W[lo];
			++lo;
		}
		int l=i-1;
		while(top>0&&sh[top-1]<=h)
		{
			--top;
			add(sl[top],l,h-sh[top],0,n+1,0);
			l=sl[top];
		}
		setp(i-1,dp[i-1]+h,0,n+1,0);
		sh[top]=h;
		sl[top]=l;
		++top;
		dp[i]=qmin(lo,i,0,n+1,0);
	}
	cout<<dp[n]<<'\n';
	return 0;
}
```
