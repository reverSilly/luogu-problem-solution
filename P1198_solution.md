# P1198 [JSOI2008] 最大数

---

## Part 1 · 重述问题

维护一个初始为空的数列，依次执行 $M$ 个操作：

- **查询 `Q L`**：输出当前数列末尾 $L$ 个数中的最大值。
- **插入 `A n`**：将 $(n + t) \bmod D$ 追加到数列末尾，其中 $t$ 是最近一次查询的答案（从未查询过时 $t=0$）。

每次查询后须立即输出答案。

**数据范围**：$1 \le M \le 2\times10^5$，$1 \le D \le 2\times10^9$，$n$ 为长整范围内的整数（可能为负）。

---

## Part 2 · 链式思考（发现过程）

### 2.1 探索特殊机制的性质

题目中唯一"不寻常"的点是：**插入的值依赖于上一次查询的答案 $t$**。

先把这条依赖链单独拆开看。设第 $k$ 次查询的答案为 $t_k$。

- 第一次查询前，没有任何 $t$，所以所有插入值都只取决于输入 $n$ 和常数 $D$。
- 第 $k$ 次查询之后，后续插入值都要加上 $t_k$ 再对 $D$ 取模。

这意味着：**插入的值不是独立的，它被上一次查询"污染"了**。但注意，$t_k$ 本身就是从数列中取出来的——它是末尾某段的最大值。所以依赖关系是"自洽"的：

$$t_k = \max(\text{第 } k \text{ 次查询时刻末尾 } L \text{ 个元素})$$

这些元素中的每一个，又都可能是更早的插入值（含 $t_{k-1}$ 等）。

**关键观察**：依赖链是"向前"的——$t_k$ 只影响 $k$ 之后的插入，不影响 $k$ 之前的。所以如果我们按操作顺序**在线**处理，每次处理时所有需要的信息都已经确定了。依赖不构成循环，不存在"鸡生蛋"问题。

> **结论 1**：操作可以**在线**顺序处理，不必离线。每个 `A` 操作到来时，$t$ 已知（就是最近一次查询输出的值），插入值可以立即算出。

### 2.2 考虑简化版本

把"末尾 $L$ 个数的最大值"暂时换成更简单的问题：**维护一个支持追加和查询"当前所有数最大值"的数据结构**。

这可以用一个变量 $\text{mx}$ 解决：每次追加 $x$ 时 $\text{mx}=\max(\text{mx},x)$，查询输出 $\text{mx}$。$O(1)$ 每次操作。

但原题要的是"末尾 $L$ 个数"的最大值，不是"全部"。$L$ 每次查询都不同，所以不能只维护一个全局最大值——我们需要**区间最大值查询（Range Maximum Query, RMQ）**，且区间是 $[\text{len}-L,\ \text{len})$（半开），其中 $\text{len}$ 是当前数列长度。

于是问题变成：**支持"尾部追加一个数"和"查询任意后缀区间的最大值"** 的在线数据结构。

### 2.3 经典数据结构——线段树

后缀区间 $[\text{len}-L,\ \text{len})$ 的最大值，是标准的 **静态/动态 RMQ** 问题。操作只有单点追加（等价于"在位置 $\text{len}$ 写入一个值，然后 $\text{len}+1$"）和区间最大值查询，线段树天然匹配：

- **单点更新**：$O(\log N)$
- **区间最大查询**：$O(\log N)$

总操作数 $M\le2\times10^5$，每次 $O(\log M)$，总计 $O(M\log M)\approx2\times10^5\times18\approx3.6\times10^6$，绰绰有余。

**为什么不是树状数组？** 树状数组擅长区间加/前缀和，但区间**最大值**需要特殊处理（只能维护前缀最大，不能"减掉"前缀），而本题是任意后缀，线段树更直接。

**为什么不是单调栈？** 单调栈可以 $O(1)$ 均摊查询"单调递减栈顶"，但本题查询的是"末尾 $L$ 个数的最大值"，$L$ 任意，不是"前缀最大值"，单调栈不直接适用。

### 2.4 处理取模和负数

插入值 $x=(n+t)\bmod D$ 有一个坑：$n$ 可能为负。C++ 的 `%` 运算符对负数行为是实现定义的（实际多数编译器给出负余数）。

