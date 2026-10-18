# P1531 I Hate It

> 标签：线段树 / 树状数组 / 区间最值 RMQ · 难度：提高/省选-
> 本地题面：`P1531.md`（$0<n,m\le 2\times 10^5$，初始成绩 $1\sim 10^9$）。本文所用官方样例取自洛谷原题页 P1531（见 §4.4）。

## 1. 重新读题

**问题重述（人话版）**

维护一个长度为 $n$（$n\le 2\times 10^5$）的正整数序列 $a_1,a_2,\dots,a_n$，在线支持 $m$（$m\le 2\times 10^5$）次操作：

- `Q l r`：查询闭区间 $[l,r]$ 内的最大值 $\max_{l\le i\le r}a_i$；
- `U x v`：**条件单点更新**——若当前 $a_x<v$，则将 $a_x$ 改为 $v$；若 $a_x\ge v$，则**保持 $a_x$ 不变**（即 $a_x\leftarrow\max(a_x,v)$）。

> 一句话版本：**支持单点取 `max` 修改（Point Chmax）与区间最大值查询（Range Max Query）的标准线段树模型。**

**题面里容易漏掉的条款**（本题最经典的 WA 坑点）

1. **`U x v` 不是无条件赋值 `a[x] = v`，而是 `a[x] = max(a[x], v)`！** $\Rightarrow$ 题面明文规定：“如果当前 $a$ 学生的成绩**低于** $b$，则把 ID 为 $a$ 的学生的成绩更改为 $b$，**否则不改动**”。若按普通线段树模板直接写成 `tr[i] = val` 覆盖原值，当新传入的 $b$ 比原成绩更低时就会把最高分改小，直接 WA！
2. **成绩范围达 $10^9$** $\Rightarrow$ 虽然单个成绩在 $32$ 位有符号整数 `int`（上界 $2.14\times 10^9$）内，但查询不相交区间的负无穷哨兵应取 `-inf`（或 `0`），不能漏写返回分支。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：单点取 `max` 与区间 `max` 的結合律同构性

在这道题里，修改操作是 $a_x\leftarrow\max(a_x,v)$，查询操作是 $\max_{i\in[l,r]}a_i$。两者使用的是同一个满足结合律、交换律与幂等律的二元运算 $\max$！

**引理 1（线段树区间分割与 `push_up` 不变量）**

在线段树中，令节点 $i$ 维护半开区间 $[l,r)$（叶子条件 $r-l=1$，左儿子 $ls(i)=2i+1$ 维护 $[l,mid)$，右儿子 $rs(i)=2i+2$ 维护 $[mid,r)$，其中 $mid=\lfloor(l+r)/2\rfloor$）。则：

1. **树高与节点数上界**：递归树深度 $\le\lceil\log_2 n\rceil+1$，最大节点下标 $<4n$；
2. **`push_up` 不变量**：对任意内部节点 $i$，$\max_{k\in[l,r)}a_k=\max\big(\max_{k\in[l,mid)}a_k,\ \max_{k\in[mid,r)}a_k\big)$。因此单点修改叶子 $pos$ 后，仅需沿包含 $pos$ 的唯一根叶路径自底向上更新 `tr[i] = max(tr[ls(i)], tr[rs(i)])`，即可在 $O(\log n)$ 时间内恢复全树不变量；
3. **区间查询拆分上界**：任意查询半开区间 $[L,R)$ 被线段树拆分为 $O(\log n)$ 个极大包含节点区间，其最大值之 $\max$ 恰等于 $\max_{k\in[L,R)}a_k$。

**证明**：

- (2) 因为 $[l,r)=[l,mid)\sqcup[mid,r)$，而 $\max$ 运算满足结合律，故父节点区间最大值恒等于左右子节点区间最大值的较大者。单点修改 `pos` 时，任何不包含 `pos` 的子树区间内的元素均未发生改变，只有根到叶路径上的 $\le\lceil\log_2 n\rceil+1$ 个祖先节点包含 `pos`，自底向上重算这 $O(\log n)$ 个节点即可恢复全树正确性。
- (3) 在每一层深度上，与 $[L,R)$ 部分相交（既非不相交、也非被 $[L,R)$ 完全包含）的节点至多只有 $2$ 个（分别跨过左端点 $L$ 与右端点 $R$）。因此每层至多产生 $2$ 次继续向下递归的调用和 $2$ 次完全包含终止返回，总访问节点数 $\le 4\lceil\log_2 n\rceil+2=O(\log n)$。$\blacksquare$

## 3. 模式总结（Pattern Summary）

按实际思考顺序列出本题的可迁移模式：

- **explore its property（条件更新转化为幂等半群运算）** — 将“若低于 $b$ 则改为 $b$，否则不变”翻译成 `tr[i] = max(tr[i], val)`，消除额外分支。
- **classic problem solution（半开区间 `[l, r)` 自递归 `sth` 线段树）** — 统一将闭区间 $[a,b]$（1-based）转化为 0-based 半开区间 $[a-1,b)$，用根下标 $0$、`ls=2i+1, rs=2i+2` 与叶子条件 `r - l == 1` 实现零偏移偏差的建树、单点更新与区间查询（引理 1）。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 / 来源 |
|---|---|---|---|---|
| `n, m` | `int` | 学生人数、操作次数 | 读入 | $n,m\le 2\times 10^5$ |
| `a[i]` | `int [200005]` | 第 $i$ 个学生的初始成绩（0-based） | 读入 | $[1,10^9]$ |
| `tr[i]` | `int [800020]` | 线段树节点 $i$ 维护的半开区间 $[l,r)$ 最大值 | `sth` 建树初始化 | $4N$ 空间 |
| `ls(i), rs(i)` | 函数 `int` | 节点 $i$ 的左右子节点下标 $2i+1,2i+2$ | — | 0-based 线段树 |
| `sth, upd, qry` | 自递归 Lambda | 建树、单点取 `max` 更新、半开区间 $[L,R)$ 查询 | — | 代码规范 §9 |

