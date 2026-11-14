# P2637 [USACO08NOV] 第一次，第二次，成交！Going Once, Going Twice, Gone! B — 题解

## 一、解题过程：人类思考叙事

**题意重述。** FJ 有 $n$ 批干草（$n\le1000$），$m$ 个农夫（$m\le1000$），第 $i$ 个农夫愿意为**每批**干草付 $p_i$（$1\le p_i\le10^6$），且每个农夫**只想买一批**。FJ 定一个**统一售价** $x$：所有出价 $p_i\ge x$ 的农夫都会买（但干草只有 $n$ 批，买到的农夫数不超过 $n$）。求让 FJ **总收入最大**的售价；若有多个售价收入相同且最大，取**最低**的那个。输出这个最低售价和最大收入。

**第一反应：把收入写成售价的函数。** 设 $\mathrm{cnt}(x)=\#\{i:p_i\ge x\}$ 为出价不低于 $x$ 的农夫数。能买到的农夫数是 $\min(n,\mathrm{cnt}(x))$（干草不够就只卖 $n$ 批），每批卖 $x$，故
$$\mathrm{rev}(x)=x\cdot\min\bigl(n,\mathrm{cnt}(x)\bigr).$$
$\mathrm{cnt}(x)$ 是关于 $x$ 的**阶梯递减函数**：$x$ 越大，买得起的人越少。收入就是"单价 $\times$ 销量"，单价涨、销量跌，是个典型的权衡。

**第二步：最优售价一定落在某个 $p_i$ 上。** 这是本题的关键观察。$\mathrm{cnt}(x)$ 只在 $x$ 跨过某个 $p_i$ 时才跳变；在两个相邻出价值之间，$\mathrm{cnt}$ 是**常数**，于是 $\mathrm{rev}(x)=x\cdot\text{常数}$ 关于 $x$**严格递增**。所以在每一段"平台"上，收入在该段的**右端点**（也就是某个 $p_i$）取最大。把售价定在两个出价之间是"白白降价"——不如抬到下一个出价值。于是**候选售价只需枚举 $\{p_1,\dots,p_m\}$**，不必枚举 $1..10^6$ 的所有整数。

**第三步：怎么算每个候选的销量。** 把 $p$ 排序。对候选价 $x=p_i$，$\mathrm{cnt}(x)=\#\{p_j\ge x\}$。在升序数组里，这等于 $m$ 减去"第一个 $\ge x$ 的位置"（`lower_bound`）。有重复出价时，所有等于 $x$ 的都被算进去，正确。销量 $\min(n,\mathrm{cnt})$，收入 $x\cdot\text{销量}$。

**第四步：平局取最低价。** 我要的是"收入最大里售价最低"。把 $p$ **升序**枚举，用"严格大于才更新"记录最优：第一次达到最大收入时的售价就是最小的那个（后面更大的售价即使收入相同也不更新）。这天然实现了平局取低。

**第五步：核对与边界。** 题面样例 $n=5,m=4,p=\{2,8,10,7\}$：

| 售价 $x$ | $\mathrm{cnt}$ | 销量 $\min(5,\mathrm{cnt})$ | 收入 |
|---|---|---|---|
| $10$ | $1$ | $1$ | $10$ |
| $8$ | $2$ | $2$ | $16$ |
| $7$ | $3$ | $3$ | $21$ |
| $2$ | $4$ | $4$ | $8$ |

最大收入 $21$，对应最低售价 $7$，输出 `7 21`，与样例解释一致。

边界我想了几种：$n<m$（草不够，销量被 $n$ 截断，比如 $n=1$ 时只卖给出价最高者，售价 $=\max p_i$）；出价全相同（$\mathrm{cnt}$ 在该价处一次性跳到 $m$）；$n\ge m$（草够卖，销量 $=\mathrm{cnt}$）。这些都被统一公式覆盖，不用特判。收入上界 $10^6\times1000=10^9$，`int` 勉强够，但我用 `long long` 更稳。

