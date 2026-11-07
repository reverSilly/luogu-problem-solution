# P1937 [USACO10MAR] Barn Allocation G

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $N$ 个畜栏排成一排，第 $i$ 个畜栏的容量是 $C_i$。有 $M$ 头牛，第 $i$ 头申请使用编号在 $[A_i,B_i]$ 内的连续畜栏；为了让它能自由走动，**这个区间里的每个畜栏都要为它预留 $1$ 单位容量**。同一畜栏的预留总量不能超过它的容量。问最多能满足多少头牛。

### 1.2 关键约束与规模暗示

- $N,M\le10^5$，$C_i\le10^5$。
- 请求是区间，容量是每个点上的“叠加次数”上限：若接受牛 $i$，所有 $p\in[A_i,B_i]$ 的已用容量都加 $1$。
- 要对每个请求做“区间最小值查询 + 区间整体减 $1$”，$N,M=10^5$ 提示使用带懒标记的线段树，配合按右端点排序的贪心。
- 需要 $O((N+M)\log N)$，$O(NM)$ 或每次暴力扫区间都会超时。

### 1.3 题面歧义与样例判定

“在 $A_i\sim B_i$ 范围内的畜栏中为它预留至少一单位容量”可以有两种读法：

1. 只要区间内**某个**畜栏留 $1$ 单位即可；
2. 区间内**每个**畜栏都要留 $1$ 单位。

样例可以判定：

- 容量为 $(1,3,2,1,3)$，四头牛分别为 $[1,3],[2,5],[2,3],[4,5]$。
- 若按读法 1，把四头牛分别安排到畜栏 $1,5,2,4$ 即可，四头全满足；但样例答案是 $3$。
- 样例解释明确说“畜栏 $3,4$ 请求太多了”：畜栏 $3$ 被牛 $1,2,3$ 同时占用（$3>2$），畜栏 $4$ 被牛 $2,4$ 同时占用（$2>1$）。

所以本题按**读法 2**：接受一个请求 = 把它区间内每个点的负载加 $1$。下面都基于这个模型。

## 2. 思维链（Chain-of-thought）

### 2.1 形式化

设接受集合为 $S$。对每个畜栏 $p$，定义负载
$$
\operatorname{load}_S(p)=\bigl|\{i\in S: A_i\le p\le B_i\}\bigr|.
$$
约束是 $\operatorname{load}_S(p)\le C_p$ 对所有 $p$ 成立。目标是最大化 $|S|$。

把“已用容量”记成剩余量
$$
r_p=C_p-\operatorname{load}_S(p).
$$
接受区间 $I=[A,B]$ 的充要条件就是
$$
\min_{p\in I} r_p\ge1.
$$
接受后再令所有 $p\in I$ 的 $r_p$ 减 $1$。

### 2.2 贪心策略

按**右端点 $B_i$ 从小到大**排序，依次尝试每个请求：若当前 $r$ 在 $[A_i,B_i]$ 上的最小值 $\ge1$，就接受它并做区间减 $1$；否则跳过。

直观理由：右端点越早，区间越“早结束”，对后面的请求占用未来位置的时间越短，应该优先满足。

### 2.3 正确性证明（交换论证）

**引理 1（接受判定的精确性）**

设当前已接受集合 $S$ 可行。则 $S\cup\{I\}$ 可行，当且仅当 $\min_{p\in I} r_p\ge1$。

**证明**：加入 $I$ 只会让 $I$ 内每个点的负载加 $1$，其余点不变。故可行当且仅当对每个 $p\in I$ 都有 $\operatorname{load}_S(p)+1\le C_p$，即 $r_p\ge1$；对所有 $p\in I$ 同时成立等价于区间最小值 $\ge1$。$\blacksquare$

接下来证明贪心最优。设请求已按右端点非降序编号为 $1,2,\dots,M$，$I_k$ 表示第 $k$ 个请求，$B_k$ 为其右端点。设 $G$ 为贪心接受集合，$O$ 为一个最优解，并且在所有最优解中，$O$ 与 $G$ 的**相同前缀最长**：即存在最大的 $k$，使得对所有 $j<k$，$I_j\in G\iff I_j\in O$。

假设 $G\ne O$，取第一个分歧位置 $k$。分两种情况。

