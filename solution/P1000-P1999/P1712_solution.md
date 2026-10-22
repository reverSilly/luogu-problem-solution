# P1712 [NOI2016] 区间

## 1. 重新读题

数轴上有 $n$ 个闭区间 $[l_i, r_i]$（$1 \le n \le 5 \times 10^5$，$0 \le l_i \le r_i \le 10^9$），第 $i$ 个区间的长度定义为 $\text{len}_i = r_i - l_i$。

要求从中选出 $m$ 个区间（$1 \le m \le 2 \times 10^5$），使得这 $m$ 个区间的交集非空（即存在某个实数点 $x$ 同时属于这 $m$ 个区间），并最小化所选 $m$ 个区间的**最大长度与最小长度之差**。若不存在合法方案，输出 `-1`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：两个维度（“长度极差”与“公共交点”）如何解耦？

问题同时涉及两个约束：
1. **目标函数**：$\max_{i \in S} \text{len}_i - \min_{i \in S} \text{len}_i$ 最小；
2. **可行性条件**：$|S| = m$ 且 $\bigcap_{i \in S} [l_i, r_i] \ne \emptyset$。

如果先把所有区间按长度 $\text{len}_i$ 从小到大排序：$\text{len}_0 \le \text{len}_1 \le \dots \le \text{len}_{n-1}$，那么任何选出的子集 $S$ 若其最短区间下标为 $L$、最长区间下标为 $R$，目标代价就只取决于 $L$ 和 $R$，等于 $\text{len}_R - \text{len}_L$！

### 2.2 规约到连续下标窗口与双指针单调性（附完整证明）

**引理 1（连续窗口充分性）**：将区间按长度升序排序后，若在下标区间 $[L, R]$ 内能选出 $m$ 个有公共交点的区间，这等价于：将 $[L, R]$ 内的**全部**区间都画在数轴上时，数轴上被覆盖次数最多的点至少被覆盖了 $m$ 次。

**证明**：
- 若 $[L, R]$ 内存在一个大小为 $m$ 的子集 $S$ 共同包含点 $x$，则把 $[L, R]$ 的所有区间画在数轴上时，点 $x$ 被覆盖的次数至少为 $|S| = m$，从而全局最大覆盖次数 $\ge m$。
- 反之，若将 $[L, R]$ 内所有区间画在数轴上后，某点 $x$ 被覆盖了 $c \ge m$ 次，则覆盖点 $x$ 的那 $c$ 个区间都在 $[L, R]$ 内；从中任取 $m$ 个区间（必要时包含端点 $L$ 与 $R$，代价仍不超过 $\text{len}_R - \text{len}_L$），它们都包含点 $x$，构成合法方案。$\blacksquare$

**引理 2（端点离散化的等价性）**：设 $V$ 为所有区间的左右端点集合 $\{l_i, r_i\}_{i=0}^{n-1}$。若干闭区间存在实数公共交点 $x \in \mathbb{R}$，当且仅当它们存在属于端点集合 $V$ 的公共交点 $x^* \in V$。

**证明**：
若干闭区间 $\{[l_i, r_i]\}_{i \in S}$ 的交集是一个闭区间 $\big[\max_{i \in S} l_i,\ \min_{i \in S} r_i\big]$。该闭区间非空当且仅当 $x^* = \max_{i \in S} l_i \le \min_{i \in S} r_i$。此时左端点 $x^* = \max_{i \in S} l_i$ 本身就属于端点集合 $V$，且满足对一切 $i \in S$ 有 $l_i \le x^* \le r_i$。因此只需在离散化后的至多 $2n$ 个端点坐标上维护覆盖次数即可，绝不会漏掉任何非端点交点。$\blacksquare$

**引理 3（双指针单调性）**：记 $\text{ok}(L, R)$ 表示下标窗口 $[L, R]$ 内的最大单点覆盖次数是否 $\ge m$。对于固定的右端点 $R$，使 $\text{ok}(L, R)$ 成立的最大左端点 $L^*(R)$ 关于 $R$ 单调不降。

**证明**：
向集合中加入新区间只会使数轴上每个点的覆盖次数单调不减，从集合中删去区间只会使覆盖次数单调不增。设 $R_1 < R_2$。对于任意 $L < L^*(R_1)$，由于 $[L^*(R_1), R_1] \subseteq [L^*(R_1), R_2]$，必有 $\text{ok}(L^*(R_1), R_2)$ 成立，故最优左端点无需回退到 $L^*(R_1)$ 左侧，即 $L^*(R_1) \le L^*(R_2)$。$\blacksquare$

