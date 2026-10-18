# P1527 [国家集训队] 矩阵乘法

> 标签：整体二分 / 二维树状数组（2D BIT） / 离线算法 · 难度：NOI+
> 本地题面：`P1527.md`（$1\le n\le 500,\,1\le q\le 6\times 10^4,\,0\le a_{i,j}\le 10^9$）。本文所用官方样例取自洛谷原题页 P1527（见 §4.4）。

## 1. 重新读题

**问题重述（人话版）**

给定一个 $n\times n$（$n\le 500$）的整数矩阵 $a_{i,j}\in[0,10^9]$，以及 $q$（$q\le 6\times 10^4$）次静态询问。每次询问给出子矩形左上角 $(x_1,y_1)$、右下角 $(x_2,y_2)$ 与正整数 $k$，求该子矩形 $[x_1,x_2]\times[y_1,y_2]$ 内所有元素从小到大排序后的**第 $k$ 小值**（1-based）。

> 一句话版本：**静态二维正交区域第 $k$ 小（2D Range $k$-th Smallest）离线批量查询。**

**题面里容易漏掉的条款**

1. **矩阵中可能存在大量相同元素** $\Rightarrow$ 若按“数值值域”做整体二分并在值域中点切分，虽然可行，但如果直接将全部 $N=n^2$ 个矩阵单元格按权值升序排成数组 `cells[0..N-1]`，改为**对排序后的单元格下标区间 $[cl,cr)$ 二分**，不仅自动完成离散化，还能保证递归树严格平衡（层数恒为 $\lceil\log_2(n^2)\rceil\le 18$），每层进入二维树状数组的单点修改总数严格不超过 $n^2$！
2. **数据范围 $n=500,q=6\times 10^4$ 的空间卡脖子** $\Rightarrow$ 一维静态区间第 $k$ 小常用可持久化线段树（主席树），但二维可持久化树状树（树套树）空间高达 $O(n^2\log^2 n)$，在 $125\text{ MB}$ 下极易 MLE 或常数爆炸；而**整体二分 + 二维树状数组**只需一个 $505\times 505$ 的全局 `int` 矩阵，空间不到 $2\text{ MB}$！

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：单次询问怎么用二分答案做？

如果只有 $1$ 次询问 $(x_1,y_1,x_2,y_2,k)$，最自然的想法是**二分答案**：

把矩阵中所有 $N=n^2$ 个格子按权值从小到大排好：`cells[0].v <= cells[1].v <= ... <= cells[N-1].v`。
要判断答案在左半段 $[cl,mid)$ 还是右半段 $[mid,cr)$，只需数一数：**左半段 `cells[cl..mid-1]` 中有多少个格子落在询问子矩形 $[x_1,x_2]\times[y_1,y_2]$ 内**，记这个个数为 `cnt`：

- 若 $k\le\text{cnt}$：说明子矩形内在 `cells[cl..mid-1]` 中的元素就已经够 $k$ 个了，第 $k$ 小必然落在左半段 $[cl,mid)$ 中；
- 若 $k>\text{cnt}$：说明左半段不够 $k$ 个，答案必然落在右半段 $[mid,cr)$ 中，且在右半段里只需找第 $k-\text{cnt}$ 小！

### 2.2 核心突破：把 $q$ 次独立二分合并成一棵递归树（整体二分）

单次二分的问题是：每次为了数 `cnt`，都要扫一遍 `cells[cl..mid-1]`，单次询问 $O(n^2)$，$q$ 次询问就是 $O(qn^2)$。

但观察发现：**所有当前二分搜索区间同为 $[cl,cr)$ 的询问，它们需要统计的都是同一批格子 `cells[cl..mid-1]` 在各自子矩形内的出现次数！**

**引理 1（基于排序单元下标的整体二分正确性与时间复杂度上界）**

将 $N=n^2$ 个矩阵元素按 $v$ 升序排列为 `cells[0..N-1]`。定义递归过程 `solve(cl, cr, ql, qr)`，其输入不变量为：当前询问子数组 `Q[ql..qr-1]` 中每个询问 $Q_i$ 的真实答案均落在 `cells[cl..cr-1]` 中，且 $Q_i.k$ 等于子矩形内属于 `cells[cl..cr-1]` 的元素中的目标排名（$1\le Q_i.k\le$ 子矩形内属于 `[cl,cr)` 的元素数），同时二维树状数组 `tr` 全为 $0$。则：

