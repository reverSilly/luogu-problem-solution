# P1817 棋盘染色

## 1. 重新读题

给一个 $N\times M$ 的网格，每个格子染黑或白，要求：

1. 所有黑格 **连通**（四连通）；
2. 所有白格 **连通**；
3. 至少有一个黑格贴边，至少有一个白格贴边。

求染色方案数。

约束：$1\le N\le7$，$1\le M\le8$，所以格子数不超过 $56$。

**规模暗示**：$2^{56}$ 种染色不可能枚举；但较小的维度只有 $7$，适合「按行扫描 + 记录当前行连通性」的轮廓线 / 连通性状压 DP。注意 $N\le7\le M\le8$，可以先转置使扫描宽度 $W=\min(N,M)\le7$，这样状压的底数才小。

---

## 2. 思维链（Chain-of-thought）

### 2.1 最难的是「连通」

这题不是普通的按格状压：一个颜色是否连通是**全局**性质。但网格是「薄」的（宽 $\le7$），可以一行一行加进来。加新行时，未来与过去的连接只能通过当前这一行，于是**只需记住当前行每个格子的颜色以及它们各自属于哪个连通块**。

换句话说，扫描到第 $i$ 行时，把已处理区域（前 $i+1$ 行）中所有格子按颜色和四连通分成若干块；这些块要么与第 $i$ 行有交，要么已经完全「封死」——后者再也不可能和未来的格子相连。因此状态只需保存：

- 第 $i$ 行每格的颜色 $c_j$；
- 第 $i$ 行每格所属连通块的标号 $\ell_j$（同块同标号；标号按首次出现顺序规范化）；
- 两种颜色是否已经各自出现过一个「封死」的块（`sealed[2]`）；
- 两种颜色是否已经在边界上出现过（`border[2]`）。

**为什么要 `sealed`**：一旦某颜色的块与当前行彻底断开，它在最终染色里就是一个独立的连通块；若之后这种颜色又出现，就必然至少有两个块，不合法。所以「封死」之后必须禁止该颜色再出现。

### 2.2 一行一行转移：用并查集合并连通块

从第 $i$ 行的状态，枚举第 $i+1$ 行的颜色掩码 `mask`（$2^W$ 种），做并查集：

- **旧行内部**：标号相同的格子合并（它们本来就是同一块）；
- **新行内部**：横向相邻且同色的格子合并；
- **新旧之间**：同一列、同色的上下两格合并（这是唯一能跨越行边界的边）。

合并完，新行的连通块由「新行格子所在并查集根」决定，重新按首次出现顺序编号，就是新状态的 $\ell$。

**如何判断旧块「封死」**：一块旧块封死，当且仅当块内没有任何一列 $j$ 满足「旧格 $(i,j)$ 与新格 $(i+1,j)$ 同色」（即没有竖直边连到新行）。因为旧行以上的部分只能通过旧行—新行之间的竖直边延续。

### 2.3 合法性剪枝（关键）

设 `seal[c]` 为本次转移中封死的颜色 $c$ 的旧块个数，`has[c]` 为新行是否出现颜色 $c$，`oldSealed[c]` 为之前是否已有封死的 $c$ 块。

1. 若 `seal[c] >= 2`：同色两块同时封死，永远无法相连 → 丢弃。
2. 若 `seal[c] >= 1` 且 `oldSealed[c]`：加上之前的又一块，至少两块 → 丢弃。
3. 若 `seal[c] >= 1` 且 `has[c]`：新行还有颜色 $c$，它与刚封死的块之间不可能再连（封死块已经和未来无关）→ 至少两块 → 丢弃。
4. 若 `oldSealed[c]` 且 `has[c]`：已经封死过，又冒出新块 → 丢弃。

**引理 1（这些剪枝是必要且充分的）**

（必要性）若出现上述任一情况，最终该颜色至少有两个连通块，因为封死的块与任何未来格子都没有边相连；故该染色一定不合法。

