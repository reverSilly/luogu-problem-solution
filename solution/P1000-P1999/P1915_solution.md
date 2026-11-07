# P1915 [NOI2010] 成长快乐

## 1. 重新读题

> **重要：这是一道提交答案（output-only）题。** 本题没有传统意义上的评测输入：官方给出 `nemo1.in ~ nemo10.in` 十份数据，选手需要分别提交 `nemo1.out ~ nemo10.out`，由 checker 检查方案合法性并按照吃到的重量总和给分。因此：无法用"读入 → 输出一个数"的常规方式在洛谷/本地普通评测；下面给出的程序是一个**构造器**，它读入**一份** `nemoK.in`，输出对应的合法成长计划（可直接重定向为 `nemoK.out`）；本仓库不包含、也不会伪造任何官方数据或评分表，文末的自测只用我自己按题意手造的小数据。

Nemo 初始体重 $w_0$，时刻 $0$ 在 $(x_0,y_0)$，最大速度 $V$。海里有 $n$ 只虾，第 $i$ 只体重 $w_i$，时刻 $0$ 在 $(x_i,y_i)$，速度向量 $(p_i,q_i)$，做匀速直线运动。Nemo 可在 $[0,T]$ 内任意移动（速度不超过 $V$）。当 Nemo 与虾**同时到达同一点**、且该虾体重 $w_i\le$ Nemo 当前体重时，可以吃掉它，体重增加 $w_i$。

要输出一串事件 $(t,x,y,s)$：时刻 $t$ 在 $(x,y)$ 吃掉编号 $s$ 的虾。下面讨论的都是"如何构造一个好方案"，而不是"如何求一个唯一答案"。

### 输出必须满足的合法性（checker 的判据）

1. $0\le t\le T$，且 $t$ 按输出顺序不减；
2. 从上一个事件位置（首个事件从 $(x_0,y_0)$）到 $(x,y)$ 的欧氏距离 $\le V\cdot\Delta t$；
3. $(x,y)$ 与编号 $s$ 的虾在时刻 $t$ 的位置一致（绝对/相对误差 $10^{-4}$）；
4. 吃之前 $w_s\le$ Nemo 当前体重；每只虾至多一次；
5. 报告的总重量 $w$ 等于实际吃掉的重量和。

### 规模暗示

这是启发式/构造题：$n$ 不会太大（原题数据为十组固定文件），但**没有"标准答案"**，只有分数档位。所以我们的目标是"稳定生成合法且尽量重的方案"，而不是证明最优。

## 2. 思维链（Chain-of-thought）

### 2.1 先把"吃"翻译成几何条件

固定当前状态：时刻 $t_0$、位置 $P_0$、体重 $W$。对虾 $i$，令

$$r=P_i(t_0)-P_0,\qquad u=(p_i,q_i).$$

若在 $t_0+s$ 时刻追上它（$s\ge0$），需要

$$|r+su|\le Vs.$$

这是"以 $P_0$ 为起点、速度上限 $V$ 的圆盘"与"虾的匀速直线轨迹"的相遇条件。两边平方（都非负）：

$$f(s)=\big(|u|^2-V^2\big)s^2+2(r\cdot u)s+|r|^2\le0.$$

记 $A=|u|^2-V^2,\ B=2(r\cdot u),\ C=|r|^2$。

**引理 1（最早拦截时刻）**：$f(s)\le0$ 在 $s\ge0$ 上的最小解为：

- 若 $C=0$：$s=0$；
- 若 $A>0$：判别式 $\Delta=B^2-4AC\ge0$ 且大根 $s_2\ge0$ 时有解，最小解为小根 $s_1$；否则无解；
- 若 $A=0$：$B<0$ 时有解，最小解为 $-C/B$；否则无解；
- 若 $A<0$：必有解，最小解为大根 $s_2$。

**证明**：

