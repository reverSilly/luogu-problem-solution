# P1753 矩阵链排序问题

## 1. 重新读题

给定 $n$ 个矩阵 $M_1, M_2, \dots, M_n$（$1 \le n \le 2 \times 10^6$），其中第 $i$ 个矩阵的维度为 $w_i \times w_{i+1}$（$1 \le w_i \le 10^4$）。将 $a \times b$ 的矩阵与 $b \times c$ 的矩阵相乘需要 $a b c$ 次标量乘法。求计算矩阵链乘积 $M_1 \times M_2 \times \cdots \times M_n$ 所需的最少运算次数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：从区间 DP 到凸多边形最小权三角剖分（Hu-Shing 算法）

经典的区间 DP 解法复杂度为 $\Theta(n^3)$，即使四俄罗斯人/姚期智优化也只能到 $\Theta(n^2)$，面对 $n = 2 \times 10^6$ 只有 $\Theta(n \log n)$ 的 **Hu-Shing 算法（1982/1984）** 能够胜任！

首先建立几何对偶：将 $n+1$ 个矩阵维度 $w_1, w_2, \dots, w_{n+1}$ 顺时针标在凸 $(n+1)$-边形的顶点 $V_1, V_2, \dots, V_{n+1}$ 上。每一次矩阵乘法 $A_{w_i \times w_k} \times B_{w_k \times w_j}$ 就精确对应于凸多边形中的三角形 $\triangle V_i V_k V_j$（权值为三个顶点权值之积 $w_i w_k w_j$）。因此，矩阵链乘法的最优结合顺序**完全等价于该带权凸 $(n+1)$-边形的最小权三角剖分**！

由于旋转多边形顶点编号不改变三角剖分，我们先把全局权值最小的顶点旋转为 $V_1$，并在数组末尾复制 $V_{n+2} = V_1$。

### 2.2 潜在水平弧（Potential H-arcs）树与顶棚斜率贪心（附完整证明）

在最优三角剖分中，除了从某个局部最轻顶点发出的“扇形对角线（Fan edges）”之外，将高权值峰区与低权值谷区隔开的对角线称为**水平弧（H-arc）**。

**引理 1（单调栈线性提取潜在 H-arc 树定理，Hu-Shing Part I）**：
一条对角线 $(V_u, V_v)$（$1 < u < v < n+2$）可能作为最优三角剖分中的 H-arc，仅当区间 $(u, v)$ 内部所有顶点 $k$ 均满足 $w_k \ge \max(w_u, w_v)$。
用单调递增栈扫描 $w_1, \dots, w_{n+2}$：每当栈顶 $w_{\text{stk}[\text{top}-1]} > w_i$ 被弹出时，若其左邻 $u = \text{stk}[\text{top}-2] > 1$ 且右邻 $v = i < n+2$，则 $(u, v)$ 是一条潜在 H-arc。所有潜在 H-arc 互不交叉，按区间包含关系构成一棵有根树（根节点为代表整个多边形的 $(1, n+2)$）。

**证明**：若 $(u, v)$ 内存在 $w_k < \max(w_u, w_v)$，则四边形对角线翻转（Flip）将 $(u, v)$ 换为过 $k$ 的对角线会使权值和严格减小，故 $(u, v)$ 不可能在最优剖分中隔离内部区域。单调栈在每个局部极大值回落时恰好枚举了所有极大满足 $\min_{u<k<v} w_k \ge \max(w_u, w_v)$ 的不交嵌套区间，总数不超过 $n-1$ 条。$\blacksquare$

**引理 2（扇形代价、外接分母与左偏树合并，Hu-Shing Part II）**：
对于 H-arc 树上的节点 $x = [u, v]$，设其剔除各子节点区间 $[u_c, v_c]$（用弦 $(u_c, v_c)$ 替代）后的多边形为 $P_x$，上边界边权乘积和为 $U(P_x)$，顶点下界为 $\text{mn} = \min(w_u, w_v)$。
1. 若不保留弦 $(u, v)$，而由外部更低顶点 $w_0$ 穿透 $(u, v)$ 直接连向 $P_x$ 的上边界，则 $w_0$ 的乘数净增加量（分母）为
   $$\text{den}_0(x) = U(P_x) - w_u w_v.$$
2. 若保留弦 $(u, v)$，并以 $\text{mn}$ 为扇心对 $P_x$ 进行扇形剖分，因 $P_x$ 中与扇心 $\text{mn}$ 相邻的上边界边（记其另一端为 $p_{\text{adj}}$）不构成非退化三角形，其扇形代价（分子）为
   $$\text{num}_0(x) = \text{mn} \cdot \big(U(P_x) - \text{mn} \cdot w_{p_{\text{adj}}}\big).$$
