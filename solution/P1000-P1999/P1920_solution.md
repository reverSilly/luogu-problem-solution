# P1920 成功密码

## 1. 重新读题

### 1.1 问题重述（人话版）

给定实数 $x$ 与正整数 $N$，求

$$S_N(x)=\sum_{i=1}^{N}\frac{x^{i}}{i},$$

输出保留 $4$ 位小数。

### 1.2 关键约束与规模暗示

- $1\le N\le 10^{18}$：**不能**老老实实循环 $N$ 次，必须找闭式或快速收敛的办法。
- $0<x\le1$，且 $x$ **最多 4 位小数**：这是本题真正的命门。它意味着若 $x<1$，则 $x\le0.9999$，从而 $1-x\ge10^{-4}$——「离 1 有多近」被一个常数 $10^{-4}$ 卡住了。
- 只要求 $4$ 位小数，允许 $10^{-5}$ 量级的误差；这为我们「截断级数」提供了合法空间。
- $N$ 可能很小（比如 $N=1$），此时答案就是 $x$，任何渐近公式都必须能与小 $N$ 无缝衔接。

## 2. 思维链（Chain-of-thought）

### 2.1 先看这个和是什么

$S_N(x)=\sum_{i=1}^{N}x^i/i$ 是幂级数

$$\sum_{i\ge1}\frac{x^i}{i}=-\ln(1-x)\qquad(|x|<1)$$

的部分和。因此有两种完全不同的行为：

- 若 $x<1$：级数**收敛**，$S_\infty(x)=-\ln(1-x)$，$S_N$ 与它的差是「尾巴」，随 $N$ 指数衰减；
- 若 $x=1$：级数变成调和级数 $H_N$，**发散**，没有初等闭式，$H_N$ 随 $N$ 对数增长。$N$ 最大 $10^{18}$，$H_N\approx42$。

所以必须**分两类**处理。这也解释了为什么 $30\%$ 的数据只给 $N\le10^6$：那一档直接循环就能过，而正解要处理 $N=10^{18}$ 的两种极端。

### 2.2 情形一：$x<1$，用尾部估计决定循环次数

直接逐项加。问题是：加到第 $K$ 项停下，误差多大？

**引理 1（尾项上界）**：对 $0<x<1$ 与 $K<N$，

$$0\le\sum_{i=K+1}^{N}\frac{x^{i}}{i}\le\frac{x^{K+1}}{(K+1)(1-x)}.$$

**证明**：当 $i\ge K+1$ 时 $\dfrac{1}{i}\le\dfrac{1}{K+1}$，于是

$$\sum_{i=K+1}^{N}\frac{x^i}{i}\le\frac{1}{K+1}\sum_{i=K+1}^{N}x^i\le\frac{1}{K+1}\sum_{i=K+1}^{\infty}x^i=\frac{x^{K+1}}{(K+1)(1-x)},$$

最后一步是首项 $x^{K+1}$、公比 $x$ 的等比级数。$\square$

**引理 2（项数上界）**：由于 $x\le0.9999$，取 $K=4\times10^5$ 就有尾项 $<10^{-18}$。

**证明**：由 $1-t\le e^{-t}$ 得 $x\le0.9999\le e^{-10^{-4}}$，故

$$x^{K+1}\le e^{-10^{-4}(K+1)}=e^{-40.0001}\approx4.2\times10^{-18}.$$

又 $1-x\ge10^{-4}$，代入引理 1：

$$\text{tail}\le\frac{4.2\times10^{-18}}{4\times10^{5}\cdot10^{-4}}\approx1.1\times10^{-19}.$$

$\square$

$1.1\times10^{-19}$ 远小于 $10^{-5}$，足够保证 4 位小数正确。实现上采用**自适应停机**：维护 `xp` $=x^{i}$，每加一项就估计 $\text{tail}\approx x^{i+1}/((i+1)(1-x))$，小于 $10^{-13}$ 就停。

数据规模的分析就在这里用掉了「4 位小数」这个条件：它把 $1/(1-x)$ 压到 $10^4$ 以内，于是项数只与 $\log(1/\varepsilon)$ 有关，约 $3\times10^5$，而不是随 $N$ 增长。

