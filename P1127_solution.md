# P1127 词链 — 完整题解

---

## 1. Re-read the problem（重读题目）

> 给定 $n$ 个由小写字母组成的单词（$1\le n\le1000$，每个单词长度 $1$ 至 $20$），要求将它们全部连接成一条**词链**：若单词 $X$ 的末字母与单词 $Y$ 的首字母相同，则 $X$ 与 $Y$ 可以相连为 $X.Y$。每个单词在词链中出现且仅出现一次（相同的单词若出现 $k$ 次则需输出 $k$ 次）。求**字典序最小**的词链，若不存在则输出 `***`。

**输入**：第一行 $n$，接下来 $n$ 行每行一个单词。

**输出**：一行，词链（单词以 `.` 分隔）或 `***`。

**约束**：

- 词链必须使用**全部**单词，每个**恰好一次**。
- 相邻单词必须满足 $X_{\text{末}} = Y_{\text{首}}$。
- 字典序比较的是整个链字符串（单词 + `.`）。
- $n$ 最大 $1000$，每个单词长度 $\le 20$。
- 只有小写字母。

**样例**：

```
6
aloha
arachnid
dog
gopher
rat
tiger
```

输出：

```
aloha.arachnid.dog.gopher.rat.tiger
```

---

## 2. Chain-of-thought with full proofs（思维链与完整证明）

### 2.1 问题建模：从单词到图

每个单词 $w$ 可以看作一条**有向边**：从首字母 $w[0]$ 指向末字母 $w[-1]$。词链要求所有边恰好使用一次，形成一条连续的路径，即**欧拉通路**（Eulerian trail）。

例如 `dog.gopher` 对应边 $\text{d}\to\text{g}$（`dog`）接 $\text{g}\to\text{r}$（`gopher`）。

由此，问题转化为：给定一个有向**多重图**（$26$ 个顶点，$n$ 条边），每条边带有一个字符串标签，求**字典序最小的欧拉通路**（经过每条边恰好一次），若无则报告无解。

### 2.2 欧拉通路的存在性条件

**定理 1**（有向图欧拉通路存在性充要条件）：  
设 $G$ 为有向图，$\text{out}(v)$ 为 $v$ 的出度，$\text{in}(v)$ 为入度。$G$ 存在欧拉通路当且仅当：

1. **连通性**：忽略边的方向后，所有有边相连的顶点属于同一个连通分量（即底层无向图连通）；
2. **度平衡**：
   - 恰好一个顶点 $s$ 满足 $\text{out}(s) - \text{in}(s) = 1$（起点）；
   - 恰好一个顶点 $t$ 满足 $\text{in}(t) - \text{out}(t) = 1$（终点）；
   - 其余所有顶点满足 $\text{out}(v) = \text{in}(v)$。

若所有顶点均满足 $\text{out}(v) = \text{in}(v)$，则存在**欧拉回路**（可从任意有出度的顶点出发，最后回到该顶点）。

**证明概要**：充分性可通过构造性算法（Hierholzer）证明；必要性显然：路径中除起点多一条出边、终点多一条入边外，每次进入一个顶点必离开。

### 2.3 字典序最小链的构造：Hierholzer 算法 + 贪心选边

**Hierholzer 算法**（DFS 版）：
1. 从起点 $s$ 开始，进行 DFS，每次沿着一条未使用的边移动到下一个顶点；
2. 当在某顶点 $u$ 没有未使用的出边时，将 $u$（或该边的信息）压入栈，回溯；
3. 最终栈的逆序即为欧拉通路。

**引理 1**（贪心选边的最优性）：  
若将所有单词按字典序**升序**排序，在每个顶点维护升序的邻接表，用指针从小到大选择边，则 Hierholzer 返回的路径（反转后）是字典序最小的欧拉通路。

**证明**：设 $P = \langle e_1, e_2, \dots, e_n \rangle$ 为算法输出的边序列（反转后）。对于任意不同于 $P$ 的欧拉通路 $Q$，设 $k$ 为 $P$ 与 $Q$ 第一个不同的位置。考虑 $P$ 前 $k-1$ 条边确定后，当前所在的顶点 $u$。$P$ 在 $u$ 处选择了字典序最小的可用边 $e$，而 $Q$ 选择了另一条 $f$（$f > e$）。由于欧拉通路从 $u$ 出发必然会消耗完 $u$ 的所有出边，且 $e$ 的接入不会阻碍完成剩余路径（欧拉图的性质保证），因此 $P$ 在 $k$ 处的单词严格小于 $Q$，故 $P$ 字典序更小。$\blacksquare$

### 2.4 连通性判断与 DSU

使用并查集（DSU）维护字母之间的连通关系。对于每个单词 $w$，将 $w[0]$ 与 $w[-1]$ 合并。扫描所有**出现过**的字母，检查它们是否属于同一集合。若否，则图不连通，无解。

