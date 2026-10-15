# P1110 [ZJOI2007] 报表统计 解题报告

---

## 1. 重新读题

我们有一个长度为 $n$ 的非负整数序列，需要维护三种操作：

1. **`INSERT i k`**：在「原数列的第 $i$ 个元素」后面插入 $k$。重点是**每个原始位置 $i$ 都有一个"槽位"**，所有追加到该位置的元素按插入顺序排列在该槽位中。于是整个序列可以看作 $n$ 个槽位首尾相接：
   $$
   \underbrace{a_1,\; \text{(插入在1后的元素)}\;,\; \cdots}_{\text{槽位1}},\;
   \underbrace{a_2,\; \cdots}_{\text{槽位2}},\;
   \cdots,\;
   \underbrace{a_n}_{\text{槽位 $n$}}
   $$
   插入 `INSERT i k` 总是接到槽位 $i$ 的末尾，不影响其他槽位。槽位 $i$ 的第一个元素永远是 $a_i$。

2. **`MIN_GAP`**：查询**当前完整序列**中相邻两个元素的差的绝对值的最小值。

3. **`MIN_SORT_GAP`**：查询**整个多集**中任意两个元素的差的绝对值的最小值（即全部元素中最接近的一对的距离）。

数据范围：$n,m\le 5\times 10^5$，值域 $0\le a_i,k\le5\times10^8$。

要求：在线处理所有操作，不能离线。

---

## 2. 思维链与完整证明

### 2.1 序列的邻接结构

把整个序列在逻辑上分成 $n$ 个槽位。令：

- $\text{head}[i] = a_i$：槽位 $i$ 的第一个元素，永远不变。
- $\text{tail}[i]$：槽位 $i$ 的最后一个元素，初始为 $a_i$，每次 `INSERT i k` 后更新为 $k$。

则当前序列中所有相邻对有两种来源：

| 类型 | 表示 | 说明 |
|------|------|------|
| 槽内相邻 | $(\text{tail}[i],\;k)$ | `INSERT i k` 时产生，连接旧尾与新元素 |
| 槽间相邻 | $(\text{tail}[i],\;\text{head}[i+1])$ | 槽位 $i$ 的末尾与槽位 $i+1$ 的开头 |

当 `INSERT i k` 发生时：
1. 新产生槽内相邻对 $(\text{tail}[i],\,k)$。
2. 若 $i<n$：原有槽间相邻对 $(\text{tail}[i],\,\text{head}[i+1])$ 被**断开**，新产生槽间相邻对 $(k,\,\text{head}[i+1])$。

于是 `MIN_GAP` 的维护归结为维护一个**可删除的多集**，每次插入时删除至多一个旧对、添加至多两个新对。

### 2.2 MIN_GAP 维护

用 `multiset<int>` 存储所有相邻差的绝对值。记 $g = \text{gap}$。

- 初始：$\forall i\in[1,n-1]$，插入 $|a_i-a_{i+1}|$。
- 对于 `INSERT i k`：
  ```
  if i < n:
      g.erase(g.find(|tail[i] - a[i+1]|))
  g.insert(|tail[i] - k|)
  if i < n:
      g.insert(|k - a[i+1]|)
  tail[i] = k
  ```
- 查询：`*g.begin()`。

**关键细节**：`multiset::erase(value)` 会删除所有等于 value 的元素，所以必须用 `erase(find(value))` 删除仅一个。

### 2.3 MIN_SORT_GAP 维护

这是全局多集中最接近的一对。如果有**重复元素**，则答案为 $0$ 且永远不变。

用 `multiset<int>` 维护所有已出现的值。每次插入 $k$ 时：
1. 在 `vals` 中 `lower_bound(k)` 找到第一个 $\ge k$ 的位置 `it`。
2. 若 `it != vals.end()` 且 `*it == k`，则存在重复元素，将 `minsg` 置为 $0$。
3. 否则：
   - 若 `it != vals.end()`：用 `*it - k` 更新 `minsg`。
   - 若 `it != vals.begin()`：用 `k - *prev(it)` 更新 `minsg`。
4. 将 $k$ 插入 `vals`。

由于 `minsg` 是全局最小值，只减不增，一旦为 $0$ 可直接跳过后续所有 `MIN_SORT_GAP` 相关的插入检查。

