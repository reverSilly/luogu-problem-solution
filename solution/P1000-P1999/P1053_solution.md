# P1053 [NOIP 2005 提高组] 篝火晚会 题解

---

## 1. 题意分析

有 $n$ 个同学，初始按 $1,2,\dots,n$ 的顺序坐成一圈。每个同学有两个最希望相邻的同学。佳佳可以下达"循环移位"命令：

$$(b_1,b_2,\dots,b_{m-1},b_m)$$

表示 $b_1$ 移到 $b_2$ 的位置，$b_2$ 移到 $b_3$ 的位置，……，$b_m$ 移到 $b_1$ 的位置。该命令的代价为 $m$。

问：能否通过若干命令得到一个满足所有同学意愿的圆排列？若能，求最小总代价；若不能，输出 $-1$。

**数据范围**：$3\le n\le 5\times 10^4$。

---

## 2. 关键观察与转化

### 2.1 目标圆排列的构造

每个同学恰好给出 $2$ 个期望相邻的同学。由于相邻关系是双向的（若 $i$ 希望与 $j$ 相邻，则 $j$ 的期望邻接中也应有 $i$），因此每个同学在图中的度恰好为 $2$。这意味着整个图由若干个环构成。

若图不连通（即多于一个环），则不可能将所有同学排成一个同时满足所有人意愿的圈，输出 $-1$。

若图连通，则从任意一个同学出发，沿期望邻接关系走一遍，即可唯一确定一个圆排列（顺时针方向）。将其反转即得到逆时针方向的圆排列。目标圆排列有两种可能的方向，我们都需要考虑。

### 2.2 代价的实质

一次命令 $(b_1,\dots,b_m)$ 的本质是对这 $m$ 个位置执行一个**循环置换**（cyclic shift），代价为 $m$。

考虑从**初始排列**到**目标排列**的置换 $\pi$。将 $\pi$ 分解为若干不相交的循环：

- 长度为 $1$ 的循环（即固定点）：该同学已经坐在正确的位置，不需要任何命令。
- 长度为 $k\;(k\ge 2)$ 的循环：可以用一次命令将其还原，代价为 $k$。

因此，**最小总代价** 等于所有非平凡循环的长度之和：

$$\text{cost} = \sum_{k\ge 2} k = n - (\text{固定点个数})$$

问题转化为：在所有可能的旋转（以及翻转）下，最大化固定点的个数。

### 2.3 旋转与固定点计数

圆排列是循环的，我们可以自由选择"起始位置"。换言之，我们可以将目标圆排列旋转任意偏移 $d$。

设目标圆排列为数组 $t[0..n-1]$，初始排列为 $a[0..n-1]$，其中 $a[i] = i+1$（$0$-based 下即 $a[i] = i$ 对应的同学编号）。

对于编号为 $i$ 的同学（从 $0$ 开始编号），设其在目标圆排列中的位置为 $pos[i]$（即 $t[pos[i]] = i$）。若旋转偏移为 $d$，那么该同学在旋转后的圆排列中位于位置 $(pos[i] - d + n) \bmod n$。当这个位置等于 $i$ 时，该同学成为固定点：

$$(pos[i] - d + n) \bmod n = i \quad\Longrightarrow\quad d \equiv pos[i] - i \pmod{n}$$

所以，对每个同学 $i$，可以计算出一个偏移量 $d_i = (pos[i] - i + n) \bmod n$。对相同的 $d$ 计数，$cnt[d]$ 就是在旋转 $d$ 下的固定点个数。

对目标圆排列的正反两个方向各做一次上述计数，取最大的 $cnt[d]$，答案即为 $n - \max cnt[d]$。

---

## 3. 算法设计

### 3.1 可行性判定

建立邻接表。对每个同学检查其期望邻接是否恰好为 $2$ 个（输入保证，可省略）。从 $1$ 号同学出发，尝试构造一个环：

- 维护当前节点 $cur$ 和前驱 $prev$。
- 每次选择 $cur$ 的邻接中不是 $prev$ 的那一个作为下一个节点。
- 若某步发现邻接数不为 $2$ 或无法继续，则无解。
- 若走完 $n$ 步后无法回到起点，也无解。

### 3.2 目标圆排列构造

从 $1$ 号出发，按上述规则走一圈，得到顺时针方向的圆排列 $t[0..n-1]$。翻转 $t$ 得到逆时针方向 $rev[0..n-1]$。

### 3.3 固定点计数

对方向 $t$：

- 计算 $pos[t[i]] = i$（位置映射）。
- 对 $i = 0..n-1$，令 $d = (pos[i] - i + n) \bmod n$，$cnt[d]++$。
- 取 $mx = \max cnt[d]$。

对方向 $rev$ 重复上述过程，更新 $mx$。

### 3.4 输出答案

$$ans = n - mx$$

若之前判定无解，输出 $-1$。

---

## 4. 复杂度分析

- **时间复杂度**：$O(n)$。构造圆排列一次遍历 $O(n)$；两个方向的固定点计数各 $O(n)$。
- **空间复杂度**：$O(n)$。存储邻接表、目标圆排列、位置映射、计数数组。

$n\le 5\times 10^4$，$O(n)$ 的算法完全满足要求。

---

## 5. 代码实现

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n;
vector<int> g[mxn];
int t[mxn],pos[mxn],cnt[mxn];

void build(int s,int tarr[])
{
	int prev=0,cur=s;
	for(int i=0;i<n;++i)
	{
		tarr[i]=cur;
		int nxt=g[cur][0];
		if(nxt==prev)
			nxt=g[cur][1];
		prev=cur;
		cur=nxt;
	}
}

int calc(int tarr[])
{
	fill_n(cnt,n,0);
	for(int i=0;i<n;++i)
		pos[tarr[i]]=i;
	for(int i=0;i<n;++i)
	{
		int d=(pos[i]-i+n)%n;
		++cnt[d];
	}
	int mx=0;
	for(int i=0;i<n;++i)
		mx=max(mx,cnt[i]);
	return mx;
}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
	{
		int u,v;
		cin>>u>>v;--u;--v;
		g[i-1].push_back(u);
		g[i-1].push_back(v);
	}

	for(int i=0;i<n;++i)
	{
		if(g[i].size()!=2)
		{
			cout<<-1<<'\n';
			return 0;
		}
	}

	{
		int prev=-1,cur=0;
		for(int i=0;i<n;++i)
		{
			int nxt=g[cur][0];
			if(nxt==prev)
				nxt=g[cur][1];
			prev=cur;
			cur=nxt;
		}
		if(cur!=0)
		{
			cout<<-1<<'\n';
			return 0;
		}
	}

	build(0,t);
	int mx=calc(t);

	reverse(t,t+n);
	mx=max(mx,calc(t));

	int ans=n-mx;
	cout<<ans<<'\n';
	return 0;
}
```
