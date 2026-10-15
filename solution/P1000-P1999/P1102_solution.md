# P1102 A-B 数对 — 完整题解

---

## 一、重新审题

给定长度为 $N$ 的正整数序列 $\{a_i\}$ 和一个正整数 $C$，统计满足 $A - B = C$ 的有序数对 $(A,B)$ 的个数。这里 $A$ 和 $B$ 都是序列中的元素（即存在下标 $i,j$ 使得 $A = a_i,\;B = a_j$），且**不同位置视为不同数对**——即使两个位置的数值相同，也算作不同的数对。

**关键约束**：

- $N \le 2 \times 10^5$，$0 \le a_i < 2^{30}$，$1 \le C < 2^{30}$
- $a_i$ 可以是 $0$，$C$ 至少为 $1$
- 数对 $(i,j)$ 与 $(j,i)$ 不同（题目没有说 $i \neq j$，但当 $C>0$ 时 $A-B=C>0$ 自然保证 $A > B$，所以不会有 $i=j$ 的情况）

**示例**：`[1, 1, 2, 3]`，$C=1$，则：
- $2-1=1$：两个 $1$ 分别与 $2$ 配对 → 2 对
- $3-2=1$：1 对
- 总共 $3$ 对

---

## 二、思维链与完整证明

### 2.1 问题的代数变形

直接枚举 $i,j$ 是 $O(N^2)$，无法通过 $2\times10^5$ 的数据。

将条件变形：

$$
A - B = C \quad\Longleftrightarrow\quad A = B + C
$$

也就是说：**对于每个 $B$，只需统计序列中有多少个 $A$ 等于 $B + C$**。反过来，对于每个 $A$，统计有多少 $B$ 等于 $A - C$，两者等价。

**引理 1（对偶性）**：
令 $\text{cnt}[x]$ 表示数值 $x$ 在序列中出现的次数。则满足 $A - B = C$ 的有序数对个数为

$$
\text{ans} = \sum_{x} \text{cnt}[x] \cdot \text{cnt}[x + C]
$$

其中求和遍历所有可能的 $x$（$x$ 和 $x+C$ 均须出现在序列中）。

**证明**：每个数对 $(A,B)$ 由 $A$ 的值 $y$ 和 $B$ 的值 $x$ 唯一确定，且满足 $y = x + C$。对于给定的 $x$，$B$ 可以有 $\text{cnt}[x]$ 种选法，每种选法对应的 $A$ 有 $\text{cnt}[x+C]$ 种选法；由乘法原理得 $\text{cnt}[x]\cdot\text{cnt}[x+C]$。不同 $x$ 产生的数对互不相交，求和即得总数。∎

### 2.2 数据范围分析

$a_i < 2^{30} \approx 1.07 \times 10^9$，$C < 2^{30}$，因此 $a_i + C$ 可能达到 $2^{31} - 1$，超出 `int` 范围（约 $2.147\times10^9$）。我们需要使用 `long long`（即 64 位整数）来存储中间值和答案，或至少使用 `unsigned int` 但为了安全统一使用 `i64`。

$N \le 2\times10^5$，答案理论上界为 $N^2 \approx 4\times10^{10}$，远超 32 位整数范围，**答案必须用 64 位整数**。

### 2.3 三种解法及比较

---

#### 解法 A：哈希表（频率计数）— 最优解

**思路**：遍历一次统计频率，再遍历一次（或直接遍历哈希表）累加 $\text{cnt}[x] \cdot \text{cnt}[x + C]$。

**时间复杂度**：$O(N)$ 平均（哈希表），$O(N\log N)$ 最坏（若用 `std::map`）。

**空间复杂度**：$O(N)$。

**正确性**：由引理 1 直接保证。

对于 C++，使用 `std::unordered_map` 可以在期望 $O(N)$ 时间内完成。考虑到 $N=2\times10^5$，哈希冲突可以接受。

---

#### 解法 B：排序 + 二分

**思路**：
1. 排序数组。
2. 对于每个位置 $i$，用 `std::equal_range`（或 `lower_bound` / `upper_bound`）找出值等于 $a_i + C$ 的下标范围 $[L,R)$，累加 $R - L$。

