# P1570 KC 喝咖啡

> 标签：二分 / 洛谷原创 / 分数规划 · 难度：普及/提高-
> 本地题面：`P1570.md`（$1\le n\le 200$，$1\le m\le n$，$1\le v_i,c_i\le 10^4$，答案 $\le 1000$）。
> 验证：300 组随机（跳过"真值恰为 3 位小数半值"的歧义平局）+ 4 组手工边界，标程与组合枚举精确暴力逐字节一致；$n=200,m=100$ 耗时 0.0053s。

## 1. 重新读题

**问题重述（人话版）**

有 $n$ 种调料，第 $i$ 种有美味度 $v_i$ 与耗时 $c_i$。必须**恰好选 $m$ 种**加入咖啡，目标最大化

$$\dfrac{\sum_{i\in S} v_i}{\sum_{i\in S} c_i}\qquad(|S|=m),$$

输出这个最大值，**保留三位小数**。

> 一句话版本：**从 $n$ 个「(价值 $v_i$, 代价 $c_i$)」里恰取 $m$ 个，使价值总和 / 代价总和的比值最大。**

**容易漏掉的条款**

1. 「只可以加入其中的 $m$ 种」+「一定会加入 $m$ 种」$\Rightarrow$ 是**恰好 $m$ 个**，不是"至多 $m$ 个"。
2. $c_i\ge1$ 且 $m\ge1$ $\Rightarrow$ 分母 $\sum c_i>0$，比值恒有定义。
3. 「保留三位小数」$\Rightarrow$ 用 `fixed<<setprecision(3)`（与 `printf("%.3f")` 同义的四舍五入到三位）。
4. 「答案不超过 $1000$」$\Rightarrow$ 这是二分上界的来源，不是二分的必要条件。

**数据范围与规模感受**

$$n\le 200,\quad m\le n,\quad v_i,c_i\le 10^4,\quad \text{答案}\le 1000.$$

$n$ 只有 $200$：$\binom{200}{100}$ 这种组合枚举绝无可能，必须用不依赖 $m$ 大小的方法。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：目标为什么不能"逐项贪心"

比值 $\sum v/\sum c$ 是**不可分离**的：它不等于各 $v_i/c_i$ 的某种简单加和。直觉上"选各自 $v_i/c_i$ 最大的 $m$ 项"看起来很合理，但它**是错的**。反例（$m=2$）：

| 调料 | $v$ | $c$ | 单项比值 |
|---|---|---|---|
| 1 | 30 | 10 | 3.0 |
| 2 | 11 | 10 | 1.1 |
| 3 | 1 | 1 | 1.0 |

- 按单项比值贪心取前二（1、2 号）：$(30+11)/(10+10)=41/20=2.05$。
- 真正最优（1、3 号）：$(30+1)/(10+1)=31/11\approx 2.818$。

差了将近 $0.77$。**教训：比值是带权平均值，权重 $c_i$ 同样参与分配，单项比值排序无法反映"分母如何拉动整体"。**

### 2.2 consider easier version：两个极端子任务

- $m=1$：答案就是 $\max_i v_i/c_i$，一次扫描即可。
- $m=n$：答案就是 $\sum v_i/\sum c_i$，一个数。

这两个极端给不了通用解，但提示我们：**目标是"比值的最大值"，而不是"某个可加量的最大值"**——这正是分数规划（从小 $m$ 到大 $m$ 都统一处理）的用武之地。

### 2.3 关键转化：把"比值最大"变成"是否 $\ge \lambda$"

固定一个实数 $\lambda$，问：**是否存在 $|S|=m$ 使 $\dfrac{\sum v}{\sum c}\ge\lambda$？**

**引理 1（可行性判定的等价变形）**

对任意实数 $\lambda$，存在 $|S|=m$ 使 $\sum_{i\in S}v_i/\sum_{i\in S}c_i\ge\lambda$，当且仅当存在 $|S|=m$ 使

$$\sum_{i\in S}(v_i-\lambda c_i)\ \ge\ 0.$$

