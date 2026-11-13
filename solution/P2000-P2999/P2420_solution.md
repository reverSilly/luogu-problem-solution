# P2420 让我们异或吧 — 题解

## 一、解题过程：人类思考叙事

题目：一棵 $N$ 个结点的树，每条边带一个整数权值 $w$。有 $M$ 次询问，每次给两个点 $u,v$，问 $u$ 到 $v$ 的**唯一路径**上所有边权的**异或和**。数据范围 $N,M\le 10^5$，边权在 `int` 范围内（可能为负）。

**第一反应：每次询问现算路径。** 树上两点路径唯一，可以 DFS/BFS 找出路径再逐边异或。单次 $O(N)$，$M$ 次就是 $O(NM)=10^{10}$，必然超时。回头看 $40\%$ 子任务 $N,M\le 3000$，$O(NM)=9\times10^6$ 能过——这暗示正解要把"每次询问"的代价压到远低于 $O(N)$，最好 $O(1)$。

**能不能预处理，让每次询问 $O(1)$？** 异或和路径有关，路径又由两端点决定。联想到"树上路径求和"的经典套路：根 prefix。若是**边权求和**，我们会令 $s[u]$ 为根到 $u$ 的边权和，则 $\text{sum}(u,v)=s[u]+s[v]-2s[\text{lca}]$。异或有没有类似的东西？

**关键：异或的代数结构比加法更"干净"。** 异或满足 $x\oplus x=0$（自逆）、$x\oplus 0=x$、交换律、结合律。这意味着"重复的部分会自动抵消"，不需要像加法那样显式减去 $2s[\text{lca}]$。于是大胆猜想：

> 令 $d[u]$ 为**根到 $u$ 路径上边权的异或和**，则 $\text{xor}(u,v)=d[u]\oplus d[v]$。

**为什么？拿小例子验证。** 设根为 $1$，边 $1\text{-}2$ 权 $3$、$1\text{-}3$ 权 $5$、$2\text{-}4$ 权 $7$。则 $d[1]=0,\ d[2]=3,\ d[3]=5,\ d[4]=3\oplus7=4$。询问 $(2,3)$：路径 $2\to1\to3$，异或 $3\oplus5=6$；而 $d[2]\oplus d[3]=3\oplus5=6$，吻合。询问 $(1,4)$：路径 $1\to2\to4$，异或 $3\oplus7=4$；$d[1]\oplus d[4]=0\oplus4=4$，吻合。

**直觉解释（也是后面要证的引理）：** 设 $l=\text{lca}(u,v)$。根到 $u$ 的路径 = 根到 $l$ + $l$ 到 $u$；根到 $v$ 的路径 = 根到 $l$ + $l$ 到 $v$。两者都含"根到 $l$"这一段，异或时它出现两次，$x\oplus x=0$ 抵消；剩下的恰好是 $l\to u$ 与 $l\to v$ 两段，拼起来就是 $u\to v$ 的路径。**公共前缀自动消失**，这正是异或比加法省事的地方。

**剩下就是求所有 $d[u]$。** 从根做一次遍历，沿边 $u\to v$（权 $w$）时 $d[v]=d[u]\oplus w$，一次 $O(N)$ 即可。这里有个工程坑：$N$ 可达 $10^5$，若树退化成链，递归 DFS 深度 $10^5$ 可能爆栈，所以用**显式队列的 BFS**（迭代）来算 $d$，稳妥。

**边界自查：** $u=v$ 时路径为空，异或应为 $0$，公式给 $d[u]\oplus d[u]=0$，对；$N=1$ 时无边，$d[1]=0$，任何询问都是 $0$，对。负权也不影响——异或按位运算，`int` 补码表示下照样满足上述代数律。

至此算法成型：BFS 预处理 $d$，每次询问输出 $d[u]\oplus d[v]$，总 $O(N+M)$。

---

## 二、思维链：完整证明

**引理 1（异或的代数律）**：对任意整数 $a,b,c$，有
(i) 交换律 $a\oplus b=b\oplus a$；
(ii) 结合律 $(a\oplus b)\oplus c=a\oplus(b\oplus c)$；
(iii) 自逆 $a\oplus a=0$；
(iv) 单位元 $a\oplus 0=a$。

*证明*：异或按二进制位独立定义为"模 2 加法"：第 $k$ 位 $(a\oplus b)_k=a_k+b_k\pmod 2$。模 2 加法满足交换律、结合律，故 (i)(ii) 逐位成立。对 (iii)，每一位 $a_k+a_k=2a_k\equiv0\pmod2$，故 $a\oplus a$ 每位为 $0$，即 $0$。对 (iv)，$a_k+0=a_k$，故 $a\oplus0=a$。这些性质对负数同样成立，因为补码只是位的排列，逐位论证不依赖符号。证毕。