**一个容易犯的错**：只看「当前项 $x^i/i<10^{-5}$」就停。当前项小不代表尾巴小：尾巴被 $1/(1-x)\le10^4$ 放大了。必须用引理 1 的整体界（或再乘上这个放大因子）。例如 $x=0.9999$ 时，项到 $10^{-9}$ 时尾巴仍可能有 $10^{-5}$ 量级。

### 2.3 情形二：$x=1$，调和数

$S_N(1)=H_N=\sum_{i=1}^N 1/i$。$N\le10^{18}$ 时不能循环。用调和数的经典渐近展开（Euler–Maclaurin）：

$$H_N=\ln N+\gamma+\frac{1}{2N}-\frac{1}{12N^{2}}+\frac{1}{120N^{4}}-\frac{1}{252N^{6}}+\cdots$$

其中 $\gamma=0.5772156649015328606\ldots$ 是 Euler–Mascheroni 常数。

**推导（要点）**：对 $f(t)=1/t$ 用 Euler–Maclaurin 求和公式

$$\sum_{k=1}^{N}f(k)=\int_{1}^{N}f(t)\,dt+\frac{f(1)+f(N)}{2}+\sum_{r\ge1}\frac{B_{2r}}{(2r)!}\Bigl(f^{(2r-1)}(N)-f^{(2r-1)}(1)\Bigr)+R.$$

代入 $f(t)=1/t$：$\int_1^N dt/t=\ln N$，$\frac{f(1)+f(N)}2=\frac12+\frac1{2N}$，$\frac{B_2}{2!}(f'(N)-f'(1))=\frac1{12}-\frac1{12N^2}$，$\frac{B_4}{4!}(f'''(N)-f'''(1))=-\frac1{120}+\frac1{120N^4}$，$\ldots$ 把与 $N$ 无关的常数项合并，其极限正是 $\gamma$（这正是 $\gamma$ 的定义），于是得到上面的展开。

**引理 3（截断误差）**：对上式只保留到 $-\dfrac{1}{12N^2}$ 项，误差不超过 $\dfrac{1}{120N^4}$。因此当 $N\ge10^{6}$ 时误差约 $8\times10^{-27}$，对 4 位小数毫无影响。

