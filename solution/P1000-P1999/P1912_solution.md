# P1912 [NOI2009] 诗人小G

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $N$ 个句子，必须**保持原顺序**排成若干行；同一行内相邻句子之间用一个空格隔开，句子本身不可拆开。给定行标准长度 $L$ 和指数 $P$。

若某一行由第 $j+1$ 到第 $i$ 个句子组成，这一行的实际长度是「这些句子的字符数之和 + 它们之间的空格数」，即

$$
\ell(j,i)=\Big(\sum_{k=j+1}^{i}len_k\Big)+(i-j-1).
$$

这一行的不协调度为 $|\ell(j,i)-L|^P$。整首诗的代价是所有行不协调度之和。求最小代价；若最小代价 $>10^{18}$ 输出 `Too hard to arrange`，否则先输出代价，再输出任意一种达到最小代价的排版。每组数据后输出一行 20 个 `-`。

### 1.2 关键约束与规模暗示

- $T\le 10$，句子长度 $\le 30$，$P\le 10$，$L\le 3\times 10^6$，$N\le 10^5$（前三个测试点更小）。
- 测试点 $1$：$N\le18$ —— 允许指数级/暴力。
- 测试点 $2,3$：$N\le2000$ —— 允许 $O(N^2)$ 的 DP。
- 测试点 $4,5$：$N\le10^5,L\le200,P\le10$。
- 测试点 $6,7$：$N\le10^5,L\le3\times10^6,P=2$。
- 测试点 $8,9,10$：$N\le10^5,L\le3\times10^6,P\le10$。

$N$ 到 $10^5$ 而 $O(N^2)$ 是 $10^{10}$，**必须在 DP 上做优化**。$L$ 最大 $3\times10^6$、$P\le10$，单行代价可以大到 $10^{66}$ 量级，所以「超过 $10^{18}$」的判定要小心数值范围。这是本组最难的题，核心是**决策单调性（四边形不等式）优化 DP**。

---

## 2. 思维链（Chain-of-thought）

### 2.1 把「排版」抽象成「切分点」

排版方案只由「在哪里换行」决定：设最后一个切点序列为 $0=p_0<p_1<\dots<p_m=N$，第 $k$ 行是句子 $p_{k-1}+1\sim p_k$。

为了把一行的长度写成两个「只和下标有关」的量之差，定义

$$
S_i=\sum_{k=1}^{i}len_k,\qquad A_i=S_i+i .
$$

那么第 $j+1\sim i$ 句组成的行长度

$$
\ell(j,i)=S_i-S_j+(i-j-1)=(S_i+i)-(S_j+j)-1=A_i-A_j-1 .
$$

注意 $A_i-A_{i-1}=len_i+1\ge2$，所以 $A$ 是**严格递增**的（这一点在后面的凸性论证里会反复用到）。于是这一行的代价是

$$
w(j,i)=\big|A_i-A_j-1-L\big|^P .
$$

### 2.2 第一个 DP：$O(N^2)$

设 $f_i$ 为排好前 $i$ 个句子的最小代价，$f_0=0$。枚举最后一行的起点 $j+1$：

$$
f_i=\min_{0\le j<i}\Big(f_j+\big|A_i-A_j-1-L\big|^P\Big).
$$

**引理 0（DP 正确性）**：上式递推得到的 $f_i$ 就是前 $i$ 句的最小代价。

**证明**（对 $i$ 归纳）：最优方案的最后一行的第一个句子设为 $j+1$（$0\le j<i$）。它前面 $j$ 个句子的排版必须也是最优的（否则替换成更优的会整体更优），代价 $\ge f_j$；最后一行代价恰为 $w(j,i)$。故最优代价 $\ge \min_j(f_j+w(j,i))$。反过来任取 $j$ 和「前 $j$ 句的最优排版」拼上最后一行，都是合法方案，其代价为 $f_j+w(j,i)$，故最优代价 $\le\min_j(f_j+w(j,i))$。两者相等。$\square$

$O(N^2)$ 对 $N=10^5$ 是 $10^{10}$，不行。下面要利用 $w$ 的特殊结构。

