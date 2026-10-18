# P1522 [USACO2.4] 牛的旅行 Cow Tours

> 标签：图论 / 最短路 / 枚举 / Floyd-Warshall · 难度：提高/省选-
> 本地题面：`P1522.md`（$1\le N\le 150$，$0\le X_i,Y_i\le 10^5$）。

## 1. 重新读题

**问题重述（人话版）**

平面上有 $N$（$N\le 150$）个牧区，第 $i$ 个牧区坐标为 $(X_i,Y_i)$。给定 $N\times N$ 的 $0/1$ 对称邻接矩阵 $M$，$M_{i,j}=1$ 表示 $i,j$ 之间有一条长度为欧氏距离 $\mathrm{euc}(i,j)=\sqrt{(X_i-X_j)^2+(Y_i-Y_j)^2}$ 的无向边。

每个**极大连通分量**称为一个“牧场”。一个牧场 $C$ 的**直径**定义为该连通块内任意两点间最短路径长度的最大值：

$$D(C)=\max_{u,v\in C}\mathrm{dist}(u,v).$$

已知图中至少有两个不连通的牧区。现在要求选出属于**两个不同牧场**的一对牧区 $(u,v)$（即 $u\in C_1,v\in C_2,C_1\ne C_2$），在它们之间新连一条长度为 $\mathrm{euc}(u,v)$ 的直线路径，使得合并产生的新牧场 $C_1\cup C_2$ 的直径尽可能小。输出这个最小的新牧场直径（保留 $6$ 位小数）。

> 一句话版本：**在无向带权图中选两个不同连通分量连一条桥边 $(u,v)$，最小化合并后连通块 $C(u)\cup C(v)$ 的直径。**

**题面里容易漏掉的条款**（其中第 1 条是本题最著名的 WA 坑点）

1. **新牧场的直径不一定经过新连的那条边 $(u,v)$** $\Rightarrow$ 合并后的新牧场是 $C_1\cup C_2$，其直径是 $C_1\cup C_2$ 中**所有点对**最短路的最大值！如果 $C_1$ 原本的内部直径 $D(C_1)$ 本身就已经比跨桥最远距离 $\mathrm{mxd}[u]+\mathrm{euc}(u,v)+\mathrm{mxd}[v]$ 更大，那么合并后新牧场的直径就是 $D(C_1)$，而不是跨桥距离！
2. **题目问的是“合并出的那个新牧场”的直径，而非全图所有牧场的最大直径** $\Rightarrow$ 若图中本来有 $3$ 个牧场 $C_1,C_2,C_3$，连接 $C_1$ 与 $C_2$ 时，题面明确说“使得连通后**这个新的更大的牧场**的直径尽可能小”，因此候选直径只需对 $D(C_1)$、$D(C_2)$ 与跨桥距离取 $\max$。
3. **邻接矩阵 $0/1$ 字符可能紧挨着无空格** $\Rightarrow$ 如 `01000000`，读入时必须用 `char ch; cin >> ch;` 逐字符读取，而不能按 `int` 读一整行（否则一整行会被当成一个十进制大整数）。

**数据范围与规模感受**

$$1\le N\le 150,\qquad 0\le X_i,Y_i\le 10^5 .$$

$N\le 150$ 是标准的 **Floyd-Warshall 全源最短路**规模：$N^3\approx 3.375\times 10^6$ 次运算，几毫秒即可算出全部点对的最短路 $\mathrm{dist}(i,j)$，随后枚举所有不同连通块点对 $(u,v)$ 只需 $O(N^2)$。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：加一条桥边 $(u,v)$ 后，点对最短路怎么变？

设 $u\in C_1,v\in C_2$（$C_1\ne C_2$），新加边 $e=(u,v)$ 的长度为 $w=\mathrm{euc}(u,v)>0$。考察合并后连通块 $C_{\mathrm{new}}=C_1\cup C_2$ 中任意两点 $a,b$ 的新最短路距离 $\mathrm{dist}'(a,b)$：

**引理 1（桥边分解引理与新牧场直径公式）**

在 $C_1$ 与 $C_2$ 之间仅添加一条边 $(u,v)$（权值 $w=\mathrm{euc}(u,v)$）后：

1. 对任意 $a,b\in C_1$，有 $\mathrm{dist}'(a,b)=\mathrm{dist}(a,b)$；同理对任意 $a,b\in C_2$，有 $\mathrm{dist}'(a,b)=\mathrm{dist}(a,b)$。
2. 对任意 $a\in C_1,b\in C_2$，有
   $$\mathrm{dist}'(a,b)=\mathrm{dist}(a,u)+w+\mathrm{dist}(v,b).$$