**情况 A：贪心拒绝 $I_k$，而 $O$ 接受 $I_k$。**

贪心拒绝意味着 $G_{k-1}\cup\{I_k\}$ 不可行（引理 1），于是存在 $p\in I_k$ 使
$$
\operatorname{load}_{G_{k-1}}(p)=C_p.
$$
由于 $O$ 与 $G$ 在前 $k-1$ 个请求上完全一致，$O$ 中覆盖 $p$ 的“前缀请求”恰好也是 $G_{k-1}$ 中覆盖 $p$ 的那些；再加上 $I_k$ 本身覆盖 $p$，得到
$$
\operatorname{load}_{O}(p)\ge \operatorname{load}_{G_{k-1}}(p)+1=C_p+1>C_p,
$$
与 $O$ 可行矛盾。故情况 A 不会发生。

**情况 B：贪心接受 $I_k$，而 $O$ 不接受 $I_k$。**

因为贪心接受，$G_{k-1}\cup\{I_k\}$ 可行，所以对每个 $p\in I_k$ 有
$$
\operatorname{load}_{G_{k-1}}(p)\le C_p-1. \tag{1}
$$

令
$$
X=\{J\in O\setminus G_{k-1}: J\text{ 覆盖 }B_k\}.
$$
注意 $O\setminus G_{k-1}$ 中的请求编号都 $\ge k$，且 $I_k\notin O$，故它们的编号 $>k$，从而右端点 $\ge B_k$（排序保证）。

若 $X=\varnothing$：对任意 $p\in I_k$ 和任意 $J\in O\setminus G_{k-1}$ 覆盖 $p$，有 $A_J\le p\le B_k\le B_J$，因此 $J$ 也覆盖 $B_k$，即 $J\in X$，矛盾。所以没有任何“额外请求”覆盖 $I_k$ 中的点。结合 (1)，
$$
\operatorname{load}_{O\cup\{I_k\}}(p)=\operatorname{load}_{G_{k-1}}(p)+1\le C_p
$$
对 $p\in I_k$ 成立，其余点不变，于是 $O\cup\{I_k\}$ 可行，规模比 $O$ 大 $1$，与 $O$ 最优矛盾。故 $X\ne\varnothing$。

取 $J\in X$ 使左端点 $A_J$ 最小，令
$$
O'=(O\setminus\{J\})\cup\{I_k\}.
$$
验证 $O'$ 可行：

