# P1070 道路游戏 题解

## 一、题意重述

有一条长度为 $n$ 的环形马路，$n$ 个工厂与 $n$ 段道路交替环绕。第 $i$ 段道路连接工厂 $i$ 与工厂 $i+1$（模 $n$ 意义下）。时刻从 $1$ 到 $m$，第 $i$ 段道路在时刻 $t$ 上有金币 $a_{i,t}$（正整数）。在工厂 $i$ 购买一个机器人需花费 $c_i$（正整数）。机器人一经购买，立即沿顺时针行走，每单位时间走一段路，收集途经道路在当前时刻的金币。机器人可设定行走次数 $k\in[1,p]$，走完后立即消失。任何时候至多存在一个机器人；机器人消失的瞬间可以在任意工厂购买下一个机器人。求经过 $m$ 个单位时间后，总金币减去总花费的最大值。

**约束**：$2\le n\le 1000,\;1\le m\le 1000,\;1\le p\le m$，金币与花费 $\le 100$。

---

## 二、推演过程

### 2.1 朴素 DP

设 $dp[t]$ 表示恰好经过 $t$ 个单位时间后的最大净收益。$dp[0]=0$。

考虑最后一个机器人。设它于时刻 $b$（即前 $b$ 个单位时间已处理完）被购买于工厂 $s$，行走 $k$ 次（$1\le k\le p$），则它覆盖了时间段 $(b,b+k]$：

- 第 $1$ 步（全局时刻 $b+1$）：在道路 $s$ 上，收集 $a_{s,\,b+1}$，到达工厂 $s+1$。
- 第 $j$ 步（全局时刻 $b+j$）：在道路 $(s+j-1)\bmod n$ 上，收集 $a_{(s+j-1)\bmod n,\,b+j}$，到达工厂 $s+j$。
- 走完 $k$ 步后消失，此时全局时刻为 $b+k$。

因此

$$
dp[t]=\max_{
\substack{1\le k\le p\\0\le s<n}}
\Bigl\{\,dp[t-k]+\sum_{j=1}^{k} a_{(s+j-1)\bmod n,\;t-k+j}\;-\;c_s\Bigr\}
$$

直接枚举 $k,s$ 是 $O(m\,p\,n)$，$1000^3=10^9$ 不可接受。

### 2.2 对角线的关键观察

考察一个机器人单步收集的金币 $(i,t)$，其行列满足：

$$
i\equiv (s+j-1)\pmod n,\qquad t=b+j
$$

两式相减（模 $n$）：

$$
i-t\equiv (s+j-1)-(b+j)\equiv s-b-1\pmod n
$$

记 $d\equiv i-t\pmod n$，则对于同一个机器人，（道路编号 $-$ 时刻）$\bmod n$ 是**常数**，等于 $d\equiv s-b-1\pmod n$。

**引理 1**：一个机器人收集的所有金币位于一条固定的"对角线" $d=(i-t)\bmod n$ 上。不同 $d$ 之间互不干扰。

**证明**：由上述推导，机器人每一步的 $(i-t)\bmod n$ 均为 $s-b-1$，与步数 $j$ 无关。□

据此定义对角线上的价值函数：

$$
g_{d,t}=a_{(t+d)\bmod n,\;t}
$$

它表示时刻 $t$ 在第 $d$ 条对角线上出现的金币数量（即该时刻位于该对角线的唯一道路上的金币）。于是机器人在时间段 $(b,b+k]$ 内收集的总金币为前缀和形式：

$$
\sum_{j=1}^{k} g_{d,\;b+j}=pref_{d,\,b+k}-pref_{d,\,b},\qquad
pref_{d,\,x}=\sum_{\tau=1}^{x} g_{d,\tau}
$$

### 2.3 DP 重写

将 $k=t-b$ 代入，$b=t-k$。对角线 $d$ 由起始工厂 $s$ 与起始时刻 $b$ 决定：

$$
d\equiv s-b-1\pmod n\;\Longrightarrow\; s\equiv b+d+1\pmod n
$$

