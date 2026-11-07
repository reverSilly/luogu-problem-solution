# P1933 [NOI2010] 旅行路线

## 1. 重新读题

### 1.1 人话重述

给出一个 $n\times m$ 的 $01$ 矩阵 $T$（$n\le3$，$m\le50$，记 $N=nm$），以及一个长度为 $N$ 的 $01$ 串 $L$。求满足下列条件的**有向哈密顿路径**条数，对 $11192869$ 取模：

1. 从某个格子出发，每步走到上下左右相邻且未访问过的格子，恰好把 $N$ 个格子各访问一次（因此一共 $N-1$ 条路径边）；
2. 起点必须位于矩阵边界（第一行 / 最后一行 / 第一列 / 最后一列）；
3. 若第 $i$ 个被访问的格子是 $(x,y)$，则要求 $T_{x,y}=L_i$。

两条路线只要格子访问顺序不同就算不同（反向走是另一条路线）。

### 1.2 关键约束与规模暗示

- $n\le3$：矩阵非常“窄”。沿列扫描时，轮廓线上最多只有 $3$ 个横向插头加 $1$ 个纵向插头，共 $4$ 个，这是轮廓线 DP 能做的根本原因。
- $m\le50$，$N\le150$：状态里可以带 $O(N)$、$O(N^2)$ 级别的附加信息。
- 计数要取模，模数 $11192869$。
- 颜色序列 $L$ 把“路径的访问顺序”和“每个格子的颜色”绑定在一起，这是本题与普通插头 DP 最大的不同：我们需要在状态中记录**路径顶点在 $L$ 中的位置**。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先看一条路径的“度数—下标”结构

把一条合法路线按访问顺序编号：第 $i$ 个被访问的格子下标为 $i$（$0\le i<N$）。对路径的每条边，它连接的两个格子访问下标相差 $1$。

于是只看图论结构：

**引理 1（度数决定下标范围）**：设 $d(v)$ 为格子 $v$ 在路径中的度数（参与了几条路径边）。

- 路径的两个端点度数均为 $1$，它们正是访问下标 $0$ 与 $N-1$ 的格子；
- 其余格子度数均为 $2$，访问下标在 $[1,N-2]$ 中。

**证明**：一条哈密顿路径是有 $N$ 个顶点、$N-1$ 条边的连通无环图，恰有两个端点度数为 $1$，其余度数为 $2$。沿路径从起点走到终点，访问顺序就是 $0,1,\dots,N-1$，端点对应首尾。$\blacksquare$

这条引理把“颜色序列”变成了局部的约束：只要我们给每个格子确定它的访问下标，就能立刻检查 $T_{x,y}=L_{\text{index}}$；并且**度数为 $1$ 的格子只能取下标 $0$ 或 $N-1$，度数为 $2$ 的格子只能取 $[1,N-2]$**。

### 2.2 轮廓线扫描：把已处理部分切成若干条“链”

按**列优先**顺序扫描：外层 $c=0,1,\dots,m-1$，内层 $r=0,1,\dots,n-1$。处理到格子 $(r,c)$ 时，轮廓线把格子分成：

- 已处理：列 $<c$ 的全部格子，以及第 $c$ 列第 $0..r-1$ 行的格子；
- 未处理：第 $c$ 列第 $r..n-1$ 行的格子，以及列 $>c$ 的全部格子。

在已处理的格子里，已经确定了一部分路径边。由于每个格子度数不超过 $2$，已处理的边形成若干条互不相交的**链**（部分路径）。每条链的“断口”就是与未处理格子之间的路径边，称为**插头**（plug）。轮廓线上的插头数量：

- 每一行的横向插头至多 $1$ 个（要么是 $(i,c-1)\to(i,c)$，要么是 $(i,c)\to(i,c+1)$）；
- 当前行之前还会有一个纵向插头 $(r-1,c)\to(r,c)$。

因此至多 $n+1\le4$ 个插头——这就是“窄”带来的压缩。