**引理 2（路径分解）**：把树以 $1$ 为根。对结点 $u$，记 $P(u)$ 为根到 $u$ 路径上的边集，$d[u]=\bigoplus_{e\in P(u)}w_e$。设 $l=\text{lca}(u,v)$，则
$$\bigoplus_{e\in\text{path}(u,v)}w_e=d[u]\oplus d[v].$$

*证明*：分三步。

**第一步（拆分 $d[u],d[v]$）**：因为 $l$ 是 $u,v$ 的最近公共祖先，根到 $u$ 的路径必经过 $l$，且可拆成边不相交的两段 $P(u)=P(l)\cup Q_u$，其中 $Q_u$ 是 $l$ 到 $u$ 的边集（$l=u$ 时 $Q_u=\varnothing$）。同理 $P(v)=P(l)\cup Q_v$。由引理 1 的结合律与交换律，
$$d[u]=\Big(\bigoplus_{e\in P(l)}w_e\Big)\oplus\Big(\bigoplus_{e\in Q_u}w_e\Big)=A\oplus B,$$
$$d[v]=A\oplus C,$$
其中 $A=\bigoplus_{e\in P(l)}w_e$，$B=\bigoplus_{e\in Q_u}w_e$，$C=\bigoplus_{e\in Q_v}w_e$。这里用到 $P(l)$ 与 $Q_u$ 边不相交，故异或可直接按段拆分（每条边恰被计一次）。

**第二步（消去公共前缀）**：
$$d[u]\oplus d[v]=(A\oplus B)\oplus(A\oplus C).$$
由结合律、交换律重排为 $(A\oplus A)\oplus(B\oplus C)$。由自逆 $A\oplus A=0$，再由单位元 $0\oplus(B\oplus C)=B\oplus C$。故 $d[u]\oplus d[v]=B\oplus C$。

**第三步（认出 $B\oplus C$ 就是路径异或）**：树上 $u$ 到 $v$ 的唯一路径经过 $l$，其边集为 $Q_u\cup Q_v$（边不相交：$Q_u$ 在 $l$ 的 $u$ 侧子树方向，$Q_v$ 在 $v$ 侧，二者仅在 $l$ 相接而无公共边）。异或与边的方向无关（异或的是边权这个数，不是有向量），故
$$\bigoplus_{e\in\text{path}(u,v)}w_e=\Big(\bigoplus_{e\in Q_u}w_e\Big)\oplus\Big(\bigoplus_{e\in Q_v}w_e\Big)=B\oplus C.$$
结合第二步即得 $\bigoplus_{e\in\text{path}(u,v)}w_e=d[u]\oplus d[v]$。证毕。

**推论（边界）**：当 $u=v$ 时 $l=u=v$，$Q_u=Q_v=\varnothing$，$B=C=0$，路径为空集，异或为 $0$；公式给 $d[u]\oplus d[u]=0$，一致。当 $N=1$ 时根即唯一结点，$P(1)=\varnothing$，$d[1]=0$，任何询问 $d[1]\oplus d[1]=0$，一致。

**引理 3（BFS 正确求出所有 $d$）**：从根 $1$ 出发做 BFS，沿边 $u\to v$（权 $w$）时令 $d[v]=d[u]\oplus w$，则遍历结束后每个 $d[v]$ 都等于 $\bigoplus_{e\in P(v)}w_e$。

*证明*：对结点深度 $\text{dep}(v)$ 归纳。根 $\text{dep}=0$，$P(1)=\varnothing$，初始化 $d[1]=0$，成立。设深度 $<k$ 的结点都已正确。深度为 $k$ 的结点 $v$ 在 BFS 中恰被其父 $u$（深度 $k-1$）第一次访问时赋值（树中每点只有一个父，BFS 用 `vis` 保证只赋一次）。此时 $P(v)=P(u)\cup\{(u,v)\}$，故
$$\bigoplus_{e\in P(v)}w_e=\Big(\bigoplus_{e\in P(u)}w_e\Big)\oplus w=d[u]\oplus w,$$
最后一步用归纳假设 $d[u]=\bigoplus_{e\in P(u)}w_e$。算法正是令 $d[v]=d[u]\oplus w$，故 $d[v]$ 正确。BFS 访问所有结点（树连通），故全部 $d$ 正确。证毕。

**引理 4（复杂度）**：算法时间 $O(N+M)$，空间 $O(N)$。

*证明*：建图读入 $N-1$ 条边 $O(N)$；BFS 每点入队出队各一次、每条边被考察两次，$O(N)$；$M$ 次询问每次 $O(1)$ 异或，$O(M)$。合计 $O(N+M)$。存邻接表 $O(N)$、$d/\text{vis}/Q$ 各 $O(N)$，空间 $O(N)$。证毕。

**定理**：算法对每个询问输出正确的路径异或和。

