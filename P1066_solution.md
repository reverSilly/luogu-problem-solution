# P1066 [NOIP 2006 提高组] 2^k 进制数 — 完整题解

---

## 一、发现叙事

### 1.1 从题目条件出发

题目给出了三个条件：

> (i) $r$ 至少是个 $2$ 位的 $2^k$ 进制数。  
> (ii) 作为 $2^k$ 进制数，除最后一位外，$r$ 的每一位严格小于它右边相邻的那一位。  
> (iii) 将 $r$ 转换为二进制数 $q$ 后，$q$ 的总位数不超过 $w$。

条件 (ii) 告诉我们，$r$ 的数字从左到右**严格递增**。这意味着 $r$ 的所有数字互不相同，且一旦选定了若干个互不相同的数字，它们只能按唯一的方式排列（从小到大）来满足严格递增条件。于是，**计数问题转化为组合选择问题**：从合法的数字池中选出若干个不同的数字，按升序排列成 $r$。

条件 (iii) 限制了 $r$ 的二进制表示的长度。由于每个 $2^k$ 进制位对应 $k$ 个二进制位（最高位可能不足 $k$ 位），这个条件实质上限制了 $r$ 的位数上限以及最高位的取值范围。

条件 (i) 则是一个平凡的下界：数字个数至少为 $2$。

### 1.2 二进制划分与分段

题目给出了一个重要的解释：将 $w$ 位二进制串 $S$ 从右起划分为若干个长度为 $k$ 的段，每段对应一位 $2^k$ 进制数。

设 $w = q \cdot k + h$，其中 $0 \le h < k$：

- 当 $h = 0$ 时，$S$ 恰好被分为 $q$ 段，每段 $k$ 位。第 $i$ 段（从右数）对应 $r$ 的第 $i$ 位（从低位到高位），取值范围均为 $[0,\;2^k-1]$。
- 当 $h > 0$ 时，$S$ 被分为 $q+1$ 段：最左边一段长度为 $h$（最高有效段），其余 $q$ 段长度为 $k$。最高位的取值范围为 $[0,\;2^h-1]$，其余各位的取值范围为 $[0,\;2^k-1]$。

令 $n = 2^k$ 为进制基数，则每个完整 $k$ 位段对应的数字取值范围为 $[0,\;n-1]$，高位 $h$ 位段对应的取值范围为 $[0,\;2^h-1]$。

### 1.3 基于位数的分类讨论

设 $r$ 共有 $t$ 位（$t \ge 2$），数字从左到右为 $d_1 < d_2 < \dots < d_t$。

**情形一：$t$ 位数字全部落在完整的 $k$ 位段内。**

这等价于 $t \le q$ 当 $h>0$，或 $t \le q$ 当 $h=0$（此时 $q = w/k$）。此时每位数字的取值范围均为 $[0,\;n-1]$，且 $d_1 \ne 0$（否则最高位为 $0$，实际位数不足 $t$）。因此，我们需要从 $\{1,2,\dots,n-1\}$ 中选出 $t$ 个不同的数字，升序排列。方案数为：

$$
\binom{n-1}{t}
$$

**情形二：$h > 0$ 且 $t = q+1$，最高位落在 $h$ 位段内。**

此时 $d_1 \in [1,\;2^h-1]$（$0$ 不能做最高位），$d_2,\dots,d_{q+1} \in [0,\;n-1]$ 且严格递增。对于固定的 $d_1 = i$，需要从 $\{i+1,\dots,n-1\}$ 中选出 $q$ 个不同的数字。方案数为 $\displaystyle \binom{n-1-i}{q}$。

对 $i = 1$ 到 $2^h-1$ 求和：

$$
\sum_{i=1}^{2^h-1} \binom{n-1-i}{q}
$$

### 1.4 组合恒等式的化简

利用**冰球棍恒等式**（Hockey-stick identity）：