3. 记每个顶点的偏心率（同连通块内最远最短路距离）为
   $$\mathrm{mxd}[x]=\max_{y\in C(x)}\mathrm{dist}(x,y),$$
   每个连通块 $C$ 的原直径为 $D(C)=\max_{x\in C}\mathrm{mxd}[x]$。则合并后新牧场 $C_1\cup C_2$ 的直径为
   $$D(C_1\cup C_2)=\max\Big\{D(C_1),\ D(C_2),\ \mathrm{mxd}[u]+\mathrm{euc}(u,v)+\mathrm{mxd}[v]\Big\}.$$

**证明**：

1. 因为 $(u,v)$ 是连接 $C_1$ 与 $C_2$ 的**唯一**一条边（割边/桥），任何从 $a\in C_1$ 出发进入 $C_2$ 又回到 $b\in C_1$ 的路径都必须至少经过边 $(u,v)$ 往返两次，这必然在点 $u$ 处形成长度为正的闭回路，删去该回路后路径严格变短且完全落在 $C_1$ 内部。因此同侧点对的最短路绝不经过 $(u,v)$，$\mathrm{dist}'(a,b)=\mathrm{dist}(a,b)$。
2. 对异侧点对 $a\in C_1,b\in C_2$，任何从 $a$ 到 $b$ 的简单路径都必须恰好沿 $u\to v$ 跨过唯一桥边 $(u,v)$ 一次，从而被拆成三段独立路径：$a\leadsto u$（在 $C_1$ 内）、边 $(u,v)$、$v\leadsto b$（在 $C_2$ 内）。三段各自取最短即得 $\mathrm{dist}'(a,b)=\mathrm{dist}(a,u)+w+\mathrm{dist}(v,b)$。
3. 将 $C_1\cup C_2$ 中的无序点对 $\{a,b\}$ 划分为三类：$\{a,b\}\subseteq C_1$、$\{a,b\}\subseteq C_2$、以及 $a\in C_1,b\in C_2$。
   - 前两类的最大值分别为 $\max_{a,b\in C_1}\mathrm{dist}(a,b)=D(C_1)$ 与 $\max_{a,b\in C_2}\mathrm{dist}(a,b)=D(C_2)$；
   - 第三类中，因 $a\in C_1$ 与 $b\in C_2$ 的选择彼此独立：
     $$\max_{a\in C_1,\,b\in C_2}\big(\mathrm{dist}(a,u)+w+\mathrm{dist}(v,b)\big)=\Big(\max_{a\in C_1}\mathrm{dist}(a,u)\Big)+w+\Big(\max_{b\in C_2}\mathrm{dist}(v,b)\Big)=\mathrm{mxd}[u]+w+\mathrm{mxd}[v].$$
   对三类取最大值即得公式 (3)。$\blacksquare$

### 2.2 反复碰壁：两个典型错误思路与反例

**失败 1：认为新牧场直径一定等于 $\mathrm{mxd}[u]+\mathrm{euc}(u,v)+\mathrm{mxd}[v]$，漏掉 $\max(D(C_1),D(C_2))$。**

- 反例（$N=4$，见 `P1522_check.py` 边界用例）：
  - 牧场 $C_1=\{0,1,2\}$：$0=(0,0),\,1=(100,0),\,2=(50,0)$，边为 $0\text{—}2$ 与 $2\text{—}1$。于是 $C_1$ 是一条长 $100$ 的链，其内部直径为 $D(C_1)=\mathrm{dist}(0,1)=100$，而中点 $2$ 的偏心率只有 $\mathrm{mxd}[2]=\max(50,50)=50$。
  - 牧场 $C_2=\{3\}$：孤立点 $3=(50,1)$，$D(C_2)=0,\mathrm{mxd}[3]=0$。
  - 若连接中点 $2$ 与点 $3$（距离 $1$），跨桥最大距离仅为 $\mathrm{mxd}[2]+1+\mathrm{mxd}[3]=50+1+0=51$；但合并后的牧场里，点 $0$ 到点 $1$ 的距离依然是 **$100$**！若漏写 `max(diam[c1], diam[c2], ...)`，会错误输出 `51.000000` 而非真实直径 `100.000000`。

**失败 2：贪心地只连接两个距离最近的不同牧场点对 $(u,v)$，或者只连接两个偏心率最小の中心点。**

