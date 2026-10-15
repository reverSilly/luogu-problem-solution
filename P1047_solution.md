# P1047 [NOIP 2005 普及组] 校门外的树 题解

---

## 一、题目分析

### 题意简述

有一条长度为 $l$ 的马路，从 $0$ 到 $l$ 的每个整数点上都种有一棵树，共计 $l+1$ 棵。现要建造 $m$ 个地铁区域，每个区域用区间 $[u, v]$ 表示（包含端点）。需要将每个区域内的所有树（包括端点处的两棵树）移走。区域之间可能存在重叠。问最终马路上还剩多少棵树。

### 数据范围

- $1 \leq l \leq 10^4$
- $1 \leq m \leq 100$
- $0 \leq u \leq v \leq l$

数据规模非常小，即使是 $O(l \times m)$ 的算法也能轻松通过。但这道题作为经典入门题，非常适合用来练习多种基础算法思想。

### 关键点

1. 树的总数为 $l+1$（包含 $0$ 和 $l$ 两个端点）。
2. 移走区间包含端点，即闭区间 $[u, v]$。
3. 区域可能重叠，不能重复计算移走的树。

---

## 二、算法思路

本题的核心在于"区间覆盖"的计数。由于数据范围小，主要有以下几种思路：

### 思路 1：直接模拟（布尔数组标记）

开一个长度为 $l+1$ 的布尔数组，初始全为 `true`（有树）。对每个区间 $[u, v]$，将对应位置标记为 `false`（移走）。最后统计 `true` 的个数。

### 思路 2：差分数组

使用差分数组 $diff$ 来记录每个位置被覆盖的次数。令 $diff[u]$ 加 1，$diff[v+1]$ 减 1。做一次前缀和后，每个位置的值表示该点被多少个区域覆盖。统计值为 0 的位置个数即为剩余树木。

### 思路 3：区间合并

将所有区间按左端点排序，然后从左到右合并重叠区间，计算所有合并后区间的总长度。用总树数减去总长度即为答案。

---

## 三、多种解法

### 解法 1：布尔数组模拟（最直观）

这是最直接的模拟方法，适合初学者理解问题的本质。

```cpp
#include<bits/stdc++.h>
using namespace std;

bool hasTree[10005];

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int l, m;
    cin >> l >> m;
    for (int i = 0; i <= l; ++i)
    {
        hasTree[i] = true;
    }
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        cin >> u >> v;
        for (int j = u; j <= v; ++j)
        {
            hasTree[j] = false;
        }
    }
    int ans = 0;
    for (int i = 0; i <= l; ++i)
    {
        if (hasTree[i])
        {
            ++ans;
        }
    }
    cout << ans << '\n';

    return 0;
}
```

### 解法 2：差分数组（推荐）

差分数组是处理"多次区间加减，最后单点查询"问题的经典工具。相比解法 1，它不需要内层循环遍历区间内的每个点，将时间复杂度从 $O(l \times m)$ 优化到 $O(l + m)$。

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};
int diff[mxn];

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int l, m;
    cin >> l >> m;
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        cin >> u >> v;
        diff[u] += 1;
        diff[v + 1] -= 1;
    }
    int cur = 0, ans = 0;
    for (int i = 0; i <= l; ++i)
    {
        cur += diff[i];
        if (cur == 0)
        {
            ++ans;
        }
    }
    cout << ans << '\n';

    return 0;
}
```

**差分数组原理说明**：

区间 $[u, v]$ 加 1 等价于在差分数组上做两次单点修改：$diff[u] \text{ += } 1$ 表示从 $u$ 位置开始生效，$diff[v+1] \text{ -= } 1$ 表示从 $v+1$ 位置开始失效。做完所有区间操作后，对整个差分数组做一次前缀和，$cur = \sum_{j=0}^{i} diff[j]$ 即为位置 $i$ 被覆盖的次数。

### 解法 3：区间合并

当区间数 $m$ 远小于长度 $l$ 时（本题 $m \leq 100$），区间合并法更加高效，它只关心区间的端点。

```cpp
#include<bits/stdc++.h>
using namespace std;

struct range
{
    int l, r;
};

