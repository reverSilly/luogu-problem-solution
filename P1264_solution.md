# P1264 [ICPC 2002 Taejon R] K-联赛 题解

## 1. 重述问题

有 $n$ 支球队，每队已胜 $w_i$ 场、已负 $d_i$ 场（$d_i$ 在本题中不重要，因为无平局，胜负对称）。第 $i$ 队与第 $j$ 队之间还剩 $a_{ij}$ 场比赛未赛（$a_{ij}=a_{ji},\,a_{ii}=0$）。所有队参加的总场数相同。每场比赛只有胜负两种结果。

问：哪些球队**有可能**在所有未赛比赛结束后，取得**最高的胜场数**（允许并列第一）？

约束：$n \le 25$，$w_i, d_i \le 100$，$a_{ij} \le 10$。

---

## 2. 思维链（发现过程）

### 2.1 探索性质：冠军的定义与最优策略

"取得最高胜场数"意味着该队的最终胜场数 $\ge$ 所有其他队的最终胜场数。由于允许并列，团队 $x$ 能成为冠军当且仅当存在一种未赛比赛的结果安排，使得：

$$\forall j \ne x: \quad W_j^{\text{final}} \le W_x^{\text{final}}$$

其中 $W_j^{\text{final}} = w_j + (\text{team } j \text{ 赢的未赛场次数})$。

**关键洞察：** 为了让 $x$ 最容易成为冠军，应让 $x$ 赢得它所有的未赛比赛。

*证明（交换论证）：* 假设存在某安排使 $x$ 当冠军，但 $x$ 没有赢所有未赛比赛。则存在某场 $x$ 参与的比赛 $x$ 输了。将该场比赛改为 $x$ 赢（对手输），$x$ 的胜场 $+1$，对手的胜场 $-1$。原来 $W_x \ge W_j$ 对所有 $j$ 成立，修改后 $W_x$ 增加、$W_j$ 不变或减少，不等式仍成立。故 $x$ 赢所有未赛比赛也是合法安排。$\square$

因此：**$x$ 能当冠军 $\iff$ 存在未赛比赛结果，使得 $x$ 赢所有它的未赛比赛，且所有其他队 $j$ 的最终胜场 $\le k$**，其中 $k = w_x + R_x$，$R_x = \sum_{j \ne x} a_{xj}$ 是 $x$ 的未赛总场数。

### 2.2 考虑简化版本：固定 x 后，其余比赛如何分配

固定 $x$ 后，$x$ 赢所有它的比赛。其余比赛（不涉及 $x$ 的 $a_{ij}$ 场，$i,j \ne x$）需在 $i$ 和 $j$ 之间分配胜负。约束：每队 $j \ne x$ 从这些比赛中赢的场数 $\le k - w_j$。

这是一个**分配问题**：每场比赛的"胜场"分配给参赛两队之一，每队有容量上限。

### 2.3 经典问题识别：转化为最大流

用**最大流**建模（$x$ 固定后）：

- **源 $S$**：产生"胜场资源"。
- **比赛节点 $G_{ij}$**：对每对 $(i,j)$，$i<j$，$i,j \ne x$，一个节点，代表这 $a_{ij}$ 场比赛。
- **球队节点 $T_j$**：对每队 $j \ne x$，一个节点。
- **汇 $T$**：吸收"胜场"。

边：
1. $S \to G_{ij}$，容量 $= a_{ij}$（这场比赛产生 $a_{ij}$ 个胜场）。
2. $G_{ij} \to T_i$，容量 $= \infty$；$G_{ij} \to T_j$，容量 $= \infty$（胜场可分配给 $i$ 或 $j$）。
3. $T_j \to T$，容量 $= k - w_j$（队 $j$ 最多再赢 $k - w_j$ 场）。

**定理 1（充分性）：** 若最大流 $= \sum_{i<j,\, i,j \ne x} a_{ij}$（满流），则存在安排使 $x$ 当冠军。

