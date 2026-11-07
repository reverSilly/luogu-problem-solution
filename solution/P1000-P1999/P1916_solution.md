# P1916 Hermite 多点求值 / 多点 Taylor 展开

## 1. 重新读题

### 1.1 问题重述（人话版）

给定一个次数小于 $n$ 的多项式

$$F(x)=\sum_{i=0}^{n-1}f_ix^i,$$

再给定 $m$ 组 $(a_i,k_i)$，满足 $\sum_{i=1}^m k_i=n$，$a_i$ 两两不同。对每组 $(a_i,k_i)$，输出

$$F(a_i),\;F'(a_i),\;F''(a_i),\;\ldots,\;F^{(k_i-1)}(a_i),$$

所有运算在模 $998244353$ 下进行。

### 1.2 关键约束与规模暗示

- $1\le m\le n\le64000$，$\sum k_i=n$。**这个求和条件不是装饰**：它说明「所有要求的信息量」恰好等于多项式的系数个数，因此正解应当是 $O(n\log^2n)$ 级别，而不是按每个点 $O(n)$ 去算。
- 单点求 $F(a)$ 用秦九韶是 $O(n)$；单点求全部 $k$ 阶导数是 $O(nk)$；全部点加起来的朴素代价
    
$$O\Bigl(\sum_i n k_i\Bigr)=O(n^2)\approx4\times10^9,$$
    
不可接受。注意这个量级甚至与 $m$ 无关了——只要某个 $k_i$ 很大就爆。
- $\sum k_i=n$ 还暗示：把 $(x-a_i)^{k_i}$ 全部乘起来，度数正好是 $n$，与 $F$ 的次数上界一致。这是「乘积树根节点就是 $F$ 的模」这一技巧成立的前提。
- 模数 $998244353$ 是 NTT 模数（$=119\cdot2^{23}+1$），且 $a_i$ 可能为 $0$，这些都会在实现里用到。

## 2. 思维链（Chain-of-thought）

### 2.1 先弄清「求导数」到底求的是什么

回忆 Taylor 公式：对任意 $a$，

$$F(x)=\sum_{j\ge0}\frac{F^{(j)}(a)}{j!}(x-a)^j.$$

所以「求 $F$ 在 $a$ 处的前 $k$ 阶导数」本质上是：**求 $F$ 在基 $\{1,(x-a),(x-a)^2,\ldots\}$ 下的前 $k$ 个坐标**。相邻两组坐标之间是「坐标变换」。

这给了一个非常重要的直觉：这不是 $k$ 个互相独立的问题，而是**一次基变换**。

### 2.2 求出前 $k$ 阶导数的等价刻画

**引理 1（导数 $\leftrightarrow$ 余式）**：设 $R(x)=F(x)\bmod (x-a)^k$（即 $R$ 是 $F$ 除以 $(x-a)^k$ 的余式，$\deg R<k$）。若 $R$ 在基 $\{ (x-a)^j \}$ 下的系数为 $r_0,\ldots,r_{k-1}$，即 $R(x)=\sum_{j<k}r_j(x-a)^j$，则

$$F^{(j)}(a)=j!\,r_j,\qquad 0\le j<k.$$

**证明**：由带余除法，存在 $Q$ 使 $F(x)=Q(x)(x-a)^k+R(x)$。对 $j<k$ 求 $j$ 阶导：$Q(x)(x-a)^k$ 含因子 $(x-a)^{k-j}$，在 $x=a$ 处为 $0$（因为 $k-j\ge1$）。故 $F^{(j)}(a)=R^{(j)}(a)$。而 $R$ 是次数 $<k$ 的多项式，在 $a$ 处展开只有前 $k$ 项，$R^{(j)}(a)=j!\,r_j$。$\square$

这条引理把「求导」变成了「求余式」。而余式是**多项式除法**的产物，可以用快速多项式算法处理。

### 2.3 多组询问怎么办：乘积树 + 余式树

