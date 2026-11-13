# P1779 [JAG 2009 Summer Camp #2] 魔法杀手

## 1. 重新读题

有 $N$（$1 \le N \le 100$）只怪兽，第 $i$ 只怪兽的生命值为 $HP_i$（$1 \le HP_i \le 10^5$）。共有 $M$（$1 \le M \le 100$）种魔法，每种魔法消耗魔力 $MP_j$、造成伤害 $D_j$，作用目标为 `Single`（单体）或 `All`（全体），每种魔法均可无限次施放。

当所有怪兽的生命值都降至 $\le 0$ 时获胜。求击败所有怪兽所需消耗的最小总魔力。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：全体伤害与单体伤害的完全解耦

由于魔法施放顺序不影响总伤害，我们可以把所有魔法分成两类独立处理：
1. 先决定所有 `All`（全体）魔法一共造成了多少点全体伤害 $A$；
2. 固定全体伤害 $A$ 后，第 $i$ 只怪兽还剩下 $\max(0, HP_i - A)$ 点生命值，这些剩余生命值必须分别用 `Single`（单体）魔法独立打掉！

**引理 1（两套完全背包预处理 + 枚举全体伤害解耦定理）**：
记 $H = \max_{1 \le i \le N} HP_i \le 10^5$。
- 用所有 `All` 魔法跑一次容量为 $H$ 的完全背包（超出 $H$ 的伤害视为 $H$），求出造成至少 $a$ 点全体伤害的最小魔力 $\text{all\_dp}[a]$（$0 \le a \le H$）；
- 用所有 `Single` 魔法跑一次容量为 $H$ 的完全背包，求出对单只怪兽造成至少 $h$ 点伤害的最小魔力 $\text{sin\_dp}[h]$（$0 \le h \le H$）；
则全局最小魔力消耗等于：
$$\min_{0 \le a \le H} \left( \text{all\_dp}[a] + \sum_{i=1}^{N} \text{sin\_dp}\big[\max(0, HP_i - a)\big] \right).$$

**证明**：若存在 $0$ 魔力且正伤害的魔法，答案直接为 $0$（在完全背包中也自然得到 $\text{dp} = 0$）。对于任意合法方案，设其全体魔法总伤害对 $H$ 取 $\min$ 后为 $a \in [0, H]$，则其全体魔法耗蓝至少为 $\text{all\_dp}[a]$，且对每只怪兽 $i$ 单独施放的单体魔法总伤害必 $\ge \max(0, HP_i - a)$，耗蓝至少为 $\text{sin\_dp}[\max(0, HP_i - a)]$。反之，取达到各自完全背包最优值的魔法组合即可合法击败所有怪兽。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将魔法按 `All` 与 `Single` 拆分，发现一旦固定全体伤害总量 $a$，各怪兽的剩余血量完全独立（引理 1）。
- **classic problem solution** — 分别对 `All` 魔法和 `Single` 魔法运行上限为 $H = \max HP_i$ 的完全背包并做后缀最小值收敛，最后 $O(N \cdot H)$ 枚举全体伤害 $a$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 怪兽数与魔法数 | 标准输入 | 维度 |
| `hp[i]` | `int [105]` | 第 $i$ 只怪兽的生命值 | 标准输入 | monster HP |
| `all_dp[a]` | `i64 [100005]` | 造成至少 $a$ 点全体伤害的最小魔力 | `all_dp[0]=0`，其余 `inf` | AOE knapsack |
| `sin_dp[h]` | `i64 [100005]` | 造成至少 $h$ 点单体伤害的最小魔力 | `sin_dp[0]=0`，其余 `inf` | Single knapsack |

### 4.2 算法步骤

1. 读入 $N$ 只怪兽的 $HP_i$，记最大生命值为 $H$。
2. 初始化 `all_dp[0] = sin_dp[0] = 0`，其余位置为 `inf`。
3. 读入 $M$ 种魔法：忽略 $D_j = 0$ 的无用魔法；根据 `All` 或 `Single` 更新对应的完全背包：`dp[min(H, x + dmg)] = min(..., dp[x] + mp)`。
4. 从 $H - 1$ 倒序至 $0$ 用 `dp[x + 1]` 更新 `dp[x]`（因为造成更多伤害同样满足“至少 $x$ 点伤害”）。
5. 枚举 $a = 0 \dots H$：若 `all_dp[a] < inf`，计算 `all_dp[a] + sum_i sin_dp[max(0, hp[i] - a)]` 并更新全局最小值 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：两次完全背包耗时 $O(M \cdot H)$，枚举全体伤害耗时 $O(N \cdot H)$，总时间复杂度为 $O((N + M) H) \le 200 \times 10^5 = 2 \times 10^7$ 次操作，耗时 $< 20\text{ ms}$。
- **空间复杂度**：两个长度为 $100005$ 的一维数组，空间复杂度为 $\Theta(H)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1779 [JAG 2009 Summer Camp #2] 魔法杀手
// https://www.luogu.com.cn/problem/P1779
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxh{100005};
constexpr i64 inf{1145141919810LL};

int n,m;
int hp[105];
i64 all_dp[mxh],sin_dp[mxh];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int H=0;
	for(int i=0;i<n;++i)
	{
		cin>>hp[i];
		H=max(H,hp[i]);
	}

	fill_n(all_dp+1,H,inf);
	fill_n(sin_dp+1,H,inf);

	cin>>m;
	for(int i=0;i<m;++i)
	{
		string name,type;
		i64 mp,dmg;
		cin>>name>>mp>>type>>dmg;
		if(dmg<=0)
			continue;
		i64*dp=(type=="All")?all_dp:sin_dp;
		for(int x=0;x<=H;++x)
			if(dp[x]<inf)
			{
				int nx=(int)min((i64)H,x+dmg);
				dp[nx]=min(dp[nx],dp[x]+mp);
			}
	}

	for(int x=H-1;x>=0;--x)
	{
		all_dp[x]=min(all_dp[x],all_dp[x+1]);
		sin_dp[x]=min(sin_dp[x],sin_dp[x+1]);
	}

	i64 ans=inf;
	for(int a=0;a<=H;++a)
		if(all_dp[a]<inf)
		{
			i64 cur=all_dp[a];
			bool ok=true;
			for(int i=0;i<n;++i)
			{
				int rem=max(0,hp[i]-a);
				if(sin_dp[rem]>=inf){ok=false;break;}
				cur+=sin_dp[rem];
			}
			if(ok)
				ans=min(ans,cur);
		}
	cout<<ans<<'\n';
	return 0;
}
```
