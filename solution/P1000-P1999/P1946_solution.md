# P1946 Olympic

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $n$ 支队伍（$n\le 15$），第 $i$ 支赢得 $G_i$ 枚金牌、$S_i$ 枚银牌、$B_i$ 枚铜牌。我们要选三个整数权重

$$
1000\ge P_g\ge P_s\ge P_b\ge 1,
$$

队伍 $i$ 的总分为

$$
\mathrm{score}_i=G_iP_g+S_iP_s+B_iP_b .
$$

所有队伍按总分降序排名；**总分相同时我们的队伍排在最前**。第 $1$ 支队伍就是我们的队伍。

要求让我们的名次尽可能靠前（名次数值尽量小）。在达到最优名次的所有三元组中，按 $P_g\to P_s\to P_b$ 的顺序输出字典序最小的一组。

### 1.2 关键约束与规模暗示

- $n\le 15$：极小，可以放心对每支队伍单独做算术，不需要任何数据结构。
- 权重上界 $P=1000$：合法三元组个数为

$$
\binom{P+2}{3}=\binom{1002}{3}=167\,167\,000 .
$$

- 若对每个三元组再扫 $n$ 支队伍，运算量约 $1.67\times 10^8\times 15\approx 2.5\times 10^{9}$，超时。所以必须**把 $n$ 这一维摊掉**：对固定的 $(P_g,P_s)$，让 $P_b$ 的扫描只花 $O(1)$ 摊销代价。
- 题面另外给了「$10\%$ 数据最优解 $P_g\le 10$、$30\%$ 数据最优解 $P_g\le 100$」。这说明**答案的 $P_g$ 往往很小**，出题人预期的做法是「$P_g$ 从小到大枚举 + 早退」。这提示我们用**带下界剪枝的搜索**：只要剪枝是可证明的（不是靠数据弱），最坏复杂度仍然可控。

---

## 2. 思维链（Chain-of-thought）

### 2.1 第一步：把「名次」变成「计数」

排名只看总分。设当前三元组为 $(P_g,P_s,P_b)$，记

$$
c=\#\{\,i\ge 2:\mathrm{score}_i>\mathrm{score}_1\,\}.
$$

**引理 1（名次公式）**：我们的名次恰为 $c+1$。

**证明**：按总分降序排列后，排在我们前面的队伍，其总分必然严格大于我们：若其总分等于我们，按题目的平局规则（同分时我们最前）它应排在我们后面。所以前方队伍集合恰为 $\{i\ge2:\mathrm{score}_i>\mathrm{score}_1\}$，数量为 $c$，名次为 $c+1$。$\square$

于是问题变成：

> 最小化 $c$；在 $c$ 最小的前提下，使 $(P_g,P_s,P_b)$ 字典序最小。

记第 $i$ 支队伍相对我们的差值为

$$
dG_i=G_i-G_1,\quad dS_i=S_i-S_1,\quad dB_i=B_i-B_1,
$$

则

$$
\mathrm{score}_i-\mathrm{score}_1=dG_iP_g+dS_iP_s+dB_iP_b=:f_i(P_g,P_s,P_b),
$$

队伍 $i$ 超过我们当且仅当 $f_i>0$。

### 2.2 第二步：暴力枚举，看看瓶颈在哪

合法三元组有 $1.67\times 10^8$ 个。逐一代入 $n=15$ 支队伍求 $c$ 是 $2.5\times10^9$ 次乘加，不可接受。

但注意一件很关键的事：**固定 $P_g,P_s$ 之后，每个 $f_i$ 只是 $P_b$ 的一次函数**。一次函数在整数点上「由正变负」最多发生一次。这意味着，对固定的 $(P_g,P_s)$，$c(P_b)$ 作为 $P_b$ 的函数是「分段常数」，跳变点只有 $O(n)$ 个。于是我们不必对每个 $P_b$ 重算 $n$ 个 $f_i$，而是：

1. 用 $O(n)$ 算出 $P_b=1$ 时的 $c$；
2. 用 $O(n)$ 求出每个队伍的跳变位置，做成差分事件；
3. 用 $O(P_s)$ 从左到右扫描 $P_b$，边扫边更新 $c$。

