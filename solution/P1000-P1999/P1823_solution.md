# P1823 [COI 2007] Patrik 音乐会的等待

## 1. 重新读题

### 1.1 人话版

$n$ 个人站成一排，第 $i$ 个人身高 $h_i$。一对人 $(a,b)$（$a<b$）能互相看见，当且仅当：两人相邻，或者站在他们中间的所有人都不比他们俩高（即中间的最大身高 $\le\min(h_a,h_b)$）。求能互相看见的无序对数量。

### 1.2 关键约束与规模暗示

- $1\le n\le5\times10^5$，身高 $<2^{31}$。
- $O(n^2)$ 枚举所有对是 $2.5\times10^{11}$，必超时；需要 $O(n)$ 或 $O(n\log n)$。
- 答案最大可达 $\binom{n}{2}\approx1.25\times10^{11}$，**必须用 64 位整数**。
- "中间没人更高"是典型的最大值/单调栈结构；身高可以相等，相等会带来计数细节。

---

## 2. 思维链（Chain-of-thought）

### 2.1 explore its property：把"看得见"写成最大值条件

对 $a<b$，定义区间 $(a,b)$ 内最大身高 $\mathrm{mx}(a,b)=\max_{a<k<b}h_k$（空区间记为 $-\infty$）。则
$$
(a,b)\ \text{可见}\iff \mathrm{mx}(a,b)\le\min(h_a,h_b).
$$

### 2.2 固定右端点，向左扫描

固定新来的人 $j$（高度 $H=h_j$），看它能和左边哪些 $i$ 配对。令 $i$ 从 $j-1$ 向左走，维护 $\mathrm{cur}=\max(h_{i+1},h_{i+2},\dots,h_{j-1})$（初始 $-\infty$）。

$i$ 可见 $\iff \mathrm{cur}\le h_i$ 且 $\mathrm{cur}\le H$。

- 第二条件说明：**一旦 $\mathrm{cur}>H$，再往左的人都不可能可见**，可以停止。
- 第一条件 $\mathrm{cur}\le h_i$ 说明：可见的 $i$ 恰好是那些"从右往左扫描时的前缀最大值位置"（允许相等）。

由此定义**记录**：从 $i=j-1$ 向左扫，维护 $\mathrm{cur}$；若 $h_p\ge\mathrm{cur}$ 则 $p$ 是记录，随后 $\mathrm{cur}\leftarrow\max(\mathrm{cur},h_p)$。记录位置构成递减序列 $q_1=j-1>q_2>\cdots>q_M$，且 $h_{q_1}\le h_{q_2}\le\cdots\le h_{q_M}$。

### 2.3 关键结构：把记录按等高分组

把相邻且等高的记录合成一组。栈里从**底到顶**存 $G_1,\dots,G_t$，其中：

- $G_1$ 在最左、最高，$G_t$ 在最右、最矮（即 $q_1$ 所在组）；
- 组高 $v_1>v_2>\cdots>v_t$，组大小 $c_1,\dots,c_t$。

**引理 1（相邻记录之间的最大值）**
对记录位置 $p$：

- 若 $p$ 不是所在组的最右元素，则 $\mathrm{mx}(p,\,j-1)=h_p$（等于该组组高）；
- 若 $p$ 是 $G_k$ 的最右元素且 $k<t$，则 $\mathrm{mx}(p,\,j-1)=v_{k+1}$；
- 若 $p=q_1=j-1$，则区间为空，$\mathrm{mx}=-\infty$。

**证明**：按扫描顺序，记录 $q_m$ 被记下后，$\mathrm{cur}$ 变成 $h_{q_m}$，这正是 $\max(h_{q_m},h_{q_m+1},\dots,h_{j-1})$。在 $q_m$ 与它右边相邻的记录 $q_{m-1}$ 之间没有记录，说明这些位置的 $h$ 都严格小于当时的 $\mathrm{cur}=h_{q_{m-1}}$，故 $\max(h_{q_m+1..j-1})=h_{q_{m-1}}$。同组内相邻记录等高，故为组高；跨组则取下一组组高；$q_1=j-1$ 时区间为空。$\blacksquare$

### 2.4 算出每个新人的贡献

**引理 2（右端点 $j$ 的可见对数）**
设新来者高度 $H$，记录分组如上。

- 若所有 $v_k>H$（记 $r=t+1$）：贡献恰为 $1$（只有相邻的 $q_1=j-1$）。
- 否则令 $r$ 为最小的满足 $v_r\le H$ 的下标，贡献为
$$
\sum_{k=r}^{t}c_k+[\,r>1\,].
$$

**证明**：由引理 1，对记录 $p$ 分情况判断 $\mathrm{mx}(p,j-1)\le\min(h_p,H)$：

- **组 $k>r$**：$v_k<H$（严格，因为组高严格递减且 $v_r\le H$）。非最右元素：$\mathrm{mx}=v_k\le H$ 且 $v_k=h_p$，可见；最右元素（$k<t$）：$\mathrm{mx}=v_{k+1}<v_k<H$，可见；若 $k=t$，其最右元素就是 $q_1$，恒可见。故 $c_k$ 个全可见。
- **组 $r$**：$v_r\le H$。非最右元素：$\mathrm{mx}=v_r\le H$，可见；最右元素（$r<t$）：$\mathrm{mx}=v_{r+1}<v_r\le H$，可见；若 $r=t$ 则最右元素是 $q_1$，恒可见。故 $c_r$ 个全可见。
- **组 $r-1$**（若 $r>1$）：$v_{r-1}>H$。非最右元素：$\mathrm{mx}=v_{r-1}>H$，不可见；最右元素：$\mathrm{mx}=v_r\le H$ 且 $v_r\le v_{r-1}=h_p$，可见。恰 $1$ 个。
- **组 $k<r-1$**：最右元素需要 $v_{k+1}\le H$，但 $v_{k+1}\ge v_{r-1}>H$；非最右元素需要 $v_k\le H$，也不可能。故 $0$ 个。

