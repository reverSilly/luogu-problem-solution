# P1857 质数取石子

## 1. 重新读题

### 1.1 问题重述（人话版）

桌上有 $n$ 颗石子。两人轮流取，每次必须取走**质数**颗石子，不能不取。谁轮到自己的回合时取不了（即剩余石子数小于 $2$），谁就输。

题目额外规定了双方的“步数偏好”：**必胜方希望尽快结束，必败方希望尽量拖延**。求在双方都采取最优策略时，先手若必胜，需要的最少步数（一人取一次算一步）；若先手必败，输出 $-1$。

### 1.2 关键约束与规模暗示

- 石子数 $n \le 20000$，数据组数 $N \le 10$。
- 只能取质数，天然要求先筛出 $[1,20000]$ 内所有质数。
- 状态只由“当前剩余石子数”决定，是典型的**无环博弈**：每次操作都严格减少石子数，所以状态转移构成 DAG，可以按 $n$ 从小到大递推。
- $N$ 组数据可以共用一张表：只需对所有询问取最大值 $mx$，一次递推到 $mx$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：先不管“步数”，只看胜负

先把“最少/最多步数”这层剥掉，只回答“当前是必胜态还是必败态”。

设 $n$ 为当前石子数，$\mathcal P$ 为所有不超过 $n$ 的质数。一步操作把 $n$ 变成 $n-p\ (p\in\mathcal P)$。

**定义**：$n$ 是必败态（lose）当且仅当所有可达的 $n-p$ 都是必胜态；特别地，$n<2$ 时没有任何合法操作，是必败态。

**引理 1（胜负判据）**：对任意 $n\ge 2$，
$$
n\text{ 必败}\iff \forall p\in\mathcal P,\ n-p\text{ 必胜}.
$$

**证明**：这是有向无环博弈的胜负定义本身，但为避免循环论证，我们用归纳法确认它自洽。对 $n$ 归纳：

- $n=0,1$：无操作，按定义必败，引理中“所有可达态都必胜”是空真命题，结论一致。
- 设对所有 $<n$ 的数引理成立。若存在 $p$ 使 $n-p$ 必败，则先手取 $p$ 后把必败态留给对手，先手必胜；若所有 $n-p$ 都必胜，则先手无论怎么取，留给对手的都是必胜态，先手必败。两种情况互斥且穷尽。$\blacksquare$

注意关键点：**只需要**知道每个可达子态的胜负，不需要知道步数。这说明两个量可以分开算。

### 2.2 加入步数：两种角色的目标不同

题目里“步数”是一个从当前状态到终局的整局总步数。双方偏好相反：

- 处于**必胜态**的人：他希望总步数尽量小；
- 处于**必败态**的人：他明知会输，但希望总步数尽量大。

**引理 2（步数最优性方程）**：设 $\mathrm{step}(n)$ 表示从 $n$ 出发、双方按各自偏好行动时，从 $n$ 到终局的总步数。则
$$
\mathrm{step}(n)=
\begin{cases}
0, & n<2,\\[2pt]
1+\min\{\mathrm{step}(n-p):p\in\mathcal P,\ n-p\text{ 必败}\}, & n\text{ 必胜},\\[2pt]
1+\max\{\mathrm{step}(n-p):p\in\mathcal P\}, & n\text{ 必败},
\end{cases}
$$
其中必败态的 $\max$ 只可能在 $n\ge 2$ 时出现。

**证明**：对 $n$ 归纳。

- $n<2$：无合法操作，整局步数为 $0$，正确。
- 必胜态：先手必须把局面交给对手的必败态（由引理 1，这样的 $p$ 存在）。交出后，对手在必败态上会按 $\max$ 拖延，因此先手选 $p$ 后的总步数是 $1+\mathrm{step}(n-p)$。先手要最小化它，故取 $\min$。由于 $\mathrm{step}(n-p)$ 已由归纳假设是对手最优行为下的真实步数，取 $\min$ 就是先手最优。
- 必败态且 $n\ge2$：先手所有走法都留给对手必胜态。无论先手取哪个 $p$，之后对手都会以“必胜方尽快”的方式结束，总步数为 $1+\mathrm{step}(n-p)$。先手（必败方）要拖延，故取 $\max$。同样由归纳假设，各子态的值已是真实值。

三种情形覆盖全部状态；递推依赖的所有 $n-p<n$，归纳良基。$\blacksquare$

### 2.3 一个容易踩的坑：必败态也要取 $\max$

初学者常把“必败”直接记成 $-1$ 或直接不存步数。但题目要求必败方拖延，而胜方步数在更高层会被引用：例如 $n$ 是必胜态、先手取 $p$ 让对手落到必败态 $n-p$，此时 $n-p$ 上还会继续走很多步，这些步数必须计入总步数。因此**每个状态都要存步数**，只是胜利状态最终不输出而已。