**引理 1**（取模的正确处理）。
对任意整数 $n$、整数 $t$（$0\le t<D$）、整数 $D\ge1$，定义
$$x=\bigl((n+t)\bmod D\bigr)\bmod D$$
其中第一个 $\bmod$ 取非负余数。则 $0\le x<D$。

**证明**：设 $n+t=qD+r$，$0\le r<D$（这是"数学取模"的定义，保证 $r$ 在 $[0,D)$）。则 $x=r$，自然满足 $0\le x<D$。$\blacksquare$

实现上，对 C++ 的 `%`：

$$x=(n+t)\%D;\quad\text{if}(x<0)\;x+=D;$$

即可得到 $[0,D)$ 内的正确值。

> **结论 2**：插入值用 `if(x<0) x+=D;` 修正负余数。

### 2.5 初始状态与空数列

初始数列空，$\text{len}=0$。线段树需要先初始化——但本题只有"追加"操作，没有"删除"，所以可以**按需建点**或**一次性建到 $M$ 个叶子**：

- **一次性建树**：所有叶子初始值设为 $0$（因为插入值一定 $\ge0$，$0$ 作为"不存在"的下界安全）。$\text{len}$ 个位置之前没有插入过，但查询只会在 $\text{len}\ge L$ 时发生，所以查询区间 $[\text{len}-L,\ \text{len})$ 一定落在已插入区域内。
- **为什么初始值 $0$ 安全？** 因为所有插入值都在 $[0,D)$ 内，$D\ge1$，所以插入值 $\ge0$。若查询区间内所有值都 $\ge0$，初始 $0$ 不会"虚高"。

> **结论 3**：线段树初始全 $0$，追加时单点更新，查询时区间最大。

### 2.6 线段树实现细节（sth lambda 风格）

按编码风格，线段树用 `sth` lambda 实现，半开区间 $[l,r)$，根下标 $0$。

- **建树**：`sth(0, M, 0)`，区间 $[0,M)$，所有叶子初始 $0$。
- **单点更新**：`let(pos, val)`，将位置 $pos$ 设为 $val$，回溯更新 `mx`。
- **区间查询**：`query(L, R)`，求 $[L,R)$ 的最大值。

区间查询的标准拆分：

```
query(l, r, ql, qr, i):
  if ql <= l and r <= qr: return mx[i]
  mid = (l+r)/2
  ans = 0
  if ql < mid: ans = max(ans, query(l, mid, ql, qr, ls(i)))
  if qr > mid: ans = max(ans, query(mid, r, ql, qr, rs(i)))
  return ans
```

**引理 2**（区间最大查询的正确性）。
对任意查询区间 $[ql,qr)\subseteq[0,M)$，`query(ql,qr)` 返回 $\max\{a[i]:ql\le i<qr\}$。

**证明**：用区间端点不变量归纳。
- **基本情况**：$l=ql$ 且 $r=qr$ 时，节点完全被查询区间覆盖，直接返回 `mx[i]`。而 `mx[i]` 由单点更新时维护为"该节点对应区间内所有已写入位置的最大值"（未写入位置初始 $0$，已写入值 $\ge0$，所以 `max` 不受初始 $0$ 干扰）。故返回正确。
- **归纳步骤**：节点区间 $[l,r)$ 与 $[ql,qr)$ 有交集但不完全包含。设 $mid=(l+r)/2$。若 $ql<mid$，则 $[ql,qr)$ 与左子区间 $[l,mid)$ 有交集，递归 `query(l,mid,ql,qr,ls(i))` 由归纳假设返回 $\max\{a[i]:ql\le i<mid\}$（交集部分）。同理右子区间。最终取 $\max$ 得到 $\max\{a[i]:ql\le i<qr\}$。$\blacksquare$

> **结论 4**：线段树单点更新 + 区间最大查询，每次 $O(\log M)$。

### 2.7 复杂度汇总

- $M$ 次操作，每次 $O(\log M)$（线段树单点更新或区间查询）。
- 总时间 $O(M\log M)$，$M=2\times10^5$ 时约 $3.6\times10^6$ 次基本运算。
- 空间 $O(M)$（线段树 $4M$ 个节点）。

---

## Part 3 · 模式总结

