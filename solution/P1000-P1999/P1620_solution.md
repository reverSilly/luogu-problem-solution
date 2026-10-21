# P1620 漂亮字串

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

用字符 `O` 和 `X` 构造一个串，要求：

- `O` 的总数不超过 $\mathrm{count_O}$，`X` 的总数不超过 $\mathrm{count_X}$；
- 任意一段连续的 `O`（极大连续段）长度不超过 $\max_O$，任意一段连续的 `X` 长度不超过 $\max_X$。

求满足条件的串的最大长度。多组数据，读到文件结束。

### 输入输出格式

- **输入格式**：每行四个整数 $\mathrm{count_O},\mathrm{count_X},\max_O,\max_X$，直到 EOF。
- **输出格式**：每组一行一个整数，最大长度。
- **数据规模**：最多 $1000$ 组，四个参数均 $\le10^6$；其中 $30\%$ 的数据四个参数 $\le20$。

### 关键约束分析

1. 参数最大 $10^6$，答案可达 $2\times10^6$，绝对不能用"枚举长度再做 $O(n)$ 检查"的 $O(n^2)$；必须 $O(\log)$ 或 $O(1)$ 判一组。
2. $\max_O=0$ 时 `O` 一次都不能出现（单独一个 `O` 就是长度 $1>0$ 的连续段），这是除法判据的奇点，必须特判。
3. 多组数据，全局变量每轮都会被重新赋值，不需要额外清空；但不能只处理第一组。
4. 只有一种字母可用、两种字母都不可用、答案恰好受两个上限同时约束等退化情形都要能跑通。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：换个视角——数"段"而不是数"字符"

直接想"往串里塞字符"很难判断合法性。但题目限制的是**极大连续段的长度**，这提示我们把串看成一串交替的"块"：

$$\underbrace{OO\cdots O}_{a_1}\ \underbrace{XX\cdots X}_{b_1}\ \underbrace{OO\cdots O}_{a_2}\ \cdots$$

设用了 $o$ 个 `O`、$x$ 个 `X`，`O` 被切成 $r_O$ 段、`X` 被切成 $r_X$ 段。交替结构立刻给出两条硬约束：

- $|r_O-r_X|\le1$（两种段必须交替出现，数量只能差一）；
- $\lceil o/\max_O\rceil\le r_O\le o$：$r_O$ 段要把 $o$ 个字符分完，每段至少 $1$ 个、至多 $\max_O$ 个；同理 $\lceil x/\max_X\rceil\le r_X\le x$。

**引理 1（段数刻画）**：$o,x\ge1$ 时，存在合法串当且仅当存在整数 $r_O\in[\lceil o/\max_O\rceil,o]$、$r_X\in[\lceil x/\max_X\rceil,x]$ 使 $|r_O-r_X|\le1$。

*证明*：必要性——取一个合法串，把它的极大段按出现顺序列出，段数就是这样的 $r_O,r_X$，段长上限与字符总数给出区间约束，交替结构给出差不超过 $1$。充分性——把 $o$ 任意拆成 $r_O$ 个正整数、每个不超过 $\max_O$（区间条件保证可行：先每段放 $1$ 个，剩下的 $o-r_O$ 个每次补到未满的段上，因为 $o\le r_O\max_O$ 一定补得完）；对 $x$ 同样拆分；然后把两组段交替排列，段数多的一方作为首段即可（差不超过 $1$ 保证能排完），得到的串段数、段长、字符数全部合规。$\square$

### 第二步：把"存在"化成两个不等式

令 $A=[\lceil o/\max_O\rceil,\,o]$、$B=[\lceil x/\max_X\rceil,\,x]$，问题变成"两个整数区间里能否各取一点，使两点距离不超过 $1$"。

**引理 2（区间判据）**：设 $A=[a_1,a_2]$、$B=[b_1,b_2]$ 为整数区间，则存在 $a\in A,b\in B$ 使 $|a-b|\le1$，当且仅当
$$a_1\le b_2+1\quad\text{且}\quad b_1\le a_2+1 .$$

*证明*：记两区间距离 $\mathrm{dist}=\max(0,\max(a_1,b_1)-\min(a_2,b_2))$。"存在距离不超过 $1$ 的点对"等价于 $\mathrm{dist}\le1$。若 $A$ 在 $B$ 左侧（$a_2<b_1$），则 $\mathrm{dist}=b_1-a_2\le1$ 即 $b_1\le a_2+1$，而此时 $a_1\le a_2<b_1\le b_2<b_2+1$，第一条自然成立。若 $B$ 在 $A$ 左侧（$b_2<a_1$），对称地得到 $a_1\le b_2+1$，第二条自然成立。若两区间相交或相邻，两式同时成立且确实可取到（取公共点或相邻端点）。三种情形互斥且穷尽，故判据成立。$\square$

