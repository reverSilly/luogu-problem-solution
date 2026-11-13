# P2440 木材加工 — 题解

## 一、解题过程：人类思考叙事

题意：$n$ 根原木，长度 $L_i$ 为正整数。要把它们切成 $k$ 段**长度都等于 $l$** 的小段（$l$ 也是正整数，允许有剩余），求 $l$ 的最大值；如果连 $1\text{cm}$ 都凑不出 $k$ 段，输出 `0`。范围：$n\le10^5$，$k\le10^8$，$L_i\le10^8$。题面自带一个小例子：$L=[11,21]$，$k=6$，答案 $5$。

**第一反应：枚举 $l$。** $l$ 最大能到 $10^8$，每次枚举要扫一遍 $n$ 根原木算能切多少段，$10^8\times10^5=10^{13}$，彻底没戏。必须找到某种单调性来二分。

**第二反应：$l$ 是不是「总长除以 $k$」？** $\lfloor\sum L_i/k\rfloor$ 看起来很像答案，但它是**上界而不是答案**。反例：$L=[10,10]$，$k=3$。$\sum L/k=6$，可 $l=6$ 时每根只能切 $\lfloor10/6\rfloor=1$ 段，共 $2<3$ 段，不够；真正的答案是 $l=5$（每根 $2$ 段，共 $4\ge3$）。**碎料不能拼接**，每根原木尾部都会剩下不足 $l$ 的一段浪费掉，所以平均值会高估。这个反例把「直接算公式」的路堵死了，也点出了本题的核心量：

$$f(l)=\sum_{i=1}^{n}\left\lfloor \frac{L_i}{l}\right\rfloor .$$

**关键性质：$f$ 关于 $l$ 单调不增。** $l$ 越大，每根能切的段数越少（或不变），总和自然不增。于是「$l$ 可行」这个谓词 $P(l)=[f(l)\ge k]$ 具有**前缀性**：若 $l$ 可行，则所有比它小的正整数也可行。可行的 $l$ 恰好是 $\{1,2,\dots,\text{ans}\}$（或空集）。这正是二分答案的标准形状——**把最优化问题化归为判定问题**。

**边界在哪？** 上界：$l>\max L_i$ 时每根都切不出 $1$ 段，$f(l)=0<k$（$k\ge1$），所以答案 $\le\max L_i$。下界与无解：$f(1)=\sum L_i$，若它都 $<k$，说明连 $1\text{cm}$ 的段都凑不够，输出 `0`。这一条必须**先判**，否则二分会在一个全不可行的区间里乱跑。

**二分的写法坑。** 求「最大的可行 $l$」时，若写 `mid=(l+r)>>1` 配 `l=mid`，当 `r=l+1` 时 `mid=l`，`check(mid)` 为真会让 `l=mid=l`，区间不缩小 ⇒ **死循环**。必须偏上界取 `mid=(l+r+1)>>1`，配 `l=mid / r=mid-1`。这是二分答案最常见的翻车点。

**数值坑。** $f(1)=\sum L_i$ 最大 $10^5\times10^8=10^{13}$，远超 `int`。累加器要用 `long long`。另一个小技巧：一旦 `cnt>=k` 就立刻 `return true`——既省去后续扫描（$k$ 很小时极快），又把累加器的值卡在 $k+\max\lfloor L_i/l\rfloor\le2\times10^8$ 以内。即便如此，仍用 `i64` 更稳妥。

**规模核算。** 二分 $\lceil\log_2 10^8\rceil=27$ 轮，每轮 $O(n)=10^5$ 次除法，约 $2.7\times10^6$ 次运算，远远够用。实测 $n=10^5$、$L_i$ 随机 $\le10^8$、$k=10^8$ 时耗时 $0.06$s。

顺手验证题面例子：$L=[11,21],k=6$。$f(5)=\lfloor11/5\rfloor+\lfloor21/5\rfloor=2+4=6\ge6$ ✓；$f(6)=1+3=4<6$ ✗。答案 $5$，与题面一致。

---

## 二、思维链：完整证明