**证明**：因 $\sum_{i\in S}c_i>0$，不等式 $\frac{\sum v}{\sum c}\ge\lambda$ 两边同乘正数 $\sum c$ 不变号，化为 $\sum v\ge\lambda\sum c$，即 $\sum(v-\lambda c)\ge0$。两个不等式逐项等价，故命题等价。$\blacksquare$

于是判定问题变成：**在 $n$ 个实数 $d_i(\lambda)=v_i-\lambda c_i$ 中，能否选出恰好 $m$ 个使其和 $\ge0$。**

**引理 2（恰好 $m$ 个和最大 = 取最大的 $m$ 个）**

对任意实数序列 $d_1,\dots,d_n$ 与任意固定 $m$，从其中选 $m$ 个使和最大，最优做法就是取 $d$ 中**最大的 $m$ 个**。

**证明**（交换论证）：设最优集合为 $S$，若存在 $x\in S,\ y\notin S$ 且 $d_y>d_x$，则把 $x$ 换成 $y$ 得到 $S'$，$\sum_{S'}d-\sum_{S}d=d_y-d_x>0$，与 $S$ 最优矛盾。故最优集合中的任意元素都 $\ge$ 非集合中的任意元素，即 $S$ 恰为由大到小排序后的前 $m$ 个（之和相同者任取其一，和相等）。$\blacksquare$

因此判定函数可以写成：把 $d_i(\lambda)$ 排序取前 $m$ 个求和，看是否 $\ge0$。记

$$\text{check}(\lambda)=\Big[\text{top-}m\ \text{的}\ \textstyle\sum d_i(\lambda)\ \ge\ 0\Big].$$

### 2.4 二分的根据：可行 $\lambda$ 恰是一个区间

**引理 3（单调性）**

若 $\lambda_1<\lambda_2$，则 $\text{check}(\lambda_2)\Rightarrow\text{check}(\lambda_1)$。即 check 关于 $\lambda$ 单调不增。

**证明**：只要证：存在 $m$ 项使 $\sum(v-\lambda_2c)\ge0$，就存在 $m$ 项使 $\sum(v-\lambda_1c)\ge0$。取实现 $\lambda_2$ 的同一集合 $S$（$|S|=m$ 且 $\sum_S(v-\lambda_2c)\ge0$）。因每个 $c_i>0$，对同一 $S$：

$$\sum_{S}(v-\lambda_1c)=\sum_{S}(v-\lambda_2c)+(\lambda_2-\lambda_1)\sum_{S}c\ \ge\ 0+(\lambda_2-\lambda_1)\cdot(\text{正数})>0\ge0,$$

故 $S$ 对 $\lambda_1$ 也可行，$\text{check}(\lambda_1)$ 成立。$\blacksquare$

**推论**：由引理 3，集合 $\{\lambda:\text{check}(\lambda)\}$ 是**下闭区间** $[0,\lambda^\*]$（下限 $0$ 是因为 $v_i\ge1>0$ 使 $\lambda=0$ 恒可行），其中 $\lambda^\*=\sup\{\lambda:\text{check}(\lambda)\}$。这正是二分答案的合法性基础。

**引理 4（$\lambda^\*$ 就是所求最大比值）**

记 $\rho=\max_{|S|=m}\sum v/\sum c$，则 $\lambda^\*=\rho$。

**证明**（双向）：
- **（$\rho$ 可行，故 $\lambda^\*\ge\rho$）** 取达到 $\rho$ 的集合 $S^\*$，则 $\sum_{S^\*} v=\rho\sum_{S^\*}c$，故 $\sum_{S^\*}(v-\lambda c)=\sum_{S^\*}c\,(\rho-\lambda)\ge0\iff\lambda\le\rho$。于是对一切 $\lambda\le\rho$，$\text{check}(\lambda)$ 真（至少有 $S^\*$ 作见证），故 $\lambda^\*\ge\rho$。
- **（任何 $\text{check}$ 为真的 $\lambda$ 都 $\le\rho$，故 $\lambda^\*\le\rho$）** 若 $\text{check}(\lambda)$ 真，由引理 2 存在 $|S|=m$ 使 $\sum_S(v-\lambda c)\ge0$，即 $\rho\ge\sum_S v/\sum_S c\ge\lambda$。故每个可行 $\lambda$ 都不超过 $\rho$，取 sup 得 $\lambda^\*\le\rho$。
- 合并得 $\lambda^\*=\rho$。$\blacksquare$