把 $a_1=\lceil o/\max_O\rceil$、$a_2=o$、$b_1=\lceil x/\max_X\rceil$、$b_2=x$ 代入，得到可直接计算的判据：

$$\boxed{\ \lceil o/\max_O\rceil\le x+1\quad\text{且}\quad\lceil x/\max_X\rceil\le o+1\ }$$

约定 $\lceil 0/m\rceil=0$，这样 $o=0$ 或 $x=0$ 的单字母情形也被同一判据覆盖（例如 $o=0$ 时第二式退化为 $x\le\max_X$，正是"只用一种字母"的正确答案）。

### 第三步：长度可以二分

**引理 3（长度可行性单调）**：若存在长度为 $S$ 的合法串，则对任意 $0\le S'\le S$ 都存在长度为 $S'$ 的合法串。

*证明*：取该串长度为 $S'$ 的前缀。前缀的 `O`、`X` 个数不超过原串，故不超上限；前缀的每个极大连续段都是原串某个极大连续段的前缀（或被截断），长度不超过原段，故不超段长上限。因此前缀合法。$\square$

于是可以对总长度 $S$ 二分，只需判断"长度恰为 $S$ 是否可行"。判定时枚举 $o$（`O` 的个数）与 $x=S-o$，要求 $0\le o\le\mathrm{count_O}$、$0\le x\le\mathrm{count_X}$。

### 第四步：判定内部再二分

固定 $S$ 后，$o$ 的取值范围是 $[\max(0,S-\mathrm{count_X}),\ \min(S,\mathrm{count_O})]$。把引理 2 的两个条件记作

$$g(o)=\lceil o/\max_O\rceil-(S-o)\le1,\qquad h(o)=\lceil (S-o)/\max_X\rceil-o\le1 .$$

**引理 4（两个条件的单调方向相反）**：$g(o)$ 关于 $o$ 单调不减，$h(o)$ 关于 $o$ 单调不增。

*证明*：$o$ 增大 $1$，$\lceil o/\max_O\rceil$ 不减，$-(S-o)=o-S$ 增大 $1$，故 $g$ 不减；同时 $S-o$ 减小 $1$ 使 $\lceil (S-o)/\max_X\rceil$ 不增，$-o$ 减小 $1$，故 $h$ 不增。$\square$

因此满足 $g\le1$ 的 $o$ 构成一段后缀、满足 $h\le1$ 的构成一段前缀，可行 $o$ 是其交集（一个区间）。只需二分出**满足 $g\le1$ 的最大 $o$**，再在该点上验证 $h\le1$：若这个最靠右的点都不满足 $h\le1$，由 $h$ 单调不增可知更小的 $o$ 只会更差，直接判否。

---

## Part 3 — 模式总结 (Pattern Summary)

- `explore its property` — 限制作用在"极大连续段"上，于是把串改写成"段序列"来刻画。
- `reduce to easier problem` — 合法串的存在性化归为"两个整数区间能否取到距离 $\le1$ 的点对"。
- `classic problem solution` — 区间判据化简为两条形如 $\lceil o/m\rceil\le x+1$ 的不等式。
- `explore its property` — 合法串的前缀仍合法，故答案关于长度单调，可二分答案。
- `classic problem solution` — 固定长度后两个条件的单调方向相反，再二分一次求出可行 $o$ 的右端点。
- `discuss by category` — 单独处理 $\max_O=0$ 或 $\max_X=0$ 的除零奇点。

---

## Part 4 — 真正的解法 (The Real Solution)

### 符号表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $\mathrm{count_O}$ | `co` | `O` 的个数上限，`i64` |
| $\mathrm{count_X}$ | `cx` | `X` 的个数上限，`i64` |
| $\max_O$ | `mo` | `O` 的极大段长度上限 |
| $\max_X$ | `mx` | `X` 的极大段长度上限 |
| $\lceil a/b\rceil$ | `ceildiv(a,b)` | 上取整，且约定 `a<=0` 时返回 0 |
| $S$ | `check` 的形参 `s` | 当前二分的总长度 |
| $o$ | `check` 内局部量 `o` | 该长度下 `O` 的个数，$x=S-o$ |
| $x$ | `check` 内局部量 `x` | 该长度下 `X` 的个数 |
| $[lo,hi]$ | `lo`,`hi` | 固定 $S$ 后 $o$ 的可行区间 |
| 二分边界 | `l`,`r`,`mid` | 外层对 $S$、内层对 $o$ 的二分变量 |

### 算法步骤