记 $L_1,\dots,L_n$ 为原木长度（正整数），$k\ge1$ 为需要的段数，$l$ 为正整数候选长度。定义
$$f(l)=\sum_{i=1}^{n}\left\lfloor\frac{L_i}{l}\right\rfloor,\qquad P(l)=[\,f(l)\ge k\,].$$

### 引理 1（单根原木的最大段数）

从一根长度为 $L$ 的原木中切出长度为 $l$ 的小段，最多能切 $\lfloor L/l\rfloor$ 段，且这个数目可以达到。

*证明*：
**上界**：切出的 $t$ 段是原木上互不重叠的子区间，每段长 $l$，故总长 $tl\le L$，即 $t\le L/l$。$t$ 是整数，故 $t\le\lfloor L/l\rfloor$。
**可达**：令 $t=\lfloor L/l\rfloor$，依次切下 $[0,l],[l,2l],\dots,[(t-1)l,tl]$。由 $tl\le L$ 知这些区间都落在原木内且互不重叠，故确实切出 $t$ 段，剩余 $L-tl<l$ 作为废料。证毕。

### 引理 2（不可拼接 ⇒ 总数可加；「恰好 $k$ 段」等价于「至少 $k$ 段」）

用长度 $l$ 切割全部原木，能得到的长度为 $l$ 的段数最大值恰为 $f(l)$；并且「能切出 $k$ 段长度为 $l$ 的木头」当且仅当 $P(l)$ 为真。

*证明*：
**可加性**：题目只允许切割、不允许把不同原木的碎料拼接，故每一段小段必完整来自某一根原木。任一全局切割方案在每根原木上诱导一个局部方案，由引理 1 第 $i$ 根至多贡献 $\lfloor L_i/l\rfloor$ 段，求和得全局段数 $\le f(l)$。反之，对每根原木独立采用引理 1 的可达构造，把它们的段合起来即得 $f(l)$ 段，故最大值就是 $f(l)$。
**等价性**：若 $f(l)\ge k$，从这 $f(l)$ 段中任取 $k$ 段即可（其余部分留在原木上作为剩余，题目明确允许「木头有可能有剩余」），故能切出 $k$ 段。若能切出 $k$ 段，则最大段数 $f(l)\ge k$。两方向都得证。证毕。

> 这条引理把「恰好 $k$」这个看似麻烦的要求化成了「$\ge k$」，是能把判定写成简单求和的前提。

### 引理 3（单调性与前缀性）

若 $1\le l_1\le l_2$，则 $f(l_1)\ge f(l_2)$；从而 $P(l_2)\Rightarrow P(l_1)$。可行长度集合 $S=\{l\ge1:P(l)\}$ 要么是空集，要么恰为 $\{1,2,\dots,\max S\}$。

*证明*：对每个 $i$，$l_1\le l_2\Rightarrow L_i/l_1\ge L_i/l_2$。取整函数 $\lfloor\cdot\rfloor$ 单调不减（若 $x\ge y$ 而 $\lfloor x\rfloor<\lfloor y\rfloor$，则 $\lfloor x\rfloor+1\le\lfloor y\rfloor\le y\le x$，与 $\lfloor x\rfloor$ 是不超过 $x$ 的最大整数矛盾），故 $\lfloor L_i/l_1\rfloor\ge\lfloor L_i/l_2\rfloor$。逐项求和得 $f(l_1)\ge f(l_2)$。于是 $f(l_2)\ge k\Rightarrow f(l_1)\ge k$，即 $P(l_2)\Rightarrow P(l_1)$。

若 $S\ne\varnothing$，令 $A=\max S$（$S$ 有上界，见引理 4，故最大值存在）。对任意 $1\le l\le A$，由前缀性 $P(A)\Rightarrow P(l)$，故 $l\in S$；而 $l>A$ 时 $l\notin S$（否则与 $A$ 的最大性矛盾）。所以 $S=\{1,\dots,A\}$。证毕。

**这条引理是二分的合法性依据**：判定谓词在整数轴上「真的一段 + 假的一段」，没有真假交错。

### 引理 4（上下界与无解判定）