这样每个 $(P_g,P_s)$ 的代价是 $O(n+P_s)$。总代价

$$
\sum_{P_g=1}^{P}\sum_{P_s=1}^{P_g}O(n+P_s)
=O\!\left(nP^2+\frac{P^3}{6}\right)
\approx 7.5\times10^6+1.67\times10^8,
$$

常数很小（每次扫描只有一次加法与两次比较），可以接受。下面把「跳变位置」算准。

### 2.3 第三步：一次函数的翻转位置

**引理 2（单调翻转）**：固定 $P_g,P_s$，令 $c_i=dG_iP_g+dS_iP_s$，则 $f_i(P_b)=c_i+dB_iP_b$。

- 若 $dB_i>0$，则 $\{P_b\in[1,P_s]:f_i>0\}$ 是一个后缀 $[t,P_s]$；
- 若 $dB_i<0$，则它是一个前缀 $[1,t]$；
- 若 $dB_i=0$，则要么整个区间都 $>0$，要么整段都 $\le 0$。

**证明**：$f_i$ 是 $P_b$ 的仿射函数，斜率 $dB_i$。若斜率 $>0$ 则严格递增，取值 $>0$ 的解集是一条上射线，与整数区间 $[1,P_s]$ 相交即得后缀；斜率 $<0$ 时严格递减，同理得前缀；斜率为 $0$ 时常数。$\square$

具体阈值（整数算术）：

- $dB_i>0$：$c_i+dB_iP_b>0\iff P_b>\left\lfloor\dfrac{-c_i}{dB_i}\right\rfloor$。记 $t=\left\lfloor\dfrac{-c_i}{dB_i}\right\rfloor+1$，则从 $P_b=t$ 起开始超过我们。
- $dB_i<0$：记 $e=-dB_i>0$，则 $c_i-eP_b>0\iff P_b<\dfrac{c_i}{e}\iff P_b\le\left\lceil\dfrac{c_i}{e}\right\rceil-1=\left\lfloor\dfrac{c_i-1}{e}\right\rfloor$（$c_i>0$ 时）。记 $t=\left\lfloor\dfrac{c_i-1}{e}\right\rfloor$，则 $P_b\le t$ 时超过我们，$P_b=t+1$ 时不再超过。
- $dB_i=0$：$c_i>0$ 时恒超过。

这里的除法是**向下取整**，负数也要正确取整（C++ 的 `/` 向零取整，必须手写 `floordiv`）。只要阈值落在 $[2,P_s]$ 内才需要登记事件；落在区间外的事件不会发生。

**引理 3（扫描正确性）**：设 `cnt` 为 $P_b=1$ 时的超越数，`diff[t]` 记录在 $P_b=t$ 处超越数的增量。从左到右做前缀和，得到的恰好是每个 $P_b$ 的超越数。

**证明**：由引理 2，每支队伍对 $\mathrm{cnt}(P_b)$ 的贡献是一个示性函数：恒 0、恒 1、从 $t$ 起为 1（后缀）、到 $t$ 为止为 1（前缀，等价于在 $t+1$ 处减 1）。把每个队伍在「起始值 $P_b=1$ 的贡献」与「跳变事件」处累加，前缀和即逐点重构出每个队伍示性函数之和。求和不改变这点。故扫描值精确等于 $c(P_b)$。$\square$

### 2.4 第四步：剪枝——线性函数在三角形上的最小值在顶点

光有上面还不够优雅：$P_g$、$P_s$ 两重循环里有很多分支显然不可能更优。我们要一个**可证明的下界**。

**引理 4（顶点最小值）**：设 $f$ 是仿射函数（一次函数），$K$ 是凸多边形，则 $f$ 在 $K$ 上的最小值一定能在 $K$ 的某个顶点处取到。

**证明**：任取 $x\in K$，把 $x$ 写成顶点的凸组合 $x=\sum_j\lambda_jv_j$（$\lambda_j\ge0,\sum\lambda_j=1$）。由仿射性 $f(x)=\sum_j\lambda_jf(v_j)\ge\min_jf(v_j)$。对任意 $x$ 成立，故全局最小等于顶点最小。$\square$

