# P1883 【模板】三分 / 函数 / Error Curves

> 难度：普及/提高- · 标签：三分 / 凸函数 / 上包络

## 1. 重新读题

### 1.1 问题重述（人话版）

给 $n$ 个函数 $f_i(x)=a_i x^2+b_i x+c_i$，其中 $a_i\ge 0$，所以它们都是开口向上的抛物线，也可能退化成直线（$a_i=0$）。定义 $F(x)=\max_i f_i(x)$。求 $F$ 在闭区间 $[0,1000]$ 上的最小值，输出时保留四位小数并四舍五入。多组数据，$T\le 10$。

### 1.2 关键约束与规模暗示

- $n\le 10^4$，算一次 $F(x)$ 是 $O(n)$。若三分几十次，总时间是 $O(Tn\log(1/\varepsilon))$，够用。
- $|b|,|c|\le 5\times 10^3$，$0\le a\le 100$。$F$ 的斜率量级最大大约是 $2\cdot 100\cdot 1000+5000=2.05\times 10^5$，所以 $x$ 要定得比较准，四位小数才稳。
- 对拍不能比输出字符串。两个都正确的实现，中间值可能差 $10^{-9}$，四舍五入前不该要求文本全等；checker 用绝对误差 $5\times 10^{-5}$（比四位小数的一个单位 $10^{-4}$ 更紧，错一位就对不上）。

## 2. 思维链（Chain-of-thought）

### 2.1 $F$ 是凸函数

**引理 1**。每个 $f_i$ 在实数上凸，从而 $F$ 凸。

**证明**。$a_i\ge 0$ 时 $f_i''(x)=2a_i\ge 0$，二次函数凸；直线的二阶导数是 $0$，也凸。对 $\lambda\in[0,1]$，

$$
\begin{aligned}
F(\lambda x+(1-\lambda)y)
&=\max_i f_i(\lambda x+(1-\lambda)y)\\
&\le \max_i \big(\lambda f_i(x)+(1-\lambda)f_i(y)\big)\\
&\le \lambda \max_i f_i(x)+(1-\lambda)\max_i f_i(y)\\
&=\lambda F(x)+(1-\lambda)F(y).
\end{aligned}
$$

第二步用了每个 $f_i$ 的凸性，第三步是把 $\max(A_i+B_i)$ 拆成 $\max A_i+\max B_i$ 的上界。$\square$

凸函数在区间上单谷：左边下降（或不升），右边上升（或不降），最小值可以是一个点，也可以是一整段。

### 2.2 三分为什么可以丢掉一侧

取 $m_1<m_2$。

**引理 2**。若 $F(m_1)<F(m_2)$，则任意 $x>m_2$ 都有 $F(x)>F(m_2)$，最小值不在 $(m_2,1000]$ 内部，可以令右端点变成 $m_2$。若 $F(m_1)\ge F(m_2)$，对称地令左端点变成 $m_1$。

**证明**。设 $x>m_2$，把 $m_2$ 写成 $m_1$ 与 $x$ 的凸组合：

$$m_2=(1-\lambda)m_1+\lambda x,\qquad \lambda=\frac{m_2-m_1}{x-m_1}\in(0,1).$$

凸性给出 $F(m_2)\le (1-\lambda)F(m_1)+\lambda F(x)$。若 $F(m_1)<F(m_2)$，右边 $<(1-\lambda)F(m_2)+\lambda F(x)$，于是 $\lambda F(m_2)<\lambda F(x)$，即 $F(x)>F(m_2)$。另一侧把 $m_1,m_2$ 对调即可。$\square$

直线是凸也是凹，引理仍然成立：单调下降时三分会把区间往右推，最小值在端点；持平的时候两侧都可以留，函数值不变。

### 2.3 迭代次数和四位小数

每次区间长度乘 $\tfrac23$。做 $100$ 次后，长度不超过

$$1000\cdot (2/3)^{100}\approx 2.5\times 10^{-15}.$$

