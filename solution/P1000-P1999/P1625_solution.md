# P1625 求和

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

给定 $n,m$（$m>1$，$n>0$，$n+m\le500$），求

$$S=\sum_{i=1}^{n}\frac{1}{i\,(i+1)\cdots(i+m-1)}.$$

把 $S$ 写成最简分数 $X/Y$（$\gcd(X,Y)=1$），分两行输出 $X$ 与 $Y$。

### 输入输出格式

- **输入格式**：一行两个整数 $n,m$。
- **输出格式**：第一行 $X$，第二行 $Y$，要求 $X,Y$ 互质。
- **数据规模**：$50\%$ 数据 $n\le50$；$100\%$ 数据 $n+m\le500$。

### 关键约束分析

1. $n+m\le500$ 意味着每一项的分母是**不超过 $499$ 的 $m$ 个连续整数之积**，通分的公分母会是 $\mathrm{lcm}$ 级别，最坏可达 $10^{1000}$ 以上——必须使用大整数（高精度）。
2. 若直接逐项做分数加法，每步都要对大整数求 $\gcd$ 并约分，$n$ 次约分代价高且实现容易出错。数据范围暗示存在**闭式公式**。
3. $m>1$ 保证 $m-1\ne0$，可以除以 $m-1$；$n>0$ 保证求和非空。
4. 边界：$m=2$ 时公式退化为 $1-1/(n+1)=\frac{n}{n+1}$，可用来手算校验；$n=1$ 时 $S=1/m!$ 附近的形式也可校对。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：先试着写前几项，找望远镜结构

题目本身给了两个提示：$\sum i=\frac{n(n+1)}2$，以及 $\sum_{i=1}^{n-1}\frac{1}{i(i+1)}=1-\frac1n$。第二个正是 $m=2$ 的本问题。它的推导只用到

$$\frac{1}{i(i+1)}=\frac1i-\frac1{i+1},$$

即**把一项拆成相邻两项之差**，求和时中间项两两抵消。

那么 $m>2$ 时能否也拆？自然猜测：

$$\frac{1}{i(i+1)\cdots(i+m-1)}=\frac{1}{m-1}\left(\frac{1}{i\cdots(i+m-2)}-\frac{1}{(i+1)\cdots(i+m-1)}\right).$$

**引理 1（望远镜恒等式）**：对任意 $i\ge1,m\ge2$，

$$\frac{1}{i(i+1)\cdots(i+m-1)}=\frac{1}{m-1}\left(\prod_{j=i}^{i+m-2}\frac1j-\prod_{j=i+1}^{i+m-1}\frac1j\right).$$

*证明*：通分右边括号内两项，公分母为 $i(i+1)\cdots(i+m-1)$：

$$\prod_{j=i}^{i+m-2}\frac1j-\prod_{j=i+1}^{i+m-1}\frac1j=\frac{(i+m-1)-i}{i(i+1)\cdots(i+m-1)}=\frac{m-1}{i(i+1)\cdots(i+m-1)}.$$

两边除以 $m-1$ 即得。$\square$

### 第二步：求和

**引理 2（求和公式）**：设 $A=(m-1)!$，$Q=(n+1)(n+2)\cdots(n+m-1)$，则

$$S=\frac{1}{m-1}\left(\frac1A-\frac1Q\right)=\frac{Q-A}{(m-1)AQ}.$$

*证明*：对 $i$ 从 $1$ 到 $n$ 累加引理 1，右端是望远镜和：

$$\sum_{i=1}^n\left(\prod_{j=i}^{i+m-2}\frac1j-\prod_{j=i+1}^{i+m-1}\frac1j\right)=\prod_{j=1}^{m-1}\frac1j-\prod_{j=n+1}^{n+m-1}\frac1j=\frac1A-\frac1Q,$$

中间的 $\prod_{j=i+1}^{i+m-2}\frac1j$（$2\le i\le n$）逐项抵消，只剩首项与末项。再乘 $\frac1{m-1}$ 并通分即得。$\square$

这一步把 $n$ 项求和变成了两个大整数的减法，$\mathcal{O}(1)$ 次大整数运算。

### 第三步：分子一定是正的

**引理 3（$Q>A$）**：$\dfrac{Q}{A}=\dbinom{n+m-1}{m-1}\ge m\ge2$，故 $Q-A>0$。

*证明*：$Q=\frac{(n+m-1)!}{n!}$，$A=(m-1)!$，于是

$$\frac QA=\frac{(n+m-1)!}{n!\,(m-1)!}=\binom{n+m-1}{m-1}.$$

由 $n\ge1$ 得 $n+m-1\ge m$，而 $\binom{n+m-1}{m-1}\ge\binom{m}{m-1}=m$。$\square$

因此 $X=Q-A$ 直接用"大数减小数"即可，不必处理符号。