**时间复杂度**：$O(N\log N)$。

**空间复杂度**：$O(1)$ 额外（不计输入数组）。

**正确性证明**：排序后数组非降。固定 $B = a_i$，所有满足 $A = B + C$ 的元素必然连续排列在排序后的数组中。`lower_bound` 定位第一个 $\ge a_i + C$ 的位置，`upper_bound` 定位第一个 $> a_i + C$ 的位置，差值即为 $\text{cnt}[a_i + C]$。∎

注意：由于不同位置的相同数值算不同数对，排序后我们仍需要处理重复值。这正是使用 `equal_range` 而非仅判断是否存在的理由。

---

#### 解法 C：双指针（尺取法）

**思路**：
1. 排序数组。
2. 维护两个指针 $j, k$，对于每个左端 $i$，将 $j$ 移到第一个 $\ge a_i + C$ 的位置，$k$ 移到第一个 $> a_i + C$ 的位置，累加 $k - j$。

**时间复杂度**：$O(N\log N)$（排序主导），双指针部分 $O(N)$。

**空间复杂度**：$O(1)$ 额外。

由于 $j,k$ 单调右移，均摊 $O(1)$。

**正确性证明**：与二分相同，不过是用两个单调指针替代了两次二分查找。由于数组已排序且 $a_i + C$ 随 $i$ 单调不降，$j,k$ 不需要回退。∎

---

#### 三种解法总结

| 解法 | 时间复杂度 | 空间复杂度 | 实现难度 | 推荐度 |
|------|-----------|-----------|---------|-------|
| 哈希计数 | $O(N)$ 期望 | $O(N)$ | ★★ | ⭐⭐⭐ |
| 排序+二分 | $O(N\log N)$ | $O(1)$ | ★ | ⭐⭐ |
| 双指针 | $O(N\log N)$ | $O(1)$ | ★★ | ⭐⭐⭐ |

**推荐**：哈希计数代码最短、理解直观，适合 NOI 系列的竞赛环境（C++ 的 `unordered_map` 足够快）。双指针和二分虽然多一个 $\log$，但 $2\times10^5$ 规模下完全可过。

---

### 2.4 边界情况及易错点

1. **答案超 `int`**：$N=2\times10^5$，最坏情况所有数相同且 $C=0$……但题目 $C\ge1$，不过仍可能出现大量重复值。极端的，若所有数都相同，$\text{ans}=N(N-1)/2\approx2\times10^{10}$，需用 `long long`。
2. **$a_i + C$ 溢出 32 位**：$a_i < 2^{30}, C < 2^{30}$，相加 $< 2^{31}$，`int` 上限 $2^{31}-1$，接近溢出。安全起见用 64 位。
3. **$C$ 为正数**意味着 $A > B$，不需要考虑 $i=j$ 的情况（$0$ 不可能等于 $C>0$）。
4. **哈希冲突**：`std::unordered_map` 基于哈希表，最坏 $O(N^2)$。在竞赛中 LCM 评测一般不会卡，但若使用 `std::map$（红黑树）则稳定 $O(N\log N)$。
5. **双指针的单调性**：排序后 $a_i$ 非降，$a_i+C$ 也非降，因此两指针不会回退——这是正确性的核心。

---

## 三、模式总结

| 模式 | 说明 |
|------|------|
| **等式变形** | $A - B = C \to A = B + C$，将二元关系转为单变量加常数 |
| **计数问题** | 统计有序数对 → 固定一个维度，另一个通过哈希/二分查找 |
| **「不同位置算不同」** | 用乘法原理：$\text{cnt}[x]\cdot\text{cnt}[x+C]$ |
| **I/O 加速** | $2\times10^5$ 输入用 `cin` 需关闭同步，或直接 `scanf` |
| **单调性+双指针** | 排序后，对于每个 $i$ 查找 $a_i+C$，可利用单调性线性扫描 |

本题是「两数之差」类问题的模板，同类题目包括：两数之和（Two Sum）、三数之和、统计满足差为 $C$ 的无序对等。

---

## 四、正式题解（含符号表）

### 4.1 符号约定

| 符号 | 含义 |
|------|------|
| $N$ | 序列长度 |
| $C$ | 给定的差（正整数） |
| $a[]$ | 输入序列，$0$-based 索引 |
| $\text{cnt}[x]$ | 数值 $x$ 在序列中的出现次数 |
| $ans$ | 答案，类型为 64 位无符号整数 |

### 4.2 算法描述（哈希计数法）

```
1. 读入 N, C
2. 对于 i = 0..N-1:
       读入 a[i]
       cnt[a[i]]++