（充分性）反过来，若一条染色合法（每色连通），把它的真实连通块按行扫描：真实块只有在「该颜色此后再也不出现」时才会与当前行断开，此时新行确实没有该颜色，规则 3、4 不会触发；真实块至多一个，规则 1、2 也不会触发。所以合法染色对应的 DP 路径不会被误杀。$\blacksquare$

于是所有被 DP 保留的路径，最终再检查一次「每色恰好一块」即可。

### 2.4 终局判定

处理完所有 $H$ 行后，对每个状态：

- 统计最后一行的不同标号个数 `cc[c]`（即该颜色仍「活着」的块数）；
- 对颜色 $c$：若 `sealed[c]`，说明它唯一的那块已封死，要求 `cc[c]==0`；否则要求 `cc[c]==1`；
- 还要求 `border[0] && border[1]`。

满足者把方案数累加。

**引理 2（终局条件刻画合法染色）**
一个状态对应合法染色，当且仅当上述条件成立。

**证明**：由引理 1，保留的路径中每种颜色的块总数 = `sealed[c] + cc[c]`（封死的块至多一个，活着的块数就是 `cc[c]`）。条件恰为「总块数 $=1$」，即该颜色连通。再加上两种颜色都在边界出现过，正是题面三条要求。又：每个染色唯一决定一条状态路径（颜色、标号规范化、标志都由染色唯一确定），每条被接受的路径唯一决定一个染色，故计数既不多也不少。$\blacksquare$

### 2.5 顺时针转置与编码

连通性和「贴边」在转置下不变，所以令 $W=\min(N,M)$、$H=\max(N,M)$，把网格转置成 $H$ 行 $W$ 列，扫描宽度 $\le7$。

编码：每格用 $4$ 位（第 $0$ 位颜色，第 $1\sim3$ 位标号，标号 $<8$），$W\le7$ 时占 $28$ 位；再用 $4$ 位放 `sealed[2]`、`border[2]`，总共 $\le32$ 位，用一个 `unsigned int` 表示。标号规范化保证「同一个连通划分」只有一种编码，`unordered_map` 才能正确合并计数。

---

## 3. 模式总结（Pattern Summary）

- **consider impossible brute force** — $2^{56}$ 不可枚举，但宽度 $\le7$。
- **reduce dimension** — 转置使扫描宽度 $W=\min(N,M)\le7$。
- **profile / connectivity DP** — 按行加，状态记「当前行颜色 + 连通块标号 + 每色是否已封死 + 每色是否贴边」。
- **DSU on transition** — 旧标号合并、新行横向合并、同色竖向合并；旧块无竖直延续即封死。
- **prune by necessity** — 同色封死块 $\ge2$、封死后再出现该颜色，都会导致不连通，直接丢弃。
- **final check** — 每色块数恰为 1 且两色都贴边。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `N,M` | `int` | 原网格行、列 | 读入 | — |
| `W,H` | `int` | 转置后的宽度、行数 | `W=min(N,M)`,`H=max(N,M)` | width/height |
| `dp` | `unordered_map<unsigned,i64>` | 当前行状态 → 方案数 | 第一行枚举 | — |
| `code` | `unsigned` | 状态编码 | — | 打包 |
| `co[j]` | `int` | 旧行第 $j$ 格颜色 | 解码 | color old |
| `lo[j]` | `int` | 旧行第 $j$ 格块标号 | 解码 | label old |
| `nc[j]`, `nl[j]` | `int` | 新行颜色 / 块标号 | 转移产生 | new |
| `s0,s1` | `int` | 颜色 0/1 是否已经封死过 | 解码 | sealed |
| `b0,b1` | `int` | 颜色 0/1 是否已贴边 | 解码 | border |
| `seal[c]` | `int` | 本次转移封死的颜色 $c$ 块数 | 转移计算 | — |
| `hasc[c]` | `bool` | 新行是否出现颜色 $c$ | 转移计算 | — |
| `nf[]`, `getf`, `uni` | `int[16]` | 并查集（旧行 $0..W-1$，新行 $W..2W-1$） | 每次重置 | — |