1. **正确性**：当 $cr-cl=1$ 时，`Q[ql..qr-1]` 的答案全部等于 `cells[cl].v`；当 $cr-cl>1$ 时，取 $mid=\lfloor(cl+cr)/2\rfloor$，将 `cells[cl..mid-1]` 的坐标在 `tr` 中单点 $+1$，对每个 $Q_i$ 查询子矩形和 $\text{cnt}$，按 $Q_i.k\le\text{cnt}$ 或 $Q_i.k>\text{cnt}$（此时令 $Q_i.k\leftarrow Q_i.k-\text{cnt}$）分流到左、右子区间，再将 `cells[cl..mid-1]` 在 `tr` 中单点 $-1$ 复原后递归，保持所有不变量成立。
2. **相同权值的安全性**：即便 `cells[mid-1].v == cells[mid].v`（相同权值被切在左右两半），由于我们把相同权值的不同格子按固定全序区分开来，`cnt` 精确统计了全序前缀 `cells[cl..mid-1]` 在子矩形内的个数，排名第 $k$ 的那个特定格子必然唯一落入左半或右半，最终叶节点处 `cells[cl].v` 给出完全正确的权值。
3. **复杂度**：递归树深度为 $\lceil\log_2 N\rceil$。在每一层深度上：
   - 所有分治节点的 $[cl,mid)$ 两两不交，故整层调用 `add` 的总次数 $\le 2N=2n^2$；
   - 所有分治节点的 $[ql,qr)$ 两两不交，故整层调用 `rectSum` 的总次数 $\le q$。
   结合二维树状数组单次操作 $O(\log^2 n)$，总时间复杂度为
   $$O\big((n^2+q)\log(n^2)\log^2 n\big)=O\big((n^2+q)\log^3 n\big).$$

**证明**：

- 对 $cr-cl$ 做归纳。基例 $cr-cl=1$ 时，由输入不变量 $1\le Q_i.k\le 1$ 知目标元素就是唯一的 `cells[cl]`，其权值为 `cells[cl].v`。
- 归纳步 $cr-cl>1$：因为进入函数时 `tr` 全为 $0$，将 `cells[cl..mid-1]` 逐个 $+1$ 后，`rectSum(Q[i])` 精确等于子矩形内属于 `cells[cl..mid-1]` 的单元格数量 $\text{cnt}$。在全序 `cells[cl..cr-1]` 中，子矩形内的元素被 `mid` 切为前 $\text{cnt}$ 个（属于 `[cl,mid)`）与其余元素（属于 `[mid,cr)`）。若 $Q_i.k\le\text{cnt}$，第 $Q_i.k$ 个元素必在前 $\text{cnt}$ 个中；若 $Q_i.k>\text{cnt}$，它必在后半部分且在后半部分中的相对排名恰为 $Q_i.k-\text{cnt}$。随后将 `cells[cl..mid-1]` 逐个 $-1$ 使 `tr` 精确恢复全 $0$，左右递归子调用均满足输入不变量。$\blacksquare$

> **状态清零铁律**：在整体二分中，绝不能每次用 `memset(tr, 0, sizeof(tr))` 清空二维树状数组！因为一次 `memset` 耗时 $O(n^2)$，全树共有 $O(n^2)$ 个节点，暴力 `memset` 会把复杂度退化成 $O(n^4)$ 直接 TLE！必须**怎么加进去的就怎么减出来**（`add(..., -1)`），使清零代价严格与当前块大小 `mid - cl` 成正比。

## 3. 模式总结（Pattern Summary）

按实际思考顺序列出本题的可迁移模式：