*证明*：由引理 3，预处理得到的 $d$ 全部正确；由引理 2，$d[u]\oplus d[v]$ 恰为所求路径异或；边界由推论覆盖。故每次输出正确。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **explore its property** — 异或满足 $x\oplus x=0$、交换/结合律，"重复段自动抵消"，比加法少一个 $-2s[\text{lca}]$ 项。
2. **consider easier version** — 先想"边权求和"的根 prefix 套路 $s[u]+s[v]-2s[\text{lca}]$，再把加法换成异或。
3. **classic problem solution** — 树上根 prefix：$d[u]=$ 根到 $u$ 的边权异或，一次遍历 $O(N)$ 预处理。
4. **reduce to easier problem** — 路径异或化为 $d[u]\oplus d[v]$，公共前缀被自逆律消去，询问 $O(1)$。
5. **explore its property** — 用 BFS（迭代）而非递归 DFS 求 $d$，规避 $N=10^5$ 链状树的爆栈风险。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 | 初值 / 记忆点 |
|------|------|------|----------------|
| $n,m$ | 结点数、询问数 | `int` | 输入 |
| `graph[u]` | 邻接表，存 `{v,w}` | `vector<pair<int,int>>[]` | 无向边双向插入 |
| $d[u]$ | 根到 $u$ 路径边权异或和 | `int[]` | 全局默认 0，BFS 中赋值；$d[\text{root}]=0$ |
| `vis[u]` | BFS 是否已访问 | `bool[]` | 全局默认 false |
| `Q` | BFS 队列 | `int[]` | 数组模拟，`hd/tl` 双指针 |
| $u,v$ | 询问两端点（读入 1-based，内部 `--` 转 0-based） | `int` | 每次询问 |

### 算法步骤

1. I/O 加速（`cin.tie(nullptr);cout.tie(nullptr);ios::sync_with_stdio(false);`）。
2. 读 $n$，读 $n-1$ 条边 `u v w`，`--u;--v` 转 0-based，双向存入 `graph`。
3. BFS：根 $0$ 入队，`vis[0]=1,d[0]=0`；出队 $u$，对每条邻边 `(v,w)`，若 `!vis[v]` 则 `vis[v]=1,d[v]=d[u]^w,v` 入队。
4. 读 $m$，对每个询问 `u v`（`--u;--v`），输出 `d[u]^d[v]`。

### 复杂度

- 时间 $O(N+M)$；空间 $O(N)$。

### 验证状态

- 编译：`g++ -std=c++20 -O2` 通过。
- 题面无样例，手造 2 组：① 4 点树（边权 3,5,7）询问 `(1,4),(2,3),(4,4)` → `4,6,0`；② $N=1$ 询问 `(1,1)` 两次 → `0,0`。均与手算一致。
- 对拍：暴力（每次询问从 $u$ 做 BFS 重建到 $v$ 的路径再逐边异或，独立于根 prefix 法）与标程在 **300 组**随机数据（$N\le8$、边权含负数 $[-5,15]$、随机询问）上比对，全部一致。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2420 让我们异或吧
// 思路：d[u]=根到 u 的边权异或和；path(u,v) 的异或 = d[u]^d[v]
//       公共根->lca 段因 x^x=0 自动抵消。BFS 预处理 d，询问 O(1)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m,d[mxn];
vector<pair<int,int>>graph[mxn];
int Q[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,u,v,w;i<n;++i)
	{
		cin>>u>>v>>w;--u;--v;
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}
	// BFS from root 0, d[u]=xor of edge weights on root->u path
	int hd=0,tl=0;
	Q[tl++]=0;vis[0]=1;d[0]=0;
	while(hd<tl)
	{
		int u=Q[hd++];
		for(auto[v,w]:graph[u])
			if(!vis[v])
			{
				vis[v]=1;
				d[v]=d[u]^w;
				Q[tl++]=v;
			}
	}
	cin>>m;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		cout<<(d[u]^d[v])<<'\n';
	}
	return 0;
}
```

### 代码说明

- `d` 是全局 `int` 数组，默认 0；根 `d[0]=0` 显式置好，其余在 BFS 中赋值。树连通，BFS 必访问所有点，故不会有"漏赋值的 0"被误用。
- 用**数组队列 BFS**而非递归 DFS：$N=10^5$ 的链状树会让递归深度达 $10^5$，在部分评测栈限制下爆栈；迭代 BFS 无此风险。
- `for(auto[v,w]:graph[u])` 用结构化绑定取邻点与边权，`!vis[v]` 保证每点只赋一次 $d$。
- 询问内部 `--u;--v` 转 0-based，与建图一致；输出 `d[u]^d[v]`，换行 `'\n'`。
- 边权可为负，`int` 异或按补码逐位运算，引理 1 的代数律对负数同样成立，无需特判。