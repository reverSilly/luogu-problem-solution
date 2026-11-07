# P1856 [IOI 1998 / USACO5.5] 矩形周长 Picture

## 1. 重新读题

### 1.1 问题重述（人话版）

给出 $N$ 个边平行于坐标轴的矩形（左下角、右上角坐标均为整数，坐标可为负）。它们可以互相重叠、包含。求这 $N$ 个矩形**并集**的周长（只算外边界，被其他矩形盖住的边不算）。

### 1.2 关键约束与规模暗示

- $1\le N<5000$，坐标范围 $[-10^4,10^4]$。
- 不能枚举格子（坐标范围 $2\times10^4$，二维是 $4\times10^8$，且 $N$ 次矩形覆盖也吃不消）。
- 标准做法是**扫描线 + 线段树**：沿 $x$ 扫描，用线段树维护 $y$ 方向上的覆盖长度与覆盖段数。
- $N<5000$ 允许 $O(N\log N)$；坐标需要离散化（或直接用值域 $20005$ 的线段树）。
- 空行、负数坐标、矩形相互包含都需要处理。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：把周长拆成竖边与横边

并集的边界由若干竖直段和水平段组成。沿 $x$ 从 $-\infty$ 扫到 $+\infty$：

- **竖直边界**出现在某个矩形的左右边界所在直线 $x=x_0$ 上；
- **水平边界**出现在两个相邻“事件坐标”之间的开区间 $(x,x')$ 内。

于是分别统计。

**引理 1（横边公式）**：设扫过 $x$ 处所有事件之后，当前被覆盖的 $y$ 集合恰好由 $c$ 个互不相交的连续段组成，且下一个事件坐标为 $x'$，则区间 $(x,x')$ 内的水平边界总长为
$$
2c\,(x'-x).
$$

**证明**：在开区间 $(x,x')$ 内，所有矩形的 $x$ 覆盖情况不变，因此被覆盖的 $y$ 集合恒定，为 $c$ 个互不相交的连续段 $[l_1,r_1],\dots,[l_c,r_c]$。对每个连续段 $[l_t,r_t]$，它在整个开区间 $(x,x')$ 上都不变，所以它的上边界 $y=r_t$ 与下边界 $y=l_t$ 各自在 $(x,x')$ 上完整地属于并集边界，长度都是 $x'-x$。$c$ 个段共贡献 $2c$ 条这样的水平线段，总长 $2c\,(x'-x)$。$\blacksquare$

### 2.2 竖直边界：为什么不能简单取“长度差”

**错误想法**：在每个事件处，用 $|\text{新覆盖长度}-\text{旧覆盖长度}|$ 作为竖边长度。

这个想法在“同一 $x$ 处既有矩形结束又有矩形开始”时会出错。看一个具体反例：

- 矩形 $A=[-1,0]\times[0,2]$，矩形 $B=[0,1]\times[1,3]$。

在 $x=0$ 处，$A$ 结束、$B$ 开始。若先删 $A$：长度 $2\to0$，贡献 $2$；再加 $B$：$0\to2$，贡献 $2$；合计 $4$。但真实的竖边界只有两段：$A$ 右边在 $y\in[0,1]$ 的部分和 $B$ 左边在 $y\in[2,3]$ 的部分，总长 $2$。$y\in[1,2]$ 的那部分被对方挡住，不是边界。

原因：竖直边界的长度是**左侧覆盖集合与右侧覆盖集合的对称差**，不能只用两个长度相减。

### 2.3 修复：同一 $x$ 内“先加后删”

把 $x=x_0$ 处的矩形分成三部分：

- $L$：在 $x_0$ 结束的矩形的 $y$ 区间并集；
- $R$：在 $x_0$ 开始的矩形的 $y$ 区间并集；
- $M$：跨越 $x_0$ 的矩形的 $y$ 区间并集。

左侧覆盖集合是 $C^-=L\cup M$，右侧是 $C^+=R\cup M$。

**引理 2（竖边公式）**：在 $x_0$ 处的竖边界总长为
$$
m\big((L\cup M)\;\triangle\;(R\cup M)\big)
= m\big(R\setminus(L\cup M)\big)+m\big(L\setminus(R\cup M)\big),
$$
其中 $m$ 表示一维长度，$\triangle$ 为对称差。

**证明**：一点 $y$ 若两侧都被覆盖，则它处于并集内部（矩形有正宽度，可在 $y$ 上下小扰动仍被覆盖），不是边界；若两侧都未被覆盖，则它不在并集闭包中，也不是边界；只有恰好一侧被覆盖时才是竖边界。所以竖边界恰为对称差，其长度可分解为两个互不相交的部分 $R\setminus(L\cup M)$ 与 $L\setminus(R\cup M)$。$\blacksquare$

**引理 3（先加后删的实现等价性）**：若先把所有 $R$ 类事件加入（此时覆盖集合从 $C^-$ 变为 $C^-\cup R$），再删除所有 $L$ 类事件（变为 $(C^-\cup R)\setminus L=C^+$），并把每个单次事件造成的覆盖长度变化量取绝对值累加，则总和恰等于引理 2 的竖边界长度。

**证明**：加入阶段长度单调不减，逐个累加 $|\Delta|$ 望远镜等于
$$
m(C^-\cup R)-m(C^-)=m\big(R\setminus C^-\big)=m\big(R\setminus(L\cup M)\big).
$$
删除阶段长度单调不增，逐个累加 $|\Delta|$ 望远镜等于
$$
m(C^-\cup R)-m\big((C^-\cup R)\setminus L\big)
=m\big(L\setminus(C^-\cup R)\big)
=m\big(L\setminus(R\cup M)\big).
$$
两段相加正是引理 2 的右侧。$\blacksquare$

这就是排序的关键：**同一 $x$ 的事件中，加入（矩形左边界）必须排在删除（矩形右边界）之前**。若顺序反了，就会出现 2.2 的双倍计数。

### 2.4 线段树维护什么

离散化所有矩形的 $y$ 坐标，得到 $y_0<y_1<\dots<y_{k-1}$，线段树的第 $i$ 个叶子表示半开区间 $[y_i,y_{i+1})$，共 $k-1$ 个叶子。

每个结点维护：

- `cov`：完全覆盖该结点区间的矩形层数；
- `len`：该结点区间内被覆盖的总长度；
- `num`：该结点区间内覆盖集合的连续段数；
- `lc, rc`：左/右端点是否被覆盖。

**引理 4（合并公式）**：
$$
\mathit{num}=\mathit{num}_L+\mathit{num}_R-\big[\mathit{rc}_L\land \mathit{lc}_R\big].
$$

**证明**：左右两半的连续段拼接时，只有当左半的右端点与右半的左端点都被覆盖时，这两段才连成一段，段数减一；其余情况段数相加。$\blacksquare$

当 `cov>0` 时，整个结点区间被完全覆盖：`len` 直接是该区间坐标长度，`num=1`，`lc=rc=true`，不需要看孩子（但孩子仍需在递归更新中维护，以便 `cov` 降回 $0$ 时能正确合并）。

### 2.5 整体流程

1. 读入矩形，收集所有 $y$ 坐标并离散化。
2. 每个矩形拆成两个事件：$(x_1,+1)$ 与 $(x_2,-1)$，附带 $y$ 区间对应的叶子范围。
3. 按 $x$ 升序排序；$x$ 相同时，$+1$（加入）在前，$-1$（删除）在后。
4. 依次处理：每次线段树更新后，竖边答案累加 `|len_root - last_len|`，并更新 `last_len`。
5. 处理完同一 $x$ 的全部事件后，横边答案累加 `2 * num_root * (next_x - x)`。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 周长 = 竖边 + 横边，分别沿扫描线统计。
- **find counterexample** — 用“$A$ 结束、$B$ 开始但互相遮挡”的反例推翻“长度差”做法。
- **derive lemma** — 竖边界等于左右覆盖集合的对称差，可拆成两类新暴露部分。
- **fix event order** — 同一 $x$ 处先加入后删除，使逐事件 $|\Delta|$ 望远镜求和正确。
- **classic problem solution** — 扫描线 + 线段树维护覆盖长度、连续段数、端点覆盖标记。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 矩形数 | 读入 | $\le 5000$ |
| `ys[k]` | `int [mxn]` | 离散化后的 $y$ 坐标 | 所有 $y_1,y_2$ 去重排序 | y-scale |
| `ev[cnt]` | `struct{x,t,y1,y2}` | 扫描事件 | 每个矩形两个 | event |
| `ev.t` | `int` | $+1$ 加入，$-1$ 删除 | —— | type |
| `cov[i]` | `int [4*mxn]` | 结点被整段覆盖的层数 | 0 | cover |
| `len[i]` | `int [4*mxn]` | 结点内覆盖长度 | 0 | length |
| `num[i]` | `int [4*mxn]` | 结点内覆盖连续段数 | 0 | number |
| `lc[i], rc[i]` | `bool [4*mxn]` | 左右端点是否被覆盖 | false | left/right cover |
| `last` | `int` | 上一次事件后的根覆盖长度 | 0 | last length |
| `ans` | `i64` | 周长 | 0 | 输出 |

### 4.2 算法步骤

1. 读入矩形，把 $y_1,y_2$ 全部放入 `ys`，排序去重。
2. 为每个矩形生成两个事件，$y$ 区间用 `lower_bound` 转成叶子下标区间 $[\,[l,r)\,]$。
3. 事件按 $(x,\ -t)$ 排序（$+1$ 在 $-1$ 前）。
4. 线段树初始化全零。按 $x$ 分组扫描：
   - 组内按序更新，累加 `abs(len[0]-last)`，刷新 `last`；
   - 组结束若还有下一组，累加 `2*num[0]*(next_x-x)`。
5. 输出 `ans`。

### 4.3 复杂度分析

- 时间：事件排序 $O(N\log N)$；每次线段树更新 $O(\log N)$，共 $2N$ 次；总计 $O(N\log N)$。
- 空间：$O(N)$（离散化坐标、事件、线段树）。

---

## 5. 示例代码

```cpp
//
// 题目：P1856 [IOI 1998 / USACO5.5] 矩形周长 Picture
// https://www.luogu.com.cn/problem/P1856
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

struct rect
{
	int x1,y1,x2,y2;
};

struct event
{
	int x,t,l,r;                 // t=+1 加入，-1 删除；[l,r) 为叶子区间
};

int n,cnt,ysn;
rect R[mxn];
event ev[mxn*2];
int ys[mxn*2];
int cov[mxn*8],len[mxn*8],num[mxn*8];
bool lc[mxn*8],rc[mxn*8];

bool byx(event a,event b)
{
	if(a.x!=b.x)
		return a.x<b.x;
	return a.t>b.t;              // 同一 x：先加后删
}

void pushup(int l,int r,int i)
{
	if(cov[i]>0)
	{
		len[i]=ys[r]-ys[l];
		num[i]=1;
		lc[i]=rc[i]=true;
	}
	else if(r-l==1)
	{
		len[i]=0;
		num[i]=0;
		lc[i]=rc[i]=false;
	}
	else
	{
		int L=i*2+1,Rr=i*2+2;
		len[i]=len[L]+len[Rr];
		num[i]=num[L]+num[Rr]-(rc[L]&&lc[Rr]?1:0);
		lc[i]=lc[L];
		rc[i]=rc[Rr];
	}
}

void upd(int l,int r,int i,int ql,int qr,int d)
{
	if(ql<=l&&r<=qr)
	{
		cov[i]+=d;
		pushup(l,r,i);
		return;
	}
	int mid=(l+r)>>1;
	if(ql<mid)
		upd(l,mid,i*2+1,ql,qr,d);
	if(mid<qr)
		upd(mid,r,i*2+2,ql,qr,d);
	pushup(l,r,i);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>R[i].x1>>R[i].y1>>R[i].x2>>R[i].y2;
		ys[ysn++]=R[i].y1;
		ys[ysn++]=R[i].y2;
	}

	sort(ys,ys+ysn);
	ysn=int(unique(ys,ys+ysn)-ys);

	for(int i=0;i<n;++i)
	{
		int l=int(lower_bound(ys,ys+ysn,R[i].y1)-ys);
		int r=int(lower_bound(ys,ys+ysn,R[i].y2)-ys);
		ev[cnt]={R[i].x1,1,l,r};
		++cnt;
		ev[cnt]={R[i].x2,-1,l,r};
		++cnt;
	}

	sort(ev,ev+cnt,byx);

	i64 ans{0};
	int last{0};
	for(int i=0;i<cnt;)
	{
		int x=ev[i].x;
		int j=i;
		while(j<cnt&&ev[j].x==x)
		{
			upd(0,ysn-1,0,ev[j].l,ev[j].r,ev[j].t);
			ans+=abs(len[0]-last);
			last=len[0];
			++j;
		}
		if(j<cnt)
			ans+=2LL*num[0]*(ev[j].x-x);
		i=j;
	}
	cout<<ans<<'\n';
	return 0;
}
```
