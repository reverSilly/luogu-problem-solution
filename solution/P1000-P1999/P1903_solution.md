# P1903 【模板】带修莫队 / [国家集训队] 数颜色 / 维护队列

## 1. 重新读题

### 1.1 人话重述

有一排 $n$ 支画笔，第 $i$ 支的颜色为 $a_i$（$1\le a_i\le 10^6$）。维护两种操作，共 $m$ 次：

- `Q L R`：询问区间 $[L,R]$ 内一共有多少种不同的颜色；
- `R P C`：把第 $P$ 支画笔的颜色改成 $C$。

操作**强制按输入顺序发生**（即后面的询问看到的是前面所有修改之后的结果），但我们可以离线处理。

### 1.2 关键约束与规模暗示

- $n,m\le 133333$。这个数字是 $O(n^{5/3})$ 级别的经典“带修莫队”标尺：如果只有询问，普通莫队是 $O(n\sqrt n)$；加上修改后时间维变成第三维，块大小要从 $\sqrt n$ 调到 $n^{2/3}$。
- 颜色值 $\le 10^6$，可以开一个值域计数桶 `cnt[]`，用“计数由 $0$ 变正 / 由正变 $0$”来 $O(1)$ 维护颜色种数。
- “本题可能轻微卡常数”——常数优化体现在：用三关键字排序 + 合理的块长，避免每次询问重开数组。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先剥掉修改：普通莫队

如果所有操作都是询问，问题就是“静态区间不同颜色数”。做法：

1. 把询问按左端点分块，块内按右端点排序；
2. 维护当前窗口 $[L,R]$ 和桶 `cnt[color]`，以及 `distinct`；
3. 窗口边界一次移动一格，进入/离开时更新桶，$O(1)$；
4. 总移动量 $O(n\sqrt n)$。

**引理 0（桶维护正确）**：设窗口多重集为 $S$。当把一个颜色 $c$ 加入窗口时，若 `cnt[c]` 从 $0$ 变成 $1$，则颜色种数 $+1$，否则不变；把一个颜色 $c$ 移出时，若 `cnt[c]` 从 $1$ 变成 $0$，则颜色种数 $-1$，否则不变。于是 `distinct` 始终等于 $|S|$ 中不同颜色的个数。$\blacksquare$

现在把“修改”加回来。

### 2.2 把修改看成第三个指针：时间维

一次修改只改变**一个位置**的颜色。把所有修改按发生顺序编号 $0,1,\dots,K-1$。一个询问发生之前，已经执行了若干次修改，记这个次数为 $t$。于是每个询问可以写成三元组 $(l,r,t)$。

我们让当前状态也带一个“已执行前 $T$ 次修改”的标记：

- 当前窗口 $[L,R]$；
- 当前数组 `now[]`，等于初始数组执行前 $T$ 次修改后的结果；
- 桶 `cnt[]` 与 `distinct` 描述 `now[L..R]`。

三重指针 $(L,R,T)$ 从上一个询问的三元组移动到下一个询问的三元组，每次只移动一格。移动 $L,R$ 时按引理 0 更新；移动 $T$ 时执行/撤销一次修改。

**引理 1（时间指针的可逆性）**：预先对每次修改记录三元组 $(p,\text{oldc},\text{newc})$，其中 `oldc` 是修改发生前第 $p$ 位的颜色（通过按顺序模拟初始数组得到）。

- 执行第 $T$ 次修改（$T$ 从 $0$ 起）：若 $L\le p\le R$，则从窗口中删去 `oldc`、加入 `newc`；然后 `now[p]=newc`，$T\gets T+1$。
- 撤销第 $T-1$ 次修改：此时第 $p$ 位当前的正是 `newc`；若 $L\le p\le R$，则删去 `newc`、加入 `oldc`；然后 `now[p]=oldc`，$T\gets T-1$。

两个操作严格互逆，且每次都把“当前数组”变成“执行前 $T$ 次修改后的数组”，所以 $T$ 指针任意来回移动都不会破坏状态。$\blacksquare$

**引理 2（三指针到位即答案）**：当三个指针都等于 $(l,r,t)$ 时，`distinct` 恰好是询问的答案。因为引理 1 保证 `now[]` 正确，引理 0 保证桶描述的是 `now[l..r]`。$\blacksquare$

### 2.3 排序方式与块长的完整推导