### 2.3 先试贪心，立刻失败

一个自然想法：从左往右，往当前行里塞句子，直到「再塞一个会让这一行离 $L$ 更远」就换行。用 $L=10,P=2$，句子长度 $6,6,3$ 打脸：

- 贪心：第一行塞 $6$ 后长度 $6$（差 $4$，代价 $16$），再塞一个长度 $13$（差 $3$，代价 $9$），因为 $9<16$ 就继续塞；第二行只剩 $3$（差 $7$，代价 $49$）。总代价 $9+49=58$。
- 最优：第一行只放 $6$（代价 $16$），第二行放 $6$ 和 $3$，长度 $6+1+3=10$（代价 $0$），总代价 $16$。

失败原因：**当前行「稍微好一点」可能让后面的行「坏很多」**，局部贪心看不到。必须全局 DP。DP 也有 $O(N^2)$ 的瓶颈，需要发现转移点随 $i$ 单调。

### 2.4 关键观察：代价核是凸函数

记 $g(x)=|x|^P$。因为 $P\ge1$ 是整数，$g$ 在 $\mathbb R$ 上是**凸函数**（$P=1$ 时是 $|x|$，$P\ge2$ 时是光滑凸函数）。而

$$
w(j,i)=g\big(A_i-A_j-1-L\big).
$$

凸性会把「转移点单调」变成可能。我们要把它严格化。

### 2.5 引理 1：四边形不等式（Monge 性）

**引理 1**：对任意 $a\le b\le c\le d$，有

$$
w(a,c)+w(b,d)\ \le\ w(a,d)+w(b,c).
$$

**证明**：记

$$
\alpha=A_c-A_a-1-L,\qquad \beta=A_c-A_b-1-L .
$$

因为 $a\le b$ 且 $A$ 单调不减，$A_a\le A_b$，故

$$
\alpha=A_c-A_a-1-L\ \ge\ A_c-A_b-1-L=\beta .
$$

记 $\Delta=A_d-A_c\ge0$（$A$ 单调不减）。则

$$
A_d-A_a-1-L=\alpha+\Delta,\qquad A_d-A_b-1-L=\beta+\Delta .
$$

于是引理等价于

$$
g(\alpha)+g(\beta+\Delta)\le g(\alpha+\Delta)+g(\beta),
$$

即

$$
g(\alpha+\Delta)-g(\alpha)\ \ge\ g(\beta+\Delta)-g(\beta).
$$

**凸函数在等长区间上的增量单调不减**：设 $u\le v$，则

$$
g(v+\Delta)-g(u+\Delta)\ \ge\ g(v)-g(u).
$$

证明：凸函数几乎处处可导且导数 $g'$ 单调不减（$P=1$ 时取次导数即可），

$$
g(v+\Delta)-g(u+\Delta)=\int_{u}^{v}g'(t+\Delta)\,dt
\ \ge\ \int_{u}^{v}g'(t)\,dt=g(v)-g(u),
$$

因为 $t+\Delta\ge t$ 且 $g'$ 单调不减。

取 $u=\beta,\ v=\alpha$（$\beta\le\alpha$）即得所需不等式。$\square$

这里 $A$ 必须严格递增吗？证明只用到 $A$ 单调不减（$\alpha\ge\beta$、$\Delta\ge0$），严格递增只是让 $\Delta=0$ 时也成立得自然。本题 $len_i\ge1$ 保证严格递增。

### 2.6 引理 2：最优决策点随 $i$ 单调不减

记 $\mathrm{opt}(i)$ 是让 $f_i$ 取到最小值的 $j$（并列时取最小的那个）。

**引理 2**：存在一组最优决策点满足 $\mathrm{opt}(1)\le\mathrm{opt}(2)\le\dots\le\mathrm{opt}(N)$。

**证明**：反设存在 $c<d$ 且 $\mathrm{opt}(c)=b>\mathrm{opt}(d)=a$（于是 $a<b$，且 $a,b<c$）。由最优性：

$$
f_b+w(b,c)\le f_a+w(a,c),\tag{1}
$$
$$
f_a+w(a,d)\le f_b+w(b,d).\tag{2}
$$

