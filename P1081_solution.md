# P1081 [NOIP 2012 提高组] 开车旅行

---

## 一、再读题目

数轴上有 $n$ 座城市，编号 $1\dots n$ 自西向东排列。每个城市有一个互不相同的海拔 $h_i$。定义 $i,j$ 间的**距离**为 $|h_i-h_j|$。

两人 **小 A**（第一天开车，之后与小 B 轮流）和 **小 B** 从某个起点 $s$ 出发，始终**向东行驶**（只能去编号更大的城市）。他们有一个总里程上限 $x$。

驾驶规则：

- **小 B** 总是选择**最近**的城市（距离最小，若等距则选海拔更低者）作为下一站；
- **小 A** 总是选择**第二近**的城市（同一距离定义）作为下一站。

若无人能按规则选到下一站，或到达下一站会让总里程超过 $x$，则旅行结束。

需要回答两个问题：

1. 给定 $x_0$，求使 $\displaystyle\frac{\text{小A总里程}}{\text{小B总里程}}$ 最小的起点 $s$（若小 B 里程为 $0$ 则比值为无穷大，两个无穷大相等；若有多个 $s$ 比值相同，选海拔最高的）。
2. $m$ 组询问 $(s_i,x_i)$，输出小 A 和小 B 各自的总里程。

**数据范围**：$n,m\le 10^5,\;|h_i|\le 10^9,\;x_i\le 10^9,\;h_i$ 互不相同。

---

## 二、思维链与完整证明

### 2.1 预计算：每个城市的下一个目标

由于只能向东，对城市 $i$ 我们只关心 $j>i$。定义：

- $B(i)$：从 $i$ 出发小 B 会去的城市（最近）；
- $A(i)$：从 $i$ 出发小 A 会去的城市（第二近）。

距离定义是 $|h_i-h_j|$，因此最近的 $j$ 就是在 $h_i$ 的两侧找海拔最接近的值。**这等价于在已出现的海拔集合中查找 $h_i$ 的前驱和后继**。因为若 $j$ 是最近点，则 $h_j$ 要么是 $h_i$ 的**前驱**（小于 $h_i$ 的最大值），要么是**后继**（大于 $h_i$ 的最小值）。

但第二近可能是前驱的前驱、后继的后继，也可能是前驱和后继中较远的那一个。所以需要从 **4 个候选**中选出前两名。

**算法**：从右向左扫描 $i=n\dots1$，用一个 `std::set` 维护所有已扫描城市（即 $i$ 右侧的城市）的 $(h_j,j)$ 对。对每个 $i$：

- 用 `lower_bound` 找到 $\ge h_i$ 的第一个位置 -> 后继 $s_1$；
- 后继的下一个 -> 再后继 $s_2$；
- 前驱 $p_1$ 是 `prev(lower_bound)`；
- 前驱的前一个 -> 再前驱 $p_2$。

收集至多 4 个候选，按 $(|h_i-h_j|,\;h_j)$ 升序排序。排序后第一个就是 $B(i)$，第二个就是 $A(i)$。

**正确性证明**：任意 $j>i$ 与 $i$ 的距离 $|h_i-h_j|$，前驱和后继是 $h_i$ 左右最近的点，因此距离最小的点一定在前驱或后继中。第二小的点可能是另一个前驱/后继，也可能是第二前驱或第二后继。$4$ 个候选覆盖了所有可能比第一候选更近的情形，因此取前两名正确。

### 2.2 交替行驶的结构与倍增

从 $i$ 出发的**一轮完整行驶**定义为：小 A 先开（到 $A(i)$），然后小 B 再开（到 $B(A(i))$）。我们记：

- $to[i][0]=B(A(i))$，即一轮后的位置；
- $da[i][0]=|h_i-h_{A(i)}|$，小 A 在该轮中的里程；
- $db[i][0]=|h_{A(i)}-h_{B(A(i))}|$，小 B 在该轮中的里程。

若 $A(i)=0$（不存在第二近）或 $B(A(i))=0$（小 B 无法从 $A(i)$ 出发），则一轮无法完成，$to[i][0]=0$。

倍增递推（$k\ge 1$）：