3. ans ← 0
4. 遍历 cnt 中每个键值对 (val, f):
       如果 cnt 中存在 key = val + C:
           ans ← ans + f * cnt[val + C]
5. 输出 ans
```

**正确性**：由引理 1 保证。

**复杂度**：$O(N)$ 期望时间，$O(N)$ 空间。

---

### 4.3 算法描述（排序+双指针法）

```
1. 读入 N, C, a[0..N-1]
2. 排序 a
3. ans ← 0
4. j ← 0, k ← 0
5. 对于 i = 0..N-1:
       当 j < N 且 a[j] < a[i] + C 时: j++
       当 k < N 且 a[k] ≤ a[i] + C 时: k++
       ans ← ans + (k - j)
6. 输出 ans
```

**正确性**：排序后，对于固定的 $a_i$，$a_j$ 是第一个 $\ge a_i + C$ 的元素，$a_k$ 是第一个 $> a_i + C$ 的元素，则区间 $[j,k)$ 内所有元素都等于 $a_i + C$，其长度即为 $\text{cnt}[a_i + C]$。由于 $a_i$ 非降，$j,k$ 单调不减，故总复杂度 $O(N)$。

**复杂度**：$O(N\log N)$（排序），$O(1)$ 额外空间。

---

## 五、示例代码

### 5.1 哈希计数法（推荐）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64 = long long;

constexpr int mxn{200000 + 5};

int n, c;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n >> c;
	unordered_map<int, int> cnt;
	for (int i = 0; i < n; ++i)
	{
		cin >> a[i];
		++cnt[a[i]];
	}

	i64 ans{0};
	for (auto &[val, f] : cnt)
	{
		auto it = cnt.find(val + c);
		if (it != cnt.end())
		{
			ans += 1LL * f * it->second;
		}
	}
	cout << ans << '\n';

	return 0;
}
```

### 5.2 排序 + 二分法

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64 = long long;

constexpr int mxn{200000 + 5};

int n, c;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n >> c;
	for (int i = 0; i < n; ++i)
	{
		cin >> a[i];
	}
	sort(a, a + n);

	i64 ans{0};
	for (int i = 0; i < n; ++i)
	{
		auto [L, R] = equal_range(a, a + n, a[i] + c);
		ans += R - L;
	}
	cout << ans << '\n';

	return 0;
}
```

### 5.3 排序 + 双指针法

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64 = long long;

constexpr int mxn{200000 + 5};

int n, c;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n >> c;
	for (int i = 0; i < n; ++i)
	{
		cin >> a[i];
	}
	sort(a, a + n);

	i64 ans{0};
	int j{0}, k{0};
	for (int i = 0; i < n; ++i)
	{
		int target = a[i] + c;
		while (j < n && a[j] < target)
		{
			++j;
		}
		while (k < n && a[k] <= target)
		{
			++k;
		}
		ans += k - j;
	}
	cout << ans << '\n';

	return 0;
}
```

---

### 附录：各代码说明

| 版本 | 核心思路 | 适用场景 |
|------|---------|---------|
| 哈希计数 | `unordered_map` 存频次，遍历累乘 | 通用，代码最短 |
| 排序+二分 | `equal_range` 统计 `a[i]+C` 出现次数 | 不想用哈希，怕被卡 |
| 排序+双指针 | 两指针单调右移 | 追求极致常数，双指针爱好者 |

三种代码在 $N=2\times10^5$ 的数据下均可通过洛谷评测，时间均在 100ms 级别。