$(1)+(2)$ 得

$$
w(b,c)+w(a,d)\ \le\ w(a,c)+w(b,d).\tag{3}
$$

而引理 1（取 $a<b$，$c<d$）给出

$$
w(a,c)+w(b,d)\ \le\ w(a,d)+w(b,c).\tag{4}
$$

$(3)$ 与 $(4)$ 方向相反，故两边都取等号。代回 $(2)$ 可知 $b$ 在位置 $d$ 也达到最优（$f_b+w(b,d)=f_a+w(a,d)$），所以可以把 $\mathrm{opt}(d)$ 改成 $b$ 而不破坏最优性。反复执行这种「向右迁移」的修正，最终得到一组单调不减的最优决策点。$\square$

### 2.7 引理 3：单交叉性质（算法真正依赖的东西）

引理 2 说「决策点可以单调」，但 $f_i$ 是边算边用的，不能先知道所有 $\mathrm{opt}$。我们需要一个**在线**可用的更强性质。

**引理 3**：固定两个候选 $j<i$，定义

$$
D(x)=\big(f_i+w(i,x)\big)-\big(f_j+w(j,x)\big),\qquad x>i .
$$

则 $D(x)$ 关于 $x$ **单调不增**。因此集合 $\{x:\ i\text{ 不劣于 }j\}$ 是一个后缀区间。

**证明**：取 $i<x<x'$，在引理 1 中取 $a=j,\ b=i,\ c=x,\ d=x'$（顺序 $j<i<x<x'$ 成立），得

$$
w(j,x)+w(i,x')\le w(j,x')+w(i,x).
$$

移项得

$$
w(i,x')-w(j,x')\ \le\ w(i,x)-w(j,x),
$$