固定 $P_g$，考察区域

$$
T_{P_g}=\{(P_s,P_b):1\le P_b\le P_s\le P_g\}.
$$

它是三角形，顶点为 $(1,1)$、$(P_g,1)$、$(P_g,P_g)$（可以逐条边验证：$P_s=1$ 退化为点 $(1,1)$；$P_b=1$ 是 $P_s$ 从 $1$ 到 $P_g$ 的边；$P_b=P_s$ 是另一条边）。

对第 $i$ 支队伍，$f_i=dG_iP_g+dS_iP_s+dB_iP_b$ 在 $(P_s,P_b)$ 上仿射。若它在三个顶点处都 $>0$，由引理 4 它在整个 $T_{P_g}$ 上都 $>0$，即这支队伍**无论 $P_s,P_b$ 怎么取都排在我们前面**。把这种队伍数记为 $\mathrm{forced}(P_g)$。显然

$$
\forall (P_s,P_b)\in T_{P_g}:\quad c(P_g,P_s,P_b)\ \ge\ \mathrm{forced}(P_g).
$$

所以这个 $P_g$ 下能取得的最好名次至少是 $\mathrm{forced}(P_g)+1$。

**引理 5（$P_g$ 层剪枝正确）**：若 $\mathrm{forced}(P_g)+1\ge \mathrm{bestRank}$，则跳过整个 $P_g$ 不损失最优解。

**证明**：对这个 $P_g$ 的任意三元组，名次 $\ge\mathrm{forced}+1\ge\mathrm{bestRank}$。我们只在名次**严格变小**时更新答案；名次相等时，先枚举到的（字典序更小的）三元组才是需要的，后面这些不可能更优。所以跳过它们不影响最终答案。$\square$

同理，固定 $(P_g,P_s)$ 后，$f_i$ 只随 $P_b$ 变化，是仿射函数，区间 $[1,P_s]$ 的顶点是 $P_b=1$ 与 $P_b=P_s$。若某队伍在两端都 $>0$，则它在整个区间都 $>0$（引理 4 的一维版本：线性函数最小值在端点）。记这种队伍数为 $\mathrm{forced2}(P_g,P_s)$，则任意 $P_b$ 都有 $c\ge\mathrm{forced2}$。若 $\mathrm{forced2}+1\ge\mathrm{bestRank}$，整个 $(P_g,P_s)$ 分支都可以剪掉。

扫描 $P_b$ 时还有一个免费的早停：

**引理 6（扫描早停）**：若某个 $P_b$ 处 $c=\mathrm{forced2}$，则之后所有 $P_b$ 的名次都 $\ge c+1$，可以立即停止本 $(P_g,P_s)$ 的扫描。

**证明**：由定义 $\mathrm{forced2}$ 支队伍在区间内每个 $P_b$ 都超过我们，所以任意 $P_b$ 都有 $c(P_b)\ge\mathrm{forced2}=c$。它们名次都 $\ge c+1$，而我们刚刚已经处理过当前 $P_b$。又因为要求字典序最小，同一个名次只需保留最早出现的 $P_b$，后续同分或更差都不需要。$\square$

### 2.5 第五步：字典序最小

**引理 7（枚举顺序给出字典序最小）**：按 $P_g$ 升序、$P_s$ 升序、$P_b$ 升序枚举，且只在名次**严格变小**时更新答案，最终保存的三元组就是「最优名次中字典序最小」的那一组。

**证明**：设最优名次为 $R^\ast$。所有名次为 $R^\ast$ 的三元组按字典序排列，取第一个 $T^\ast$。按上述顺序枚举时，$T^\ast$ 之前的任何三元组名次都 $>R^\ast$（否则与 $T^\ast$ 是字典序第一个矛盾），它们只会把 bestRank 改到 $>R^\ast$ 或不变；枚举到 $T^\ast$ 时必然有 $R^\ast<\mathrm{bestRank}$，于是被记录。此后所有名次 $\ge R^\ast$ 的三元组都不会触发严格更新（名次为 $R^\ast$ 的字典序都在 $T^\ast$ 之后）。所以最终保存的正是 $T^\ast$。$\square$

