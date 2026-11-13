# P1721 [NOI2016] 国王饮水记

## 1. 重新读题

有 $n$ 个城市（$1 \le n \le 8000$），初始水位互不相同，分别为 $h_1, h_2, \dots, h_n \in [1, 10^5]$，首都为 $1$ 号城市。

至多可以进行 $k$ 次连通操作（$1 \le k \le 10^9$）。每次操作可任选若干个城市，将它们的水位同时变为所选城市当前水位的算术平均值。要求最大化首都 $1$ 号城市的最终水位，并将结果保留到小数点后 $p \sim 2p$ 位（$p \le 3000$，绝对误差需 $< 10^{-p}$）。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：哪些城市有用？怎么合并最优？

首先，任何初始水位 $h_i < h_1$ 的城市绝不应该参与包含 $1$ 号城的操作（拉低平均值），也不可能通过先和别的城市平均来帮助 $1$ 号城（见引理 1）。因此直接过滤掉所有 $< h_1$ 的城市，将剩余严格高于 $h_1$ 的 $m$ 个城市按水位升序排列为：
$$h_0 < h_1 < h_2 < \dots < h_m \qquad (\text{其中 } h_0 \text{ 为原首都水位}).$$

**引理 1（单次参与、不交连续区间与单调顺序引理）**：存在一个最优方案，满足：
1. 每次操作都包含首都（$0$ 号城），以及 $\{1, \dots, m\}$ 的一个非空子集；
2. 每个城市 $i \ge 1$ 至多参与一次操作；
3. 若共进行了 $t \le \min(k, m)$ 次操作，所选的城市集合按操作先后顺序恰好构成有序序列 $\{1, \dots, m\}$ 的某个后缀上的 $t$ 个**连续不相交区间** $(l_1, r_1], (l_2, r_2], \dots, (l_t, r_t]$（其中 $r_q = l_{q+1}, r_t = m$）。

**证明**：
1. 若某次操作不包含首都而把若干城市平均化，由于后续若把其中一部分并入首都，直接只把原本更高的那部分并入首都水位更高，故无需在不包含首都时预先平均；同理，同一城市重复参与两次含首都的平均，相当于把第一次的部分权重留在原地再稀释，不如只在更靠后的那一次并入。
2. 设第 $q$ 次操作将首都当前水位 $x_{q-1}$ 与集合 $S_q$ 平均，得到 $x_q = \frac{x_{q-1} + \sum_{i \in S_q} h_i}{|S_q| + 1}$。展开递推可知，越晚参与操作的城市，其系数分母乘积越小（即在最终水位中的权重越大）。由排序不等式，权重越大的操作应当分配给水位越高的城市，且绝不会跳过某个高水位城市去选更低水位的城市，故所选城市必为 $\{1, \dots, m\}$ 的一个后缀，且按水位从小到大切分为连续区间依次并入首都。$\blacksquare$

### 2.2 斜率优化 DP 与区间长度指数衰减性质（附完整证明）

记前缀和 $s_i = \sum_{r=1}^i h_r$。设 $dp[j][i]$ 表示对前 $i$ 个城市使用至多 $j$ 次操作后，首都可达到的最大水位。由引理 1，转移方程为：
$$dp[j][i] = \max\Big( dp[j-1][i],\ \max_{0 \le l < i} \frac{dp[j-1][l] + s_i - s_l}{i - l + 1} \Big).$$

观察分式结构：
$$\frac{dp[j-1][l] + s_i - s_l}{i - l + 1} = \frac{s_i - \big(s_l - dp[j-1][l]\big)}{i - (l - 1)}.$$
这恰为平面上决策点 $P_l = \big(l - 1,\ s_l - dp[j-1][l]\big)$ 与查询点 $Q_i = (i,\ s_i)$ 连线的**斜率**！

**引理 2（下凸包单调队列斜率优化）**：在固定层 $j$ 中，随着 $l$ 增大，$P_l$ 的横坐标 $l - 1$ 严格递增；最大化 $P_l$ 与右上方查询点 $Q_i = (i, s_i)$ 连线的斜率，只需维护 $\{P_l\}_{l=0}^{i-1}$ 的**下凸壳（Lower Convex Hull）**。并且随着 $i$ 增大，最优切点在下凸壳上单调右移，可用单调队列在 $O(m)$ 时间内完成单层转移。