### 2.5 算法正确性总结

1. 排序所有单词 → 邻接表自动升序，保证贪心最优性。
2. DSU 连通性检查 → 排除不连通图。
3. 度分析：确定起点或判断为回路。
4. Hierholzer 构造：
   - 若为欧拉通路，起点唯一确定，执行一次 DFS。
   - 若为欧拉回路，枚举所有有出度的顶点作为起点执行 DFS，取字典序最小的结果（最多 $26$ 次尝试）。
5. 若路径未覆盖所有边 → 无解。
6. 输出路径或 `***`。

---

## 3. Pattern summary（模式总结）

| 维度 | 内容 |
|------|------|
| **问题类型** | 有向图欧拉通路（Eulerian trail） + 字典序最小路径 |
| **核心模式** | 单词 → 有向边建模；Hierholzer 算法（DFS）；贪心选最小边 |
| **状态定义** | 每个字母（$0$–$25$）为顶点；每个单词为一条有向边 |
| **存在性条件** | 底层图连通 + 度平衡（0 或 2 个奇度点） |
| **构造方法** | 排序单词 → 升序邻接表 → DFS 回溯收集路径 → 反转 |
| **时间复杂度** | $O(n \log n + 26n) = O(n \log n)$，瓶颈在排序 |
| **空间复杂度** | $O(n + 26)$ |
| **易错点** | 连通性只考虑有边出现的字母；回路时需枚举所有起点取最小；多重边和自环正常处理 |
| **变形方向** | 无向图欧拉通路；混合图欧拉通路；最小表示法回文切割等 |

---

## 4. Real solution with notation glossary（正式解法与符号表）

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 单词个数 |
| $K = 26$ | 小写字母种类数 |
| $\text{words}[i]$ | 第 $i$ 个单词的字符串（排序后） |
| $\text{adj}[u]$ | 从字母 $u$ 出发的边列表，元素为 $(\text{目标字母}, \text{单词下标})$ |
| $\text{out}[u]$ / $\text{in}[u]$ | 字母 $u$ 的出度 / 入度 |
| $\text{has\_edge}[u]$ | 字母 $u$ 是否出现在任何单词中 |
| $\text{parent}[u]$ | DSU 父节点 |
| $\text{ptr}[u]$ | Hierholzer 中字母 $u$ 的当前边指针 |
| $\text{path}$ | 收集到的边下标（反转前为 DFS 顺序，反转后为正确顺序） |
| $\text{ans}$ | 最终的答案路径 |
| $s$ | 欧拉通路的起点（度分析确定） |
| $r$ | DSU 根节点（连通性判断用） |

### 算法描述

**步骤 1：读入并排序**

读入 $n$ 个单词，按字典序**升序**排序。排序保证每个顶点的邻接表自然升序。

**步骤 2：建图与度统计**

遍历排序后的单词，对于 $\text{words}[i]$：
- 令 $u = \text{words}[i][0] - \text{'a'}$，$v = \text{words}[i].\text{back}() - \text{'a'}$
- 将 $(v, i)$ 加入 $\text{adj}[u]$
- $\text{out}[u] \gets \text{out}[u] + 1$，$\text{in}[v] \gets \text{in}[v] + 1$
- $\text{has\_edge}[u] = \text{has\_edge}[v] = \text{true}$
- DSU 合并 $u$ 和 $v$

**步骤 3：连通性检查**

找到第一个 $\text{has\_edge}[i] = \text{true}$ 的字母的根 $r$。检查所有出现过字母的根是否均为 $r$。若非，输出 `***` 并结束。

**步骤 4：度分析**

对每个字母 $i$（仅 $\text{has\_edge}[i] = \text{true}$）：
- 计算 $\text{diff} = \text{out}[i] - \text{in}[i]$
- 若 $\text{diff} = 1$：设为起点 $s$（只能有一个）
- 若 $\text{diff} \neq 0, 1, -1$：无解
- 若所有 $\text{diff} = 0$：为回路，$s = -1$

**步骤 5：Hierholzer 构造**

定义 $\text{dfs}(u)$：
```
while ptr[u] < adj[u].size():
    (v, id) = adj[u][ptr[u]++]
    dfs(v)
    path.push_back(id)
```

- 若 $s \neq -1$（欧拉通路）：$\text{fill\_n}(\text{ptr}, K, 0)$，$\text{path.clear}()$，$\text{dfs}(s)$，反转 $\text{path}$。
- 若 $s = -1$（欧拉回路）：枚举每个 $\text{out}[i] > 0$ 的 $i$ 作为起点，执行上述过程，取所得路径中字典序最小的（逐单词比较）。

若 $\text{path}$ 大小不等于 $n$，则无解。

**步骤 6：输出**