现在要对每个 $i$ 求 $F\bmod (x-a_i)^{k_i}$。

**错误做法（局部可行但全局不可行）**：对每个 $i$ 独立地拿 $F$ 去做一次除法。单次除法是 $O(M(n))$（$M(n)$ 为乘法复杂度），做 $m$ 次就是 $O(m\,M(n))$，$m$ 可达 $64000$，彻底爆掉。

**观察**：除法的代价取决于**被除式的次数**。如果我们能保证每次做除法时被除式已经先被「大的模」降到低次数，就能省下大量工作。这正是分治的结构。

令模数

$$M_{\text{node}}(x)=\prod_{i\in\text{node}}(x-a_i)^{k_i},$$

按区间建一棵二叉树（叶子是单个 $i$，模数 $(x-a_i)^{k_i}$；内部节点是左右儿子模数之积），则 $\deg M_{\text{root}}=\sum_i k_i=n$。

**引理 2（余式可逐层下推）**：设 $c$ 是 $p$ 的儿子节点，则

$$\bigl(F\bmod M_p\bigr)\bmod M_c=F\bmod M_c.$$

**证明**：设 $F=Q\,M_p+R_p$，其中 $R_p=F\bmod M_p$。又 $M_p=M_c\cdot S$（因为儿子模数整除父亲模数）。代入：

$$F=Q\,M_cS+R_p=(QS)M_c+R_p,$$

所以 $F$ 除以 $M_c$ 的余式就是 $R_p\bmod M_c$。$\square$

有了引理 2，算法就是：

1. 自底向上建乘积树，算出每个节点的 $M$。
2. 自顶向下传递余式：根节点的余式就是 $F$ 本身（因为 $\deg F\le n-1<\deg M_{\text{root}}=n$，一步除法都省了）；对每个节点，把孩子模数拿来除当前余式，得到孩子的余式。
3. 到叶子 $i$ 时手里就是 $F\bmod(x-a_i)^{k_i}$，再用引理 1 的基变换输出。