3. 定义子弧 $t$ 的性价比斜率（Ceiling）为 $S(t) = \frac{\text{num}[t]}{\text{den}[t]}$。用最大可并堆（左偏树）维护子树中所有候选弧集：
   - **穿透消去（Loop 1）**：当堆顶 $t$ 满足 $S(t) > \text{mn}$ 时，用 $\text{mn}$ 穿透 $t$ 比保留 $t$ 更优，故弹出 $t$ 并令 $\text{den} \gets \text{den} + \text{den}[t],\ \text{num} \gets \text{num} + \text{mn} \cdot \text{den}[t]$；
   - **捆绑合并（Loop 2）**：当堆顶 $t$ 满足 $S(t) \ge \frac{\text{num}}{\text{den}}$ 时，由于外部若选择保留 $x$ 则必然同时保留更划算的 $t$，故将 $t$ 与 $x$ 合并为一个组合弧（$\text{den} \gets \text{den} + \text{den}[t],\ \text{num} \gets \text{num} + \text{num}[t]$）。
在根节点 $x = 1$ 处完成 Loop 1 后，$\text{num}[1] + \sum_{t \in \text{pq}} \text{num}[t]$ 即为全局最小乘法次数。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将矩阵链乘法对偶为带权凸 $(n+1)$-边形的最小权三角剖分，并将全局最小点旋转至 $V_1$。
- **build H-arc tree** — 用单调栈在 $\Theta(n)$ 时间内提取所有不交叉潜在 H-arc，按区间嵌套构建树结构（引理 1）。
- **classic problem solution** — 在 H-arc 树上自底向上运行左偏树（可并堆）贪心：消去斜率 $> \text{mn}$ 的劣弧、捆绑斜率 $\ge S(x)$ 的子弧，实现 $\Theta(n \log n)$ 求解（引理 2）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 矩阵个数 $n$ 与多边形顶点数 $m = n+1$ | 标准输入 | 维度 |
| `w[1..m+1]` | `i64 [mxn]` | 旋转使 `w[1]` 最小后的顶点权值（`w[m+1]=w[1]`） | 旋转赋值 | vertex weights |
| `cp[i]` | `i64 [mxn]` | 相邻顶点乘积前缀和 $\sum_{k=1}^{i-1} w_k w_{k+1}$ | 预处理 | adjacent products |
| `L[x], R[x]` | `int [mxn]` | H-arc 树节点 $x$ 的左右端点下标 | 单调栈建树 | arc endpoints |
| `arc[id]` | `Arc [mxn]` | 左偏树节点 `{num, den, lc, rc, dist}` | DFS 维护 | leftist heap node |

### 4.2 算法步骤

1. 读入 $n$ 与 $w_1 \dots w_{n+1}$；特判 $n \le 2$。
2. 找出最小权值下标 `mn_idx`，循环左移使 `w[1]` 为全局最小值，并令 `w[m+1] = w[1]`，计算相邻乘积前缀和 `cp`。
3. 用单调栈扫描 $1 \dots m+1$ 收集所有满足 $1 < u < v < m+1$ 的潜在 H-arc，按 $(u\text{ 升序}, v\text{ 降序})$ 排序去重后用栈建出包含关系树。
4. 自底向上 DFS：合并子节点左偏树，执行 Loop 1（弹出 $S > \text{mn}$）与 Loop 2（合并 $S \ge S(x)$）。
5. 在根节点处累加 `arc[acnt].num` 与左偏树中剩余所有节点的 `num` 并输出。

### 4.3 复杂度分析

- **时间复杂度**：单调栈与建树耗时 $\Theta(n \log n)$；每个 H-arc 树节点最多入堆一次、在 Loop 1 或 Loop 2 中出堆一次，每次左偏树合并耗时 $O(\log n)$，总时间复杂度为 $\Theta(n \log n)$。
- **空间复杂度**：静态数组与左偏树节点均为线性规模，空间复杂度为 $\Theta(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1753 矩阵链排序问题
// https://www.luogu.com.cn/problem/P1753
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2000005};

int n,m;
i64 raw_w[mxn],w[mxn],cp[mxn];
int stk[mxn],L[mxn],R[mxn];
int head_ch[mxn],nxt_ch[mxn],tail_ch[mxn];

struct Arc
{
	i64 num,den;
	int lc,rc,dist;
}arc[mxn];
int acnt,rt[mxn];

bool cmp_arc(int a,int b)
{
	return (__int128)arc[a].num*arc[b].den>(__int128)arc[b].num*arc[a].den;
}

