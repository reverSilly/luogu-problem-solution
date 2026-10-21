# P1859 不听话的机器人

> 难度：普及/提高- · 标签：动态规划 DP

## 1. 重新读题
### 1.1 问题重述（人话版）

迷宫是 $M\times M$ 的格子，`.` 能走，`*` 不能走。机器人从 $(X_0,Y_0)$ 出发，一开始面朝上：沿初始朝向走一步，$X$ 坐标变小。依次收到 $N$ 条指令，只能整条接受或整条拒绝：

- `FORWARD` / `BACK`：沿当前朝向前进或后退一格；
- `LEFT` / `RIGHT`：左转或右转 $90^\circ$，不移动。

接受后的每一步都不能出界或踩障碍。问最少拒绝几条，才能让执行剩下指令的全过程都安全。

### 1.2 关键约束与规模暗示

$M\le 100$，$N\le 1000$。位置 $M^2$、朝向 $4$、指令进度 $N$，状态量 $4NM^2\le 4\cdot10^7$。要的是最少拒绝数，适合按指令分层的最短路 / DP，不能 $2^N$ 枚举。网格用 1-based，与输入坐标一致。

## 2. 思维链（Chain-of-thought）

约定朝向 $0,1,2,3$ 为上、右、下、左，增量
$$
(\Delta x,\Delta y)\in\{(-1,0),(0,1),(1,0),(0,-1)\}.
$$
`RIGHT` 把朝向 $+1\pmod 4$，`LEFT` 把朝向 $+3\pmod 4$。`BACK` 走 $-\Delta$。

**引理 1（拒绝是局部的）。** 一条指令要么完全不执行，要么在执行瞬间位置和朝向都合法。转弯永远合法，因为它不改坐标。

证明：题面只禁止走到障碍或界外，没有别的持续惩罚；不执行则状态不变。$\blacksquare$

**引理 2（最优子结构）。** 令 $dp_{i,x,y,d}$ 为处理完前 $i$ 条指令后，位于 $(x,y)$、朝向 $d$ 的最少拒绝数。则
$$
dp_{i+1,x,y,d}=\min(\ldots,\ dp_{i,x,y,d}+1)
$$
（拒绝第 $i$ 条），并且若接受合法，再转移到新位置或新朝向、拒绝数不变。答案是所有 $x,y,d$ 上 $dp_{N,x,y,d}$ 的最小值。

证明：任何合法的接受/拒绝序列，其前 $i$ 条结束时必处于某个格子和朝向，拒绝数不小于该状态的 $dp$。第 $i+1$ 条只有拒绝、或接受后走到唯一确定的新状态这两种选择，没有旁支。若最优序列的前缀不是对应状态的最优，把前缀换成更少拒绝的合法前缀，后缀照旧仍合法（后缀只依赖当前坐标和朝向），总拒绝数下降，矛盾。全部拒绝始终合法，所以从起点出发每个 $i$ 都至少有一条可达路径，答案不会停留在哨兵。初始 $dp_{0,X_0,Y_0,0}=0$，朝向 $0$ 是向上。$\blacksquare$

Vijos 样例迷宫与指令序列去掉第 3 条后可以安全走完，完整执行会在最后一步踩上 `*`，因此答案是 $1$。朝向定义与「初始向上使 $X$ 减小」一致，样例被这个 DP 算出 $1$。

滚动数组只留相邻两层：下一层先全部填成 $1145141919$，再从上一层转移。$0$ 是合法拒绝数，不能把未访问当成 $0$。

## 3. 模式总结（Pattern Summary）

- **state is pose plus progress** — 位置、朝向、处理到第几条指令
- **reject is always an edge** — 拒绝：同一姿态，代价 $+1$
- **turn or step is the other edge** — 接受：转弯改朝向，平移要检查 `.`
- **initial heading is up** — 朝向 $0$ 的 $\Delta x=-1$
- **roll two layers** — $N$ 有 $1000$，只保留当前层和下一层

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| `m,n,sx,sy` | `int` | 边长、指令数、起点 | 读入 | $M,N,X_0,Y_0$ |
| `g[105][105]` | `char` | 地图 | 读入 | `.` 或 `*` |
| `cmd[1005]` | `int` | 指令种类 $0..3$ | 读入 | 前退后、左右转 |
| `dp[2][105][105][4]` | `int` | 最少拒绝数 | `inf` | 滚动层 |
| `dx,dy` | `constexpr int[4]` | 上右下左 | 常量 | 朝向 $0$ 向上 |
| `inf` | `constexpr int` | $1145141919$ | 常量 | 不可达 |