由引理 1–3，我们只需将端点离散化到 $[0, \text{vcnt})$，用一棵支持**区间加 $\pm 1$** 与**全局最大值查询**（直接读取根节点 `mx[0]`）的懒标记线段树配合双指针扫描即可！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将区间按长度排序后，“最长减最短”转化为有序下标窗口 $[L, R]$ 的端点长度差 $\text{len}_R - \text{len}_L$。
- **reduce to easier problem** — 利用闭区间交的左端点性质，将“选 $m$ 个区间有公共点”等价为“窗口 $[L, R]$ 在离散化端点上的最大覆盖次数 $\ge m$”（引理 1、引理 2）。
- **classic problem solution** — 证明可行窗口左端点随右端点单调递增（引理 3），用双指针 + 区间加/全局最大值线段树在 $O(n \log n)$ 时间内求解。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 区间总数与需选区间数 | 标准输入 | 维度 |
| `a[i]` | `itv [mxn]` | 离散化后的区间 `{l, r, len}` | 按 `len` 升序排序 | intervals |
| `vals` | `int [mxn*2]` | 排序去重后的原始端点表，长度 `vcnt` | 离散化 | coordinate values |
| `mx[i], tag[i]` | `int [mxn*8]` | 线段树节点的最大覆盖次数与区间加懒标记 | 全 `0` | segment tree |
| `ans` | `int` | 最小长度差 | 初值 `inf` | answer |

### 4.2 算法步骤

1. 读入 $n, m$ 及所有区间 $[l_i, r_i]$，记录原始长度 `len = r - l`，并将所有 $l_i, r_i$ 放入 `vals` 排序去重得到 `vcnt` 个离散坐标。
2. 将每个区间的 $l_i, r_i$ 替换为其在 `vals` 中的下标（$0 \le l_i \le r_i < \text{vcnt}$），并将区间数组 `a` 按 `len` 从小到大排序。
3. 初始化双指针 `l = 0, r = 0`，`ans = inf`。
4. 枚举右指针 `r = 0 .. n-1`：
   - 在线段树上将区间 $[a[r].l, a[r].r + 1)$ 整体加 $1$；
   - 当根节点最大值 `mx[0] >= m` 时，用 `a[r].len - a[l].len` 更新 `ans` 的最小值，随后在线段树上将 $[a[l].l, a[l].r + 1)$ 整体减 $1$ 并令 `++l`。
5. 若 `ans == inf` 输出 `-1`，否则输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：离散化与排序耗时 $O(n \log n)$；双指针过程中每个区间恰好入窗一次（线段树 $+1$）、出窗至多一次（线段树 $-1$），每次线段树操作 $O(\log n)$，总时间复杂度为 $O(n \log n)$。
- **空间复杂度**：离散化数组与 $4 \times 2n$ 线段树数组，空间复杂度为 $O(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1712 [NOI2016] 区间
// https://www.luogu.com.cn/problem/P1712
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{500005};
constexpr int inf{1145141919};

int n,m;
struct itv{int l,r,len;}a[mxn];
int vals[mxn*2],vcnt;
int mx[mxn*8],tag[mxn*8];

int ls(int i){return i*2+1;}
int rs(int i){return i*2+2;}

void apply(int i,int v)
{
	mx[i]+=v;
	tag[i]+=v;
}
void push_down(int i)
{
	if(tag[i])
	{
		apply(ls(i),tag[i]);
		apply(rs(i),tag[i]);
		tag[i]=0;
	}
}
void upd(int L,int R,int v,int l,int r,int i)
{
	if(R<=l||r<=L)
		return;
	if(L<=l&&r<=R)
	{
		apply(i,v);
		return;
	}
	push_down(i);
	int mid=(l+r)>>1;
	upd(L,R,v,l,mid,ls(i));
	upd(L,R,v,mid,r,rs(i));
	mx[i]=max(mx[ls(i)],mx[rs(i)]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		cin>>a[i].l>>a[i].r;
		a[i].len=a[i].r-a[i].l;
		vals[vcnt++]=a[i].l;
		vals[vcnt++]=a[i].r;
	}
	sort(vals,vals+vcnt);
	vcnt=unique(vals,vals+vcnt)-vals;
	for(int i=0;i<n;++i)
	{
		a[i].l=lower_bound(vals,vals+vcnt,a[i].l)-vals;
		a[i].r=lower_bound(vals,vals+vcnt,a[i].r)-vals;
	}
	sort(a,a+n,[](const itv&x,const itv&y){return x.len<y.len;});

	int ans=inf;
	for(int l=0,r=0;r<n;++r)
	{
		upd(a[r].l,a[r].r+1,1,0,vcnt,0);
		while(mx[0]>=m)
		{
			ans=min(ans,a[r].len-a[l].len);
			upd(a[l].l,a[l].r+1,-1,0,vcnt,0);
			++l;
		}
	}
	cout<<(ans==inf?-1:ans)<<'\n';
	return 0;
}
```