- $C=0$ 表示此刻已在同一位置，$s=0$ 可行。
- $A>0$：抛物线开口向上，$f\le0$ 的解集是两根之间的闭区间 $[s_1,s_2]$。因 $f(0)=C>0$，$0$ 不在区间内，故要么 $0<s_1$，要么 $0>s_2$。若 $s_2<0$，则所有 $s\ge0>s_2$ 都落在区间外，$f(s)>0$ 无解；若 $s_2\ge0$，最小可行点就是 $s_1$（且此时必有 $s_1>0$）。
- $A=0$：$f(s)=Bs+C$ 为一次函数，$C>0$。$B\ge0$ 时对 $s\ge0$ 恒正，无解；$B<0$ 时解集为 $s\ge-C/B$，最小解即 $-C/B$。
- $A<0$：抛物线开口向下，$f\le0$ 在两根之外。又 $\Delta=B^2-4AC>0$（因 $-4AC>0$），且 $f(0)=C>0$ 说明 $0$ 在两根之间，即 $s_1<0<s_2$。于是 $s\ge0$ 中最小可行点就是 $s_2$。$\blacksquare$

实现上不必背四种情况：把 $0$、$s_1$、$s_2$ 都作为候选，代入 $f$（或直接算距离与 $Vs$）检验，取最小可行者即可。

### 2.2 体重约束带来的单调性

**引理 2（可吃集合单调扩张）**：若某时刻体重为 $W$，则所有 $w_i\le W$ 的虾可吃；吃掉任意一只后体重只增不减，因此原本可吃的虾仍然可吃，且可能有新的虾变为可吃。

**证明**：体重更新为 $W+w_i\ge W$（$w_i>0$），条件 $w_j\le W$ 蕴含 $w_j\le W+w_i$。$\blacksquare$

所以"先吃轻的、小体重先攒起来"在可行性意义上是正确的方向；但**吃哪只、什么顺序**会影响能跑多远，不能仅凭体重排序。

### 2.3 失败尝试：只按"最早能吃到"贪心

最自然的贪心是每一步选"最早能拦截"的虾。反例（手造，$V=1,T=10,w_0=5$）：

- 虾 A：$w=4$，静止在 $(5,0)$，最早 $t=5$ 吃到；
- 虾 B：$w=5$，静止在 $(-9,0)$，最早 $t=9$ 吃到。

先吃 A：$t=5$ 在 $(5,0)$，到 B 距离 $14$，剩余 $5$ 个单位时间只能走 $5$，吃不到 B，总计 $4$。
直接吃 B：$t=9$ 到达，总计 $5$。最优是 $5>4$。

可见"最早"会把时间花在低价值目标上，之后来不及去高价值目标。更一般地，这是一个带时间窗、带体重前置约束的**不完全信息路径规划**，贪心没有最优性保证。

### 2.4 改进：带权评分 + 多次随机重启

在每一步，对所有"当前可吃（$w_i\le W$）且能在 $T$ 前拦截"的虾计算最早拦截时刻 $t_i$，用评分

$$\text{score}_i=t_i-\lambda\,w_i+\varepsilon_{\text{rand}}$$

选最小者。$\lambda$ 越大越偏向"重虾"，$\lambda=0$ 退化为"最早"。我们取若干档 $\lambda\in\{0,\ 0.5/V,\ 1/V,\ 2/V,\ \text{随机}\}$，每档独立跑一遍完整的贪心（一次"重启"），最后保留总重量最大的方案。随机项只用于打破并列、产生多样性。

**说明（诚实声明）**：这是启发式，**不保证最优**。它保证的是：每一步选中的虾都经过引理 1 检验可达、且满足体重条件，因此输出一定合法（在浮点误差内）。得分取决于数据，由官方 checker 判定。

### 2.5 数值与合法性

