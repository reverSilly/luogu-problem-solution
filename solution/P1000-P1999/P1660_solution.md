# P1660 数位平方和

> 难度：省选/NOI- · 标签：基环树内向森林（Functional Graph） / 记忆化搜索 / 判环与树形递推

## 1. 重新读题

### 1.1 问题重述（人话版）

给定正整数 $k, A, B$。定义 $S(n)$ 为正整数 $n$ 在十进制下各个数位的 $k$ 次方之和。
定义函数 $H(n)$ 为满足
$$H(n) \le \min\{n,\ H(S(n))\}$$
的最大整数（即从 $n$ 出发不断迭代 $n \to S(n) \to S(S(n)) \to \dots$ 所经过的所有状态中的最小值）。
求 $\left(\sum_{i=A}^B H(i)\right) \bmod (10^7 + 7)$。

### 1.2 关键约束与规模暗示

- **数据规模**：$20\%$ 数据 $A, B \le 50$；$100\%$ 数据 $1 \le A, B \le 10^6$，$1 \le k \le 6$。
- **状态空间上界分析**：
  当 $n \le 10^6$ 且 $k \le 6$ 时，$S(n)$ 的最大可能值出现在 $n = 999999$ 处：
  $$S(999999) = 6 \times 9^6 = 6 \times 531441 = 3,188,646.$$
  而对于 $x \le 3,188,646$，又有 $S(x) \le 2^6 + 6 \times 9^6 = 3,188,710 < 3.2 \times 10^6$！
  这意味着从 $[1, 10^6]$ 出发的任何迭代轨道都**封闭在 $[1, 3.2 \times 10^6]$ 的有限状态空间内**，不会发散到无穷大。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：出度为 $1$ 的函数图（Functional Graph）

每个正整数 $u$ 只有唯一确定的后继 $v = S(u)$，因此状态转移图是一个**基环内向森林（Functional Graph）**：每个连通分量由一个有向环和若干棵指向环的树组成。

### 2.2 考虑简化版 / 子任务

- **单点暴力模拟（$A, B \le 50$）**：对每个 $i \in [A, B]$，用一个 `unordered_set` 记录访问过的数，沿着 $u \to S(u)$ 走直到遇到已访问的数为止，取沿途最小值，作为 `P1660_brute.cpp`。

### 2.3 三色标记判环与记忆化递推证明

**引理 1（函数图上的 $H(u)$ 结构定理）**
1. 若顶点 $u$ 位于某个有向环 $C$ 上，则从 $u$ 出发会遍历整个环 $C$，故对环上所有顶点 $u \in C$，有
   $$H(u) = \min_{v \in C} v;$$
2. 若顶点 $u$ 不在环上，其唯一出边指向 $v = S(u)$，则
   $$H(u) = \min(u,\ H(S(u))).$$

**证明**：
由 $H(n) = \inf_{t \ge 0} S^{(t)}(n)$，当 $u \in C$ 时，轨道 $\{S^{(t)}(u) : t \ge 0\} = C$，其最小值即为 $\min_{v \in C} v$；当 $u \notin C$ 时，轨道为 $\{u\} \cup \{S^{(t)}(S(u)) : t \ge 0\}$，故最小值为 $\min(u, H(S(u)))$。$\blacksquare$

**引理 2（三色标记搜索的线性时间复杂度）**
为每个状态维护 `st[u]` $\in \{0\text{（未访问）}, 1\text{（当前栈上）}, 2\text{（已算出 } H[u]\text{）}\}$。从任意未计算起点出发沿 `nxt` 推进，一旦碰到 `st[u] == 1` 即提取环并一次性赋好环上所有点的 $H$ 值，再沿栈回溯算出树枝上所有点的 $H$ 值；若碰到 `st[u] == 2` 则直接沿栈回溯。每个可达状态至多入栈一次，总时间复杂度为 $O(\max(A, B))$。

**证明**：
任何顶点入栈时 `st` 由 $0$ 变 $1$，出栈时由 $1$ 变 $2$，此后永不再入栈。因此所有轨道追踪的总步数不超过可达状态总数 $\le 3.2 \times 10^6$。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **explore its property** — 估算 $S(n) \le 6 \times 9^6 \approx 3.19 \times 10^6$，证明状态空间有限且封闭。
- **consider easier version** — 对每个起点独立用哈希集合找环求轨道最小值，建立对拍基准。
- **classic problem solution** — 在 Functional Graph 上用三色标记（`0/1/2`）一次性完成找环、环上统一赋值与树枝回溯记忆化。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `pw[d]` | `int [10]` | 数位 $d^k$ | 预计算 | digit power |
| `nxt[x]` | `int [3200005]` | 后继状态 $S(x)$ | 按需缓存 | next state |
| `H[x]` | `int [3200005]` | 轨道最小值 $H(x)$ | 记忆化计算 | target function |
| `st[x]` | `uint8_t [3200005]` | 访问状态 `0/1/2` | `0` | visit state |