*证明：* 满流意味着每个 $G_{ij}$ 的 $a_{ij}$ 单位流量全部流出（$G_{ij}$ 出边到 $T_i$ 和 $T_j$，容量 $\infty$，必然全部分配）。设 $G_{ij} \to T_i$ 流量 $f_{ij}^i$，$G_{ij} \to T_j$ 流量 $f_{ij}^j$，$f_{ij}^i + f_{ij}^j = a_{ij}$。由整数最大流定理（容量均整数），$f_{ij}^i$ 为整数。安排：队 $i$ 赢 $f_{ij}^i$ 场、队 $j$ 赢 $f_{ij}^j$ 场。队 $i$ 从所有非 $x$ 比赛获得的总胜场 $= \sum_{j \ne x, j \ne i} f_{ij}^{\text{team } i} \le k - w_i$（因 $T_i \to T$ 容量限制）。故队 $i$ 最终胜场 $= w_i + \text{wins from non-}x\text{ games} \le w_i + (k - w_i) = k$。$x$ 最终胜场 $= k$。所有队 $\le k$，$x$ 当冠军。$\square$

**定理 2（必要性）：** 若 $x$ 能当冠军，则最大流 $= \sum_{i<j,\, i,j \ne x} a_{ij}$。

*证明：* 设存在安排使 $x$ 当冠军（$x$ 赢所有未赛比赛）。对每对 $(i,j)$，$i<j$，$i,j \ne x$，设队 $i$ 赢 $w_{ij}$ 场（$0 \le w_{ij} \le a_{ij}$）。构造流：$S \to G_{ij}$ 流量 $a_{ij}$，$G_{ij} \to T_i$ 流量 $w_{ij}$，$G_{ij} \to T_j$ 流量 $a_{ij} - w_{ij}$，$T_i \to T$ 流量 $= $ 队 $i$ 从非 $x$ 比赛赢的总场数 $\le k - w_i$（因 $x$ 当冠军）。合法流，总流量 $= \sum_{i<j,\, i,j \ne x} a_{ij}$。最大流 $\ge$ 该值。又最大流 $\le$ 源出总容量 $= \sum_{i<j,\, i,j \ne x} a_{ij}$。故相等。$\square$

**整数性：** 所有容量均为整数，由整数最大流定理，存在整数最大流，对应合法的比赛结果安排。

### 2.4 特殊情况处理

- **$k - w_j < 0$**：队 $j$ 已胜 $w_j > k$ 场，无论后续如何，$j$ 胜场 $> k = W_x^{\text{final}}$，$x$ 不可能当冠军。早停。
- **$k - w_j = 0$**：$T_j \to T$ 容量 0，自然处理。
- **$n = 1$**：队 0 必为冠军，$R_0 = 0$，$k = w_0$，无比赛节点，$f_{\text{target}} = 0$，$f = 0$，判定通过。

### 2.5 复杂度

- 节点数：$2 + (n-1) + \binom{n-1}{2}$，$n=25$ 时 $\le 2 + 24 + 276 = 302$。
- 边数（含反向边）：$\approx 2 \times (1 \cdot m + 2 \cdot m + (n-1)) = 2 \times (3 \cdot 276 + 24) = 1680$。
- 每队跑一次 Dinic，$n = 25$ 次，每次毫秒级，总时间远小于 1 秒。

---

## 3. 模式总结

```
- explore its property — 冠军 = 胜场 ≥ 所有对手；exchange argument 证明"让 x 赢所有比赛"是最优策略
- consider easier version — 固定 x 后，问题简化为"将胜场分配给参赛队，每队有容量上限"
- classic problem solution — 最大流建模：S→game→team→T，满流 iff 可行安排存在（充分+必要双定理）
- reduce to easier problem — 每队独立判定（n 次最大流），V≈300, E≈840，规模极小
- classic problem solution — Dinic 最大流（BFS 分层 + DFS 增广 + 当前弧优化）
```

---

