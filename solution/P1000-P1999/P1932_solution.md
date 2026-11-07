# P1932 A+B  A-B  A*B  A/B A%B Problem

## 1. 重新读题

### 1.1 问题重述（人话版）

读入两个整数 $A,B$（各占一行，十进制，位数不超过 $2\times10^4$），依次输出五行：

$$
A+B,\quad A-B,\quad A\times B,\quad A\div B,\quad A\bmod B.
$$

### 1.2 关键约束与规模暗示

- 位数上限是 $2\times10^4$，不是数值上限。普通 `long long`（约 $19$ 位）完全不够，必须手写高精度。
- 题面写 $A,B>0$，同时特别注明“**由于数据有修改，减法运算结果可能带负号**”：即 $A-B$ 可能为负（$A<B$），而 $A,B$ 本身仍是非负/正数。
- 五位运算里最麻烦的是除法/取模：需要 $O(n^2)$ 级别的高精度长除法。
- 输出结果长度：乘法最多约 $4\times10^4$ 位，加法/减法约 $2\times10^4$ 位。

> 题面存在一点歧义：提示里既说 $|A|,|B|\le2\times10^4$ 是“位数而不是绝对值”，又说 $A,B>0$。稳妥的做法是把大整数实现成**带符号**的，并采用“向零截断”的除法语义（与 C++ 内建整数一致）；当输入全为正时，它自然退化为普通的向下取整除法，不会影响任何一组合法数据。下面的实现就是这种更一般的版本。

## 2. 思维链（Chain-of-thought）

### 2.1 表示：为什么用 $10^4$ 进制而不是 $10^9$

最直接的想法是一位十进制数字存一个 `int`，但 $2\times10^4$ 位时乘法是 $4\times10^8$ 次基本运算，偏慢。把若干十进制位压成一个“大位”（limb）可以成倍加速。

设基 $B=10^4$，把一个数按每 $4$ 位一组、**低位在前**存成数组 $d_0,d_1,\dots$，即
$$
x=\sum_{i\ge0}d_i B^i,\qquad 0\le d_i<B.
$$
$2\times10^4$ 位十进制数只有约 $5000$ 个 limb。

**引理 1（$B=10^4$ 时乘法不会溢出 64 位）**

设两个操作数各不超过 $N,M$ 个 limb（$N,M\le5000$）。学校式乘法在累加时，某个位置最多累加 $\min(N,M)$ 个形如 $d_i\cdot e_j$ 的乘积。每个乘积 $<B^2=10^8$，故累加值
$$
<5000\times10^8=5\times10^{11}<2^{63}.
$$
用 `long long` 保存中间量是安全的。

**证明**：由 $0\le d_i,e_j<B$ 得 $d_i e_j<B^2$；同一 $i+j$ 的数对至多 $\min(N,M)$ 个。$5\times10^{11}$ 远小于 `long long` 上限 $9.22\times10^{18}$。$\blacksquare$

若改用 $B=10^9$，单个乘积就接近 $10^{18}$，累加几个就会溢出，必须引入 `__int128`。$10^4$ 是一个“乘法安全、limb 数又足够少”的折中。

### 2.2 加减乘：学校竖式

- **比较**：先比 limb 数（即数量级），相同再从高位到低位逐位比。
- **加法**：从低到高逐位相加，进位 $0/1$。
- **减法**：只对 $|a|\ge|b|$ 做“大减小”，逐位借位；结果为 $0$ 时统一记成空数组。
- **乘法**：先做卷积
  $$
  c_k=\sum_{i+j=k}d_i e_j,
  $$
  再从低到高处理进位。

这些都是标准竖式，正确性由“每个位置的值小于基 $B$、进位唯一”保证。

### 2.3 除法：长除法的正确性

高精度除法的核心是逐 limb 的长除法。

