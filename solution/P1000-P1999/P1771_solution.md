# P1771 方程的解

## 1. 重新读题

给定两个正整数 $k, x$（$2 \le k \le 100$，$x \le 2^{31}-1$），令 $g(x) = x^x \bmod 1000$（保证 $k \le g(x)$）。求不定方程
$$a_1 + a_2 + \dots + a_k = g(x)$$
的**正整数解** $(a_1, a_2, \dots, a_k)$ 的组数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：快速幂求 $g(x)$ 与隔板法组合数

第一步：用二进制快速幂在 $O(\log x)$ 时间内算出 $n = g(x) = x^x \bmod 1000$。

第二步：求 $a_1 + a_2 + \dots + a_k = n$（$a_i \ge 1$）的正整数解个数。

**引理 1（隔板法公式）**：不定方程 $a_1 + a_2 + \dots + a_k = n$ 的正整数解组数等于 $\binom{n - 1}{k - 1}$。

**证明**：将 $n$ 表示为排成一行的 $n$ 个小球，相邻小球之间共有 $n - 1$ 个空隙。将这 $n$ 个小球分成 $k$ 个非空连续段，等价于在 $n - 1$ 个空隙中任选 $k - 1$ 个空隙插入隔板，两部分之间构成双射，故方案数为 $\binom{n - 1}{k - 1}$。$\blacksquare$

由于 $n < 1000, k \le 100$，$\binom{999}{99}$ 最大可达约 $10^{140}$，远超 64 位整数上限，因此用高精度整数（顺次乘以 $n - i$ 再除以 $i$）即可算出精确值。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 先用快速幂算 $n = x^x \bmod 1000$，再用隔板法将方程正整数解组数化为 $\binom{n-1}{k-1}$（引理 1）。
- **classic problem solution** — 使用压位高精度整数递推执行 $\binom{n-1}{i} = \binom{n-1}{i-1} \cdot (n - i) / i$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `k, x` | `i64` | 变量个数 $k$ 与参数 $x$ | 标准输入 | 输入参数 |
| `n` | `int` | $g(x) = x^x \bmod 1000$ | 快速幂计算 | RHS sum |
| `a` | `vector<int>` | 基数 $10^4$ 的高精度整数（低位在前） | `{1}` | BigInt |

### 4.2 算法步骤

1. 读入 $k, x$，用快速幂计算 `n = qpow(x % 1000, x, 1000)`。
2. 初始化高精度整数 `a = {1}`。
3. 循环 $i = 1 \dots k - 1$：将 `a` 乘以 $(n - i)$，再将 `a` 除以 $i$。
4. 从高位到低位输出高精度整数 `a`。

### 4.3 复杂度分析

- **时间复杂度**：快速幂 $O(\log x)$，组合数递推 $k$ 步，高精度位数不超过 $40$ 个万进制位，总时间复杂度为 $O(\log x + k^2)$，耗时 $< 1\text{ ms}$。
- **空间复杂度**：长度为 $40$ 的 `vector<int>`，空间复杂度为 $O(k)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1771 方程的解
// https://www.luogu.com.cn/problem/P1771
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 qpow(i64 a,i64 b,i64 m)
{
	i64 r=1%m;
	a%=m;
	for(;b;b>>=1,a=a*a%m)
		if(b&1)
			r=r*a%m;
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 k,x;
	cin>>k>>x;
	int n=(int)qpow(x,x,1000);

	vector<int>a={1};
	for(int i=1;i<k;++i)
	{
		int mul=n-i,carry=0;
		for(int&d:a)
		{
			int cur=d*mul+carry;
			d=cur%10000;
			carry=cur/10000;
		}
		while(carry)
		{
			a.push_back(carry%10000);
			carry/=10000;
		}
		int rem=0;
		for(int j=(int)a.size()-1;j>=0;--j)
		{
			int cur=rem*10000+a[j];
			a[j]=cur/i;
			rem=cur%i;
		}
		while(a.size()>1&&a.back()==0)
			a.pop_back();
	}

	cout<<a.back();
	for(int j=(int)a.size()-2;j>=0;--j)
		cout<<setw(4)<<setfill('0')<<a[j];
	cout<<'\n';
	return 0;
}
```
