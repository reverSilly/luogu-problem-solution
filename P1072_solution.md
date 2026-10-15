# P1072 [NOIP 2009 提高组] Hankson 的趣味题 题解

---

## 1. 重新读题 — 简化题意

给定四个正整数 $a_0,a_1,b_0,b_1$，求有多少个正整数 $x$ 满足：

$$
\begin{cases}
\gcd(x,a_0)=a_1\\
\operatorname{lcm}(x,b_0)=b_1
\end{cases}
$$

数据范围：$a_0,a_1,b_0,b_1\le 2\times10^9$，$n\le 2000$。输入保证 $a_1\mid a_0$ 且 $b_0\mid b_1$。

---

## 2. 思路链 — 发现与证明

### 2.1 从必要条件出发

拿到这道题，先看两个条件给 $x$ 施加了什么限制。

条件一 $\gcd(x,a_0)=a_1$：最大公约数是 $a_1$，说明 $a_1$ 整除 $x$，否则最大公约数不可能达到 $a_1$。所以

$$
a_1\mid x. \tag{1}
$$

条件二 $\operatorname{lcm}(x,b_0)=b_1$：最小公倍数是 $b_1$，说明 $x$ 整除 $b_1$，否则最小公倍数不可能只有 $b_1$。所以

$$
x\mid b_1. \tag{2}
$$

由 (1)(2) 得 $a_1\mid x\mid b_1$，于是 $a_1\mid b_1$ 是必要条件——如果 $b_1$ 不是 $a_1$ 的倍数，答案直接为 $0$。

### 2.2 枚举法的思路

既然 $x$ 是 $b_1$ 的约数，最朴素的想法就是枚举 $b_1$ 的所有约数，逐个验证两个条件。$b_1\le 2\times10^9$，其约数个数上限约为 $1536$（$2\times10^9$ 以内约数最多的数是 $1\,963\,980\,000$，有 $1536$ 个约数），枚举所有约数的代价很小。

关键问题：如何高效枚举 $b_1$ 的所有约数？

标准做法：从 $1$ 到 $\lfloor\sqrt{b_1}\rfloor$ 枚举 $i$，若 $i\mid b_1$，则 $i$ 和 $b_1/i$ 都是 $b_1$ 的约数。$ \sqrt{2\times10^9}\approx 44721$，枚举代价可接受。

### 2.3 正确性验证

对于枚举出的每个 $d$（$d\mid b_1$）：

- 先检查 $a_1\mid d$（必要条件 (1) 的快速剪枝）；
- 再验证 $\gcd(d,a_0)=a_1$ 和 $\operatorname{lcm}(d,b_0)=b_1$。

只需证：枚举 $b_1$ 的所有约数不会漏解。

**证明**：由条件二得 $x\mid b_1$（已证），所以所有合法的 $x$ 都是 $b_1$ 的约数。枚举 $b_1$ 的全部约数并验证条件，必然覆盖所有解。$\square$

### 2.4 复杂度分析

- 外层枚举 $\sqrt{b_1}$ 次，约 $4.5\times10^4$ 次迭代；
- 每次迭代做一次取模判断是否为约数（$O(1)$）；
- 对每个约数做 $\gcd$ 和 $\operatorname{lcm}$ 计算，单次 $O(\log b_1)$；
- 每组数据总操作数约 $O(\sqrt{b_1}\cdot\log b_1)$；
- $n\le 2000$，最坏情况总运算约 $2\,000\times 4.5\times10^4 \approx 9\times10^7$ 次，C++ 能轻松通过。

---

## 3. 模式总结

- **性质**：$\gcd$ 条件推出 $a_1\mid x$；$\operatorname{lcm}$ 条件推出 $x\mid b_1$。
- **手法**：枚举 $b_1$ 的所有约数，用 $a_1\mid d$ 快速剪枝，再暴力验证 $\gcd$ 和 $\operatorname{lcm}$ 条件。
- **要点**：验证 $\operatorname{lcm}$ 时用公式 $\operatorname{lcm}(x,b_0)=x/\gcd(x,b_0)\cdot b_0$，注意开 `long long` 防溢出。

---

## 4. 真正的解法

先定义所有符号：

| 符号 | 含义 |
|------|------|
| $a_0,a_1,b_0,b_1$ | 输入的四元组 |
| $n$ | 数据组数 |
| $x$ | 待求的未知正整数 |
| $d$ | $b_1$ 的一个约数，作为候选 $x$ |
| $\gcd$ | 最大公约数 |
| $\operatorname{lcm}$ | 最小公倍数 |
| $\mathrm{ans}$ | 每组数据的答案 |

算法步骤：

1. 读入 $n$。
2. 对每组数据：
   1. 读入 $a_0,a_1,b_0,b_1$。
   2. 若 $b_1\bmod a_1\neq 0$，说明 $a_1\nmid b_1$，无解，输出 $0$ 并进入下一组。
   3. 令 $\mathrm{ans}\gets 0$。
   4. 枚举 $i$ 从 $1$ 到 $\lfloor\sqrt{b_1}\rfloor$：
      - 若 $b_1\bmod i\neq 0$，跳过。
      - 令 $d\gets i$。若 $d\bmod a_1=0$ 且 $\gcd(d,a_0)=a_1$ 且 $\operatorname{lcm}(d,b_0)=b_1$，则 $\mathrm{ans}\gets\mathrm{ans}+1$。
      - 令 $d'\gets b_1 / i$。若 $d'\neq d$ 且 $d'\bmod a_1=0$ 且 $\gcd(d',a_0)=a_1$ 且 $\operatorname{lcm}(d',b_0)=b_1$，则 $\mathrm{ans}\gets\mathrm{ans}+1$。
   5. 输出 $\mathrm{ans}$。

其中 $\operatorname{lcm}(x,b_0)$ 的计算使用公式：

```cpp
auto lcm = [](i64 a, i64 b) { return a / gcd(a, b) * b; };
```

**时间复杂度**：$O(n\sqrt{b_1}\log b_1)$。
**空间复杂度**：$O(1)$。

**验证**：通过 Luogu P1072 评测（已确认可通过所有测试点）。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 gcd(i64 a,i64 b)
{
    for(;b;swap(a,b),b%=a);
    return a;
}

i64 lcm(i64 a,i64 b)
{
    return a/gcd(a,b)*b;
}

int main()
{
    cin.tie(nullptr);
    cout.tie(nullptr);
    ios::sync_with_stdio(false);

    int n;
    cin>>n;
    while(n--)
    {
        int a0,a1,b0,b1;
        cin>>a0>>a1>>b0>>b1;

        if(b1%a1)
        {
            cout<<"0\n";
            continue;
        }

        int ans=0;
        for(int i=1;i*i<=b1;++i)
        {
            if(b1%i)
                continue;

            if(i%a1==0&&gcd(i,a0)==a1&&lcm(i,b0)==b1)
                ++ans;

            int j=b1/i;
            if(j!=i&&j%a1==0&&gcd(j,a0)==a1&&lcm(j,b0)==b1)
                ++ans;
        }
        cout<<ans<<'\n';
    }
    return 0;
}
```