设被除数 $a$、除数 $b>0$ 都以 $B$ 进制表示。从 $a$ 的最高位开始，维护当前余数 $r$，每读入一个 limb $a_i$ 就做
$$
r\leftarrow r\cdot B+a_i,
$$
然后求这一位的商
$$
q_i=\left\lfloor\frac{r}{b}\right\rfloor,
$$
并令
$$
r\leftarrow r-q_i b.
$$

**引理 2（每一位的商都在 $[0,B-1]$ 内）**

若进入该步前 $0\le r<b$，则 $0\le rB+a_i<bB$，故 $q_i=\lfloor(rB+a_i)/b\rfloor\in[0,B-1]$。

**证明**：$rB+a_i\ge0$ 且 $<bB$，除以 $b$ 后落在 $[0,B)$。$\blacksquare$

**引理 3（长除法结果正确）**

按上述过程处理完所有 limb 后，$q$ 的各位拼成的数满足 $a=qb+r$ 且 $0\le r<b$，即 $q=\lfloor a/b\rfloor$，$r=a\bmod b$。

**证明**：归纳不变量：处理完前 $k$ 个高位 limb 后，当前余数 $r$ 等于“这些高位所组成的数”对 $b$ 取模的结果，且 $0\le r<b$。初始 $r=0$ 成立；每步由引理 2，$q_i$ 是合法的商位且新余数 $<b$，不变量保持。处理完所有位后，整个数被写成 $qb+r$，由带余除法的唯一性得结论。$\blacksquare$

**引理 4（二分求商位）**

对固定的 $r,b$，函数 $f(q)=b\cdot q$ 关于 $q$ 单调不减，因此可以在 $[0,B-1]$ 上二分出最大的满足 $bq\le r$ 的 $q$，它就是 $\lfloor r/b\rfloor$。

**证明**：$b>0$ 时乘法关于 $q$ 严格递增；最大的可行 $q$ 正是 floor。$\blacksquare$

### 2.4 带符号运算与“向零截断”

用 `sign + magnitude` 表示整数，零统一为“空 magnitude、正号”。

- 加法：同号则绝对值相加；异号则大绝对值减小绝对值，符号跟随绝对值较大者。
- 减法：$A-B=A+(-B)$。
- 乘法：符号异或，绝对值相乘。
- 除法：$|q|=\lfloor|A|/|B|\rfloor$，$q$ 的符号为两符号异或；$|r|=|A|\bmod|B|$，$r$ 的符号与 $A$ 相同。这正是 C++ 内建整数除法的“向零截断”语义：
  $$
  A=qB+r,\qquad |r|<|B|,\qquad \operatorname{sgn}(r)\in\{0,\operatorname{sgn}(A)\}.
  $$
  当 $A,B>0$ 时就退化为普通的向下取整，与题面一致。

### 2.5 边界

- 输入有前导零：解析后 `trim` 掉。
- 结果为 $0$：必须输出 `0`，不能输出 `-0` 或空行。
- $A=0$：商和余数都是 $0$。
- 题面保证 $B>0$，不会出现除以零；代码不为它做额外处理。
- 减法 $A-B$ 为负时，输出前加 `-`。

### 2.6 复杂度

设两个数分别有 $N,M$ 个 limb（$N,M\le5000$）。

- 加/减/比较：$O(N+M)$。
- 乘法：$O(NM)$。
- 除法：对每个商位二分 $O(\log B)$ 次，每次比较 $O(M)$；加上被除数长度，上界 $O(NM\log B)$。实际由于先比较长度，只有余数长度接近除数时才付出 $O(M)$，$N=M=5000$ 的极端数据也只需数千万次基本运算。
- 空间 $O(N+M)$。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 位数到 $2\times10^4$，只能手写高精度。
- **choose representation** — 压成 $10^4$ 进制 limb，兼顾速度与 $64$ 位乘法安全。
- **classic problem solution** — 学校式竖式加/减/乘 + 逐 limb 长除法。
- **prove by invariant** — 长除法每一步余数 $<b$，商位落在 $[0,B-1]$，二分求商位。
- **pitfall** — 带符号、向零截断、零的规范化输出、前导零。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `BASE` | `constexpr int = 10000` | 进制 | — | 每 limb 4 位十进制 |
| `num` | `struct{bool neg; vector<int> d;}` | 带符号大整数，`d` 低位在前 | 解析输入 | digits |
| `a,b` | `num` | 输入的两个数 | 读入 | — |
| `q,r` | `num` | 商、余数 | 由 `divNum` 算出 | quotient/remainder |
| `gprod` | `int[5005]` | 除法二分时存放 `b*mid` 的临时缓冲区 | — | scratch |
| `trim(v)` | 函数 | 去掉高位的零 limb | — | — |
| `divmod(a,b,r)` | 返回商，`r` 输出余数 | 无符号长除法 | — | — |

