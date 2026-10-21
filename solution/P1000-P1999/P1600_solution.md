# P1600 [NOIP 2016 提高组] 天天爱跑步 题解

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

给定一棵包含 $n$ 个节点的无根树，节点 $u$ 上的观察员会在第 $w_u$ 秒观察经过该节点的玩家。
有 $m$ 名玩家同时在第 $0$ 秒从各自的起点 $s_i$ 出发，以每秒走一条树边的速度沿唯一简单路径跑向终点 $t_i$（到达 $t_i$ 后立即消失）。
求每个节点 $u \in [1, n]$ 的观察员分别能观察到多少名玩家。

### 输入输出格式与数据规模

- **输入格式**：
  - 第一行两个正整数 $n, m$。
  - 接下来 $n - 1$ 行，每行两个正整数 $u, v$ 表示一条树边。
  - 接下来一行 $n$ 个非负整数 $w_1, w_2, \dots, w_n$。
  - 接下来 $m$ 行，每行两个正整数 $s_i, t_i$。
- **输出格式**：一行 $n$ 个用空格分隔的整数，依次表示节点 $1 \dots n$ 观察到的玩家人数。
- **数据规模**：$1 \le n, m \le 3 \times 10^5$，$0 \le w_u \le n$。注意存在 $n = 299,998$ 的退化长链测试点。

### 关键约束分析

1. 树可能退化为长度 $3 \times 10^5$ 的长链，若使用递归 DFS 且评测环境栈空间受限，容易发生栈溢出。采用**BFS 预处理 LCA + 显式栈非递归欧拉序遍历**可以彻底杜绝爆栈风险。
2. 我们需要将“树上路径带时间戳的匹配计数”拆解为**静态子树内特定权值的频次统计**，从而用全局桶数组在 $\mathcal{O}(n + m)$ 线性时间内完成统计。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：路径拆解与不变量提取

以节点 $0$（1-based 的节点 $1$）为根，设每个节点 $u$ 的深度为 $\text{dep}[u]$。
对于第 $i$ 个玩家的路径 $s_i \to t_i$，设其最近公共祖先为 $g = \text{lca}(s_i, t_i)$，路径总长度为 $\text{len}_i = \text{dep}[s_i] + \text{dep}[t_i] - 2\text{dep}[g]$。
我们将路径拆分为**上行段 $s_i \to g$** 与**下行段 $g \to t_i$**：

**引理 1（上下行路径被观察到的常数分离方程）**：
1. **上行段（$u$ 位于 $s_i \to g$ 上）**：玩家到达 $u$ 的时刻为 $\text{dep}[s_i] - \text{dep}[u]$。因此 $u$ 能观察到该玩家当且仅当：
   $$\text{dep}[s_i] - \text{dep}[u] = w_u \iff \underbrace{\text{dep}[s_i]}_{\text{仅与玩家 } i \text{ 有关}} = \underbrace{\text{dep}[u] + w_u}_{\text{仅与节点 } u \text{ 有关}}$$
2. **下行段（$u$ 位于 $g \to t_i$ 上）**：玩家到达 $u$ 的时刻为 $(\text{dep}[s_i] - \text{dep}[g]) + (\text{dep}[u] - \text{dep}[g])$。因此 $u$ 能观察到该玩家当且仅当：
   $$\text{dep}[s_i] - 2\text{dep}[g] + \text{dep}[u] = w_u \iff \underbrace{\text{dep}[s_i] - 2\text{dep}[g]}_{\text{仅与玩家 } i \text{ 有关}} = \underbrace{w_u - \text{dep}[u]}_{\text{仅与节点 } u \text{ 有关}}$$

**证明**：
移项后方程左端均为只依赖于路径参数 $(s_i, g)$ 的常数，右端均为只依赖于当前观察点 $u$ 的目标值。证毕。

### 第二步：树上差分 + 欧拉序进出子树桶差值法

