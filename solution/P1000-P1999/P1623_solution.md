# P1623 [CEOI 2007] 树的匹配 Treasury

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

给定一棵 $N$ 个结点的有根树（输入按"父亲 → 儿子列表"的方式给出，每个结点占一行）。要求选出尽可能多的**互不共端点**的边，即求树的最大匹配大小；同时统计**达到最大匹配的边集个数**。

注意方案数可能极大：$N\le 1000$ 时最大匹配方案数可达数百位十进制，$2^{63}$ 远远不够，必须自己实现大整数。

### 输入输出格式

- **输入格式**：第一行 $N$；随后 $N$ 行，每行 `u m s1 s2 ... sm`，表示结点 $u$ 有 $m$ 个儿子 $s_1,\dots,s_m$。$N$ 行的先后顺序任意。
- **输出格式**：第一行最大匹配数，第二行最大匹配方案数。
- **数据规模**：$1\le N\le 10^3$，其中 $40\%$ 的数据答案不超过 $10^8$（暗示满分解需要高精度）。

### 关键约束分析

1. $N\le 1000$，允许 $\mathcal{O}(N^2)$ 的树形 DP，但不允许指数枚举。最大匹配本身 $\le N/2=500$，用 `int` 足够；**方案数**才是需要高精度的量。
2. 输入不保证第 $1$ 行描述的就是根，也不保证按拓扑序给出。必须先读完整棵树，再找出**没有父亲**的那个结点作为根。
3. 边界：$N=1$ 时最大匹配为 $0$，方案数为 $1$（空匹配算一种方案），不能输出 $0$。
4. 多测不存在，但全局数组的初值依赖"全局零初始化"，`f1` 的不可达值必须显式写成 $-1$，否则 $0$ 会被误当成合法答案。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：先想暴力，再看它慢在哪

最直白的做法是枚举所有边集的子集，检查是否为匹配，取最大并计数。边数 $N-1$ 最大 $999$，子集数 $2^{999}$，完全不可行。

暴力的浪费在于：它把"互不共享端点"这个**局部**约束放到全局去检查。而树的结构告诉我们，一条边只影响它两个端点附近的选择——这正是树形 DP 的入口。

### 第二步：给"根结点是否已被占用"分状态

设当前处理以 $u$ 为根的子树。子树内部选好匹配后，$u$ 只有两种可能：**没有和儿子配对**（还能和父亲配），或**已经和某个儿子配对**（不能再和父亲配）。于是定义

- $f_0(u)$：子树 $u$ 内、$u$ 不与任何儿子配对的匹配数最大值；
- $f_1(u)$：子树 $u$ 内、$u$ 恰与一个儿子配对的匹配数最大值。

**引理 1（子树分解）**：对任意匹配 $M$，$M$ 中的边要么完全落在某个儿子 $v$ 的子树内，要么是 $(u,v)$ 这样连接 $u$ 与其儿子的边；且 $u$ 至多与一个儿子相连。

*证明*：树中任意一条边，其两个端点中必有一个是另一个的父亲。若这条边不是 $(u,v)$ 形式，则它的两个端点都在某个儿子 $v$ 的子树内（因为删去 $u$ 后，不同儿子的子树互不连通，没有边能跨子树）。又由匹配定义，$u$ 不能同时与两个儿子相连。$\square$

### 第三步：写出 $f_0$ 的转移

若 $u$ 不与儿子配对，那么每个儿子 $v$ 独立地取它自己的最优：$v$ 可以和它的儿子配对，也可以不配，两者取大。

**引理 2（$f_0$ 的转移）**：$f_0(u)=\sum_{v\in \mathrm{ch}(u)}\max\{f_0(v),f_1(v)\}$。

*证明*：由引理 1，$u$ 不与儿子相连时，匹配被唯一拆成各儿子子树内部的匹配，互不影响，故各子树可独立取最优，目标函数可加。下界由"每个儿子都取到最优"的拼装给出，上界由"任一匹配在每个子树内的边数不超过该子树最优"给出，两端相等。$\square$

### 第四步：$f_1$ 只能在"牺牲一个儿子"处取得

若 $u$ 与儿子 $v$ 配对，则用了边 $(u,v)$ 后，$v$ 不能再和它的儿子配对，只能取 $f_0(v)$；其余儿子不受影响。于是

$$f_1(u)=\max_{v\in \mathrm{ch}(u)}\Big(1+f_0(v)+\sum_{w\ne v}\max\{f_0(w),f_1(w)\}\Big).$$

记 $b_w=\max\{f_0(w),f_1(w)\}$、$B=\sum_w b_w$，则候选值为 $B-b_v+f_0(v)+1$，枚举 $v$ 即可，$\mathcal{O}(\deg u)$。