- 对 $p\notin I_k$：加入 $I_k$ 不影响；删去 $J$ 只会让负载不增。✓
- 对 $p\in I_k$ 且 $p<A_J$：若存在 $J'\in O\setminus G_{k-1}$ 覆盖 $p$，则 $A_{J'}\le p<A_J$ 且 $B_{J'}\ge B_k\ge p$，故 $J'$ 覆盖 $B_k$，即 $J'\in X$，与 $A_J$ 的最小性矛盾。因此没有额外请求覆盖 $p$，由 (1) 有
  $$
  \operatorname{load}_{O'}(p)=\operatorname{load}_{G_{k-1}}(p)+1\le C_p.
  $$
  ✓
- 对 $p\in I_k$ 且 $p\ge A_J$：由 $J\in X$ 知 $A_J\le B_k\le B_J$，故 $J$ 覆盖整个 $[A_J,B_k]$，特别覆盖 $p$。删去 $J$ 使负载减 $1$，加入 $I_k$ 使负载加 $1$，净变化为 $0$，而 $O$ 本身可行，故
  $$
  \operatorname{load}_{O'}(p)=\operatorname{load}_{O}(p)\le C_p.
  $$
  ✓

所以 $O'$ 也是最优解。并且 $O'$ 与 $G$ 在第 $k$ 个位置也一致：它包含 $G_{k-1}$ 与 $I_k$，而被删去的 $J$ 编号 $>k$。这与“$O$ 具有最长相同前缀”矛盾。

两种情况都矛盾，说明假设 $G\ne O$ 不成立，即 $G$ 就是最优解。$\blacksquare$

### 2.4 数据结构

需要一个数组 $r_0,\dots,r_{N-1}$，支持：

- 区间最小值查询；
- 区间整体加 $-1$。

带懒标记的线段树即可。区间采用半开 $[l,r)$，根下标 $0$，叶子条件 `r-l==1`。

### 2.5 复杂度

- 排序 $O(M\log M)$。
- 每个请求一次区间查询 + 至多一次区间修改，各 $O(\log N)$。
- 总时间 $O((N+M)\log N)$，空间 $O(N)$。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 用样例确认“每个点都要预留容量”，问题变成区间负载不超过点容量。
- **explore its property** — 维护剩余容量 $r_p$，接受条件化为区间最小值 $\ge1$。
- **exchange argument** — 按右端点排序；用“最长相同前缀”的最优解做交换，证明贪心不劣。
- **classic problem solution** — 线段树支持区间加与区间最小值，$O((N+M)\log N)$。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n,m` | `int` | 畜栏数、请求数 | 读入 | — |
| `c[i]` | `int[100005]` | 第 $i$ 个畜栏的容量 | 读入 | capacity |
| `Q[i]` | `struct{int a,b;}` | 一个请求的左右端点（0-based 闭区间） | 读入后 `--` | query |
| `mn[]` | `int[4*100005]` | 线段树区间最小值 | 由 `c` 建树 | minimum |
| `lz[]` | `int[4*100005]` | 区间加的懒标记 | $0$ | lazy |
| `ans` | `int` | 接受的请求数 | $0$ | answer |

### 4.2 算法步骤

1. 读入 $N,M$ 与容量 $c_0,\dots,c_{N-1}$。
2. 读入 $M$ 个请求 $(A_i,B_i)$，转成 0-based 的闭区间 $[A_i-1,B_i-1]$。
3. 按右端点升序排序（右端点相同则按左端点升序，顺序不影响正确性）。
4. 以 $c$ 建线段树（区间最小值 + 懒标记加）。
5. 依次处理每个请求 $[l,r]$：
   - 查询 $[l,r+1)$ 的最小值；
   - 若 $>0$：对 $[l,r+1)$ 区间加 $-1$，`++ans`。
6. 输出 `ans`。

### 4.3 复杂度分析

- 时间 $O((N+M)\log N)$。
- 空间 $O(N)$。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m;
int c[mxn];
struct req
{
	int a,b;
};
req Q[mxn];
int mn[4*mxn],lz[4*mxn];

void build(int l,int r,int i)
{
	if(r-l==1)
	{
		mn[i]=c[l];
		return;
	}
	int mid=(l+r)>>1;
	build(l,mid,i*2+1);
	build(mid,r,i*2+2);
	mn[i]=min(mn[i*2+1],mn[i*2+2]);
}

void push(int i)
{
	if(lz[i])
	{
		mn[i*2+1]+=lz[i];
		lz[i*2+1]+=lz[i];
		mn[i*2+2]+=lz[i];
		lz[i*2+2]+=lz[i];
		lz[i]=0;
	}
}

void upd(int L,int R,int v,int l,int r,int i)
{
	if(L<=l&&r<=R)
	{
		mn[i]+=v;
		lz[i]+=v;
		return;
	}
	push(i);
	int mid=(l+r)>>1;
	if(L<mid)
		upd(L,R,v,l,mid,i*2+1);
	if(R>mid)
		upd(L,R,v,mid,r,i*2+2);
	mn[i]=min(mn[i*2+1],mn[i*2+2]);
}

int qry(int L,int R,int l,int r,int i)
{
	if(L<=l&&r<=R)
		return mn[i];
	push(i);
	int mid=(l+r)>>1;
	int res=1145141919;
	if(L<mid)
		res=min(res,qry(L,R,l,mid,i*2+1));
	if(R>mid)
		res=min(res,qry(L,R,mid,r,i*2+2));
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>c[i];
	for(int i=0;i<m;++i)
	{
		cin>>Q[i].a>>Q[i].b;
		--Q[i].a;
		--Q[i].b;
	}
	sort(Q,Q+m,[](req x,req y){return x.b<y.b||(x.b==y.b&&x.a<y.a);});
	build(0,n,0);
	int ans=0;
	for(int i=0;i<m;++i)
	{
		int l=Q[i].a,r=Q[i].b;
		if(qry(l,r+1,0,n,0)>0)
		{
			upd(l,r+1,-1,0,n,0);
			++ans;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
```