$$
\sum_{j=r}^{N} \binom{j}{r} = \binom{N+1}{r+1}
$$

令 $j = n-1-i$，则 $i$ 从 $1$ 到 $2^h-1$ 对应 $j$ 从 $n-2$ 到 $n-2^h$。于是：

$$
\sum_{i=1}^{2^h-1} \binom{n-1-i}{q}
= \sum_{j=n-2^h}^{n-2} \binom{j}{q}
= \sum_{j=q}^{n-2} \binom{j}{q} - \sum_{j=q}^{n-2^h-1} \binom{j}{q}
= \binom{n-1}{q+1} - \binom{n-2^h}{q+1}
$$

（约定 $\binom{a}{b}=0$ 当 $a<b$。）

### 1.5 汇总公式

令：

- $n = 2^k$
- $m = \lfloor w/k \rfloor$
- $h = w \bmod k$

则总答案：

$$
\text{Ans} = 
\begin{cases}
\displaystyle \sum_{t=2}^{m} \binom{n-1}{t}, & h = 0 \\[1.2em]
\displaystyle \sum_{t=2}^{m} \binom{n-1}{t} \;+\; \binom{n-1}{m+1} - \binom{n-2^h}{m+1}, & h > 0
\end{cases}
$$

其中求和上限 $t$ 不会超过 $n-1$，因为 $\binom{n-1}{t}=0$ 当 $t > n-1$。

---

## 二、推理链与严格证明

### 2.1 数字与二进制串的对应关系

**引理 1.** 设 $r$ 是一个 $2^k$ 进制数，其数字为 $d_1 d_2 \dots d_t$（$d_1$ 为最高位）。将每个数字 $d_i$ 写为 $k$ 位二进制（最高位 $d_1$ 可能不足 $k$ 位），拼接后得到二进制串 $q$，则 $|q| = (t-1)\cdot k + \lceil\log_2(d_1+1)\rceil$。

**证明.** 除了最高位 $d_1$ 外，其余 $t-1$ 个数字每个占 $k$ 位二进制。$d_1$ 需要 $\lceil\log_2(d_1+1)\rceil$ 位二进制，当 $d_1$ 来自 $h$ 位段时该值不超过 $h$。$\square$

**推论.** 条件 (iii) 等价于：在 $w = qk + h$ 的划分下，$r$ 最多占据 $q+1$ 个段（当 $h>0$），或 $q$ 个段（当 $h=0$）。$r$ 的数字 $d_1$ 若来自 $h$ 位段，则 $d_1 \le 2^h-1$。

### 2.2 严格递增序列的计数

**引理 2.** 从集合 $\{a, a+1, \dots, b\}$ 中选出 $t$ 个不同元素，按升序排列得到长度为 $t$ 的严格递增序列，方案数为 $\displaystyle \binom{b-a+1}{t}$。

**证明.** 从 $b-a+1$ 个元素中任选 $t$ 个，存在唯一的升序排列方式。$\square$

### 2.3 主定理的证明

**定理.** 设 $n=2^k$, $m=\lfloor w/k\rfloor$, $h = w \bmod k$。满足题目条件的 $r$ 的个数由上述分段公式给出。

**证明.** 根据最高位是否受 $h$ 位段限制，分两种情形。

**情形 A（$h=0$，所有段等长 $k$ 位）。** 此时 $r$ 的数字个数 $t$ 满足 $2 \le t \le m$，且每位数字取值范围为 $[0,n-1]$。由条件 (ii) 及 $d_1 \ne 0$，需选取 $t$ 个互不相同的数字组成严格递增序列，数字选自 $\{1,2,\dots,n-1\}$。由引理 2，方案数为 $\binom{n-1}{t}$。对 $t=2$ 到 $m$ 求和即得。

**情形 B（$h>0$，最高段含 $h$ 位）。** 此时最大可能位数为 $m+1$。