$$
\begin{aligned}
to[i][k] &= to[\,to[i][k-1]\,][k-1] \\
da[i][k] &= da[i][k-1] + da[\,to[i][k-1]\,][k-1] \\
db[i][k] &= db[i][k-1] + db[\,to[i][k-1]\,][k-1]
\end{aligned}
$$

其中 $to[i][k-1]=0$ 时停止递推（$da[i][k],db[i][k]$ 自然保留为 $0$）。

**倍增的正确性**：由于行驶规则完全由当前城市决定，与历史无关（无后效性），$2^{k}$ 轮的总效果可以由两个 $2^{k-1}$ 轮拼接而成。结合律成立。

### 2.3 回答询问

对一个询问 $(s,x)$：

1. 设当前城市 $cur=s$，$totA=totB=0$。
2. 从大到小枚举 $k$（如 $k=17\dots0$）：
   - 若 $to[cur][k]\neq 0$ 且 $totA+totB+da[cur][k]+db[cur][k]\le x$，则加入这 $2^k$ 轮的里程，$cur\leftarrow to[cur][k]$。
3. 最后尝试一次小 A 的单独移动：若 $A(cur)\neq 0$ 且剩余里程足够，则加上小 A 的距离。

**为什么最后只需要试小 A？** 因为倍增循环已经处理了整轮（A→B），而小 A 先开车，所以多出来的第一步一定是小 A 的。若加了小 A 后还有余量，小 B 是否还能移动？会的，但那会在下一轮倍增中被处理——但由于我们是从大到小枚举倍增块，任何剩余空间都会尽可能用大的块填满，最终剩余的空间连最小的一轮（A→B）都不够，自然连小 B 的单独移动也不够。但若剩余空间够小 A 但不够小 B，则单独补小 A 即可。

### 2.4 比值比较

对小 A 里程 $a$ 和小 B 里程 $b$：

- 若 $b=0$，比值视为 $+\infty$；
- 否则比值为 $\dfrac{a}{b}$。

比较两组 $(a_1,b_1)$ 和 $(a_2,b_2)$ 的大小：

```
若 b1=0 且 b2=0: 相等，选海拔更高的起点
若 b1=0:         (a1,b1) 比值无穷大 > (a2,b2) 有限比值
若 b2=0:         (a1,b1) 有限比值 < (a2,b2) 无穷大
否则:           比较 a1*b2 < a2*b1
```

由于 $x\le 10^9$，$a,b\le 10^9$，$a\times b\le 10^{18}< 2^{63}$，可用 `long long` 安全交叉相乘。

---

## 三、模式总结

| 难题要素 | 本题对应模式 |
|---|---|
| **交替决策的确定性跳转** | 用 set 有序维护右侧海拔，O(log n) 找出最近/第二近 |
| **长链模拟** | 倍增（binary lifting），预处理 $2^k$ 轮后的位置和累积里程 |
| **两维度交替** | 将 A→B 打包为一轮，拆掉交替性 |
| **前缀和 + 上限约束** | 用倍增块的总里程与 $x$ 比较，快速跳过满足条件的段 |
| **比值比较** | 交叉相乘避免浮点，特判分母为 0 |
| **最优起点枚举** | 对 $n$ 个起点各做一次对数级查询，复杂度 O(n log n) 可接受 |

**核心技巧**：把**交替驱动**的二人博弈转化为**一轮轮独立的确定性跳转**，再用倍增将 O(n) 模拟降为 O(log n)。

---

## 四、完整解法（含符号表）

### 符号约定

| 符号 | 含义 |
|---|---|
| $n$ | 城市数量 |
| $h_i$ | 城市 $i$ 的海拔 |
| $x_0$ | 问题一的里程上限 |
| $m,\;s_i,\;x_i$ | 询问数、起点、里程上限 |
| $A(i)$ | 从 $i$ 出发小 A 的去向（第二近） |
| $B(i)$ | 从 $i$ 出发小 B 的去向（最近） |
| $to[i][k]$ | 从 $i$ 出发经过 $2^k$ 轮的位置 |
| $da[i][k],\,db[i][k]$ | 上述 $2^k$ 轮中小 A / 小 B 的里程 |
| $totA,\,totB$ | 询问结果中的累积里程 |

### 算法步骤