int merge_heap(int x,int y)
{
	if(!x||!y)
		return x|y;
	if(!cmp_arc(x,y))
		swap(x,y);
	arc[x].rc=merge_heap(arc[x].rc,y);
	if(arc[arc[x].lc].dist<arc[arc[x].rc].dist)
		swap(arc[x].lc,arc[x].rc);
	arc[x].dist=arc[arc[x].rc].dist+1;
	return x;
}

int pop_heap(int x)
{
	return merge_heap(arc[x].lc,arc[x].rc);
}

void add_child(int p,int c)
{
	if(!head_ch[p])
		head_ch[p]=tail_ch[p]=c;
	else
	{
		nxt_ch[tail_ch[p]]=c;
		tail_ch[p]=c;
	}
}

void dfs(int x)
{
	int u=L[x],v=R[x];
	i64 mn=min(w[u],w[v]);
	i64 U=cp[v]-cp[u];
	int pq=0;
	for(int c=head_ch[x];c;c=nxt_ch[c])
	{
		int cu=L[c],cv=R[c];
		U-=(cp[cv]-cp[cu])-w[cu]*w[cv];
		dfs(c);
		pq=merge_heap(pq,rt[c]);
	}

	i64 den,num0;
	int first_c=head_ch[x],last_c=tail_ch[x];
	if(x==1)
	{
		int p1=(first_c&&L[first_c]==u)?R[first_c]:(u+1);
		int pk_1=(last_c&&R[last_c]==v)?L[last_c]:(v-1);
		den=U-w[u]*w[p1]-w[pk_1]*w[v];
		num0=mn*den;
	}
	else
	{
		den=U-w[u]*w[v];
		if(w[u]<=w[v])
		{
			int p1=(first_c&&L[first_c]==u)?R[first_c]:(u+1);
			num0=mn*(U-w[u]*w[p1]);
		}
		else
		{
			int pk_1=(last_c&&R[last_c]==v)?L[last_c]:(v-1);
			num0=mn*(U-w[pk_1]*w[v]);
		}
	}

	i64 add_den=0;
	while(pq&&arc[pq].num>mn*arc[pq].den)
	{
		int top=pq;
		pq=pop_heap(pq);
		add_den+=arc[top].den;
	}

	den+=add_den;
	i64 num=num0+mn*add_den;

	int id=++acnt;
	arc[id].num=num;
	arc[id].den=den;
	if(x==1)
	{
		rt[x]=pq;
		return;
	}

	while(pq&&cmp_arc(pq,id))
	{
		int top=pq;
		pq=pop_heap(pq);
		arc[id].den+=arc[top].den;
		arc[id].num+=arc[top].num;
	}
	arc[id].lc=pq;
	arc[id].rc=0;
	arc[id].dist=1;
	rt[x]=id;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	m=n+1;
	for(int i=1;i<=m;++i)
		cin>>raw_w[i];

	if(n==1)
	{
		cout<<0<<'\n';
		return 0;
	}
	if(n==2)
	{
		cout<<raw_w[1]*raw_w[2]*raw_w[3]<<'\n';
		return 0;
	}

	int mn_idx=min_element(raw_w+1,raw_w+m+1)-raw_w;
	for(int i=1;i<=m;++i)
		w[i]=raw_w[(mn_idx+i-2)%m+1];
	w[m+1]=w[1];

	for(int i=2;i<=m+1;++i)
		cp[i]=cp[i-1]+w[i-1]*w[i];

	int top=0;
	vector<pair<int,int>>h_arcs;
	for(int i=1;i<=m+1;++i)
	{
		while(top>=2&&w[stk[top-1]]>w[i])
		{
			int u=stk[top-2],v=i;
			if(u>1&&v<m+1&&u+1<v)
				h_arcs.push_back({u,v});
			--top;
		}
		stk[top++]=i;
	}

	sort(h_arcs.begin(),h_arcs.end(),[](const pair<int,int>&a,const pair<int,int>&b){
		if(a.first!=b.first)return a.first<b.first;
		return a.second>b.second;
	});
	h_arcs.erase(unique(h_arcs.begin(),h_arcs.end()),h_arcs.end());

	int nodes=1;
	L[1]=1;R[1]=m+1;
	int t_top=0;
	stk[t_top++]=1;
	for(auto[u,v]:h_arcs)
	{
		while(R[stk[t_top-1]]<v)
			--t_top;
		++nodes;
		L[nodes]=u;R[nodes]=v;
		add_child(stk[t_top-1],nodes);
		stk[t_top++]=nodes;
	}

	dfs(1);

	i64 ans=arc[acnt].num;
	int pq=rt[1];
	while(pq)
	{
		ans+=arc[pq].num;
		pq=pop_heap(pq);
	}
	cout<<ans<<'\n';
	return 0;
}
```