- 当 $t \le m$ 时，$t$ 位数字全部落在完整的 $k$ 位段内，与情形 A 相同，方案数为 $\binom{n-1}{t}$。
- 当 $t = m+1$ 时，最高位 $d_1$ 受限于 $h$ 位段，$d_1 \in [1, 2^h-1]$。剩余 $m$ 位从 $\{d_1+1,\dots,n-1\}$ 中选取。方案数为：

$$
\sum_{i=1}^{2^h-1} \binom{n-1-i}{m}
$$

由冰球棍恒等式化简得 $\binom{n-1}{m+1} - \binom{n-2^h}{m+1}$。

合并两类即证。$\square$

### 2.4 边界情况验证

**边界 1：$w < 2k$。** 此时 $m = 0$ 或 $1$。当 $m=0$ 时（$w<k$），即使 $h>0$，最大位数 $m+1=1$，不满足"至少 2 位"的条件，总和为 $0$。当 $m=1$ 时，只可能有 $t=2$ 的情形（若 $h>0$ 则 $t=2$ 已在 $t\le m$ 中），公式正确退化。

**边界 2：$2^k-1 < m$。** 此时 $n-1$ 较小。当 $t > n-1$ 时 $\binom{n-1}{t}=0$，求和自动终止，无需特殊处理。

---

## 三、规律总结

### 3.1 核心洞察

| 洞察 | 解释 |
|------|------|
| 严格递增 → 组合选择 | 数字互异且唯一排序，计数等价于从合法集合中选子集 |
| 二进制长度约束 → 段划分 | $w = qk + h$ 决定了最高位的取值范围和最大位数 |
| 高位不足 $k$ 位 → 分类讨论 | 最高段不足 $k$ 位时单独枚举，用冰球棍恒等式封闭 |

### 3.2 解题流程图

```
输入 k, w
  │
  ├─ 计算 n = 2^k, m = w/k, h = w%k
  │
  ├─ [h == 0]
  │     Ans = Σ C(n-1, t), t=2..min(m, n-1)
  │
  └─ [h > 0]
        Ans = Σ C(n-1, t), t=2..min(m, n-1)
              + C(n-1, m+1) - C(2^k-2^h, m+1)   [若 m+1 ≤ n-1]
```

### 3.3 关键技巧

- **递推组合数**：$\displaystyle \binom{n}{t} = \binom{n}{t-1} \cdot \frac{n-t+1}{t}$，避免阶乘的重复计算。
- **高精度**：答案不超过 200 位，采用十进制分段高精度（基 $10^9$）实现。
- **提前截断**：当 $t > n-1$ 时组合数为 $0$，循环到 $\min(\text{max\_digits},\;n-1)$ 即可。

---

## 四、正式解法与符号表

### 4.1 符号表

| 符号 | 含义 |
|------|------|
| $k$ | 输入参数，进制为 $2^k$ |
| $w$ | 输入参数，二进制表示最大长度 |
| $n = 2^k$ | 进制基数 |
| $m = \lfloor w/k \rfloor$ | 完整 $k$ 位段的个数 |
| $h = w \bmod k$ | 最高段可能剩余的位数 |
| $n_{\text{full}} = n-1$ | 完整段的数字最大值 |
| $n_{\text{high}} = 2^k - 2^h$ | 冰球棍恒等式中的辅助参数 |
| $\text{mx}$ | 需要计算的最大位数，$\text{mx}=m+(h>0)$ |
| $C(a, b)$ | 二项式系数 $\binom{a}{b}$ |

### 4.2 算法步骤

