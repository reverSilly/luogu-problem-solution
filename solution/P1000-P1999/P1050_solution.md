# P1050 [NOIP 2005 普及组] 循环 题解

## 1. 题目理解

给定正整数 $n$（$1 \le n < 10^{100}$）和 $k$（$1 \le k \le 100$），求 $n$ 的正整数次幂的最后 $k$ 位的最小循环长度 $L$。

**循环的定义**：对于任意正整数 $a$，$n^{a}$ 与 $n^{a+L}$ 的最后 $k$ 位相同。如果这样的 $L$ 不存在，输出 $-1$。

### 关键观察

- $n$ 的数据范围极大（100 位十进制数），无法用 C++ 内置整数类型存储，必须手写高精度。
- $k \le 100$，意味着我们自始至终只需要关注每个数的最后 $k$ 位。
- 循环长度本身也可能极大（可能超过 $10^{100}$），需要高精度存储。

### 核心难点

- 如何高效确定后 $k$ 位的循环长度？
- 如何处理 $n$ 与 $10$ 不互质（即 $n$ 含有因子 $2$ 或 $5$）时可能出现的"循环不存在"的情况？

---

## 2. 算法思路

### 2.1 逐位递推思想

定义 $L_i$ 为最后 $i$ 位（即模 $10^i$ 意义下）的循环长度。核心思想是**从低位到高位逐位确定**：

1. 先求最后 $1$ 位的循环长度 $L_1$。
2. 已知最后 $i-1$ 位的循环长度 $L_{i-1}$，利用它来确定最后 $i$ 位的循环长度 $L_i$。
3. 由于 $L_i$ 必然是 $L_{i-1}$ 的整数倍（否则最后 $i-1$ 位的循环会被破坏），设 $L_i = L_{i-1} \times t$，只需找出最小的 $t$。

### 2.2 数学推导

设当前已得到 $L = L_{i-1}$（最后 $i-1$ 位的循环长度），现在要求最后 $i$ 位的 $t$。

令模数 $m = 10^i$，令 $\mathrm{base} = n^{L} \bmod m$。考虑 $n$ 的第 $a$ 次幂与第 $a + tL$ 次幂：

$$
n^{a + tL} = n^{a} \times (n^{L})^{t} = n^{a} \times \mathrm{base}^{t} \pmod{m}
$$

我们需要对任意 $a \ge 1$ 有 $n^{a + tL} \equiv n^{a} \pmod{m}$，即：

$$
n^{a} \times (\mathrm{base}^{t} - 1) \equiv 0 \pmod{m} \quad \forall a \ge 1
$$

由于 $n^{a}$ 中 $2$ 和 $5$ 的幂次随 $a$ 增大而单调不减，最难满足的条件是 $a = 1$ 的情形。因此只需检查：

$$
n \times (\mathrm{base}^{t} - 1) \equiv 0 \pmod{10^{i}}
$$

### 2.3 $t$ 的上界

当 $i \ge 2$ 时，根据上一步的循环性质，$\mathrm{base} \equiv 1 \pmod{10^{i-1}}$。设：

$$
\mathrm{base} = 1 + c \cdot 10^{i-1} \pmod{10^{i}} \quad (0 \le c \le 9)
$$

由二项式展开：

$$
\mathrm{base}^{t} = (1 + c \cdot 10^{i-1})^{t} \equiv 1 + t \cdot c \cdot 10^{i-1} \pmod{10^{i}}
$$

（因为 $2(i-1) \ge i$ 对 $i \ge 2$ 成立，高次项均被模数消去。）

代入条件 $n \times (\mathrm{base}^{t} - 1) \equiv 0 \pmod{10^{i}}$ 得：

$$
n \times t \times c \times 10^{i-1} \equiv 0 \pmod{10^{i}}
\iff n \times t \times c \equiv 0 \pmod{10}
$$

因此只需在 $t = 1, 2, \dots, 10$ 中搜索最小的 $t$ 满足 $10 \mid n \times c \times t$。如果 $c = 0$（即 $\mathrm{base} \equiv 1 \pmod{10^{i}}$），则 $t = 1$；如果 $c \neq 0$，则 $t \le 10$ 一定存在或不存在（不存在时说明循环不存在，输出 $-1$）。

当 $i = 1$（最后 $1$ 位）时，可以直接枚举 $t$ 找到循环（最大不超过 $4$）。

### 2.4 算法流程

```
1. 读入字符串 s 和整数 k
2. 构造 n 的最后 k 位 BigInt
3. ans = 1               // 当前循环长度 L
4. pow_n = n             // n^ans，始终保持最后 k 位
5. for pos = 0 to k-1:
       mod = 10^(pos+1)  // 当前要确定的模数
       base = pow_n % mod
       cur = n % mod
       
       t = 1, 2, ... 搜索到最大次数上限（pos=0 时为 4，否则为 10）
            cur = cur * base % mod
            if cur == n % mod:
                找到 t，更新 ans *= t，pow_n = pow_n^t % 10^k
                break
       
       若未找到：输出 -1 并结束
6. 输出 ans
```

---

## 3. 高精度实现

需要实现两种高精度运算：

### 3.1 数字表示（BigInt）

用 `vector<int>` 存储数字，**低位在前**（Least Significant Digit first）。例如整数 $12345$ 存储为 `{5, 4, 3, 2, 1}`。这样对于截断最后 $p$ 位的操作，只需保留前 $p$ 个元素。

所有 BigInt 始终只保留最后 $k$ 位数字（高位置 $0$）。

### 3.2 乘法（截断至 $m$ 位）

