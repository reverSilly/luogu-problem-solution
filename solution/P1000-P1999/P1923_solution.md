# P1923 【深基9.例4】求第 k 小的数

## 1. 重新读题

### 1.1 人话重述

给定 $n$ 个整数 $a_i$，求第 $k$ 小的数。这里“最小”记为第 $0$ 小，也就是说要把数组升序排列后输出下标为 $k$ 的元素（$0$-based）。

$1\le a_i<10^9$，$1\le n<5\times10^6$，$0\le k<n$，$n$ 为奇数。

### 1.2 关键约束与规模暗示

- $n$ 高达 $5\times10^6$，排序 $O(n\log n)$ 在 $5\times10^6$ 规模下常数偏大，且本题明确要求练习**分治**，所以应当写期望 $O(n)$ 的快速选择（quickselect）。
- 值域 $10^9$，不能开桶做值域计数。
- 数据量巨大，输入输出必须加速。
- 题目不保证元素互不相同，因此划分必须能处理大量相等元素，否则会退化。

---

## 2. 思维链（Chain-of-thought）

### 2.1 从“排序”到“只找第 k 个”

排序会把所有元素的相对顺序都确定下来，但我们只要一个位置。注意到：如果随便选一个数 $p$ 作为基准，把数组分成

$$
S_<=\{a_i<p\},\qquad S_==\{a_i=p\},\qquad S_>=\{a_i>p\},
$$

那么：

- 若 $k<|S_<|$，答案一定在 $S_<$ 中；
- 若 $|S_<|\le k<|S_<|+|S_=|$，答案就是 $p$；
- 否则答案在 $S_>$ 中，且在新数组中的下标为 $k-|S_<|-|S_=|$。

这就是 quickselect。每轮只需要递归/迭代处理一个子区间，另一个子区间直接丢弃。

### 2.2 为什么必须是“三路”划分

如果只分成“小于等于”和“大于”两路，考虑所有元素相等的情况：每轮只能确定一个元素，规模只减少 $1$，复杂度退化为 $O(n^2)$。而三路划分会把所有等于基准的元素一次性确定下来，全相等时一轮结束。

**引理 1（三路划分不变量）**：用三个指针 $i,lt,gt$ 扫描区间 $[l,r]$，初始 $i=lt=l,\ gt=r$，基准值 $pv$，循环条件 $i\le gt$：

- 若 $a_i<pv$，交换 $a_i$ 与 $a_{lt}$，然后 $i\gets i+1,\ lt\gets lt+1$；
- 若 $a_i>pv$，交换 $a_i$ 与 $a_{gt}$，然后 $gt\gets gt-1$（注意 $i$ 不动）；
- 若 $a_i=pv$，$i\gets i+1$。

循环结束时：
$$
a[l..lt-1]<pv,\qquad a[lt..gt]=pv,\qquad a[gt+1..r]>pv.
$$

**证明**（对循环次数归纳）：维护不变量

$$
a[l..lt-1]<pv,\qquad a[lt..i-1]=pv,\qquad a[gt+1..r]>pv .
$$

初始 $lt=i=l$、$gt=r$，三段均为空，成立。

- 情况 $a_i<pv$：把 $a_i$ 换到 $lt$ 位置。原 $a_{lt}$ 属于等值段（若 $lt<i$）或就是 $a_i$ 自己（若 $lt=i$）。交换后 $a_{lt}<pv$，小于段右扩一格；等值段左端右移一格，长度不变（当 $lt=i$ 时等值段为空），$i$ 也右移，不变量保持。
- 情况 $a_i>pv$：把 $a_i$ 换到 $gt$ 位置，$gt$ 左移。大于段左扩一格；换到 $i$ 的原 $a_{gt}$ 尚未检查，故 $i$ 不动，等值/待检查边界相应调整，不变量保持。
- 情况 $a_i=pv$：等值段右扩一格，$i$ 右移，不变量保持。

循环结束时 $i>gt$，待检查段为空，三段恰好覆盖 $[l,r]$。$\blacksquare$

**引理 2（迭代正确性）**：维护“答案在当前区间 $[l,r]$ 内且目标下标为 $k$”。每次划分后按 2.1 的三条规则把 $l,r$ 收缩到包含答案的那一段；若命中等值段则直接返回 $pv$。因此算法返回的正是第 $k$ 小数。

**证明**：划分后三段是原区间的划分，且段内元素大小关系严格分开，故第 $k$ 小只可能在 $S_<$（若 $k<|S_<|$）、$S_=$（若 $k$ 落在中间）或 $S_>$（否则）。收缩后目标下标相应平移。归纳到区间长度为 $1$ 时，$a[l]$ 即答案。$\blacksquare$

### 2.3 随机基准为什么能做到期望线性

若基准固定（比如总取最左），构造单调数据即可让每轮只缩小一个元素，退化为 $O(n^2)$。因此基准在区间内**均匀随机**选取。