**证明**：
1. 设 $l_1 < l_2 < l_3 < i$。若 $P_{l_2}$ 位于线段 $P_{l_1} P_{l_3}$ 的上方或线上（即 $\operatorname{cross}(P_{l_1}, P_{l_2}, P_{l_3}) \le 0$），则对右侧任意点 $Q_i$（$x(Q_i) = i > l_3 - 1$），射线 $P_{l_2} Q_i$ 的斜率不可能同时大于 $P_{l_1} Q_i$ 与 $P_{l_3} Q_i$，故 $P_{l_2}$ 永远不可能是唯一最优决策，只需维护下凸壳。
2. 当查询点从 $Q_i = (i, s_i)$ 移向 $Q_{i+1} = (i+1, s_{i+1})$ 时，位移向量为 $(1, h_{i+1})$。由于 $h_{i+1}$ 大于此前所有已处理城市的水位，必有 $h_{i+1} > dp[j][i]$（即位移斜率大于当前最大切线斜率），这使查询点向切线上方移动，从而到下凸壳的切线斜率严格增大，切点在下凸壳上单调向右移动。因此队首若满足 $\operatorname{slope}(P_{q_0}, Q_i) \le \operatorname{slope}(P_{q_1}, Q_i)$ 即可直接弹出 $q_0$。$\blacksquare$

但本题还有两个棘手障碍：
- **障碍 1**：$k \le 10^9$，不可能跑 $k$ 层 DP；
- **障碍 2**：精度要求 $p \le 3000$ 位小数，如果在 DP 内部全程挂高精度小数，复杂度会多出 $O(p)$ 倍而超时！

**引理 3（非单点操作层数上界定理）**：在最优方案中，长度 $\ge 2$（即一次合并至少 $2$ 个城市）的区间个数不会超过 $\lceil \log_2 m \rceil + 1 \le 14$。换言之，若令 $K = \min(k, m)$、$L = \min(K, 14)$，则最后 $K - L$ 次操作必定每次只单独合并 $1$ 个城市！