- 坐标/速度均为实数，求解二次方程用 `double`，并用"代回距离 $\le V s$"复核根的可行性，避免增根/舍入误差。
- 输出至少 6 位小数（程序输出 10 位）。
- 事件之间时间不减；吃虾后体重即时增加。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 把"追上虾"化成关于时间差的二次不等式（引理 1）。
- **explore its property** — 体重只增不减，可吃集合单调扩张（引理 2）。
- **counterexample** — 纯"最早到达"贪心会被低价值目标拖死。
- **randomized heuristic** — 评分 $t_i-\lambda w_i$ + 多档 $\lambda$ 重启取最优。
- **constructive output** — 直接输出事件序列，交给 checker 验证。

## 4. 解法说明（The Real Solution）

> 再次强调：本题为提交答案题，本程序是**构造器**，一次处理一份输入文件，不能按常规 OJ 方式评测。

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 虾的只数 | 读入 | — |
| `W0,V,T,px0,py0` | `double` | 初始体重、速度上限、时限、初始位置 | 读入 | 开头五个实数 |
| `a[i]` | `shrimp[200005]` | 第 $i$ 只虾的 $w,x,y,p,q$ | 读入 | 五元组 |
| `ate[i]` | `bool[]` | 是否已吃 | `false` | eaten |
| `t,cx,cy,w` | `double` | 当前时刻、位置、体重 | $0,px0,py0,W0$ | 状态 |
| `earliest(i,t,cx,cy)` | 函数 | 从当前状态最早吃到 $i$ 的时刻，无解返回 $-1$ | — | 引理 1 |
| `lam` | `double` | 评分中重量的权重 $\lambda$ | 每轮设定 | lambda |
| `plan` | 结构体 | 一条完整方案：$\sum w$ 与事件序列 | 空 | 存档 |

### 4.2 算法步骤

1. 读入 $w_0,V,T,x_0,y_0$、$n$ 和所有虾。
2. 对每个试次 `tr = 0..7`：
   1. 清空 `ate`，按试次选 $\lambda$；
   2. 初始化 $t=0$、位置 $(x_0,y_0)$、体重 $w_0$；
   3. 循环：
      - 对每只未吃且 $w_i\le w$ 的虾，用 `earliest` 求 $t_i$；
      - 丢弃无解或 $t_i>T$ 的；
      - 取 $\text{score}=t_i-\lambda w_i$（加微小随机扰动）最小者；
      - 若没有候选则退出；否则在 $t_i$ 时刻的位置吃掉它，更新 $t$、位置、$w$，记录事件；
   4. 若本轮总重更大，替换最优方案。
3. 输出：事件数 $k$、总重 $w$、$k$ 行 $(t,x,y,s)$，$t,x,y$ 保留 10 位小数。

### 4.3 复杂度

- 单轮：最多吃 $n$ 只，每步扫描 $O(n)$ 只虾并 $O(1)$ 求拦截时间，总计 $O(n^2)$。
- 共 8 轮：$O(8n^2)$；空间 $O(n)$。
- 输出 $O(n)$ 行。

## 5. 示例代码

