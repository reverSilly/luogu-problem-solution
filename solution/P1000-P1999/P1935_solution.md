# P1935 [国家集训队] 圈地计划

## 1. 重新读题

### 1.1 问题重述（人话版）

有一个 $N\times M$ 的网格（$N,M\le10$）。每个格子 $(i,j)$ 有两种开发方式：

- 建商业区，收益 $A_{i,j}$；
- 建工业区，收益 $B_{i,j}$。

此外，若格子 $(i,j)$ 有 $k$ 个**有公共边**的邻居与它类型不同，则它额外获得 $k\cdot C_{i,j}$ 的收益。求最大总收益。

### 1.2 关键约束与规模暗示

- $N,M\le10$，格子数 $V=NM\le100$，非常小。
- $A,B,C\le10^3$，总收益上界约 $100\times2000+180\times2000<6\times10^5$，`int` 足够。
- 每个格子只有两种选择，且收益只涉及**相邻格子的类型是否相同**——这是典型的二元标号（binary labeling）问题，规模又允许建网络流。
- 题目标签含“最小割”，提示把“最大化收益”转成“最小化损失”。

## 2. 思维链（Chain-of-thought）

### 2.1 先把收益写成统一形式

设 $x_u\in\{\text{商},\text{工}\}$ 表示格子 $u$ 的选择。先处理“相邻不同”的额外收益。

格子 $u$ 的额外收益是 $C_u\times(\text{与 }u\text{ 相邻且类型不同的格子数})$。把所有格子加起来，同一条无向边 $uv$ 会被 $u$ 和 $v$ 各算一次：

$$
\sum_u C_u\cdot\#\{v\sim u:x_v\ne x_u\}
=\sum_{uv\in E}(C_u+C_v)\cdot[x_u\ne x_v].
$$

**引理 1（边权分解）**

上式成立。

**证明**：左边按“格子—邻居”的有序对计数；每个有序对 $(u,v)$（$u\sim v$）恰好贡献 $C_u[x_u\ne x_v]$。把它们按无向边 $\{u,v\}$ 配对相加，得到 $(C_u+C_v)[x_u\ne x_v]$。$\blacksquare$

于是目标函数为
$$
\text{profit}(x)=\sum_u\bigl([x_u=\text{商}]A_u+[x_u=\text{工}]B_u\bigr)
+\sum_{uv\in E}(C_u+C_v)[x_u\ne x_v].
$$

### 2.2 为什么直接最小割不好做

最小割擅长表达“**不同**要付代价”（割边容量）或“**相同**要付代价”，而不擅长直接表达“不同给奖励”。常见技巧是把奖励改写成“常数减去惩罚”：

若想要“不同就奖励 $W$”，等价于
$$
W[x_u\ne x_v]=W-W[x_u=x_v],
$$
也就是“**相同就扣 $W$**”。

而在最小割里，一条连接 $u,v$ 的无向边恰好是“**异侧（两个变量取值不同）时付 $W$**”：若直接把它接在 $x_u,x_v$ 上，被割对应的是 $x_u\ne x_v$，会变成“不同扣钱”，方向正好反了。

### 2.3 棋盘染色：把“不同”翻成“相同”

网格是二分图。按 $(i+j)$ 的奇偶把格子染成黑白，则每条边都连接一个白格和一个黑格。

定义新变量 $y_u$：

- 白格：$y_u=x_u$；
- 黑格：$y_u=1-x_u$（把黑格的选择含义翻转）。

**引理 2（翻转后“不同”变“相同”）**

对任意相邻的白格 $u$ 与黑格 $v$，
$$
x_u\ne x_v\iff y_u=y_v.
$$

**证明**：设 $x=1$ 表示商业、$0$ 表示工业。白格 $y_u=x_u$，黑格 $y_v=1-x_v$。于是
$$
x_u\ne x_v\iff x_u=1-x_v\iff y_u=y_v.
$$
$\blacksquare$

这就把问题变成了：选 $y_u\in\{0,1\}$，最大化单点收益（黑白格对 $y$ 的含义不同）+ 相邻 $y$ **相同**时的奖励 $C_u+C_v$。

### 2.4 建最小割网络

设源点 $s$，汇点 $t$。一个割把点集分成 $S$（含 $s$）与 $T$（含 $t$）。约定 $u\in S\iff y_u=1$。

**单点边**：希望割的代价恰好等于“没拿到的单点收益”。