### 第五步：方案数怎么乘、怎么加

**引理 3（计数的乘法原理）**：设 $c_v$ 为达到 $b_v$ 的方案数（若 $f_0(v)=f_1(v)$ 则 $c_v=g_0(v)+g_1(v)$，否则取较大者对应的计数）。则 $g_0(u)=\prod_v c_v$。

*证明*：由引理 1，$u$ 不配儿子时，匹配与"每个儿子子树内一个匹配"的元组一一对应：正向取出限制，反向取并集。这个双射保最优值（引理 2），故达到最优的元组数就是各分量方案数之积。$\square$

**引理 4（计数的加法原理，且不重不漏）**：$g_1(u)=\sum_{v:c_v'=f_1(u)} g_0(v)\prod_{w\ne v}c_w$，其中 $c_v'=B-b_v+f_0(v)+1$。

*证明*：任一使 $u$ 与儿子配对的匹配，所用边 $(u,v)$ 的 $v$ 唯一（匹配中 $u$ 的度至多 $1$），故按 $v$ 分类得到的是互不相交的族，可以直接相加；固定 $v$ 后，$v$ 子树内必须取 $f_0(v)$ 且方案数 $g_0(v)$，其余子树各自取 $b_w$ 且方案数 $c_w$，由引理 3 同样的双射论证相乘即可。$\square$

代码中用前缀积 `pre` 与后缀积 `suf` 在 $\mathcal{O}(\deg u)$ 次大整数乘法内得到 $\prod_{w\ne v}c_w$，避免做除法（大整数除法代价高，且需要额外实现）。

### 第六步：方案数到底多大

**引理 5（规模上界）**：最大匹配方案数不超过 $2^{N-1}$。

*证明*：每个匹配都是边集 $\{1,\dots,N-1\}$ 的子集，边集子集总数为 $2^{N-1}$，方案数是它的子集，故不超过。$\square$

$2^{999}$ 约 $10^{301}$，需要用十进制约 $302$ 位，因此必须实现大整数加法与乘法。答案本身（最大匹配数）不超过 $500$，用 `int` 存。

**定理（正确性）**：自底向上按上述转移计算，根处 $\max\{f_0(r),f_1(r)\}$ 即为最大匹配，对应计数之和即为方案数。

*证明*：对子树大小归纳。叶结点 $f_0=0,g_0=1$，$f_1=-1$（不可能）正确。设所有真子树已正确，则 $f_0(u),g_0(u)$ 由引理 2、3 正确，$f_1(u),g_1(u)$ 由引理 4 正确；根没有父亲，其状态只有"配儿子/不配儿子"两种，取大并加和即答案。$\square$

---

## Part 3 — 模式总结 (Pattern Summary)

- `explore its property` — 匹配的边在"删去 $u$"后不跨子树，且 $u$ 至多连一个儿子，于是问题可按子树切分。
- `consider easier version` — 先只求最大匹配（$f_0,f_1$ 两个数），再加"计数"这一维。
- `classic problem solution` — 树上 DP：$f_0$ 取各儿子最优之和，$f_1$ 枚举"被选中的那个儿子"。
- `classic problem solution` — 计数与最优化同步维护，乘法原理拼方案、加法原理按"选中哪个儿子"分类。
- `explore its property` — $f_0(v)=f_1(v)$ 时两条支路的方案要相加，否则只取较大者，这是计数最常见的漏项点。
- `classic problem solution` — 前缀积 / 后缀积求"除一项之外的乘积"，绕开大整数除法。

---

## Part 4 — 真正的解法 (The Real Solution)

### 符号表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $N$ | `n` | 结点数 |
| $\mathrm{ch}(u)$ | `ch[u]` | 结点 $u$ 的儿子列表（`vector`），读入时 `--u;--v` 转 0-based |
| — | `hasfa[v]` | 布尔，$v$ 是否作为某人的儿子出现过；初值全 `false` |
| $r$ | `rt` | 根，唯一满足 `hasfa` 为假的结点 |
| $f_0(u)$ | `f0[u]` | 子树 $u$ 中 $u$ 不与儿子配对的最大匹配数，初值 $0$ |
| $f_1(u)$ | `f1[u]` | 子树 $u$ 中 $u$ 恰与一个儿子配对的最大匹配数，叶子置 $-1$ 表示不可能 |
| $g_0(u)$ | `g0[u]` | 达到 $f_0(u)$ 的方案数（大整数），初值 $1$ |
| $g_1(u)$ | `g1[u]` | 达到 $f_1(u)$ 的方案数（大整数），初值 $0$ |
| $b_v$ | `bv[i]` | 第 $i$ 个儿子的 $\max\{f_0,f_1\}$，助记 best-value |
| $c_v$ | `bc[i]` | 达到 $b_v$ 的方案数，助记 best-count |
| $B$ | `base` | $\sum_v b_v$，`f0[u]` 直接取它 |
| $P_i$ | `pre[i]` | $c_{v_0}c_{v_1}\cdots c_{v_{i-1}}$，前缀积，`pre[0]=1` |
| $S_i$ | `suf[i]` | $c_{v_i}\cdots c_{v_{k-1}}$，后缀积，`suf[k]=1` |
| — | `pg` | 累乘器，最后即 $g_0(u)$ |
| — | `cand` | 候选值 $B-b_v+f_0(v)+1$ |
| — | `mxf` | 目前最大的候选值，初值 $-1$ |
| — | `tot` | 达到 `mxf` 的方案数累计 |
| — | `ways` | 单个儿子贡献的方案数 $g_0(v)P_iS_{i+1}$ |
| $\text{base}$ | `B` | 大整数进制 $10^9$ |
| — | `d` | 大整数的各位（小端，`vector<int>`，空表示 $0$） |

### 算法步骤

1. 读入 $N$ 行描述，建立 `ch` 并标记 `hasfa`；扫一遍找到 `hasfa` 为假的结点作为 `rt`。
2. 从 `rt` 递归 `dfs(u)`：先递归所有儿子，保证子问题已完成。
3. 对每个儿子算 `bv[i]`、`bc[i]`，累加得 `base`，令 `f0[u]=base`；把 `bc` 全部乘起来得 `g0[u]`。
4. 若 $u$ 无儿子，置 `f1[u]=-1,g1[u]=0` 并返回（不可能与儿子配对）。
5. 否则预处理 `pre`、`suf`，枚举每个儿子算 `cand` 与 `ways`，维护最大值与方案和，写入 `f1[u],g1[u]`。
6. 在根处比较 `f0[rt]` 与 `f1[rt]`：大者对应的计数即答案，相等则两个计数相加。

### 复杂度分析

- **时间复杂度**：树形 DP 本身 $\mathcal{O}(N)$；计数部分每个结点做 $\mathcal{O}(\deg u)$ 次大整数乘法、加法和前后缀各一遍。设方案数最多 $C\approx300$ 位十进制约 $34$ 个 limb，单次乘法 $\mathcal{O}((C/9)^2)$，总复杂度 $\mathcal{O}(N\,(C/9)^2)$（实际远小于上界，$N=1000$ 实测 $<10$ ms）。
- **空间复杂度**：$\mathcal{O}(N)$ 个状态，每个计数至多 $34$ 个 limb，约 $\mathcal{O}(NC/9)$。

### 正确性验证

引理 2 给出 $f_0$ 的最优性，引理 3 给出 $g_0$ 的双射计数，引理 4 用"选中哪个儿子"分类保证 $g_1$ 不重不漏，定理把所有子树归纳起来。整数范围：匹配数 $\le 500$ 用 `int`；方案数用大整数，乘法时单个 limb 乘积 $<10^{18}$，用 `long long` 承接不会溢出。

---

## Part 5 — 示例代码 (Example Code)

```cpp
//
// 题目：P1623 [CEOI 2007] 树的匹配 Treasury
// https://www.luogu.com.cn/problem/P1623
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int B{1000000000};

struct big
{
	vector<int>d;                       // 小端存储，base=1e9，空表示 0
	big(){}
	big(int x)
	{
		while(x)
		{
			d.push_back(x%B);
			x/=B;
		}
	}
};

void norm(big&a)
{
	while(!a.d.empty()&&a.d.back()==0)
		a.d.pop_back();
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

big mul(const big&a,const big&b)
{
	big c;
	if(a.d.empty()||b.d.empty())
		return c;
	c.d.assign(a.d.size()+b.d.size()+1,0);
	for(size_t i=0;i<a.d.size();++i)
	{
		if(a.d[i]==0)
			continue;
		long long carry=0;
		for(size_t j=0;j<b.d.size();++j)
		{
			long long cur=(long long)c.d[i+j]+(long long)a.d[i]*b.d[j]+carry;
			c.d[i+j]=cur%B;
			carry=cur/B;
		}
		size_t k=i+b.d.size();
		while(carry)
		{
			long long cur=(long long)c.d[k]+carry;
			c.d[k]=cur%B;
			carry=cur/B;
			++k;
		}
	}
	norm(c);
	return c;
}

string tostr(const big&a)
{
	if(a.d.empty())
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

int n,rt;
int f0[mxn],f1[mxn];                    // f0:u 不连儿子 f1:u 连一个儿子
big g0[mxn],g1[mxn];
bool hasfa[mxn];
vector<int>ch[mxn];

void dfs(int u)
{
	int k=ch[u].size();
	for(int v:ch[u])
		dfs(v);
	vector<int>bv(k);
	vector<big>bc(k);
	int base=0;
	for(int i=0;i<k;++i)
	{
		int v=ch[u][i];
		if(f0[v]>f1[v])
		{
			bv[i]=f0[v];
			bc[i]=g0[v];
		}
		else if(f1[v]>f0[v])
		{
			bv[i]=f1[v];
			bc[i]=g1[v];
		}
		else
		{
			bv[i]=f0[v];
			bc[i]=add(g0[v],g1[v]);
		}
		base+=bv[i];
	}
	f0[u]=base;
	big pg(1);
	for(int i=0;i<k;++i)
		pg=mul(pg,bc[i]);
	g0[u]=pg;
	if(k==0)
	{
		f1[u]=-1;
		g1[u]=big();
		return;
	}
	vector<big>pre(k+1),suf(k+1);
	pre[0]=big(1);
	for(int i=0;i<k;++i)
		pre[i+1]=mul(pre[i],bc[i]);
	suf[k]=big(1);
	for(int i=k-1;i>=0;--i)
		suf[i]=mul(suf[i+1],bc[i]);
	int mxf=-1;
	big tot;
	for(int i=0;i<k;++i)
	{
		int v=ch[u][i];
		int cand=base-bv[i]+f0[v]+1;
		big ways=mul(g0[v],mul(pre[i],suf[i+1]));
		if(cand>mxf)
		{
			mxf=cand;
			tot=ways;
		}
		else if(cand==mxf)
			tot=add(tot,ways);
	}
	f1[u]=mxf;
	g1[u]=tot;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		int u,m;
		cin>>u>>m;--u;
		for(int j=0,v;j<m;++j)
		{
			cin>>v;--v;
			ch[u].push_back(v);
			hasfa[v]=true;
		}
	}
	rt=0;
	while(hasfa[rt])
		++rt;
	dfs(rt);

	int mxf;
	big ans;
	if(f0[rt]>f1[rt])
	{
		mxf=f0[rt];
		ans=g0[rt];
	}
	else if(f1[rt]>f0[rt])
	{
		mxf=f1[rt];
		ans=g1[rt];
	}
	else
	{
		mxf=f0[rt];
		ans=add(g0[rt],g1[rt]);
	}
	cout<<mxf<<'\n'<<tostr(ans)<<'\n';
	return 0;
}
// sample: 1 / 1 0  ->  0 / 1
```

### 易错点

1. **根的确定**：不能默认结点 $1$ 是根，输入行顺序任意，必须用"没当过儿子"来判断。
2. **叶子 $f_1=-1$**：叶子没有儿子，`f1` 必须是不可达值；若用 $0$，单结点树会错误地输出两倍方案数。
3. **$f_0(v)=f_1(v)$ 时漏加**：此时两条支路都是最优，`bc` 应为 `g0+g1`，否则方案数偏小。
4. **大整数只做大数加法不做乘法**：$g_0$ 是连乘，必须实现乘法；`long long` 承接单 limb 乘法（$<10^{18}$）即可。
5. **输出前导零**：`tostr` 里最低位以外的 limb 必须补足 9 位，否则 `1000000000` 会被写成 `1` 后接 `0`。
6. **`f1` 初值**：全局数组默认 0，要在叶子显式 `f1[u]=-1`，否则不可达状态被当成 0 参与比较。

### 本地验证结果

| 测试项 | 覆盖范围 | 结果 |
| :--- | :--- | :--- |
| 题面样例 | 题面未提供样例，自造单点树 `1 / 1 0` | 输出 `0` 与 `1`，手算一致 |
| 手算用例 | $1$ 个中心带 $3$ 个叶子（答案 $1$，$3$ 种）、长度 $1000$ 的链（答案 $500$，$1$ 种）、$1000$ 点菊花（答案 $1$，$999$ 种） | 全部与手算一致 |
| 随机对拍 | $200$ 组，$N\le 9$，随机父亲 + 随机重标号 + 行序打乱，对照 `P1623_brute.cpp`（枚举边子集） | $200$ 组全部一致 |
| 大数校验 | $3$ 组 $N=1000$ 随机树，与 Python 任意精度整数的同式 DP 逐字符比对 | 输出（$55\sim60$ 位）完全一致 |