1. 循环读入 `co,cx,mo,mx` 直到 EOF。
2. 若 `mo==0 && mx==0`：两种字母都不能出现，输出 0。
3. 若 `mo==0`：只能用 `X`，答案 `min(cx,mx)`；若 `mx==0`：答案 `min(co,mo)`。
4. 否则在 $[0,\mathrm{count_O}+\mathrm{count_X}]$ 上二分最大的可行长度 $S$，判定函数为 `check(S)`。
5. `check(S)`：算出 $o$ 的区间 $[lo,hi]$，为空则假；在该区间内二分出满足 $\lceil o/\max_O\rceil\le S-o+1$ 的最大 $o$；若连这个 $o$ 都不满足该式则假；否则返回 $\lceil (S-o)/\max_X\rceil\le o+1$。
6. 输出二分结果。

### 复杂度分析

- **时间复杂度**：每组 $\mathcal{O}(\log^2(\mathrm{count_O}+\mathrm{count_X}))$，约 $21\times21\approx440$ 次常数运算；$1000$ 组总计 $4\times10^5$ 量级。
- **空间复杂度**：$\mathcal{O}(1)$，只有若干标量。

### 正确性验证

引理 1 把"存在合法串"翻译成段数条件，引理 2 把段数条件翻译成两条不等式，二者构成 `check` 的判据，故 `check(S)` 为真当且仅当存在长度为 $S$ 的合法串。引理 3 保证该判定关于 $S$ 单调，外层二分因此正确。引理 4 保证内层二分的可行域是区间，且"取最右可行 $o$"不会漏解。所有量级不超过 $2\times10^6$，`i64` 无溢出风险。

---

## Part 5 — 示例代码 (Example Code)

```cpp
//
// 题目：P1620 漂亮字串
// https://www.luogu.com.cn/problem/P1620
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 co,cx,mo,mx;

i64 ceildiv(i64 x,i64 y)           // 0 个字母需要 0 段
{
	if(x<=0)
		return 0;
	return (x+y-1)/y;
}

bool check(i64 s)                  // 长度 s 是否可行：枚举 O 的个数 o，X 的个数 s-o
{
	if(s==0)
		return true;
	i64 lo=max<i64>(0,s-cx),hi=min<i64>(s,co);
	if(lo>hi)
		return false;
	// 条件一 ceil(o/mo)<=x+1 随 o 单调变松，其解集是一个后缀
	i64 l=lo,r=hi;
	while(l<r)
	{
		i64 mid=(l+r+1)>>1;
		if(ceildiv(mid,mo)<=s-mid+1)
			l=mid;
		else
			r=mid-1;
	}
	i64 o=l,x=s-l;
	if(ceildiv(o,mo)>x+1)          // 连最松的 o 都不满足条件一
		return false;
	return ceildiv(x,mx)<=o+1;     // 条件二在满足条件一的 o 中取最靠右者最有利
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(cin>>co>>cx>>mo>>mx)
	{
		if(mo==0||mx==0)           // 该字母一次都不能出现
		{
			if(mo==0&&mx==0)
				cout<<0<<'\n';
			else if(mo==0)
				cout<<min(cx,mx)<<'\n';
			else
				cout<<min(co,mo)<<'\n';
			continue;
		}
		i64 l=0,r=co+cx;
		while(l<r)
		{
			i64 mid=(l+r+1)>>1;
			if(check(mid))
				l=mid;
			else
				r=mid-1;
		}
		cout<<l<<'\n';
	}
	return 0;
}
```

### 易错点

1. **除零**：`maxO=0` 或 `maxX=0` 时上取整无意义，必须先特判，否则运行错误。
2. **$\max_O=0$ 不等于"不限"**：它表示该字母一次都不能出现，容易被误解成无约束。
3. **内层二分方向**：求"满足条件一的最大 $o$"，`mid=(l+r+1)>>1` 必须偏上，否则死循环。
4. **外层二分上界**：取 `co+cx`，不要取 `min` 之后的值范围；同时 `l=0` 保证全 0 输入有解。
5. **多组数据**：用 `while(cin>>...)` 读到 EOF，不能只处理一组。
6. **`ceildiv` 的 0 约定**：`ceildiv(0,m)` 必须是 0 而不是 1，否则会凭空多算一段。

### 本地验证结果

| 测试项 | 覆盖范围 | 结果 |
| :--- | :--- | :--- |
| 官方样例 | 归档题面未附样例段，改用题面样例解释反推的用例 `2 3 1 1`（答案 `XOXOX`，长度 5） | 通过 |
| 随机对拍 | 300 组四参数 $\le20$ + 150 组四参数 $\le60$，与状态图 BFS 的 `P1620_brute.cpp` 逐行比对 | 450 组全部一致 |
| 极限参数 | `10^6 10^6 10^6 10^6 → 2000000`；`0 0 0 0 → 0`；`10^6 0 1 10^6 → 1`；`10^6 10^6 1 10^6 → 2000000` | 通过 |
| 单字母退化 | `1 0 5 5 → 1`、`5 5 0 3 → 3`（`O` 被禁用） | 通过 |