---

## 二、思维链：完整证明

**记号。** 出价 $p_1,\dots,p_m\in[1,10^6]$，干草数 $n\in[1,1000]$。售价 $x$ 为正整数。$\mathrm{cnt}(x)=\#\{i:p_i\ge x\}$，$\mathrm{buy}(x)=\min(n,\mathrm{cnt}(x))$，$\mathrm{rev}(x)=x\cdot\mathrm{buy}(x)$。设 $p$ 的不同值升序为 $v_1<v_2<\cdots<v_k$（$k\le m$），并记 $v_0=0$。

---

### 引理 1（收入公式）

统一售价 $x$ 下，FJ 的总收入为 $\mathrm{rev}(x)=x\cdot\min(n,\mathrm{cnt}(x))$。

*证明*：题面"每个出价 $\ge x$ 的农夫将买到草料"，且每个农夫只买一批、每批付 $x$。出价 $\ge x$ 的农夫共 $\mathrm{cnt}(x)$ 个，但干草只有 $n$ 批，故实际成交批数为 $\min(n,\mathrm{cnt}(x))$（供不应求时只卖 $n$ 批，供过于求时卖 $\mathrm{cnt}(x)$ 批）。每批成交价都是 $x$，故总收入 $=x\cdot\min(n,\mathrm{cnt}(x))$。当 $\mathrm{cnt}(x)>n$ 时具体哪 $n$ 个农夫买到不影响收入（都付 $x$）。证毕。

---

### 引理 2（最优售价必为某个 $p_i$）

$\max_{x\ge1}\mathrm{rev}(x)$ 在某个 $x\in\{p_1,\dots,p_m\}$ 处取到。

*证明*：把正整数售价按 $p$ 的不同值分段。对 $j=0..k-1$，考虑区间 $I_j=(v_j,\,v_{j+1}]$（$v_0=0$，故 $I_0=[1,v_1]$）；另考虑 $I_k=(v_k,\infty)$。

**在 $I_j$（$j<k$）上 $\mathrm{cnt}$ 为常数**：对 $x\in(v_j,v_{j+1}]$，$p_i\ge x\iff p_i\ge v_{j+1}$。这是因为 $p_i$ 只能取 $v_1..v_k$：若 $p_i\le v_j$ 则 $p_i<x$（因 $x>v_j$）；若 $p_i\ge v_{j+1}$ 则 $p_i\ge x$（因 $x\le v_{j+1}$）。故 $\mathrm{cnt}(x)=\#\{p_i\ge v_{j+1}\}$ 在 $I_j$ 上恒定，记为 $c_j$，$\mathrm{buy}(x)=\min(n,c_j)$ 也恒定。于是 $\mathrm{rev}(x)=x\cdot\min(n,c_j)$ 在 $I_j$ 上关于 $x$ **严格递增**（系数 $\min(n,c_j)\ge0$；若 $c_j=0$ 则恒 $0$）。故 $I_j$ 上的最大值在右端点 $x=v_{j+1}$ 取到（$c_j>0$ 时严格最大；$c_j=0$ 时全 $0$，端点亦最大）。

**在 $I_k=(v_k,\infty)$ 上**：$x>v_k\ge p_i$ 对所有 $i$，$\mathrm{cnt}(x)=0$，$\mathrm{rev}(x)=0$。

综上，任何售价的收入都不超过某个 $v_{j+1}\in\{p_i\}$ 处的收入（$I_k$ 的 $0$ 也不超过 $v_1$ 处的非负收入）。故全局最大值在某个 $p_i$ 处取到。证毕。

---

### 引理 3（最低的最优售价也是某个 $p_i$；升序枚举可得）

设 $R=\max_x\mathrm{rev}(x)$。则达到 $R$ 的最低售价 $\min\{x:\mathrm{rev}(x)=R\}$ 属于 $\{p_i\}$；把 $p$ 升序枚举、"收入严格更大才更新最优"，记录的售价即该最低最优售价。

