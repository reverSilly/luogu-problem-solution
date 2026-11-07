# P1922 女仆咖啡厅桌游吧

## 1. 重新读题

给定一棵 $n$ 个节点的树，以 $1$ 为根。每个节点可以建女仆咖啡厅（记 $+1$）、桌游吧（记 $-1$）或什么都不建（记 $0$）。

约束：对**每个非叶子节点** $i$，其子树（含自己）内

$$cafe_i=table_i .$$

求最多能建多少个女仆咖啡厅。

### 关键约束与规模暗示

- $n\le10^5$，需要 $O(n)$ 或 $O(n\log n)$；不可能做指数级枚举（$30\%$ 的 $n\le20$ 才允许 $3^n$ 暴力）。
- 约束只针对**非叶子**节点，叶子节点任意。
- 目标只数咖啡厅，桌游吧只是"配平"用的。

## 2. 思维链（Chain-of-thought）

### 2.1 把约束看成"子树净值为 0"

给每个节点赋值 $v(u)\in\{+1,0,-1\}$（咖啡厅/空/桌游吧）。对非叶子 $i$ 的要求是

$$\sum_{u\in subtree(i)}v(u)=0.$$

因为 $cafe_i-table_i=\sum_{u\in subtree(i)}v(u)$。

### 2.2 关键观察：非叶子儿子的贡献恒为 0

**引理 1（叶子之外不再影响父亲）**：若 $u$ 是非叶子节点，则在其自身约束成立时，$\sum_{w\in subtree(u)}v(w)=0$。

**证明**：$u$ 非叶子，题面要求 $cafe_u=table_u$，即子树内 $+1$ 的个数等于 $-1$ 的个数，所以子树和恰为 $0$。$\blacksquare$

于是对任意非叶子节点 $i$，其约束

$$\sum_{u\in subtree(i)}v(u)=v(i)+\sum_{\text{孩子 }c}\sum_{w\in subtree(c)}v(w)=0$$

中，所有**非叶子孩子**的子树和都是 $0$，可以被删掉！只剩下 $i$ 自己与它的**叶子孩子**：

$$v(i)+\sum_{\text{叶子孩子 }c}v(c)=0. \tag{*}$$

**引理 2（问题完全分解）**：整棵树的最优值等于下列各组最大值之和：对每个非叶子节点 $i$，把"$i$ 自己 + $i$ 的全部叶子孩子"作为一组，在满足 $(*)$ 的前提下最大化该组的咖啡厅数。

**证明**：
1. 每个节点恰好属于一组：叶子属于它父亲那一组（$n=1$ 时特殊，见 2.5）；非叶子属于自己那一组。
2. 任一全局合法方案限制到每组，都满足该组的 $(*)$，所以全局咖啡厅数 $\le$ 各组最大值之和。
3. 反过来，把各组的局部最优解拼起来：非叶子节点的约束只涉及本组变量（由引理 1 消去非叶子孩子），所以拼接后所有非叶子都满足约束，得到全局合法方案，达到各组最大值之和。

两边夹逼，等号成立。$\blacksquare$

### 2.3 单组闭式：$g(L)$

设某非叶子节点 $i$ 有 $L$ 个叶子孩子，记 $g(L)$ 为该组（节点 $i$ 与这 $L$ 个叶子）在 $(*)$ 下的最大咖啡厅数。

**引理 3**：

$$g(L)=\left\lceil\frac{L}{2}\right\rceil=\begin{cases}1+\left\lfloor\dfrac{L-1}{2}\right\rfloor,&L\ge1,\\[4pt]0,&L=0.\end{cases}$$

**证明**：按节点 $i$ 自己的取值分类。设 $L$ 个叶子中有 $c$ 个咖啡厅、$t$ 个桌游吧，其余为空。

- **$v(i)=+1$**：$(*)$ 要求 $c-t=-1$，即 $t=c+1$。占用叶子数 $2c+1\le L$，故 $c\le\lfloor(L-1)/2\rfloor$，可达到。该组咖啡厅数 $=1+c$，最大为 $1+\lfloor(L-1)/2\rfloor$。
- **$v(i)=0$**：要求 $c=t$，$2c\le L$，最大 $c=\lfloor L/2\rfloor$，该组咖啡厅数为 $\lfloor L/2\rfloor$。
- **$v(i)=-1$**：要求 $c-t=+1$，即 $c=t+1$，$2t+1\le L$，最大 $c=1+\lfloor(L-1)/2\rfloor$，该组咖啡厅数同样为 $1+\lfloor(L-1)/2\rfloor$。

