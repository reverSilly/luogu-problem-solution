# P2345 [USACO04OPEN] MooFest G — 题解

## 一、解题过程：人类思考叙事

题意：有 $n$ 头奶牛，第 $i$ 头的听力为 $v_i$、位置为 $x_i$，所有 $x_i$ 互不相同。任意两头奶牛 $i,j$ 交流会产生

$$\max(v_i,v_j)\times |x_i-x_j|$$

的音量，求全部 $\binom{n}{2}$ 对奶牛的音量之和。数据范围是 $n\le 2\times 10^4$，$1\le v_i,x_i\le 2\times 10^4$。

拿到题的第一反应是照定义做：二重循环枚举所有奶牛对，取 $\max$、取绝对值、相乘、累加。对数约为 $\frac{n(n-1)}{2}\approx 2\times 10^8$，每对只有几次基本运算。这个做法在快机器上也许能挤过去，但题面背景里还挂着 P5094 加强版，说明出题人并不认为 $O(n^2)$ 是终点；而且 $2\times 10^8$ 次 64 位乘法没有多少余量。于是把它当作保底，先想能不能把其中一个 $n$ 换成 $\log$。

盯住公式看，阻碍我们把每头奶牛的贡献单独累加的东西有两个：

1. $\max(v_i,v_j)$：它把两头奶牛的听力绑在一起；
2. $|x_i-x_j|$：它把两头奶牛的位置绑在一起。

排序可以把"比较"变成"下标关系"，所以先试排序。按 $x$ 升序排：对 $i<j$ 有 $|x_i-x_j|=x_j-x_i$，绝对值消失了。但 $\max(v_i,v_j)$ 仍在，展开后是

$$\max(v_i x_j-v_i x_i,\ v_j x_j-v_j x_i),$$

两个候选式同时混着 $v_i,v_j,x_i,x_j$，还是没法对固定的 $j$ 单独求和。**按 $x$ 排序只解决了一半。**

再试按 $v$ 升序排。此时对 $i<j$ 必有 $v_i\le v_j$，于是 $\max(v_i,v_j)=v_j$，最大值直接"跳"出了公式：

$$S=\sum_{j=0}^{n-1} v_j D_j,\qquad D_j=\sum_{i<j}|x_j-x_i|.$$

每处理到第 $j$ 头奶牛，$D_j$ 就是"它与所有已经处理过的奶牛的距离之和"。处理完再把它插入集合，于是问题变成：**维护一个点集，支持插入一个坐标，查询它到当前点集的距离和**。

为什么按 $v$ 排序成功、按 $x$ 排序失败？因为绝对值是坐标的事，而坐标正是被插入集合的对象；只要知道"左边有多少个点、坐标和是多少"，绝对值就能拆成加法。$\max$ 是听力的事，听力和位置之间没有可用的代数关系，所以只能靠排序先把它消掉。让 $\max$ 先消失、把绝对值留给数据结构，是这里唯一可行的分工。

现在拆 $D_j$。设已插入点集为 $P$，查询位置为 $x$。按 $y<x$ 与 $y>x$ 分成左右两类（坐标互不相同，不会相等）：

- 左边的点：$|x-y|=x-y$，贡献 $x\cdot c_L-S_L$，其中 $c_L$ 是左边点数、$S_L$ 是左边坐标和；
- 右边的点：$|x-y|=y-x$，贡献 $(S_P-S_L)-x\cdot(c_P-c_L)$。

所以只需要四个量：$c_L,S_L,c_P,S_P$。而"插入一个坐标 / 查询坐标 $\le k$ 的个数与和"正是树状数组的拿手好戏。坐标值域只有 $2\times 10^4$，连离散化都省了；开两棵树，一棵存个数，一棵存坐标和。

手算一个小例子核对。三头牛 $(v,x)=(3,1),(2,4),(5,2)$：

