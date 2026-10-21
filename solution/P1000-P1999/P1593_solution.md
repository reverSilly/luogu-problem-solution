# P1593 因子和 题解

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

给定两个整数 $a$ 和 $b$，求 $a^b$ 的所有正约数之和 $\sigma_1(a^b)$ 对 $9901$ 取模的结果。

### 输入输出格式与数据规模

- **输入格式**：一行两个整数 $a, b$。
- **输出格式**：一行一个整数，表示 $a^b$ 的因子和模 $9901$ 的值。
- **数据规模**：$1 \le a \le 5 \times 10^7$，$0 \le b \le 5 \times 10^7$。

### 关键约束分析

1. 模数 $9901$ 是一个**素数**（因为 $\sqrt{9901} \approx 99.5$，经检验不被 $2 \dots 97$ 整除）。
2. 当求等比数列和 $\frac{p^{k+1} - 1}{p - 1} \bmod 9901$ 时，若 $p - 1$ 是 $9901$ 的倍数（即 $p \equiv 1 \pmod{9901}$），分母模 $9901$ 为 $0$，不存在乘法逆元，必须单独处理！

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：积性函数分解与等比数列求和

将 $a$ 进行标准质因数分解：
$$a = p_1^{e_1} p_2^{e_2} \dots p_m^{e_m}$$
则 $a^b = p_1^{b e_1} p_2^{b e_2} \dots p_m^{b e_m}$。
由约数和函数的积性，有：
$$\sigma_1(a^b) = \prod_{i=1}^m \left(1 + p_i + p_i^2 + \dots + p_i^{b e_i}\right)$$

### 第二步：模素数 $9901$ 下的三种余数情形

对于单个质因子 $p$ 及其指数 $k = b \cdot e$，令 $r = p \bmod 9901$。我们要计算 $G(p, k) = \sum_{j=0}^k p^j \bmod 9901$：

**引理 1（模素数等比数列求和的三分支完备公式）**：
设模数 $M = 9901$ 为素数，$r = p \bmod M$，则：
1. 若 $r = 0$（即 $9901 \mid p$）：此时 $p^1, p^2, \dots, p^k$ 均为 $9901$ 的倍数，故 $G(p, k) \equiv 1 \pmod{9901}$；
2. 若 $r = 1$（即 $p \equiv 1 \pmod{9901}$）：此时每一项 $p^j \equiv 1^j = 1 \pmod{9901}$，共 $k + 1$ 项相加，故 $G(p, k) \equiv (k + 1) \bmod{9901}$；
3. 若 $2 \le r < 9901$：此时 $\gcd(r - 1, 9901) = 1$，由费马小定理，$(r - 1)$ 在模 $9901$ 下的乘法逆元为 $(r - 1)^{9901 - 2} \bmod 9901$，故：
   $$G(p, k) \equiv (r^{k+1} - 1) \cdot (r - 1)^{9899} \pmod{9901}$$

**证明**：
三种情形完整覆盖了 $r \in [0, 9900]$ 的所有余数类，且在第三种情形中 $1 \le r - 1 \le 9899$，由于 $9901$ 是素数，$\gcd(r - 1, 9901) = 1$ 恒成立，费马小定理求逆元完全合法。证毕。

---

## Part 3 — 算法流程与复杂度分析 (Algorithm & Complexity)

### 算法步骤

1. 读入 $a, b$。特判若 $a == 0$ 输出 $0$；若 $b == 0$（此时 $a^0 = 1$）输出 $1$。
2. 试除法枚举 $i = 2 \dots \lfloor\sqrt{a}\rfloor$ 分解 $a$ 的质因子，统计每个质因子 $i$ 的次数 $\text{cnt}$，将 `geom_sum(i, cnt * b)` 乘入答案。
3. 若分解结束后 $a > 1$，说明剩余的 $a$ 是一个大于 $\sqrt{a}$ 的质因子，将 `geom_sum(a, b)` 乘入答案。
4. 输出最终答案。

