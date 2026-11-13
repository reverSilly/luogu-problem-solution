# P1792 [国家集训队] 种树

## 1. 重新读题

在一个包含 $n$（$n \le 2 \times 10^5$）个位置的环形广场上，顺时针编号为 $1 \dots n$，第 $i$ 个位置的美观度为 $A_i$（$-1000 \le A_i \le 1000$）。要求从中选出**恰好 $m$ 个两两不相邻**的位置种树，使得美观度总和最大；若不存在合法方案，输出 `Error!`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：无解条件与反悔贪心（双向链表 + 大根堆）

首先判断何时无解：在长度为 $n$ 的环上，相邻位置不能同时种树，最多只能选 $\lfloor n / 2 \rfloor$ 个位置。因此当 $m > \lfloor n / 2 \rfloor$ 时直接输出 `Error!`；当 $m \le \lfloor n / 2 \rfloor$ 时必有解。

如果每一步直接贪心选当前权值最大的位置 $u$，会遇到经典反例：例如 `[8, 10, 8]` 选 $m = 2$ 棵树，第一步选了中间的 $10$，第二步就没法再选了，而最优解是放弃 $10$、改选两侧的两个 $8$（总和 $16$）！

**引理 1（环上最大权独立集的反悔贪心缩点定理）**：
设 $u$ 是当前所有位置中权值 $A_u$ 最大的一个。在选 $k$ 个不相邻位置的最优解 $S^*$ 中，必有以下二选一成立：
1. 要么 $u \in S^*$（此时 $u$ 的左右邻居 $L_u, R_u$ 均不在 $S^*$ 中）；
2. 要么 $u \notin S^*$，但此时**必然同时有 $L_u \in S^*$ 且 $R_u \in S^*$**！（因为若 $L_u, R_u$ 中至多只有一个属于 $S^*$，将其替换为权值最大的 $u$ 不会引发新的相邻冲突且权值和不减。）

因此，当我们贪心选上全局最大值 $u$（收益 $+A_u$）后，只需将 $(L_u, u, R_u)$ 三个位置从双向循环链表中删去，并在原位放回一个权值为
$$A'_u = A_{L_u} + A_{R_u} - A_u$$
的“反悔缩点”：若后续某一步选上了这个缩点（收益 $+A'_u$），累加后的总贡献恰为 $A_u + (A_{L_u} + A_{R_u} - A_u) = A_{L_u} + A_{R_u}$，完美等价于撤销 $u$ 并同时选上 $L_u, R_u$，且对外侧邻居的排斥边界也与选 $\{L_u, R_u\}$ 完全一致！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 环上最多可种 $\lfloor n / 2 \rfloor$ 棵不相邻的树；对全局最大值 $u$，最优解要么选 $\{u\}$、要么同时选其左右邻居 $\{L_u, R_u\}$（引理 1）。
- **classic problem solution** — 用大根堆维护当前最大权值，配合环形双向链表将 $(L_u, u, R_u)$ 缩为权值 $A_{L_u} + A_{R_u} - A_u$ 的反悔节点，循环 $m$ 步即可在 $\Theta(n \log n)$ 时间内求解。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 环长与树苗数 | 标准输入 | 维度 |
| `a[i]` | `i64 [mxn]` | 位置 $i$ 的当前（或反悔后）权值 | 标准输入 | value |
| `L[i], R[i]` | `int [mxn]` | 环形双向链表的左、右邻居下标 | 循环相连 | doubly linked list |
| `del[i]` | `bool [mxn]` | 节点 $i$ 是否已被合并删除 | `false` | deleted flag |
| `pq` | `priority_queue` | 维护 `{a[i], i}` 的大根堆 | 压入 $1 \dots n$ | max-heap |

### 4.2 算法步骤

1. 读入 $n, m$ 及 $a[1 \dots n]$。若 $m > n / 2$，输出 `Error!` 并结束。
2. 初始化环形双向链表 `L[i], R[i]`，将所有 `(a[i], i)` 压入大根堆 `pq`。
3. 循环 $m$ 次：
   - 弹出堆顶直到取出未删除节点 $u$（`!del[u]`）；
   - 令 `ans += a[u]`，取出左右邻居 `l = L[u], r = R[u]`；
   - 更新反悔权值 `a[u] = a[l] + a[r] - a[u]`，标记 `del[l] = del[r] = true`；
   - 在双向链表中删去 $l, r$（令 `L[u] = L[l], R[L[l]] = u, R[u] = R[r], L[R[r]] = u`），将更新后的 `(a[u], u)` 重新压入 `pq`。
4. 输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：初始建堆 $\Theta(n \log n)$，共执行 $m$ 次反悔缩点，每次压入 $1$ 个新元素、弹出至多常数个已删除元素，总时间复杂度为 $\Theta((n + m) \log n)$（耗时约 $45\text{ ms}$）。
- **空间复杂度**：链表与堆占用 $\Theta(n)$ 空间。

---

## 5. 示例代码

```cpp
//
// 题目：P1792 [国家集训队] 种树
// https://www.luogu.com.cn/problem/P1792
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{200005};

int n,m;
i64 a[mxn];
int L[mxn],R[mxn];
bool del[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=1;i<=n;++i)
		cin>>a[i];

	if(m>n/2)
	{
		cout<<"Error!\n";
		return 0;
	}

	priority_queue<pair<i64,int>>pq;
	for(int i=1;i<=n;++i)
	{
		L[i]=(i==1)?n:(i-1);
		R[i]=(i==n)?1:(i+1);
		pq.push({a[i],i});
	}

	i64 ans=0;
	for(int step=0;step<m;++step)
	{
		while(del[pq.top().second])
			pq.pop();
		int u=pq.top().second;
		pq.pop();

		ans+=a[u];
		int l=L[u],r=R[u];
		a[u]=a[l]+a[r]-a[u];
		del[l]=del[r]=true;

		L[u]=L[l];R[L[l]]=u;
		R[u]=R[r];L[R[r]]=u;
		pq.push({a[u],u});
	}

	cout<<ans<<'\n';
	return 0;
}
```