- 按 $v$ 排序后是 $(2,4),(3,1),(5,2)$。
- 第 1 头：集合空，$D=0$，贡献 $0$；插入 $x=4$。
- 第 2 头 $x=1$：左边没有点，右边点集 $\{4\}$，$D=4-1=3$，贡献 $3\times 3=9$；插入 $1$。
- 第 3 头 $x=2$：左边 $\{1\}$ 贡献 $2-1=1$，右边 $\{4\}$ 贡献 $4-2=2$，$D=3$，贡献 $5\times 3=15$。

总计 $24$。按定义直接算：$\max(3,2)|1-4|=9$，$\max(3,5)|1-2|=5$，$\max(2,5)|4-2|=10$，和也是 $24$。吻合。

最后是数值范围。最坏情况下每对贡献不超过 $2\times 10^4\cdot(2\times 10^4-1)<4\times 10^8$，对数少于 $2\times 10^8$，总和小于 $8\times 10^{16}$，`int` 必然溢出，必须用 64 位整数。树状数组里的坐标和最大约 $4\times 10^8$，虽然塞得进 `int`，但为了不在乘法处发生隐式溢出，也一并用 `long long`。

---

## 二、思维链：完整证明

约定：把奶牛按 $v$ 升序重新编号为 $0,1,\dots,n-1$，即 $v_0\le v_1\le\cdots\le v_{n-1}$，位置仍记 $x_i$（互不相同）。原答案记为

$$S=\sum_{0\le i<j\le n-1}\max(v_i,v_j)\,|x_i-x_j|.$$

**引理 1（排序消去 max）**：对任意 $i<j$，有 $\max(v_i,v_j)=v_j$，因此

$$S=\sum_{j=0}^{n-1}v_jD_j,\qquad D_j=\sum_{i<j}|x_j-x_i|.$$

*证明*：由升序 $v_i\le v_j$，两个数中较大者就是 $v_j$，故 $\max(v_i,v_j)=v_j$。把原式按"较大下标 $j$"分组：

$$S=\sum_{j=0}^{n-1}\sum_{i<j}\max(v_i,v_j)|x_j-x_i|
=\sum_{j=0}^{n-1}\sum_{i<j}v_j|x_j-x_i|
=\sum_{j=0}^{n-1}v_jD_j.$$

原式的每一对 $\{i,j\}$ 在且仅在"较大下标为 $j$"的那一组中出现一次，分组只是改变求和顺序，不重不漏。$v$ 相等时 $\max$ 仍等于 $v_j$，所以同 $v$ 的任意先后都不影响正确性。证毕。

**引理 2（前缀集合不变量）**：设循环变量为 $j$（从 $0$ 开始）。第 $j$ 次迭代开始时，两棵树状数组中恰好存有坐标 $\{x_0,\dots,x_{j-1}\}$，变量 `tot` 等于这些坐标之和。

*证明*：对 $j$ 归纳。$j=0$ 时集合为空、`tot`$\,=0$，成立。设第 $j$ 次迭代开始时成立。该次迭代中只有查询（不修改树）和末尾的"把 $x_j$ 插入两棵树、`tot+=x_j`"两类操作，所以第 $j+1$ 次迭代开始时集合变为 $\{x_0,\dots,x_j\}$，`tot` 为对应之和。证毕。

**引理 3（绝对值按左右拆分）**：设已插入点集为 $P$，查询位置为 $x$，令 $L=\{y\in P:y<x\}$，$c_L=|L|$，$S_L=\sum_{y\in L}y$，$c_P=|P|$，$S_P=\sum_{y\in P}y$。则

$$\sum_{y\in P}|x-y|=x\,c_L-S_L+(S_P-S_L)-x(c_P-c_L).$$

*证明*：因为所有坐标互不相同且 $x$ 尚未插入，$P$ 被分成 $L=\{y<x\}$ 与 $R=\{y>x\}$ 两个不交的部分。对 $y\in L$ 有 $|x-y|=x-y$，对 $y\in R$ 有 $|x-y|=y-x$，于是

