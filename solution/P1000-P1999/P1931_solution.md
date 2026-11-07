# P1931 套利

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $n$ 种货币和一张汇率表。一条记录 `c_i r c_j` 表示 $1$ 单位 $c_i$ 可以换成 $r$ 单位 $c_j$。问：是否存在一种换汇顺序，从 $1$ 单位某种货币出发，经过若干次兑换（可以回到原货币），最终得到严格多于 $1$ 单位的同种货币？也就是问汇率乘积是否存在大于 $1$ 的环。

输入多组数据，每组之间可能有空行，$n=0$ 结束；第 $i$ 组输出 `Case i: Yes` 或 `Case i: No`。

### 1.2 关键约束与规模暗示

- $n\le30$，非常小。
- $m$ 是汇率条数（题面未给上界，但规模与 $n$ 同阶，读入时按实际条数处理）。
- 货币名不含空格。多组数据 + 终止哨兵 $n=0$。
- $n\le30$ 意味着 $O(nm)$ 的 Bellman–Ford 完全够用，不需要更复杂的算法。

## 2. 思维链（Chain-of-thought）

### 2.1 把“套利”翻译成图论语言

把每种货币看成一个点，一条汇率 `c_i r c_j` 看成有向边 $c_i\to c_j$，边权为 $r$。沿着一个环走一圈，初始 $1$ 单位货币会变成
$$
\prod_{e\in\text{环}} r_e
$$
单位。套利存在，当且仅当图中存在一个有向环，其边权乘积 $>1$。

于是问题变成：**判断有向图是否存在乘积大于 $1$ 的环**。

### 2.2 为什么直接比较乘积不可行

环长最多 $n\le30$，看似可以直接枚举所有环，但环的数量是指数级的；而且乘积可能下溢成 $0$ 或上溢成 `inf`，浮点比较不可靠。需要一个把“乘法”变成“加法”的变换。

**引理 1（对数变换）**

设所有汇率 $r_e>0$。令 $w_e=\log r_e$。则对任意环 $C$，
$$
\prod_{e\in C}r_e>1\iff\sum_{e\in C}w_e>0\iff\sum_{e\in C}(-w_e)<0.
$$

**证明**：$\log$ 严格单调递增，且 $\log\prod r_e=\sum\log r_e$。于是
$$
\prod r_e>1\iff\log\prod r_e>\log1=0\iff\sum w_e>0.
$$
最后一步只是两边乘 $-1$。$\blacksquare$

所以只要检测图中是否存在**正权环**（或者把边权取相反数后检测负权环）。

### 2.3 只需检测简单环

**引理 2（有正环当且仅当有正简单环）**

若图中存在一个闭合 walk 的总权为正，则其中必有一个简单环总权为正。

**证明**：把闭合 walk 分解成若干个简单环的并（按经过的边拆分，允许重复分解；形式上不断取出 walk 中第一个重复顶点形成的简单环）。walk 的总权等于这些简单环权之和。若所有简单环的权都 $\le0$，总和也 $\le0$，与总权为正矛盾。$\blacksquare$

因此不需要关心绕圈次数，只要判断“是否存在一个正环”。

### 2.4 Bellman–Ford 判定正环

标准 Bellman–Ford 判定负环需要从某个源点出发，但套利的环可能出现在任意位置。处理方法是：**把所有点的初始距离都设成 $0$**，相当于加一个到所有点权为 $0$ 的超级源。

做 $n$ 轮松弛，每轮对所有边尝试
$$
d_v\leftarrow\max(d_v,\ d_u+w_{uv}).
$$

**引理 3（判定准则）**

若第 $n$ 轮仍能发生松弛，则存在正环；否则不存在。

**证明**：