(a) 若 $l>\max_i L_i$，则 $f(l)=0$，故 $P(l)$ 假；因此 $S\subseteq[1,\max_i L_i]$。
(b) $P(1)$ 假 $\iff \sum_i L_i<k \iff S=\varnothing$，此时答案为 $0$。
(c) 更强的上界：若 $P(l)$ 真，则 $l\le\lfloor\sum_i L_i/k\rfloor$。

*证明*：
(a) $l>\max L_i\ge L_i$ 对每个 $i$ 成立，故 $0<L_i/l<1$，$\lfloor L_i/l\rfloor=0$，$f(l)=0$。而 $k\ge1$，故 $P(l)$ 假。
(b) $f(1)=\sum_i\lfloor L_i/1\rfloor=\sum_i L_i$。由引理 3 的前缀性，$P(1)$ 假意味着没有任何 $l\ge1$ 可行（否则 $1$ 也可行），即 $S=\varnothing$；反之 $S=\varnothing$ 显然 $P(1)$ 假。题目规定此时输出 $0$。
(c) $P(l)$ 真即 $\sum_i\lfloor L_i/l\rfloor\ge k$。又 $\lfloor L_i/l\rfloor\le L_i/l$，故 $\sum_i L_i/l\ge k$，即 $l\le\sum_i L_i/k$；$l$ 为整数，故 $l\le\lfloor\sum_i L_i/k\rfloor$。证毕。

> (c) 解释了为什么「总长除以 $k$」只是上界：$\lfloor10/6\rfloor+\lfloor10/6\rfloor=2<3$，上界 $6$ 不可达，真实答案 $5$。实现中用 (a) 的 $\max L_i$ 作右端点即可，(c) 可作为可选的更紧上界。

### 引理 5（二分答案的正确性与终止性）

设已知 $P(1)$ 真。取 $l=1,\ r=\max_i L_i$，循环执行：`mid=(l+r+1)/2`（下取整）；若 $P(mid)$ 真则 $l\leftarrow mid$，否则 $r\leftarrow mid-1$；直到 $l=r$。则循环必终止，且终值 $l$ 就是 $\max S$。

*证明*：
**不变量**：$P(l)$ 真，且 $P(r+1)$ 假（当 $r+1\le\max L_i$ 时；若 $r=\max L_i$，由引理 4(a) 知 $P(r+1)$ 亦假）。
初始：$l=1$ 由前提 $P(1)$ 真；$r=\max L_i$，$P(r+1)$ 假由引理 4(a)。
保持：设当前 $l<r$。则 $mid=\lfloor(l+r+1)/2\rfloor$ 满足 $l+1\le mid\le r$（因 $l<r\Rightarrow l+r+1\ge2l+2\Rightarrow mid\ge l+1$；又 $l+r+1\le2r\Rightarrow mid\le r$）。
- 若 $P(mid)$ 真：置 $l=mid$，$P(l)$ 真保持；$r$ 不变，$P(r+1)$ 假保持。
- 若 $P(mid)$ 假：置 $r=mid-1$。需证 $P(r+1)=P(mid)$ 假——正是本分支的前提，成立；$l$ 不变，$P(l)$ 真保持。

**终止性**：两种分支都使 $r-l$ 严格减小（前者 $l$ 增大至少 $1$，后者 $r$ 减小至少 $1$），且始终 $l\le r$。非负整数 $r-l$ 严格递减必在有限步内到 $0$，故循环终止于 $l=r$。

**终值正确**：终止时 $P(l)$ 真且 $P(l+1)$ 假。由引理 3 的前缀性，所有 $\le l$ 的正整数都可行、所有 $>l$ 的都不可行，故 $l=\max S$，即答案。证毕。

**为何必须偏上界取 mid**：若改用 $mid=\lfloor(l+r)/2\rfloor$ 且仍用 `l=mid`，当 $r=l+1$ 时 $mid=l$，$P(mid)$ 真分支使 $l$ 不变、$r$ 不变，$r-l$ 不减小，循环永不终止。偏上界取法保证 $mid\ge l+1$，杜绝了这一退化。

### 引理 6（累加器范围与提前退出）