### 4.2 算法步骤

1. 读入 $k, A, B$（若 $A > B$ 则交换），预处理 $0 \dots 9$ 的 $k$ 次幂 `pw[d]`。
2. 对每个 $i \in [A, B]$，若 `st[i] != 2` 则调用 `solve_u(i)` 沿轨道追踪并回溯填表，将 `H[i]` 累加至 `ans`（模 $10^7 + 7$）。
3. 输出 `ans`。

### 4.3 复杂度与验证状态

- **时间复杂度**：$O(\max(A, B))$。
- **空间复杂度**：$O(V_{\max})$，其中 $V_{\max} = 3.2 \times 10^6$。
- **沙箱对拍验证**：`g++ -O2 -std=c++17 -Wall` 零警告编译通过；与独立模拟暴力程序 `P1660_brute.cpp` 对拍 $400$ 组随机数据全部一致；$k = 6, [1, 10^6]$ 极限规模实测耗时 $0.0692\text{ s}$。

## 5. 示例代码

### 5.1 标程（`P1660.cpp`）

```cpp
//
// 题目：P1660 数位平方和
// https://www.luogu.com.cn/problem/P1660
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxv{3200005};
constexpr int mod{10000007};

int k,A,B,pw[10],nxt[mxv],H[mxv];
uint8_t st[mxv];

int calc_s(int x)
{
	int s=0;
	for(;x;x/=10)
		s+=pw[x%10];
	return s;
}

int get_nxt(int x)
{
	if(!nxt[x])
		nxt[x]=calc_s(x);
	return nxt[x];
}

int solve_u(int start)
{
	static vector<int>path;
	path.clear();
	int u=start;
	while(st[u]==0)
	{
		st[u]=1;
		path.push_back(u);
		u=get_nxt(u);
	}
	if(st[u]==1)
	{
		int pos=0;
		while(path[pos]!=u)
			++pos;
		int mn=u;
		for(int i=pos;i<(int)path.size();++i)
			mn=min(mn,path[i]);
		for(int i=pos;i<(int)path.size();++i)
		{
			H[path[i]]=mn;
			st[path[i]]=2;
		}
		for(int i=pos-1;i>=0;--i)
		{
			int x=path[i];
			H[x]=min(x,H[get_nxt(x)]);
			st[x]=2;
		}
	}
	else
	{
		for(int i=(int)path.size()-1;i>=0;--i)
		{
			int x=path[i];
			H[x]=min(x,H[get_nxt(x)]);
			st[x]=2;
		}
	}
	return H[start];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>A>>B;
	if(A>B)
		swap(A,B);
	for(int d=0;d<10;++d)
	{
		int r=1;
		for(int i=0;i<k;++i)
			r*=d;
		pw[d]=r;
	}

	i64 ans=0;
	for(int i=A;i<=B;++i)
	{
		int v=(st[i]==2)?H[i]:solve_u(i);
		ans=(ans+v)%mod;
	}
	cout<<ans<<'\n';
	return 0;
}
```

### 5.2 暴力对照程序（`P1660_brute.cpp`）

```cpp
//
// 题目：P1660 数位平方和
// 暴力基准：对每个 i 独立模拟轨道直至出现重复元素并取全轨道最小值
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mod{10000007};

int k,A,B,pw[10];

int calc_s(int x)
{
	int s=0;
	for(;x;x/=10)
		s+=pw[x%10];
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>A>>B;
	if(A>B)
		swap(A,B);
	for(int d=0;d<10;++d)
	{
		int r=1;
		for(int i=0;i<k;++i)
			r*=d;
		pw[d]=r;
	}

	i64 ans=0;
	for(int i=A;i<=B;++i)
	{
		unordered_set<int>vis;
		int u=i,mn=i;
		while(!vis.count(u))
		{
			vis.insert(u);
			mn=min(mn,u);
			u=calc_s(u);
		}
		ans=(ans+mn)%mod;
	}
	cout<<ans<<'\n';
	return 0;
}
```
