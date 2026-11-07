# P1927 防护伞

## 1. 重新读题

### 1.1 人话重述

平面上有 $N$ 个黑子（点），第 $i$ 个坐标为 $(x_i,y_i)$。要造一个圆面（防护伞）盖住所有黑子，且**圆心必须选在某个黑子所在的位置**。求满足条件的最小圆面积，精确到小数点后 $4$ 位，$\pi$ 取 $3.1415926535$。

$2\le N\le1000$，$-10000\le x_i,y_i\le10000$。

### 1.2 关键约束与规模暗示

- 圆心只能在 $N$ 个给定点中选，因此候选只有 $N$ 个，可以枚举。
- 对每个候选圆心，只需知道“最远的黑子有多远”，这需要对所有点扫一遍，$O(N)$。
- 总复杂度 $O(N^2)=10^6$，非常轻松。
- 坐标范围小，距离平方可用 64 位整数精确存储，避免开根号带来的精度损失。

---

## 2. 思维链（Chain-of-thought）

### 2.1 固定圆心时，半径是多少

**引理 1**：若圆心固定为点 $O$，则能盖住所有黑子的最小半径等于
$$
R=\max_{1\le j\le N}|O-P_j| .
$$

**证明**：

- 必要性：圆必须包含每个 $P_j$，所以半径至少为每个 $|O-P_j|$，即至少为最大值。
- 充分性：以 $O$ 为圆心、$R=\max_j|O-P_j|$ 为半径的圆，由 $R$ 的定义包含所有 $P_j$。

故最小半径恰为 $R$。$\blacksquare$

于是面积
$$
S=\pi R^2=\pi\cdot\max_j|O-P_j|^2 .
$$
注意我们只需要**距离平方的最大值**，最后再乘 $\pi$，全程不必开方。

### 2.2 圆心怎么选

圆心只能取某个黑子 $P_i$。定义
$$
D_i=\max_{1\le j\le N}\bigl[(x_i-x_j)^2+(y_i-y_j)^2\bigr].
$$
答案为
$$
\boxed{\pi\cdot\min_{1\le i\le N}D_i } .
$$

**证明**：由引理 1，圆心取 $P_i$ 时最小面积为 $\pi D_i$。在所有合法圆心（即所有 $P_i$）中取最小，得到全局最小面积。$\blacksquare$

### 2.3 精度处理

- 坐标差最大为 $20000$，平方最大 $4\times10^8$，两项相加最大 $8\times10^8$，用 `long long` 存储距离平方完全精确。
- 只在最后乘一次 $\pi$，用 `long double` 或 `double` 均可；输出固定 4 位小数。
- 若所有点重合，$D_i=0$，输出 `0.0000`。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 固定圆心时最小半径就是到最远点的距离。
- **avoid sqrt** — 比较与乘法都只用距离平方，最后才乘 $\pi$。
- **enumeration** — 圆心只能取给定点，枚举 $N$ 个候选。
- **precision** — 64 位整数存平方距离，浮点只做最后一次乘法。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| $N$ | `int` | 黑子个数 | 读入 | 数量 |
| `x[i],y[i]` | `long long[1005]` | 第 $i$ 个黑子坐标 | 读入 | coordinates |
| `best` | `long long` | 最小化的“最大距离平方” | $-1$ | best |
| `far` | `long long` | 当前圆心到所有点的最大距离平方 | $0$ | farthest |
| `PI` | `long double` | $\pi$ 的近似值 | $3.1415926535$ | pi |

### 4.2 算法步骤

1. 读入 $N$ 与所有点坐标。
2. 对每个 $i=0,\dots,N-1$：
   - 令 `far=0`；
   - 对每个 $j=0,\dots,N-1$，计算
     $$
     d^2=(x_i-x_j)^2+(y_i-y_j)^2,
     $$
     更新 `far=max(far,d2)`；
   - 用 `best=min(best,far)` 更新（`best` 初值为 $-1$ 表示未赋值）。
3. 输出 `PI * best`，保留 4 位小数。

### 4.3 复杂度

- 时间：$O(N^2)$，$N\le1000$ 时约 $10^6$ 次运算。
- 空间：$O(N)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1927 防护伞
// https://www.luogu.com.cn/problem/P1927
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr long double PI{3.1415926535L};

int n;
long long x[mxn],y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];

	long long best{-1};
	for(int i=0;i<n;++i)
	{
		long long far{0};
		for(int j=0;j<n;++j)
		{
			long long dx=x[i]-x[j],dy=y[i]-y[j];
			long long d2=dx*dx+dy*dy;
			if(d2>far)far=d2;
		}
		if(best<0||far<best)best=far;
	}

	long double ans=PI*(long double)best;
	cout<<fixed<<setprecision(4)<<(double)ans<<'\n';
	return 0;
}
```
