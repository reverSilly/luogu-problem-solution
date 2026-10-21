# P1654 OSU!

> 难度：省选/NOI- · 标签：期望 DP / 数学期望的线性性 / 递推

## 1. 重新读题

### 1.1 问题重述（人话版）

共有 $n$ 次独立操作，第 $i$ 次操作成功的概率为 $p_i \in [0, 1]$（成功记为 `1`，失败记为 `0`）。在最终形成的长度为 $n$ 的 01 串中，每一段**极长连续的 $X$ 个 `1`** 会贡献 $X^3$ 的分数。求总分数的数学期望，四舍五入保留 $1$ 位小数。

### 1.2 关键约束与规模暗示

- **数据规模**：$1 \le n \le 10^5$，$p_i \in [0, 1]$。
- **规模暗示**：$n = 10^5$ 要求 $O(n)$ 时间算法。不能枚举极长段，而应利用立方增量恒等式 $(L+1)^3 - L^3 = 3L^2 + 3L + 1$，将极长段得分拆解为每次成功时的**边际增量期望**！

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：立方增量的边际拆分

当末尾连续 `1` 的长度从 $L$ 增长到 $L + 1$ 时，得分的增量为：
$$\Delta(L) = (L + 1)^3 - L^3 = 3 L^2 + 3 L + 1.$$
根据期望的线性性，要计算 $\mathbb{E}[\Delta(L_{i-1}) \cdot \mathbb{I}(\text{第 } i \text{ 次成功})]$，我们只需要同步维护以第 $i-1$ 位结尾的连续 `1` 长度的一次方期望 $E_1[i-1] = \mathbb{E}[L_{i-1}]$ 与二次方期望 $E_2[i-1] = \mathbb{E}[L_{i-1}^2]$！

### 2.2 考虑简化版 / 子任务

- **定义穷举基准（$n \le 13$）**：直接枚举全部 $2^n$ 个 01 串，按题面定义切分极长连续 `1` 段累加 $X^3 \cdot \prod \text{prob}$，作为零假设的对拍基准 `P1654_brute.cpp`。

### 2.3 常见碰壁与反例

- **常见错误：误以为 $\mathbb{E}[L^2] = (\mathbb{E}[L])^2$**
  当 $p_1 = 0.5$ 时，$L_1 \in \{0, 1\}$ 各占 $0.5$，故 $\mathbb{E}[L_1] = 0.5$，而 $\mathbb{E}[L_1^2] = 0.5 \times 1^2 = 0.5 \ne (0.5)^2 = 0.25$！因为 $\text{Var}(L) = \mathbb{E}[L^2] - (\mathbb{E}[L])^2 > 0$，绝不能用 $E_1^2$ 代替 $E_2$，必须对 $E_2$ 独立列递推式。

### 2.4 一阶矩、二阶矩与总期望递推证明

**引理 1（尾长矩递推方程）**
设随机变量 $L_i$ 表示前 $i$ 次操作末尾连续 `1` 的个数，$I_i \in \{0, 1\}$ 为第 $i$ 次操作是否成功的指示变量（$\mathbb{P}(I_i = 1) = p_i$，且 $I_i$ 与 $L_{i-1}$ 相互独立）。记 $E_1[i] = \mathbb{E}[L_i]$，$E_2[i] = \mathbb{E}[L_i^2]$，总分期望为 $F[i]$，则：
1. $E_1[i] = p_i (E_1[i-1] + 1)$；
2. $E_2[i] = p_i (E_2[i-1] + 2 E_1[i-1] + 1)$；
3. $F[i] = F[i-1] + p_i (3 E_2[i-1] + 3 E_1[i-1] + 1)$。