即 $D(x')\le D(x)$。$\square$

直观地说：新候选 $i$ 一旦在某个位置追上了老候选 $j$，之后就**再也不会输回去**。这正是「维护每个候选的接管区间」的依据。

### 2.8 从单调性到算法：候选栈 + 二分

把每个候选 $j$ 看成「从某个位置开始一直最优」的候选人。用一个栈维护若干候选 $q_0,q_1,\dots,q_t$ 及它们的**接管起点** $st_0<st_1<\dots<st_t$：

$$
q_k\text{ 在区间 }[st_k,\ st_{k+1}-1]\text{ 内是最优的},\qquad q_t\text{ 负责到 }N .
$$

按 $i=1,2,\dots,N$ 处理：

1. **取当前最优**：维护队首指针 `head`，当 $st_{head+1}\le i$ 时右移（说明 $i$ 已进入下一个候选的接管区间）。此时 $q_{head}$ 就是位置 $i$ 的最优决策，算出 $f_i$，记下前驱 `pre[i]`。
2. **插入候选 $i$**（它只能负责 $>i$ 的位置）：
   - 由引理 3，$i$ 对每个老候选的「胜利集合」都是后缀，交集仍是后缀，所以 $i$ 只会从某个位置起接管一段后缀。
   - 从栈顶往下比较：设栈顶为 $t$，它负责区间的起点为 $l=\max(st_{tail},i+1)$。若在 $l$ 处 $i$ 已经不比 $t$ 差（$val(i,l)\le val(t,l)$），由引理 3，$i$ 在 $t$ 的整个剩余区间内都不比 $t$ 差，$t$ 可以永久弹掉。
   - 若栈被弹空，说明 $i$ 从 $i+1$ 起统治一切，重置栈为 $(i,\ i+1)$。
   - 否则，剩下的栈顶 $t$ 在 $l$ 处仍优于 $i$；由于 $D$ 单调不增，用二分找到**第一个** $x\in[l,N]$ 使 $val(i,x)\le val(t,x)$。若存在则把 $(i,x)$ 压栈（此时 $x>l\ge st_{tail}$，起点严格递增，不变量保持）。

**正确性（不变量）**：设处理完位置 $i$ 后栈满足上述区间划分。查询时 `head` 右移得到的是包含 $i$ 的区间对应的候选，由不变量它就是 $i$ 处最优。插入时，被弹掉的候选在剩余整个区间内都被 $i$ 压住（引理 3），不会损失最优性；二分找到的 $x$ 是 $i$ 第一次不劣于栈顶的位置，于是把栈顶区间截断在 $x-1$，$i$ 接管 $[x,N]$。若 $i$ 从未追上栈顶，则不插入。归纳成立。$\square$

**复杂度**：每个候选最多入栈、出栈各一次；每次插入做一次二分，二分中每次 `val` 计算需要 $P$ 次乘法。总时间 $O(N\log N\cdot P)$，空间 $O(N)$。

### 2.9 「Too hard」与数值精度

最小代价可能远大于 $10^{18}$：单行长度偏差可达约 $3\times10^6$，$P=10$ 时一项就有 $\sim10^{66}$。

- 用 `long double` 存 $f$。在 x86-64 上 `long double` 是 80 位扩展精度，有效位 $64$ 位，**所有小于 $2^{64}\approx1.8\times10^{19}$ 的整数都能精确表示**，且两个这样的整数相加（结果仍小于 $2^{64}$）也精确。
- 于是凡是「与 $10^{18}$ 阈值相关」的运算都是精确整数运算，判断 $f_N>10^{18}$ 不会因舍入出错；真正超过 $2^{64}$ 的值已经比阈值大十几个数量级，舍入误差（相对 $10^{-19}$）不可能把它拉回阈值附近。
- 幂用「循环乘 $P$ 次」实现，不用 `pow`，避免浮点 `pow` 的额外误差。
- 输出代价时 `(long long)(f[N]+0.5L)`。

如果改用「把代价截断到 $10^{18}+1$」的整型饱和方案，饱和会破坏代价函数的凸性/Monge 结构，可能让决策单调性失效；因此这里选择**不饱和的 `long double`**，保留完整的凸性。

### 2.10 还原方案

`pre[i]` 记录 $f_i$ 取最优时的切点 $j$。从 $i=N$ 沿 `pre` 往回跳，每次把句子 $pre[i]+1\sim i$ 用单个空格拼成一行，收集后翻转输出即可。由于每一步都取了最优转移，最终代价必为 $f_N$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 排版只由切点决定，行代价写成 $w(j,i)=|A_i-A_j-1-L|^P$，其中 $A_i=S_i+i$ 严格递增。
- **consider easier version** — 先写 $O(N^2)$ 的区间切分 DP，确认正确性。
- **try greedy and fail** — 用长度 $6,6,3$、$L=10,P=2$ 说明「本行更接近 $L$」的贪心会害了下一行。
- **explore its property** — 核 $g(x)=|x|^P$ 凸，导出四边形不等式（Monge）。
- **classic problem solution** — 由 Monge 得决策点单调 + 单交叉，使用「候选栈 + 二分接管点」做在线 1D1D 优化，$O(N\log N\cdot P)$。
- **defer reconstruction** — DP 时只记前驱，最后沿 `pre` 回溯拼行。
- **boundary handling** — `long double` 精确表示 $<2^{64}$ 的整数，用于安全判定 $10^{18}$ 阈值。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n,L,P` | `int` | 句子数、行标准长度、指数 | 读入 | 参数 |
| `s[i]` | `string [100005]` | 第 $i$ 个句子（1-based） | 读入 | sentence |
| `A[i]` | `i64 [100005]` | $A_i=S_i+i$，$A_0=0$ | 递推 `A[i]=A[i-1]+len+1` | 前缀 |
| `f[i]` | `long double [100005]` | 前 $i$ 句的最小代价 | `f[0]=0` | DP |
| `pre[i]` | `int [100005]` | $f_i$ 的最优最后切点 $j$ | — | predecessor |
| `q[]`, `st[]` | `int [100005]` | 候选栈与各自的接管起点 | `q[0]=0, st[0]=1` | queue/start |
| `head,tail` | `int` | 栈内活动区间 $[head,tail]$ | 0 | 指针 |

### 4.2 算法步骤

1. 读入 $T$，对每组数据：
   1. 读入 $n,L,P$ 与 $n$ 个句子；计算 $A_i=A_{i-1}+|s_i|+1$。
   2. 初始化 `f[0]=0`，候选栈 `q[0]=0, st[0]=1`，`head=tail=0`。
   3. 对 $i=1\dots n$：
      - `while(head<tail && st[head+1]<=i) ++head;`
      - `j=q[head];` 计算 `f[i]=val(j,i); pre[i]=j;`
      - 若 $i=n$ 结束；
      - 弹栈：`while(head<=tail)`，取栈顶 `t`、`l=max(st[tail],i+1)`，若 `val(i,l)<=val(t,l)` 则 `--tail`，否则 `break`；
      - 若 `head>tail`，重置为只有 `(i,i+1)`；
      - 否则对栈顶 `t` 在 $[l,n]$ 上二分第一个满足 `val(i,x)<=val(t,x)` 的 $x$，若存在则压入 `(i,x)`。
   4. 若 `f[n]>1e18` 输出 `Too hard to arrange`；否则输出代价并按 `pre` 回溯输出每一行。
   5. 输出 `--------------------`。

### 4.3 复杂度分析

- **时间复杂度**：$O(N\log N\cdot P)$，每个候选入栈/出栈各一次，每次插入一次二分，二分内每次代价计算 $O(P)$。$N=10^5,P\le10$ 约 $10^7$ 级运算。
- **空间复杂度**：$O(N)$（$A,f,pre,q,st$ 与字符串数组）。

---

## 5. 示例代码

```cpp
//
// 题目：P1912 [NOI2009] 诗人小G
// https://www.luogu.com.cn/problem/P1912
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,L,P;
i64 A[mxn];
long double f[mxn];
int pre[mxn];
int q[mxn],st[mxn];
string s[mxn];

long double pw(long double b,int e)
{
	long double r=1;
	while(e--)
		r*=b;
	return r;
}

long double val(int j,int i)
{
	i64 d=A[i]-A[j]-1-L;
	if(d<0)
		d=-d;
	return f[j]+pw((long double)d,P);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
	{
		cin>>n>>L>>P;
		A[0]=0;
		for(int i=1;i<=n;++i)
		{
			cin>>s[i];
			A[i]=A[i-1]+(i64)s[i].size()+1;
		}

		f[0]=0;
		int head{0},tail{0};
		q[0]=0;
		st[0]=1;
		for(int i=1;i<=n;++i)
		{
			while(head<tail&&st[head+1]<=i)
				++head;
			int j=q[head];
			f[i]=val(j,i);
			pre[i]=j;

			if(i==n)
				break;

			// 弹掉所有「在剩余区间内都被 i 压制」的候选
			while(head<=tail)
			{
				int t=q[tail];
				int l=max(st[tail],i+1);
				if(val(i,l)<=val(t,l))
					--tail;
				else
					break;
			}
			if(head>tail)
			{
				head=0;
				tail=0;
				q[0]=i;
				st[0]=i+1;
				continue;
			}
			int t=q[tail];
			int l=max(st[tail],i+1);
			int lo=l,hi=n+1;
			while(lo<hi)
			{
				int mid=(lo+hi)>>1;
				if(val(i,mid)<=val(t,mid))
					hi=mid;
				else
					lo=mid+1;
			}
			if(lo<=n)
			{
				++tail;
				q[tail]=i;
				st[tail]=lo;
			}
		}

		if(f[n]>1e18L)
			cout<<"Too hard to arrange\n";
		else
		{
			cout<<(i64)(f[n]+0.5L)<<'\n';
			vector<string> lines;
			for(int cur=n;cur>0;)
			{
				int j=pre[cur];
				string line;
				for(int k=j+1;k<=cur;++k)
				{
					if(k>j+1)
						line+=' ';
					line+=s[k];
				}
				lines.push_back(line);
				cur=j;
			}
			reverse(lines.begin(),lines.end());
			for(const string&l:lines)
				cout<<l<<'\n';
		}
		cout<<"--------------------\n";
	}
	return 0;
}
```