### 2.6 完整正确性

**定理**：算法输出的三元组在最优名次下字典序最小。

**证明**：合法性：枚举范围就是 $1\le P_b\le P_s\le P_g\le1000$，每个候选都合法。完备性：剪枝只跳过「不可能严格改进 bestRank」的分支（引理 5、6），而不剪枝的部分会逐点扫描每个 $(P_g,P_s,P_b)$，所以真正的最优解所在分支一定被完整访问。最优性：每个被访问到的三元组都用引理 3 的扫描精确计算了名次，因此 bestRank 最终等于最优名次；引理 7 保证保存的三元组字典序最小。$\square$

### 2.7 复杂度

- 时间：$O\!\left(nP^2+P^3/6\right)$，$P=1000$，约 $1.7\times10^8$ 次极简运算；实际因引理 5/6 的剪枝远小于此。
- 空间：$O(1)$（只有若干定长小数组）。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 同分我们最前 $\Rightarrow$ 名次 $=1+\#\{\text{分数严格高于我们的队伍}\}$，把排名问题化为计数问题。
- **consider easier version** — 先只看「固定 $(P_g,P_s)$ 时 $c$ 关于 $P_b$ 的变化」：一次函数最多翻转一次，于是可以差分事件 + 线性扫描。
- **classic problem solution** — 枚举 + 下界剪枝：仿射函数在凸多边形上的最小值在顶点，用「三个顶点都赢我们的队伍数」做下界。
- **defer reconstruction / 顺序即字典序** — 升序枚举，只在严格变优时更新，天然得到字典序最小。
- **复杂度配平** — 用 $O(n+P_s)$ 的线性事件扫描把每个 $(P_g,P_s)$ 压缩，总复杂度 $O(nP^2+P^3)$。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 队伍数 | 读入 | $\le 15$ |
| `G[i],S[i],B[i]` | `i64 [20]` | 第 $i$ 队三种奖牌数 | 读入 | 下标 0 是我们 |
| `Pg,Ps,Pb` | `int` | 当前枚举的三元组 | 循环变量 | 权重 |
| `bestRank` | `int` | 目前最优名次 | $n+1$ | 越小越好 |
| `bPg,bPs,bPb` | `int` | 最优名次下字典序最小的三元组 | $1,1,1$ | answer |
| `forced` | `int` | 固定 $P_g$ 时，三角形三顶点都赢我们的队伍数 | 每轮重算 | 下界 |
| `forced2` | `int` | 固定 $(P_g,P_s)$ 时，$P_b=1$ 与 $P_b=P_s$ 都赢我们的队伍数 | 每轮重算 | 下界 |
| `cnt` | `int` | $P_b=1$ 时超过我们的队伍数 | 每轮重算 | count |
| `diff[t]` | `int [1005]` | 在 $P_b=t$ 处超越数的增量 | 0 | difference |
| `touched[]` | `int [1005]` | 本轮被写过的 `diff` 下标，用于只清这些位置 | — | 懒清空 |
| `cur` | `int` | 扫描到当前 $P_b$ 时的超越数 | `cnt` | 当前值 |

### 4.2 算法步骤

1. 读入 $n$ 与所有 $(G_i,S_i,B_i)$，下标从 $0$ 开始，队伍 $0$ 是我们。
2. `bestRank=n+1`，`bPg=bPs=bPb=1`。
3. 对 $P_g=1,2,\dots,1000$（若 `bestRank==1` 立即退出）：
   1. 计算 $\mathrm{forced}$：对每支 $i\ge1$，在顶点 $(P_s,P_b)=(1,1),(P_g,1),(P_g,P_g)$ 处算 $f_i$，三点都 $>0$ 则 `++forced`。
   2. 若 $\mathrm{forced}+1\ge\mathrm{bestRank}$，跳过本 $P_g$。
