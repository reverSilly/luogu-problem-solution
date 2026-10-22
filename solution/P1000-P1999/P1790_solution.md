# P1790 矩形分割

## 1. 重新读题

给定一个长为 $a$、宽为 $b$（$1 \le a \le 6,\ 2 \le b \le 6$）的方格矩形（共 $a \times b$ 个单位方格）。求将该矩形沿网格线切分成**两个四连通部分**（且每个部分至少包含一个位于大矩形最外圈的方格）的方案总数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：分割线与网格点简单路径的双射

把 $a \times b$ 个方格的顶点看作 $(a+1) \times (b+1)$ 的格点网 $\{(x, y) : 0 \le x \le a,\ 0 \le y \le b\}$。
什么时候把大矩形切成两半，且两半都至少含有一个最外圈方格？
- 如果切口是一个不接触外边界的内部闭合环，那么环内部的那一块完全被包在矩形内部，不含任何外圈方格，非法！
- 反之，只要切口是一条**从一个外边界格点出发、中间只经过严格内部格点 $(1 \dots a-1) \times (1 \dots b-1)$、最终到达另一个外边界格点的简单路径**，它就会把矩形外框切成两段非空弧，从而分出的两部分必然各自包含外边界方格且连通！

**引理 1（内部格点简单路径计数定理）**：
将矩形合法切分为两部分的方案，与格点网中满足以下条件的**无向简单路径**一一对应：
1. 两个端点均位于外边界（$x \in \{0, a\}$ 或 $y \in \{0, b\}$），且不属于同一条外边界上的相邻退化边；
2. 路径上的所有中间顶点均属于严格内部格点集合 $\{1, \dots, a-1\} \times \{1, \dots, b-1\}$（特别地，当 $a = 1$ 时无内部格点，仅有 $b - 1$ 条长度为 $1$ 的横切线段）。
若枚举每个边界起点向内部做 DFS，每条长度 $\ge 2$ 的无向割线恰好被正反两个方向各搜到 $1$ 次，故将有向路径总数除以 $2$ 即为答案！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用平面图对偶性将“切分为两个含外圈方格的连通块”转化为“连接两个外边界格点且中间仅走严格内部格点的简单路径”（引理 1）。
- **classic problem solution** — 从所有严格内部格点的边界邻居出发在 $(a-1)\times(b-1)$ 内部网格上回溯搜索简单路径，结果除以 $2$（特判 $a=1$ 时直接输出 $b-1$）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `a, b` | `int` | 矩形边长（$1 \le a \le 6, 2 \le b \le 6$） | 标准输入 | 维度 |
| `vis[x][y]` | `bool [8][8]` | 格点 $(x, y)$ 是否已在当前路径上 | `false` | visited |
| `ans` | `i64` | 有向割线路径计数 | `0` | directed paths |

### 4.2 算法步骤

1. 读入 $a, b$。若 $a = 1$，直接输出 $b - 1$ 并结束。
2. 定义 `dfs(x, y)`：
   - 若 $(x, y)$ 已到达外边界（$x = 0 \lor x = a \lor y = 0 \lor y = b$），令 `++ans` 并返回；
   - 标记 `vis[x][y] = true`，向四个方向 $(nx, ny)$ 扩展（若 `!vis[nx][ny]` 则递归 `dfs(nx, ny)`），回溯置 `vis[x][y] = false`。
3. 枚举四条外边界上的非角点格点，先将该边界点标记为 `vis = true`（防止第一步退回同一边界点），从其紧邻的内部格点调用 `dfs`，最后输出 `ans / 2`。

### 4.3 复杂度分析

- **时间复杂度**：内部格点至多 $5 \times 5 = 25$ 个，且任意到达边界的步数即终止，在 $a, b \le 6$ 下总搜索节点数约 $10^5$，耗时 $< 5\text{ ms}$。
- **空间复杂度**：$8 \times 8$ 的布尔数组与深度 $\le 25$ 的递归栈，空间复杂度为 $\Theta(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1790 矩形分割
// https://www.luogu.com.cn/problem/P1790
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int a,b;
bool vis[10][10];
i64 ans;
const int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};

void dfs(int x,int y)
{
	if(x==0||x==a||y==0||y==b)
	{
		++ans;
		return;
	}
	vis[x][y]=true;
	for(int d=0;d<4;++d)
	{
		int nx=x+dx[d],ny=y+dy[d];
		if(!vis[nx][ny])
			dfs(nx,ny);
	}
	vis[x][y]=false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;
	if(a==1)
	{
		cout<<b-1<<'\n';
		return 0;
	}

	for(int x=1;x<a;++x)
	{
		vis[x][0]=true;dfs(x,1);vis[x][0]=false;
		vis[x][b]=true;dfs(x,b-1);vis[x][b]=false;
	}
	for(int y=1;y<b;++y)
	{
		vis[0][y]=true;dfs(1,y);vis[0][y]=false;
		vis[a][y]=true;dfs(a-1,y);vis[a][y]=false;
	}

	cout<<ans/2<<'\n';
	return 0;
}
```