设询问数为 $Q$（$\le m$），数组长度 $n$，修改数 $K\le m$，块大小为 $B$。排序关键字为

$$
\Big(\big\lfloor l/B\big\rfloor,\ \big\lfloor r/B\big\rfloor,\ t\Big).
$$

分别估计三个指针的总移动量。

**$L$ 指针**：同一个 $l$ 块内，相邻询问的 $l$ 之差不超过 $B$，故块内累计 $O(QB)$；跨块时 $l$ 的总跳变也只有 $O(n)$ 量级。因此
$$
\text{cost}_L=O(QB).
$$

**$R$ 指针**：固定一个 $l$ 块。该块内的询问按 $r$ 块升序排列。在同一个 $r$ 块内，$r$ 被限制在宽度 $B$ 的区间内，但第三关键字 $t$ 会使 $r$ 在这个宽度 $B$ 内来回摆动，最坏每次询问 $O(B)$，故这一部分总计 $O(QB)$。而 $r$ 块之间是单调推进的，一个 $l$ 块最多扫过 $O(n)$ 的 $r$ 跨度，$l$ 块共 $n/B$ 个，故这部分 $O(n^2/B)$。于是
$$
\text{cost}_R=O\!\left(QB+\frac{n^2}{B}\right).
$$

**$T$ 指针**：固定一对 $(l\text{ 块},r\text{ 块})$ 后，块内询问按 $t$ 升序，$T$ 单调推进，总移动量不超过 $m$。这样的二元块共有 $(n/B)^2$ 个，故
$$
\text{cost}_T=O\!\left(m\cdot\frac{n^2}{B^2}\right).
$$

于是总移动量
$$
O\!\left(QB+\frac{n^2}{B}+\frac{mn^2}{B^2}\right).
$$

取 $Q\approx m\approx n$，令 $B$ 平衡第一项与第三项：
$$
nB=\frac{n^3}{B^2}\ \Longrightarrow\ B^3=n^2\ \Longrightarrow\ B=n^{2/3}.
$$
此时
$$
nB=n^{5/3},\qquad \frac{n^2}{B}=n^{4/3},\qquad \frac{n^3}{B^2}=n^{5/3},
$$
故总复杂度
$$
\boxed{O\!\left(n^{5/3}\right)}.
$$
这也解释了为什么不能用 $B=\sqrt n$：那时第三项会退化成 $O(n^2)$。

### 2.4 实现中的坑

- 修改要记录 `old` 颜色，必须在读入时用一份模拟数组按顺序算出来；
- 询问的 $t$ 是“它之前已经发生了多少次修改”，不是它在输入中的下标；
- 排序比较器里对 $l/B$、$r/B$、$t$ 三级比较，缺一不可；
- 颜色值域 $10^6$，`cnt` 开到 $10^6+5$；
- 最后按询问原始编号输出。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 静态区间不同颜色数可用桶 + 双指针增量维护。
- **consider easier version** — 先做无修改的普通莫队，得到 $O(n\sqrt n)$。
- **add the missing dimension** — 把“第几次修改”当成时间维 $t$，询问变成三元组 $(l,r,t)$。
- **three-key sorting** — 按 $(l/B,r/B,t)$ 排序，使三个指针的总移动量可控。
- **block size optimization** — 平衡 $L$ 与 $T$ 的代价，取 $B=n^{2/3}$，得到 $O(n^{5/3})$。
- **reversibility** — 修改可执行可撤销，故时间指针可以来回走。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| $n,m$ | `int` | 画笔数、操作数 | 读入 | 规模 |
| `a[i]` | `int[mxn]` | 初始颜色（0-based） | 读入 | array |
| `now[i]` | `int[mxn]` | 当前时刻的数组 | 初始为 `a` | now |
| `cnt[c]` | `int[mxv]` | 当前窗口内颜色 $c$ 的出现次数 | 全 0 | count |
| `Q[i]` | `query[mxn]` | 询问 $(l,r,t,\text{id})$ | 读入时收集 | query |
| `M[i]` | `modify[mxn]` | 修改 $(p,\text{oldc},\text{newc})$ | 读入时模拟得到 old | modify |
| `L,R,T` | `int` | 当前左端点、右端点、已执行修改数 | $0,-1,0$ | 三指针 |
| `distinct` | `int` | 当前窗口颜色种数 | $0$ | 答案 |
| `B` | `int` | 块大小 | $\max(1,\lfloor n^{2/3}\rfloor)$ | block |