*证明*：由引理 2 的分段分析，在每个区间 $I_j$（$j<k$，$c_j>0$）上 $\mathrm{rev}$ 严格递增，故 $I_j$ 内部（非右端点）的 $x$ 满足 $\mathrm{rev}(x)<\mathrm{rev}(v_{j+1})\le R$，不可能达到 $R$；$c_j=0$ 的区间收入恒 $0$，若 $R>0$ 则达不到，若 $R=0$ 则最低售价是 $1$——但 $R=0$ 不可能（$x=v_1$ 时 $\mathrm{cnt}\ge1$，$\mathrm{buy}\ge1$，$\mathrm{rev}\ge v_1\ge1>0$）。$I_k$ 收入恒 $0<R$。故达到 $R$ 的售价只能是各段右端点 $v_{j+1}\in\{p_i\}$，最低者 $=\min\{v_j:\mathrm{rev}(v_j)=R\}\in\{p_i\}$。

升序枚举 $p$（即按 $v$ 升序，重复值不影响）时，设最优收入首次达到 $R$ 是在售价 $x^\*$。因升序，$x^\*$ 是所有达到 $R$ 的候选里最小的；"严格更大才更新"保证之后收入 $=R$ 的更大售价不会覆盖 $x^\*$。故记录的售价 $=x^\*=\min\{x:\mathrm{rev}(x)=R\}$。证毕。

---

### 引理 4（用 `lower_bound` 算 $\mathrm{cnt}$）

把 $p$ 升序排序后，对任意 $x$，$\mathrm{cnt}(x)=m-\bigl(\text{第一个 }\ge x\text{ 的下标}\bigr)$，即 `m-(lower_bound(p,p+m,x)-p)`。

*证明*：升序数组中，`lower_bound` 返回第一个 $\ge x$ 的元素下标 $t$（若全 $<x$ 则 $t=m$）。下标 $<t$ 的元素都 $<x$（共 $t$ 个），下标 $\ge t$ 的元素都 $\ge x$（共 $m-t$ 个，升序保证）。故 $\#\{p_i\ge x\}=m-t$。重复值：所有等于 $x$ 的元素都在 $t$ 及其后，被计入，正确。证毕。

---

### 引理 5（值域与溢出）

$\mathrm{rev}(x)\le10^6\times1000=10^9$，`int` 可容纳，但用 `i64` 更稳妥。

*证明*：售价候选 $x=p_i\le10^6$，销量 $\mathrm{buy}(x)\le n\le1000$，故 $\mathrm{rev}\le10^6\times10^3=10^9<2^{31}-1\approx2.147\times10^9$，`int` 不溢出。代码仍用 `i64` 存收入，留足余量、避免任何中间乘法隐患。证毕。

---

### 引理 6（复杂度）

时间 $O(m\log m)$，空间 $O(m)$。

*证明*：排序 $O(m\log m)$；枚举 $m$ 个候选，每个一次 `lower_bound` $O(\log m)$，共 $O(m\log m)$。存储 $p$ 数组 $O(m)$。$m\le1000$ 时远低于限制。证毕。

---

### 定理

程序输出最低的最优售价与最大收入。

*证明*：引理 1 给出收入公式；引理 2 把候选缩到 $\{p_i\}$；引理 4 正确算每个候选的销量与收入；引理 3 保证升序枚举 + 严格更新得到最低最优售价；引理 5 保证不溢出；引理 6 给出复杂度。证毕。

---

## 三、模式总结（按实际思考顺序）