### 4.2 算法步骤

1. 读入两行字符串 `sa,sb`，解析成带符号大整数 `a,b`（去掉前导零）。
2. 计算并输出：
   1. `a+b`（`addNum`）。
   2. `a-b`（`addNum(a, -b)`）。
   3. `a*b`（`mulNum`）。
3. 调用 `divNum(a,b,q,r)`：
   - 对绝对值做逐 limb 长除法（引理 2–4）；
   - 商符号为 `a.neg != b.neg`，余数符号为 `a.neg`；
   - 零结果强制符号为正。
4. 依次输出 `q`、`r`。

### 4.3 复杂度分析

- 时间：加/减 $O(N+M)$，乘法 $O(NM)$，除法 $O(NM\log BASE)$ 上界（实际远低）。
- 空间：$O(N+M)$。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int BASE{10000};
constexpr int mxn{5005};

int gprod[mxn];

void trim(vector<int>&a)
{
	while(!a.empty()&&a.back()==0)
		a.pop_back();
}

int cmp(const vector<int>&a,const vector<int>&b)
{
	if(a.size()!=b.size())
		return a.size()<b.size()?-1:1;
	for(int i=(int)a.size()-1;i>=0;--i)
		if(a[i]!=b[i])
			return a[i]<b[i]?-1:1;
	return 0;
}

vector<int> add(vector<int>a,const vector<int>&b)
{
	int carry=0;
	int n=max(a.size(),b.size());
	a.resize(n,0);
	for(int i=0;i<n;++i)
	{
		int cur=a[i]+(i<(int)b.size()?b[i]:0)+carry;
		a[i]=cur%BASE;
		carry=cur/BASE;
	}
	if(carry)
		a.push_back(carry);
	trim(a);
	return a;
}

// a>=b
vector<int> sub(vector<int>a,const vector<int>&b)
{
	int borrow=0;
	for(int i=0;i<(int)a.size();++i)
	{
		int cur=a[i]-(i<(int)b.size()?b[i]:0)-borrow;
		if(cur<0)
		{
			cur+=BASE;
			borrow=1;
		}
		else
			borrow=0;
		a[i]=cur;
	}
	trim(a);
	return a;
}

vector<int> mul(const vector<int>&a,const vector<int>&b)
{
	if(a.empty()||b.empty())
		return {};
	vector<long long>c(a.size()+b.size(),0);
	for(int i=0;i<(int)a.size();++i)
		for(int j=0;j<(int)b.size();++j)
			c[i+j]+=(long long)a[i]*b[j];
	vector<int>res;
	long long carry=0;
	for(long long x:c)
	{
		long long cur=x+carry;
		res.push_back(cur%BASE);
		carry=cur/BASE;
	}
	while(carry)
	{
		res.push_back(carry%BASE);
		carry/=BASE;
	}
	trim(res);
	return res;
}

int mulSmallInto(const vector<int>&b,int k,int*out)
{
	long long carry=0;
	int len=b.size();
	for(int i=0;i<len;++i)
	{
		long long cur=(long long)b[i]*k+carry;
		out[i]=cur%BASE;
		carry=cur/BASE;
	}
	int L=len;
	while(carry)
	{
		out[L++]=carry%BASE;
		carry/=BASE;
	}
	while(L>0&&out[L-1]==0)
		--L;
	return L;
}