- 为什么只看 $\mathrm{euc}(u,v)$ 最小不对？因为离得很近的两个边界点可能是各自连通块直径的端点（$\mathrm{mxd}[u],\mathrm{mxd}[v]$ 极大），连上后两条长链首尾相接，总直径翻倍；
- 为什么只连两个中心点（$\mathrm{mxd}$ 最小）也不对？因为两个中心点在平面上的欧氏距离 $\mathrm{euc}(u,v)$ 可能远大于其它点的距离。
- 由于合法点对 $(u,v)$ 总数至多 $\binom{N}{2}\le 11175$，直接枚举全部异块点对 $(u,v)$ 并用引理 1(3) $O(1)$ 算新直径，既精确又只需 $O(N^2)$ 时间。

## 3. 模式总结（Pattern Summary）

按实际思考顺序列出本题的可迁移模式：

- **explore its property（割边独立性）** — 两个不连通分量之间只加一条边时，该边必为桥（割边），同侧最短路不变，异侧最短路拆分为“左偏心距 + 桥长 + 右偏心距”（引理 1）。
- **classic problem solution（Floyd-Warshall 全源最短路）** — $N\le 150$ 直接跑 $O(N^3)$ Floyd，一次性获得连通性判定（$\mathrm{dist}(i,j)<\infty$）、各点偏心率 $\mathrm{mxd}[i]$ 与各连通块原直径 $D(C)$。
- **failed attempt（忽略原块内部直径）** — 被“中点架桥”反例击穿（跨桥距离 $51$ 小于原直径 $100$）；新直径必须对 $D(C_1),D(C_2)$ 与跨桥极值三者取 $\max$。
- **input defense（逐字符读取 0/1 矩阵）** — 用 `char ch; cin >> ch;` 兼容无空格与有空格的 $0/1$ 邻接矩阵。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 / 来源 |
|---|---|---|---|---|
| `n` | `int` | 牧区总数 $N$ | 读入 | $1\le n\le 150$ |
| `px[i], py[i]` | `double [155]` | 牧区 $i$ 的平面坐标 $(X_i,Y_i)$ | 读入 | point x, y |
| `dis[i][j]` | `double [155][155]` | 点 $i$ 到点 $j$ 的最短路径长度 | 对角线 $0$，有边为 $\mathrm{euc}(i,j)$，其余 `inf` ($10^{18}$) | Floyd 表 |
| `comp[i]` | `int [155]` | 牧区 $i$ 所属连通块编号（0-based） | `-1` | component ID |
| `mxd[i]` | `double [155]` | 牧区 $i$ 在自身连通块内的最远最短路距离 | $0$ | 引理 1 $\mathrm{mxd}[i]$ |
| `diam[c]` | `double [155]` | 连通块 $c$ 的原内部直径 $D(c)$ | $0$ | $\max_{i\in c}\mathrm{mxd}[i]$ |
| `ans` | `double` | 所有异块加边方案中新牧场直径的最小值 | `inf` ($10^{18}$) | 最终答案 |

### 4.2 编号步骤

1. 读入 $n$ 及各点坐标 `px[i], py[i]`；逐字符读入 $n\times n$ 邻接矩阵初始化 `dis[i][j]`（`i == j` 为 $0$，`ch == '1'` 为 `hypot(px[i]-px[j], py[i]-py[j])`，否则为 `inf`）。
2. 三重循环跑 Floyd-Warshall 更新全源最短路 `dis[i][j]`。
3. 扫描每个顶点 $i=0\dots n-1$：
   - 按 `dis[i][j] < inf / 2` 划分连通块编号 `comp[i]`；
   - 计算偏心率 `mxd[i] = max_{j: comp[j]==comp[i]} dis[i][j]`；
   - 更新连通块内部直径 `diam[comp[i]] = max(diam[comp[i]], mxd[i])`。
4. 枚举所有满足 `comp[i] != comp[j]` 的无序点对 $(i,j)$，按引理 1(3) 计算合并后直径 `nd = max({diam[comp[i]], diam[comp[j]], mxd[i] + euc(i, j) + mxd[j]})`，用 `nd` 更新 `ans = min(ans, nd)`。
5. 用 `fixed << setprecision(6)` 输出 `ans`。

### 4.3 复杂度

- **时间**：Floyd 全源最短路 $O(N^3)$，偏心率与枚举加边 $O(N^2)$，总时间复杂度 $O(N^3)\approx 3.4\times 10^6$ 次浮点运算。
- **空间**：`dis[155][155]` 矩阵，空间复杂度 $O(N^2)\approx 200\text{ KB}$。

### 4.4 验证状态小结

所有结论均由本地真实编译运行（`g++ -O2 -std=c++17`）及 `appendix/P1522/P1522_check.py` 验证：