- **无正环时**：任意最大权 walk 可以取成简单路径。理由：若 walk 中有重复顶点，则中间形成一个闭合 walk；其总权 $\le0$（否则由引理 2 有正简单环），删掉它不会让总权变小。于是存在最优 walk 长度 $\le n-1$。初始 $d\equiv0$ 对应“任意点出发的空 walk”，归纳可知第 $k$ 轮后 $d_v$ 已不小于任何以 $v$ 结尾、边数 $\le k$ 的 walk 的最大权；$k=n-1$ 时已覆盖所有简单路径，第 $n$ 轮不会更新。
- **有正环时**：设环 $C$ 总权 $\delta>0$。因为所有点初始距离都是 $0$，从环上任意点出发都可以先绕若干圈再走到环上各点；每多绕一圈权值增加 $\delta$，故松弛值无上界，不可能在 $n$ 轮内稳定，第 $n$ 轮必然还能更新。$\blacksquare$

### 2.5 浮点误差

$w=\log r$ 是浮点数。若真实乘积恰为 $1$（例如 $2\times0.5$），理论上 $\sum w=0$，但双精度下可能得到约 $-10^{-16}$ 的微小值。因此松弛条件写成
$$
d_v<d_u+w-\varepsilon,\qquad \varepsilon=10^{-12}.
$$
只有当“收益”超过 $\varepsilon$ 时才认为更优，避免把数值噪声误判成正环。$\varepsilon$ 远大于单次运算误差（约 $10^{-16}$），又远小于题目中有意义的套利幅度。

### 2.6 复杂度

每组数据：建图 $O(n+m)$，Bellman–Ford $O(nm)$。$n\le30$，非常快。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 套利存在 $\iff$ 有向图存在边权乘积 $>1$ 的环。
- **reduce to easier problem** — 取对数把“乘积 $>1$”变成“环权和 $>0$”。
- **classic problem solution** — 全源初始为 $0$ 的 Bellman–Ford，第 $n$ 轮仍松弛即有正环。
- **pitfall** — 多组数据与空行；浮点用 $\varepsilon$ 容差，不能直接和 $0$ 比较。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 货币种数 | 读入，`0` 结束 | — |
| `name[i]` | `string[35]` | 第 $i$ 种货币名 | 读入 | — |
| `id` | `unordered_map<string,int>` | 名称到编号 | 每组重建 | index |
| `m` | `int` | 汇率条数 | 读入 | — |
| `E[i]` | `edge[100005]` | `{u,v,w=log(r)}` | 读入 | edges |
| `d[i]` | `double[35]` | 当前最大对数收益 | 全 $0$ | distance |

### 4.2 算法步骤

1. 读入 $n$；若 $n=0$ 结束。
2. 读入 $n$ 个货币名并建立名称到编号的映射。
3. 读入 $m$，再读 $m$ 条形如 `a r b` 的记录，存边 $(id_a,id_b,\log r)$。
4. 令所有 `d[i]=0`。执行 $n$ 轮：
   - 遍历所有边，若 `d[v] < d[u]+w-1e-12`，更新 `d[v]` 并标记本轮有更新。
   - 若本轮无更新，提前结束（无正环）。
   - 若这是第 $n$ 轮且仍有更新，标记存在正环。
5. 输出 `Case 序号: Yes/No`。

### 4.3 复杂度分析

- 时间：每组 $O(nm)$。
- 空间：$O(n+m)$。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{35};
constexpr int mxm{100005};

struct edge
{
	int u,v;
	double w;
};

int n,m;
string name[mxn];
edge E[mxm];
double d[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int cs=0;
	while(cin>>n&&n)
	{
		++cs;
		unordered_map<string,int>id;
		for(int i=0;i<n;++i)
		{
			cin>>name[i];
			id[name[i]]=i;
		}
		cin>>m;
		for(int i=0;i<m;++i)
		{
			string a,b;
			double r;
			cin>>a>>r>>b;
			E[i]={id[a],id[b],log(r)};
		}

		fill_n(d,n,0.0);
		bool cyc=false;
		for(int it=0;it<n;++it)
		{
			bool upd=false;
			for(int i=0;i<m;++i)
				if(d[E[i].v]<d[E[i].u]+E[i].w-1e-12)
				{
					d[E[i].v]=d[E[i].u]+E[i].w;
					upd=true;
				}
			if(!upd)
				break;
			if(it==n-1)
				cyc=true;
		}
		cout<<"Case "<<cs<<": "<<(cyc?"Yes":"No")<<'\n';
	}
	return 0;
}
```
