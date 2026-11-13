# P1752 点菜

## 1. 重新读题

餐馆共有 $m$ 道菜（$m \le 2 \times 10^5$），第 $i$ 道菜具有美味度 $x_i$ 和价格 $y_i$。共有 $n$ 个人（$n \le 5 \times 10^4$），每周每人最多点一道菜：
- 其中 $p$ 个人挑剔：第 $i$ 个挑剔的人只点美味度 $\ge a_i$ 的菜（不限价格）；
- 其中 $q$ 个人贫穷：第 $j$ 个贫穷的人只点价格 $\le b_j$ 的菜（不限美味度）；
- 其余 $r = n - p - q$ 个人既不挑剔也不贫穷，可以点任意菜。

求所有人最少需要来几周，才能把全部 $m$ 道菜都至少点过一遍；若永远无法点完，输出 `-1`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：二分周数 $W$ 后的二维匹配贪心

显然，“$W$ 周能否点完全部 $m$ 道菜”关于 $W$ 具有单调性：若 $W$ 周可行，则 $W+1$ 周一定可行，且上界为 $m$。因此外层直接**二分周数 $W \in [\lceil m/n \rceil,\, m]$**！

当周数 $W$ 固定后，每个人最多可以点 $W$ 道菜。此时有三类顾客：
1. **挑剔者（$p$ 人）**：每人可点 $W$ 道满足 $x \ge a_i$ 的菜；
2. **贫穷者（$q$ 人）**：每人可点 $W$ 道满足 $y \le b_j$ 的菜；
3. **普通人（$r = n - p - q$ 人）**：总共可点 $r \cdot W$ 道任意菜。

挑剔者只看美味度 $x$，而贫穷者只看价格 $y$！这启发我们：**让挑剔者优先消灭那些“价格最高”的菜**，把价格便宜的菜留给贫穷者！

### 2.2 贪心交换论证（附完整证明）

**引理 1（固定周数 $W$ 的两阶段大根堆贪心判定）**：
1. **第一阶段（挑剔者选最贵菜）**：将挑剔者按美味度下限 $a_i$ 从大到小排序，菜品按美味度 $x$ 从大到小排序。依次考虑每个挑剔者 $i$：将所有美味度 $\ge a_i$ 的尚未入堆菜品压入一个**按价格 $y$ 排序的大根堆**，随后让挑剔者 $i$ 从堆顶弹出至多 $W$ 道价格最贵的菜。
2. **第二阶段（普通人与贫穷者消去剩余菜）**：将所有剩余未入堆的菜品也压入价格大根堆（此时堆中剩余的菜只能由贫穷者和普通人承担）。将贫穷者按价格上限 $b_j$ 从大到小排序；每次取出堆顶最贵的菜 $y$：
   - 若存在尚未用过的、且满足 $b_j \ge y$ 的最宽容贫穷者，则让该贫穷者承担至多 $W$ 道菜（或等价地：先让普通人吃掉堆顶最贵的 $r \cdot W$ 道菜，再将剩余菜按价格从大到小与贫穷者按 $b_j$ 从大到小贪心匹配，每人最多 $W$ 道）！

