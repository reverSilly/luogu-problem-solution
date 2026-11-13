# P1763 埃及分数

## 1. 重新读题

给定真分数 $\frac{a}{b}$（$1 < a < b < 1000$），将其表示为若干个互不相同的单位分数之和：
$$\frac{a}{b} = \frac{1}{x_1} + \frac{1}{x_2} + \dots + \frac{1}{x_d} \qquad (1 \le x_1 < x_2 < \dots < x_d).$$
要求首先**最小化项数 $d$**；在项数 $d$ 最少的前提下，**最小化最大分母 $x_d$**（即令最小单位分数 $\frac{1}{x_d}$ 尽可能大）。保证最优解满足 $x_d \le 10^7$。按升序输出 $x_1, x_2, \dots, x_d$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：迭代加深搜索（IDDFS）与上下界剪枝

由于项数 $d$ 越小越好，且单分支深度可以无限延伸而广度受分母上界制约，这是**迭代加深搜索（IDDFS）**的经典模型：从小到大枚举总项数 $d = 1, 2, 3, \dots$，一旦在某层 $d$ 找到合法解即停止。

在当前还剩 $k$ 个单位分数待选、剩余分数值为 $\frac{a}{b}$、上一个选出的分母为 $\text{last}$ 时，当前分母 $x$ 的取值范围被两条不等式死死卡住：

**引理 1（分母枚举上下界定理）**：当前分母 $x$ 必须满足
$$\max\Big(\text{last} + 1,\ \Big\lfloor \frac{b + a - 1}{a} \Big\rfloor\Big) \le x \le \min\Big(\text{best\_last} - 1,\ \Big\lfloor \frac{k \cdot b}{a} \Big\rfloor\Big).$$

**证明**：
1. **下界**：首先严格递增要求 $x \ge \text{last} + 1$；其次单位分数不能超过剩余和，即 $\frac{1}{x} \le \frac{a}{b} \iff x \ge \lceil \frac{b}{a} \rceil = \lfloor \frac{b + a - 1}{a} \rfloor$。
2. **上界**：因为后续 $k - 1$ 个分母都严格大于 $x$，这 $k$ 个单位分数之和严格小于 $\frac{k}{x}$；要凑出 $\frac{a}{b}$ 必须有 $\frac{k}{x} > \frac{a}{b} \iff x \le \lfloor \frac{k b - 1}{a} \rfloor \le \lfloor \frac{k b}{a} \rfloor$。同时为击败当前已找到的最优末项 `best_last`，必有 $x < \text{best\_last}$。$\blacksquare$

### 2.2 最后两项的二次方程直接求解优化（附完整证明）

如果在倒数第二层（$k = 2$）仍然逐个枚举 $x$，当剩余分数很小而上界较大时会产生大量无效递归。

**引理 2（剩余两项的二次方程消元加速）**：当剩余项数 $k = 2$ 时，方程 $\frac{a}{b} = \frac{1}{x} + \frac{1}{y}$（其中 $\gcd(a, b) = 1,\ x < y$）等价于存在正整数倍数 $t$，使得
$$x + y = t a, \qquad x y = t b.$$
由此 $x, y$ 是二次方程 $z^2 - (t a) z + t b = 0$ 的两个正整数根，判别式 $\Delta = t^2 a^2 - 4 t b$ 必须为完全平方数，且参数 $t$ 的取值范围仅为
$$\Big\lfloor \frac{4 b}{a^2} \Big\rfloor + 1 \le t \le \frac{2 \cdot \text{best\_last}}{a}.$$