```cpp
//
// 题目：P1915 [NOI2010] 成长快乐
// https://www.luogu.com.cn/problem/P1915
// 提交答案题（output-only）：本程序为构造器。
// 用法：./P1915 < nemoK.in > nemoK.out
//
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};
constexpr double eps{1e-9};
constexpr double big{1e100};

int n;
double W0,V,T,px0,py0;

struct shrimp
{
	double w,x,y,p,q;
};
shrimp a[mxn];
bool ate[mxn];

// 从时刻 t、位置 (cx,cy) 出发最早追上第 i 只虾的时刻；追不到返回 -1
double earliest(int i,double t,double cx,double cy)
{
	double rx=a[i].x+a[i].p*t-cx;
	double ry=a[i].y+a[i].q*t-cy;
	double ux=a[i].p,uy=a[i].q;
	double A=ux*ux+uy*uy-V*V;
	double B=2*(rx*ux+ry*uy);
	double C=rx*rx+ry*ry;
	auto ok=[&](double s) // 代回距离复核，防止浮点增根
	{
		double dx=rx+ux*s,dy=ry+uy*s;
		double d=sqrt(dx*dx+dy*dy);
		return d-V*s<=1e-7*(1.0+d+V*s);
	};
	if(C<=eps)
		return t;
	double cand[4];
	int cc=0;
	if(fabs(A)<1e-12)
	{
		if(B<0)
			cand[cc++]=-C/B;
	}
	else
	{
		double disc=B*B-4*A*C;
		if(disc>=0)
		{
			double sd=sqrt(disc);
			cand[cc++]=(-B-sd)/(2*A);
			cand[cc++]=(-B+sd)/(2*A);
		}
	}
	double best=big;
	for(int k=0;k<cc;++k)
		if(cand[k]>=-eps&&ok(max(0.0,cand[k])))
			best=min(best,max(0.0,cand[k]));
	if(best==big)
		return -1;
	return t+best;
}

struct plan
{
	int k=0;
	double w=0;
	vector<int> id;
	vector<double> t,x,y;
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>W0>>V>>T>>px0>>py0;
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].w>>a[i].x>>a[i].y>>a[i].p>>a[i].q;

	mt19937 rn(20090507);
	int trials=8;
	plan bestp;
	for(int tr=0;tr<trials;++tr)
	{
		fill_n(ate,n,false);
		double lam;
		switch(tr)
		{
			case 0:lam=0;break;
			case 1:lam=0.5/V;break;
			case 2:lam=1.0/V;break;
			case 3:lam=2.0/V;break;
			default:lam=(rn()%1000)/1000.0*3.0/V;break;
		}
		plan cur;
		double t=0,cx=px0,cy=py0,w=W0;
		while(true)
		{
			int bi=-1;
			double bt=0,bs=big;
			for(int i=0;i<n;++i)
			{
				if(ate[i]||a[i].w>w+1e-9)
					continue;
				double ti=earliest(i,t,cx,cy);
				if(ti<0||ti>T+1e-9)
					continue;
				double sc=ti-lam*a[i].w;
				sc+=(rn()%1000)*1e-9*tr; // 轻微扰动，让不同重启产生差异
				if(sc<bs)
				{
					bs=sc;
					bt=ti;
					bi=i;
				}
			}
			if(bi<0)
				break;
			ate[bi]=true;
			t=bt;
			cx=a[bi].x+a[bi].p*t;
			cy=a[bi].y+a[bi].q*t;
			w+=a[bi].w;
			++cur.k;
			cur.w+=a[bi].w;
			cur.id.push_back(bi);
			cur.t.push_back(t);
			cur.x.push_back(cx);
			cur.y.push_back(cy);
		}
		if(cur.w>bestp.w)
			bestp=cur;
	}

	cout<<bestp.k<<'\n';
	cout<<fixed<<setprecision(10)<<bestp.w<<'\n';
	for(int i=0;i<bestp.k;++i)
		cout<<bestp.t[i]<<' '<<bestp.x[i]<<' '<<bestp.y[i]<<' '<<bestp.id[i]+1<<'\n';
	return 0;
}
```

### 本地自测（手造数据，非官方数据）

用两组自己按题意写的小输入测试（`/tmp`，未提交进仓库）：

1. 初始 $(0,0)$、$w_0=1,V=1,T=10$；一只静止的 $1$ 重量虾在 $(3,0)$，一只静止的 $2$ 重量虾在 $(0,4)$。程序先 $t=3$ 吃第一只，再 $t=8$ 吃第二只，总重 $3$，事件位置与距离约束复核通过。
2. 更复杂的一组包含相向/背向运动的虾，程序输出 3 个事件、总重 $5$；逐条复核"距离 $\le V\Delta t$、位置等于虾的实时位置、吃前体重足够、$t\le T$"均成立。

（由于是提交答案题，这里只验证**合法性**，不声称最优性。）