## 4. 正式解法

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 球队数（$1 \le n \le 25$） |
| $w[i]$ | 队 $i$ 已胜场数（0-based，$0 \le w[i] \le 100$） |
| $a[i][j]$ | 队 $i$ 与队 $j$ 未赛场次（$a[i][j]=a[j][i]$, $a[i][i]=0$） |
| $R_x$ | 队 $x$ 未赛总场数 $= \sum_{j \ne x} a[x][j]$ |
| $k$ | 队 $x$ 最终胜场目标 $= w[x] + R_x$ |
| $S$ | 源节点（下标 $0$） |
| $T$ | 汇节点（下标 $\text{tot}-1$） |
| $G_{ij}$ | 比赛节点，对应队 $i$ 与队 $j$（$i<j$，$i,j \ne x$） |
| $T_j$ | 球队节点，对应队 $j$（$j \ne x$） |
| $E[]$ | 边数组，`{v, w, nx}`：终点、剩余容量、下一条边下标 |
| $H[]$ | 链式前向星头数组，$H[u]$ = 节点 $u$ 第一条出边下标（$-1$ 表示无） |
| $d[]$ | BFS 分层距离（$-1$ 表示未访问） |
| $cur[]$ | DFS 当前弧指针 |
| $\text{inf}$ | 无穷容量哨兵 $= 1145141919$ |
| $m$ | 固定 $x$ 后的比赛节点数 $= \binom{n-1}{2}$（去掉 $x$ 后的对数） |
| $f_{\text{target}}$ | 目标流值 $= \sum_{i<j,\, i,j \ne x} a[i][j]$ |
| $f$ | 实际最大流值 |

### 节点编号方案（$x$ 固定后）

- $S = 0$
- 比赛节点：编号 $1$ 到 $m$，按 $(i,j)$ 字典序（$i < j$，$i,j \ne x$）依次编号
- 球队节点：编号 $m+1$ 到 $m+(n-1)$，按队号升序（跳过 $x$）
- $T = m + (n-1) + 1$
- 总节点数 $\text{tot} = m + (n-1) + 2$

### 算法步骤

1. **读入** $n$，$w[0 \ldots n-1]$（注意输入是 $w_1, d_1, w_2, d_2, \ldots$，$d_i$ 忽略），$a[0][0] \ldots a[n-1][n-1]$。

2. **对每队 $x$（$x = 0$ 到 $n-1$）：**
   1. 计算 $R_x = \sum_{j \ne x} a[x][j]$，$k = w[x] + R_x$。
   2. **早停**：若存在 $j \ne x$ 使 $w[j] > k$，$x$ 不能当冠军，跳到下一队。
   3. **建网络**：
      - 清空 $H[]$（全 $-1$），$cnt = 0$。
      - 枚举 $(i,j)$（$i < j$，$i,j \ne x$），为每对分配比赛节点编号 $g$（从 $1$ 起）。
      - 加边 $S \to g$，容量 $= a[i][j]$。
      - 加边 $g \to \text{teamNode}(i)$，容量 $= \text{inf}$；$g \to \text{teamNode}(j)$，容量 $= \text{inf}$。
      - 对每队 $j \ne x$，加边 $\text{teamNode}(j) \to T$，容量 $= k - w[j]$。
   4. **跑 Dinic**，得最大流 $f$。
   5. 计算 $f_{\text{target}} = \sum_{i<j,\, i,j \ne x} a[i][j]$。
   6. 若 $f = f_{\text{target}}$，$x$ 能当冠军，加入答案列表。

3. **输出**答案列表中所有队编号（$+1$ 转 1-based），升序，空格分隔，行末换行。

### 复杂度

- 每队：Dinic $O(V^2 E)$，$V \le 302$，$E \le 1680$。
- $n$ 队：$O(n V^2 E)$，实际远小于 1 秒。

### 验证状态