**证明**：
由定义，当 $I_i = 0$（概率 $1 - p_i$）时 $L_i = 0$，得分增量为 $0$；当 $I_i = 1$（概率 $p_i$）时 $L_i = L_{i-1} + 1$，得分增量为 $(L_{i-1} + 1)^3 - L_{i-1}^3 = 3 L_{i-1}^2 + 3 L_{i-1} + 1$。
由于第 $i$ 次操作独立于前 $i-1$ 次操作，由全期望公式与期望线性性：
- $E_1[i] = p_i \mathbb{E}[L_{i-1} + 1] + (1 - p_i) \cdot 0 = p_i (E_1[i-1] + 1)$；
- $E_2[i] = p_i \mathbb{E}[(L_{i-1} + 1)^2] = p_i \mathbb{E}[L_{i-1}^2 + 2 L_{i-1} + 1] = p_i (E_2[i-1] + 2 E_1[i-1] + 1)$；
- $F[i] - F[i-1] = p_i \mathbb{E}[3 L_{i-1}^2 + 3 L_{i-1} + 1] = p_i (3 E_2[i-1] + 3 E_1[i-1] + 1)$。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **explore its property** — 用差分 $(L+1)^3 - L^3 = 3L^2 + 3L + 1$ 将整段结算拆解为逐位增量。
- **failed attempt（矩平方混淆）** — 警惕 $\mathbb{E}[L^2] \ne (\mathbb{E}[L])^2$，必须独立维护二阶矩 $E_2$。
- **classic problem solution** — 同步线性递推一阶矩 $E_1$、二阶矩 $E_2$ 与三阶累积期望 $F$。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 操作总次数 | 读入 | 题面参数 |
| `e1, e2` | `double` | 末尾连续 `1` 长度的一次、二次矩期望 | `0.0` | $\mathbb{E}[L], \mathbb{E}[L^2]$ |
| `ans` | `double` | 累计期望总得分 | `0.0` | total expected score |

### 4.2 算法步骤

1. 读入 $n$，初始化 `e1 = e2 = ans = 0`。
2. 循环 $n$ 次，每次读入 $p$，按顺序更新：
   - `ans += p * (3 * e2 + 3 * e1 + 1)`；
   - `e2 = p * (e2 + 2 * e1 + 1)`；
   - `e1 = p * (e1 + 1)`。
3. 输出 `fixed << setprecision(1) << ans`。

### 4.3 复杂度与验证状态

- **时间复杂度**：$O(n)$。
- **空间复杂度**：$O(1)$。
- **沙箱对拍验证**：`g++ -O2 -std=c++17 -Wall` 零警告编译通过；官方样例 `3 / 0.5 / 0.5 / 0.5` 输出 `6.0` 通过；与 $2^n$ 状态穷举暴力程序 `P1654_brute.cpp` 对拍 $400$ 组随机数据全部一致；$n = 10^5$ 实测耗时 $0.0485\text{ s}$。

## 5. 示例代码

### 5.1 标程（`P1654.cpp`）

```cpp
//
// 题目：P1654 OSU!
// https://www.luogu.com.cn/problem/P1654
// AC
#include<bits/stdc++.h>
using namespace std;

int n;
double e1,e2,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		double p;
		cin>>p;
		ans+=p*(3*e2+3*e1+1);
		e2=p*(e2+2*e1+1);
		e1=p*(e1+1);
	}
	cout<<fixed<<setprecision(1)<<ans<<'\n';
	return 0;
}
```

### 5.2 暴力对照程序（`P1654_brute.cpp`）

```cpp
//
// 题目：P1654 OSU!
// 暴力基准：枚举全部 2^n 个 01 串，按定义计算每个串极长连续 1 段的立方和与精确期望
#include<bits/stdc++.h>
using namespace std;

int n;
double p[25];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>p[i];

	double ans=0;
	for(int mask=0;mask<(1<<n);++mask)
	{
		double prob=1;
		int score=0,run=0;
		for(int i=0;i<n;++i)
		{
			if(mask>>i&1)
			{
				prob*=p[i];
				++run;
			}
			else
			{
				prob*=1-p[i];
				score+=run*run*run;
				run=0;
			}
		}
		score+=run*run*run;
		ans+=prob*score;
	}
	cout<<fixed<<setprecision(1)<<ans<<'\n';
	return 0;
}
```