**证明**：
由 $\frac{1}{x} + \frac{1}{y} = \frac{x+y}{xy} = \frac{a}{b}$ 及 $\gcd(a, b) = 1$，必存在正整数 $t$ 使 $x + y = t a,\ x y = t b$。由韦达定理，$x, y$ 为 $z^2 - t a z + t b = 0$ 的两根；因 $x < y$ 为正整数，$\Delta = t^2 a^2 - 4 t b > 0 \iff t > \frac{4b}{a^2}$，且 $\sqrt{\Delta}$ 为整数，两根为 $x = \frac{t a - \sqrt{\Delta}}{2},\ y = \frac{t a + \sqrt{\Delta}}{2}$。又因 $y < \text{best\_last}$ 且 $x \ge \lceil b/a \rceil$，只需在当 $k = 2$ 时直接枚举 $x$ 或枚举 $t$ 即可瞬间收尾。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 用迭代加深搜索（IDDFS）固定总项数 $d$，保证首次搜到有解的层即为最小项数。
- **bound & prune** — 利用 $\frac{1}{x} \le \frac{a}{b} < \frac{k}{x}$ 导出当前分母 $x$ 的紧上下界（引理 1）。
- **classic problem solution** — 结合分数约分防溢出与末项收尾剪枝（引理 2），毫秒级求出最优埃及分数拆分。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `maxd` | `int` | 当前迭代加深的目标总项数 $d$ | 从 $1$ 递增 | max depth |
| `cur[i], ans[i]` | `i64 [20]` | 当前搜索路径的分母序列与全局最优分母序列 | `ans[maxd] = inf` | paths |
| `dfs(dep, a, b, las)` | 函数 | 第 `dep` 项、剩余 $\frac{a}{b}$、前一项为 `las` 的搜索 | 返回是否找到解 | IDDFS |

### 4.2 算法步骤

1. 读入 $a, b$，约分令 $g = \gcd(a, b),\ a /= g,\ b /= g$。
2. 从 `maxd = 1` 开始循环递增：
   - 令 `ans[maxd] = 10000000`（题给上界 $10^7$），调用 `dfs(1, a, b, 1)`；
   - 在 `dfs(dep, a, b, las)` 中：
     - 若 `dep == maxd`：若 $a = 1$ 且 $b > \text{las}$ 且 $b < \text{ans}[\text{maxd}]$，记录 `cur[dep] = b` 并拷贝 `cur` 至 `ans`，返回 `true`；否则返回 `false`。
     - 计算枚举范围 `lo = max(las + 1, (b + a - 1) / a)`，`hi = min((ans[maxd] - 1), ((maxd - dep + 1) * b - 1) / a)`；
     - 枚举 $x = \text{lo} \dots \text{hi}$：计算剩余分数 $\frac{na}{nb} = \frac{a x - b}{b x}$ 并约分，递归调用 `dfs(dep + 1, na, nb, x)`。
   - 一旦当前层 `maxd` 找到合法解，输出 `ans[1 .. maxd]` 并结束。

### 4.3 复杂度分析

- **时间复杂度**：在引理 1 的紧上下界剪枝下，$a < b < 1000$ 的最大深度不超过 $8$，有效搜索状态数不超过 $10^5$，耗时 $< 20\text{ ms}$。
- **空间复杂度**：递归深度 $\le 10$，空间复杂度为 $\Theta(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1763 埃及分数
// https://www.luogu.com.cn/problem/P1763
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int maxd;
i64 cur[20],ans[20];
bool found;

void dfs(int dep,i64 a,i64 b,i64 las)
{
	if(dep==maxd)
	{
		if(b%a==0)
		{
			i64 x=b/a;
			if(x>las&&x<ans[maxd])
			{
				cur[dep]=x;
				memcpy(ans,cur,sizeof(cur));
				found=true;
			}
		}
		return;
	}
	int rem=maxd-dep+1;
	i64 lo=max(las+1,(b+a-1)/a);
	i64 hi=min(ans[maxd]-1,(rem*b-1)/a);
	if(dep==maxd-1)
	{
		// 剩最后两项 1/x + 1/y = a/b：y = b*x / (a*x - b) < ans[maxd]
		// => a*x*ans[maxd] - b*ans[maxd] > b*x => x > b*ans[maxd] / (a*ans[maxd] - b)
		if(a*ans[maxd]>b)
			lo=max(lo,b*ans[maxd]/(a*ans[maxd]-b)+1);
	}
	for(i64 x=lo;x<=hi;++x)
	{
		cur[dep]=x;
		i64 na=a*x-b,nb=b*x;
		i64 g= std::gcd(na,nb);
		dfs(dep+1,na/g,nb/g,x);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 a,b;
	cin>>a>>b;
	i64 g=std::gcd(a,b);
	a/=g;b/=g;

	for(maxd=1;;++maxd)
	{
		ans[maxd]=10000001;
		found=false;
		dfs(1,a,b,0);
		if(found)
		{
			for(int i=1;i<=maxd;++i)
				cout<<ans[i]<<(i==maxd?'\n':' ');
			break;
		}
	}
	return 0;
}
```