### 第四步：约分

$X/Y=\frac{Q-A}{(m-1)AQ}$，令 $g=\gcd(X,Y)$，则 $X/g$ 与 $Y/g$ 互质。

**引理 4（约分后互质）**：若 $g=\gcd(X,Y)$，则 $\gcd(X/g,\,Y/g)=1$。

*证明*：反设存在 $d>1$ 同时整除 $X/g$ 与 $Y/g$，则 $dg$ 同时整除 $X$ 与 $Y$，且 $dg>g$，与 $g$ 是最大公约数矛盾。$\square$

**引理 5（规模）**：$Y=(m-1)AQ\le 500\cdot 500!\cdot 500!<10^{2280}$，即至多约 $254$ 个 $10^9$ 进制位。

*证明*：$A=(m-1)!\le 499!$，$Q$ 是 $m-1$ 个不超过 $499$ 的整数之积 $\le499^{498}<500!$ 的量级，直接放缩 $(m-1)AQ\le499\cdot499!\cdot499!<10^{2280}$（$500!<10^{1135}$）。$\square$

所以必须自己实现大整数：加、减、乘、比较、折半、左移、二进制 $\gcd$、以及**带余除法**（把 $X,Y$ 各除以 $g$）。除法用"逐位二分商"实现：从高位到低位维护余数，对每一位二分出 $0\sim10^9-1$ 中满足 $b\cdot d\le r$ 的最大 $d$，复杂度 $\mathcal{O}(D\cdot30\cdot D)$。

**引理 6（二进制 gcd 正确）**：算法反复使用 $\gcd(a,b)=\gcd(a-b,b)$（$a\ge b$）与 $\gcd(2a,2b)=2\gcd(a,b)$，先削去公共因子 $2$ 的幂 $2^g$，再在奇数上做减法直到一方为 $0$，最后把结果左移 $g$ 位，得到的正是 $\gcd(X,Y)$。

*证明*：两条恒等式都是最大公约数的标准性质（前者因为 $a-b$ 与 $b$ 的公因子集和 $a,b$ 的公因子集相同；后者因为公共因子 $2$ 可以整体提出）。循环中每次至少让两数之一减半或变小，必然终止；终止时一数为 $0$，另一数即为奇部最大公约数，再乘回 $2^g$。$\square$

**定理**：程序输出的 $X/g$ 与 $Y/g$ 就是题目要求的最简分子分母。

*证明*：引理 2 给出 $S$ 的精确分式表示，引理 3 保证分子为正、减法方向正确，引理 4 保证约分后互质，引理 6 保证 $g$ 确实被正确求出，引理 5 保证大整数容量足够。$\square$

---

## Part 3 — 模式总结 (Pattern Summary)

- `explore its property` — 通项可以拆成相邻两项之差，求和是望远镜和。
- `consider easier version` — 题目自带 $m=2$ 的提示，先把它推广到一般 $m$。
- `classic problem solution` — 望远镜求和把 $n$ 项级数化为两个大整数的差。
- `classic problem solution` — 手写 $10^9$ 进制大整数，支持加/减/乘/比较。
- `classic problem solution` — 二进制 gcd 求公约数，避免大整数除法反复做欧几里得。
- `classic problem solution` — 逐位二分商的带余除法，完成约分。

---

## Part 4 — 真正的解法 (The Real Solution)

### 符号表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $n,m$ | `n`, `m` | 输入的两个参数；$m>1$ |
| $A=(m-1)!$ | `A` | 初值 $1$，循环 `i=2..m-1` 乘上去 |
| $Q=(n+1)\cdots(n+m-1)$ | `Bb` | 初值 $1$，循环 `k=n+1..n+m-1` 乘上去（名字避开进制常量 `B`） |
| $X=Q-A$ | `X` | 分子（未约分），恒正 |
| $Y=(m-1)AQ$ | `Y` | 分母（未约分） |
| $g=\gcd(X,Y)$ | `g` | 最大公约数，二进制 gcd 求得 |
| $X/g$ | `Xu` | 约分后的分子，即输出第一行 |
| $Y/g$ | `Yu` | 约分后的分母，即输出第二行 |
| 进制 | `B` | 大整数进制 $10^9$ |
| 数位 | `d` | `vector<int>`，小端存放，空表示 $0$ |
| 余数 | `r` | `divmod` 的输出参数 |

### 算法步骤

1. 读入 $n,m$；用 `mulSmall` 循环累乘得到 $A=(m-1)!$ 与 `Bb`$\,=(n+1)\cdots(n+m-1)$。
2. $X\leftarrow Q-A$（大数减法，`sub` 保证 $a\ge b$）。
3. $Y\leftarrow (m-1)\cdot A\cdot Q$：先 `mul(A,Bb)`，再 `mulSmall(...,m-1)`。
4. $g\leftarrow\text{gcdb}(X,Y)$：先剥去公共的 $2$ 的幂，再在奇数上用减法迭代，最后左移回 $g$ 位。
5. 用 `divmod` 分别求 $X/g$、$Y/g$，按"最高位原样、其余补足 9 位"的格式输出。

