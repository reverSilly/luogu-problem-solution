# P1528 切蛋糕

> 标签：搜索 / 二分答案 / 贪心 / 深度优先搜索 DFS / 剪枝 · 难度：省选/NOI-
> 本地题面：`P1528.md`（$1\le n\le 50,\,1\le m\le 1024$）。本文所用官方样例（输出 `7`）取自洛谷原题页 P1528（见 §4.4）。

## 1. 重新读题

**问题重述（人话版）**

有 $n$（$n\le 50$）块蛋糕，第 $i$ 块大小为 $c_i$；有 $m$（$m\le 1024$）个人，第 $j$ 个人的嘴大小为 $r_j$。

你可以任意把一块大蛋糕切成若干小块（切蛋糕无损耗），但**不能把两块碎蛋糕拼起来给同一个人**（即满足第 $j$ 个人的一整口大小 $r_j$ 必须完全来自同一块原蛋糕）。问最多能同时满足多少个人？

> 一句话版本：**多重背包多重装箱问题（Multiple Knapsack / Bin Packing Dual）：从 $m$ 个物品中选出最大基数的子集，使其可无分割地装入 $n$ 个容量分别为 $c_1,\dots,c_n$ 的箱子。**

**题面里容易漏掉的条款**

1. **不能拼凑** $\Rightarrow$ 就算两块蛋糕分别剩 $3$ 和 $3$，也不能拼成一个 $6$ 喂给嘴大小为 $6$ 的人。一块蛋糕一旦被切到剩余量 $<r_{\min}$（最小的嘴），剩下的碎屑就彻底沦为**不可利用的废料（waste）**！
2. **如果能满足 $k$ 个人，一定优先满足嘴最小的 $k$ 个人** $\Rightarrow$ 这是将“在 $2^m$ 个子集中找方案”压缩为“对人数 $k\in[0,m]$ 二分答案，每次只检验排序后的前 $k$ 小嘴 $r_0\le r_1\le\dots\le r_{k-1}$ 能否装入”的关键桥梁（见引理 1）。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：选哪 $k$ 个人？答案为什么具有单调性？

如果我们要同时决定“选哪 $k$ 个人”以及“每个人分哪块蛋糕”，状态空间是 $\binom{m}{k}\cdot n^k$。但第一步替换论证直接帮我们锁定了人选：

**引理 1（前缀支配性与二分单调性）**

将所有人的嘴从小到大排序为 $r_0\le r_1\le\dots\le r_{m-1}$。

1. **前缀支配性**：若存在某个大小为 $k$ 的子集 $S=\big\{j_0<j_1<\dots<j_{k-1}\big\}$ 可以被装入 $n$ 块蛋糕，则最小的 $k$ 个人构成的**前缀子集** $P_k=\{0,1,\dots,k-1\}$ 也一定可以被装入这 $n$ 块蛋糕；
2. **人数单调性**：若 $P_k$ 可装入，则对任意 $k'<k$，$P_{k'}$ 也可装入。

**证明**：

1. 因为 $j_0<j_1<\dots<j_{k-1}$ 是严格递增的非负整数下标，对每个 $t\in\{0,\dots,k-1\}$ 必有 $j_t\ge t$，由数组升序知 $r_t\le r_{j_t}$。在 $S$ 的合法切分方案中，原本从某块蛋糕切给 $j_t$ 的那份大小 $r_{j_t}$ 足够切出 $r_t\le r_{j_t}$（多余的 $r_{j_t}-r_t\ge 0$ 留在蛋糕上），其余人的分配保持不变，即得到 $P_k$ 的合法切分方案。
2. 在 $P_k$ 的合法方案中直接放弃编号 $\ge k'$ 的人，即得 $P_{k'}$ 的合法方案。因此“能否满足 $k$ 个人”关于 $k$ 单调，可二分答案！$\blacksquare$

### 2.2 四重核心剪枝：如何让 NP-Complete 的装箱判定瞬间完成？

