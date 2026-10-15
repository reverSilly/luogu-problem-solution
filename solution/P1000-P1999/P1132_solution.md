# P1132 数字生成游戏 — 完整题解

---

## 1. 重新阅读问题

我们有一个**不含数字 0** 的初始数字 $s$（$s<10^5$，因此长度 $L\le 5$）。允许三种操作：

1. **交换**：交换 $s$ 的任意两位，得到一个新的数字（长度不变）。
2. **删除**：删除 $s$ 的任意一位，得到一个新的数字（长度减 1）。
3. **插入**：在 $s$ 的相邻两位 $s_i,s_{i+1}$ 之间插入一个数字 $x$，要求 $s_i<x<s_{i+1}$（长度加 1）。

**附加限制**：规则 3 生成的新数位数**不能超过初始 $s$ 的位数** $L$。例如 $s=143$（$L=3$）时，无法生成 $1243$（长度 4）；但 $s=1443$（$L=4$）时，可以先删除为 $143$ 再插入得到 $1243$。

给定 $m\le 5\times10^4$ 个询问，每个询问给出不含 0 的目标数 $t$，求从 $s$ 出发到达 $t$ 的**最少操作次数**，若不可达则输出 $-1$。询问之间独立，每次只考虑从原始 $s$ 出发。

---

## 2. 思维链与完整证明

### 2.1 问题本质：无权图最短路

三种操作每次代价均为 1，所有可生成的数构成一个**无权有向图**（实际上所有边可逆？需要验证）：

- **交换是可逆的**：交换同一对数字即可恢复。
- **删除与插入不互逆**：删除后丢失信息，但插入可引入新数字。
- 因此图是**有向的**，但 BFS 仍然正确适用于有向无权图。

目标：从源点 $s$ 到每个目标 $t$ 的最短路径长度。

### 2.2 状态空间上界

**引理 1**：任何可达数字都不含 0，且长度不超过 $L$。

*证明*：
- 初始 $s$ 不含 0。
- 交换只重排已有数字，不引入 0。
- 删除只移除数字，不引入 0。
- 插入要求 $s_i < x < s_{i+1}$，$s_i\ge1$，故 $x\ge2$，不可能为 0。
- 题目明确限制规则 3 生成数长度 $\le L$，而规则 1 保持长度、规则 2 减少长度。
- 由数学归纳法，所有可达状态均不含 0 且长度 $\le L$。□

**推论**：状态空间大小 $\displaystyle\sum_{k=1}^{L}9^k = \frac{9^{L+1}-9}{8} \le \frac{9^6-9}{8}=66420$。$L\le5$，上界约 $6.6\times10^4$ 个状态。

### 2.3 三种操作的邻接生成

对当前数字 $cur$（长度为 $len$），记其十进制字符串为 $\overline{d_0d_1\dots d_{len-1}}$。

- **规则 1（交换）**：枚举 $0\le i<j<len$，交换 $d_i,d_j$，得到新数字。
	生成数量：$\binom{len}{2}\le\binom{5}{2}=10$。
- **规则 2（删除）**：枚举 $0\le i<len$，删除 $d_i$，得到新数字。
	生成数量：$len\le5$。
- **规则 3（插入）**：仅当 $len<L$ 时应用。枚举 $0\le i<len-1$，记 $a=d_i,\;b=d_{i+1}$。插入数字 $x$ 满足 $a<x<b$，即 $x\in\{a+1,\dots,b-1\}$。
	生成数量：$\sum_{i=0}^{len-2}\max(0,b-a-1)$，最多 $4\times7=28$。

每个状态最多约 $10+5+28=43$ 条出边。总边数 $\le 6.6\times10^4\times43\approx 2.8\times10^6$，完全可行。

### 2.4 BFS 正确性

**引理 2**：在无权有向图上从源点 $s$ 做 BFS，首次访问某顶点时的距离即为从 $s$ 到该顶点的最短路径长度。

*证明*：BFS 按距离递增顺序扩展顶点。若存在更短路径，该路径上的前一顶点会更早出队，从而当前顶点会更早被访问。□

**引理 3**：所有可能被访问的状态都满足长度 $\le L$ 且不含 0，BFS 不会遗漏合法状态。

*证明*：由引理 1，任何合法后继都满足这些约束，而我们的生成规则正确实现了三种操作并检查了长度限制。□

### 2.5 不可达判断

若 $t$ 的长度 $>L$，由引理 1 可知永远无法生成，直接输出 $-1$。否则查询 BFS 结果数组，$-1$ 表示不可达。

### 2.6 复杂度分析

| 项目 | 复杂度 | 说明 |
|------|--------|------|
| 状态数 | $O(9^L)\le 6.6\times10^4$ | |
| 出边数/状态 | $O(L^2)\le 43$ | |
| BFS 总操作 | $O(9^L\cdot L^2)\approx 2.8\times10^6$ | |
| 回答询问 | $O(m)$，$m\le5\times10^4$ | 查表 $O(1)$ |
| 空间 | $O(10^5)$ | 距离数组 |

---

## 3. 模式总结