于是：

$$
\begin{aligned}
dp[t]&=\max_{\substack{1\le k\le p\\0\le d<n}}
\Bigl\{\,dp[t-k]+pref_{d,\,t}-pref_{d,\,t-k}-c_{(t-k+d+1)\bmod n}\Bigr\} \\[4pt]
&=\max_{0\le d<n}\Bigl\{\,pref_{d,\,t}+\max_{1\le k\le p}
\underbrace{\bigl(dp[t-k]-pref_{d,\,t-k}-c_{(t-k+d+1)\bmod n}\bigr)}_{ \text{记为 }val_{d,\,t-k}}\Bigr\}
\end{aligned}
$$

**核心观察**：内层 $\max$ 是关于 $k$ 的滑动窗口——对每个固定的 $d$，我们只需维护 $val_{d,\,\tau}$ 在长度为 $p$ 的窗口 $[t-p,\;t-1]$ 上的最大值。

### 2.4 单调队列优化

对每条对角线 $d$ 维护一个单调递减队列 $Q_d$，存储索引 $\tau$（即时刻），按 $val_{d,\,\tau}$ 从大到小排列。每步 $t=1\to m$ 执行：

1. **更新本时刻的对角线前缀和**：
   $$pref_{d,\,t}=pref_{d,\,t-1}+g_{d,\,t}\quad(\forall d)$$

2. **计算 $dp[t]$**：对每个 $d$，弹出 $Q_d$ 中 $<t-p$ 的过期索引，取队首索引 $best$，则
   $$dp[t]=\max_d\bigl\{pref_{d,\,t}+val_{d,\,best}\bigr\}$$

3. **准备未来转移**：对每个 $d$，计算
   $$val_{d,\,t}=dp[t]-pref_{d,\,t}-c_{(t+d+1)\bmod n}$$
   然后从 $Q_d$ 尾部弹出所有 $val\le val_{d,\,t}$ 的索引，再将 $t$ 压入队尾。

答案即为 $dp[m]$。

**引理 2**（单调队列正确性）：队列 $Q_d$ 始终维护窗口 $[t-p,t-1]$ 内 $val_{d,\tau}$ 的单调递减顺序，队首即为窗口最大值。

**证明**：初始时队列为空（或仅有 $0$）。每次新元素 $t$ 插入前，先移除过期元素（队首条件），再维护单调性（队尾弹出所有 $\le$ 当前值的元素），保证了队列的单调递减性。由于窗口索引严格递增，过期元素从队首弹出不影响正确性。□

### 2.5 复杂度

- 时间：$O(nm)$——内外各一重循环，单调队列摊还 $O(1)$。
- 空间：$O(nm)$ 存储 $val$ 和 $pref$（或滚动 $O(n)$ 的前缀和与 $O(nm)$ 的 $val$ 数组）。

$n,m\le 1000$ 下约 $10^6$ 次操作，轻松通过。

---

## 三、模式总结

- **DP 套滑动窗口最大值**：$dp[t]=\max(c_1+dp[t-k]+c_2)$ 型转移，若内层 $\max$ 只依赖 $t-k$ 且步长连续，常可提出 $c_1$ 后对每个平行维度维护单调队列。
- **对角线压缩**：环形坐标偏移 $(i-t)\bmod n$ 将二维时空矩阵压缩为 $n$ 条独立的对角线，使机器人行走路径退化为单条对角线上的连续段。
- **状态重参数化**：将 $(k,s)$ 枚举转化为 $(d,b)$，利用 $d$ 不变性降低维度。

**思考流程**：朴素 $O(mpn)$ → 观察 $(i-t)\bmod n$ 不变 → 定义为对角线 → 前缀和加速路径求和 → 提出 $pref_{d,t}$ → 对每个 $d$ 独立滑窗 → 单调队列。

---

