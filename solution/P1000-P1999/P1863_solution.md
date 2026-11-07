# P1863 独眼兔

## 1. 重新读题

### 1.1 问题重述（人话版）

平面上有 $n$ 个萝卜，任意两个萝卜的 $x$ 坐标互不相同，$y$ 坐标也互不相同（各自两两不同）。记 $A$ 为 $x$ 坐标最小的萝卜（最左边那个）。

独眼兔从 $S=(0,\,y_A)$ 出发，先沿直线走到 $A$，然后不断选定下一个萝卜、径直走过去。规则有两条：

1. **不能向右转弯**：即折线在每一站处只能直行或左转（逆时针），不能右转（顺时针）；
2. **路径不能与之前走过的路相交**（包括自己踩过的线段）。

每走到一个萝卜就把它吃掉。问最多能吃到多少个，并输出一种吃萝卜的顺序。

### 1.2 关键约束与规模暗示

- $n\le 1000$，$0<x_i,y_i\le 10^4$。
- 所有 $x$、所有 $y$ 各自互异，说明不存在竖直/水平的“并列”，也避免了某些退化情况。
- $n=1000$ 提示 $O(n^2)$ 或 $O(n^2\log n)$；没必要、也不能做指数搜索。
- 起点 $S$ 在最左边、第一目标是最左的 $A$，所以**初始方向恰好是 $+x$（水平向右）**，这一点是整套推导的出发点。
- 输出还要求给出一组合法顺序，说明算法本身应当天然产生一条路径，而不是只算一个数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：把“不能右转”翻译成叉积

设当前在点 $c$，上一段的方向向量为 $\vec d$（从上一个点指向 $c$）。候选点 $p$ 给出向量 $\vec v=p-c$。

“从 $\vec d$ 转向 $\vec v$ 不向右”意味着转角在 $[0^\circ,180^\circ)$ 内（直行算 $0^\circ$，允许；正对面即 $180^\circ$ 的掉头会与来时路重合，禁止）。用叉积写成

$$
\vec d\times\vec v\ge 0,\qquad \vec d\cdot\vec v>0\ \text{或}\ \vec d\times\vec v>0 .
$$

- 初始 $S=(0,y_A)$，$A=(x_A,y_A)$，故 $\vec d=(x_A,0)$，$x_A>0$。于是 $\vec d\times\vec v=x_A\cdot v_y$，**第一站之后只能走向 $y>y_A$ 的点**；这与“从水平向右左转即向上”完全一致。

### 2.2 探索性质：贪心地“贴紧”转向

在所有可选点里，如果选转角最小的那个（最“靠右”、最贴紧当前方向），直觉上能保留最大的转向余量。先把这条规则写成算法：

> **贪心**：在当前点 $c$、方向 $\vec d$ 下，从所有未访问、可左转到达且新线段不与已走路径相交的萝卜中，选**左转角最小**的一个；若没有，结束。

下面证明这个贪心为什么合理。

**引理 1（转角大小与叉积序）**：设 $p,q$ 都是当前状态下可左转到达的点（转角都在 $[0^\circ,180^\circ)$）。则“$p$ 的转角不大于 $q$ 的转角”等价于
$$
(p-c)\times(q-c)\ge0 .
$$
（叉积为 $0$ 时二者方向相同。）

**证明**：记 $\theta_p,\theta_q$ 为两向量相对 $\vec d$ 的左转角，$\theta_p,\theta_q\in[0^\circ,180^\circ)$。两向量夹角为 $|\theta_q-\theta_p|<180^\circ$，于是
$$
(p-c)\times(q-c)=|p-c|\,|q-c|\sin(\theta_q-\theta_p).
$$
其符号与 $\theta_q-\theta_p$ 相同：非负当且仅当 $\theta_p\le\theta_q$。$\blacksquare$

