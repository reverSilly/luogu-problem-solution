# P1114 "非常男女" 计划 — 完整题解

---

## 1. 重新阅读问题

给定一个长度为 $n\;(1\le n\le 10^5)$ 的 01 序列，$0$ 代表女生，$1$ 代表男生。要求找出**最长连续子区间**，使得该区间内男生人数和女生人数相等。输出该区间长度；若不存在则输出 $0$。

---

## 2. 思维链（含完整证明）

### 2.1 问题转化

"男女人数相等" 等价于区间内 $1$ 的个数等于 $0$ 的个数。
令变换后的序列 $b[i]$ 满足：

$$
b[i] = \begin{cases}
+1, & a[i] = 1\;(男生) \\
-1, & a[i] = 0\;(女生)
\end{cases}
$$

则对任意区间 $[l, r]$（$0$ 起始下标），区间和

$$
S(l,r) = \sum_{i=l}^{r} b[i] = (\#男生) - (\#女生)
$$

男女人数相等 $\iff S(l,r)=0$。

### 2.2 前缀和模型

定义前缀和数组 $pref$，其中 $pref[0]=0$，$pref[k]=\sum_{i=0}^{k-1} b[i]$（即前 $k$ 个元素的变换和）。

那么区间 $[l, r]$ 的和为：

$$
S(l,r) = pref[r+1] - pref[l]
$$

条件 $S(l,r)=0$ 等价于：

$$
pref[r+1] = pref[l]
$$

区间长度 = $r - l + 1 = (r+1) - l$。

因此问题转化为：

> **在 $pref[0..n]$ 中找两个下标 $i<j$ 使得 $pref[i]=pref[j]$，最大化 $j-i$。**

### 2.3 正确性证明（充要性）

- **必要性**：若 $[l, r]$ 男女人数相等，则 $pref[r+1]-pref[l]=0$，故 $pref[r+1]=pref[l]$。
- **充分性**：若 $pref[i]=pref[j]\;(i<j)$，令 $l=i,\;r=j-1$，则 $pref[r+1]-pref[l]=pref[j]-pref[i]=0$，故区间 $[l,r]$ 内男生等于女生，且长度为 $j-i$。

所以只需对每个不同的前缀和值记录**最早出现位置**和**最晚出现位置**，最大差值即为答案。

### 2.4 值域分析

$b[i] = \pm 1$，$n\le 10^5$，因此 $pref[k] \in [-n, n]$，值域宽度 $2n+1$。可以使用偏移数组代替哈希表，$O(1)$ 访问。

### 2.5 边界情况

- 若整个序列没有相等子区间：所有 $pref$ 值均唯一，最大差值为 $0$。
- 空区间：不合法，题目要求至少选一个人（尽管输出允许 $0$）。
- $pref[0]=0$ 必须在扫描前记录。

### 2.6 算法步骤

1. 读入 $n$ 及长度为 $n$ 的 01 序列。
2. 分配数组 `first` 大小为 $2n+1$，初始化全为 $-1$。
3. 设偏移量 `base = n`，将 $pref$ 值映射到 $[0, 2n]$。
4. `first[0 + base] = 0`，记录 $pref[0]=0$ 出现在下标 $0$。
5. 遍历 $i=1$ 到 $n$：
   - 根据 $a[i-1]$ 更新 `pref += (a[i-1] == 1 ? 1 : -1)`。
   - 若 `first[pref + base] == -1`，则 `first[pref + base] = i`。
   - 否则 `ans = max(ans, i - first[pref + base])`。
6. 输出 `ans`。

### 2.7 复杂度

- 时间：$O(n)$，单遍扫描，每次 $O(1)$。
- 空间：$O(n)$，`first` 数组 $2n+1$ 个整数。

---

## 3. 模式总结

| 模式 | 说明 |
|------|------|
| **相等条件 → 差分变换** | 将"两种元素数量相等"转化为变换后的区间和为 $0$ |
| **区间和 → 前缀和差** | 区间 $[l,r]$ 的和转化为 $pref[r+1]-pref[l]$ |
| **相等前缀和 → 同值最远距离** | $pref[i]=pref[j]$ 时区间长度为 $j-i$，记录每个值的首末位置 |
| **整数有界 → 偏移数组** | 值域已知且较小（$\pm n$），用偏移数组代替哈希表 |

### 相关题目

- LeetCode 525. Contiguous Array（几乎相同）
- Luogu P3131 [USACO16JAN] Subsequences Summing to Sevens（模数前缀和）
- Luogu P3941 入阵曲（二维推广）

---

## 4. 真实解法（含符号表）

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 人数 |
| $a[0..n-1]$ | 原始 01 序列 |
| $b[i]$ | 变换值：$a[i]=1\to +1,\;a[i]=0\to -1$ |
| $pref[k]$ | 前 $k$ 个元素的 $b$ 之和，$pref[0]=0$ |
| $first[v]$ | 前缀和值 $v$ 首次出现的下标 |
| $base$ | 偏移量 $=n$，将 $[-n,n]$ 映射到 $[0,2n]$ |
| $ans$ | 最长区间长度 |

### 算法（伪代码）

```
输入 n, 数组 a
base ← n
first[0..2n] ← {-1}
first[0 + base] ← 0
pref ← 0, ans ← 0
for i ← 1 to n:
    pref ← pref + (a[i-1]==1 ? 1 : -1)
    idx ← pref + base
    if first[idx] == -1:
        first[idx] ← i
    else:
        ans ← max(ans, i - first[idx])
输出 ans
```

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
constexpr int mxn{100005};

int n, ans, pref, a[mxn];
int first[mxn<<1]; // 2n+1

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin >> n;
	for (int i{0}; i < n; ++i)
	{
		cin >> a[i];
	}

	int const base{n};
	fill_n(first, n * 2 + 1, -1);
	first[0 + base] = 0;

	for (int i{1}; i <= n; ++i)
	{
		pref += (a[i - 1] == 1 ? 1 : -1);
		int idx{pref + base};
		if (first[idx] == -1)
		{
			first[idx] = i;
		}
		else
		{
			ans = max(ans, i - first[idx]);
		}
	}

	cout << ans << '\n';

	return 0;
}
```

### 代码说明

- **(L6)** `first[mxn<<1]`：大小 $2n+1$，`mxn` 已含冗余。
- **(L17)** 偏移 `base = n`，保证下标非负。
- **(L18)** `fill_n` 初始化为 $-1$。
- **(L19)** 前缀 $pref[0]=0$ 出现在位置 $0$。
- **(L22)** 根据 $a[i-1]$ 更新 $pref$，$0$ 对应 $-1$，$1$ 对应 $+1$。
- **(L23–28)** 首次出现则记录下标，否则更新答案。
- 下标 $i$ 表示已处理 $i$ 个元素（即 $pref[i]$），与数学定义一致。
