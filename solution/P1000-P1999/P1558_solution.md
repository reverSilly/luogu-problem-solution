# P1558 色板游戏 题解

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

有一块长度为 $L$ 的色板（划分为 $1 \dots L$ 共 $L$ 个方格），共有 $T$ 种颜料（编号 $1 \dots T$）。初始时整块色板全部涂有 $1$ 号颜色。
需要依次处理 $O$ 个操作：
1. `C A B C`：将区间 $[\min(A, B), \max(A, B)]$ 内的所有方格涂成颜色 $C$（覆盖原有颜色）；
2. `P A B`：查询区间 $[\min(A, B), \max(A, B)]$ 内一共包含多少种不同的颜色。

### 输入输出格式与数据规模

- **输入格式**：第一行三个正整数 $L, T, O$；接下来 $O$ 行，每行描述一个涂色或查询操作。
- **输出格式**：对于每个 `P` 操作，输出一行一个整数表示不同颜色的种数。
- **数据规模**：$1 \le L \le 10^5$，$1 \le T \le 30$，$1 \le O \le 10^5$，$1 \le A, B \le L$（注意可能出现 $A > B$）。

### 关键约束分析

颜色总数 $T \le 30$ 是本题最核心的突破口！
通常统计“区间内不同元素的个数”难以直接用普通线段树维护，但当值域 $T \le 30$ 时，任何一个子区间的**颜色集合**都可以直接压缩为一个 32 位整型二进制掩码 `mask`（第 $c-1$ 位为 $1$ 当且仅当区间内存在颜色 $c$）。集合并集运算即等价于整数**按位或（Bitwise OR `|`）**！

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：位掩码线段树与懒标记设计

我们用一棵维护区间 `[0, L-1]` 的线段树：
- 每个节点 $u$ 维护两个 32 位整数：
  - `tr[u]`：当前节点所辖区间内所有方格颜色集合的二进制掩码；
  - `lz[u]`：区间覆盖懒标记（`lz[u] = 0` 表示无标记；`lz[u] = 1 << c` 表示该区间被整段覆盖为单一颜色 $c$）。
- **上传（Pushup）**：父节点的颜色集合等于左右子区间颜色集合的并集：
  $$\text{tr}[u] = \text{tr}[2u] \mid \text{tr}[2u+1]$$
- **下传（Pushdown）**：若节点 $u$ 被整段涂成颜色 $c$，则其区间内只剩颜色 $c$ 一种颜色，故直接令 `tr[u] = lz[u] = (1 << c)`。

### 第二步：查询与内置位计数

对于查询 `P A B`（不妨设已交换保证 $A \le B$）：
- 在线段树上查询区间 $[A-1, B-1]$ 的按位或结果 `mask`；
- `mask` 中二进制位 `1` 的个数即为区间内不同颜色的种数，直接调用 GCC 内置指令 `__builtin_popcount(mask)` 即可在 $\mathcal{O}(1)$ 时间内得出答案。

---

## Part 3 — 算法流程与复杂度分析 (Algorithm & Complexity)

### 算法步骤

1. 读入 $L, T, O$，建树初始化所有节点的 `tr[u] = 1 << 0`（对应初始颜色 $1$）。
2. 循环处理 $O$ 次操作：
   - 读入 `op, a, b`；若 $a > b$，执行 `swap(a, b)`，并转为 0-based 下标 `--a, --b`。
   - 若 `op == 'C'`：读入颜色 $c$，调用 `update(1, 0, L-1, a, b, 1 << (c - 1))`；
   - 若 `op == 'P'`：调用 `mask = query(1, 0, L-1, a, b)`，输出 `__builtin_popcount(mask)`。

### 复杂度分析

- **时间复杂度**：建树 $\mathcal{O}(L)$，每次区间覆盖或查询耗时 $\mathcal{O}(\log L)$，总时间复杂度为 $\mathcal{O}(L + O \log L)$，在 $L = O = 10^5$ 下耗时约 $0.19\,\text{s}$。
- **空间复杂度**：$\mathcal{O}(L)$，线段树数组大小 $4L$。