range a[105];

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int l, m;
    cin >> l >> m;
    for (int i = 0; i < m; ++i)
    {
        cin >> a[i].l >> a[i].r;
    }

    sort(a, a + m, [](range x, range y)
    {
        return x.l < y.l;
    });

    int removed = 0;
    int curL = a[0].l, curR = a[0].r;
    for (int i = 1; i < m; ++i)
    {
        if (a[i].l <= curR)
        {
            curR = max(curR, a[i].r);
        }
        else
        {
            removed += curR - curL + 1;
            curL = a[i].l;
            curR = a[i].r;
        }
    }
    removed += curR - curL + 1;

    cout << l + 1 - removed << '\n';

    return 0;
}
```

---

## 四、时空复杂度分析

| 解法 | 时间复杂度 | 空间复杂度 | 适用场景 |
| :--- | :--- | :--- | :--- |
| 布尔数组模拟 | $O(l + m \times v)$ — 最坏 $O(l \times m)$ | $O(l)$ | 数据量极小，入门理解 |
| 差分数组 | $O(l + m)$ | $O(l)$ | 通用场景，推荐写法 |
| 区间合并 | $O(m \log m)$ | $O(m)$ | 区间数少、长度大时更优 |

### 详细分析

**解法 1（布尔模拟）**：
- 时间复杂度：初始化 $O(l)$，每个区间最坏遍历整个马路，最坏 $O(l \times m) = 10^6$，仍可接受。
- 空间复杂度：$O(l)$ 布尔数组。

**解法 2（差分数组）**：
- 时间复杂度：区间标记 $O(m)$，前缀和统计 $O(l)$，总复杂度 $O(l + m)$，是本题渐进最优解。
- 空间复杂度：$O(l)$ 的差分数组。

**解法 3（区间合并）**：
- 时间复杂度：排序 $O(m \log m)$，扫描合并 $O(m)$，总复杂度 $O(m \log m)$。当 $l$ 远大于 $m$ 时最省时（不需要遍历整个 $l$）。
- 空间复杂度：$O(m)$ 存储区间。

---

## 五、代码实现（差分数组 — 推荐方案）

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};
int diff[mxn];

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int l, m;
    cin >> l >> m;
    for (int i = 0; i < m; ++i)
    {
        int u, v;
        cin >> u >> v;
        diff[u] += 1;
        diff[v + 1] -= 1;
    }

    int cur = 0, ans = 0;
    for (int i = 0; i <= l; ++i)
    {
        cur += diff[i];
        if (cur == 0)
        {
            ++ans;
        }
    }

    cout << ans << '\n';

    return 0;
}
```

### 代码说明

1. **全局数组** `diff[mxn]`：差分数组，`mxn` 取 $l_{\max} + 5$ 遵循"+5 松弛约定"，防止 $v = l$ 时 $v+1$ 越界。

2. **输入加速**：`cin.tie(nullptr)` 与 `ios::sync_with_stdio(false)` 解除 C++ 标准流与 C 标准 IO 的绑定，加快输入速度。

3. **区间标记**：对每个区间 $[u, v]$，执行 `diff[u] += 1` 和 `diff[v + 1] -= 1$。

4. **前缀和统计**：用变量 `cur` 维护当前位置被覆盖的次数。每步先将 `diff[i]` 累加到 `cur` 上，若 `cur == 0` 说明该位置没有被任何区域覆盖，树保留，答案加 1。

5. **输出**：输出留存树木总数 `ans`，换行用 `'\n'` 而非 `endl`，遵循规范。

### 易错点提醒

- 树的总数是 $l+1$ 而不是 $l$，因为两端 $0$ 和 $l$ 都种有树。
- 区间是闭区间 $[u, v]$，表示 $u$ 和 $v$ 位置的树也要移走。
- 差分数组要开到 $l+2$ 大小（或更大），因为 $v$ 可能等于 $l$，$v+1$ 会访问到 $l+1$。

---

**总结**：本题虽然简单，但三种解法分别体现了"模拟"、"差分"和"区间合并"三种重要思想。推荐使用**差分数组**解法，它代码简洁、效率高，是处理区间覆盖类问题的基础工具。