**关键设计**：每条链的所有格子在最终路径中占据一段**连续的访问下标区间** $[lo,hi]$（因为链内部就是一段连续的子路径）。链的长度满足
$$
hi-lo+1=\text{链中格子数}.
$$
链的两个端点是“低端”（下标 $lo$ 一侧）与“高端”（下标 $hi$ 一侧）。若某个端点还是插头，则它连向的未处理格子的下标被唯一确定：

- 低端插头：未来格子的下标必为 $lo-1$；
- 高端插头：未来格子的下标必为 $hi+1$。

这就是状态需要携带的核心信息。

### 2.3 状态定义

在任意时刻，维护如下状态：

- `slot[0..n-1]`：每一行对应的横向插头，空或“属于某条链的某一端”；
- `slot[n]`：纵向插头（上一个格子向下连到当前格子），空或同上；
- 每条链记录区间 $[lo,hi]$；
- 两个布尔标记：下标 $0$ 的端点、下标 $N-1$ 的端点是否已经出现。

一个插头用二元组 $(\text{链编号},\text{端标记})$ 表示，端标记 $0$ 表示低端、$1$ 表示高端。由于每条链只有两个端点，同一条链最多有两个插头（一个低端一个高端）；没有插头的链说明两端都已封闭，只能在校验整条路径时出现。

**引理 2（状态充分性）**：未来能走出的所有局面只取决于插头的位置、每个插头的端标记、各链的区间 $[lo,hi]$，以及两个极值端点标记。

**证明**：下一次处理只与当前格子相邻的 $4$ 条边有关；其中“已处理方向”的边正是插头所描述的信息。扩展或合并链时，只需要知道链的区间和待连接端是低端还是高端（引理 3），不需要知道链内部的具体格子排布。两个极值端点标记决定了下标 $0$、$N-1$ 是否已被使用。$\blacksquare$

因此可以把拥有相同这些字段的状态合并，只保留方案数。

### 2.4 转移：数一数当前格子的入边

设当前格子 $(r,c)$，它的四条边：

- 左、上两条边若存在，一定通向“已处理”区域，对应两个插头（可能有 $0/1/2$ 个）；
- 右、下两条边若存在，一定通向“未处理”区域，可自行决定是否使用。

令 $k_p\in\{0,1,2\}$ 为左、上两条边中已使用的条数，再枚举 `right`、`down` 是否使用（注意不能越界）。当前格子的度数
$$
d=k_p+\text{right}+\text{down}
$$
必须是 $1$ 或 $2$。

#### 情形 A：$k_p=0$（左、上都没有边）

- $d=1$：当前格子是整条路径的一个端点，因此它的下标只能是 $0$ 或 $N-1$。
  - 若下标为 $0$：它必须是路线起点，检查它是否在矩阵边界；在状态中标记“下标 $0$ 已用”。
  - 若下标为 $N-1$：标记“下标 $N-1$ 已用”。
  
  新建一条长度 $1$ 的链，区间 $[e,e]$；唯一的那条未来边是它的“另一端”：$e=0$ 时未来下标为 $1$，是高端插头；$e=N-1$ 时未来下标为 $N-2$，是低端插头。
- $d=2$：两条边都通向未来。当前格子是某条链的中间点，下标 $e$ 可以取 $[1,N-2]$ 中任意值（受颜色约束）。新建链 $[e,e]$，两条未来边分别是低端插头（未来下标 $e-1$）与高端插头（未来下标 $e+1$）；把这两个端标记分配给 `right`/`down` 有 $2$ 种方式。

#### 情形 B：$k_p=1$（恰好一个已处理方向的插头）

设该插头属于链 $S$、端标记为 $t$。由 2.2，
$$
e=\begin{cases}lo_S-1,&t=\text{低端},\\ hi_S+1,&t=\text{高端}.\end{cases}
$$
先检查 $0\le e<N$ 且 $T_{r,c}=L_e$。

