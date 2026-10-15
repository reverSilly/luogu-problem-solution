# P1076 [NOIP 2012 普及组] 寻宝 题解

---

## 1. Re-read the problem

藏宝楼有 $N+1$ 层，最顶层是宝藏。下面 $N$ 层每层 $M$ 个房间，编号 $0\sim M-1$，呈逆时针环形排列。每个房间有两个属性：

- $stair_{i,j}\in\{0,1\}$：第 $i$ 层 $j$ 号房间是否有通往上一层的楼梯；
- $sign_{i,j}$：房间内指示牌上的数字 $x$。

小明从第 $0$ 层的某个房间出发。**每一层**的过程如下：

1.  进入该层的某个房间 $cur$（第一层由输入指定，之后由上一层的上楼结果决定）。
2.  记下该房间指示牌上的数字 $x = sign_{i,cur}$，**将其累加到密钥**。
3.  从 $cur$ 房间开始（**包含自身**），沿逆时针方向（编号递增，模 $M$）寻找**第 $x$ 个有楼梯的房间**（编号记为 $k$）。
4.  从房间 $k$ 的楼梯上到**上一层**的 $k$ 号房间，重复步骤 1–3。
5.  到达顶层（第 $N$ 层）后停止。

**密钥** = 每层第一个进入的房间内指示牌上 $x$ 的总和 $\bmod 20123$。

**约束**：$N\le 10^4$，$M\le 100$，$x\le 10^6$。房间总数 $N\cdot M\le 10^6$。

---

## 2. Chain-of-thought with full proofs

### 2.1 朴素模拟的问题

最直接的思路：对每一层，从当前房间开始，一个房间一个房间地检查是否有楼梯，数到第 $x$ 个停止。每层最坏走 $M$ 步，总体 $O(NM)=10^6$ —— 看起来不大。但 $x$ 的最大值是 $10^6$，如果 $x\gg M$，则需要绕很多圈，实际复杂度会退化到 $O(N\cdot\min(x,M)) \approx 10^{10}$，超时。

因此关键优化是：**避免 $x$ 很大时的无效绕圈**。

### 2.2 模运算降次

设第 $i$ 层有楼梯的房间总数为 $cnt_i$。从当前房间开始，沿逆时针方向，每经过 $cnt_i$ 个有楼梯的房间恰好回到当前房间（因为房间排布是循环的，楼梯的分布也是循环的）。于是：

> 找「第 $x$ 个有楼梯的房间」等价于找「第 $((x-1)\bmod cnt_i) + 1$ 个有楼梯的房间」。

**证明**：设 $S = cnt_i$。令 $f(p)$ 为从当前房间逆时针方向第 $p$ 个有楼梯的房间编号。显然 $f(p+S) = f(p)$（绕一整圈后回到相同的相对位置）。因此对于任意 $x\ge 1$，存在唯一的 $r = ((x-1)\bmod S)+1$ 使得 $f(x) = f(r)$，且 $1\le r\le S$。$\blacksquare$

由于 $S\le M\le 100$，$r$ 至多为 $100$。这样每层至多检查 $100$ 个房间，总复杂度 $O(NM) = 10^6$，完全可行。

### 2.3 算法流程

1.  **预处理**：读入 $N,M$，用数组储存每个房间的 $stair$ 和 $sign$，同时统计每层楼梯数 $cnt_i$。

2.  **逐层模拟**：
    - 维护当前房间编号 $cur$，初始为输入的房间号。
    - 对 $i = 0$ 到 $N-1$：
      1. $ans \leftarrow ans + sign_{i,cur}$。（模 $20123$）
      2. $step \leftarrow (sign_{i,cur} - 1) \bmod cnt_i + 1$。
      3. 从 $cur$ 开始，沿逆时针方向移动，每遇到一个 $stair=1$ 的房间就令 $step \leftarrow step-1$；
         当 $step=0$ 时停止，当前房间 $pos$ 即为上楼房间。
      4. $cur \leftarrow pos$，进入下一层。

3.  输出 $ans$。

### 2.4 正确性论证