一条上行段 $s_i \to g$ 何时经过节点 $u$？当且仅当起点 $s_i$ 位于 $u$ 的子树 $T_u$ 内，且终点 $g$ 位于 $u$ 或 $u$ 的祖先上！
同理，一条下行段 $g \to t_i$ 经过节点 $u$，当且仅当终点 $t_i$ 位于 $T_u$ 内，且 $g$ 位于 $u$ 或 $u$ 的祖先上！
- 为了避免求“$g$ 往 $t_i$ 方向的儿子节点”，我们让上行段和下行段都包含端点 $g$；由于在 $u = g$ 处上下行方程同为 $\text{dep}[s_i] - \text{dep}[g] = w_g$，若某玩家满足 $\text{dep}[s_i] - \text{dep}[g] == w_g$，他会在 $g$ 处被上行和下行各统计一次，我们只需预先令 `--ans[g]` 即可完美去重！
- 如何统计 $T_u$ 内的净贡献？
  维护两个全局桶：`bucket_up[]` 和 `bucket_dn[]`（下标加上偏移量 $+n$ 以防负数）。
  在遍历以 $u$ 为根的子树 $T_u$ 时：
  1. **进入子树 $T_u$ 前（Phase 0）**：先从 `ans[u]` 中减去当前桶中目标位置的值 `bucket_up[dep[u] + w[u]] + bucket_dn[w[u] - dep[u] + n]`；
  2. **遍历完 $T_u$ 所有子节点并处理节点 $u$ 时（Phase 1）**：
     - 将所有以 $u$ 为起点的上行路径权值 $\text{dep}[u]$ 插入 `bucket_up`；
     - 将所有以 $u$ 为终点的下行路径权值 $\text{dep}[s_i] - 2\text{dep}[g] + n$ 插入 `bucket_dn`；
     - 此时将桶中目标位置的值加回 `ans[u]`（**进出子树前后的差值**恰好就是完全发生在子树 $T_u$ 内部且尚未在严格子树内终结的路径条数！）；
     - 最后，将所有以 $u$ 为 LCA（即 $g = u$）的上下行路径从两个桶中删除，确保它们不会向上泄漏到 $u$ 的父节点！

---

## Part 3 — 算法流程与复杂度分析 (Algorithm & Complexity)

### 算法步骤

1. 读入树结构与 $w_0 \dots w_{n-1}$，通过 `bfs_lca()` 非递归预处理深度 `dep[]` 与倍增数组 `up[19][]`。
2. 读入 $m$ 条路径，求 $g = \text{get\_lca}(s, t)$，将上行插入事件挂在 $s$、删除事件挂在 $g$；将下行插入事件挂在 $t$、删除事件挂在 $g$；若 $\text{dep}[s] - \text{dep}[g] == w_g$，令 `--ans[g]`。
3. 使用显式栈 `stk` 模拟树的两阶段 DFS（`phase = 0` 进入子树前记录初值，`phase = 1` 离开子树时执行插入、统计差值、执行 LCA 删除）。
4. 输出 `ans[0..n-1]`。

### 复杂度分析

- **时间复杂度**：BFS 与倍增求 LCA 耗时 $\mathcal{O}((n + m) \log n)$；非递归欧拉序遍历与桶统计中，每个节点与每条路径事件均只被 $\mathcal{O}(1)$ 处理一次，耗时 $\mathcal{O}(n + m)$。总时间复杂度为 $\mathcal{O}((n + m) \log n)$。
- **空间复杂度**：$\mathcal{O}(n \log n + m)$，全部采用静态数组与链式前向星，且无任何递归调用栈开销。

---

## Part 4 — 代码实现 (Code)