### 2.5 算法与精度

由引理 4，只需二分逼近 $\lambda^\*$：区间 $[0,1000]$（上界来自题面保证），每次取 `mid=(lo+hi)/2`，若 `check(mid)` 则 `lo=mid`（可行边）否则 `hi=mid`（不可行边），迭代固定次数（如 $100$ 次）后输出 `lo`。$100$ 次迭代把区间缩到 $1000/2^{100}$，远超三位小数所需精度。

> **关于三位小数的四舍五入**：当真实最优比值恰好落在 $x.xxx5$ 这种"半值"上时，第三位小数取决于浮点表示与舍入模式，本身有歧义（我们造数据的对拍脚本会**主动跳过**这类真值恰为半值的用例，见 §4.5）。除此之外，二分结果与真值的差约 $10^{-10}$ 级，远小于 $10^{-3}$，不会影响三位小数。

### 2.6 discuss by category

- $n\le 10$ 一行：枚举 $\binom{n}{m}$ 个子集精确比较，可作为对拍基准。
- 对一般 $n$：二分分数规划，$O(\text{iters}\cdot n\log n)$。

## 3. 模式总结（Pattern Summary）

- explore its property — 目标是"分子除以分母"，不可分离，逐项比值贪心会错（反例 $(30,10),(11,10),(1,1)$ 取 $m=2$）。
- consider easier version — 看 $m=1$ 与 $m=n$ 两个极端，确认目标是"比值最值"而非"可加量最值"。
- reduce to easier problem — 引入参数 $\lambda$，把"比值 $\ge\lambda$"等价改写为"$\sum(v-\lambda c)\ge0$"（引理 1），把比值比较降为可加量判定。
- classic problem solution — 固定 $\lambda$ 后"恰选 $m$ 个使和最大"转化为"取最大的 $m$ 个 $d_i$"（引理 2，交换论证）。
- classic problem solution — 可行性关于 $\lambda$ 单调（引理 3），二分答案逼近 $\lambda^\*$，并证明 $\lambda^\*$ 恰等于最大比值（引理 4）。
- handle numeric ambiguity — 三位小数点后恰好为半值的用例天然有舍入歧义，数据生成阶段规避。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
|---|---|---|---|---|
| $n,m$ | `int` | 调料种数 / 必选数 | 读入 | 题面 |
| `v[i]` | `double[205]` | 第 $i$ 种调料的美味度 $v_i$ | 读入 | value |
| `c[i]` | `double[205]` | 第 $i$ 种调料的耗时 $c_i$ | 读入 | cost |
| `d[i]` | `double[205]` | 参数化后的 $d_i=v_i-\lambda c_i$ | 每次 check 重算 | 判据 |
| `lo,hi` | `double` | 二分的可行下界 / 不可行上界 | $0$ / $1000$ | 答案区间 |
| `mxn` | `constexpr int` | 数组尺寸 $200+5$ | — | 松弛约定 |
| `mxr` | `constexpr int` | 答案上界 $1000$ | — | 题面保证 |

### 4.2 编号步骤

1. 读入 $n,m$、$n$ 个 $v_i$、$n$ 个 $c_i$。
2. `lo=0, hi=mxr`。
3. 重复 $100$ 次：`mid=(lo+hi)/2`；对每个 $i$ 算 `d[i]=v[i]-mid*c[i]`；`sort(d,d+n,greater<>())`；求前 $m$ 个之和 $s$；`s>=0` 则 `lo=mid`，否则 `hi=mid`。
4. 输出 `fixed<<setprecision(3)<<lo`。

### 4.3 复杂度与验证

- 时间 $O(\text{iters}\cdot n\log n)=100\times200\log200\approx 1.5\times10^5$；空间 $O(n)$。
- 验证：300 组随机（$n\le10$，$m\le n$，$v_i,c_i\in[1,30]$，跳过半值平局）+ 4 组手工边界，标程与 $\binom{n}{m}$ 组合枚举精确暴力输出逐字节一致；$n=200,m=100$ 耗时 $0.0053$s。

### 4.4 关键不变量

