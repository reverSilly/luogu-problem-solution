# P1945 无边的网格

## 1. 重新读题

### 1.1 问题重述（人话版）

一个 $R$ 行 $C$ 列的方格表，有 $(R+1)(C+1)$ 个格点（整点）。求：

1. 四个顶点都是格点的**正方形**个数 $ans_1$；
2. 三个顶点都是格点的**正三角形**个数 $ans_2$。

输出 $ans_1$ 与 $ans_2$。

### 1.2 关键约束与规模暗示

- 30%：$R,C\le 50$，可以枚举 $O(R^2C^2)$ 甚至 $O((RC)^2)$。
- 50%：$R,C\le10^3$，可以 $O(\min(R,C))$ 求和。
- 70%：$R,C\le10^5$，且答案 $<2^{63}$，可以用 `long long` 推公式。
- 90%：$R,C\le10^{100}$；100%：$R,C\le10^{1000}$。**必须用高精度**，且 $R,C$ 大到不能用任何定长整数读入。
- 数据范围里专门写了 $ans_1,ans_2<2^{63}$ 只到 $10^5$ 那一档，说明更大范围下答案会超出 64 位，必须大整数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先看三角形：一个数论上的「不可能」

**引理 1**：不存在三个顶点都在整点上的非退化正三角形，故 $ans_2=0$ 恒成立。

**证明**：设正三角形三个顶点为整点。用鞋带公式，其面积
$$
S=\frac12\left|x_1(y_2-y_3)+x_2(y_3-y_1)+x_3(y_1-y_2)\right|
$$
是 $\frac12$ 的整数倍（因为坐标全是整数，括号内是整数）。

另一方面，设边长为 $a$。两个整点间距离的平方 $a^2=(\Delta x)^2+(\Delta y)^2$ 是**整数**，所以正三角形面积又等于
$$
S=\frac{\sqrt3}{4}a^2.
$$
若 $a>0$，则 $S$ 是无理数（$\sqrt3$ 乘一个非零有理数）。但无理数不可能等于一个整数或半整数，矛盾。因此只能 $a=0$，三点重合，不是正三角形。$\square$

所以后半问永远输出 $0$，真正的工作量全在正方形计数。

### 2.2 把正方形按「外接轴对齐盒子」分类

任何格点正方形都有一个外接的、边与坐标轴平行的最小矩形（下称**外接盒**）。先只看形状，不管位置。

**引理 2（参数化）**：设外接盒是 $s\times s$ 的正方形。则能放进这个盒子、四顶点为格点的正方形恰有 $s$ 种（形状），且它们一一对应于整数对 $(a,b)$，$a\ge1,\ b\ge0,\ a+b=s$。

**证明**：取正方形在外接盒**下边**上的那个顶点（记作 $P$），再沿逆时针方向取下一个顶点（它一定在外接盒的**右边**上，记作 $Q$）。设盒子的左下角为原点、边平行坐标轴，边长为 $s$。因为 $P$ 在下边、$Q$ 在右边，$P=(x_0,0)$，$Q=(s,y_0)$，令
$$
a=s-x_0>0,\qquad b=y_0\ge0.
$$
由正方形边等长且相邻边垂直，把 $\overrightarrow{PQ}=(a,b)$ 旋转 $90^\circ$ 得到 $\overrightarrow{QR}=(-b,a)$，于是另外两个顶点是 $R=(s-b,\ y_0+a)=(a,\ s)$ 与 $P+(-b,a)=(x_0-b,\ a)$。要落回盒内，必须有 $a+b=s$；此时 $x_0=s-a=b$，四个顶点化为 $(b,0),(s,b),(a,s),(0,a)$，分别触碰下、右、上、左四条边，盒子确实是最小的。反过来，任给 $a\ge1,b\ge0,a+b=s$，按上述公式构造出四点。相邻边向量为 $\overrightarrow{PQ}=(a,b)$ 与 $\overrightarrow{QR}=(-b,a)$，有 $|\overrightarrow{PQ}|^2=|\overrightarrow{QR}|^2=a^2+b^2$ 且点积 $a(-b)+ba=0$，即邻边等长且垂直；再依次旋转即知四点构成正方形，坐标全为整数，外接盒恰为 $s\times s$。

唯一性：正方形确定后，$s$ 是外接盒边长（唯一），$P,Q$ 由「下边触顶点、逆时针下一个」唯一确定，于是 $(a,b)$ 唯一。所以这是双射。固定 $s$ 时，$(a,b)$ 的个数就是 $a=1,2,\dots,s$ 共 $s$ 个（对应 $b=s-a\ge0$）。$\square$