$$\sum_{y\in P}|x-y|=\sum_{y\in L}(x-y)+\sum_{y\in R}(y-x)
=x c_L-S_L+(S_P-S_L)-x(c_P-c_L).$$

这里 $|R|=c_P-c_L$，$\sum_{y\in R}y=S_P-S_L$。证毕。

**引理 4（Fenwick 树的前缀不变量）**：对下标 $i\in[1,V]$ 定义区间 $I_i=(i-\operatorname{lowbit}(i),\,i]$。维护数组 $t$，初值全 $0$。一次 `add(p,w)` 依次对

$$p,\ p+\operatorname{lowbit}(p),\ p+\operatorname{lowbit}(p+\operatorname{lowbit}(p)),\ \dots$$

中不超过 $V$ 的下标执行 $t_i\mathrel{+}=w$。则任意时刻都有

$$t_i=\sum_{p\in I_i}a_p,$$

其中 $a_p$ 是位置 $p$ 上已插入的权值和。并且 `sigma(k)` 沿 $k,k-\operatorname{lowbit}(k),\dots$ 累加 $t$，返回值恰为 $\sum_{p\le k}a_p$。

*证明*：分三步。

**(1) 更新命中的恰是覆盖 $p$ 的区间。** 记更新链为 $i_0=p$，$i_{r+1}=i_r+\operatorname{lowbit}(i_r)$。

先证链上每个 $i_r$ 都满足 $p\in I_{i_r}$，对 $r$ 归纳。$r=0$ 时 $p\in(p-\operatorname{lowbit}(p),p]$ 成立。设 $p\in I_{i_r}$，令 $2^m=\operatorname{lowbit}(i_r)$。$i_r$ 的第 $0..m-1$ 位全为 $0$、第 $m$ 位为 $1$，故 $i_{r+1}=i_r+2^m$ 的第 $0..m$ 位全为 $0$，从而 $\operatorname{lowbit}(i_{r+1})\ge 2^{m+1}$。于是

$$i_{r+1}-\operatorname{lowbit}(i_{r+1})
\le i_{r+1}-2^{m+1}
=i_r+2^m-2^{m+1}
=i_r-2^m
=i_r-\operatorname{lowbit}(i_r)<p,$$

又 $i_{r+1}>i_r\ge p$，所以 $p\in I_{i_{r+1}}$。

再证不在链上的 $i$ 都不满足 $p\in I_i$。若 $i<p$，则区间右端点 $i<p$，不覆盖。若 $i\ge p$ 且 $i$ 不在链上，则 $i\ne p$，故 $i_0=p<i$，链上至少有一个元素小于 $i$；取其中最后一个 $i_r$。则

$$i_r<i<i_{r+1}=i_r+2^m,\qquad 2^m=\operatorname{lowbit}(i_r).$$

写 $i=i_r+r'$，其中 $1\le r'<2^m$。由于 $i_r$ 的低 $m$ 位全为 $0$、第 $m$ 位为 $1$，$i$ 的低 $m$ 位恰好等于 $r'$，所以 $\operatorname{lowbit}(i)=\operatorname{lowbit}(r')\le r'$。因此

$$i-\operatorname{lowbit}(i)=i_r+r'-\operatorname{lowbit}(r')\ge i_r\ge p.$$

区间 $I_i=(i-\operatorname{lowbit}(i),i]$ 左端开，故 $p$ 即使等于左端点也不属于 $I_i$；若左端点更大则更不属于。两种情况都有 $p\notin I_i$。

于是每次 `add(p,w)` 给 $t_i$ 的增量恰为 $w\cdot[p\in I_i]$，对所有更新累加即得 $t_i=\sum_{p\in I_i}a_p$。