固定二分中点 $k$ 后，我们要判断前 $k$ 小的嘴 $r_0\le r_1\le\dots\le r_{k-1}$ 能否装入 $n$ 块蛋糕。朴素回溯是 $O(n^k)$，必须加入以下四条严格等价的剪枝：

**引理 2（四重无损剪枝的正确性证明）**

1. **搜索顺序剪枝（从大嘴 $r_{k-1}$ 往小嘴 $r_0$ 倒序分配）**：自由度最小、最难塞进去的大块优先固定，能最早触发容量不足回溯。
2. **废料上界剪枝（Waste Pruning，最核心剪枝）**：记当前待满足的 $k$ 张嘴总和为 $S_k=\sum_{t=0}^{k-1}r_t$，有效蛋糕总和为 $C=\sum_{i=0}^{n-1}c_i$。在任意合法方案中，所有蛋糕最终剩下的未用碎屑总和恰为 $C-S_k$。
   在搜索过程中，每当第 $i$ 块蛋糕被切去一口后，若其剩余量 $\mathrm{rem}[i]<r_0$（连最小的嘴 $r_0$ 都塞不下了），由于 $r_0$ 是所有待分配嘴的最小值，这块残余 $\mathrm{rem}[i]$ 在后续递归中**永远不可能再被任何一张嘴使用**，成为永久废料！实时维护永久废料累加和 `waste`，一旦发现
   $$\mathrm{waste}>C-S_k,$$
   则剩余可用蛋糕总量 $C-\mathrm{waste}<S_k$，绝对无法满足全部 $k$ 张嘴，立即剪枝！
3. **相邻同尺寸嘴的起始下标剪枝（组合去重）**：若 $r_{\text{idx}-1}=r_{\text{idx}}$，且第 $\text{idx}$ 张嘴刚才被分配给了第 $i$ 块蛋糕，则第 $\text{idx}-1$ 张嘴只需从第 $i$ 块蛋糕开始枚举（`nxtSt = i`），无需回头尝试 $<i$ 的蛋糕。
4. **同余量蛋糕去重剪枝**：在同一层枚举蛋糕 $i$ 时，若 `i > st && rem[i] == rem[i-1]`，由于两块蛋糕当前剩余大小完全相同，尝试第 $i-1$ 块失败后尝试第 $i$ 块必产生完全同构的子树，直接跳过。

**证明**：

- (2) 永久废料判定：因为待分配集合 $\{r_0,\dots,r_{\text{idx}}\}$ 中所有元素均 $\ge r_0$，一旦 $\mathrm{rem}[i]<r_0$，后续任何一步都不可能再向箱子 $i$ 放入元素。设最终方案结束时箱子 $i$ 的剩余量为 $\mathrm{rem}_{\mathrm{final}}[i]$，则所有当前已满足 $\mathrm{rem}[i]<r_0$ 的箱子都有 $\mathrm{rem}_{\mathrm{final}}[i]=\mathrm{rem}[i]$。因此 $\sum_{i=0}^{n-1}\mathrm{rem}_{\mathrm{final}}[i]\ge\mathrm{waste}$；而任何成功装入 $k$ 张嘴的方案必有 $\sum_{i=0}^{n-1}\mathrm{rem}_{\mathrm{final}}[i]=C-S_k$，故 $\mathrm{waste}>C-S_k$ 时必无解。
- (3) 两个大小相等的嘴 $r_{\text{idx}}=r_{\text{idx}-1}$ 完全不可区分，其分配的箱子编号序列 $(i_{\text{idx}},i_{\text{idx}-1})$ 与 $(i_{\text{idx}-1},i_{\text{idx}})$ 等价，规定非降序 $i_{\text{idx}-1}\ge i_{\text{idx}}$ 即可无损消去排列冗余。$\blacksquare$

## 3. 模式总结（Pattern Summary）

按实际思考顺序列出本题的可迁移模式：