### 复杂度分析

- **时间复杂度**：试除法分解质因数耗时 $\mathcal{O}(\sqrt{a})$，不同质因子个数不超过 $9$ 个，每个质因子快速幂耗时 $\mathcal{O}(\log(b) + \log(\text{mod}))$。总时间复杂度为 $\mathcal{O}(\sqrt{a})$，当 $a = 5 \times 10^7$ 时仅需约 $7000$ 次循环，耗时 `< 0.001 s`。
- **空间复杂度**：$\mathcal{O}(1)$。

---

## Part 4 — 代码实现 (Code)

```cpp
//
// 题目：P1593 因子和
// https://www.luogu.com.cn/problem/P1593
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr long long mod{9901};

long long qpow(long long a,long long b)
{
	a%=mod;
	long long res=1;
	while(b>0)
	{
		if(b&1)
			res=res*a%mod;
		a=a*a%mod;
		b>>=1;
	}
	return res;
}

// 计算等比数列 1 + p + p^2 + ... + p^k (mod 9901)
long long geom_sum(long long p,long long k)
{
	p%=mod;
	if(p==0)
		return 1;
	if(p==1)
		return (k+1)%mod;
	long long num=(qpow(p,k+1)-1+mod)%mod;
	long long inv=qpow(p-1,mod-2);
	return num*inv%mod;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	long long a,b;
	if(!(cin>>a>>b))
		return 0;
	if(a==0)
	{
		cout<<0<<'\n';
		return 0;
	}
	if(b==0)
	{
		cout<<1<<'\n';
		return 0;
	}

	long long ans=1;
	for(long long i=2;i*i<=a;++i)
		if(a%i==0)
		{
			long long cnt=0;
			while(a%i==0)
			{
				++cnt;
				a/=i;
			}
			ans=ans*geom_sum(i,cnt*b)%mod;
		}
	if(a>1)
		ans=ans*geom_sum(a,b)%mod;

	cout<<ans<<'\n';
	return 0;
}
```

### 符号对照表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $a, b$ | `a`, `b` | 底数与指数 |
| $p, k$ | `p`, `k` | 当前质因子及其在 $a^b$ 中的总指数 $b \cdot e_i$ |
| $G(p, k)$ | `geom_sum(p, k)` | $\sum_{j=0}^k p^j \bmod 9901$ |

---

## Part 5 — 易错点与总结 (Pitfalls & Takeaway)

### 常见易错点

1. **漏判 $p \equiv 1 \pmod{9901}$**：当输入的 $a$ 含有质因子 $p$ 满足 $p \bmod 9901 = 1$（例如素数 $p = 19803$ 或 $p = 9901k + 1$）时，`p - 1` 模 $9901$ 为 $0$，此时 `qpow(p - 1, mod - 2)` 返回 $0$，导致结果错误变成 $0$！必须特判 `if (p == 1) return (k + 1) % mod;`。
2. **漏判 $b = 0$**：数据范围允许 $b = 0$，此时 $a^0 = 1$，因子和为 $1$。

### 本地对拍与验证结果

| 测试项 | 覆盖范围与规模 | 结果 |
| :--- | :--- | :--- |
| 边界与模退化测试 | $(2,3), (1,0), (9901,5), (9902,100), (19803,50)$ | 全部通过 |
| 随机压力对拍 | $300$ 组满范围随机 $(a, b)$ vs `P1593_brute.cpp`（无逆元分治等比求和） | 全部一致 |

### 方法归纳 (Pattern Summary)

- **触发信号**：求幂次 $a^b$ 的约数和 $\sigma_1(a^b) \bmod M$。
- **核心套路**：分解质因数转化为等比数列求和乘积；若使用求逆元公式，务必特判公比 $p \equiv 1 \pmod M$ 时退化为 $(k + 1) \bmod M$（或改用 $\mathcal{O}(\log k)$ 二分分治求和以绕开逆元）。