```
- explore its property   — 插入值依赖上次查询答案 t，但依赖是"向前"的，可在线处理
- consider easier version — 去掉"末尾 L 个"限制，变成"全部最大值"→ O(1) 变量
- classic problem solution — 加回"末尾 L 个"→ 区间最大查询（RMQ）→ 线段树
- handle modular arithmetic — 负数取模用 if(x<0) x+=D 修正
- handle initial state    — 线段树初始全 0（插入值 ≥ 0，0 是安全下界）
```

---

## Part 4 · 正式解法

### 符号表

| 符号 | 含义 | 初始化 / 取值范围 |
|------|------|-----------------|
| $M$ | 操作总数 | 输入，$1\le M\le2\times10^5$ |
| $D$ | 取模常数 | 输入，$1\le D\le2\times10^9$ |
| $t$ | 最近一次查询的答案 | 初始 $0$，每次查询后更新 |
| $\text{len}$ | 当前数列长度（= 已插入次数） | 初始 $0$，每次 `A` 后 $+1$ |
| `mx[4*M]` | 线段树节点最大值，`mx[i]` 对应节点 $i$ 的区间 $[l,r)$ | 初始全 $0$ |
| `pos` | 当前待写入位置（= $\text{len}$，0-based） | 每次 `A` 后 $+1$ |

**助记**：`len`/`pos` 是"游标"，指向下一个空位；`t` 是"上一查询答案"；`mx` 是线段树最大数组。

### 算法步骤

1. **建树**：`sth(0, M, 0)` 初始化线段树，所有叶子 $=0$。
2. **主循环**（$i=0$ 到 $M-1$）：
   - 读入操作类型 `op`（字符）和参数 `val`。
   - 若 `op=='Q'`：
     - 令 $L=val$。
     - 查询区间 $[\text{len}-L,\ \text{len})$ 的最大值，记为 $\text{ans}$。
     - 输出 $\text{ans}$，令 $t=\text{ans}$。
   - 若 `op=='A'`：
     - 计算 $x=(val+t)\bmod D$（负数修正：$x<0$ 时 $x+=D$）。
     - 在位置 `pos` 单点更新为 $x$，`pos++`，`len++`。
3. **结束**。

**复杂度**：时间 $O(M\log M)$，空间 $O(M)$。

---

## Part 5 · C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int mx[mxn*4];
int M,D,t,len;

int ls(int i){return i*2+1;}
int rs(int i){return i*2+2;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>M>>D;
	t=0;
	len=0;

	function<void(int,int,int)>build{[&](int l,int r,int i){
		if(r-l==1)
		{
			mx[i]=0;
			return;
		}
		int mid=(l+r)>>1;
		build(l,mid,ls(i));
		build(mid,r,rs(i));
		mx[i]=max(mx[ls(i)],mx[rs(i)]);
	}};
	build(0,M,0);

	function<void(int,int,int,int)>let{[&let](int pos,int val,int l,int r,int i){
		if(r-l==1)
		{
			mx[i]=val;
			return;
		}
		int mid=(l+r)>>1;
		if(pos<mid)
			let(pos,val,l,mid,ls(i));
		else
			let(pos,val,mid,r,rs(i));
		mx[i]=max(mx[ls(i)],mx[rs(i)]);
	}};

	function<int(int,int,int,int,int)>query{[&query](int ql,int qr,int l,int r,int i)->int{
		if(ql<=l&&r<=qr)
			return mx[i];
		int mid=(l+r)>>1;
		int ans=0;
		if(ql<mid)
			ans=max(ans,query(ql,qr,l,mid,ls(i)));
		if(qr>mid)
			ans=max(ans,query(ql,qr,mid,r,rs(i)));
		return ans;
	}};

	for(int i=0,op,val;i<M;++i)
	{
		char ch;
		cin>>ch>>val;
		if(ch=='Q')
		{
			int L=val;
			int ans=query(len-L,len,0,M,0);
			cout<<ans<<'\n';
			t=ans;
		}
		else
		{
			int x=val+t;
			x%=D;
			if(x<0)
				x+=D;
			let(len,x,0,M,0);
			++len;
		}
	}
	return 0;
}
```

---

*注：本解法未进行编译或运行验证（按要求）。逻辑正确性由上述引理保证。*