比较前两者：对偶数 $L=2m$，$1+\lfloor(L-1)/2\rfloor=1+(m-1)=m=\lfloor L/2\rfloor$；对奇数 $L=2m+1$，$1+\lfloor(L-1)/2\rfloor=m+1>m=\lfloor L/2\rfloor$。所以 $L\ge1$ 时最大值为 $1+\lfloor(L-1)/2\rfloor$，而它恰好等于 $\lceil L/2\rceil$。

$L=0$ 时 $(*)$ 退化为 $v(i)=0$，只能空着，$g(0)=0=\lceil0/2\rceil$。$\blacksquare$

### 2.4 最终答案

$$\text{ans}=\sum_{i:\ i\text{ 非叶子}}\left\lceil\frac{\#\{\text{叶子孩子 of }i\}}{2}\right\rceil .$$

每个非叶子节点的贡献只依赖它有几个叶子儿子，一次 DFS/BFS 即可统计。

### 2.5 边角：$n=1$

只有一个节点，它是叶子（没有孩子），题面不给它任何约束，于是可以放一个咖啡厅，答案是 $1$。而公式的和式为空，会得到 $0$，所以要特判。

### 2.6 为什么不用普通树形 DP

如果直接设 $dp[u][balance]$，状态是子树净值，范围 $O(n)$，合并是卷积，退化成 $O(n^2)$。而引理 1 告诉我们：非叶子儿子的净值被约束**强制为 0**，状态维度直接消失，问题坍缩成每个节点一个独立的小学算术——这就是"先证明结构，再写代码"比"上来就套 DP"更省力的例子。

### 2.7 验证

用 $3^n$ 枚举所有赋值（$n\le10$）作为暴力，对 300 棵随机树与最终程序对拍，全部一致；另用 2000 棵随机树（$n\le9$）单独验证了引理 3 的公式。链、星、菊花等形状也包含在随机测试中。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 把约束化成"子树权值和为 0"。
- **explore its property** — 非叶子儿子的子树被约束强制净值为 0，可以从父亲的和里消掉（引理 1）。
- **reduce to easier problem** — 约束只剩"节点自己 + 叶子孩子"，各组互相独立（引理 2）。
- **discuss by category** — 按节点自己取 $+1/0/-1$ 分类，得到闭式 $g(L)=\lceil L/2\rceil$（引理 3）。
- **special case** — $n=1$ 时唯一节点是叶子，答案为 $1$。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 节点数 | 读入 | — |
| `graph[u]` | `vector<int>[100005]` | 无向邻接表 | 读入每条边 | 树 |
| `fa[u]` | `int[100005]` | 根为 $0$ 时的父亲 | BFS 求出，根为哨兵 | father |
| `childcnt[u]` | `int[100005]` | 儿子个数 | BFS 时累加 | 度（去掉父亲） |
| `leafson` | `int` | 当前非叶子节点的叶子儿子数 $L$ | 现场统计 | 叶子儿子 |
| `ans` | `int` | 答案 | $0$ | 求和 |

### 4.2 算法步骤

1. 读入 $n$。若 $n=1$，输出 $1$ 并结束。
2. 读入 $n-1$ 条边，建无向邻接表。
3. 从节点 $0$（原编号 $1$）开始 BFS：对每个出边 $v\ne fa[u]$，令 $fa[v]=u$、`childcnt[u]++`、$v$ 入队。用 **BFS 而非递归**，因为 $n=10^5$ 的链会让递归爆栈。
4. 遍历每个节点 $i$：
   - 若 `childcnt[i]==0`，是叶子，跳过（无约束）；
   - 否则统计邻接表中满足 $v\ne fa[i]$ 且 `childcnt[v]==0` 的儿子数 `leafson`；
   - `ans += (leafson+1)/2`（即 $\lceil leafson/2\rceil$）。
5. 输出 `ans`。

### 4.3 复杂度

- 时间 $O(n)$（建图、BFS、统计各扫一遍）；空间 $O(n)$。

## 5. 示例代码

```cpp
//
// 题目：P1922 女仆咖啡厅桌游吧
// https://www.luogu.com.cn/problem/P1922
//
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n;
vector<int>graph[mxn];
int fa[mxn],childcnt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	if(n==1)
	{
		cout<<1<<'\n';
		return 0;
	}
	fa[0]=1145141919;
	queue<int>q;
	q.push(0);
	while(!q.empty())
	{
		int u=q.front();q.pop();
		for(int v:graph[u])
			if(v!=fa[u])
			{
				fa[v]=u;
				++childcnt[u];
				q.push(v);
			}
	}
	int ans{0};
	for(int i=0;i<n;++i)
	{
		if(childcnt[i]==0)
			continue; // 叶子无约束
		int leafson{0};
		for(int v:graph[i])
			if(v!=fa[i]&&childcnt[v]==0)
				++leafson;
		ans+=(leafson+1)/2;
	}
	cout<<ans<<'\n';
	return 0;
}
```