1. **预计算 $A(i),B(i)$**：从右向左扫描，用 `set` 维护右侧 $(h_j,j)$，取前驱/后继共至多 4 个候选，排序取前两名。
2. **倍增表**：对 $i=1\dots n$ 初始化 $to[i][0],da[i][0],db[i][0]$（若 $A(i)=0$ 或 $B(A(i))=0$ 则 $to[i][0]=0$）；然后 $k=1\dots \lfloor\log_2 n\rfloor$ 递推。
3. **问题一**：枚举 $s=1\dots n$，调用 `query(s, x0)` 获得 $(a,b)$，用交叉相乘比较比值，记录最优起点。
4. **问题二**：对每个询问调用 `query(s_i, x_i)`，输出 $(a,b)$。

---

## 五、参考代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr int lg{18};

int n, h[mxn];
int ga[mxn], gb[mxn];		// A(i), B(i)
int to[mxn][lg];
i64 da[mxn][lg], db[mxn][lg];
int x0, m;

void pre()
{
	set<pair<int,int>> s; // (altitude, index)
	for (int i = n; i >= 1; i--)
	{
		vector<tuple<int,int,int>> cand;
		auto it = s.lower_bound({h[i], i});
		if (it != s.end())
		{
			cand.push_back({abs(h[i] - it->first), it->first, it->second});
			auto it2 = next(it);
			if (it2 != s.end())
				cand.push_back({abs(h[i] - it2->first), it2->first, it2->second});
		}
		if (it != s.begin())
		{
			auto it2 = prev(it);
			cand.push_back({abs(h[i] - it2->first), it2->first, it2->second});
			if (it2 != s.begin())
			{
				auto it3 = prev(it2);
				cand.push_back({abs(h[i] - it3->first), it3->first, it3->second});
			}
		}
		sort(cand.begin(), cand.end());
		if (cand.size() >= 1) gb[i] = get<2>(cand[0]);
		if (cand.size() >= 2) ga[i] = get<2>(cand[1]);
		s.insert({h[i], i});
	}
}

void build()
{
	for (int i = 1; i <= n; i++)
		if (ga[i] && gb[ga[i]]) // one full round: A then B
		{
			to[i][0] = gb[ga[i]];
			da[i][0] = abs(h[i] - h[ga[i]]);
			db[i][0] = abs(h[ga[i]] - h[gb[ga[i]]]);
		}
	for (int j = 1; j < lg; j++)
		for (int i = 1; i <= n; i++)
			if (to[i][j-1])
			{
				to[i][j] = to[to[i][j-1]][j-1];
				da[i][j] = da[i][j-1] + da[to[i][j-1]][j-1];
				db[i][j] = db[i][j-1] + db[to[i][j-1]][j-1];
			}
}

void query(int s, int x, i64 &totA, i64 &totB)
{
	totA = totB = 0;
	int cur = s;
	for (int j = lg - 1; j >= 0; j--)
		if (to[cur][j] && totA + totB + da[cur][j] + db[cur][j] <= x)
		{
			totA += da[cur][j];
			totB += db[cur][j];
			cur = to[cur][j];
		}
	// leftover A move
	if (ga[cur] && totA + totB + abs(h[cur] - h[ga[cur]]) <= x)
		totA += abs(h[cur] - h[ga[cur]]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n;
	for (int i = 1; i <= n; i++) cin >> h[i];

	pre();
	build();

	// part 1
	cin >> x0;
	int ans = 0;
	i64 bestA = 0, bestB = 0;
	for (int s = 1; s <= n; s++)
	{
		i64 a, b;
		query(s, x0, a, b);
		if (!ans)
		{
			ans = s; bestA = a; bestB = b;
			continue;
		}
		bool better = false;
		if (b == 0 && bestB == 0)
			better = h[s] > h[ans];
		else if (b == 0)		// current = INF
			better = false;
		else if (bestB == 0)	// best = INF
			better = true;
		else
			better = a * bestB < bestA * b;
		if (better) { ans = s; bestA = a; bestB = b; }
	}
	cout << ans << '\n';

	// part 2
	cin >> m;
	while (m--)
	{
		int s, x; cin >> s >> x;
		i64 a, b;
		query(s, x, a, b);
		cout << a << ' ' << b << '\n';
	}

	return 0;
}
```