- $d=1$：当前格子成为该链的新端点（另一条边不存在）。由引理 1，$e$ 必须是 $0$ 或 $N-1$，并做好相应标记；把 $t$ 这一端“封闭”（不再有插头），区间扩展为包含 $e$。
- $d=2$：当前格子的另一条边通向未来。由引理 1，此时 $e\in[1,N-2]$。区间扩展为包含 $e$，并在**同一端** $t$ 生成新的插头（未来格子的下标继续是 $e-1$ 或 $e+1$）。

#### 情形 C：$k_p=2$（左、上都有边）

此时 $d=2$，不能再有右/下边。当前格子成为连接两条链的“桥”。

设左插头属于链 $A$、端标记 $t_A$，上插头属于链 $B$、端标记 $t_B$。按情形 B 的公式算出两侧要求的 $e_A,e_B$。

**引理 3（合并判据）**：这次合并能形成合法路径，当且仅当：

1. $e_A=e_B=e$，且 $0\le e<N$、$e\notin\{0,N-1\}$、$T_{r,c}=L_e$；
2. $A\ne B$（否则同一个链的两端被当前格子连上，形成环，不是路径）；
3. 合并后的区间
   $$
   [\,lo',hi'\,]=\bigl[\min(lo_A,lo_B),\ \max(hi_A,hi_B)\bigr]
   $$
   满足
   $$
   hi'-lo'+1=len(A)+len(B)+1,
   $$
   其中 $len(\cdot)=hi-lo+1$。

满足后，合并链的两个未消耗端保持不变（端标记也不变），长度按上式增加。

**证明**：

- 必要性：在一条路径中下标是连续递增 $1$ 的。链 $A$、当前格、链 $B$ 被拼成一段连续的路径，所以 $e$ 必须同时紧贴两段区间（即等于各自区间的端点外一格），这给出条件 1。若 $A=B$，拼起来首尾相接成环，而哈密顿路径无环，故条件 2 必要。
- 区间大小条件：三段（$A$ 的 $len(A)$ 个、当前格 $1$ 个、$B$ 的 $len(B)$ 个）总共 $len(A)+len(B)+1$ 个格子，它们的下标必须恰好填满一个长度相同的连续区间。若并集区间比这个长度小，说明两段下标有重叠（同一访问时刻出现两个格子）；若比它大，说明中间有空洞（某个访问时刻没有格子）。两者都不可能，故等式是必要条件。
- 充分性：等式成立说明三段区间的下标互不重叠且相邻无缝，拼成的链下标连续；条件 2 保证不闭合成环。于是得到一条更长的合法链。$\blacksquare$

由大小等式还能自动推出“一个消耗低端、另一个消耗高端”：若两端都消耗低端，则 $lo_A=lo_B$，并集大小为 $\max(hi_A,hi_B)-lo_A+1$，而所需大小为 $len(A)+len(B)+1$，两者之差为 $\min(hi_A,hi_B)-lo_A+2\ge2>0$，等式必不成立；两端都消耗高端同理。因此合并后“低端仍是原来某一端、高端仍是另一原来某一端”，端标记可以直接保留。

### 2.5 收尾：什么时候算走完

**引理 4（完成判据）**：当某条链的两个端点都不再是插头（即两端都封闭）时，它已经无法再与任何未处理格子相连。由于最终路线必须包含所有格子且连通，这条链必须已经是整条路线。这等价于：

- 它的区间是 $[0,N-1]$（于是链中有 $N$ 个格子，即全部格子）；
- 下标 $0$ 和 $N-1$ 的端点都已出现。

反过来，满足这两条的封闭链就是一条完整合法路线。

**证明**：封闭链与未处理格子之间没有任何路径边，若还存在其他格子（属于别的链或未处理），最终路径就无法连通。区间 $[0,N-1]$ 的长度为 $N$，链中格子数为 $hi-lo+1=N$，即包含全部 $N$ 个格子。两个极值端点标记保证恰有两个度 $1$ 格子且下标为 $0,N-1$。$\blacksquare$