int cmpArr(const int*a,int la,const vector<int>&b)
{
	while(la>0&&a[la-1]==0)
		--la;
	if(la!=(int)b.size())
		return la<(int)b.size()?-1:1;
	for(int i=la-1;i>=0;--i)
		if(a[i]!=b[i])
			return a[i]<b[i]?-1:1;
	return 0;
}

void subInPlace(vector<int>&a,const int*b,int lb)
{
	int borrow=0;
	for(int i=0;i<(int)a.size();++i)
	{
		int s=(i<lb?b[i]:0)+borrow;
		if(a[i]<s)
		{
			a[i]+=BASE-s;
			borrow=1;
		}
		else
		{
			a[i]-=s;
			borrow=0;
		}
	}
	trim(a);
}

vector<int> divmod(const vector<int>&a,const vector<int>&b,vector<int>&r)
{
	vector<int>q(a.size()+1,0);
	r.clear();
	for(int i=(int)a.size()-1;i>=0;--i)
	{
		r.insert(r.begin(),a[i]);
		trim(r);
		int lo=0,hi=BASE-1,best=0;
		while(lo<=hi)
		{
			int mid=(lo+hi)>>1;
			int L=mulSmallInto(b,mid,gprod);
			if(cmpArr(gprod,L,r)<=0)
			{
				best=mid;
				lo=mid+1;
			}
			else
				hi=mid-1;
		}
		q[i]=best;
		if(best)
		{
			int L=mulSmallInto(b,best,gprod);
			subInPlace(r,gprod,L);
		}
	}
	trim(q);
	return q;
}

struct num
{
	bool neg{false};
	vector<int>d;
};

num parse(string s)
{
	num x;
	int i=0;
	if(!s.empty()&&s[0]=='-')
	{
		x.neg=true;
		i=1;
	}
	else if(!s.empty()&&s[0]=='+')
		i=1;
	for(int j=(int)s.size();j>i;)
	{
		int k=max(i,j-4);
		int v=0;
		for(int p=k;p<j;++p)
			v=v*10+(s[p]-'0');
		x.d.push_back(v);
		j=k;
	}
	trim(x.d);
	if(x.d.empty())
		x.neg=false;
	return x;
}

string tostr(const num&x)
{
	if(x.d.empty())
		return "0";
	string s=to_string(x.d.back());
	for(int i=(int)x.d.size()-2;i>=0;--i)
	{
		string t=to_string(x.d[i]);
		s+=string(4-t.size(),'0')+t;
	}
	return (x.neg?"-":"")+s;
}

num addNum(const num&a,const num&b)
{
	if(a.neg==b.neg)
		return {a.neg,add(a.d,b.d)};
	if(cmp(a.d,b.d)>=0)
		return {a.neg,sub(a.d,b.d)};
	return {b.neg,sub(b.d,a.d)};
}

num negNum(num a)
{
	if(!a.d.empty())
		a.neg=!a.neg;
	return a;
}

num mulNum(const num&a,const num&b)
{
	num r{a.neg!=b.neg,mul(a.d,b.d)};
	if(r.d.empty())
		r.neg=false;
	return r;
}

void divNum(const num&a,const num&b,num&q,num&r)
{
	vector<int>rr;
	q.d=divmod(a.d,b.d,rr);
	q.neg=a.neg^b.neg;
	r.d=rr;
	r.neg=a.neg;
	if(q.d.empty())
		q.neg=false;
	if(r.d.empty())
		r.neg=false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string sa,sb;
	cin>>sa>>sb;
	num a=parse(sa),b=parse(sb);
	num q,r;
	divNum(a,b,q,r);

	cout<<tostr(addNum(a,b))<<'\n';
	cout<<tostr(addNum(a,negNum(b)))<<'\n';
	cout<<tostr(mulNum(a,b))<<'\n';
	cout<<tostr(q)<<'\n';
	cout<<tostr(r)<<'\n';
	return 0;
}
```