| 格子颜色 | 边 $s\to u$ 容量 | 边 $u\to t$ 容量 | 验证 |
| :--- | :--- | :--- | :--- |
| 白（$y=x$） | $A_u$ | $B_u$ | $u\in S\Rightarrow$ 商，割 $u\to t$ 付 $B_u$；$u\in T\Rightarrow$ 工，割 $s\to u$ 付 $A_u$ |
| 黑（$y=1-x$） | $B_u$ | $A_u$ | $u\in S\Rightarrow y=1\Rightarrow$ 工，割 $u\to t$ 付 $A_u$；$u\in T\Rightarrow$ 商，割 $s\to u$ 付 $B_u$ |

两种情况下都有
$$
\text{单点割代价}=(A_u+B_u)-\text{该格实际获得的单点收益}.
$$

**相邻边**：对每条网格边 $uv$，加一条容量 $W_{uv}=C_u+C_v$ 的**无向边**（实现时加 $u\to v$ 与 $v\to u$ 各一条容量 $W$ 的有向边）。它被割当且仅当 $u,v$ 分居两侧，即 $y_u\ne y_v$；由引理 2，这等价于原选择 $x_u=x_v$。因此
$$
\text{该边割代价}=W_{uv}\cdot[x_u=x_v]
=W_{uv}-W_{uv}\cdot[x_u\ne x_v].
$$

**引理 3（割容量与收益的恒等关系）**

对任意标号 $x$，令 $S=\{s\}\cup\{u:y_u=1\}$，则该割的容量为
$$
\text{cap}(S)=\underbrace{\sum_u(A_u+B_u)+\sum_{uv\in E}(C_u+C_v)}_{\text{常数 }K}-\text{profit}(x).
$$

**证明**：由上面的单点表，
$$
\sum_{u}\text{单点割代价}
=\sum_u(A_u+B_u)-\sum_u(\text{单点收益}).
$$
由相邻边分析，
$$
\sum_{uv}\text{相邻边割代价}
=\sum_{uv}W_{uv}-\sum_{uv}W_{uv}[x_u\ne x_v].
$$
两式相加，右边正好是 $K$ 减去引理 1 的目标函数。$\blacksquare$

**引理 4（割与标号一一对应）**

任意一个 $s$-$t$ 割都唯一确定一个标号（$u$ 在 $S$ 侧则 $y_u=1$，否则 $y_u=0$），且引理 3 的恒等式成立。

**证明**：割的定义就是把顶点划分成含 $s$ 的 $S$ 与含 $t$ 的 $T$；按约定读出 $y$ 即可。反向由任意 $y$ 构造 $S$ 也唯一。恒等式只依赖每条边两端是否同侧，故成立。$\blacksquare$

因此
$$
\max_x\text{profit}(x)=K-\min_{S}\text{cap}(S)=K-\text{maxflow}(s,t).
$$

由最大流最小割定理，Dinic 求出的最大流就是最小割容量。

### 2.5 正确性小结与边界

- 所有容量 $A_u,B_u,C_u+C_v\ge0$，网络合法。
- 单个格子、没有相邻边时，最小割就是 $\min(A,B)$，答案 $A+B-\min(A,B)=\max(A,B)$，符合直觉。
- 常数 $K$ 与最大流都用 `int` 即可（上界远小于 $2^{31}$）。
- 网格染色只用于建模；代码里按 $(i+j)\&1$ 分支加边。

### 2.6 复杂度

- 点数 $V=NM+2\le102$。
- 边数：单点边至多 $2NM$ 条有向弧，相邻边至多 $2\times2\times(\text{邻接数})$ 条弧，总数约 $10^3$ 量级。
- Dinic 的理论上界 $O(V^2E)$，在这个规模下瞬间完成。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 相邻额外收益可按无向边拆成 $C_u+C_v$，条件是两端类型不同。
- **consider easier version** — “不同就奖励”改写成“常数 − 相同就扣钱”，才适合割模型。
- **reduce to easier problem** — 棋盘黑白染色并翻转黑格含义，把“不同”变成“相同”。
- **classic problem solution** — 二元标号的最小割建模：单点边付损失，相邻无向边付 $C_u+C_v$。
- **prove by identity** — 证明割容量恒等于常数减收益，故答案 $=K-\text{maxflow}$。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `N,M` | `int` | 网格行数、列数 | 读入 | — |
| `A[i][j]` | `int[15][15]` | 商业收益 | 读入 | — |
| `B[i][j]` | `int[15][15]` | 工业收益 | 读入 | — |
| `C[i][j]` | `int[15][15]` | 每有一个异类邻居的额外收益 | 读入 | — |
| `cells` | `int` | 格子数 $NM$ | 计算 | — |
| `s,t` | `int` | 源点、汇点 | `s=cells,t=cells+1` | — |
| `H[],E[],cnt` | 链式前向星 | 网络流图 | `H` 置 $-1$ | head/edge |
| `total` | `int` | 常数 $K$ | 累加 | — |
| `dep[],cur[]` | `int` | Dinic 分层、当前弧 | 每次 BFS 重置 | — |