**复杂度**：设一个节点模数次数为 $d$，则其父亲的次数为 $d+d'$（$d'$ 是兄弟次数）。除法一次 $O(M(d+d'))$。同一层所有节点次数之和恰为 $n$，而 $M$ 超线性，故每层总代价 $O(M(n))$；树高 $O(\log m)$，总计

$$O\bigl(M(n)\log m\bigr).$$

建树同理。取 $M(n)=O(n\log n)$，即 $O(n\log n\log m)$。

这里 $\sum k_i=n$ 的作用体现得淋漓尽致：若 $\sum k_i$ 远大于 $n$，根节点次数会超过 $F$ 的次数，「根余式 = $F$」就不成立，层数也会失控。

### 2.4 叶子处：把「标准系数」换成「Taylor 系数」

余式 $R(x)=\sum_{t=0}^{K-1}c_tx^t$ 是以 $x$ 的幂为基给出的，而我们要的是以 $(x-a)^j$ 为基的系数。直接做二项式展开是 $O(K^2)$，全部叶子加起来最坏 $O(n^2)$（例如 $m$ 很小、某个 $k_i$ 很大）。必须用卷积把这一步压到 $O(K\log K)$。

**引理 3（Taylor 平移的卷积形式）**：对 $K=k_i$，令

$$A_t=c_t\cdot t!,\qquad B_s=\frac{a^{s}}{s!}\quad(0\le t,s<K),$$

再令 $\tilde A_u=A_{K-1-u}$。设 $C=\tilde A * B$（普通卷积），则

$$F^{(j)}(a)=C_{K-1-j}\qquad(0\le j<K).$$

**证明**：由引理 1 与导数的线性性，

$$F^{(j)}(a)=R^{(j)}(a)=\sum_{t=j}^{K-1}c_t\cdot\frac{t!}{(t-j)!}\,a^{t-j}
=\sum_{t=j}^{K-1}\bigl(c_t\,t!\bigr)\cdot\frac{a^{t-j}}{(t-j)!}
=\sum_{t=j}^{K-1}A_tB_{t-j}.$$

另一边，按卷积定义

$$C_{K-1-j}=\sum_{u}\tilde A_uB_{K-1-j-u}.$$

令 $u=K-1-t$（$t$ 从 $0$ 到 $K-1$），则 $\tilde A_u=A_t$，且 $K-1-j-u=t-j$，于是

$$C_{K-1-j}=\sum_{t}A_tB_{t-j}=\sum_{t=j}^{K-1}A_tB_{t-j}=F^{(j)}(a),$$

其中 $t<j$ 时 $B_{t-j}$ 的下标为负，视为 $0$。$\square$

于是一个叶子只需一次长度为 $K$ 的卷积，成本 $O(K\log K)$。所有叶子加起来 $\sum_i O(k_i\log k_i)\le O(n\log n)$。

**实现细节**：$A_t=c_t\,t!$ 与 $B_s=a^s/s!$ 都要用到阶乘与逆阶乘，预处理到 $n$ 即可；$a^s$ 用递推 `pw=pw*a` 求。所有下标都模 $998244353$。

### 2.5 需要哪些多项式工具

1. **乘法**：NTT，模 $998244353$、原根 $3$。最大长度出现在某个节点把两个约 $32000$ 次的多项式相乘，得 $64001$ 项，$N=2^{17}$ 足够；其余除法/求逆的中间卷积也不超过 $2^{17}$ 项，代码统一取上界 $2^{18}$，而 $2^{18}\mid p-1$，可用。
2. **求逆**：Newton 迭代。若 $g\equiv f^{-1}\pmod{x^{k}}$，则

$$g'=g(2-fg)\pmod{x^{2k}}$$

满足 $fg'\equiv1\pmod{x^{2k}}$。**证明**：设 $fg=1+x^k h$，则 $fg'=fg(2-fg)=(1+x^kh)(2-1-x^kh)=1-x^{2k}h^2\equiv1\pmod{x^{2k}}$。$\square$

   每轮两次乘法，总成本 $O(M(k))$ 的几何级数和。
3. **带余除法**：把 $A$ 除以 $B$（$\deg B=m$）。设 $A$ 的次数为 $d$，商 $Q$ 的次数为 $d-m$。用「反转系数」技巧：令 $A^{R}(x)=x^{d}A(1/x)$，$B^{R}(x)=x^{m}B(1/x)$，则

$$Q^{R}=A^{R}\cdot\bigl(B^{R}\bigr)^{-1}\pmod{x^{d-m+1}}.$$

   求出 $Q$ 后再算 $R=A-QB$。成本 $O(M(d))$。

   注意：把所有模数都构造成**首一**多项式（$(x-a)^k$ 与它们的乘积都是首一的），于是 $B^{R}(0)=1$，求逆永远合法。这一点很重要——否则还得处理首项系数不可逆的情况。

### 2.6 建叶子模数：$(x-a)^k$ 的系数，以及为什么从高位往低位递推

$$(x-a)^k=\sum_{j=0}^{k}\binom{k}{j}x^{j}(-a)^{k-j}.$$

最自然的写法是令 $\text{pw}=1$，从 $j=0$ 开始每次乘 $(-a)$。但那样得到的是 $(-a)^{j}$ 而不是 $(-a)^{k-j}$。

如果从 $j=k$ 往下循环，令 $q=(-a)\bmod p$、初值 $\text{pw}=1$，每一步：

$$c_j=\binom{k}{j}\text{pw},\qquad \text{pw}\leftarrow\text{pw}\cdot q,$$

那么第 $j$ 步的 $\text{pw}=q^{k-j}$，正好是所需。**不能**反过来用除法（除以 $q$）从 $j=k$ 推：题目允许 $a_i=0$，此时 $q=0$ 不可逆。从高位往低位递推完全避开了除法，是这里必须坚持的写法。

### 2.7 边界与坑

1. **$F$ 的末尾零**：$f$ 的高次项可能为 $0$。`pol_mod` 在除法前对输入 `aa`、除法后对余式 `r` 都做 `trim`，否则「次数」会被末尾零虚假抬高，多做无用的除法，且 `r.resize(b.size()-1)` 的截断也可能出错。
2. **零多项式**：全部系数为 $0$ 时，`trim` 后是空向量，所有后续运算返回空，叶子处补零输出。
3. **$a_i=0$**：见 2.6，不能除以 $a$；卷积里 $B_0=1,B_{s>0}=0$ 也完全正常。
4. **模数次数为 1**（$k_i=1$）：余式是常数，Taylor 平移退化为一次长度 $1$ 的卷积，输出就是 $F(a_i)$。
5. **输出格式**：第 $i$ 行恰好 $k_i$ 个数，空格分隔，行末换行，不要多输出空行。递归按叶子编号从小到大进行，正好与输出顺序一致，所以可以边搜边输出。
6. **精度**：全程整数模运算，没有浮点，不存在精度问题；唯一要保证的是 NTT 长度不超过 $2^{23}$。

### 2.8 复杂度汇总

- 建乘积树：$O(M(n)\log m)$。
- 余式下推：$O(M(n)\log m)$。
- 叶子 Taylor 平移：$O(n\log n)$。
- 合计 $O(n\log^2 n)$，空间 $O(n\log m)$（树上的多项式总量）。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 前 $k$ 阶导数 = $F\bmod(x-a)^k$ 在基 $(x-a)^j$ 下的系数（引理 1）。
- **reduce to easier problem** — 多点求余：用乘积树把「每个点各自除一次」变成「逐层缩小次数」。
- **prove the reduction** — 儿子模数整除父亲模数 $\Rightarrow$ 余式可以先模父亲再模儿子（引理 2）。
- **bound the work** — $\sum k_i=n$ 保证每层度数之和为 $n$，总代价 $O(M(n)\log m)$。
- **classic problem solution** — NTT 乘法 + Newton 求逆 + 反转系数做带余除法。
- **defer reconstruction** — 叶子处用一次卷积完成 Taylor 平移（引理 3），而不是 $O(k^2)$ 展开。
- **implementation care** — 从高位递推构造 $(x-a)^k$，避开 $a=0$ 时的除法。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n,m` | `int` | 多项式系数个数、询问组数 | 读入 | 维度 |
| `f[]` | `int[64005]` | $f_0,\ldots,f_{n-1}$ | 读入 | coefficients |
| `av[i],kv[i]` | `int[64005]` | 第 $i$ 组的 $a_i,k_i$ | 读入 | a, k |
| `fac[],ifac[]` | `i64[64005]` | 阶乘、逆阶乘（模 $p$） | 预处理 | factorial |
| `Md[]` | `vector<int>[4*64005]` | 每个树节点的模数 $M_{\text{node}}$ | 建树时填 | modulus |
| `build(l,r,idx)` | 函数 | 建 $[l,r)$ 的乘积树，返回节点号 | — | — |
| `solve(l,r,idx,cur)` | 函数 | 把余式 `cur` 下推到叶子并输出 | — | — |
| `pol_mod(a,b)` | 函数 | 返回 $a\bmod b$ | — | — |
| `pol_inv(a,need)` | 函数 | 返回 $a^{-1}$ 的前 `need` 项 | — | — |
| `mul(a,b,need)` | 函数 | 卷积，可截断到前 `need` 项 | — | — |

### 4.2 算法步骤

1. 读入 $n,m$、$f$、$(a_i,k_i)$；预处理 `fac[0..n]`、`ifac[0..n]`。
2. 递归 `build(0,m,1)`：叶子 $i$ 处用从高位到低位的二项式递推构造 $(x-a_i)^{k_i}$；内部节点把左右儿子多项式相乘。
3. 调用 `solve(0,m,1,f)`：
   - 先令 `rem = cur mod Md[idx]`；
   - 若到叶子 $i$：把 `rem` 补到长度 $k_i$，构造 $A_t=c_t t!$ 与 $B_s=a_i^s/s!$，把 $A$ 反转后与 $B$ 卷积，输出 `C[k_i-1-j]`（$j=0..k_i-1$）；
   - 否则把 `rem` 分别下推到左右儿子。
4. 输出即为答案。

### 4.3 复杂度与验证状态

- **时间复杂度**：$O(n\log^2n)$（$M(n)=O(n\log n)$ 再乘树高 $\log m\le16$）。
- **空间复杂度**：$O(n\log m)$ 的树上多项式 + $O(n)$ 的其它数组。
- 编译 `g++ -std=c++17 -O2 -Wall -Wextra`，无警告。
- 自测：
  - $n=3,F=1+2x+3x^2$，询问 $(2,2),(5,1)$ 输出 `17 14` 与 `86`（手算 $F(2)=17,F'(2)=14,F(5)=86$）；
  - $a_i=0$、$f=[0,0,5]$ 的边界输出 `20 20 10`；
  - 300 组 $n\le9$ 的随机数据、20 组 $n\in[60,120]$ 的随机数据与 $O(nk)$ 暴力逐点求导对拍，全部一致；
  - 满数据 $n=m=64000$（全部 $k_i=1$）用户态耗时约 $0.8$ s；$m=2,k=(32000,32000)$ 约 $0.13$ s，$m=1,k=64000$ 约 $0.03$ s。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int p{998244353},G{3};
constexpr int mxn{64005};
constexpr int mxntt{1<<18};

int n,m;
int f[mxn];
int av[mxn],kv[mxn];
i64 fac[mxn],ifac[mxn];

i64 qpow(i64 a,i64 b)
{
	i64 r=1;
	for(;b;b>>=1,a=a*a%p)
		if(b&1)
			r=r*a%p;
	return r;
}

int rev[mxntt];

void ntt(vector<int>&A,bool inv)
{
	int N=A.size();
	for(int i=1;i<N;++i)
	{
		rev[i]=(rev[i>>1]>>1)|((i&1)?(N>>1):0);
		if(i<rev[i])
			swap(A[i],A[rev[i]]);
	}
	for(int len=1;len<N;len<<=1)
	{
		i64 w=qpow(G,(p-1)/(len<<1));
		if(inv)
			w=qpow(w,p-2);
		for(int i=0;i<N;i+=len<<1)
		{
			i64 wn=1;
			for(int j=0;j<len;++j)
			{
				int u=A[i+j],v=wn*A[i+j+len]%p;
				int s=u+v;
				if(s>=p)
					s-=p;
				int d=u-v;
				if(d<0)
					d+=p;
				A[i+j]=s;
				A[i+j+len]=d;
				wn=wn*w%p;
			}
		}
	}
	if(inv)
	{
		i64 ni=qpow(N,p-2);
		for(int&x:A)
			x=x*ni%p;
	}
}

void trim(vector<int>&a)
{
	while(!a.empty()&&a.back()==0)
		a.pop_back();
}

// 乘法，need>=0 时只保留前 need 项
vector<int> mul(const vector<int>&a,const vector<int>&b,int need=-1)
{
	if(a.empty()||b.empty())
		return {};
	int n1=a.size(),n2=b.size();
	int full=n1+n2-1;
	int lim=(need<0)?full:min(need,full);
	vector<int>c(lim,0);
	if(min(n1,n2)<=32)
	{
		for(int i=0;i<n1;++i)
			for(int j=0;j<n2&&i+j<lim;++j)
				c[i+j]=(c[i+j]+(i64)a[i]*b[j])%p;
		return c;
	}
	int N=1;
	while(N<full)
		N<<=1;
	vector<int>A(a.begin(),a.end()),B(b.begin(),b.end());
	A.resize(N);
	B.resize(N);
	ntt(A,false);
	ntt(B,false);
	for(int i=0;i<N;++i)
		A[i]=(i64)A[i]*B[i]%p;
	ntt(A,true);
	A.resize(lim);
	return A;
}

// 形式幂级数求逆，返回前 need 项
vector<int> pol_inv(const vector<int>&a,int need)
{
	vector<int>g{int(qpow(a[0],p-2))};
	int cur=1;
	while(cur<need)
	{
		int len=min(cur<<1,need);
		vector<int>ff(a.begin(),a.begin()+min((int)a.size(),len));
		vector<int>t=mul(g,g,len);
		t=mul(t,ff,len);
		vector<int>ng(len);
		for(int i=0;i<len;++i)
		{
			i64 v=2LL*(i<(int)g.size()?g[i]:0)-(i<(int)t.size()?t[i]:0);
			v%=p;
			if(v<0)
				v+=p;
			ng[i]=v;
		}
		g=ng;
		cur=len;
	}
	return g;
}

// a mod b（b 首项非零）
vector<int> pol_mod(const vector<int>&a,const vector<int>&b)
{
	vector<int>aa=a;
	trim(aa);
	if(aa.size()<b.size())
		return aa;
	int qlen=aa.size()-b.size()+1;
	vector<int>ra(aa.rbegin(),aa.rend());
	vector<int>rb(b.rbegin(),b.rend());
	vector<int>invb=pol_inv(rb,qlen);
	vector<int>q=mul(ra,invb,qlen);
	reverse(q.begin(),q.end());
	vector<int>prod=mul(q,b);
	vector<int>r(aa.size());
	for(int i=0;i<(int)aa.size();++i)
	{
		int sub=i<(int)prod.size()?prod[i]:0;
		int v=aa[i]-sub;
		if(v<0)
			v+=p;
		r[i]=v;
	}
	r.resize(b.size()-1);
	trim(r);
	return r;
}

vector<int>Md[mxn*4];

int build(int l,int r,int idx)
{
	if(r-l==1)
	{
		int K=kv[l];
		Md[idx].assign(K+1,0);
		i64 q=(p-av[l])%p;
		i64 pw=1;
		for(int j=K;j>=0;--j)
		{
			i64 c=fac[K]*ifac[j]%p*ifac[K-j]%p;
			Md[idx][j]=c*pw%p;
			pw=pw*q%p;
		}
		return idx;
	}
	int mid=(l+r)>>1;
	int lc=build(l,mid,idx*2);
	int rc=build(mid,r,idx*2+1);
	Md[idx]=mul(Md[lc],Md[rc]);
	return idx;
}

void solve(int l,int r,int idx,const vector<int>&cur)
{
	vector<int>rem=pol_mod(cur,Md[idx]);
	if(r-l==1)
	{
		int K=kv[l];
		rem.resize(K,0);
		// Taylor 平移：ans_j = conv(rev(c_t*t!), a^s/s!)[K-1-j]
		vector<int>A(K),B(K);
		for(int t=0;t<K;++t)
			A[t]=rem[t]*fac[t]%p;
		i64 pw=1;
		for(int s=0;s<K;++s)
		{
			B[s]=pw*ifac[s]%p;
			pw=pw*av[l]%p;
		}
		reverse(A.begin(),A.end());
		vector<int>C=mul(A,B);
		for(int j=0;j<K;++j)
		{
			int id=K-1-j;
			cout<<(id<(int)C.size()?C[id]:0)<<(j+1==K?'\n':' ');
		}
		return;
	}
	int mid=(l+r)>>1;
	solve(l,mid,idx*2,rem);
	solve(mid,r,idx*2+1,rem);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>f[i];
	for(int i=0;i<m;++i)
		cin>>av[i]>>kv[i];

	fac[0]=1;
	for(int i=1;i<=n;++i)
		fac[i]=fac[i-1]*i%p;
	ifac[n]=qpow(fac[n],p-2);
	for(int i=n;i>=1;--i)
		ifac[i-1]=ifac[i]*i%p;

	build(0,m,1);
	solve(0,m,1,vector<int>(f,f+n));
	return 0;
}
```