在 `check(l)` 中逐项累加 $cnt\mathrel{+}=\lfloor L_i/l\rfloor$ 并在 $cnt\ge k$ 时立即返回真：
(a) 返回值与「完整求和后判断 $f(l)\ge k$」一致；
(b) 累加过程中 $cnt\le k-1+\max_i\lfloor L_i/l\rfloor\le k+\max_i L_i\le2\times10^8$；
(c) 若不提前退出，$cnt$ 最大可达 $\sum_i L_i=10^{13}$，必须用 64 位整数。

*证明*：
(a) 各项非负，故部分和单调不减。若某前缀和已 $\ge k$，完整和也 $\ge k$，返回真正确；若扫完全部仍 $<k$，完整和 $<k$，返回假正确。
(b) 提前退出发生在「加完某一项后首次 $\ge k$」，故退出前一刻 $cnt\le k-1$，加上当前项 $\le\max_i\lfloor L_i/l\rfloor\le\max_i L_i\le10^8$，得 $cnt\le k-1+10^8\le2\times10^8-1<2^{31}-1$。
(c) $l=1$ 时 $f(1)=\sum L_i\le10^5\times10^8=10^{13}>2^{31}-1\approx2.1\times10^9$。证毕。

实现统一用 `i64` 累加，(b) 只是说明提前退出额外带来的安全裕度与速度收益。

### 定理（算法正确性）

程序输出即题目所求的最大 $l$（无解时输出 $0$）。

*证明*：由引理 2，「能切出 $k$ 段长度 $l$」等价于 $P(l)$。若 $P(1)$ 假，由引理 4(b) 无解，程序输出 $0$，正确。否则由引理 4(a) 有 $\max S\le\max L_i$，程序在 $[1,\max L_i]$ 上执行引理 5 的二分，终值即 $\max S$。`check` 的返回值由引理 6(a) 保证与 $P$ 一致，累加由引理 6(b)(c) 保证不溢出。证毕。

### 引理 7（复杂度）

时间 $O(n\log(\max_i L_i))$，空间 $O(n)$。

*证明*：二分区间长度初值 $\le\max L_i\le10^8$，每轮 $r-l$ 至少减半（$mid$ 取中点，两分支都把区间缩到不超过原来的一半），故轮数 $\le\lceil\log_2 10^8\rceil+1=28$。每轮 `check` 至多扫 $n$ 项，每项一次整数除法与一次加法，$O(n)$。合计 $O(n\log\max L_i)\le10^5\times28=2.8\times10^6$ 次基本运算。空间只有长度 $n$ 的原木数组与常数个变量，$O(n)$。证毕。

---

## 三、模式总结（按实际思考顺序）

- **explore its property** — 单根原木最多切 $\lfloor L_i/l\rfloor$ 段且可达；碎料不可拼接 ⇒ 总段数 $f(l)=\sum\lfloor L_i/l\rfloor$。
- **consider easier version** — 先放弃「直接求最大 $l$」，改问判定题「给定 $l$，够不够 $k$ 段？」。
- **explore its property** — 用反例 $L=[10,10],k=3$ 打掉「$l=\sum L/k$」的幻想，认清浪费项的存在。
- **explore its property** — $f$ 单调不增 ⇒ 可行 $l$ 构成前缀 $\{1..ans\}$，这是二分的合法性前提。
- **classic problem solution** — 二分答案；求最大可行值必须偏上界取 `mid=(l+r+1)>>1`，否则死循环。
- **discuss by category** — 先判 $f(1)<k$ 输出 `0`；右端点取 $\max L_i$（$l$ 再大就一段也切不出）。
- **explore its property** — 累加器可达 $10^{13}$ 需 `i64`；`cnt>=k` 提前返回既提速又把值域压到 $2\times10^8$。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|------|------|-------------|--------|
| $n$ | 原木根数 | `int`，读入 | $\le10^5$ |
| $k$ | 需要的小段数 | `int`，读入 | $\le10^8$ |
| `a[i]` | 第 $i$ 根原木长度 $L_{i+1}$ | `int[mxn]`，读入 | 内部 0-based |
| `mx` | $\max_i L_i$，二分右端点 | `int`，初值 $0$，读入时取 max | 引理 4(a) |
| `check(l)` | 判定 $P(l)=[f(l)\ge k]$ | `bool` 函数 | 提前退出 |
| `cnt` | `check` 内的段数累加器 | `i64`，每次调用置 $0$ | 防溢出 |
| `l,r` | 二分区间左右端点（闭） | `int`，`l=1,r=mx` | 不变量：$P(l)$ 真、$P(r+1)$ 假 |
| `mid` | 试探长度 | `int`，`(l+r+1)>>1` | **偏上界** |
| `mxn` | $10^5+5$ | `constexpr int` | +5 松弛 |