当上述条件满足时，把当前状态的方案数累加到答案；注意是**累加当前状态的方案数**（同一个轮廓状态可能由多条不同的部分路径合并而来）。

### 2.6 与题面的一一对应

**引理 5（计数正确性）**：

- 任意一条合法有向路线，按扫描顺序读它的边集与下标，会唯一确定一条从初始状态到“完成”的转移序列，且所有检查（度数、颜色、下标范围、区间合并、端点标记）都会通过；
- 任意一条通过全部检查并完成的转移序列，反推给每个格子确定度数与下标，得到的是一张覆盖所有格子的单一路径，下标沿路径连续，颜色满足 $T=L$，且下标 $0$ 的端点（起点）在边界上；
- 二者互逆，因此答案恰为合法路线数。

**证明**：路线的边集给出每个格子的度数，引理 1 给出可能的下标范围；DP 在每一步都按同一规则确定下标与链，故转移序列唯一。反向地，每个格子度数为 $1$ 或 $2$、连边合法，所有链最终合并成一条封闭链（引理 4），说明得到的图连通、无环、恰有两个度 $1$ 顶点，是一条哈密顿路径；下标连续且覆盖 $[0,N-1]$，故第 $i$ 个访问格子的颜色正是 $L_i$。起点（下标 $0$）在边界由创建时的检查保证。$\blacksquare$

关于“方向”：一条无向哈密顿路径有两种走向（两个端点分别作为下标 $0$）。DP 对两种走向都会各自构造出一条转移序列；只有在对应端点位于边界且颜色序列匹配 $L$ 时才会被接受。因此有向路线被恰当地计数。

### 2.7 实现上的压缩

- 用 $8$ 位存 $lo,hi$（$+1$ 后存，$0$ 表示空链），$3$ 位存一个插头的（链编号，端标记），整个状态可以塞进一个 `uint64_t`，用哈希表转移。
- 链编号按“在插头中第一次出现的顺序”重新编号（canonicalization），避免同构状态被重复存储。
- $n=1$ 时 $N=1$，没有边，单独判掉。

### 2.8 复杂度

设轮廓状态数为 $S$。每个格子对每个状态做常数次转移（枚举 `right/down` 四种），因此

- 时间 $O(nm\cdot S)$；
- 空间 $O(S)$。

$n\le3$ 时轮廓至多 $4$ 个插头、至多 $3$ 条链；每条链用区间 $[lo,hi]$ 描述。最坏理论情况下 $S$ 可达 $O(N^4)$ 量级（两条“两端都开放”的自由链各贡献一个区间），但实际可达状态远小于该上界，$N\le150$ 时运行稳定。

---

## 3. 模式总结（Pattern Summary）

- **degree-index structure** — 路径端点度 $1$ 且下标为 $0,N-1$，其余度 $2$ 且下标在中间。
- **contour / plug DP** — 列优先扫描，轮廓线上至多 $n+1\le4$ 个插头。
- **strand intervals** — 每条部分路径是一段连续下标区间 $[lo,hi]$，插头标识低端 / 高端。
- **local extension** — 当前格子度数只能为 $1$ 或 $2$，按“已处理入边数 $k_p$”分三类转移。
- **merge criterion** — 两链合并需下标相邻且并集长度等于两链长度之和加一，否则有重叠或空洞。
- **completion test** — 封闭链必须是整条路径：区间 $[0,N-1]$ 且两端极值标记齐全。
- **state canonicalization** — 链编号按插头首次出现重排，再压成 `uint64_t` 哈希。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| $n,m$ | `int` | 行数、列数（$n\le3$） | 读入 | 规模 |
| $N$ | `int` | $nm$，路径长度 | 计算 | total |
| `T[r][c]` | `int[4][55]` | 格子颜色 $0/1$ | 读入 | terrain |
| `L[i]` | `int[205]` | 期望颜色序列 | 读入 | label |
| `lo[s],hi[s]` | `int[3]` | 第 $s$ 条链占据的访问下标区间；$-1$ 表示空 | $-1$ | interval |
| `op[s]` | `int[3]` | 第 $s$ 条链的低/高端是否仍有插头（bit0 低端，bit1 高端） | $0$ | open ends |
| `slot[i]` | `int[4]` | 第 $i$ 个插头：$0$ 为空，否则 `sid*2+end+1` | $0$ | plug |
| `flags` | `int` | bit0：下标 $0$ 端点已出现；bit1：下标 $N-1$ 端点已出现 | $0$ | extremes |
| `ways` | `int` | 到达某状态的方案数（模 $11192869$） | 初始状态为 $1$ | multiplicity |
| `g_ans` | `int` | 已完成路线的总数 | $0$ | answer |