| 观察 | 结论 |
|------|------|
| 数据范围：$s<10^5$ 且 $m$ 大 | 状态空间有限，可**预处理所有可达状态**，$O(1)$ 回答询问 |
| 三种操作代价均为 1 | **无权图最短路 → BFS** |
| 不含 0 且长度受限 | 状态上界 $\sum 9^k\approx 6.6\times10^4$，可以用**静态定长数组**存储距离 |
| 插入受相邻数字大小约束 | 插入只能在 $a<b$ 的位置进行，取值范围 $(a,b)$ |
| 规则 3 长度限制 | 仅在 $len<L$ 时应用规则 3，保证不超限 |

**核心思想**：将数字视为字符串，在隐式图上做 BFS，预处理 $s$ 到所有可达状态的最短距离。

---

## 4. 真实解法

### 4.1 符号约定

| 符号 | 含义 |
|------|------|
| $s$ | 初始数字（字符串形式） |
| $L=|s|$ | 初始数字长度 |
| $dist[x]$ | 从 $s$ 到数字 $x$ 的最短步数，初始 $-1$ |
| $len$ | 当前状态的数字长度 |
| $d_i$ | 数字字符串的第 $i$ 位 |
| $a,b$ | 插入位置左右相邻的数字 |

### 4.2 算法流程

```
1. 读入 s，计算 L = |s|。
2. 初始化 dist[0..100000] = -1。
3. start = int(s)，dist[start] = 0，将 start 入队。
4. BFS 循环：
   a. 出队 cur，记 d = dist[cur]。
   b. 将 cur 转为字符串 cur_s。
   c. 规则 1：对所有 i<j，交换 cur_s[i],cur_s[j]，得到 nxt。
       若 dist[nxt] == -1，设 dist[nxt]=d+1，入队。
   d. 规则 2：对所有 i，删除 cur_s[i]，得到 nxt。
       若 nxt 非空且 dist[nxt] == -1，设 dist[nxt]=d+1，入队。
   e. 规则 3：若 len < L，对所有相邻位置 (i,i+1)：
       令 a=cur_s[i], b=cur_s[i+1]（数值）。
       对 x ∈ (a,b)，插入 x 到 i 与 i+1 之间，得 nxt。
       若 dist[nxt] == -1，设 dist[nxt]=d+1，入队。
5. 对每个询问 t：
   if |t| > L or dist[t] == -1: 输出 -1
   else: 输出 dist[t]
```

### 4.3 关键实现细节

- 用 `to_string` / `stoi` 系列或手写函数在字符串与整数间转换。由于数字极小，`to_string` + 手写 `to_int` 均可。
- 距离数组大小取 $100000+5$，因为最大 5 位数为 $99999$。
- 规则 2 删除后字符串可能为空（单数字删除），需跳过。
- 规则 3 中 $a<b$ 才可能有 $x$，$a\ge b$ 时内层循环直接跳过。

### 4.4 正确性要点

- BFS 保证最短路（无权图）。
- 长度检查 `len < L` 保证了规则 3 的约束。
- 所有生成操作都不引入 0，状态空间一致。
- 预处理一次，$O(1)$ 回答每个询问。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int N{100000};

int dist[N + 5];
string s;
int L;

int to_int(const string& str)
{
	int res{0};
	for(char c : str)
		res = res * 10 + (c - '0');
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> s;
	L = s.size();

	fill_n(dist, N + 5, -1);

	queue<int> q;
	int start = to_int(s);
	dist[start] = 0;
	q.push(start);

	while(!q.empty())
	{
		int cur = q.front(); q.pop();
		int d = dist[cur];
		string cur_s = to_string(cur);
		int len = cur_s.size();

		// Rule 1: swap
		for(int i{0}; i < len; ++i)
		{
			for(int j{i + 1}; j < len; ++j)
			{
				swap(cur_s[i], cur_s[j]);
				int nxt = to_int(cur_s);
				if(dist[nxt] == -1)
				{
					dist[nxt] = d + 1;
					q.push(nxt);
				}
				swap(cur_s[i], cur_s[j]);
			}
		}

		// Rule 2: delete
		for(int i{0}; i < len; ++i)
		{
			string nxt_s = cur_s.substr(0, i) + cur_s.substr(i + 1);
			if(nxt_s.empty()) continue;
			int nxt = to_int(nxt_s);
			if(dist[nxt] == -1)
			{
				dist[nxt] = d + 1;
				q.push(nxt);
			}
		}

		// Rule 3: insert (only when len < L)
		if(len < L)
		{
			for(int i{0}; i + 1 < len; ++i)
			{
				int a = cur_s[i] - '0';
				int b = cur_s[i + 1] - '0';
				for(int x{a + 1}; x < b; ++x)
				{
					string nxt_s = cur_s.substr(0, i + 1)
					               + char('0' + x)
					               + cur_s.substr(i + 1);
					int nxt = to_int(nxt_s);
					if(dist[nxt] == -1)
					{
						dist[nxt] = d + 1;
						q.push(nxt);
					}
				}
			}
		}
	}

	int m; cin >> m;
	while(m--)
	{
		int t; cin >> t;
		if(to_string(t).size() > L || dist[t] == -1)
			cout << "-1\n";
		else
			cout << dist[t] << '\n';
	}

	return 0;
}
```