### 算法步骤

1. I/O 加速：`cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);`。
2. 读入 $n,k$ 与 $n$ 个 $L_i$ 存入 `a`，同时求出 `mx`。
3. 调 `check(1)`：若为假，输出 `0` 并结束（引理 4(b)）。
4. 令 `l=1,r=mx`；当 `l<r` 时取 `mid=(l+r+1)>>1`，`check(mid)` 真则 `l=mid`，否则 `r=mid-1`。
5. 输出 `l`。
6. `check(l)` 内部：`cnt=0`；对 $i=0\dots n-1$ 累加 `a[i]/l`，一旦 `cnt>=k` 立即返回真；扫完返回假。

### 复杂度

- 时间：$O(n\log(\max L_i))\le 10^5\times28\approx2.8\times10^6$ 次运算。
- 空间：$O(n)$，一个 `int[100005]`。

### 验证状态

编译 `g++ -std=c++20 -O2` 通过；题面例子 $L=[11,21],k=6\to5$ 正确；对拍采用「从 $\max L_i$ 往下逐一枚举」的暴力，穷举 $n\le3$、$L_i\le5$、$k\le20$ 共 **3100 组**零不一致，另跑 **300 组**随机数据（$n\le10,L\le60,k\le200$）的真实可执行文件比对零不一致；极端规模 $n=10^5$ 全 $10^8$、$k=10^8\to100000$，$k=1\to10^8$，全 $1$、$k=10^8\to0$ 均正确，随机大数据耗时 $0.06$s，并用独立证书程序验证 $f(l)\ge k>f(l+1)$。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2440 木材加工
// 思路：f(l)=sum(L_i/l) 单调不增，可行 l 构成前缀 ⇒ 二分答案
//       先判 check(1)，不可行输出 0；求最大可行值要偏上界取 mid
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,k,a[mxn];

bool check(int l)
{
	i64 cnt=0;
	for(int i=0;i<n;++i)
	{
		cnt+=a[i]/l;
		if(cnt>=k)return 1;//够 k 段就立刻返回，顺带避免累加过大
	}
	return 0;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	int mx=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		mx=max(mx,a[i]);
	}
	if(!check(1))//连 1cm 都切不出 k 段
	{
		cout<<0<<'\n';
		return 0;
	}
	int l=1,r=mx;
	while(l<r)
	{
		int mid=(l+r+1)>>1;//偏上界取 mid，否则 l=mid 会死循环
		if(check(mid))l=mid;
		else r=mid-1;
	}
	cout<<l<<'\n';
	return 0;
}
```

### 代码说明

- `check` 的提前返回既是常数优化，也把 `cnt` 的值域限制在 $2\times10^8$ 内；但类型仍写 `i64`，因为「不提前退出」的版本会累到 $10^{13}$。
- `mx` 在读入时顺手求出，避免第二次扫描；它是二分右端点的合法上界（引理 4(a)）。
- `check(1)` 失败必须**先判并直接输出 0**：此时整个区间都不可行，二分的不变量 $P(l)$ 真无法建立。
- `mid=(l+r+1)>>1` 与 `l=mid / r=mid-1` 是配套写法，换成 `(l+r)>>1` 会死循环。
- `l+r+1` 最大 $2\times10^8+1$，`int` 不溢出。
- 全程只用整数除法，没有浮点误差；换行用 `'\n'`。