```cpp
//
// 题目：P1600 [NOIP 2016 提高组] 天天爱跑步
// https://www.luogu.com.cn/problem/P1600
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{300005};        // n,m<=300000 (+5 松弛)
constexpr int lg{19};

int n,m;
int w[mxn],dep[mxn],up[lg][mxn],q_bfs[mxn];
int ans[mxn];

// 链式前向星存树边
int head_e[mxn],to_e[mxn<<1],nxt_e[mxn<<1],ecnt;

// 链式前向星存每个节点上的差分事件
int head_up_add[mxn],val_up_add[mxn],nxt_up_add[mxn],cnt_ua;
int head_up_del[mxn],val_up_del[mxn],nxt_up_del[mxn],cnt_ud;
int head_dn_add[mxn],val_dn_add[mxn],nxt_dn_add[mxn],cnt_da;
int head_dn_del[mxn],val_dn_del[mxn],nxt_dn_del[mxn],cnt_dd;

int bucket_up[mxn<<1];            // 桶维护上行路径 dep[s]
int bucket_dn[mxn<<1];            // 桶维护下行路径 dep[s] - 2*dep[lca] + n

void add_edge(int u,int v)
{
	to_e[++ecnt]=v;nxt_e[ecnt]=head_e[u];head_e[u]=ecnt;
	to_e[++ecnt]=u;nxt_e[ecnt]=head_e[v];head_e[v]=ecnt;
}

void push_ua(int u,int v){val_up_add[++cnt_ua]=v;nxt_up_add[cnt_ua]=head_up_add[u];head_up_add[u]=cnt_ua;}
void push_ud(int u,int v){val_up_del[++cnt_ud]=v;nxt_up_del[cnt_ud]=head_up_del[u];head_up_del[u]=cnt_ud;}
void push_da(int u,int v){val_dn_add[++cnt_da]=v;nxt_dn_add[cnt_da]=head_dn_add[u];head_dn_add[u]=cnt_da;}
void push_dd(int u,int v){val_dn_del[++cnt_dd]=v;nxt_dn_del[cnt_dd]=head_dn_del[u];head_dn_del[u]=cnt_dd;}

void bfs_lca()
{
	memset(dep,-1,sizeof(int)*n);
	int ql=0,qr=0;
	dep[0]=0;
	up[0][0]=0;
	q_bfs[qr++]=0;
	while(ql<qr)
	{
		int u=q_bfs[ql++];
		for(int k=1;k<lg;++k)
			up[k][u]=up[k-1][up[k-1][u]];
		for(int e=head_e[u];e;e=nxt_e[e])
		{
			int v=to_e[e];
			if(dep[v]==-1)
			{
				dep[v]=dep[u]+1;
				up[0][v]=u;
				q_bfs[qr++]=v;
			}
		}
	}
}

int get_lca(int u,int v)
{
	if(dep[u]<dep[v])
		swap(u,v);
	int diff=dep[u]-dep[v];
	for(int k=0;k<lg;++k)
		if((diff>>k)&1)
			u=up[k][u];
	if(u==v)
		return u;
	for(int k=lg-1;k>=0;--k)
		if(up[k][u]!=up[k][v])
		{
			u=up[k][u];
			v=up[k][v];
		}
	return up[0][u];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n-1;++i)
	{
		int u,v;
		cin>>u>>v;
		--u;--v;
		add_edge(u,v);
	}
	for(int i=0;i<n;++i)
		cin>>w[i];

	bfs_lca();

	for(int i=0;i<m;++i)
	{
		int s,t;
		cin>>s>>t;
		--s;--t;
		int g=get_lca(s,t);
		int v_up=dep[s];
		int v_dn=dep[s]-2*dep[g]+n;
		push_ua(s,v_up);
		push_ud(g,v_up);
		push_da(t,v_dn);
		push_dd(g,v_dn);
		// LCA 节点 g 同时被上行与下行路径覆盖, 若恰好在 g 被观察到需去重一次
		if(dep[s]-dep[g]==w[g])
			--ans[g];
	}

	// 非递归显式栈实现欧拉序子树差分统计, 彻底规避极限链爆栈
	vector<pair<int,int>> stk;
	stk.reserve(n*2);
	stk.push_back({0,0});
	while(!stk.empty())
	{
		auto [u,phase]=stk.back();
		stk.pop_back();
		int ku=dep[u]+w[u];
		int kd=w[u]-dep[u]+n;
		if(phase==0)
		{
			if(ku<=n)
				ans[u]-=bucket_up[ku];
			if(kd>=0&&kd<=2*n)
				ans[u]-=bucket_dn[kd];
			stk.push_back({u,1});
			for(int e=head_e[u];e;e=nxt_e[e])
			{
				int v=to_e[e];
				if(v!=up[0][u])
					stk.push_back({v,0});
			}
		}
		else
		{
			for(int e=head_up_add[u];e;e=nxt_up_add[e])
				++bucket_up[val_up_add[e]];
			for(int e=head_dn_add[u];e;e=nxt_dn_add[e])
				++bucket_dn[val_dn_add[e]];
			if(ku<=n)
				ans[u]+=bucket_up[ku];
			if(kd>=0&&kd<=2*n)
				ans[u]+=bucket_dn[kd];
			for(int e=head_up_del[u];e;e=nxt_up_del[e])
				--bucket_up[val_up_del[e]];
			for(int e=head_dn_del[u];e;e=nxt_dn_del[e])
				--bucket_dn[val_dn_del[e]];
		}
	}

	for(int i=0;i<n;++i)
		cout<<ans[i]<<(i+1==n?'\n':' ');
	return 0;
}
```