- **包含当前房间**：题目明确「如果当前房间本身就有楼梯通向上层，该房间作为第一个有楼梯的房间」。算法在循环中先检查 $stair$ 后递减 $step$，因此当前房间若含楼梯会被计数。
- **逆时针顺序**：$pos = (pos+1)\bmod M$ 模拟了编号递增并绕回 $0$ 的行为。
- **取模安全**：每一步都取模 $20123$，防止溢出，与题目要求一致。
- **边界情况**：$cnt_i = 0$？题目保证每层至少有一个楼梯（否则无法上楼），因此不会除零。

### 2.5 时间复杂度

- 输入：$O(NM)$
- 模拟：每层至多走 $M$ 步，$O(NM)$
- 总复杂度 $O(NM) \le 10^6$，空间 $O(NM)$。

---

## 3. Pattern summary

| 步骤 | 要点 | 对应代码 |
|------|------|----------|
| 预处理 | 读入 $N,M$；存储每层楼梯标志、指示牌；预计算每层楼梯总数 | `stair[i][j]`, `sign[i][j]`, `cnt[i]` |
| 模降次 | $step \leftarrow (x-1)\bmod cnt_i + 1$ | `int step=(x-1)%cnt[i]+1` |
| 环形扫描 | 从当前房间开始，检查楼梯，按需移动 | `if(stair[i][pos]) step--; if(step) pos=(pos+1)%M` |
| 累加答案 | 每层第一个进入房间的指示牌数字之和 $\bmod 20123$ | `ans=(ans+sign[i][cur])%mod` |
| 楼层递进 | 上楼后房间编号不变 | `cur = pos` |

---

## 4. Real solution with notation glossary

### 符号表

| 符号 | 含义 | 范围 |
|------|------|------|
| $N$ | 除顶层外的层数 | $1\le N\le 10^4$ |
| $M$ | 每层的房间数 | $1\le M\le 100$ |
| $stair_{i,j}$ | 第 $i$ 层 $j$ 号房间是否有楼梯 | $0/1$ |
| $sign_{i,j}$ | 第 $i$ 层 $j$ 号房间的指示牌数字 | $1\le x\le 10^6$ |
| $cnt_i$ | 第 $i$ 层有楼梯的房间总数 | $1\le cnt_i\le M$ |
| $cur$ | 当前所在房间编号 | $0\le cur<M$ |
| $pos$ | 扫描指针 | $0\le pos<M$ |
| $step$ | 还需找的有楼梯房间个数 | $1\le step\le cnt_i$ |
| $ans$ | 密钥，对 $20123$ 取模 | — |

### 解法描述

1.  读入 $N,M$。
2.  对于 $i=0..N-1$，对每个 $j=0..M-1$，读入 $stair_{i,j}$ 和 $sign_{i,j}$，同时累加 $cnt_i$。
3.  读入起始房间 $cur$。
4.  令 $ans\gets 0$。
5.  对每层 $i=0..N-1$：
    - $ans \gets ans + sign_{i,cur}$。
    - 计算 $step \gets (sign_{i,cur} - 1) \bmod cnt_i + 1$。
    - 令 $pos \gets cur$。
    - 循环直到 $step=0$：
      - 若 $stair_{i,pos}=1$ 则 $step\gets step-1$。
      - 若 $step=0$ 则跳出。
      - $pos \gets (pos+1) \bmod M$。
    - $cur \gets pos$。
6.  输出 $ans$。

### 正确性

已在 §2.2 和 §2.4 中证明。

---

## 5. Example code

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10000+5};
constexpr int mxm{100+5};
constexpr int MOD{20123};

int stair[mxn][mxm];
int sign[mxn][mxm];
int cnt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int N,M;
	cin>>N>>M;
	for(int i=0;i<N;++i)
		for(int j=0;j<M;++j)
		{
			cin>>stair[i][j]>>sign[i][j];
			if(stair[i][j])
				++cnt[i];
		}

	int cur;
	cin>>cur;

	int ans=0;
	for(int i=0;i<N;++i)
	{
		ans=(ans+sign[i][cur])%MOD;
		int step=(sign[i][cur]-1)%cnt[i]+1;
		int pos=cur;
		while(true)
		{
			if(stair[i][pos])
				--step;
			if(!step)
				break;
			pos=(pos+1)%M;
		}
		cur=pos;
	}

	cout<<ans<<'\n';
	return 0;
}
```