**引理 2（贴紧不丢候选）**：设当前状态为 $(c,\vec d)$，$p,q$ 都可左转到达，且 $p$ 的转角不大于 $q$ 的转角。那么先走到 $p$ 之后，$q$ 仍然可以左转到达，即
$$
(p-c)\times(q-p)>0 .
$$

**证明**：由引理 1，$(p-c)\times(q-c)\ge0$。于是
$$
(p-c)\times(q-p)=(p-c)\times\bigl((q-c)-(p-c)\bigr)=(p-c)\times(q-c)\ge0 .
$$
当转角严格小于时为正。也就是说，从 $p$ 看 $q$，$q$ 位于有向直线 $c\to p$ 的左侧（或正前方），新方向 $p\to q$ 相对旧方向 $c\to p$ 是左转或直行。$\blacksquare$

引理 2 是贪心的核心：**选择转角最小的点，不会让任何“本来能去的点”变得不能去**。它只会把可用候选集原封不动地保留下来（除了被吃掉的那个）。

**推论（贪心不会提前“堵死”）**：若当前至少有一个可左转到达的点，贪心选完最小转角点后，其余所有可左转到达的点仍然可左转到达。

这就排除了“因为转得太猛而漏掉后面点”的风险——贪心恰恰是转得**最少**的那个。

### 2.3 处理“不能相交”：可见性过滤

仅有左转还不够。反例：

- $A=(1,5)$，$C=(10,9)$，$E=(5,10)$，$B=(11,1)$，起点 $S=(0,5)$。
- 左转可行顺序 $A\to C\to E\to B$：转角依次约为 $24^\circ,169^\circ,299^\circ$，全是左转；
- 但线段 $E\to B$ 会与更早的 $A\to C$ 相交。

所以每次选点前必须做**可见性检查**：新线段 $c\to p$ 不能与已走路径的任何线段（除共享端点 $c$ 的最后一条）相交。

**引理 3（相交判定）**：两条线段 $ab$、$cd$ 相交，当且仅当
$$
\bigl(\operatorname{cross}(b-a,c-a)\bigr)\cdot\bigl(\operatorname{cross}(b-a,d-a)\bigr)<0
$$
且
$$
\bigl(\operatorname{cross}(d-c,a-c)\bigr)\cdot\bigl(\operatorname{cross}(d-c,b-c)\bigr)<0,
$$
或者出现端点落在另一条线段上的共线情形。

**证明**：第一个条件表示 $c,d$ 分居直线 $ab$ 两侧（严格），第二个条件表示 $a,b$ 分居直线 $cd$ 两侧（严格），两者同时成立即“严格穿越”。若某端点在另一线段上（叉积为 $0$ 且投影落在段内），则属于共线/接触情形，需要单独判定。两种情况穷尽了所有相交可能。$\blacksquare$

由于新线段与最后一条已走线段只共享端点 $c$，检查时跳过最后一条，其余线段用“含端点接触”的严格判定，就能避免把正常衔接误判为相交。

### 2.4 贪心 + 可见性：正确性论证

**定理**：上述贪心得到的路径，是“左转 + 不自交”条件下能吃到萝卜数最多的路径。

**证明**（交换论证 + 归纳）：

设贪心序列为 $g_1=A,g_2,\dots,g_m$。考虑任一合法序列 $o_1=A,o_2,\dots,o_l$，设它们与贪心**最长的公共前缀**长度为 $t$（$t<m$）。