4. 对 $P_s=1,2,\dots,P_g$（若 `bestRank==1` 立即退出）：
   1. 计算 $\mathrm{forced2}$：对每支 $i\ge1$，若 $f_i(1)>0$ 且 $f_i(P_s)>0$ 则 `++forced2`；若 $\mathrm{forced2}+1\ge\mathrm{bestRank}$，跳过本 $P_s$。
   2. 计算 `cnt`（$P_b=1$ 时的超越数）并登记事件：对每支队伍按引理 2 的阈值，决定它是「从 $t$ 起超过」（`diff[t]+=1`）还是「到 $t$ 为止超过」（`diff[t+1]-=1`），或恒超过 / 恒不超过。所有写过的下标记入 `touched`。
   3. 令 `cur=cnt`，对 $P_b=1,\dots,P_s$：`cur+=diff[Pb]`；若 `cur+1<bestRank` 则更新 `bestRank` 与答案三元组；若 `cur==forced2` 则跳出扫描。
   4. 把 `touched` 中的 `diff` 清零。
5. 输出 `bPg bPs bPb`。

### 4.3 复杂度

- 时间：$O(nP^2+P^3)$，$P=1000$，约 $1.7\times10^8$ 基础运算，剪枝后更少。
- 空间：$O(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1946 Olympic
// https://www.luogu.com.cn/problem/P1946
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{20};
constexpr int mxp{1005};
constexpr int pmax{1000};

int n;
i64 G[mxn],S[mxn],B[mxn];
int diff[mxp];
int touched[mxp];

i64 floordiv(i64 a,i64 b)	// b>0, floor(a/b)
{
	if(a>=0)
		return a/b;
	return -((-a+b-1)/b);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>G[i]>>S[i]>>B[i];

	int bestRank{n+1};
	int bPg{1},bPs{1},bPb{1};

	for(int Pg=1;Pg<=pmax&&bestRank>1;++Pg)
	{
		// 固定 Pg：三个顶点都赢我们的队伍必然一直赢，名次下界
		int forced{0};
		for(int i=1;i<n;++i)
		{
			i64 dG=G[i]-G[0],dS=S[i]-S[0],dB=B[i]-B[0];
			i64 c=dG*Pg;
			i64 f1=c+dS+dB;
			i64 f2=c+dS*Pg+dB;
			i64 f3=c+dS*Pg+dB*Pg;
			if(f1>0&&f2>0&&f3>0)
				++forced;
		}
		if(forced+1>=bestRank)
			continue;
		for(int Ps=1;Ps<=Pg&&bestRank>1;++Ps)
		{
			// 固定 (Pg,Ps)：P_b 两端都赢则整段都赢
			int forced2{0};
			for(int i=1;i<n;++i)
			{
				i64 c=(G[i]-G[0])*Pg+(S[i]-S[0])*Ps;
				i64 dB=B[i]-B[0];
				if(c+dB>0&&c+dB*Ps>0)
					++forced2;
			}
			if(forced2+1>=bestRank)
				continue;
			int cnt{0};
			int nt{0};
			for(int i=1;i<n;++i)
			{
				i64 c=(G[i]-G[0])*Pg+(S[i]-S[0])*Ps;
				i64 dB=B[i]-B[0];
				if(dB>0)
				{
					i64 t=floordiv(-c,dB)+1;	// f(Pb)>0 iff Pb>=t
					if(t<=1)
						++cnt;
					else if(t<=Ps)
					{
						diff[t]+=1;
						touched[nt++]=t;
					}
				}
				else if(dB<0)
				{
					i64 e=-dB;
					if(c>0)
					{
						i64 pm=floordiv(c-1,e);	// f(Pb)>0 iff Pb<=pm
						if(pm>=1)
						{
							++cnt;
							i64 t=pm+1;
							if(t<=Ps)
							{
								diff[t]-=1;
								touched[nt++]=t;
							}
						}
					}
				}
				else if(c>0)
					++cnt;
			}
			int cur{cnt};
			for(int Pb=1;Pb<=Ps;++Pb)
			{
				cur+=diff[Pb];
				if(cur+1<bestRank)
				{
					bestRank=cur+1;
					bPg=Pg;
					bPs=Ps;
					bPb=Pb;
				}
				if(cur==forced2)
					break;	// 已经到达本层下界
			}
			for(int t=0;t<nt;++t)
				diff[touched[t]]=0;
		}
	}

	cout<<bPg<<' '<<bPs<<' '<<bPb<<'\n';
	return 0;
}
```