- 满流 $\iff$ 可行安排（定理 1 充分 + 定理 2 必要，均完整证明）。
- 整数性保证流对应合法比赛结果。
- 早停覆盖 $w_j > k$ 的非法情况。
- $n=1$ 边界：$f_{\text{target}} = 0$，$f = 0$，判定通过。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305};
constexpr int mxe{2005};
constexpr int inf{1145141919};

int n,w[mxn],a[25][25];
struct{int v,w,nx;}E[mxe];
int H[mxn],cnt,d[mxn],cur[mxn];

void addedge(int u,int v,int ww)
{
	E[cnt]={v,ww,H[u]};H[u]=cnt++;
	E[cnt]={u,0,H[v]};H[v]=cnt++;
}

int bfs(int s,int t)
{
	fill_n(d,mxn,-1);
	d[s]=0;
	queue<int>Q;
	Q.push(s);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		for(int i=H[u];i!=-1;i=E[i].nx)
			if(E[i].w>0&&d[E[i].v]==-1)
			{
				d[E[i].v]=d[u]+1;
				Q.push(E[i].v);
			}
	}
	return d[t]!=-1;
}

int dfs(int u,int t,int lim)
{
	if(u==t)
		return lim;
	int used=0;
	for(int&i=cur[u];i!=-1;i=E[i].nx)
	{
		int v=E[i].v;
		if(E[i].w>0&&d[v]==d[u]+1)
		{
			int flow=dfs(v,t,min(lim-used,E[i].w));
			if(flow)
			{
				E[i].w-=flow;
				E[i^1].w+=flow;
				used+=flow;
				if(used==lim)
					return used;
			}
		}
	}
	return used;
}

int dinic(int s,int t)
{
	int res=0;
	while(bfs(s,t))
	{
		memcpy(cur,H,sizeof(H));
		while(int f=dfs(s,t,inf))
			res+=f;
	}
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
	{
		int _d;
		cin>>w[i]>>_d;
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];
	vector<int>ans;
	for(int x=0;x<n;++x)
	{
		int R=0;
		for(int j=0;j<n;++j)
			if(j!=x)
				R+=a[x][j];
		int k=w[x]+R;
		bool ok=true;
		for(int j=0;j<n;++j)
			if(j!=x&&w[j]>k)
			{
				ok=false;
				break;
			}
		if(!ok)
			continue;
		fill_n(H,mxn,-1);
		cnt=0;
		// count game nodes (pairs i<j, i!=x, j!=x)
		int m=0;
		for(int i=0;i<n;++i)
			if(i!=x)
				for(int j=i+1;j<n;++j)
					if(j!=x)
						++m;
		// team node mapping: team j -> m+1+rank(j) where rank counts teams <j excluding x
		// S=0, game nodes 1..m, team nodes m+1..m+n-1, T=m+n
		int S=0,T=m+n;
		// build game node edges
		int gi=1;
		for(int i=0;i<n;++i)
		{
			if(i==x)
				continue;
			for(int j=i+1;j<n;++j)
			{
				if(j==x)
					continue;
				// team node index for team t
				auto tn=[&](int t)
				{
					int rank=0;
					for(int q=0;q<t;++q)
						if(q!=x)
							++rank;
					return m+1+rank;
				};
				addedge(S,gi,a[i][j]);
				addedge(gi,tn(i),inf);
				addedge(gi,tn(j),inf);
				++gi;
			}
		}
		// team -> sink edges
		for(int j=0;j<n;++j)
		{
			if(j==x)
				continue;
			int rank=0;
			for(int q=0;q<j;++q)
				if(q!=x)
					++rank;
			addedge(m+1+rank,T,k-w[j]);
		}
		int f=dinic(S,T);
		int ftarget=0;
		for(int i=0;i<n;++i)
			if(i!=x)
				for(int j=i+1;j<n;++j)
					if(j!=x)
						ftarget+=a[i][j];
		if(f==ftarget)
			ans.push_back(x);
	}
	for(int i=0;i<ans.size();++i)
	{
		if(i)
			cout<<' ';
		cout<<ans[i]+1;
	}
	cout<<'\n';
	return 0;
}
```