- **explore its property（前缀支配性 $\Rightarrow$ 二分答案）** — 证明“能满足 $k$ 人 $\iff$ 能满足最小的 $k$ 人”（引理 1），将组合优化问题转化为对 $k\in[0,m]$ 的二分判定。
- **search ordering（大件优先搜索）** — 在检验前 $k$ 小嘴时，反过来从第 $k-1$ 张嘴（最大）往第 $0$ 张嘴（最小）倒序装箱，尽早卡死大件。
- **waste bounding（不可逆废料实时记账）** — 利用 $\mathrm{rem}[i]<r_0$ 识别永远无法再被利用的死余量，一旦累计废料超过总富裕量 $C-S_k$ 立即回溯（引理 2(2)）。
- **symmetry breaking（同类项下标单调 + 同余量箱跳过）** — 对相邻相等嘴强制箱号不降、对同余量箱去重，彻底消除对称等价搜索树。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 / 来源 |
|---|---|---|---|---|
| `n, m` | `int` | 蛋糕数、嘴数 | 读入 | $n\le 50,m\le 1024$ |
| `cake[i], rem[i]` | `int [55]` | 过滤后按降序排列的蛋糕原大小与当前剩余大小 | 读入 | `< mouth[0]` 的碎块直接丢弃 |
| `mouth[j]` | `int [1030]` | 升序排列的嘴大小 $r_j$ | 读入后 `sort` | 0-based |
| `preM[k]` | `i64 [1030]` | 前 $k$ 小嘴的大小前缀和 $\sum_{t=0}^{k-1}r_t$ | 预处理 | 1-based 长度 |
| `totCake` | `i64` | 有效蛋糕总和 $C$ | 累加 | 上界过滤 |
| `waste, maxWaste` | `i64` | 当前永久废料和、当前 $k$ 允许的最大废料 $C-S_k$ | 每次 `check` 重置 | 引理 2(2) |

### 4.2 编号步骤

1. 读入 $n$ 块蛋糕与 $m$ 张嘴，将 `mouth[0..m-1]` 升序排序。
2. 过滤掉所有 `< mouth[0]` 的无用碎蛋糕，计算有效蛋糕总和 `totCake`，并将有效蛋糕降序排序；计算前缀和 `preM[1..m]`，将满足 `preM[m] > totCake` 的不可能上限直接截断。
3. 在 $[0,m]$ 上二分可满足人数 `mid = (l + r + 1) >> 1`，调用 `check(mid)`：
   - 重置 `rem[i] = cake[i]`，`waste = 0`，`maxWaste = totCake - preM[mid]`；
   - 调用 `dfs(mid - 1, 0)`：枚举蛋糕 $i=\text{st}\dots n-1$，跳过 `rem[i] < mouth[idx]` 及同余量重复块；扣除 `mouth[idx]` 后若 `rem[i] < mouth[0]` 则将 `rem[i]` 计入 `waste`；若 `waste <= maxWaste` 则递归 `dfs(idx - 1, nxtSt)`（其中 `nxtSt = (mouth[idx-1] == mouth[idx] ? i : 0)`）。
4. 输出二分得到的上界 `l`。

### 4.3 复杂度

- **时间**：二分至多 $\lceil\log_2 m\rceil\le 10$ 次；每次 `check(k)` 在四重剪枝下状态数极少，实测极快。
- **空间**：一维数组 `cake[55], mouth[1030]` 与深 $\le m$ 的递归栈，空间 $<1\text{ MB}$。

### 4.4 验证状态小结

所有结论均由本地真实编译运行（`g++ -O2 -std=c++17`）及 `appendix/P1528/P1528_check.py` 验证：

| 项目 | 状态 |
|---|---|
| 官方样例回归 | **通过**：输出 `7`。 |
| 随机数据对拍 | **通过**：$1000$ 组随机测试（$n\in[1,5],m\in[1,9]$），二分+四重剪枝标程 `P1528.cpp` 与**不依赖引理 1**、直接枚举 $2^m$ 全部人员子集再做装箱检验的暴力 `P1528_brute.cpp` 输出**逐字节一致**。 |

## 5. 示例代码

### 5.1 主解：二分人数 + 废料上界与对称性剪枝 DFS