**证明**：
- **挑剔者阶段交换性**：对于按 $a_i$ 降序排列的挑剔者，当前大根堆中的所有菜对当前及后续所有挑剔者都是合法的（因为后续挑剔者的 $a$ 门槛更低，这些菜的美味度对后续挑剔者全都达标）。而在后续贫穷者与普通人眼中，菜品的美味度 $x$ 完全无意义，唯一限制是价格 $y$ 越低越容易被贫穷者接受。因此，让当前挑剔者优先取走堆中**价格 $y$ 最大**的 $W$ 道菜，不仅不会减少后续挑剔者可选的合法菜数，而且使最终留给贫穷者的菜品价格集合在偏序意义下逐项最小！
- **普通人与贫穷者阶段交换性**：挑剔者挑完后，剩下的菜只需看价格 $y$。普通人没有任何价格限制，总共能消去 $r \cdot W$ 道菜，显然应该直接消去剩余菜中**价格最高的 $r \cdot W$ 道菜**；此后再将余下的菜按价格从高到低依次塞给 $b_j$ 最大的贫穷者（每人至多 $W$ 道），若每一步都能满足 $y \le b_j$ 则 $W$ 可行，否则不可行。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用答案单调性二分周数 $W$，将问题转化为带容量 $W$ 的三类人群匹配可行性判定。
- **decouple two dimensions** — 挑剔者按美味度降序扫描，用大根堆维护当前可点的菜并优先取走**价格最高**的 $W$ 道，使留给贫穷者的菜价格最便宜（引理 1）。
- **classic problem solution** — 剩余菜品先由无限制普通人消去最贵的 $(n-p-q)W$ 道，再与降序排列的贫穷者贪心比对。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m, p, q` | `int` | 总人数、菜数、挑剔人数、贫穷人数 | 标准输入 | 维度 |
| `d[i]` | `pair<int,int>[mxm]` | 菜品的 `{美味度 x, 价格 y}` | 按 `x` 降序排序 | dishes |
| `a[i]` | `int [mxn]` | 挑剔者的美味度下限 | 降序排序 | picky thresholds |
| `b[j]` | `int [mxn]` | 贫穷者的价格上限 | 降序排序 | poor limits |

### 4.2 算法步骤

1. 读入 $n, m, p, q$、$m$ 道菜、$p$ 个挑剔下限 $a_i$、$q$ 个贫穷上限 $b_j$。
2. 将菜品 `d` 按美味度 `first` 降序排序，`a` 降序排序，`b` 降序排序。
3. 在区间 $[\lceil m / n \rceil,\, m]$ 上二分周数 $W$，用 `check(W)` 检验：
   - 维护价格大根堆 `pq`，指针 `ptr = 0`；
   - 遍历挑剔者 $i = 0 \dots p-1$：将所有满足 `d[ptr].first >= a[i]` 的 `d[ptr].second` 压入 `pq`；从 `pq` 弹出至多 $W$ 个元素；
   - 将剩余 `ptr < m` 的菜品价格全部压入 `pq`；
   - 先从 `pq` 弹出至多 $(n - p - q) \cdot W$ 个最贵的菜（由普通人承担）；
   - 遍历贫穷者 $j = 0 \dots q-1$：若 `pq` 非空且 `pq.top() > b[j]`，直接返回 `false`；否则从 `pq` 弹出至多 $W$ 个元素；
   - 返回 `pq.empty()`。
4. 若二分找不到合法 $W$ 输出 `-1`，否则输出最小合法 $W$。

### 4.3 复杂度分析

- **时间复杂度**：排序耗时 $O(m \log m)$；单次 `check(W)` 中每道菜入堆、出堆各一次，耗时 $O(m \log m)$，二分 $O(\log m)$ 次，总时间复杂度为 $O(m \log^2 m)$。
- **空间复杂度**：存储菜品、顾客与优先队列，空间复杂度为 $O(n + m)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1752 点菜
// https://www.luogu.com.cn/problem/P1752
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxm{200005},mxn{50005};

int n,m,p,q;
pair<int,int>d[mxm];
int a[mxn],b[mxn];

bool check(int w)
{
	priority_queue<int>pq;
	int ptr=0;
	for(int i=0;i<p;++i)
	{
		while(ptr<m&&d[ptr].first>=a[i])
		{
			pq.push(d[ptr].second);
			++ptr;
		}
		for(int k=0;k<w&&!pq.empty();++k)
			pq.pop();
	}
	while(ptr<m)
	{
		pq.push(d[ptr].second);
		++ptr;
	}
	i64 free_cap=1LL*(n-p-q)*w;
	while(free_cap>0&&!pq.empty())
	{
		pq.pop();
		--free_cap;
	}
	for(int j=0;j<q&&!pq.empty();++j)
	{
		if(pq.top()>b[j])
			return false;
		for(int k=0;k<w&&!pq.empty();++k)
			pq.pop();
	}
	return pq.empty();
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>p>>q;
	for(int i=0;i<m;++i)
		cin>>d[i].first>>d[i].second;
	for(int i=0;i<p;++i)
		cin>>a[i];
	for(int i=0;i<q;++i)
		cin>>b[i];

	sort(d,d+m,greater<pair<int,int>>());
	sort(a,a+p,greater<int>());
	sort(b,b+q,greater<int>());

	int lo=(m+n-1)/n,hi=m,ans=-1;
	while(lo<=hi)
	{
		int mid=(lo+hi)>>1;
		if(check(mid))
		{
			ans=mid;
			hi=mid-1;
		}
		else
			lo=mid+1;
	}
	cout<<ans<<'\n';
	return 0;
}
```