模拟竖式乘法，仅计算前 $m$ 位的结果：

```text
for i in [0, m):
    if a[i] == 0: continue
    for j in [0, m-i):
        res[i+j] += a[i] * b[j]
处理进位
截断到 m 位
```

### 3.3 快速幂（模 $10^m$）

标准二进制快速幂，利用上述截断乘法。由于指数 $t \le 10$，也可以直接用乘法循环。

### 3.4 答案的存储

$ans$ 同样用 BigInt 存储（低位在前），每次乘以 $t$（一个一位数），直接模拟竖式乘法即可。

### 3.5 答案的输出

从最高位向最低位输出，去除前导零。

---

## 4. 代码实现

```cpp
// https://www.luogu.com.cn/problem/P1050

#include<bits/stdc++.h>
using namespace std;

struct BigInt
{
    vector<int> d; // LSD first

    BigInt() {}

    BigInt(int x)
    {
        do
        {
            d.push_back(x % 10);
            x /= 10;
        }
        while(x);
    }

    BigInt(const string &s, int k)
    {
        int l = (int)s.size();
        for(int i = l - 1; i >= 0 && l - 1 - i < k; --i)
            d.push_back(s[i] - '0');
        while((int)d.size() < k)
            d.push_back(0);
    }
};

// a * b, keep only last m digits
BigInt mul(const BigInt &a, const BigInt &b, int m)
{
    BigInt r;
    r.d.assign(m, 0);
    int na = min(m, (int)a.d.size());
    int nb = min(m, (int)b.d.size());
    for(int i = 0; i < na; ++i)
    {
        if(!a.d[i])
            continue;
        for(int j = 0; j < nb && i + j < m; ++j)
            r.d[i + j] += a.d[i] * b.d[j];
    }
    int carry = 0;
    for(int i = 0; i < m; ++i)
    {
        r.d[i] += carry;
        carry = r.d[i] / 10;
        r.d[i] %= 10;
    }
    return r;
}

// a^e mod 10^m
BigInt qpow(BigInt a, int e, int m)
{
    BigInt r;
    r.d.assign(m, 0);
    r.d[0] = 1;
    while(e)
    {
        if(e & 1)
            r = mul(r, a, m);
        a = mul(a, a, m);
        e >>= 1;
    }
    return r;
}

// compare two BigInts (same length)
bool equal(const BigInt &a, const BigInt &b)
{
    return a.d == b.d;
}

// BigInt *= small int (0-9)
void mul_small(BigInt &a, int x)
{
    if(!x)
    {
        a.d.assign(1, 0);
        return;
    }
    int carry = 0;
    for(auto &v : a.d)
    {
        v = v * x + carry;
        carry = v / 10;
        v %= 10;
    }
    while(carry)
    {
        a.d.push_back(carry % 10);
        carry /= 10;
    }
}

// print BigInt (most significant digit first, no leading zeros)
void print(const BigInt &a)
{
    int i = (int)a.d.size() - 1;
    while(i > 0 && !a.d[i])
        --i;
    for(; i >= 0; --i)
        cout << a.d[i];
}

int main()
{
    cin.tie(nullptr);
    ios::sync_with_stdio(false);

    string s;
    int k;
    cin >> s >> k;

    BigInt n(s, k);   // last k digits of n
    BigInt ans(1);    // current cycle length L
    BigInt cur_pow = n; // n^L mod 10^k

    for(int pos = 0; pos < k; ++pos)
    {
        int mod_len = pos + 1; // keep mod_len digits

        // base = n^L mod 10^{mod_len}
        BigInt base;
        base.d.assign(cur_pow.d.begin(), cur_pow.d.begin() + mod_len);

        // n_mod = n mod 10^{mod_len}
        BigInt n_mod;
        n_mod.d.assign(n.d.begin(), n.d.begin() + mod_len);

        int limit = (pos == 0) ? 4 : 10;
        int t = -1;

        BigInt cur = n_mod;
        for(int tt = 1; tt <= limit; ++tt)
        {
            cur = mul(cur, base, mod_len);
            if(equal(cur, n_mod))
            {
                t = tt;
                break;
            }
        }

        if(t == -1)
        {
            cout << -1 << '\n';
            return 0;
        }

        // ans = ans * t
        mul_small(ans, t);

        // cur_pow = cur_pow^t mod 10^k
        cur_pow = qpow(cur_pow, t, k);
    }

    print(ans);
    cout << '\n';
    return 0;
}
```

---

## 5. 复杂度分析

### 时间复杂度

外层循环 $k$ 次（$k \le 100$）。每次循环中：

- $t$ 的搜索次数不超过 $10$。
- 每次搜索需要一次 $O(k^2)$ 的高精度乘法（两个长度不超过 $k$ 的数相乘）。
- 循环结束时需要一次高精度快速幂 $O(k^2 \log t) = O(k^2)$。
- $ans$ 乘以 $t$ 是 $O(k)$。

因此总时间复杂度为 $O(k \times 10 \times k^2) = O(10k^3) \approx O(k^3)$。$k \le 100$ 时运行极快。

### 空间复杂度

存储几个 BigInt，每个长度不超过 $k$，空间 $O(k)$。

### 正确性说明

- **数学基础**：利用二项式展开证明每一步 $t \le 10$，保证了枚举算法的终止性与正确性。
- **循环存在性**：当某步 $t$ 在 $10$ 次内未找到时，循环一定不存在，输出 $-1$。
- **高精度运算**：所有运算仅保留最后 $k$ 位，既避免了高精度复杂度爆炸，又完全符合问题要求。
