# P1126 [CERC1996] 机器人搬重物 解题报告

---

## 1. Re-read the problem

**题目类型**：最短步数搜索（无权图最短路），BFS。

**物理模型**：
- 储藏室是 $N\times M$ 的网格（$N,M\le 50$），某些格子是障碍物。
- 机器人是一个直径 $1.6$ 的球体，**球心始终在格点上**（格点是网格线交叉点，即 $[0,N]\times[0,M]$ 范围内的整数坐标点）。
- 因为球直径 $1.6>1$，球心在格点 $(x,y)$ 时，球体覆盖 **4 个格子**：以该格点为左上角的 $2\times2$ 方块。即格子 $(x,y),\;(x+1,y),\;(x,y+1),\;(x+1,y+1)$（均使用 0-indexed 格子坐标）。

**输入**：
- 第一行 $N,M$。
- 接下来 $N$ 行每行 $M$ 个数字：$0$ 为空地，$1$ 为障碍。
- 最后一行：$sx\;sy\;tx\;ty\;d_0$，其中 $(sx,sy)$ 和 $(tx,ty)$ 是 **起始与终点格子的左上角网格的行与列**（1-indexed），$d_0\in\{\tt E,S,W,N\}$ 为起始方向。

**操作与代价**：
1. 向前 1 步（Creep）—— 1 秒
2. 向前 2 步（Walk）—— 1 秒
3. 向前 3 步（Run）—— 1 秒
4. 左转（Left）—— 1 秒
5. 右转（Right）—— 1 秒

每次移动/转向均花费 **恰好 1 秒**。

**要求**：从起点到终点任意朝向的最少总时间，不可达输出 $-1$。

---

## 2. Chain-of-thought with full proofs

### 2.1 坐标系转换

输入格子坐标是 1-indexed，而格点坐标使用 0-indexed 更方便。令：

$$\text{起始格点}: \;(x_s,y_s) = (sx-1,\;sy-1)$$
$$\text{目标格点}: \;(x_t,y_t) = (tx-1,\;ty-1)$$

球心在格点 $(x,y)$ 时，占用的 4 个格子为：

$$\big\{(x,y),\;(x+1,y),\;(x,y+1),\;(x+1,y+1)\big\}$$

所以球心可停留的格点范围是 $x\in[0,N-2],\;y\in[0,M-2]$。

**合法性条件**：位置 $(x,y)$ 合法 $\iff$ 上述 4 个格子均未出界且未标记为障碍。

### 2.2 方向约定

定义方向编号 $d\in\{0,1,2,3\}$：

| $d$ | 方向 | 代号 | $\Delta x$ | $\Delta y$ |
|-----|------|------|-----------|-----------|
| 0   | 东   | E    | 0         | +1        |
| 1   | 南   | S    | +1        | 0         |
| 2   | 西   | W    | 0         | -1        |
| 3   | 北   | N    | -1        | 0         |

**转向规则**：
- 右转：$d' = (d+1)\bmod 4$
- 左转：$d' = (d+3)\bmod 4$

### 2.3 状态表示与图模型

将问题建模为无权有向图：

$$\text{状态}: \;(x,y,d),\quad x\in[0,N-2],\;y\in[0,M-2],\;d\in\{0,1,2,3\}$$

- 初始状态：$(x_s,y_s,d_0)$
- 终止状态集：$\{(x_t,y_t,d)\mid d\in\{0,1,2,3\}\}$（终点朝向任意）

**边（转移）**：