**证明**：$f(t)=1/t$ 的 $m$ 阶导数为 $f^{(m)}(t)=(-1)^m m!/t^{m+1}$，在 $[1,\infty)$ 上符号固定且绝对值单调递减，这正是 Euler–Maclaurin 余项「符号与首个被舍弃项相同、绝对值不超过它」的条件。所以截断误差不超过被舍弃的第一项 $\left|\frac{B_4}{4!}f'''(N)\right|=\frac{1}{120N^4}$。当 $N\ge10^6$ 时它约 $8.3\times10^{-27}$。$\square$

**数值精度**：$\ln N$ 用 `logl` 计算，$N\le10^{18}$ 时 $\ln N\le41.45$，`long double` 的相对误差约 $10^{-19}$，绝对误差约 $10^{-18}$；$\gamma$ 直接写成 19 位有效数字的字面量。总误差远小于 $5\times10^{-5}$。

**小 $N$ 的衔接**：对 $N\le2\times10^6$ 直接循环求和（至多两百万次，瞬间完成），此时结果精确到 `long double` 的最后一位；$N>2\times10^6$ 才走渐近式。两段在 $N=2\times10^6$ 处的差别约 $1/N=5\times10^{-7}$，实测两侧输出一致。

### 2.4 怎么判断 $x=1$

不要用 `x == 1.0` 浮点比较。把 $x$ 当字符串读入，转成整数 $x_4=\text{round}(x\cdot10^4)$：整数部分乘 $10000$ 加上小数部分补足 4 位。$x_4=10000$ 当且仅当 $x=1$。其余情形 $x=x_4/10^4$ 精确表示（分母是 $10^4$，在二进制下不精确，但用 `long double` 误差约 $10^{-19}$，可接受）。

### 2.5 复杂度

- $x<1$：至多约 $4\times10^5$ 次迭代，$O\!\left(\dfrac{\log(1/\varepsilon)}{1-x}\right)$，与 $N$ 无关；
- $x=1$：$N\le2\times10^6$ 时 $O(N)$，否则 $O(1)$。

整体在时间与精度上都远低于限制。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 这是 $-\ln(1-x)$ 的部分和：$x<1$ 收敛，$x=1$ 退化为调和级数。
- **discuss by category** — 按 $x=1$ 与 $x<1$ 分成两条完全不同的路线。
- **explore its property** — 用等比级数给出尾部上界 $x^{K+1}/((K+1)(1-x))$，把 $N$ 变成与答案无关的「截断项数」。
- **use the constraint** — 「$x$ 最多 4 位小数」$\Rightarrow 1-x\ge10^{-4}\Rightarrow$ 只需 $O(10^4\log)$ 项。
- **classic problem solution** — $x=1$ 用 Euler–Maclaurin 渐近展开 $H_N=\ln N+\gamma+\frac1{2N}-\frac1{12N^2}$，误差 $O(N^{-4})$。
- **numerical care** — 字符串解析判 $x=1$；`long double`；自适应停机用整体尾界而非单项。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `sx` | `string` | 读入的 $x$ 原文 | 读入 | string-x |
| `N` | `long long` | 项数上界 | 读入 | — |
| `x4` | `long long` | $x\cdot10^4$ 的整数值 | 由 `sx` 解析 | 4 位定点 |
| `ans` | `long double` | 累加和 | $0$ | answer |
| `xp` | `long double` | 当前 $x^i$ | $1$ | power |
| `one` | `long double` | $1-x$ | 计算 | $1-x$ |
| `tb` | `long double` | 尾项上界估计 | 每轮计算 | tail bound |

### 4.2 算法步骤

1. 读入 `sx` 与 `N`；把 `sx` 解析为四位定点整数 `x4`（整数部分 $\times10^4$ + 小数部分补零到 4 位）。
2. 若 `x4 == 10000`（即 $x=1$）：
   - 若 $N\le2\times10^6$：`for i=1..N: ans += 1.0L/i`；
   - 否则 `ans = logl(N) + γ + 1/(2N) - 1/(12N²)`。
3. 否则（$x<1$）：令 `x = x4/10000.0L`，`one = 1-x`，`xp = 1`；对 $i=1,2,\dots$ 直到 $i>N$：
   - `xp *= x`；`ans += xp/i`；
   - 计算 `tb = xp*x/((i+1)*one)`；若 `tb < 1e-13` 则跳出。
4. `printf("%.4Lf\n", ans)`。

### 4.3 复杂度与验证状态

- **时间复杂度**：$x<1$ 时至多约 $4\times10^5$ 次循环；$x=1$ 时 $O(\min(N,2\times10^6))$ 或 $O(1)$。
- **空间复杂度**：$O(1)$。
- 编译 `g++ -std=c++17 -O2 -Wall -Wextra`，无警告。
- 自测：`x=1,N=1` 得 `1.0000`；`x=1,N=10` 得 `2.9290`（$H_{10}$）；`x=0.5,N=10^6` 与直接求和一致 `0.6931`；`x=0.9999,N=10^{18}` 得 `9.2103`（$=\ln10^4$）；`x=1,N=10^{18}` 得 `42.0237`（$=\ln10^{18}+\gamma$）。$x=1$ 时 $N=2\times10^6$ 与 $2000001$ 两侧输出一致。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string sx;
	long long N;
	cin>>sx>>N;

	long long ip=0,fp=0,x4;
	int fd=0;
	bool dot=false;
	for(char c:sx)
	{
		if(c=='.')
		{
			dot=true;
			continue;
		}
		if(!dot)
			ip=ip*10+c-'0';
		else
		{
			fp=fp*10+c-'0';
			++fd;
		}
	}
	while(fd<4)
	{
		fp*=10;
		++fd;
	}
	x4=ip*10000+fp;

	long double ans=0;
	if(x4==10000)          // x == 1，调和级数
	{
		if(N<=2000000)
		{
			for(long long i=1;i<=N;++i)
				ans+=1.0L/i;
		}
		else
		{
			long double nd=(long double)N;
			ans=logl(nd)+0.577215664901532860606512090082402431L
				+1.0L/(2*nd)-1.0L/(12*nd*nd);
		}
	}
	else                   // x <= 0.9999，尾项指数衰减
	{
		long double x=(long double)x4/10000.0L;
		long double xp=1;
		long double one=1-x;
		for(long long i=1;i<=N;++i)
		{
			xp*=x;
			ans+=xp/i;
			long double tb=xp*x/((i+1)*one);
			if(tb<1e-13L)
				break;
		}
	}
	printf("%.4Lf\n",ans);
	return 0;
}
```