（检验：$s=1$ 只有 $(1,0)$，即单位正方形；$s=2$ 有 $(2,0)$ 正放的大正方形和 $(1,1)$ 的斜 45° 菱形，共 $2$ 种；$s=3$ 有 $(3,0),(2,1),(1,2)$，后两种互为镜像，确实是不同的斜正方形。）

### 2.3 加上位置

给定形状参数 $s$，外接盒是 $s\times s$，它可以在 $R\times C$ 的网格中放置的位置数为
$$
(R-s+1)(C-s+1),
$$
前提是 $1\le s\le \min(R,C)$。外接盒位置一旦确定，形状参数 $(a,b)$ 对应的正方形顶点位置也随之确定，无额外自由度。

于是
$$
ans_1=\sum_{s=1}^{\min(R,C)} s\,(R-s+1)(C-s+1).
$$

### 2.4 化简成闭式

令 $R'=\min(R,C)$，$C'=\max(R,C)$。记 $S_1=\sum s$，$S_2=\sum s^2$，$S_3=\sum s^3$（$s=1\dots R'$）。展开
$$
s(R'-s+1)(C'-s+1)
= (R'+1)(C'+1)s-(R'+C'+2)s^2+s^3,
$$
所以
$$
ans_1=(R'+1)(C'+1)\frac{R'(R'+1)}2-(R'+C'+2)\frac{R'(R'+1)(2R'+1)}6+\left(\frac{R'(R'+1)}2\right)^2.
$$

**引理 3（两个除法都整除）**：

- $R'(R'+1)$ 一定被 $2$ 整除（相邻整数一奇一偶）；
- $R'(R'+1)(2R'+1)$ 一定被 $6$ 整除。

**证明**：第一点显然。第二点：被 $2$ 整除同上；被 $3$ 整除看 $R'\bmod3$：$R'\equiv0$ 时因子 $R'$ 被 $3$ 整除；$R'\equiv1$ 时 $2R'+1\equiv0$；$R'\equiv2$ 时 $R'+1\equiv0$。$2,3$ 互质，故被 $6$ 整除。$\square$

因此高精度实现里可以放心做「先乘后除小整数」，不会产生余数。

### 2.5 为什么要高精度、怎么做

$R,C\le10^{1000}$，$ans_1$ 约为 $R^2C^2$ 量级，可能有约 $4000$ 位十进制。C++ 没有内置大整数，需要手写：

- 用 $10^4$ 进制（每「位」存 $0\sim9999$，十进制输出时每段补足 $4$ 位），乘法的中间量 $<2.5\times10^{10}$，`long long` 装得下；
- 只需支持：读入、比较、加、减、乘、除以小整数（$2$ 或 $6$）、输出；
- 减法的结果保证非负（$T_1+T_3\ge T_2$，因为原式是平方和）。

**引理 4**：按上式先算 $T_1,T_2,T_3$ 再求 $T_1-T_2+T_3$ 与直接求和相等。

**证明**：多项式展开与求和是恒等变形，已在上文逐步写出；整除性由引理 3 保证，故大整数运算不引入误差。$\square$

### 2.6 复杂度

设输入有 $D\le1000$ 位十进制，则大整数乘法 $O(D^2)$，整体常数次运算，时间 $O(D^2)$，空间 $O(D)$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 正三角形不可能：面积既是半整数又是 $\sqrt3/4\cdot$整数，矛盾。
- **explore its property** — 正方形按外接盒边长 $s$ 分类，形状有 $s$ 种（由边向量 $(a,b)$ 参数化）。
- **reduce to easier problem** — 位置数 $(R-s+1)(C-s+1)$，化成对 $s$ 求和。
- **classic problem solution** — 用 $\sum s,\sum s^2,\sum s^3$ 得到闭式，只需整数运算。
- **classic problem solution** — 手写 $10^4$ 进制大整数，先乘后除小整数，精确计算巨大答案。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `ra,rb` | `string` | 读入的 $R,C$ | 读入 | raw |
| `R` | `Big` | $\min(R,C)$ | 计算 | 短边 |
| `C` | `Big` | $\max(R,C)$ | 计算 | 长边 |
| `d` | `vector<int>` | 大整数各位（$10^4$ 进制，低位在前） | — | digits |
| `R1,C1` | `Big` | $R+1,\ C+1$ | 计算 | +1 |
| `U` | `Big` | $R(R+1)/2$ | 计算 | 一次和 |
| `T1` | `Big` | $(R+1)(C+1)\cdot U$ | 计算 | 第一项 |
| `T3` | `Big` | $U^2$ | 计算 | 第三项 |
| `W` | `Big` | $R(R+1)(2R+1)/6$ | 计算 | 平方和 |
| `T2` | `Big` | $(R+C+2)\cdot W$ | 计算 | 第二项 |
| `ans` | `Big` | $T_1-T_2+T_3$ | 计算 | 答案 |

### 4.2 算法步骤

1. 读入两个字符串 $ra,rb$，转成大整数 $A,B$。
2. 若 $B<A$ 则交换，令 $R=\min$，$C=\max$。
3. 计算：
   1. $R_1=R+1,\ C_1=C+1$；
   2. $U=R\cdot R_1/2$；
   3. $T_1=U\cdot R_1\cdot C_1$；
   4. $T_3=U\cdot U$；
   5. $R_2=2R+1$；$W=R\cdot R_1\cdot R_2/6$；
   6. $T_2=W\cdot(R+C+2)$；
   7. $ans=T_1+T_3-T_2$。
4. 输出 $ans$，空格，再输出 `0`。

### 4.3 复杂度分析

- **时间**：$O(D^2)$，$D$ 为输入十进制位数（$\le1000$）。
- **空间**：$O(D)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1945 无边的网格
// https://www.luogu.com.cn/problem/P1945
#include<bits/stdc++.h>
using namespace std;

constexpr int base{10000};

struct Big
{
	vector<int>d;                       // 10^4 进制，低位在前

	Big(){}
	Big(long long x)
	{
		if(x==0)
		{
			d.push_back(0);
			return;
		}
		while(x>0)
		{
			d.push_back(x%base);
			x/=base;
		}
	}
	Big(const string&s)
	{
		for(int i=(int)s.size();i>0;i-=4)
		{
			int l=max(0,i-4),v{0};
			for(int j=l;j<i;++j)
				v=v*10+(s[j]-'0');
			d.push_back(v);
		}
		trim();
	}
	void trim()
	{
		while(d.size()>1&&d.back()==0)
			d.pop_back();
	}
};

bool lss(const Big&a,const Big&b)
{
	if(a.d.size()!=b.d.size())
		return a.d.size()<b.d.size();
	for(int i=(int)a.d.size()-1;i>=0;--i)
		if(a.d[i]!=b.d[i])
			return a.d[i]<b.d[i];
	return false;
}

Big add(const Big&a,const Big&b)
{
	Big c;
	int n=max(a.d.size(),b.d.size()),carry{0};
	c.d.resize(n);
	for(int i=0;i<n;++i)
	{
		int s=carry+(i<(int)a.d.size()?a.d[i]:0)+(i<(int)b.d.size()?b.d[i]:0);
		c.d[i]=s%base;
		carry=s/base;
	}
	if(carry)
		c.d.push_back(carry);
	return c;
}

Big sub(const Big&a,const Big&b)        // 保证 a>=b
{
	Big c;
	int n=a.d.size(),borrow{0};
	c.d.resize(n);
	for(int i=0;i<n;++i)
	{
		int cur=a.d[i]-borrow-(i<(int)b.d.size()?b.d[i]:0);
		if(cur<0)
		{
			cur+=base;
			borrow=1;
		}
		else
			borrow=0;
		c.d[i]=cur;
	}
	c.trim();
	return c;
}

Big mul(const Big&a,const Big&b)
{
	Big c;
	int n=a.d.size(),m=b.d.size();
	vector<long long>tmp(n+m,0);
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			tmp[i+j]+=1LL*a.d[i]*b.d[j];
	long long carry{0};
	c.d.resize(n+m);
	for(int i=0;i<n+m;++i)
	{
		long long cur=tmp[i]+carry;
		c.d[i]=cur%base;
		carry=cur/base;
	}
	c.trim();
	return c;
}

Big divsmall(const Big&a,int v)
{
	Big c;
	c.d.resize(a.d.size());
	long long rem{0};
	for(int i=(int)a.d.size()-1;i>=0;--i)
	{
		long long cur=rem*base+a.d[i];
		c.d[i]=cur/v;
		rem=cur%v;
	}
	c.trim();
	return c;
}

void out(const Big&a)
{
	cout<<a.d.back();
	for(int i=(int)a.d.size()-2;i>=0;--i)
		cout<<setw(4)<<setfill('0')<<a.d[i];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string ra,rb;
	cin>>ra>>rb;
	Big R(ra),C(rb);
	if(lss(C,R))
		swap(R,C);

	Big one(1),two(2);
	Big R1=add(R,one);
	Big C1=add(C,one);

	Big U=divsmall(mul(R,R1),2);        // R(R+1)/2
	Big T1=mul(mul(U,R1),C1);
	Big T3=mul(U,U);

	Big R2=add(add(R,R),one);           // 2R+1
	Big W=divsmall(mul(mul(R,R1),R2),6);
	Big T2=mul(W,add(add(R,C),two));    // (R+C+2)*W

	Big ans=sub(add(T1,T3),T2);
	out(ans);
	cout<<' '<<0<<'\n';
	return 0;
}
```