求和即得公式。全 $>H$ 时同理只有 $q_1$ 可见。$\blacksquare$

### 2.5 单调栈如何对应这些组

**引理 3（栈不变量）**
处理完前缀 $1..i$ 后，栈从底到顶恰好是 $G_1,\dots,G_t$（组高严格递减，附带组大小）。

**证明**：加入新位置 $i+1$（高度 $H$）后，新的记录集合 $=\{$旧记录中 $h_p\ge H$ 的那些$\}\cup\{i+1\}$。原因：对旧记录 $p$，从 $i+1$ 往左扫时 $\mathrm{cur}$ 先为 $H$，$h_p\ge H$ 才能被记下；一旦某个旧记录 $h_p<H$ 被跳过，再往左的旧记录只会处理更大的 $\mathrm{cur}$（$\mathrm{cur}$ 单调不减），更不可能成为记录。而非记录位置本来就存在右边更高的元素，永远成不了记录。于是：

- 把栈顶组高 $<H$ 的组全部弹掉（它们不再是记录）；
- 若新的栈顶组高 $=H$，把 $i+1$ 并入该组（大小 $+1$）；
- 否则压入新组 $(H,1)$。

这正是算法的操作。$\blacksquare$

把引理 2 的公式翻译成栈操作：

- `while` 栈顶组高 $<H$：弹出并累加该组大小（对应 $k>r$ 以及 $v_r<H$ 时的组 $r$）；
- 若此时栈顶组高 $=H$：累加其大小（组 $r$），并把它与当前人合并（大小 $+1$）；
- 若弹出/合并后栈仍非空：再累加 $1$（组 $r-1$ 的那个最右元素）；
- 若栈一开始就空，什么也不加。

### 2.6 为什么是线性的

每个位置最多入栈一次；每次 `while` 弹出一个组，而一个组由某次入栈产生，所以总弹出次数 $O(n)$；合并不增加组数。总复杂度 $O(n)$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 可见性等价于"中间最大值 $\le\min$"。
- **explore its property** — 固定右端点向左扫，可见者恰为"从右往左的前缀最大记录"。
- **explore its property** — 记录按等高分组，组贡献分三类讨论（组高 $<H$、$=H$、$>H$）。
- **classic problem solution** — 单调栈维护这些组，边弹边累加；等高要合并计数，合并后若下面还有更高者再额外 $+1$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 人数 | 输入 | 维度 |
| `h[i]` | `i64 [mxn]` | 第 $i$ 个人的身高（$0$ 基） | 输入 | height |
| `sh[top]` | `i64 [mxn]` | 栈中第 `top` 个组的组高（底 $\to$ 顶，严格递减） | 动态维护 | stack height |
| `sc[top]` | `int [mxn]` | 该组的记录人数 | 动态维护 | stack count |
| `top` | `int` | 栈内组数 | $0$ | 栈顶指针（半开） |
| `ans` | `i64` | 可见对数 | $0$ | 答案 |

### 4.2 算法步骤

1. 读入 $n$ 与 $h_0,\dots,h_{n-1}$。
2. 对每个人 $x=h_i$，令 `cnt=1`：
   - 当栈非空且 `sh[top-1]<x`：`ans+=sc[top-1]`，`--top`；
   - 若栈非空且 `sh[top-1]==x`：`ans+=sc[top-1]`，`cnt=sc[top-1]+1`，`--top`，若栈仍非空则 `++ans`；
   - 否则若栈非空（`sh[top-1]>x`）：`++ans`；
   - 把 `(x,cnt)` 压栈。
3. 输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：每个位置入栈一次、每个组出栈一次，均摊 $O(n)$。
- **空间复杂度**：$O(n)$（两个长度为 $n$ 的数组）。

---

## 5. 示例代码

```cpp
//
// 题目：P1823 [COI 2007] Patrik 音乐会的等待
// https://www.luogu.com.cn/problem/P1823
// 与 O(n^2) 暴力对拍通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{500005};

int n;
i64 h[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>h[i];
	static i64 sh[mxn];
	static int sc[mxn];
	int top=0;
	i64 ans=0;
	for(int i=0;i<n;++i)
	{
		i64 x=h[i];
		int cnt=1;
		while(top>0&&sh[top-1]<x)     // 更矮的组全部可见，弹出
		{
			ans+=sc[top-1];
			--top;
		}
		if(top>0)
		{
			if(sh[top-1]==x)          // 等高：组内全可见，并合并
			{
				ans+=sc[top-1];
				cnt=sc[top-1]+1;
				--top;
				if(top>0)             // 再往下那个更高的人也能看见
					++ans;
			}
			else                      // 最近的更高者，仅此一对
				++ans;
		}
		sh[top]=x;
		sc[top]=cnt;
		++top;
	}
	cout<<ans<<'\n';
	return 0;
}
```