### 4.2 算法步骤

1. 读入 $n,m$、矩阵 $T$、序列 $L$，令 $N=nm$。
2. 若 $N=1$，输出 $[T_{0,0}=L_0]$。
3. 把初始状态（无插头、无链、`flags=0`）压入哈希表，方案数为 $1$。
4. 按列优先顺序枚举每个格子 $(r,c)$，用新哈希表承接：
   - 取出 `slot[r]`（左插头）与 `slot[n]`（上插头）；
   - 枚举 `right∈{0,1}`、`down∈{0,1}`（不越界），令度数 $d=k_p+\text{right}+\text{down}$，要求 $d\in\{1,2\}$；
   - 按 $k_p=0/1/2$ 分别执行 2.4 中的转移，检查颜色、下标范围、端点标记与合并判据；
   - 若某条链封闭且区间为 $[0,N-1]$、`flags==3`，把当前 `ways` 累加到答案，不再把该状态放回表；
   - 其余新状态做链编号重排（canon）后累加方案数。
5. 全部格子处理完后输出 `g_ans mod 11192869`。

### 4.3 复杂度

- 时间 $O(nm\cdot S)$，空间 $O(S)$，$S$ 为轮廓状态数；$n\le3$ 保证每步只有常数种局部选择，$N\le150$ 时状态规模可控。

---

## 5. 示例代码