```
输入 k, w
1. n_full ← 2^k - 1
2. m ← w / k (整数除法)
3. h ← w % k
4. mx ← m + (h > 0 ? 1 : 0)
5. 若 mx > n_full，令 mx ← n_full

6. ans ← 0 (高精度)
7. C ← 1 (高精度，表示 C(n_full, t))
8. 对 t = 1 到 mx：
     C ← C × (n_full - t + 1) ÷ t
     若 t ≥ 2 且 (h=0 或 t ≤ m)：
         ans ← ans + C

9. 如果 h > 0 且 m + 1 ≤ n_full：
     Ch ← 1 (高精度)
     对 t = 1 到 min(m+1, 2^k - 2^h)：
         Ch ← Ch × (2^k - 2^h - t + 1) ÷ t
     ans ← ans + C - Ch

10. 输出 ans
```

### 4.3 复杂度分析

- **时间复杂度**：$O(mx \cdot L)$，其中 $mx \le \min(2^k,\; \lceil w/k\rceil) \le 511$，$L$ 为高精度位数（$\le 25$，因为 $10^{200}$ 约 $25$ 个 $10^9$ 进制位）。计算量极小。
- **空间复杂度**：$O(L)$。

---

## 五、C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;

struct BigInt
{
    vector<int>d;
    static constexpr int B{1000000000};

    BigInt(long long x = 0)
    {
        if(!x)
            d = {0};
        else
            for(; x; x /= B)
                d.push_back(x % B);
    }

    void sh()
    {
        while(d.size() > 1 && !d.back())
            d.pop_back();
    }

    BigInt &operator+=(BigInt const &o)
    {
        int c = 0;
        int n = max(d.size(), o.d.size());
        d.resize(n);
        for(int i = 0; i < n; ++i)
        {
            d[i] += (i < (int)o.d.size() ? o.d[i] : 0) + c;
            c = d[i] >= B;
            if(c)
                d[i] -= B;
        }
        if(c)
            d.push_back(1);
        return *this;
    }

    BigInt &operator-=(BigInt const &o)
    {
        int b = 0;
        for(int i = 0; i < (int)d.size(); ++i)
        {
            d[i] -= (i < (int)o.d.size() ? o.d[i] : 0) + b;
            if(d[i] < 0)
                d[i] += B, b = 1;
            else
                b = 0;
        }
        sh();
        return *this;
    }

    BigInt operator+(BigInt const &o) const
    {
        BigInt r = *this;
        r += o;
        return r;
    }

    BigInt operator-(BigInt const &o) const
    {
        BigInt r = *this;
        r -= o;
        return r;
    }

    BigInt &operator*=(int x)
    {
        long long c = 0;
        for(int i = 0; i < (int)d.size(); ++i)
        {
            c += (long long)d[i] * x;
            d[i] = c % B;
            c /= B;
        }
        if(c)
            d.push_back(c);
        return *this;
    }

    BigInt &operator/=(int x)
    {
        long long c = 0;
        for(int i = (int)d.size() - 1; i >= 0; --i)
        {
            c = c * B + d[i];
            d[i] = c / x;
            c %= x;
        }
        sh();
        return *this;
    }

    void print()
    {
        cout << d.back();
        for(int i = (int)d.size() - 2; i >= 0; --i)
            cout << setw(9) << setfill('0') << d[i];
        cout << '\n';
    }
};

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    int k, w;
    cin >> k >> w;

    int n = (1 << k) - 1;       // n-1, max digit value
    int m = w / k;
    int h = w % k;
    int mx = m + (h > 0);        // max possible digits
    if(mx > n)
        mx = n;

    BigInt C(1);
    BigInt ans;

    for(int t = 1; t <= mx; ++t)
    {
        C *= (n - t + 1);
        C /= t;

        if(t >= 2)
        {
            if(!h || t <= m)
                ans += C;
        }
    }

    if(h > 0 && m + 1 <= n)
    {
        int hn = (1 << k) - (1 << h);  // 2^k - 2^h
        BigInt Ch(1);
        int hmx = m + 1 > hn ? hn : m + 1;
        for(int t = 1; t <= hmx; ++t)
        {
            Ch *= (hn - t + 1);
            Ch /= t;
        }
        ans += C - Ch;
    }

    ans.print();
    return 0;
}
```