### 复杂度分析

- **时间复杂度**：设大整数有 $D\le254$ 个 limb。累乘 $\mathcal{O}(mD)$，一次乘法 $\mathcal{O}(D^2)$，二进制 gcd 约 $\mathcal{O}(D^2)$（每轮 $\mathcal{O}(D)$、轮数 $\mathcal{O}(D)$），带余除法 $\mathcal{O}(30D^2)$。总 $\mathcal{O}(D^2)$ 级别，最坏用例实测 $<10$ ms。
- **空间复杂度**：$\mathcal{O}(D)$ 个 limb，常数个大整数。

### 正确性验证

引理 2 给出精确分式，引理 3 保证分子为正（避免减法借位出错），引理 4 保证输出互质，引理 6 保证 gcd 正确，引理 5 保证容量。大整数乘法中单 limb 乘积 $<10^{18}$，用 `long long` 承接不会溢出；`mulSmall` 的乘数不超过 $499$，更安全。

---

## Part 5 — 示例代码 (Example Code)

```cpp
//
// 题目：P1625 求和
// https://www.luogu.com.cn/problem/P1625
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int B{1000000000};

struct big
{
	vector<int>d;                       // 小端，base=1e9，空表示 0
	big(){}
	big(int x)
	{
		while(x)
		{
			d.push_back(x%B);
			x/=B;
		}
	}
	bool zero()const
	{
		return d.empty();
	}
};

void norm(big&a)
{
	while(!a.d.empty()&&a.d.back()==0)
		a.d.pop_back();
}

int cmp(const big&a,const big&b)
{
	if(a.d.size()!=b.d.size())
		return a.d.size()<b.d.size()?-1:1;
	for(int i=(int)a.d.size()-1;i>=0;--i)
		if(a.d[i]!=b.d[i])
			return a.d[i]<b.d[i]?-1:1;
	return 0;
}

big add(const big&a,const big&b)
{
	big c;
	c.d.assign(max(a.d.size(),b.d.size())+1,0);
	for(size_t i=0;i<a.d.size();++i)
		c.d[i]+=a.d[i];
	for(size_t i=0;i<b.d.size();++i)
		c.d[i]+=b.d[i];
	for(size_t i=0;i+1<c.d.size();++i)
		if(c.d[i]>=B)
		{
			c.d[i]-=B;
			++c.d[i+1];
		}
	norm(c);
	return c;
}

big sub(const big&a,const big&b)        // 保证 a>=b
{
	big c=a;
	for(size_t i=0;i<b.d.size();++i)
		c.d[i]-=b.d[i];
	for(size_t i=0;i+1<c.d.size();++i)
		if(c.d[i]<0)
		{
			c.d[i]+=B;
			--c.d[i+1];
		}
	norm(c);
	return c;
}

big mul(const big&a,const big&b)
{
	big c;
	if(a.zero()||b.zero())
		return c;
	c.d.assign(a.d.size()+b.d.size()+1,0);
	for(size_t i=0;i<a.d.size();++i)
	{
		if(a.d[i]==0)
			continue;
		i64 carry=0;
		for(size_t j=0;j<b.d.size();++j)
		{
			i64 cur=(i64)c.d[i+j]+(i64)a.d[i]*b.d[j]+carry;
			c.d[i+j]=cur%B;
			carry=cur/B;
		}
		size_t k=i+b.d.size();
		while(carry)
		{
			i64 cur=(i64)c.d[k]+carry;
			c.d[k]=cur%B;
			carry=cur/B;
			++k;
		}
	}
	norm(c);
	return c;
}

big mulSmall(const big&a,int x)
{
	big c;
	if(x==0||a.zero())
		return c;
	c.d.assign(a.d.size()+1,0);
	i64 carry=0;
	for(size_t i=0;i<a.d.size();++i)
	{
		i64 cur=(i64)a.d[i]*x+carry;
		c.d[i]=cur%B;
		carry=cur/B;
	}
	c.d[a.d.size()]=carry;
	norm(c);
	return c;
}

void half(big&a)
{
	i64 rem=0;
	for(int i=(int)a.d.size()-1;i>=0;--i)
	{
		i64 cur=rem*B+a.d[i];
		a.d[i]=cur>>1;
		rem=cur&1;
	}
	norm(a);
}

void shl(big&a,int g)
{
	for(int i=0;i<g;++i)
	{
		i64 carry=0;
		for(size_t j=0;j<a.d.size();++j)
		{
			i64 cur=(i64)a.d[j]*2+carry;
			a.d[j]=cur%B;
			carry=cur/B;
		}
		if(carry)
			a.d.push_back(carry);
	}
}

bool even(const big&a)
{
	return a.d.empty()||(a.d[0]&1)==0;
}

big gcdb(big a,big b)                    // 二进制 gcd
{
	if(a.zero())
		return b;
	if(b.zero())
		return a;
	int g=0;
	while(even(a)&&even(b))
	{
		half(a);
		half(b);
		++g;
	}
	while(!a.zero())
	{
		while(even(a))
			half(a);
		while(even(b))
			half(b);
		if(cmp(a,b)>=0)
			a=sub(a,b);
		else
			b=sub(b,a);
	}
	shl(b,g);
	return b;
}

big divmod(const big&a,const big&b,big&r)   // 返回商，r 为余数
{
	big q;
	if(cmp(a,b)<0)
	{
		r=a;
		return q;
	}
	q.d.assign(a.d.size(),0);
	r=big();
	for(int i=(int)a.d.size()-1;i>=0;--i)
	{
		r.d.insert(r.d.begin(),a.d[i]);
		norm(r);
		int lo=0,hi=B-1,best=0;
		while(lo<=hi)
		{
			int mid=(lo+hi)>>1;
			if(cmp(mulSmall(b,mid),r)<=0)
			{
				best=mid;
				lo=mid+1;
			}
			else
				hi=mid-1;
		}
		q.d[i]=best;
		if(best)
			r=sub(r,mulSmall(b,best));
	}
	norm(q);
	return q;
}

string tostr(const big&a)
{
	if(a.zero())
		return "0";
	string s=to_string(a.d.back());
	char buf[16];
	for(int i=(int)a.d.size()-2;i>=0;--i)
	{
		snprintf(buf,sizeof(buf),"%09d",a.d[i]);
		s+=buf;
	}
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	big A(1);                           // A=(m-1)!
	for(int i=2;i<=m-1;++i)
		A=mulSmall(A,i);
	big Bb(1);                          // Bb=(n+1)(n+2)...(n+m-1)
	for(int k=n+1;k<=n+m-1;++k)
		Bb=mulSmall(Bb,k);
	big X=sub(Bb,A);                    // S=(Q-A)/((m-1)AQ)
	big Y=mulSmall(mul(A,Bb),m-1);
	big g=gcdb(X,Y);
	big r;
	big Xu=divmod(X,g,r);
	big Yu=divmod(Y,g,r);
	cout<<tostr(Xu)<<'\n'<<tostr(Yu)<<'\n';
	return 0;
}
```