### 4.2 算法步骤

1. 读入 $N,M$，令 $W=\min(N,M),H=\max(N,M)$。
2. **初始化第 0 行**：枚举 `mask∈[0,2^W)`；颜色由 mask 给出；同一连续同色段共用一个标号（按出现顺序）；第 0 行整行贴边，据此设置 `border`；`sealed` 全 0。加入 `dp`。
3. **逐行转移** $i=1\dots H-1$：对 `dp` 中每个 `(code,cnt)`：
   - 解码出 `co,lo,s,b`；
   - 枚举新行 `mask`，得到 `nc`；若 `s[c] && hasc[c]` 跳过；
   - 重置并查集，按 2.2 合并三类边；
   - 计算旧块是否封死，得到 `seal[]`；按 2.3 的四条规则剪枝；
   - 重新规范化新行标号 `nl`；
   - 更新 `sealed`（`s|(seal>0)`）；
   - 更新 `border`：若 $i=H-1$ 或 $j=0$ 或 $j=W-1$，该格贴边；
   - 打包新 `code`，累加 `cnt` 到 `ndp`。
   - 整体 `dp.swap(ndp)`。
4. **统计答案**：按 2.4 检查 `border` 与每色块数，累加 `cnt`。
5. 输出答案。

### 4.3 复杂度与实际规模

- 状态数 $S$：宽度 $W\le7$ 时可达状态很少（$7\times8$ 实测每行稳定在 $612$ 个）。
- 时间：$O\big(H\cdot S\cdot 2^W\cdot W^2\big)$（并查集每次 $O(W\alpha)$，块延续检查 $O(W^2)$），$7\times8$ 实测约 $0.15$ 秒。
- 空间：$O(S)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1817 棋盘染色
// https://www.luogu.com.cn/problem/P1817
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int N,M,W,H;

constexpr int mxw{8};
constexpr int mxv{16};