```cpp
//
// 题目：P1933 [NOI2010] 旅行路线
// https://www.luogu.com.cn/problem/P1933
#include<bits/stdc++.h>
using namespace std;

constexpr int MOD{11192869};
constexpr int mxm{55};
constexpr int mxN{205};

int n,m,N;
int T[4][mxm];
int L[mxN];

struct St
{
	int lo[3],hi[3],op[3];
	int slot[4];
	int flags;
};

int g_ans{0};

bool isBound(int r,int c)
{
	return r==0||r==n-1||c==0||c==m-1;
}

uint64_t enc(const St&s)
{
	uint64_t k{0};
	for(int i=0;i<4;++i)
		k|=(uint64_t)(s.slot[i]&7)<<(3*i);
	k|=(uint64_t)(s.flags&3)<<12;
	for(int i=0;i<3;++i)
	{
		int l=(s.lo[i]<0?0:s.lo[i]+1);
		int h=(s.hi[i]<0?0:s.hi[i]+1);
		k|=(uint64_t)(l&255)<<(14+8*i);
		k|=(uint64_t)(h&255)<<(38+8*i);
	}
	return k;
}

St dec(uint64_t k)
{
	St s{};
	for(int i=0;i<4;++i)
		s.slot[i]=(int)((k>>(3*i))&7);
	s.flags=(int)((k>>12)&3);
	for(int i=0;i<3;++i)
	{
		int l=(int)((k>>(14+8*i))&255);
		int h=(int)((k>>(38+8*i))&255);
		s.lo[i]=l?l-1:-1;
		s.hi[i]=h?h-1:-1;
		s.op[i]=0;
	}
	for(int i=0;i<4;++i)
		if(s.slot[i])
		{
			int v=s.slot[i]-1;
			s.op[v>>1]|=1<<(v&1);
		}
	return s;
}

// relabel strand ids by first occurrence in slots, then encode
uint64_t canon(const St&s)
{
	int mp[3]={-1,-1,-1};
	int nxt{0};
	for(int i=0;i<4;++i)
	{
		if(!s.slot[i])continue;
		int v=s.slot[i]-1;
		if(mp[v>>1]<0)mp[v>>1]=nxt++;
	}
	St t{};
	for(int i=0;i<3;++i){t.lo[i]=-1;t.hi[i]=-1;t.op[i]=0;}
	for(int i=0;i<4;++i)t.slot[i]=0;
	t.flags=s.flags;
	for(int i=0;i<4;++i)
	{
		if(!s.slot[i])continue;
		int v=s.slot[i]-1;
		t.slot[i]=mp[v>>1]*2+(v&1)+1;
	}
	for(int i=0;i<3;++i)
		if(mp[i]>=0)
		{
			t.lo[mp[i]]=s.lo[i];
			t.hi[mp[i]]=s.hi[i];
			t.op[mp[i]]=s.op[i];
		}
	return enc(t);
}

int findUnused(const St&s)
{
	for(int i=0;i<3;++i)
		if(s.lo[i]<0)
			return i;
	return -1;
}

void put(unordered_map<uint64_t,int>&mp,uint64_t key,int val)
{
	if(!val)return;
	auto it=mp.find(key);
	if(it==mp.end())mp.emplace(key,val);
	else
	{
		it->second+=val;
		if(it->second>=MOD)it->second-=MOD;
	}
}

void proc(const St&st,int r,int c,int ways,unordered_map<uint64_t,int>&nxt)
{
	int lS{-1},lE{0},uS{-1},uE{0};
	if(st.slot[r]){int v=st.slot[r]-1;lS=v>>1;lE=v&1;}
	if(st.slot[n]){int v=st.slot[n]-1;uS=v>>1;uE=v&1;}
	int kp=(lS>=0)+(uS>=0);
	for(int right=0;right<=1;++right)
	{
		if(right&&c==m-1)continue;
		for(int down2=0;down2<=1;++down2)
		{
			if(down2&&r==n-1)continue;
			int deg=kp+right+down2;
			if(deg<1||deg>2)continue;
			if(kp==0)
			{
				if(deg==1)
				{
					int cand[2]={0,N-1};
					for(int ci=0;ci<2;++ci)
					{
						int e=cand[ci];
						if(e==0){if(st.flags&1)continue;if(!isBound(r,c))continue;}
						else{if(st.flags&2)continue;}
						if(T[r][c]!=L[e])continue;
						St ns=st;
						ns.slot[r]=0;ns.slot[n]=0;
						int sid=findUnused(ns);
						ns.lo[sid]=ns.hi[sid]=e;
						ns.flags|=(e==0?1:2);
						ns.op[sid]=(e==0?2:1);
						int end=(e==0?1:0);
						if(right)ns.slot[r]=sid*2+end+1;
						else ns.slot[n]=sid*2+end+1;
						put(nxt,canon(ns),ways);
					}
				}
				else
				{
					for(int e=1;e<=N-2;++e)
					{
						if(T[r][c]!=L[e])continue;
						St base=st;
						base.slot[r]=0;base.slot[n]=0;
						int sid=findUnused(base);
						base.lo[sid]=base.hi[sid]=e;
						base.op[sid]=3;
						for(int sw=0;sw<=1;++sw)
						{
							St ns=base;
							int er=sw?1:0,ed=1-er;
							ns.slot[r]=sid*2+er+1;
							ns.slot[n]=sid*2+ed+1;
							put(nxt,canon(ns),ways);
						}
					}
				}
			}
			else if(kp==1)
			{
				int sid=(lS>=0?lS:uS);
				int end=(lS>=0?lE:uE);
				int e=(end==0?st.lo[sid]-1:st.hi[sid]+1);
				if(e<0||e>=N)continue;
				if(T[r][c]!=L[e])continue;
				if(deg==1)
				{
					if(e==0){if(st.flags&1)continue;if(!isBound(r,c))continue;}
					else if(e==N-1){if(st.flags&2)continue;}
					else continue;
					St ns=st;
					ns.slot[r]=0;ns.slot[n]=0;
					ns.lo[sid]=min(ns.lo[sid],e);
					ns.hi[sid]=max(ns.hi[sid],e);
					ns.op[sid]&=~(1<<end);
					ns.flags|=(e==0?1:2);
					if(ns.op[sid]==0)
					{
						if(ns.lo[sid]==0&&ns.hi[sid]==N-1&&ns.flags==3)
						{
							g_ans+=ways;
							if(g_ans>=MOD)g_ans-=MOD;
						}
						continue;
					}
					put(nxt,canon(ns),ways);
				}
				else
				{
					if(e==0||e==N-1)continue;
					St ns=st;
					ns.slot[r]=0;ns.slot[n]=0;
					ns.lo[sid]=min(ns.lo[sid],e);
					ns.hi[sid]=max(ns.hi[sid],e);
					if(right)ns.slot[r]=sid*2+end+1;
					else ns.slot[n]=sid*2+end+1;
					put(nxt,canon(ns),ways);
				}
			}
			else
			{
				if(right||down2)continue;
				int eA=(lE==0?st.lo[lS]-1:st.hi[lS]+1);
				int eB=(uE==0?st.lo[uS]-1:st.hi[uS]+1);
				if(eA!=eB)continue;
				int e=eA;
				if(e<0||e>=N)continue;
				if(e==0||e==N-1)continue;
				if(T[r][c]!=L[e])continue;
				if(lS==uS)continue;
				int newlo=min(st.lo[lS],st.lo[uS]);
				int newhi=max(st.hi[lS],st.hi[uS]);
				int lenA=st.hi[lS]-st.lo[lS]+1;
				int lenB=st.hi[uS]-st.lo[uS]+1;
				if(newhi-newlo+1!=lenA+lenB+1)continue;
				int mergedOp{0};
				if(lE==1)mergedOp|=(st.op[lS]&1);
				if(uE==1)mergedOp|=(st.op[uS]&1);
				if(lE==0)mergedOp|=(st.op[lS]&2);
				if(uE==0)mergedOp|=(st.op[uS]&2);
				St ns=st;
				ns.slot[r]=0;ns.slot[n]=0;
				ns.lo[lS]=newlo;ns.hi[lS]=newhi;ns.op[lS]=mergedOp;
				ns.lo[uS]=-1;ns.hi[uS]=-1;ns.op[uS]=0;
				for(int i=0;i<4;++i)
				{
					if(!ns.slot[i])continue;
					int v=ns.slot[i]-1;
					if((v>>1)==uS)
						ns.slot[i]=lS*2+(v&1)+1;
				}
				if(mergedOp==0)
				{
					if(newlo==0&&newhi==N-1&&ns.flags==3)
					{
						g_ans+=ways;
						if(g_ans>=MOD)g_ans-=MOD;
					}
					continue;
				}
				put(nxt,canon(ns),ways);
			}
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	N=n*m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>T[i][j];
	for(int i=0;i<N;++i)cin>>L[i];

	if(N==1)
	{
		cout<<(T[0][0]==L[0]?1:0)<<'\n';
		return 0;
	}

	St init{};
	for(int i=0;i<3;++i){init.lo[i]=-1;init.hi[i]=-1;init.op[i]=0;}
	for(int i=0;i<4;++i)init.slot[i]=0;
	init.flags=0;

	unordered_map<uint64_t,int>cur,nxt;
	cur.emplace(canon(init),1);
	cur.reserve(4096);
	for(int c=0;c<m;++c)
	{
		for(int r=0;r<n;++r)
		{
			nxt.clear();
			nxt.reserve(cur.size()*4+16);
			for(auto&kv:cur)
				proc(dec(kv.first),r,c,kv.second,nxt);
			cur.swap(nxt);
		}
	}
	cout<<g_ans%MOD<<'\n';
	return 0;
}
```