**正确性证明**：每次只与有序序列中的前驱和后继比较即足以得到全局最小值。因为任意两个元素的差 $|x-y|$，在有序序列中必然被 $x$ 和紧随它的后继（或前驱和 $x$）的差所覆盖。若存在更小的差，其两端在有序序列中必然相邻。因此每次只比较前驱后继即可维护全局最小差值。$\square$

### 2.4 复杂度

- 时间：$O((n+m)\log (n+m))$，瓶颈在 multiset 的插入/删除/查找。
- 空间：$O(n+m)$。

---

## 3. 模式总结

| 子问题 | 核心技巧 | 数据结构 |
|--------|----------|----------|
| 维护动态序列的相邻差值的最小值 | 将序列拆分为槽位，每个槽位只关心首尾元素；相邻对的增删只发生在槽位边界和槽位末尾 | `multiset<int>` + 数组 $\text{tail}[i]$ |
| 维护多集中元素的最小差值 | 只需与前驱/后继比较；若存在重复则答案为 0 且不变 | `multiset<int>` + 前驱后继查询 |

本题的思维模式是**将复杂序列结构简化为若干不变量**：每个槽位的首元素不变、尾元素单调更新。利用这一性质，`MIN_GAP` 的维护量从 $O(n)$ 降至 $O(1)$ 次 multiset 操作。

---

## 4. 最终解法与符号表

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 原始序列长度 |
| $m$ | 操作次数 |
| $a_i$ | 原始序列第 $i$ 个元素 |
| $\text{tail}[i]$ | 槽位 $i$ 的当前最后一个元素 |
| $g$ | `multiset`，存储所有相邻差的绝对值 |
| $\text{vals}$ | `multiset`，存储所有已出现的元素值 |
| $\text{minsg}$ | 当前 `MIN_SORT_GAP` 的答案 |

### 算法流程

```
输入 n, m, a[1..n]
tail[i] = a[i]  (1 ≤ i ≤ n)
g ← ∅
for i = 1 to n-1:
    g.insert(|a[i] - a[i+1]|)
vals ← ∅
minsg ← INF
for each element in a:
    update_minsg(element)    // 前驱后继比较

for each operation:
    if INSERT i k:
        if i < n:
            g.erase(g.find(|tail[i] - a[i+1]|))
        g.insert(|tail[i] - k|)
        if i < n:
            g.insert(|k - a[i+1]|)
        tail[i] = k
        if minsg > 0:
            update_minsg(k)
    if MIN_GAP:
        输出 *g.begin()
    if MIN_SORT_GAP:
        输出 minsg
```

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
constexpr int mxn{500005};
constexpr int inf{1145141919};

int n,m;
int a[mxn];
int tail[mxn];
multiset<int> gap;
multiset<int> vals;
int minsg{inf};

void upd_minsg(int k)
{
	auto it{vals.lower_bound(k)};
	if(it!=vals.end())
	{
		if(*it==k)
		{
			minsg=0;
			return;
		}
		minsg=min(minsg,*it-k);
	}
	if(it!=vals.begin())
		minsg=min(minsg,k-*prev(it));
	vals.insert(k);
}

int main()
{
	cin.tie(nullptr)->sync_with_stdio(false);
	cin>>n>>m;
	for(int i{0};i<n;++i)
	{
		cin>>a[i];
		tail[i]=a[i];
		upd_minsg(a[i]);
	}
	for(int i{0};i<n-1;++i)
		gap.insert(abs(a[i]-a[i+1]));
	while(m--)
	{
		string op;cin>>op;
		if(op[0]=='I')
		{
			int i,k;cin>>i>>k;--i;
			if(i<n-1)
				gap.erase(gap.find(abs(tail[i]-a[i+1])));
			gap.insert(abs(tail[i]-k));
			if(i<n-1)
				gap.insert(abs(k-a[i+1]));
			tail[i]=k;
			if(minsg)
				upd_minsg(k);
		}
		else if(op[4]=='G') // MIN_GAP
			cout<<*gap.begin()<<'\n';
		else				// MIN_SORT_GAP
			cout<<minsg<<'\n';
	}
}
```