- **explore its property** — 收入 $=$ 单价 $\times$ 销量 $=x\cdot\min(n,\#\{p_i\ge x\})$，销量是阶梯递减函数。
- **explore its property** — 相邻出价之间销量恒定 ⇒ 收入关于单价严格递增 ⇒ 最优单价必在某个 $p_i$（平台右端点）。
- **reduce to easier problem** — 候选售价从 $1..10^6$ 缩到 $m$ 个出价值，枚举即可。
- **classic problem solution** — 排序 + `lower_bound` 算 $\#\{p_i\ge x\}$，$O(m\log m)$。
- **discuss by category** — 平局取最低价：升序枚举 + "严格更大才更新"；$n<m$、重复出价、$n\ge m$ 都被统一公式覆盖。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|---|---|---|---|
| `n` | 干草批数（$\le1000$） | `int`，读入 | 销量上限 |
| `m` | 农夫数（$\le1000$） | `int`，读入 | 出价个数 |
| `p[i]` | 第 $i$ 个农夫出价（$\le10^6$） | `int[mxm]`，读入后升序排序 | 候选售价来源 |
| `mxm` | $1000+5$ | `constexpr int{1005}` | +5 松弛 |
| `price` | 当前候选售价 $=p[i]$ | `int` | 升序枚举 |
| `cnt` | $\#\{p_j\ge\text{price}\}$ | `int` | `m-lower_bound` |
| `buy` | 成交批数 $\min(n,\text{cnt})$ | `int` | 草不够截到 $n$ |
| `rev` | 当前收入 $\text{price}\cdot\text{buy}$ | `i64` | 防溢出 |
| `best` | 最大收入 | `i64`，初值 $0$ | 严格更大才更新 |
| `bp` | 最低最优售价 | `int`，初值 $0$ | 升序首次达最优即最小 |

### 算法步骤

1. I/O 加速三行。
2. 读 `n,m` 与 `p[0..m-1]`；`sort(p,p+m)` 升序。
3. `best=0,bp=0`；对 $i=0..m-1$：
   1. `price=p[i]`；`cnt=m-(lower_bound(p,p+m,price)-p)`；`buy=min(n,cnt)`；`rev=(i64)price*buy`；
   2. 若 `rev>best` 或 (`rev==best` 且 `price<bp`)：`best=rev,bp=price`。
4. 输出 `bp` 与 `best`（空格分隔）。

### 复杂度

- 时间：$O(m\log m)$。
- 空间：$O(m)$。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；题面样例输出 `7 21` 吻合；另手算 $n<m$（草不够）、出价全相同、$n\ge m$ 三组边界均与公式一致。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2637 Going Once, Going Twice, Gone! B
// 思路：收入 rev(x)=x*min(n,#{p_i>=x})。相邻出价间销量恒定 ⇒ rev 关于 x 递增
//       ⇒ 最优售价必在某个 p_i。排序后枚举 p_i，lower_bound 算销量，
//       升序 + 严格更大才更新 ⇒ 平局自动取最低售价。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxm{1005};

int n,m,p[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
		cin>>p[i];
	sort(p,p+m);
	i64 best=0;int bp=0;
	for(int i=0;i<m;++i)
	{
		int price=p[i];
		int cnt=m-(lower_bound(p,p+m,price)-p);	// #{p_j>=price}
		int buy=min(n,cnt);
		i64 rev=(i64)price*buy;
		if(rev>best||(rev==best&&price<bp))	// 升序枚举，平局取更低售价
			best=rev,bp=price;
	}
	cout<<bp<<' '<<best<<'\n';
	return 0;
}
```

### 代码说明

- **只枚举 $p_i$**：引理 2 保证最优售价必在出价值上，不必扫 $1..10^6$。
- **`lower_bound` 算 $\mathrm{cnt}$**：升序数组里第一个 $\ge$price 的下标 $t$，$\mathrm{cnt}=m-t$；重复出价全部计入。
- **`min(n,cnt)`**：干草只有 $n$ 批，供不应求时销量截到 $n$。
- **平局取低**：升序枚举 + `rev>best` 才更新，第一次达到最大收入的售价即最小；`rev==best&&price<bp` 是冗余保险（升序下不会触发），写明意图。
- **`i64` 收入**：上界 $10^9$ 虽在 `int` 内，用 `long long` 留余量。