**(2) 查询区间的划分。** 查询链为 $k_0=k$，$k_{r+1}=k_r-\operatorname{lowbit}(k_r)$。节点 $k_r$ 覆盖 $I_{k_r}=(k_{r+1},k_r]$，下一个节点正是 $k_{r+1}$，所以这些区间右端点依次相接、两两不交；每步下标至少减少 $1$，最终到达 $0$，并集为 $(0,k]=[1,k]$。故

$$\texttt{sigma}(k)=\sum_r t_{k_r}=\sum_r\sum_{p\in I_{k_r}}a_p=\sum_{p=1}^{k}a_p.$$

（$k=0$ 时链为空，返回 $0$，也等于空前缀和。）证毕。

**引理 5（第 $j$ 轮算出的正是 $D_j$）**：第 $j$ 次迭代中，`cl`$\,=|\{i<j:x_i<x_j\}|$，`sl` 是这些坐标之和，$c_P=j$，$S_P=$`tot`；程序算出的 `dl+dr` 等于 $D_j$。

*证明*：由引理 2，树中恰为 $\{x_0,\dots,x_{j-1}\}$；由引理 4，`cnt.sigma(x_j-1)` 统计其中坐标 $\le x_j-1$（即 $<x_j$）的个数，`sm.sigma(x_j-1)` 统计这些坐标之和。已插入总数 $c_P$ 就是 $j$，总和就是 `tot`。把这些代入引理 3 的右端，得到 $\sum_{i<j}|x_j-x_i|=D_j$。证毕。

**引理 6（答案范围）**：$S<8\times10^{16}<2^{63}-1$，`long long` 足够；中间量 $x\,c_L\le4\times10^8$、$S_P\le4\times10^8$ 也都在 64 位范围内。

*证明*：每对贡献 $\max v\cdot|x_i-x_j|\le2\times10^4\cdot(2\times10^4-1)<4\times10^8$，对数 $\binom{2\times10^4}{2}<2\times10^8$，乘积 $<8\times10^{16}$。而 $2^{63}-1\approx9.22\times10^{18}$，余量两个数量级。证毕。

**定理（正确性）**：程序输出的值等于 $S$。

*证明*：由引理 2 与引理 5，第 $j$ 次迭代把 $v_jD_j$ 加进 `ans`；循环共 $n$ 次，由引理 1，$\sum_j v_jD_j=S$，故输出为 $S$。证毕。

**引理 7（复杂度）**：时间复杂度 $O(n\log n+n\log V)$，空间复杂度 $O(n+V)$，其中 $V=20004$ 为树状数组的有效下标上界（坐标上界 $2\times10^4$，数组再留一点松弛）。

*证明*：排序 $O(n\log n)$；每次迭代做两次前缀查询、两次单点更新，均为 $O(\log V)$，共 $O(n\log V)$；数组 `a` 占 $O(n)$，两棵树占 $O(V)$。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **explore its property** — 答案按奶牛对求和，公式里有 $\max$ 与绝对值两个不可加性障碍。
2. **consider easier version** — 先写 $O(n^2)$ 枚举作保底，并意识到它对加强版没有余量。
3. **discuss by category（比较两种排序）** — 按 $x$ 排序只消绝对值，按 $v$ 排序能消 $\max$；选后者。
4. **reduce to easier problem** — 排序后答案化为 $\sum_j v_jD_j$，$D_j$ 是"新点到已插入点集的距离和"。
5. **explore its property** — 绝对值按左右两侧拆成"个数 × 坐标 − 坐标和"。
6. **classic problem solution** — 两棵 Fenwick 树维护已插入坐标的个数与和，做动态前缀查询。
7. **verify boundary** — 答案 $8\times10^{16}$ 量级，必须 64 位；坐标互异让拆分无等号分支。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 初值 / 记忆点 |
|------|------|----------------|
| `n` | 奶牛数 | 输入 |
| `a[i].v, a[i].x` | 第 $i$ 头奶牛的听力、位置 | 输入后按 `v` 升序排序 |
| `cnt.tr[]` | 树状数组：已插入坐标的**个数** | 全 0 |
| `sm.tr[]` | 树状数组：已插入坐标的**和** | 全 0 |
| `tot` | 已插入坐标的总和 $S_P$ | 0 |
| `cl` | 已插入且坐标 $<x$ 的个数 $c_L$ | 每轮查询 |
| `sl` | 已插入且坐标 $<x$ 的坐标和 $S_L$ | 每轮查询 |
| `dl` | 左侧距离和 $x c_L-S_L$ | 每轮计算 |
| `dr` | 右侧距离和 $(S_P-S_L)-x(c_P-c_L)$ | 每轮计算 |
| `ans` | 音量总和 | 0，`long long` |
| `mxn, mxv` | 奶牛数上界、坐标值域上界（+5 松弛） | 常量 |