另一个坑：$n=1$ 取不了任何质数，是必败态且步数为 $0$，不是 $-1$ 的“步数”，输出时按题意输出 $-1$（表示必败）。这两个 $-1$ 含义不同，代码里要分开。

### 2.4 归约为经典做法

- 线性筛出 $[1,mx]$ 的所有质数；
- 按 $n=0,1,\dots,mx$ 递推；
- 对每个 $n$ 枚举质数 $p\le n$，用 $n-p$ 的胜负与步数更新。

总复杂度 $O\!\left(mx\cdot \pi(mx)\right)$，其中 $mx=20000,\ \pi(20000)=2262$，约 $2\times10^7$ 次枚举，完全可行。多组数据只查表。

**引理 3（枚举质数上界）**：若 $n$ 必败，则所有 $p\le n$ 都满足 $n-p$ 必胜；若 $n$ 必胜，则至少一个 $n-p$ 必败。因此按“是否存在必败子态”就能同时判胜负。

**证明**：由引理 1 的直接改写。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 这是无环取石子博弈，状态只与剩余石子数有关，可从小到大递推。
- **consider easier version** — 先剥掉步数，只做胜负判定（标准必胜/必败态）。
- **classic problem solution** — 胜负态 + 步数双数组一起递推；必胜取 $\min$、必败取 $\max$ 对应两人相反偏好。
- **precompute once** — 多组询问取最大值，只筛一次、只递推一次，之后 $O(1)$ 查表。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `mx` | `int` | 所有询问中的最大石子数 | 读入后取 max | max query |
| `prm[i]` | `int [mxp]` | 筛出的第 $i$ 个质数 | 线性筛 | prime |
| `pc` | `int` | 质数个数 | 0 | prime count |
| `lose[n]` | `bool [mxp]` | $n$ 是否为必败态 | `lose[0]=lose[1]=true` | 必败 |
| `step[n]` | `int [mxp]` | 从 $n$ 出发的最优总步数 | 0 | 步数 |
| `q[i]` | `int [15]` | 第 $i$ 组询问 | 读入 | query |

### 4.2 算法步骤

1. 读入 $N$ 与所有询问，取 $mx=\max q_i$。
2. 线性筛求 $[2,mx]$ 内全部质数，存入 `prm`。
3. 令 `lose[0]=lose[1]=true`，`step[0]=step[1]=0`。
4. 对 $n=2\dots mx$：
   - 枚举所有 `prm[j]<=n`，令 $m=n-\text{prm}[j]$；
   - 若存在 `lose[m]`：`lose[n]=false`，`step[n]=1+min(step[m])`；
   - 若不存在：`lose[n]=true`，`step[n]=1+max(step[m])`。
5. 对每组询问 $n$：若 `lose[n]` 输出 `-1`，否则输出 `step[n]`。

### 4.3 复杂度分析

- 时间：筛法 $O(mx)$；递推 $O\!\left(\sum_{n\le mx}\pi(n)\right)=O(mx\cdot\pi(mx))\approx2\times10^7$；询问 $O(N)$。
- 空间：`lose/step` 各 $O(mx)$，质数表 $O(\pi(mx))$，总计 $O(mx)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1857 质数取石子
// https://www.luogu.com.cn/problem/P1857
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};

int N,mx,pc;
int prm[mxn];
bool notp[mxn];
bool lose[mxn];
int step[mxn];
int q[15];

void sieve(int n)
{
	for(int i=2;i<=n;++i)
	{
		if(!notp[i])
			prm[pc++]=i;
		for(int j=0;j<pc&&1LL*i*prm[j]<=n;++j)
		{
			notp[i*prm[j]]=true;
			if(i%prm[j]==0)
				break;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>N;
	for(int i=0;i<N;++i)
	{
		cin>>q[i];
		mx=max(mx,q[i]);
	}

	sieve(mx);

	lose[0]=lose[1]=true;
	step[0]=step[1]=0;
	for(int n=2;n<=mx;++n)
	{
		int mn{1145141919},mxx{-1};
		bool haslose{false};
		for(int j=0;j<pc&&prm[j]<=n;++j)
		{
			int m=n-prm[j];
			if(lose[m])
			{
				haslose=true;
				mn=min(mn,step[m]);
			}
			else
				mxx=max(mxx,step[m]);
		}
		if(haslose)
		{
			lose[n]=false;
			step[n]=mn+1;
		}
		else
		{
			lose[n]=true;
			step[n]=mxx+1;
		}
	}

	for(int i=0;i<N;++i)
	{
		if(lose[q[i]])
			cout<<-1<<'\n';
		else
			cout<<step[q[i]]<<'\n';
	}
	return 0;
}
```
