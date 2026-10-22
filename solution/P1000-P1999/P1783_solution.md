# P1783 海滩防御

## 1. 重新读题

海滩位于条带区域 $0 \le x \le N$（$1 \le N \le 1000$），其中 $x = 0$ 为左边界、$x = N$ 为右边界。区域内建有 $M$（$1 \le M \le 800$）座信号塔，坐标为 $(X_i, Y_i)$（$0 \le X_i \le N, 0 \le Y_i \le 10^5$）。

每座信号塔的监视范围是以自身为圆心、半径为 $R$ 的闭圆盘。求最小的半径 $R$，使得所有信号塔的圆盘并集能够将从海滩下方（$y = -\infty$）到内地上方（$y = +\infty$）的所有连续通道完全封死（保留两位小数）。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：平面Alexander/对偶连通性与瓶颈最短路

**引理 1（封锁上下通道的对偶连通性充要条件）**：
在条带 $[0, N] \times \mathbb{R}$ 中，半径为 $R$ 的圆盘并集阻断所有从 $y = -\infty$ 到 $y = +\infty$ 的连续曲线，当且仅当圆盘并集中存在一条连接左边界直线 $x = 0$ 与右边界直线 $x = N$ 的连通链！
将左边界 $x = 0$ 视为虚拟源点 $S$、右边界 $x = N$ 视为虚拟汇点 $T$，则：
1. 信号塔 $i$ 的圆盘触及左边界 $x = 0 \iff R \ge X_i$（连边 $(S, i)$，权值 $X_i$）；
2. 信号塔 $i$ 的圆盘触及右边界 $x = N \iff R \ge N - X_i$（连边 $(i, T)$，权值 $N - X_i$）；
3. 信号塔 $i$ 与 $j$ 的圆盘相交 $\iff R \ge \frac{1}{2}\operatorname{dist}(i, j)$（连边 $(i, j)$，权值 $\frac{1}{2}\operatorname{dist}(i, j)$）。
因此，最小封锁半径 $R$ 恰好等于该完全图上从 $S$ 到 $T$ 的**最小瓶颈路径（Min-Max Path）**权值！

**证明**：由平面拓扑对偶原理（Hex 定理 / 矩形内上下连通与左右连通的互补性），条带内上下存在不碰圆盘的开通道，当且仅当左边界 $x=0$ 与右边界 $x=N$ 不在圆盘闭集的同一连通分支中。而圆盘间的相交条件恰为圆心距 $\le 2R$、圆盘与竖直边界相交条件恰为水平距离 $\le R$，故 $S$ 与 $T$ 连通的最小阈值 $R$ 即为 $S \to T$ 路径最大边权的最小值。$\blacksquare$

在稠密图（$M \le 800$）上直接运行不带堆的 $\Theta(M^2)$ 瓶颈 Dijkstra 算法，连二分都不需要，一步算出精确答案！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用平面上下通道与左右屏障的对偶性，将“封死海滩”转化为“左边界 $x=0$ 到右边界 $x=N$ 的圆盘连通”（引理 1）。
- **classic problem solution** — 建出含虚拟源汇点的完全图，用 $\Theta(M^2)$ 的瓶颈最短路算法（Min-Max Dijkstra）一步求出最小最大边权。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 海滩宽度 $N$ 与信号塔数 $M$ | 标准输入 | 维度 |
| `x[i], y[i]` | `double [805]` | 第 $i$ 座塔的坐标 | 标准输入 | coordinates |
| `dis[i]` | `double [805]` | 从左边界 $x=0$ 到节点 $i$ 的最小瓶颈半径 | 初值 `x[i]` | bottleneck dist |
| `vis[i]` | `bool [805]` | 节点 $i$ 是否已确定最小瓶颈值 | `false` | visited |

### 4.2 算法步骤

1. 读入 $N, M$ 及各塔坐标 $(X_i, Y_i)$。
2. 初始化每个塔 $i = 1 \dots M$ 的初始瓶颈距离 `dis[i] = x[i]`（直接连左边界），汇点 $T = M + 1$ 的初始距离 `dis[M + 1] = N`。
3. 循环 $M + 1$ 次：
   - 在未访问节点中找出 `dis[u]` 最小的节点 $u$，标记 `vis[u] = true`；若 $u == M + 1$ 则跳出；
   - 用 `max(dis[u], N - x[u])` 更新汇点 `dis[M + 1]`；
   - 对所有未访问塔 $v$，用 `max(dis[u], 0.5 * hypot(x[u] - x[v], y[u] - y[v]))` 更新 `dis[v]`。
4. 将 `dis[M + 1]` 保留两位小数输出。

### 4.3 复杂度分析

- **时间复杂度**：共 $M + 1$ 轮，每轮扫描 $M$ 个点，总时间复杂度为 $\Theta(M^2)$（$M = 800$ 时仅 $6.4 \times 10^5$ 次计算，耗时 $< 5\text{ ms}$）。
- **空间复杂度**：一维数组长度 $805$，空间复杂度为 $\Theta(M)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1783 海滩防御
// https://www.luogu.com.cn/problem/P1783
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{805};

int n,m;
double x[mxm],y[mxm],dis[mxm];
bool vis[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=1;i<=m;++i)
	{
		cin>>x[i]>>y[i];
		dis[i]=x[i];
	}
	dis[m+1]=n;

	for(int it=1;it<=m+1;++it)
	{
		int u=0;
		for(int i=1;i<=m+1;++i)
			if(!vis[i]&&(u==0||dis[i]<dis[u]))
				u=i;
		vis[u]=true;
		if(u==m+1)
			break;
		dis[m+1]=min(dis[m+1],max(dis[u],n-x[u]));
		for(int v=1;v<=m;++v)
			if(!vis[v])
			{
				double w=0.5*hypot(x[u]-x[v],y[u]-y[v]);
				dis[v]=min(dis[v],max(dis[u],w));
			}
	}

	cout<<fixed<<setprecision(2)<<dis[m+1]<<'\n';
	return 0;
}
```