**引理 3（期望复杂度）**：设 $T(n)$ 为长度为 $n$ 的区间上的期望比较次数。随机基准的秩在 $0,\dots,n-1$ 中等概率，划分后需要继续处理的那一侧长度不超过 $\max(i,n-1-i)$。

关键观察：基准落在“中间一半”范围内（即两侧长度都不超过 $3n/4$）的概率至少为 $1/2$。于是

$$
T(n)\le \frac12 T\!\left(\frac{3n}{4}\right)+\frac12 T(n)+cn
$$

其中 $cn$ 是本轮划分的代价。移项得

$$
\frac12 T(n)\le \frac12 T\!\left(\frac{3n}{4}\right)+cn
\ \Longrightarrow\
T(n)\le T\!\left(\frac{3n}{4}\right)+2cn .
$$

展开等比级数：

$$
T(n)\le 2c\left(n+\frac{3n}{4}+\left(\frac{3n}{4}\right)^2+\cdots\right)=O(n).
$$

更严格地说，即使出现“坏基准”，它的概率衰减足够快，期望仍为 $O(n)$（标准 quickselect 分析）。$\blacksquare$

### 2.4 实现上的选择

- 递归深度最坏为 $O(n)$，而 $n=5\times10^6$ 会爆栈，所以写成**迭代**：用 `while(l<r)` 不断收缩区间。
- 用三路划分处理重复元素。
- 随机数用 xorshift，避免 `rand()` 的取模偏差与开销。
- 输入用 `getchar` 快读：$5\times10^6$ 个整数，`cin` 即使关闭同步也可能偏慢。

---

## 3. 模式总结（Pattern Summary）

- **reduce the task** — 不排序，只定位第 $k$ 个位置，用划分丢弃无关部分。
- **classic problem solution** — quickselect：选基准、划分、只递归一侧。
- **handle duplicates** — 三路划分（小于 / 等于 / 大于），一次确定所有等于基准的元素。
- **randomization** — 随机基准避免固定基准被单调数据击穿，期望 $O(n)$。
- **iterative rewrite** — 用循环代替递归，防止 $5\times10^6$ 规模爆栈。
- **fast I/O** — `getchar` 快读应对大数据。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| $n,k$ | `int` | 元素个数、目标下标（0-based） | 读入 | 规模 |
| `a[i]` | `int[mxn]` | 数组 | 读入 | array |
| `l,r` | `int` | 当前答案所在区间 | $0,n-1$ | range |
| `pv` | `int` | 本轮随机基准值 | 随机取 | pivot |
| `lt,gt,i` | `int` | 三路划分的三个指针 | $l,l,l$ | lower/upper |
| `rngState` | `unsigned long long` | xorshift 随机状态 | 固定种子 | random |

### 4.2 算法步骤

1. 快读 $n,k$ 与数组 `a`。
2. 置 $l=0,\ r=n-1$。
3. 当 $l<r$：
   - 在 $[l,r]$ 内随机取一个位置，令 $pv=a[\text{pos}]$；
   - 令 $i=lt=l,\ gt=r$，按引理 1 做三路划分；
   - 若 $k<lt$，令 $r=lt-1$；
   - 否则若 $k>gt$，令 $l=gt+1$；
   - 否则（$lt\le k\le gt$）答案就是 $pv$，直接 `break`。
4. 输出 `a[k]`。

### 4.3 复杂度

- 时间：期望 $O(n)$，最坏 $O(n^2)$（随机化后实际不会出现）；空间 $O(n)$。
- 划分在原地完成，额外空间 $O(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1923 【深基9.例4】求第 k 小的数
// https://www.luogu.com.cn/problem/P1923
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5000005};

int n,k;
int a[mxn];

static unsigned long long rngState{88172645463325252ULL};

static inline unsigned long long rnd()
{
	rngState^=rngState<<7;
	rngState^=rngState>>9;
	return rngState;
}

static inline int readInt()
{
	int x{0};
	char c{};
	while(c<'0'||c>'9')
		c=(char)getchar();
	while(c>='0'&&c<='9')
	{
		x=x*10+(c-'0');
		c=(char)getchar();
	}
	return x;
}

int main()
{
	n=readInt();
	k=readInt();
	for(int i=0;i<n;++i)
		a[i]=readInt();

	int l{0},r{n-1};
	while(l<r)
	{
		int pv=a[l+(int)(rnd()%(unsigned long long)(r-l+1))];
		int i{l},lt{l},gt{r};
		while(i<=gt)
		{
			if(a[i]<pv)
				swap(a[i++],a[lt++]);
			else if(a[i]>pv)
				swap(a[i],a[gt--]);
			else
				++i;
		}
		if(k<lt)
			r=lt-1;
		else if(k>gt)
			l=gt+1;
		else
			break;
	}
	cout<<a[k]<<'\n';
	return 0;
}
```