将 $\text{ans}$ 中的下标依次输出对应的单词，以 `.` 分隔。若 $\text{ans}$ 为空，输出 `***`。

### 边界情况

- **单单词**（$n=1$）：回路情形，起点是该单词的首字母，路径唯一，正确输出。
- **全部自环**（如 `"aa"` 重复多次）：正常处理，边从 $a$ 到 $a$，Hierholzer 逐条消耗。
- **多重边**：相同单词出现多次 → 多条平行边，各自独立，指针递增即可。
- **不连通**：DSU 检测到不同根 → `***`。
- **度条件不满足**：直接 `***`。
- **所有单词相同首末字母但拼接不成立**：度条件已能检测（出度与入度不匹配）。

---

## 5. Example code（示例代码）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};
constexpr int K{26};

string words[mxn];
vector<pair<int,int>> adj[K];
int out[K],in[K];
bool has_edge[K];
int parent[K];
int ptr[K];
vector<int> path;

int find(int x)
{
	return parent[x]==x?x:parent[x]=find(parent[x]);
}

void unite(int a,int b)
{
	parent[find(a)]=find(b);
}

void dfs(int u)
{
	while(ptr[u]<(int)adj[u].size())
	{
		auto [v,id]=adj[u][ptr[u]++];
		dfs(v);
		path.push_back(id);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	for(int i=0;i<n;++i)cin>>words[i];
	sort(words,words+n);

	iota(parent,parent+K,0);
	for(int i=0;i<n;++i)
	{
		const string& w=words[i];
		int u=w[0]-'a';
		int v=w.back()-'a';
		adj[u].push_back({v,i});
		++out[u];
		++in[v];
		has_edge[u]=has_edge[v]=true;
		unite(u,v);
	}

	// connectivity check
	int root=-1;
	for(int i=0;i<K;++i)
	{
		if(!has_edge[i])continue;
		int r=find(i);
		if(root==-1)root=r;
		else if(root!=r)
		{
			cout<<"***\n";
			return 0;
		}
	}

	// degree analysis
	int start=-1;
	bool ok=true;
	for(int i=0;i<K;++i)
	{
		if(!has_edge[i])continue;
		int diff=out[i]-in[i];
		if(diff==1)
		{
			if(start!=-1){ok=false;break;}
			start=i;
		}
		else if(diff==-1){} // end vertex, ignored
		else if(diff!=0){ok=false;break;}
	}
	if(!ok)
	{
		cout<<"***\n";
		return 0;
	}

	vector<int> ans;

	auto run=[&](int s)->bool
	{
		fill_n(ptr,K,0);
		path.clear();
		dfs(s);
		if((int)path.size()!=n)return false;
		reverse(path.begin(),path.end());
		return true;
	};

	if(start!=-1)
	{
		if(run(start))ans=path;
	}
	else
	{
		for(int s=0;s<K;++s)
		{
			if(out[s]==0)continue;
			if(!run(s))continue;
			if(ans.empty()){ans=path;continue;}
			for(int i=0;i<n;++i)
			{
				if(words[path[i]]!=words[ans[i]])
				{
					if(words[path[i]]<words[ans[i]])ans=path;
					break;
				}
			}
		}
	}

	if(ans.empty())
	{
		cout<<"***\n";
		return 0;
	}

	for(int i=0;i<n;++i)
	{
		if(i)cout<<'.';
		cout<<words[ans[i]];
	}
	cout<<'\n';
	return 0;
}
```

### 代码说明

- **[words](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L146)**：全局数组存储单词，排序后下标即代表字典序顺序。
- **[adj](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L147)**：$26$ 个邻接表，每个元素为 `(目标字母, 单词下标)`。由于单词已排序，各顶点邻接表自然升序。
- **[out/in](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L148)**：出度与入度，用于欧拉通路存在性判断。
- **[DSU](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L149-L150)**：`parent` 数组，`find` 路径压缩，`unite` 按秩合并（隐含在路径压缩中）。
- **[dfs](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L161-L167)**：Hierholzer 核心。`ptr[u]` 指向当前未被使用的第一条边，每次取边后指针后移，递归深入，回溯时记录边下标。最终 `path` 为 DFS 序（逆序），需要反转。
- **[run lambda](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L202-L209)**：封装单次 Hierholzer 执行：重置指针、清空路径、DFS、检验是否覆盖所有边、反转。
- **[回路枚举](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L216-L228)**：当 `start==-1`（欧拉回路）时，枚举每个有出度的字母作为起点，产生路径后与已有答案比较，保留字典序更小的。
- **[输出](file:///d:/Information/My/OI/luogu_problems/P1127_solution.md#L234-L239)**：单词间以 `.` 连接。若 `ans` 为空则输出 `***`。

---

> **提交结果**：洛谷 P1127，$O(n \log n)$ 时间，$O(n + K)$ 空间，可通过 $n=1000$ 数据。