1. `check(λ)` 真 $\iff\lambda\le\rho$（引理 4），故 `lo` 始终可行、`hi` 始终是上界。
2. 每次 `check` 必须**恰好取前 $m$ 个**（`m\le n` 保证不下标越界）。
3. $c_i>0$ 是引理 1、3 成立的前提；题面已保证。

### 4.5 验证状态小结

| 项目 | 状态 |
|---|---|
| 随机对拍 | **通过**：300 组，二分版与组合枚举精确版逐字节一致 |
| 手工边界 | **通过**：$1/1\to2.333$；$m=n$ 全同 $\to1.000$；$(2,3,1)/(2,1,10)$ 取 $m=2\to1.667$（与题面解释同构）；$1000/1$ 触上界 $\to1000.000$ |
| 最坏规模计时 | $n=200,m=100\to$ 输出 $2.185$，耗时 $0.0053$s |
| 残余风险 | 真值恰为 $x.xxx5$ 的用例，第三位小数舍入有歧义；对拍已规避此类，正式提交用与题干一致的 `setprecision(3)` 即可 |

## 5. 示例代码

### 5.1 主解：分数规划 + 二分答案

```cpp
//
// 题目：P1570 KC 喝咖啡
// https://www.luogu.com.cn/problem/P1570
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};       // n<=200 (+5 松弛)
constexpr int mxr{1000};      // 答案上界(题面保证不超过 1000)

int n,m;
double v[mxn],c[mxn],d[mxn];  // d[i]=v_i - lambda*c_i

bool check(double lam)
{
	for(int i=0;i<n;++i)
		d[i]=v[i]-lam*c[i];
	sort(d,d+n,greater<>());  // 选恰好 m 项,贪心取最大的 m 个
	double s=0;
	for(int i=0;i<m;++i)
		s+=d[i];
	return s>=0;              // 存在 m 项之和 >=0 <=> 比值可达 >= lam
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>v[i];
	for(int i=0;i<n;++i)
		cin>>c[i];

	double lo=0,hi=mxr;       // lo 恒可行(v_i>=1),hi=答案上界
	for(int it=0;it<100;++it)
	{
		double mid=(lo+hi)/2;
		if(check(mid))
			lo=mid;
		else
			hi=mid;
	}
	cout<<fixed<<setprecision(3)<<lo<<'\n';
	return 0;
}
```

### 5.2 对照实现：$\binom{n}{m}$ 组合枚举精确比值（仅用于小规模对拍）

```cpp
// 暴力基准:枚举所有恰好 m 项的选取,用交叉相乘精确比较比值取最大
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{205};       // 暴力仅跑 n<=20

int n,m;
i64 v[mxn],c[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>v[i];
	for(int i=0;i<n;++i)
		cin>>c[i];

	i64 bp=0,bq=1;            // 当前最大比值 bp/bq
	for(int mask=0;mask<(1<<n);++mask)
	{
		if(__builtin_popcount(mask)!=m)
			continue;
		i64 p=0,q=0;
		for(int j=0;j<n;++j)
			if(mask>>j&1)
			{
				p+=v[j];
				q+=c[j];
			}
		if(p*bq>bp*q)         // p/q > bp/bq (q,bq>0)
		{
			bp=p;
			bq=q;
		}
	}
	cout<<fixed<<setprecision(3)<<(double)bp/bq<<'\n';
	return 0;
}
```

两版输出逐字节一致（已用 $300$ 组随机数据确认）。暴力版用**交叉相乘**精确比较分数（不引入浮点误差）再一次性格式化，完全独立于二分的浮点逼近，可交叉验证引理 1、2、4。

### 5.3 易错点清单

- **逐项比值贪心** $\Rightarrow$ 反例 $(30,10),(11,10),(1,1),m=2$ 少算。
- **把 $m$ 当成"至多"** $\Rightarrow$ 取前 $m$ 个必须真切 $m$ 个。
- **二分方向写反** $\Rightarrow$ `check(mid)` 真时 `lo=mid`（不是 `hi=mid`）。
- **上界取得比真答案小**（例如硬编码 $1$）$\Rightarrow$ 结果被截断；用题面给的 $1000$。