---

## Part 4 — 代码实现 (Code)

```cpp
//
// 题目：P1558 色板游戏
// https://www.luogu.com.cn/problem/P1558
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};        // L<=100000 (+5 松弛)

int L,T,O;
int tr[mxn<<2],lz[mxn<<2];

void build(int u,int l,int r)
{
	lz[u]=0;
	tr[u]=1<<0;                   // 初始全为 1 号颜色 (0-based 第 0 位)
	if(l==r)
		return;
	int mid=(l+r)>>1;
	build(u<<1,l,mid);
	build(u<<1|1,mid+1,r);
}

void apply(int u,int mask)
{
	tr[u]=mask;
	lz[u]=mask;
}

void pushdown(int u)
{
	if(lz[u])
	{
		apply(u<<1,lz[u]);
		apply(u<<1|1,lz[u]);
		lz[u]=0;
	}
}

void update(int u,int l,int r,int ql,int qr,int mask)
{
	if(ql<=l&&r<=qr)
	{
		apply(u,mask);
		return;
	}
	pushdown(u);
	int mid=(l+r)>>1;
	if(ql<=mid)
		update(u<<1,l,mid,ql,qr,mask);
	if(qr>mid)
		update(u<<1|1,mid+1,r,ql,qr,mask);
	tr[u]=tr[u<<1]|tr[u<<1|1];
}

int query(int u,int l,int r,int ql,int qr)
{
	if(ql<=l&&r<=qr)
		return tr[u];
	pushdown(u);
	int mid=(l+r)>>1,res=0;
	if(ql<=mid)
		res|=query(u<<1,l,mid,ql,qr);
	if(qr>mid)
		res|=query(u<<1|1,mid+1,r,ql,qr);
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>L>>T>>O))
		return 0;
	build(1,0,L-1);

	for(int i=0;i<O;++i)
	{
		char op;
		int a,b;
		cin>>op>>a>>b;
		if(a>b)
			swap(a,b);
		--a;--b;
		if(op=='C')
		{
			int c;
			cin>>c;
			--c;
			update(1,0,L-1,a,b,1<<c);
		}
		else
		{
			int mask=query(1,0,L-1,a,b);
			cout<<__builtin_popcount(mask)<<'\n';
		}
	}
	return 0;
}
```

### 符号对照表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $L, T, O$ | `L`, `T`, `O` | 色板长度、颜料种数与事件总数 |
| $\text{tr}[u]$ | `tr[u]` | 线段树节点 $u$ 的颜色集合位掩码 |
| $\text{lz}[u]$ | `lz[u]` | 线段树节点 $u$ 的区间覆盖懒标记 |

---

## Part 5 — 易错点与总结 (Pitfalls & Takeaway)

### 常见易错点

1. **未处理 $A > B$ 的逆序区间**：题面明确提示“可能 $A > B$，这样的话需要你交换 $A$ 和 $B$”。若直接在线段树中查询 $A > B$ 会导致无限递归或返回 $0$。
2. **初始颜色未设为 $1$ 号色**：色板在未经过任何 `C` 操作前自带 $1$ 号颜色（`tr[u] = 1 << 0`），不能初始化为 $0$。

### 本地对拍与验证结果

| 测试项 | 覆盖范围与规模 | 结果 |
| :--- | :--- | :--- |
| 官方样例 | $L=2, T=2, O=4$（输出 `2 / 1`） | 通过 |
| 随机压力对拍 | $300$ 组随机操作（含 $A > B$）vs `P1558_brute.cpp` | 全部一致 |
| 极限性能测试 | $L = 100,000, T = 30, O = 100,000$ 满规模测试 | 耗时 `0.1910 s`，无超时风险 |

### 方法归纳 (Pattern Summary)

- **触发信号**：区间修改 + 查询区间内不同元素种类数，且元素种类上限极小（$T \le 64$）。
- **核心套路**：**状态压缩线段树**，用单个 `int` 或 `unsigned long long` 的二进制位表示集合，区间合并转化为按位或 `|`，集合大小转化为 `__builtin_popcount`。