### 算法步骤

1. 读入 $n$ 与每头奶牛的 $(v,x)$，按 $v$ 升序排序。
2. 初始化两棵 Fenwick 树为空，`tot=0`，`ans=0`。
3. 依次处理排序后的第 $j$ 头奶牛（$j$ 从 0 开始）：
   1. `cl=cnt.sigma(x-1)`，`sl=sm.sigma(x-1)`；
   2. `dl=x*cl-sl`，`dr=(tot-sl)-x*(j-cl)`；
   3. `ans+=v*(dl+dr)`；
   4. `cnt.add(x,1)`，`sm.add(x,x)`，`tot+=x`。
4. 输出 `ans`。

### 复杂度

- 时间复杂度：$O(n\log n+n\log V)$，其中 $V=20004$ 是树状数组的有效下标上界。
- 空间复杂度：$O(n+V)$。

### 验证状态

g++ 16.2（`-std=c++20 -O2 -Wall -Wextra`）编译通过且无警告；与 $O(n^2)$ 暴力在 300 组随机小数据（$n\le60$，坐标互异）上完全一致；手算三组（单头牛、三头牛、同听力两头牛）分别得到 `0`、`24`、`45`；最大规模 $n=2\times10^4$、$v$ 全为 $2\times10^4$、$x=1..2\times10^4$ 时输出 `26666666600000000`，与闭式 $2\times10^4\cdot\frac{n(n^2-1)}{6}$ 相符。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2345 [USACO04OPEN] MooFest G
// 思路：按听力 v 升序排序，处理第 i 头牛时，已处理的牛听力都不超过 v_i，
//       故 max(v_i,v_j)=v_i；用两棵 BIT 维护已插入坐标的个数与坐标和，
//       把 |x_i-x_j| 按左右两侧拆开，O(log V) 求距离和。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20005},mxv{20005};

int n;
struct cow{int v,x;}a[mxn];

struct BIT
{
	i64 tr[mxv]{};
	void add(int p,i64 w)
	{
		for(int i=p;i<mxv;i+=i&-i)
			tr[i]+=w;
	}
	i64 sigma(int p)
	{
		i64 s=0;
		for(int i=p;i>0;i-=i&-i)
			s+=tr[i];
		return s;
	}
};

BIT cnt,sm;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].v>>a[i].x;
	sort(a,a+n,[](auto p,auto q){return p.v<q.v;});

	i64 tot=0,ans=0;
	for(int i=0;i<n;++i)
	{
		int v=a[i].v,x=a[i].x;
		int cl=(int)cnt.sigma(x-1);		//已插入且坐标严格小于 x 的牛数
		i64 sl=sm.sigma(x-1);			//这些牛的坐标和
		i64 dl=(i64)x*cl-sl;			//左侧距离和
		i64 dr=(tot-sl)-(i64)x*(i-cl);	//右侧距离和
		ans+=(i64)v*(dl+dr);
		cnt.add(x,1);
		sm.add(x,x);
		tot+=x;
	}
	cout<<ans<<'\n';
	return 0;
}
```