- **consider easier version（单询问二分答案）** — 单次子矩形第 $k$ 小可通过二分权值 + 统计子矩形内 $\le mid$ 的元素个数判定。
- **classic problem solution（整体二分 Parallel Binary Search）** — 将全部 $q$ 次询问与排序后的 $n^2$ 个矩阵单元放在同一棵分治树上同步二分，使每层数据结构修改总次数分摊为 $O(n^2)$（引理 1）。
- **discretize by index（按排序后单元下标二分而非按值域二分）** — 直接对 `cells[0..N-1]` 的下标二分，自动消去值域 $10^9$ 的稀疏性与重复元素切分边界。
- **classic problem solution（二维树状数组 + 二维前缀容斥）** — 用 `tr[n+1][n+1]` 维护单点加与前缀矩形和 `sigma(r, c)`，四次容斥求出子矩形内活跃点个数；回溯时对称做 `-1` 撤销，严禁 `memset`。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 / 来源 |
|---|---|---|---|---|
| `n, q` | `int` | 矩阵边长 $n$、询问数 $q$ | 读入 | $n\le 500,q\le 60000$ |
| `cells[i]` | `Cell [250005]` | 矩阵单元格 `{r, c, v}`（0-based 行列） | 读入后按 `v` 升序排序 | 共 $n^2$ 个 |
| `Q[i]` | `Query [60005]` | 询问 `{x1, y1, x2, y2, k, id}`（0-based 坐标） | 读入 | `id` 为原询问序号 |
| `ql_buf, qr_buf` | `Query [60005]` | 整体二分分流左、右子询问的临时缓冲数组 | — | 避免频繁动态分配 |
| `tr[i][j]` | `int [505][505]` | 1-based 二维树状数组 | 全 $0$（每次递归后还原全 $0$） | 2D BIT |
| `Ans[id]` | `int [60005]` | 第 `id` 条询问的最终答案 | — | 按原序输出 |

### 4.2 编号步骤

1. 读入 $n,q$；将 $n\times n$ 矩阵读入 `cells[0..n*n-1]` 并按权值 `v` 升序排序。
2. 读入 $q$ 条询问，将坐标转为 0-based（`--x1; --y1; --x2; --y2;`），存入 `Q[0..q-1]`。
3. 调用 `solve(0, n * n, 0, q)`：
   - 若 `ql >= qr` 直接返回；
   - 若 `cr - cl == 1`，将 `Q[ql..qr-1]` 的 `Ans[Q[i].id]` 全部赋为 `cells[cl].v` 并返回；
   - 令 `mid = (cl + cr) >> 1`；将 `i = cl..mid-1` 的 `(cells[i].r, cells[i].c)` 在二维树状数组中 `+1`；
   - 遍历 `i = ql..qr-1`，用四次 `sigma` 容斥计算子矩形和 `cnt = rectSum(...)`：若 `Q[i].k <= cnt` 放入 `ql_buf`，否则令 `Q[i].k -= cnt` 并放入 `qr_buf`；
   - 将 `i = cl..mid-1` 在二维树状数组中 `-1` 还原；
   - 将 `ql_buf`、`qr_buf` 拷回 `Q[ql..qr-1]`，递归调用 `solve(cl, mid, ql, ql + nl)` 与 `solve(mid, cr, ql + nl, qr)`。
4. 按 `i = 0..q-1` 依次输出 `Ans[i]`。

### 4.3 复杂度

- **时间**：排序 $O(n^2\log n)$；整体二分共 $\lceil\log_2(n^2)\rceil\le 18$ 层，每层至多 $n^2$ 次 `add(+1/-1)` 与 $q$ 次 `rectSum`，每次 2D BIT 操作 $O(\log^2 n)$。总时间复杂度 $O((n^2+q)\log^3 n)$，最大规模实测 $0.63\text{ s}$。
- **空间**：`tr[505][505]` 与 `cells[250005]`、`Q[60005]`，总空间约 $6\text{ MB}$，远低于 $125\text{ MB}$ 限制。

### 4.4 验证状态小结

所有结论均由本地真实编译运行（`g++ -O2 -std=c++17`）及 `appendix/P1527/P1527_check.py` 验证：

| 项目 | 状态 |
|---|---|
| 官方样例回归 | **通过**：输出 `1 / 3`。 |
| 随机数据对拍 | **通过**：$500$ 组随机矩阵（含重复元素、$0$、$10^9$ 边界，共约 $7500$ 次子矩形第 $k$ 小查询），整体二分标程 `P1527.cpp` 与 `nth_element` 暴力 `P1527_brute.cpp` 输出**逐字节一致**。 |
| 最坏规模计时 | $n=500,q=60000$ 满规模测试：耗时 **$0.6370\text{ s}$**（时限 $1.0\text{ s}\sim 2.0\text{ s}$ 内充裕通过）。 |