1. **转向**：$(x,y,d) \xrightarrow{1} (x,y,d')$，其中 $d'$ 为左转或右转后的方向。
2. **前进 $k$ 步**（$k=1,2,3$）：$(x,y,d) \xrightarrow{1} (x+k\Delta x_d,\;y+k\Delta y_d,\;d)$。

   **前进合法性引理**：前进 $k$ 步合法 $\iff$ 路径上的 **所有中间位置** $(x+i\Delta x_d,\;y+i\Delta y_d)$ 对于 $i=1,\dots,k$ 均合法（即该处球体不撞障碍）。

   **证明**：球体直径 $1.6>1$，从位置 $p$ 移动到 $p+\Delta p$ 必须经过两者之间所有位置对应的 $2\times2$ 方块覆盖区域。若任意中间位置非法，则球体在移动过程中必然撞上障碍物。由于移动是瞬时的（代价只与指令数有关），只需检查这些离散位置是否全部合法。

### 2.4 BFS 的正确性证明

**定理**：使用普通队列 BFS 可以求出从起始状态到每个状态的最短步数。

**证明**：
- 图中每条边的权值均为 1（每个指令耗时 1 秒）。
- BFS 按层（距离）递增的顺序扩展状态。首次访问某状态时记录的步数即为最短路，因为队列 FIFO 性质保证按距离非减顺序处理。
- 状态数至多 $(N-1)(M-1)\times 4 \le 49\times 49\times 4 = 9604$，BFS 可以在极短时间内完成。

**剪枝/边界**：
- 若 $N=1$ 或 $M=1$，则不存在任何合法的球心位置（$2\times2$ 方块会出界），直接输出 $-1$。
- 若起始位置非法，直接输出 $-1$。
- 前进时，一旦遇到非法中间位置，进一步更长的步数也必然非法，可立即停止尝试更大 $k$（单调性剪枝）。

### 2.5 关键细节

1. **障碍扩大法**：将原 $N\times M$ 障碍图转化为 $(N-1)\times(M-1)$ 的"球心可达图" $ok[i][j]$，其中 $ok[i][j]$ 为 false 当且仅当 $2\times2$ 方块内含障碍。这一步将问题简化为一个 **点障碍** 搜索。
2. **边界墙**：球心不可移动到 $x=N-1$ 或 $y=M-1$，因为 $x+1$ 或 $y+1$ 会出界。BFS 的搜索范围天然被限制在 $[0,N-2]\times[0,M-2]$。
3. **起点与终点相同**：直接输出 $0$。

---

## 3. Pattern summary

> **"格点机器人" + "多步前进 + 转向" → BFS on State Graph**
>
> **模式特征**：
> - 机器人占据多个格子的区域（通常是 $2\times2$）
> - 机器人位于 **格点**（而不是格子中心）
> - 操作包括：不同步长的直线移动 + 原地转向
> - 所有操作代价相同（或不同但为小整数）
>
> **解题模板**：
> 1. 将原始障碍图转化为 **机器人中心可达图**（障碍膨胀/腐蚀）
> 2. 定义状态 $(x, y, dir)$，状态数 = 点数 $\times$ 方向数
> 3. 使用 BFS（代价相同）或 Dijkstra（代价不同）求最短路
> 4. 前进时逐个检查中间位置，一旦非法则更长的步数也非法
> 5. 注意边界 —— 机器人中心不可太接近网格边缘
>
> **变体**：
> - 机器人形状不同（矩形、L 形等）→ 膨胀掩模不同
> - 不同步长代价不同 → Dijkstra / 0-1 BFS
> - 地图移动（机器人推箱子等）→ 状态增加箱子位置维度

---

## 4. Real solution with notation glossary

### 符号表

| 符号 | 含义 |
|------|------|
| $N,M$ | 原网格行数、列数 |
| $a_{i,j}$ | 原障碍图，$0$ 为空地、$1$ 为障碍 |
| $\mathit{ok}_{i,j}$ | 球心可达图，$\mathit{ok}_{i,j}=1$ 表示球心在格点 $(i,j)$ 时不撞障碍 |
| $\mathit{dist}_{x,y,d}$ | 从起点到状态 $(x,y,d)$ 的最短时间 |
| $q$ | BFS 队列，存储三元组 $(x,y,d)$ |
| $d$ | 当前朝向，$0$ 东、$1$ 南、$2$ 西、$3$ 北 |
| $\mathit{dx}_d,\mathit{dy}_d$ | 方向 $d$ 对应的步进量 |
| $\infty$ | 不可达标记，取 $0x3f3f3f3f$ |
| $k$ | 前进步数，$k=1,2,3$ |

### 算法流程

```
算法：BFS 求最短路

1. 读入 N, M 和障碍图 a[N][M]
2. 若 N<2 或 M<2，输出 -1 并结束

3. 球心可达图预处理：
   for i = 0..N-2:
     for j = 0..M-2:
       ok[i][j] = !(a[i][j] || a[i+1][j] || a[i][j+1] || a[i+1][j+1])

4. 读入 sx,sy,tx,ty,dir_char
   x_s = sx-1, y_s = sy-1, x_t = tx-1, y_t = ty-1
   转换 dir_char 为方向编号 d0

5. 若 ok[x_s][y_s] == false，输出 -1 并结束

6. 初始化：
   dist[0..N-2][0..M-2][0..3] = INF
   dist[x_s][y_s][d0] = 0
   q.push((x_s, y_s, d0))

7. BFS 主循环：
   while q 非空:
     (x,y,d) = q.pop()
     cur = dist[x][y][d]

     // 转向
     for each d' in {左转, 右转}:
       if dist[x][y][d'] > cur+1:
         dist[x][y][d'] = cur+1, q.push((x,y,d'))

     // 前进 k=1,2,3 步
     for k = 1..3:
       nx = x + k*dx[d], ny = y + k*dy[d]
       if nx,ny 出界: break
       // 检查所有中间位置
       blocked = false
       for step = 1..k:
         if !ok[x+step*dx[d]][y+step*dy[d]]:
           blocked = true, break
       if blocked: break      // 更长的步数也必然非法
       if dist[nx][ny][d] > cur+1:
         dist[nx][ny][d] = cur+1, q.push((nx,ny,d))

8. 答案 ans = min_{d} dist[x_t][y_t][d]
   若 ans == INF: 输出 -1
   否则输出 ans
```

### 复杂度分析

- 状态数：$O(NM)$，$N,M\le 50$。
- 每个状态至多扩展 $2$ 个转向 + $3$ 个前进（每个前进最多检查 $3$ 个中间位置），均摊常数。
- 总时间复杂度 $O(NM)$，空间复杂度 $O(NM)$。

---

## 5. Example code

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{55};
constexpr int INF{0x3f3f3f3f};

int a[mxn][mxn];
bool ok[mxn][mxn];
int dist[mxn][mxn][4];
int dx[4]{0,1,0,-1};	// E,S,W,N
int dy[4]{1,0,-1,0};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int N,M;
	cin>>N>>M;
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
			cin>>a[i][j];

	if(N<2||M<2)
	{
		cout<<-1<<'\n';
		return 0;
	}

	for(int i=0;i<N-1;++i)
		for(int j=0;j<M-1;++j)
			ok[i][j]=!(a[i][j]||a[i+1][j]||a[i][j+1]||a[i+1][j+1]);

	int sx,sy,tx,ty;
	char dir0;
	cin>>sx>>sy>>tx>>ty>>dir0;
	--sx;--sy;--tx;--ty;

	int dir;
	if(dir0=='E')dir=0;
	else if(dir0=='S')dir=1;
	else if(dir0=='W')dir=2;
	else dir=3;

	if(!ok[sx][sy])
	{
		cout<<-1<<'\n';
		return 0;
	}

	memset(dist,0x3f,sizeof(dist));
	queue<array<int,3>>q;
	dist[sx][sy][dir]=0;
	q.push({sx,sy,dir});

	while(!q.empty())
	{
		auto [x,y,d]=q.front();q.pop();
		int cur=dist[x][y][d];

		// turn left
		int nd=(d+3)&3;
		if(dist[x][y][nd]>cur+1)
		{
			dist[x][y][nd]=cur+1;
			q.push({x,y,nd});
		}
		// turn right
		nd=(d+1)&3;
		if(dist[x][y][nd]>cur+1)
		{
			dist[x][y][nd]=cur+1;
			q.push({x,y,nd});
		}
		// move forward 1~3 steps
		for(int k=1;k<=3;++k)
		{
			int nx=x+k*dx[d];
			int ny=y+k*dy[d];
			if(nx<0||nx>=N-1||ny<0||ny>=M-1)break;
			bool blocked=false;
			for(int s=1;s<=k;++s)
			{
				int mx=x+s*dx[d];
				int my=y+s*dy[d];
				if(!ok[mx][my])
				{
					blocked=true;
					break;
				}
			}
			if(blocked)break;
			if(dist[nx][ny][d]>cur+1)
			{
				dist[nx][ny][d]=cur+1;
				q.push({nx,ny,d});
			}
		}
	}

	int ans=INF;
	for(int d=0;d<4;++d)
		ans=min(ans,dist[tx][ty][d]);

	if(ans>=INF)cout<<-1<<'\n';
	else cout<<ans<<'\n';

	return 0;
}
```