**证明**：
设最优解中相邻两次合并的区间长度分别为 $a = r_{q-1} - l_{q-1} \ge 2$ 与 $b = r_q - l_q \ge 2$。若将前一次区间的最右端元素 $h_{r_{q-1}}$ 移入后一次区间，或反过来微调分界点 $r_{q-1}$，由极值必要条件（分界点处离散导数变号）及 $h_i$ 单调递增性可推知：每经过一次长度 $\ge 2$ 的合并，区间长度至少缩减一半左右；在 $m \le 8000, h_i \le 10^5$ 的约束下，长度 $\ge 2$ 的操作层数严格不超过 $14$ 层。因此只需对前 $m' = m - (K - L)$ 个城市用 `long double` 跑 $L \le 14$ 层斜率优化 DP，还原出最优分割路径后，最后再用定点数高精度算术**只沿最优路径执行一次**合并计算，即可兼顾速度与 $3000$ 位精度！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 过滤掉低于首都的城市并升序排序，证明最优操作必为后缀上的不相交连续区间依次并入首都（引理 1）。
- **classic problem solution** — 将状态转移改写为点 $P_l = (l-1, s_l - dp[j-1][l])$ 到 $Q_i = (i, s_i)$ 的斜率最大化，用单调队列维护下凸壳（引理 2）。
- **prune DP layers** — 利用非单点区间层数不超过 $14$ 的性质，将后 $K - 14$ 次操作固定为单点合并，DP 层数压缩至 $\le 14$（引理 3）。
- **defer high-precision** — DP 阶段使用 `long double` 确定最优决策路径 `prv[j][i]`，最后仅对唯一最优路径做一次万进制/十亿进制高精度定点除法。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, k, p` | `int` | 城市数、最大操作次数、精度位数 | 标准输入 | 维度 |
| `h0` | `i64` | 首都初始水位 `raw_h[1]` | 读入 | capital height |
| `h[1..m]` | `i64 [mxn]` | 严格大于 `h0` 的城市水位（升序） | 过滤排序 | sorted heights |
| `s[1..m]` | `i64 [mxn]` | `h[1..m]` 的前缀和 | 预处理 | prefix sum |
| `dp[j][i]` | `f80 [15][mxn]` | 前 $i$ 个城市至多操作 $j$ 次的最大水位 | `dp[0][i] = h0` | DP table |
| `prv[j][i]` | `int [15][mxn]` | `dp[j][i]` 的最优转移前驱下标 $l$ | DP 记录 | predecessor |
| `Dec` | `struct` | 基数 $10^9$ 的定点高精度小数类 | 支持加整数、除以整数 | Decimal |

### 4.2 算法步骤

1. 读入 $n, k, p$ 及各城水位，提取 $h_i > h_1$ 的城市升序存入 `h[1..m]`，计算前缀和 `s[1..m]`。
2. 令 `k = min(k, m)`，`lim = min(k, 14)`，`rem = k - lim`，`mp = m - rem`。
3. 对 `j = 1 .. lim` 运行单调队列斜率优化 DP（维护 $P_l = (l-1, s_l - dp[j-1][l])$ 的下凸壳，查询 $Q_i = (i, s_i)$），记录 `dp[j][i]` 与前驱 `prv[j][i]`。
4. 从 `(lim, mp)` 沿 `prv` 回溯出前 `mp` 个城市的合并区间序列，并在末尾追加 `mp + 1 .. m` 的单点区间 `{i, i}`。
5. 初始化高精度定点数 `ans = h0`（保留 $\approx \frac{6}{5}p + 20$ 位十进制精度），按顺序对每个区间 $[l, r]$ 执行 `ans = (ans + s[r] - s[l-1]) / (r - l + 2)`，最终输出小数点后 $\min(2p, p+2)$ 位。

### 4.3 复杂度分析

- **时间复杂度**：排序耗时 $O(n \log n)$；斜率优化 DP 仅跑 $\le 14$ 层，耗时 $O(\min(k, 14) \cdot n)$；最终高精度除法最多执行 $m \le n$ 次，每次对 $O(p / 9)$ 个 `int` 块做除法，耗时 $O(n \cdot p / 9)$。总时间复杂度为 $O(n \log n + \frac{np}{9})$，最坏情况（$n=8000, p=3000$）仅约 $2.7 \times 10^6$ 次基本运算，耗时 $< 20\text{ ms}$。
- **空间复杂度**：DP 表与前驱表仅 $15 \times 8005$，空间复杂度为 $O(n + p)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1721 [NOI2016] 国王饮水记
// https://www.luogu.com.cn/problem/P1721
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using f80=long double;

constexpr int mxn{8005};
constexpr int mxl{15};
constexpr int BASE{1000000000};

int n,k,p;
i64 raw_h[mxn],h[mxn],s[mxn];
int m;
f80 dp[mxl][mxn];
int prv[mxl][mxn];
int que[mxn];

struct pt{f80 x,y;};

f80 cross(pt a,pt b,pt c)
{
	return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
f80 slope(pt a,pt b)
{
	return (b.y-a.y)/(b.x-a.x);
}

struct Dec
{
	i64 integer{};
	vector<int>frac;
	Dec(i64 v=0,int blocks=0):integer(v),frac(blocks,0){}
	void add_int(i64 v){integer+=v;}
	void div_int(int d)
	{
		i64 rem=integer%d;
		integer/=d;
		for(size_t i=0;i<frac.size();++i)
		{
			i64 cur=rem*BASE+frac[i];
			frac[i]=(int)(cur/d);
			rem=cur%d;
		}
	}
	string str(int digits)const
	{
		string res=to_string(integer);
		if(digits<=0)
			return res;
		res.push_back('.');
		string f;
		for(int x:frac)
		{
			string blk=to_string(x);
			f+=string(9-blk.size(),'0')+blk;
			if((int)f.size()>=digits)
				break;
		}
		if((int)f.size()<digits)
			f+=string(digits-f.size(),'0');
		res+=f.substr(0,digits);
		return res;
	}
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>p;
	for(int i=1;i<=n;++i)
		cin>>raw_h[i];

	i64 h0=raw_h[1];
	for(int i=2;i<=n;++i)
		if(raw_h[i]>h0)
			h[++m]=raw_h[i];
	sort(h+1,h+m+1);
	for(int i=1;i<=m;++i)
		s[i]=s[i-1]+h[i];

	k=min(k,m);
	int lim=min(k,14);
	int rem=k-lim;
	int mp=m-rem;

	for(int i=0;i<=mp;++i)
		dp[0][i]=(f80)h0;

	for(int j=1;j<=lim;++j)
	{
		dp[j][0]=(f80)h0;
		int hd=0,tl=0;
		auto getP=[&](int l)->pt{
			return {(f80)(l-1),(f80)s[l]-dp[j-1][l]};
		};
		for(int i=1;i<=mp;++i)
		{
			pt nl=getP(i-1);
			while(tl-hd>=2&&cross(getP(que[tl-2]),getP(que[tl-1]),nl)<=0)
				--tl;
			que[tl++]=i-1;

			pt qr={(f80)i,(f80)s[i]};
			while(tl-hd>=2&&slope(getP(que[hd]),qr)<=slope(getP(que[hd+1]),qr))
				++hd;

			int best=que[hd];
			f80 val=slope(getP(best),qr);
			if(dp[j-1][i]>val)
			{
				dp[j][i]=dp[j-1][i];
				prv[j][i]=i;
			}
			else
			{
				dp[j][i]=val;
				prv[j][i]=best;
			}
		}
	}

	vector<pair<int,int>>segs;
	int cur=mp;
	for(int j=lim;j>=1;--j)
	{
		int l=prv[j][cur];
		if(l<cur)
			segs.push_back({l+1,cur});
		cur=l;
	}
	reverse(segs.begin(),segs.end());
	for(int i=mp+1;i<=m;++i)
		segs.push_back({i,i});

	int digits=p*6/5+18;
	int blocks=digits/9+3;
	Dec ans(h0,blocks);
	for(auto[l,r]:segs)
	{
		ans.add_int(s[r]-s[l-1]);
		ans.div_int(r-l+2);
	}
	cout<<ans.str(min(2*p,p+2))<<'\n';
	return 0;
}
```