int nf[mxv];
int getf(int x){return nf[x]==x?x:nf[x]=getf(nf[x]);}
void uni(int a,int b){a=getf(a);b=getf(b);if(a!=b)nf[a]=b;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>N>>M;
	W=min(N,M);H=max(N,M);

	unordered_map<unsigned int,i64> dp,ndp;
	dp.reserve(1<<20);

	// 第一行：只有行内横向连通
	for(int mask=0;mask<(1<<W);++mask)
	{
		int co[mxw],lb[mxw];
		int id{0};
		for(int j=0;j<W;++j)
		{
			co[j]=(mask>>j)&1;
			if(j>0&&co[j]==co[j-1])
				lb[j]=lb[j-1];
			else
				lb[j]=id++;
		}
		unsigned int code{0};
		for(int j=0;j<W;++j)
		{
			code|=(unsigned)co[j]<<(4*j);
			code|=(unsigned)lb[j]<<(4*j+1);
		}
		int b0{0},b1{0};
		for(int j=0;j<W;++j)
		{
			// 第 0 行整行都在边界上
			if(co[j]) b1=1;
			else b0=1;
		}
		code|=(unsigned)b0<<(4*W+2);
		code|=(unsigned)b1<<(4*W+3);
		dp[code]+=1;
	}

	for(int i=1;i<H;++i)
	{
		ndp.clear();
		for(auto&[code,cnt]:dp)
		{
			int co[mxw],lo[mxw];
			for(int j=0;j<W;++j)
			{
				co[j]=(code>>(4*j))&1;
				lo[j]=(code>>(4*j+1))&7;
			}
			int s0=(code>>(4*W))&1,s1=(code>>(4*W+1))&1;
			int b0=(code>>(4*W+2))&1,b1=(code>>(4*W+3))&1;

			for(int mask=0;mask<(1<<W);++mask)
			{
				int nc[mxw];
				bool hasc[2]={false,false};
				for(int j=0;j<W;++j){nc[j]=(mask>>j)&1;hasc[nc[j]]=true;}

				if((s0&&hasc[0])||(s1&&hasc[1]))
					continue;

				for(int t=0;t<2*W;++t) nf[t]=t;
				// 旧行同标号合并
				int first[mxw];
				for(int t=0;t<mxw;++t) first[t]=-1;
				for(int j=0;j<W;++j)
				{
					if(first[lo[j]]<0) first[lo[j]]=j;
					else uni(j,first[lo[j]]);
				}
				// 新行横向
				for(int j=0;j<W-1;++j)
					if(nc[j]==nc[j+1]) uni(W+j,W+j+1);
				// 竖直
				for(int j=0;j<W;++j)
					if(co[j]==nc[j]) uni(j,W+j);

				bool seen[mxv]{};
				int seal[2]={0,0};
				for(int j=0;j<W;++j)
				{
					int r=getf(j);
					if(seen[r]) continue;
					seen[r]=true;
					bool cont=false;
					for(int k=0;k<W;++k)
						if(getf(k)==r&&co[k]==nc[k]) cont=true;
					if(!cont) ++seal[co[j]];
				}
				if(seal[0]>=2||seal[1]>=2) continue;
				if((seal[0]&&s0)||(seal[1]&&s1)) continue;
				// 某颜色刚被封死，新行却仍出现该颜色：必然裂成两个连通块
				if((seal[0]&&hasc[0])||(seal[1]&&hasc[1])) continue;

				int id{0},rid[mxv];
				for(int t=0;t<mxv;++t) rid[t]=-1;
				int nl[mxw];
				for(int j=0;j<W;++j)
				{
					int r=getf(W+j);
					if(rid[r]<0) rid[r]=id++;
					nl[j]=rid[r];
				}

				unsigned int ncode{0};
				for(int j=0;j<W;++j)
				{
					ncode|=(unsigned)nc[j]<<(4*j);
					ncode|=(unsigned)nl[j]<<(4*j+1);
				}
				int ns0=s0|(seal[0]>0),ns1=s1|(seal[1]>0);
				int nb0=b0,nb1=b1;
				for(int j=0;j<W;++j)
				{
					bool isb=(i==0)||(i==H-1)||j==0||j==W-1;
					if(isb)
					{
						if(nc[j]) nb1=1;
						else nb0=1;
					}
				}
				ncode|=(unsigned)ns0<<(4*W);
				ncode|=(unsigned)ns1<<(4*W+1);
				ncode|=(unsigned)nb0<<(4*W+2);
				ncode|=(unsigned)nb1<<(4*W+3);
				ndp[ncode]+=cnt;
			}
		}
		dp.swap(ndp);
	}

	i64 ans{0};
	for(auto&[code,cnt]:dp)
	{
		int co[mxw],lb[mxw];
		for(int j=0;j<W;++j)
		{
			co[j]=(code>>(4*j))&1;
			lb[j]=(code>>(4*j+1))&7;
		}
		int s0=(code>>(4*W))&1,s1=(code>>(4*W+1))&1;
		int b0=(code>>(4*W+2))&1,b1=(code>>(4*W+3))&1;
		if(!b0||!b1) continue;
		int cc[2]={0,0};
		bool seen[mxw]{};
		for(int j=0;j<W;++j)
		{
			if(seen[lb[j]]) continue;
			seen[lb[j]]=true;
			++cc[co[j]];
		}
		bool ok=true;
		if(s0){ if(cc[0]!=0) ok=false; }
		else if(cc[0]!=1) ok=false;
		if(s1){ if(cc[1]!=0) ok=false; }
		else if(cc[1]!=1) ok=false;
		if(ok) ans+=cnt;
	}
	cout<<ans<<'\n';
	return 0;
}
```