| 项目 | 状态 |
|---|---|
| 官方样例回归 | **通过**：输出 `22.071068`，与题面 Hint 完全吻合。 |
| 随机数据对拍 | **通过**：$1000$ 组随机多连通块图（$N\in[2,16]$，连通块数 $2\sim 4$），标程 `P1522.cpp` 与每次真加边重算距离的暴力 `P1522_brute.cpp` 输出**逐字节一致**。 |
| 边界用例 | **通过**：内部直径主导反例（中点架桥距离 $51 <$ 原链直径 $100$），标程与暴力均正确输出 `100.000000`。 |
| 最坏规模计时 | $N=150$ 满规模测试：含进程启动最快耗时 **$0.0426\text{ s}$**。 |

## 5. 示例代码

### 5.1 主解：Floyd 全源最短路 + 偏心率与原直径预处理

```cpp
//
// 题目：P1522 [USACO2.4] 牛的旅行 Cow Tours
// https://www.luogu.com.cn/problem/P1522
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{155};           // N<=150 (+5 松弛)
constexpr double inf{1e18};

int n;
double px[mxn],py[mxn];
double dis[mxn][mxn];
double mxd[mxn];                  // mxd[i]=与 i 同连通块内的最远最短路距离(偏心率)
int comp[mxn];                    // comp[i]=i 所属连通块编号
double diam[mxn];                 // diam[c]=连通块 c 的原内部直径

double euc(int i,int j)
{
	return hypot(px[i]-px[j],py[i]-py[j]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>px[i]>>py[i];

	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			char ch;
			cin>>ch;
			if(i==j)
				dis[i][j]=0;
			else if(ch=='1')
				dis[i][j]=euc(i,j);
			else
				dis[i][j]=inf;
		}

	// Floyd-Warshall 全源最短路
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			if(dis[i][k]<inf/2)
				for(int j=0;j<n;++j)
					if(dis[k][j]<inf/2)
						dis[i][j]=min(dis[i][j],dis[i][k]+dis[k][j]);

	fill_n(comp,n,-1);
	int ccnt=0;
	for(int i=0;i<n;++i)
	{
		if(comp[i]==-1)
		{
			for(int j=0;j<n;++j)
				if(dis[i][j]<inf/2)
					comp[j]=ccnt;
			++ccnt;
		}
		for(int j=0;j<n;++j)
			if(dis[i][j]<inf/2)
				mxd[i]=max(mxd[i],dis[i][j]);
		diam[comp[i]]=max(diam[comp[i]],mxd[i]);
	}

	double ans=inf;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			if(comp[i]!=comp[j])
			{
				double nd=max({diam[comp[i]],diam[comp[j]],mxd[i]+euc(i,j)+mxd[j]});
				ans=min(ans,nd);
			}

	cout<<fixed<<setprecision(6)<<ans<<'\n';
	return 0;
}
```

### 5.2 对照实现：枚举加边后直接重算全对最短路（仅用于小规模对拍）

```cpp
//
// 题目：P1522 [USACO2.4] 牛的旅行 Cow Tours
// 暴力基准: 枚举每一对不同连通块顶点 (u,v), 实打实加边后求合并连通块的真实直径
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{45};
constexpr double inf{1e18};

int n;
double px[mxn],py[mxn];
double g[mxn][mxn],d[mxn][mxn];

double euc(int i,int j)
{
	return hypot(px[i]-px[j],py[i]-py[j]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>px[i]>>py[i];

	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			char ch;
			cin>>ch;
			if(i==j)
				g[i][j]=0;
			else if(ch=='1')
				g[i][j]=euc(i,j);
			else
				g[i][j]=inf;
			d[i][j]=g[i][j];
		}

	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				d[i][j]=min(d[i][j],d[i][k]+d[k][j]);

	double ans=inf;
	for(int u=0;u<n;++u)
		for(int v=u+1;v<n;++v)
			if(d[u][v]>inf/2)
			{
				double w=euc(u,v);
				double cur=0;
				for(int a=0;a<n;++a)
					if(d[a][u]<inf/2||d[a][v]<inf/2)
						for(int b=a+1;b<n;++b)
							if(d[b][u]<inf/2||d[b][v]<inf/2)
							{
								double dist_ab=min({d[a][b],d[a][u]+w+d[v][b],d[a][v]+w+d[u][b]});
								cur=max(cur,dist_ab);
							}
				ans=min(ans,cur);
			}

	cout<<fixed<<setprecision(6)<<ans<<'\n';
	return 0;
}
```