### 4.2 算法步骤

1. 读入 $n,m$、初始数组 `a`。
2. 按输入顺序扫描操作：
   - 询问：记录 $(l,r,nm,\text{id})$，其中 `nm` 是此前修改次数；
   - 修改：记录 $(p,\text{now}[p],c)$，并令 `now[p]=c`。
3. 取 $B=\max(1,\lfloor n^{2/3}\rfloor)$，把询问按 $(l/B,r/B,t)$ 升序排序。
4. 恢复 `now=a`，清空 `cnt`，置 $L=0,R=-1,T=0,\text{distinct}=0$。
5. 依次处理询问 $(ql,qr,qt)$：
   - 把 $T$ 推到 `qt`（向前执行或向后撤销修改，按引理 1 更新窗口）；
   - 把 $L$ 推到 `ql`、$R$ 推到 `qr`（扩张 / 收缩窗口，按引理 0 更新桶）；
   - 记录 `ans[id]=distinct`。
6. 按编号输出所有答案。

### 4.3 复杂度

- 时间：$O\!\left(QB+n^2/B+mn^2/B^2\right)$，取 $B=n^{2/3}$ 且 $Q,m=O(n)$ 时为 $O(n^{5/3})$。
- 空间：$O(n+m+10^6)$，即数组、操作与颜色桶。

---

## 5. 示例代码

```cpp
//
// 题目：P1903 【模板】带修莫队 / [国家集训队] 数颜色 / 维护队列
// https://www.luogu.com.cn/problem/P1903
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{133340};
constexpr int mxv{1000005};

int n,m;
int a[mxn];
int cnt[mxv];

struct query
{
	int l,r,t,id;
}Q[mxn];

struct modify
{
	int p,oldc,newc;
}M[mxn];

int ans[mxn];
int nq{0},nm{0};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>a[i];

	static int now[mxn];
	for(int i=0;i<n;++i)
		now[i]=a[i];

	for(int i=0,p,c,l,r;i<m;++i)
	{
		char ch;
		cin>>ch;
		if(ch=='Q')
		{
			cin>>l>>r;--l;--r;
			Q[nq]={l,r,nm,nq};
			++nq;
		}
		else
		{
			cin>>p>>c;--p;
			M[nm]={p,now[p],c};
			now[p]=c;
			++nm;
		}
	}

	int B=max(1,(int)pow((double)n,2.0/3.0));
	sort(Q,Q+nq,[B](const query&x,const query&y){
		int bx=x.l/B,by=y.l/B;
		if(bx!=by)return bx<by;
		int rx=x.r/B,ry=y.r/B;
		if(rx!=ry)return rx<ry;
		return x.t<y.t;
	});

	int L=0,R=-1,T=0;
	int distinct{0};
	for(int i=0;i<n;++i)
		now[i]=a[i];
	for(int i=0;i<nq;++i)
	{
		int ql=Q[i].l,qr=Q[i].r,qt=Q[i].t;
		while(T<qt)
		{
			int p=M[T].p,oc=M[T].oldc,nc=M[T].newc;
			if(L<=p&&p<=R)
			{
				if(--cnt[oc]==0)--distinct;
				if(cnt[nc]++==0)++distinct;
			}
			now[p]=nc;
			++T;
		}
		while(T>qt)
		{
			--T;
			int p=M[T].p,oc=M[T].oldc,nc=M[T].newc;
			if(L<=p&&p<=R)
			{
				if(--cnt[nc]==0)--distinct;
				if(cnt[oc]++==0)++distinct;
			}
			now[p]=oc;
		}
		while(L>ql)
		{
			--L;
			if(cnt[now[L]]++==0)++distinct;
		}
		while(R<qr)
		{
			++R;
			if(cnt[now[R]]++==0)++distinct;
		}
		while(L<ql)
		{
			if(--cnt[now[L]]==0)--distinct;
			++L;
		}
		while(R>qr)
		{
			if(--cnt[now[R]]==0)--distinct;
			--R;
		}
		ans[Q[i].id]=distinct;
	}

	for(int i=0;i<nq;++i)
		cout<<ans[i]<<'\n';
	return 0;
}
```