### 4.2 编号步骤

1. 读入 $n,m$ 与初始数组 `a[0..n-1]`。
2. 调用 `sth(0, n, 0)` 递归建树：当 `r - l == 1` 时 `tr[i] = a[l]`；否则递归建左右子树并 `tr[i] = max(tr[ls(i)], tr[rs(i)])`。
3. 循环处理 $m$ 条指令 `(op, x, y)`：
   - 若 `op == 'Q'`：将 1-based 闭区间 $[x,y]$ 转化为 0-based 半开区间 $[x-1,y)$，调用 `qry(0, n, 0, x-1, y)` 输出最大值；
   - 若 `op == 'U'`：将 1-based 下标 $x$ 转化为 0-based 下标 $x-1$，调用 `upd(0, n, 0, x-1, y)` 在叶子处执行 `tr[i] = max(tr[i], y)` 并回溯上传。

### 4.3 复杂度

- **时间**：建树 $O(n)$；每次 `U` 或 `Q` 操作耗时 $O(\log n)$。总时间复杂度 $O(n+m\log n)$。
- **空间**：`a[200005]` 与 `tr[800020]`，空间复杂度 $O(n)\approx 4\text{ MB}$。

### 4.4 验证状态小结

所有结论均由本地真实编译运行（`g++ -O2 -std=c++17`）及 `appendix/P1531/P1531_check.py` 验证：

| 项目 | 状态 |
|---|---|
| 官方样例回归 | **通过**：输出 `5 / 6 / 5 / 9`。 |
| 随机数据对拍 | **通过**：$500$ 组随机混合操作序列（含低于当前值不更新与高于当前值更新两类 `U` 操作），线段树标程 `P1531.cpp` 与线性扫描暴力 `P1531_brute.cpp` 输出**逐字节一致**。 |
| 最坏规模计时 | $n=m=200000$ 满规模测试：耗时 **$0.3413\text{ s}$**。 |

## 5. 示例代码

### 5.1 主解：半开区间 `sth` 线段树

```cpp
//
// 题目：P1531 I Hate It
// https://www.luogu.com.cn/problem/P1531
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};        // n,m<=200000 (+5 松弛)
constexpr int inf{1145141919};

int n,m;
int a[mxn],tr[mxn*4];

int ls(int i){return i*2+1;}
int rs(int i){return i*2+2;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		cin>>a[i];

	// 建树: 半开区间 [l, r), 根下标 0 (sth 模式)
	function<void(int,int,int)>sth{[&sth](int l,int r,int i){
		if(r-l==1)
			tr[i]=a[l];
		else
		{
			int mid=(l+r)>>1;
			sth(l,mid,ls(i));
			sth(mid,r,rs(i));
			tr[i]=max(tr[ls(i)],tr[rs(i)]);
		}
	}};
	sth(0,n,0);

	// 单点前缀高分更新: 仅当 a[pos] < val 时更新
	function<void(int,int,int,int,int)>upd{[&upd](int l,int r,int i,int pos,int val){
		if(r-l==1)
			tr[i]=max(tr[i],val);
		else
		{
			int mid=(l+r)>>1;
			if(pos<mid)
				upd(l,mid,ls(i),pos,val);
			else
				upd(mid,r,rs(i),pos,val);
			tr[i]=max(tr[ls(i)],tr[rs(i)]);
		}
	}};

	// 区间 [L, R) 最大值查询
	function<int(int,int,int,int,int)>qry{[&qry](int l,int r,int i,int L,int R)->int{
		if(R<=l||r<=L)
			return -inf;
		if(L<=l&&r<=R)
			return tr[i];
		int mid=(l+r)>>1;
		return max(qry(l,mid,ls(i),L,R),qry(mid,r,rs(i),L,R));
	}};

	for(int i=0;i<m;++i)
	{
		char op;
		int x,y;
		cin>>op>>x>>y;
		if(op=='Q')
			cout<<qry(0,n,0,x-1,y)<<'\n';
		else
			upd(0,n,0,x-1,y);
	}
	return 0;
}
```

### 5.2 对照实现：原数组直接维护与线性扫描（仅用于小规模对拍）

```cpp
//
// 题目：P1531 I Hate It
// 暴力基准: 直接维护原数组, 每次询问线性扫描 [l, r] 取最大值
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int n,m,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
	{
		char op;
		int x,y;
		cin>>op>>x>>y;
		if(op=='Q')
		{
			int mx=a[x-1];
			for(int j=x-1;j<y;++j)
				mx=max(mx,a[j]);
			cout<<mx<<'\n';
		}
		else
			a[x-1]=max(a[x-1],y);
	}
	return 0;
}
```