### 易错点

1. **直接把 $n$ 项分数相加**：每项分母是 $m$ 个数之积，通分后分母规模爆炸，逐项约分极易超时或写错，用闭式公式可一步到位。
2. **忘记乘 $m-1$**：望远镜公式右边的系数是 $\frac1{m-1}$，漏掉会让答案整体放大 $m-1$ 倍。
3. **$A$ 与 $Q$ 混淆**：$A$ 是 $(m-1)!$（从 $1$ 连乘到 $m-1$），$Q$ 是从 $n+1$ 连乘到 $n+m-1$，两者项数相同但起点不同。
4. **`sub` 传反**：`sub(a,b)` 假设 $a\ge b$，本题 $X=Q-A$ 中 $Q>A$（引理 3），但若把顺序写反会得到错误的大数（负借位）。
5. **约分不彻底**：只除一次 $\gcd$ 就输出，必须各除一次 $g$；`gcd` 用二进制算法时要记得最后左移回 $g$ 位。
6. **输出格式**：必须两行，且不能用科学计数或前导零；`tostr` 的非最高位要补足 9 位。

### 本地验证结果

| 测试项 | 覆盖范围 | 结果 |
| :--- | :--- | :--- |
| 题面样例 | 题面未提供样例数据，故自造 | — |
| 手算用例 | $(n,m)=(1,2)\to1/2$，$(2,2)\to2/3$，$(1,3)\to1/6$，$(2,3)\to5/24$ | 全部一致 |
| 随机对拍 | $150$ 组，$n\le8$ 且 $n+m\le11$，对照 `P1625_brute.cpp`（逐项分数累加 + `__int128`），并额外断言 $\gcd(X,Y)=1$ | $150$ 组全部一致 |
| 大数校验 | $(1,499),(2,498),(250,250),(300,200),(123,377),(100,400),(497,3)$ 等 $10$ 组，与 Python `Fraction` 精确结果逐字符比对 | 全部一致，最大分母 $1132$ 位 |
| 极限耗时 | $(1,499)$、$(250,250)$、$(480,20)$ | 均 $<0.01$ s |