### 4.2 算法步骤

1. 读入 $N,M$ 及矩阵 $A,B,C$。
2. 令 `s=cells`，`t=cells+1`，初始化邻接表。
3. 对每个格子 $u=(i,j)$：
   - `total += A[i][j]+B[i][j]`；
   - 若 $(i+j)$ 为奇数（黑）：加边 $s\to u$ 容量 $B$，$u\to t$ 容量 $A$；
   - 否则（白）：加边 $s\to u$ 容量 $A$，$u\to t$ 容量 $B$。
4. 对每条向右、向下的相邻格子对 $(u,v)$：
   - `w=C[u]+C[v]`，`total += w`；
   - 加入无向边：$u\to v$ 与 $v\to u$ 各一条容量 $w$。
5. 用 Dinic 求 $s\to t$ 最大流 `flow`。
6. 输出 `total-flow`。

### 4.3 复杂度分析

- 时间：Dinic 在 $V\le102$、$E\approx10^3$ 的图上运行，远低于任何限制。
- 空间：$O(V+E)$。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int mxm{5005};
constexpr int inf{1145141919};

struct{int v,w,nx;}E[mxm];
int H[mxn],cnt;
int dep[mxn],cur[mxn];
int s,t;

void addedge(int u,int v,int w)
{
	E[cnt]={v,w,H[u]};
	H[u]=cnt++;
	E[cnt]={u,0,H[v]};
	H[v]=cnt++;
}

bool bfs()
{
	fill_n(dep,mxn,0);
	queue<int>q;
	dep[s]=1;
	q.push(s);
	while(!q.empty())
	{
		int u=q.front();
		q.pop();
		for(int i=H[u];~i;i=E[i].nx)
		{
			int v=E[i].v;
			if(E[i].w>0&&!dep[v])
			{
				dep[v]=dep[u]+1;
				q.push(v);
			}
		}
	}
	return dep[t]>0;
}

int dfs(int u,int f)
{
	if(u==t)
		return f;
	for(int&i=cur[u];~i;i=E[i].nx)
	{
		int v=E[i].v;
		if(E[i].w>0&&dep[v]==dep[u]+1)
		{
			int r=dfs(v,min(f,E[i].w));
			if(r)
			{
				E[i].w-=r;
				E[i^1].w+=r;
				return r;
			}
		}
	}
	return 0;
}

int dinic()
{
	int flow=0;
	while(bfs())
	{
		for(int i=0;i<=t;++i)
			cur[i]=H[i];
		while(int r=dfs(s,inf))
			flow+=r;
	}
	return flow;
}

int N,M;
int A[15][15],B[15][15],C[15][15];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>N>>M;
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
			cin>>A[i][j];
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
			cin>>B[i][j];
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
			cin>>C[i][j];

	int cells=N*M;
	s=cells;
	t=cells+1;
	fill_n(H,cells+2,-1);
	cnt=0;

	int total=0;
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
		{
			int u=i*M+j;
			total+=A[i][j]+B[i][j];
			if((i+j)&1)            // black: S=industrial, T=commercial
			{
				addedge(s,u,B[i][j]);
				addedge(u,t,A[i][j]);
			}
			else                   // white: S=commercial, T=industrial
			{
				addedge(s,u,A[i][j]);
				addedge(u,t,B[i][j]);
			}
		}

	const int dx[2]={0,1},dy[2]={1,0};
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
			for(int d=0;d<2;++d)
			{
				int ni=i+dx[d],nj=j+dy[d];
				if(ni>=N||nj>=M)
					continue;
				int u=i*M+j,v=ni*M+nj;
				int w=C[i][j]+C[ni][nj];
				total+=w;
				addedge(u,v,w);
				addedge(v,u,w);
			}

	cout<<total-dinic()<<'\n';
	return 0;
}
```