### 4.2 算法步骤

1. 读地图（`cin` 逐字符，空格和换行都会被跳过）和 $N$ 条指令。
2. 起点、朝向向上的拒绝数为 $0$，其余为 `inf`。
3. 对每条指令：下一层清空为 `inf`；每个可达姿态先转移「拒绝」，再按指令尝试「接受」。
4. 输出最后一层的最小值。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

时间 $O(NM^2)$，空间 $O(M^2)$。

`g++ -O2 -std=c++17 -Wall -Wextra` 无警告。暴力是按指令深搜（拒绝或尝试执行，拒绝数不小于当前最优就剪枝），只用于 $N\le 12$。对拍含 Vijos 样例（双方输出 `1`）、$1\times 1$、只转弯、向前走出界等边界，以及 $60$ 组随机小迷宫。结果：

- `67 tests OK`
- 最大数据 $M=100,N=1000$，约 $800$ 个障碍，标程墙钟 `0.658671s`，输出 `6`

## 5. 示例代码
### 5.1 标程（`P1859.cpp`）

```cpp
//
// 题目：P1859 不听话的机器人
// https://www.luogu.com.cn/problem/P1859
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{105};
constexpr int mxn{1005};
constexpr int inf{1145141919};
constexpr int dx[4]{-1,0,1,0};
constexpr int dy[4]{0,1,0,-1};

int m,n,sx,sy;
char g[mxm][mxm];
int cmd[mxn];
int dp[2][mxm][mxm][4];

bool inside(int x,int y)
{
	return x>=1&&x<=m&&y>=1&&y<=m&&g[x][y]=='.';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n>>sx>>sy;
	for(int i=1;i<=m;++i)
		for(int j=1;j<=m;++j)
			cin>>g[i][j];
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		if(s=="FORWARD")
			cmd[i]=0;
		else if(s=="BACK")
			cmd[i]=1;
		else if(s=="LEFT")
			cmd[i]=2;
		else
			cmd[i]=3;
	}

	for(int x=1;x<=m;++x)
		for(int y=1;y<=m;++y)
			for(int d=0;d<4;++d)
				dp[0][x][y][d]=inf;
	dp[0][sx][sy][0]=0;

	int cur=0;
	for(int i=0;i<n;++i)
	{
		int nxt=cur^1;
		for(int x=1;x<=m;++x)
			for(int y=1;y<=m;++y)
				for(int d=0;d<4;++d)
					dp[nxt][x][y][d]=inf;
		for(int x=1;x<=m;++x)
			for(int y=1;y<=m;++y)
				for(int d=0;d<4;++d)
				{
					if(dp[cur][x][y][d]==inf)
						continue;
					int base=dp[cur][x][y][d];
					dp[nxt][x][y][d]=min(dp[nxt][x][y][d],base+1);
					if(cmd[i]==2)
					{
						int nd=(d+3)&3;
						dp[nxt][x][y][nd]=min(dp[nxt][x][y][nd],base);
					}
					else if(cmd[i]==3)
					{
						int nd=(d+1)&3;
						dp[nxt][x][y][nd]=min(dp[nxt][x][y][nd],base);
					}
					else
					{
						int sgn=cmd[i]==0?1:-1;
						int nx=x+sgn*dx[d];
						int ny=y+sgn*dy[d];
						if(inside(nx,ny))
							dp[nxt][nx][ny][d]=min(dp[nxt][nx][ny][d],base);
					}
				}
		cur=nxt;
	}

	int ans=inf;
	for(int x=1;x<=m;++x)
		for(int y=1;y<=m;++y)
			for(int d=0;d<4;++d)
				ans=min(ans,dp[cur][x][y][d]);
	cout<<ans<<'\n';
	return 0;
}
```