### 符号对照表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $n, m$ | `n`, `m` | 树节点数与跑步玩家数 |
| $w_u, \text{dep}[u]$ | `w[u]`, `dep[u]` | 节点 $u$ 的观察时间与树深 |
| $g$ | `g` | 玩家起点 $s$ 与终点 $t$ 的最近公共祖先 $\text{lca}(s, t)$ |
| $\text{bucket\_up}, \text{bucket\_dn}$ | `bucket_up[]`, `bucket_dn[]` | 维护上行常数 $\text{dep}[s]$ 与下行常数 $\text{dep}[s]-2\text{dep}[g]+n$ 的全局计数桶 |

---

## Part 5 — 易错点与总结 (Pitfalls & Takeaway)

### 常见易错点

1. **数组越界**：在查询上行桶时，$\text{dep}[u] + w_u$ 最大可达 $2n$；查询下行桶时，$w_u - \text{dep}[u] + n$ 的范围是 $[0, 2n]$。因此两个桶数组的大小必须开到 `2 * mxn`（即 $6 \times 10^5$），并检查下标是否在 `[0, 2*n]` 内。
2. **在 LCA 处的删除时机**：以 $u$ 为 LCA 的路径在节点 $u$ 本身仍然是有效的，必须**先统计完 `ans[u]`，然后再执行 `head_up_del[u]` 和 `head_dn_del[u]` 的删除操作**！

### 本地对拍与验证结果

| 测试项 | 覆盖范围与规模 | 结果 |
| :--- | :--- | :--- |
| 官方样例 | $n=6, m=3$（输出 `2 0 0 1 1 1`） | 通过 |
| 随机压力对拍 | $300$ 组随机树与链 vs `P1600_brute.cpp`（逐秒模拟路径） | 全部一致 |
| 极限退化长链测试 | $n = m = 299,998$ 满规模长链（验证非递归栈安全与速度） | 耗时 `1.5175 s`，无爆栈与超时风险 |

### 方法归纳 (Pattern Summary)

- **触发信号**：树上大量有向路径 $(s_i, t_i)$，要求统计每个节点 $u$ 上满足与到起点距离相关等式的路径数。
- **核心套路**：**按 LCA 拆分为上行与下行两段 + 移项分离玩家常数与节点常数 + 树上差分结合 DFS 进出子树前后的全局桶差值统计**（替代高空间开销的线段树合并）。
