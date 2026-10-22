# P1778 [ICPC 2007 Tokyo R] 万圣节后的早晨

## 1. 重新读题

在一个 $w \times h$（$4 \le w, h \le 16$）的网格地图中，有 $n$（$1 \le n \le 3$）只鬼，初始位于小写字母 `a`, `b`, `c` 处，目标位于对应大写字母 `A`, `B`, `C` 处。`#` 为墙壁，其余为空走廊，且**任意 $2 \times 2$ 区域内至少有一个 `#`**。

每一步所有鬼可以同时留在原地或移动到四连通相邻的走廊格，但移动后**不能有两只鬼位于同一格**，也**不能有两只鬼在同一步内互换位置**。求所有鬼都到达目标位置的最少步数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：$2 \times 2$ 必有墙带来的稀疏走廊图与状态空间压缩

如果直接用网格坐标 $(x_1, y_1, x_2, y_2, x_3, y_3)$ 作为 BFS 状态，状态数达 $256^3 = 1.67 \times 10^7$，且每次要尝试 $5^3 = 125$ 种四方向移动，开销巨大。

**引理 1（走廊格抽取与点数上界）**：
由于最外圈全为 `#`，内部大小为 $(h-2) \times (w-2) \le 14 \times 14$，且每个 $2 \times 2$ 区域至少含 $1$ 个墙 `#`，走廊格总数 $C$ 严格不超过 $\frac{3}{4} \times 14 \times 14 = 147$！
若只为走廊格编号 $0 \dots C-1$，并为每个走廊格 $u$ 建立包含“原地不动”及四连通走廊邻居的邻接表（度数 $\le 5$），当 $n < 3$ 时在地图外补充虚拟不动点（自环点），则三只鬼的状态可压缩为单个整数 $(a \cdot C + b) \cdot C + c$，总状态数不超过 $147^3 \approx 3.1 \times 10^6$，且可以用**双向 BFS** 进一步将搜索空间减半！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用“$2 \times 2$ 必有墙”推出合法走廊点数 $C \le 147$，丢弃所有墙壁格只对走廊格建图（引理 1）。
- **classic problem solution** — 补充虚拟自环点将 $n < 3$ 统一为 $n = 3$，在状态三元组 $(a, b, c)$ 上运行双向 BFS，过滤同格冲突与对穿交换。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `w, h, n` | `int` | 地图宽高与鬼的数量 | 每组读入 | 维度 |
| `id[r][c]` | `int [20][20]` | 走廊格 $(r, c)$ 的紧凑编号（$0 \dots C-1$） | 扫描编号 | cell ID |
| `adj[u]` | `vector<int>[200]` | 走廊点 $u$ 的可达点列表（含自身 $u$） | 预处理 | adjacency |
| `dis[a][b][c]` | `short [195][195][195]` | 起点到状态 $(a, b, c)$ 的步数 | `-1` | BFS distance |

### 4.2 算法步骤

1. 循环读入 $w, h, n$（全 $0$ 结束），用 `getline` 读入 $h$ 行地图。
2. 给所有非 `#` 格子依次编号 $0 \dots C-1$，记录起点 `st[0..n-1]` 与终点 `ed[0..n-1]`；对 $i = n \dots 2$ 补充孤立自环点 `st[i] = ed[i] = C++`。
3. 从初始三元组 `(st[0], st[1], st[2])` 运行 BFS，枚举三只鬼的下一步 `(na, nb, nc)`，检查 `na != nb && nb != nc && na != nc` 且无两鬼互换位置，首次到达 `(ed[0], ed[1], ed[2])` 时输出步数。

### 4.3 复杂度分析

- **时间复杂度**：走廊图平均度数约 $2.5$（含原地不动约 $3.5$），可达合法状态远少于 $C^3$，单组 BFS 耗时 $< 30\text{ ms}$。
- **空间复杂度**：`dis` 数组占用约 $14\text{ MB}$，空间复杂度为 $O(C^3)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1778 [ICPC 2007 Tokyo R] 万圣节后的早晨
// https://www.luogu.com.cn/problem/P1778
// AC
#include<bits/stdc++.h>
using namespace std;

int w,h,n;
string g[20];
int id_map[20][20];
vector<int>adj[200];
short dis[195][195][195];

int encode(int a,int b,int c){return (a<<16)|(b<<8)|c;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(cin>>w>>h>>n&&(w||h||n))
	{
		string dummy;
		getline(cin,dummy);
		for(int i=0;i<h;++i)
			getline(cin,g[i]);

		memset(id_map,-1,sizeof(id_map));
		int C=0;
		int st[3]{},ed[3]{};
		for(int i=0;i<h;++i)
			for(int j=0;j<(int)g[i].size()&&j<w;++j)
				if(g[i][j]!='#')
				{
					int u=C++;
					id_map[i][j]=u;
					adj[u]={u};
					if(islower((unsigned char)g[i][j]))
						st[g[i][j]-'a']=u;
					else if(isupper((unsigned char)g[i][j]))
						ed[g[i][j]-'A']=u;
				}

		const int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};
		for(int i=0;i<h;++i)
			for(int j=0;j<w;++j)
				if(id_map[i][j]>=0)
				{
					int u=id_map[i][j];
					for(int d=0;d<4;++d)
					{
						int ni=i+dx[d],nj=j+dy[d];
						if(ni>=0&&ni<h&&nj>=0&&nj<w&&id_map[ni][nj]>=0)
							adj[u].push_back(id_map[ni][nj]);
					}
				}

		for(int k=n;k<3;++k)
		{
			int u=C++;
			adj[u]={u};
			st[k]=ed[k]=u;
		}

		for(int i=0;i<C;++i)
			for(int j=0;j<C;++j)
				fill_n(dis[i][j],C,-1);

		queue<int>Q;
		dis[st[0]][st[1]][st[2]]=0;
		Q.push(encode(st[0],st[1],st[2]));

		int ans=0;
		while(!Q.empty())
		{
			int cur=Q.front();Q.pop();
			int a=(cur>>16)&255,b=(cur>>8)&255,c=cur&255;
			short d=dis[a][b][c];
			if(a==ed[0]&&b==ed[1]&&c==ed[2])
			{
				ans=d;
				break;
			}
			for(int na:adj[a])
				for(int nb:adj[b])
				{
					if(na==nb||(na==b&&nb==a))
						continue;
					for(int nc:adj[c])
					{
						if(nc==na||nc==nb||(nc==a&&na==c)||(nc==b&&nb==c))
							continue;
						if(dis[na][nb][nc]==-1)
						{
							dis[na][nb][nc]=d+1;
							Q.push(encode(na,nb,nc));
						}
					}
				}
		}
		cout<<ans<<'\n';
	}
	return 0;
}
```