## 四、最终解法

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 工厂数（道路数） |
| $m$ | 总时刻数 |
| $p$ | 机器人最大行走次数 |
| $a_{i,t}$ | 道路 $i$ 在时刻 $t$ 的金币数（$0\le i<n,\;1\le t\le m$） |
| $c_i$ | 工厂 $i$ 购买机器人的花费 |
| $g_{d,t}=a_{(t+d)\bmod n,\,t}$ | 对角线 $d$ 在时刻 $t$ 的金币 |
| $pref_{d,\,t}=\sum_{\tau=1}^t g_{d,\tau}$ | 对角线前缀和 |
| $dp[t]$ | $t$ 时刻后的最大净收益 |
| $val_{d,\,t}=dp[t]-pref_{d,\,t}-c_{(t+d+1)\bmod n}$ | 用于单调队列的比较值 |
| $Q_d$ | 对角线 $d$ 的单调队列（存时刻索引） |

### 算法步骤

1. 读入 $n,m,p$，读入 $a_{i,t}$（$n\times m$ 矩阵），读入 $c_i$。
2. 初始化 $dp[0]=0$，$dp[1..m]=-\infty$。
3. 对每条对角线 $d\in[0,n-1]$：计算 $val_{d,0}=-c_{(d+1)\bmod n}$，将 $0$ 压入 $Q_d$。
4. 对 $t=1..m$：
   a. 对每条 $d$：更新 $pref_{d}+=g_{d,t}=a_{(t+d)\bmod n,\,t}$。
   b. 对每条 $d$：弹出 $Q_d$ 队首过期索引（$<t-p$）；若队列非空，令 $best=Q_d$ 队首，用 $pref_d+val_{d,best}$ 更新 $dp[t]$ 的最大值。
   c. 对每条 $d$：计算 $val_{d,t}=dp[t]-pref_d-c_{(t+d+1)\bmod n}$；从 $Q_d$ 尾部弹出所有 $val\le val_{d,t}$ 的索引，将 $t$ 压入队尾。
5. 输出 $dp[m]$。

### 复杂度

- 时间 $O(nm)$，空间 $O(nm)$（$val$ 数组 $n\times(m+1)$）。

### 正确性

DP 转移覆盖了所有可能的最后一个机器人选取方案（任意 $k$、任意起始工厂），单调队列保证了滑动窗口最大值查询的 $O(1)$ 均摊代价。由引理 1、2 知算法正确。

---

## 五、参考代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

int n,m,p;
int a[mxn][mxn];	// gold[road][time], 0-indexed road
int cst[mxn];		// cost[factory], 0-indexed

i64 pref_cur[mxn];	// pref[d] for current time t
i64 val[mxn][mxn];	// val[d][t]
i64 dp[mxn];		// dp[t]
deque<int> dq[mxn];	// monotonic queues

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>p;
	for(int i=0;i<n;++i)
		for(int j=1;j<=m;++j)
			cin>>a[i][j];
	for(int i=0;i<n;++i)
		cin>>cst[i];

	fill_n(dp+1,m,-1145141919);
	for(int d=0;d<n;++d)
	{
		val[d][0]=-cst[(d+1)%n];
		dq[d].push_back(0);
	}

	for(int t=1;t<=m;++t)
	{
		// update prefix sums
		for(int d=0;d<n;++d)
			pref_cur[d]+=a[(t+d)%n][t];

		// compute dp[t]
		dp[t]=-1e18;
		for(int d=0;d<n;++d)
		{
			while(!dq[d].empty()&&dq[d].front()<t-p)
				dq[d].pop_front();
			if(!dq[d].empty())
			{
				int best=dq[d].front();
				if(pref_cur[d]+val[d][best]>dp[t])
					dp[t]=pref_cur[d]+val[d][best];
			}
		}

		// update for future
		for(int d=0;d<n;++d)
		{
			val[d][t]=dp[t]-pref_cur[d]-cst[(t+d+1)%n];
			while(!dq[d].empty()&&val[d][dq[d].back()]<=val[d][t])
				dq[d].pop_back();
			dq[d].push_back(t);
		}
	}

	cout<<dp[m]<<'\n';
	return 0;
}
```