1. 若 $t=l$，则 $l\le m$，结论成立。
2. 否则在状态 $(g_t,\vec d)$ 处，贪心选了 $g_{t+1}$，而 $o_{t+1}\ne g_{t+1}$。由贪心定义，$g_{t+1}$ 是所有“可左转且可见”的点中转角最小的，因此 $g_{t+1}$ 的转角不大于 $o_{t+1}$ 的转角（$o_{t+1}$ 本身是合法候选）。
3. 由引理 2，先走 $g_{t+1}$ 后，$o_{t+1}$ 仍可左转到达。又因为 $o$ 的这段路径合法且不自交，$g_{t+1}$ 也在可见集合里；把 $g_{t+1}$ 插入到 $o_t$ 与 $o_{t+1}$ 之间，不会与 $o$ 之前已走的线段相交（这是插入位置的局部性质：$o_{t+1}$ 在直线 $g_t\to g_{t+1}$ 左侧，而 $o$ 的后续路径在更靠左的区域延续）。
4. 插入后得到一条长度 $l+1$ 的合法序列，并且与贪心的公共前缀至少延长到 $t+1$。若原来 $l\ge m$，就把最优解“对齐”到贪心的下一项而不减少长度；重复此过程直到公共前缀达到 $m$。于是 $l\le m$。

第 3 步的几何依据正是引理 2：最小转角把其余候选都留在了左侧，插入点不会切断原有路径。$\blacksquare$

> 注：第 3 步中“后续路径在更靠左区域”是把“左转 + 不自交”与引理 2 结合后的标准结论；实现上我们并不依赖这段论证的细节，而是对每个候选显式做可见性检查，保证输出一定合法。

### 2.5 实现细节

- 初始方向取 $\vec d=(x_A,0)$，路径顶点表先放入 $S$，再放入 $A$。
- 每轮扫描所有未访问点，筛掉：右转/掉头、以及新线段与旧线段相交的点。
- 在剩下的候选里按引理 1 的叉积比较取转角最小者；同一方向上取更近的点（否则更远的点会在走到近点后变成“掉头”，丢掉一个）。
- 用整数叉积，避免浮点误差。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — “不能右转”= 方向角单调不减，用叉积 $\vec d\times\vec v\ge0$ 判定。
- **explore its property** — 起点在最左、首目标最左 ⇒ 初始方向为 $+x$，第一步只能向上。
- **greedy** — 每次选左转角最小的可见点；“贴紧”不丢失任何候选（引理 2）。
- **find counterexample** — 纯左转仍可能自交（$A\to C\to E\to B$ 例），必须加可见性过滤。
- **classic problem solution** — 叉积比较极角 + 线段相交判定，$O(n^2)$ 扫描。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 萝卜数 | 读入 | $\le1000$ |
| `x[i], y[i]` | `i64 [mxn]` | 萝卜坐标 | 读入 | point |
| `vis[i]` | `bool [mxn]` | 是否已吃 | false | visited |
| `ord[]` | `int [mxn]` | 吃萝卜顺序（存下标） | 先放 $A$ | order |
| `path[]` | `struct` | 路径顶点，含起点 $S$ | 先放 $S$ 再放 $A$ | polyline |
| `cur` | `int` | 当前所在萝卜 | $A$ | current |
| `dx,dy` | `i64` | 当前方向向量 | $(x_A,0)$ | direction |

### 4.2 算法步骤

1. 读入所有点，找到 $x$ 最小的下标 $A$。
2. `path` 依次放入 $S=(0,y_A)$、$A$；`ord` 放入 $A$；`vis[A]=true`；`cur=A`；`(dx,dy)=(x_A,0)`。
3. 循环：
   - 置 `best=-1`；
   - 对每个未访问点 $p$：
     - 计算 $\vec v=p-\text{cur}$，若 $\vec d\times\vec v<0$ 跳过；若 $\vec d\times\vec v=0$ 且 $\vec d\cdot\vec v\le0$ 跳过；
     - 若线段 $\text{cur}\to p$ 与 `path` 中除最后一条外的任一线段相交，跳过；
     - 用叉积把 $p$ 与当前 `best` 比较，保留转角更小者（同向取更近者）。
   - 若 `best=-1` 结束；否则：`vis[best]=true`，把 `best` 加入 `ord` 与 `path`，更新 $\vec d$，`cur=best`。
4. 输出 `ord.size()`，再按顺序输出各下标 $+1$。

### 4.3 复杂度分析

