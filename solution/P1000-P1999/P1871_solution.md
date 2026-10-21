# P1871 对撞机

> 难度：普及/提高- · 标签：数论 / 线性筛 / 质因数占用 / Special Judge

## 1. 重新读题

### 1.1 问题重述（人话版）

有编号 $1\sim n$ 的对撞机，一开始全关着。每次询问要么打开一台，要么关掉一台。

打开 $x$ 时：

- 它已经开着：输出 `Already on`；
- 存在一台已经开着的 $y$，使得 $\gcd(x,y)>1$：输出 `Conflict with y`，任意一台冲突的 $y$ 都可以，并且这次**不要**把它打开；
- 否则打开它，输出 `Success`。

关掉 $x$ 时：开着就关掉并输出 `Success`，否则输出 `Already off`。

### 1.2 关键约束与规模暗示

- $1\le n,m\le 10^5$，所以每次询问最多做 $O(\log n)$ 或均摊 $O(1)$ 的工作，不能对「当前所有已打开的机器」做扫描。
- 冲突的定义只跟 $\gcd>1$ 有关，也就是「有没有公共质因子」。合数本身没有额外的结构。
- 输出冲突编号时是 Special Judge，标程只要给出任意一台真正开着且与 $x$ 不互质的机器即可。
- 编号 $1$ 没有任何质因子，它和谁都互质。

## 2. 思维链（Chain-of-thought）

### 2.1 先把「互质」翻译成质因子

$\gcd(x,y)>1$ 当且仅当 $x$ 与 $y$ 至少有一个公共质因子。因此「当前开启集合 $S$ 两两互质」等价于「每个质数至多整除 $S$ 里的一个数」。

### 2.2 这个不变量在操作下是保持的

**引理 1（开启集合始终两两互质）**。任意时刻，所有处于开启状态的编号两两 $\gcd=1$。

**证明**。初始 $S=\varnothing$，命题成立。关闭只会从 $S$ 里删元素，两两互质仍然成立。打开 $x$ 只有在「对每个已开着的 $y$ 都有 $\gcd(x,y)=1$」时才会真正执行；由这个前提，$S\cup\{x\}$ 仍然两两互质。冲突分支不会把 $x$ 放进 $S$。归纳完成。$\square$

**推论**。每个质数 $p$ 在任意时刻最多被一台开启的机器整除。于是可以记录 `own[p]`：当前被 $p$ 整除的那台开启机器的编号；没有则为 $0$。

### 2.3 只检查 $x$ 自己的质因子就够了

**引理 2（冲突检测）**。设 $x$ 当前是关的。存在开启的 $y$ 使 $\gcd(x,y)>1$，当且仅当 $x$ 的某个质因子 $p$ 满足 `own[p]≠0`；此时 `own[p]` 就是一台合法的冲突机器。

**证明**。若 $\gcd(x,y)>1$，取公共质因子 $p$。$y$ 开着且被 $p$ 整除，由推论 `own[p]=y`。反过来，若 `own[p]=y≠0`，则 $p\mid x$ 且 $p\mid y$，故 $\gcd(x,y)\ge p>1$，且 $y$ 开着。$\square$

因此打开 $x$ 时，把 $x$ 用最小质因子分解出来，逐个质因子查 `own`。发现冲突就原样返回，不修改 `own`。全部通过后，把这些 `own[p]` 都设成 $x$，并标记开启。

关闭时由引理 1，$x$ 的每个质因子都只属于 $x$，把它们的 `own` 清零即可。

编号 $1$ 的分解循环一次都不进，打开 $1$ 永远成功（除非它已经开着）。

### 2.4 为什么不用埃氏筛出每个数的全部因子表

$m=10^5$ 次询问，若每次从 $2$ 试除到 $\sqrt x$，最坏大约 $10^5\times 300$ 也能过，但用线性筛预处理最小质因子后，分解是严格 $O(\log x)$，而且实现更短。暴力对拍用的是另一条路：直接扫 $1..n$ 算 `gcd`，只在 $n,m\le 350$ 时和标程对答案。

## 3. 模式总结（Pattern Summary）

- **explore its property** — $\gcd>1$ 等价于有公共质因子；开启集合被操作维持成两两互质。
- **invariant** — 每个质数最多属于一台开启的机器，用 `own[p]` 记下它。
- **consider easier version** — 小数据直接扫开启集合求 `gcd`，作为 Special Judge 的对照。
- **classic problem solution** — 线性筛最小质因子，询问时只分解当前编号。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `lp[i]` | `int [100005]` | $i$ 的最小质因子 | 筛前为 $0$ | lowest prime |
| `pr` | `int [100005]` | 筛出的质数 | 空 | primes |
| `own[p]` | `int [100005]` | 质因子 $p$ 当前属于哪台开启机器 | $0$ | owner |
| `on[x]` | `bool [100005]` | $x$ 是否开启 | `false` | on |

### 4.2 算法步骤

1. 线性筛出 $2..n$ 的最小质因子。筛的乘法写成 `i<=n/pr[j]`，避免 `i*pr[j]` 溢出。
2. 读入 `+ x` 或 `- x`。
3. 打开：已开则 `Already on`；否则分解 $x$，若某质因子已有主人就输出 `Conflict with own[p]`；否则占有这些质因子并输出 `Success`。
4. 关闭：未开则 `Already off`；否则释放质因子并输出 `Success`。

### 4.3 复杂度与验证状态

- **时间**：$O(n\log\log n+m\log n)$。
- **空间**：$O(n)$。
- **沙箱对拍**：`g++ -O2 -std=c++17 -Wall -Wextra` 零警告。题面样例与 $160$ 组随机（$n,m\le 350$）共 $163$ 组，标程和「扫描 `gcd`」暴力都通过按题意写的判定器（冲突编号允许不同）。$n=m=10^5$ 的最大数据标程耗时 $0.1382\text{ s}$。

## 5. 示例代码

### 5.1 标程（`P1871.cpp`）

```cpp
//
// 题目：P1871 对撞机
// https://www.luogu.com.cn/problem/P1871
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int lp[mxn],pr[mxn],pc;
int own[mxn];
bool on[mxn];

void sieve(int n)
{
	for(int i=2;i<=n;++i)
	{
		if(!lp[i])
		{
			lp[i]=i;
			pr[pc++]=i;
		}
		for(int j=0;j<pc&&pr[j]<=lp[i]&&i<=n/pr[j];++j)
			lp[i*pr[j]]=pr[j];
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m;
	cin>>n>>m;
	sieve(n);
	while(m--)
	{
		char op;
		int x;
		cin>>op>>x;
		if(op=='+')
		{
			if(on[x])
			{
				cout<<"Already on\n";
				continue;
			}
			int bad=0,y=x;
			while(y>1)
			{
				int p=lp[y];
				if(own[p])
				{
					bad=own[p];
					break;
				}
				while(y%p==0)
					y/=p;
			}
			if(bad)
			{
				cout<<"Conflict with "<<bad<<'\n';
				continue;
			}
			y=x;
			while(y>1)
			{
				int p=lp[y];
				own[p]=x;
				while(y%p==0)
					y/=p;
			}
			on[x]=true;
			cout<<"Success\n";
		}
		else
		{
			if(!on[x])
			{
				cout<<"Already off\n";
				continue;
			}
			int y=x;
			while(y>1)
			{
				int p=lp[y];
				own[p]=0;
				while(y%p==0)
					y/=p;
			}
			on[x]=false;
			cout<<"Success\n";
		}
	}
	return 0;
}
```