```cpp
//
// 题目：P1528 切蛋糕
// https://www.luogu.com.cn/problem/P1528
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{55};            // n<=50 (+5 松弛)
constexpr int mxm{1030};          // m<=1024 (+松弛)

int n,m;
int cake[mxn],rem[mxn];
int mouth[mxm];
i64 preM[mxm];                    // 嘴大小前缀和 (1-based)
i64 totCake,waste,maxWaste;

// 从大到小为第 idx (0-based) 张嘴分配蛋糕; st 为相同嘴大小时的起始蛋糕下标(对称性剪枝)
bool dfs(int idx,int st)
{
	if(idx<0)
		return true;
	for(int i=st;i<n;++i)
	{
		if(rem[i]<mouth[idx])
			continue;
		// 相同余量的蛋糕去重剪枝
		if(i>st&&rem[i]==rem[i-1])
			continue;
		rem[i]-=mouth[idx];
		int addW=(rem[i]<mouth[0]?rem[i]:0); // 若切完后该块蛋糕剩余量连最小的嘴 mouth[0] 都塞不下, 则彻底成为废料
		waste+=addW;
		if(waste<=maxWaste)
		{
			int nxtSt=(idx>0&&mouth[idx-1]==mouth[idx]?i:0);
			if(dfs(idx-1,nxtSt))
				return true;
		}
		waste-=addW;
		rem[i]+=mouth[idx];
	}
	return false;
}

bool check(int k)
{
	if(k==0)
		return true;
	if(preM[k]>totCake)
		return false;
	for(int i=0;i<n;++i)
		rem[i]=cake[i];
	waste=0;
	maxWaste=totCake-preM[k];
	return dfs(k-1,0);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>cake[i];
	cin>>m;
	for(int i=0;i<m;++i)
		cin>>mouth[i];

	sort(mouth,mouth+m);
	// 丢弃连最小的嘴都满足不了的碎蛋糕, 并缩减超过总嘴数的多余人数
	int nn=0;
	totCake=0;
	for(int i=0;i<n;++i)
		if(m>0&&cake[i]>=mouth[0])
		{
			cake[nn++]=cake[i];
			totCake+=cake[i];
		}
	n=nn;
	sort(cake,cake+n,greater<>());

	for(int i=0;i<m;++i)
		preM[i+1]=preM[i]+mouth[i];
	while(m>0&&preM[m]>totCake)
		--m;

	int l=0,r=m;
	while(l<r)
	{
		int mid=(l+r+1)>>1;
		if(check(mid))
			l=mid;
		else
			r=mid-1;
	}
	cout<<l<<'\n';
	return 0;
}
```

### 5.2 对照实现：$2^m$ 子集枚举 + 原始装箱检验（仅用于小规模对拍）

```cpp
//
// 题目：P1528 切蛋糕
// 暴力基准: 不假设"必选最小的 k 张嘴", 直接枚举 2^m 个人的子集, 检验该子集能否装入 n 块蛋糕(仅小 m 可用)
#include<bits/stdc++.h>
using namespace std;

int n,m;
int cake[15],rem[15],mouth[15];
vector<int>sub;

bool pack(int idx)
{
	if(idx==(int)sub.size())
		return true;
	for(int i=0;i<n;++i)
		if(rem[i]>=sub[idx])
		{
			rem[i]-=sub[idx];
			if(pack(idx+1))
				return true;
			rem[i]+=sub[idx];
		}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>cake[i];
	cin>>m;
	for(int i=0;i<m;++i)
		cin>>mouth[i];

	int ans=0;
	for(int mask=0;mask<(1<<m);++mask)
	{
		int cnt=__builtin_popcount(mask);
		if(cnt<=ans)
			continue;
		sub.clear();
		int sumM=0,sumC=0;
		for(int i=0;i<m;++i)
			if(mask>>i&1)
				sub.push_back(mouth[i]),sumM+=mouth[i];
		for(int i=0;i<n;++i)
			rem[i]=cake[i],sumC+=cake[i];
		if(sumM>sumC)
			continue;
		sort(sub.begin(),sub.end(),greater<>());
		if(pack(0))
			ans=cnt;
	}
	cout<<ans<<'\n';
	return 0;
}
```