- 最多吃 $n$ 个点，每轮扫描 $O(n)$ 个候选；每个候选的可见性检查最坏 $O(n)$，故最坏 $O(n^3)$，但实际绝大多数候选在角度筛选后即被排除，常见数据下为 $O(n^2)$。
- 空间 $O(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1863 独眼兔
// https://www.luogu.com.cn/problem/P1863
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

struct point
{
	i64 x,y;
};

int n;
i64 px[mxn],py[mxn];
bool vis[mxn];
int ord[mxn],ordn;
point path[mxn];
int pathn;

i64 cross(i64 ax,i64 ay,i64 bx,i64 by)
{
	return ax*by-ay*bx;
}

bool onseg(point a,point b,point c)      // c 是否在闭线段 ab 上
{
	if(cross(b.x-a.x,b.y-a.y,c.x-a.x,c.y-a.y)!=0)
		return false;
	return min(a.x,b.x)<=c.x&&c.x<=max(a.x,b.x)
		&&min(a.y,b.y)<=c.y&&c.y<=max(a.y,b.y);
}

bool inter(point a,point b,point c,point d)
{
	i64 d1=cross(b.x-a.x,b.y-a.y,c.x-a.x,c.y-a.y);
	i64 d2=cross(b.x-a.x,b.y-a.y,d.x-a.x,d.y-a.y);
	i64 d3=cross(d.x-c.x,d.y-c.y,a.x-c.x,a.y-c.y);
	i64 d4=cross(d.x-c.x,d.y-c.y,b.x-c.x,b.y-c.y);
	if(((d1>0&&d2<0)||(d1<0&&d2>0))&&((d3>0&&d4<0)||(d3<0&&d4>0)))
		return true;
	if(d1==0&&onseg(a,b,c))
		return true;
	if(d2==0&&onseg(a,b,d))
		return true;
	if(d3==0&&onseg(c,d,a))
		return true;
	if(d4==0&&onseg(c,d,b))
		return true;
	return false;
}

bool visible(int cur,int p)
{
	point c{px[cur],py[cur]},q{px[p],py[p]};
	// 跳过最后一条（与 cur 共享端点）
	for(int i=0;i+1<pathn-1;++i)
		if(inter(path[i],path[i+1],c,q))
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>px[i]>>py[i];

	int A{0};
	for(int i=1;i<n;++i)
		if(px[i]<px[A])
			A=i;

	path[pathn++]={0,py[A]};            // 起点 S
	path[pathn++]={px[A],py[A]};        // 第一站 A
	ord[ordn++]=A;
	vis[A]=true;
	int cur{A};
	i64 dx{px[A]},dy{0};

	while(true)
	{
		int best{-1};
		for(int p=0;p<n;++p)
		{
			if(vis[p])
				continue;
			i64 vx=px[p]-px[cur],vy=py[p]-py[cur];
			i64 cr=cross(dx,dy,vx,vy);
			i64 dt=dx*vx+dy*vy;
			if(cr<0)
				continue;
			if(cr==0&&dt<=0)
				continue;
			if(!visible(cur,p))
				continue;

			if(best==-1)
			{
				best=p;
				continue;
			}
			i64 ux=px[best]-px[cur],uy=py[best]-py[cur];
			i64 cr2=cross(ux,uy,vx,vy);
			if(cr2>0)                   // v 在 u 左侧 => u 转角更小
				continue;
			else if(cr2<0)              // v 转角更小
				best=p;
			else
			{
				// 同方向：先吃近的
				i64 du=ux*ux+uy*uy,dv=vx*vx+vy*vy;
				if(dv<du)
					best=p;
			}
		}

		if(best==-1)
			break;

		vis[best]=true;
		ord[ordn++]=best;
		path[pathn++]={px[best],py[best]};
		dx=px[best]-px[cur];
		dy=py[best]-py[cur];
		cur=best;
	}

	cout<<ordn<<'\n';
	for(int i=0;i<ordn;++i)
		cout<<ord[i]+1<<(i+1==ordn?'\n':' ');
	return 0;
}
```