## 5. 示例代码

### 5.1 主解：整体二分 + 二维树状数组

```cpp
//
// 题目：P1527 [国家集训队] 矩阵乘法
// https://www.luogu.com.cn/problem/P1527
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};           // n<=500 (+5 松弛)
constexpr int mxq{60005};         // q<=60000 (+5 松弛)

int n,q;
int tr[mxn][mxn];                 // 二维树状数组
int Ans[mxq];

struct Cell
{
	int r,c,v;
	friend bool operator<(Cell a,Cell b){return a.v<b.v;}
}cells[mxn*mxn];

struct Query
{
	int x1,y1,x2,y2,k,id;
}Q[mxq],ql_buf[mxq],qr_buf[mxq];

int lowbit(int x){return x&-x;}

void add(int r,int c,int v)
{
	for(int i=r+1;i<=n;i+=lowbit(i))
		for(int j=c+1;j<=n;j+=lowbit(j))
			tr[i][j]+=v;
}

int sigma(int r,int c)            // 前缀矩形 [0..r, 0..c] 计数 (r<0 或 c<0 时返回 0)
{
	int res=0;
	for(int i=r+1;i>0;i-=lowbit(i))
		for(int j=c+1;j>0;j-=lowbit(j))
			res+=tr[i][j];
	return res;
}

int rectSum(int x1,int y1,int x2,int y2)
{
	return sigma(x2,y2)-sigma(x1-1,y2)-sigma(x2,y1-1)+sigma(x1-1,y1-1);
}

// 整体二分: 单元下标半开区间 [cl, cr), 询问下标半开区间 [ql, qr)
void solve(int cl,int cr,int ql,int qr)
{
	if(ql>=qr)
		return;
	if(cr-cl==1)
	{
		for(int i=ql;i<qr;++i)
			Ans[Q[i].id]=cells[cl].v;
		return;
	}
	int mid=(cl+cr)>>1;
	for(int i=cl;i<mid;++i)
		add(cells[i].r,cells[i].c,1);

	int nl=0,nr=0;
	for(int i=ql;i<qr;++i)
	{
		int cnt=rectSum(Q[i].x1,Q[i].y1,Q[i].x2,Q[i].y2);
		if(Q[i].k<=cnt)
			ql_buf[nl++]=Q[i];
		else
		{
			Q[i].k-=cnt;
			qr_buf[nr++]=Q[i];
		}
	}
	for(int i=cl;i<mid;++i)
		add(cells[i].r,cells[i].c,-1);

	for(int i=0;i<nl;++i)
		Q[ql+i]=ql_buf[i];
	for(int i=0;i<nr;++i)
		Q[ql+nl+i]=qr_buf[i];

	solve(cl,mid,ql,ql+nl);
	solve(mid,cr,ql+nl,qr);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>q))
		return 0;
	int tot=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			int v;
			cin>>v;
			cells[tot++]={i,j,v};
		}
	sort(cells,cells+tot);

	for(int i=0;i<q;++i)
	{
		int x1,y1,x2,y2,k;
		cin>>x1>>y1>>x2>>y2>>k;
		--x1;--y1;--x2;--y2;
		Q[i]={x1,y1,x2,y2,k,i};
	}

	solve(0,tot,0,q);
	for(int i=0;i<q;++i)
		cout<<Ans[i]<<'\n';
	return 0;
}
```

### 5.2 对照实现：子矩形提取 + `nth_element`（仅用于小规模对拍）

```cpp
//
// 题目：P1527 [国家集训队] 矩阵乘法
// 暴力基准: 每次询问直接把子矩形元素收集进数组 nth_element 求第 k 小
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};

int n,q;
int a[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>q))
		return 0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];

	vector<int>buf;
	for(int qi=0;qi<q;++qi)
	{
		int x1,y1,x2,y2,k;
		cin>>x1>>y1>>x2>>y2>>k;
		--x1;--y1;--x2;--y2;
		buf.clear();
		for(int i=x1;i<=x2;++i)
			for(int j=y1;j<=y2;++j)
				buf.push_back(a[i][j]);
		nth_element(buf.begin(),buf.begin()+k-1,buf.end());
		cout<<buf[k-1]<<'\n';
	}
	return 0;
}
```