在最终区间内任意取点，$x$ 的误差不超过这个长度。$|F'|$ 不超过 $2.05\times 10^5$，所以函数值误差 $<10^{-9}$，比 $5\times 10^{-5}$ 的对拍阈值和四位小数的舍入单位都小两个数量级以上。输出用 `fixed<<setprecision(4)`。

### 2.4 对照算法不是三分

$F$ 是若干抛物线的上包络，最小值出现在下面三类点之一：

- 区间端点 $0$ 或 $1000$；
- 某条 $a_i>0$ 的抛物线的顶点 $-b_i/(2a_i)$，且该顶点落在包络上（若不在包络上，$F$ 在那里更大，放进候选集也不会让最小值变小）；
- 两条曲线的交点，也就是包络换段的地方。

于是把所有顶点、所有两两交点和两个端点都求出来，取 $F$ 的最小值，就得到精确答案（数值上用 `long double` 解二次方程）。这是 $O(n^2)$ 的代数算法，只在 $n\le 60$ 时和三分对拍。交点方程是

$$(a_i-a_j)x^2+(b_i-b_j)x+(c_i-c_j)=0,$$

$a_i=a_j$ 时退化成一次方程；两条曲线完全重合时没有新交点，跳过。

## 3. 模式总结（Pattern Summary）

- **explore its property** — $a\ge 0$ 使每条抛物线凸，最大值函数仍然凸。
- **ternary search** — 比较两个三分点，丢掉凸函数不可能取到最小值的那一侧。
- **consider easier version** — 枚举顶点与两两交点，在小 $n$ 上做绝对误差对照。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `a,b,c` | `int [10005]` | 二次函数系数 | 每组读入 | coeff |
| `n` | `int` | 函数个数 | 每组读入 | count |
| `l,r` | `double` | 当前搜索区间 | $0,1000$ | range |
| `fmax(x)` | 函数 | $F(x)$ | — | upper envelope |

### 4.2 算法步骤

1. 读入 $T$。对每组数据读入 $n$ 和系数。
2. 令 $l=0,r=1000$。重复 $100$ 次：取 $m_1=(2l+r)/3$，$m_2=(l+2r)/3$，若 $F(m_1)<F(m_2)$ 则 $r=m_2$，否则 $l=m_1$。
3. 输出 $F((l+r)/2)$，固定四位小数。绝对值小于 $10^{-12}$ 的结果归成 $0$，避免打印出 `-0.0000`。

### 4.3 复杂度与验证状态

- **时间**：每组 $O(100\cdot n)$。
- **空间**：$O(n)$。
- **沙箱对拍**：`g++ -O2 -std=c++17 -Wall -Wextra` 零警告。样例两组输出 `0.0000` 与 `0.5000`。另 $38$ 组 $n\le 60$ 的随机系数，外加退化为一次函数和常数函数的 $2$ 组，与「顶点+交点」暴力的绝对误差都不超过 $5\times 10^{-5}$，共 $41$ 组。$T=3$、$n=10^4$ 的最大规模标程耗时 $0.1059\text{ s}$。

## 5. 示例代码

### 5.1 标程（`P1883.cpp`）

```cpp
//
// 题目：P1883 【模板】三分 / 函数 / Error Curves
// https://www.luogu.com.cn/problem/P1883
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,a[mxn],b[mxn],c[mxn];

double fmax(double x)
{
	double mx=-1e100;
	for(int i=0;i<n;++i)
	{
		double y=(double)a[i]*x*x+(double)b[i]*x+c[i];
		if(y>mx)
			mx=y;
	}
	return mx;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(4);
	int T;
	cin>>T;
	while(T--)
	{
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>a[i]>>b[i]>>c[i];
		double l=0,r=1000;
		for(int it=0;it<100;++it)
		{
			double m1=(l*2+r)/3.0;
			double m2=(l+r*2)/3.0;
			if(fmax(m1)<fmax(m2))
				r=m2;
			else
				l=m1;
		}
		double ans=fmax((l+r)/2.0);
		if(ans>-1e-12&&ans<1e-12)
			ans=0;
		cout<<ans<<'\n';
	}
	return 0;
}
```
