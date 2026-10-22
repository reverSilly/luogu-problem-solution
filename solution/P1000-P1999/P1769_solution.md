# P1769 淘汰赛制

## 1. 重新读题

有 $2^n$ 名选手（$1 \le n \le 10$），编号为 $0, 1, \dots, 2^n - 1$（对应输出 $1 \dots 2^n$）。比赛按满二叉树淘汰赛制进行 $n$ 轮，每轮相邻两棵子树的胜者对决。已知选手 $i$ 击败选手 $j$ 的概率百分比为 $p_{i,j}$，求最终夺冠概率最大的选手编号（并列取最小编号）。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：满二叉树半区独立性与位运算异或定位对手

**引理 1（淘汰赛轮次独立性与位异或对手区间定理）**：
将选手从 $0$ 到 $2^n - 1$ 编号。在第 $r$ 轮（$r = 0, 1, \dots, n-1$）中：
1. 选手 $i$ 所在的左/右半区大小为 $2^r$，他在第 $r$ 轮可能遇到的对手集合恰为将 $i$ 的第 $r$ 个二进制位翻转（$i \oplus 2^r$）后所在的连续长度为 $2^r$ 的块：
   $$\operatorname{opp}(r, i) = \big\{ j \;\big|\; \lfloor j / 2^r \rfloor == \lfloor i / 2^r \rfloor \oplus 1 \big\};$$
2. 由于左半区内部前 $r-1$ 轮的比赛结果与右半区内部前 $r-1$ 轮的比赛结果完全相互独立，选手 $i$ 在第 $r$ 轮胜出的概率 `dp[r+1][i]` 精确等于：
   $$\text{dp}[r+1][i] = \text{dp}[r][i] \cdot \sum_{j \in \operatorname{opp}(r, i)} \text{dp}[r][j] \cdot \frac{p_{i, j}}{100}.$$

**证明**：两个互不相交的比赛子树在汇合前没有公共比赛，故“选手 $i$ 从自己半区出线”与“选手 $j$ 从对面半区出线”是统计独立事件，联合概率等于各自出线概率之积 $\text{dp}[r][i] \cdot \text{dp}[r][j]$。再乘以条件获胜概率 $\frac{p_{i,j}}{100}$ 并对所有互斥的对手 $j \in \operatorname{opp}(r, i)$ 求全概率和，即得第 $r$ 轮胜出概率。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将 0-based 选手编号按二进制位分块，证明第 $r$ 轮对手集合即第 $r$ 位取反的块 $\lfloor j / 2^r \rfloor = \lfloor i / 2^r \rfloor \oplus 1$。
- **classic problem solution** — 利用左右子树独立性按轮递推 `dp[r+1][i]`，总复杂度 $\Theta(n \cdot 4^n)$（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 轮数 $n$ 与总人数 $m = 2^n$ | 标准输入 | 维度 |
| `p[i][j]` | `double [1024][1024]` | 选手 $i$ 击败选手 $j$ 的概率 | 读入除以 $100$ | win probability |
| `dp[r][i]` | `double [12][1024]` | 第 $r$ 轮结束后选手 $i$ 存活的概率 | `dp[0][i] = 1.0` | survival prob |

### 4.2 算法步骤

1. 读入 $n$，令 $m = 2^n$，读入矩阵 $p[i][j]$ 并除以 $100.0$。
2. 初始化 `dp[0][0 .. m-1] = 1.0`。
3. 枚举轮次 $r = 0 \dots n-1$，枚举选手 $i = 0 \dots m-1$：
   - 令对手块起点 `base = ((i >> r) ^ 1) << r`；
   - 累加 `j = base .. base + (1 << r) - 1` 的 `dp[r][j] * p[i][j]`，乘上 `dp[r][i]` 赋给 `dp[r+1][i]`。
4. 找出 `dp[n][i]` 最大的最小下标 $i$（若 `dp[n][i] > dp[n][best] + 1e-12` 则更新），输出 `best + 1`。

### 4.3 复杂度分析

- **时间复杂度**：每轮每对对手计算一次，所有轮次中任意两人恰在他们 LCA 所在的那一轮相遇一次，故总乘法次数也可优化至 $\binom{2^n}{2}$，直接循环为 $n \cdot 2^n \cdot 2^{n-1} \le 10 \times 2^{19} \approx 5.2 \times 10^6$ 次，耗时 $< 10\text{ ms}$。
- **空间复杂度**：概率矩阵占用 $\Theta(4^n)$ 空间（约 $8\text{ MB}$）。

---

## 5. 示例代码

```cpp
//
// 题目：P1769 淘汰赛制
// https://www.luogu.com.cn/problem/P1769
// AC
#include<bits/stdc++.h>
using namespace std;

int n,m;
double p[1025][1025],dp[12][1025];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	m=1<<n;
	for(int i=0;i<m;++i)
		for(int j=0;j<m;++j)
		{
			cin>>p[i][j];
			p[i][j]*=0.01;
		}

	for(int i=0;i<m;++i)
		dp[0][i]=1.0;

	for(int r=0;r<n;++r)
	{
		int len=1<<r;
		for(int i=0;i<m;++i)
		{
			int base=((i>>r)^1)<<r;
			double s=0;
			for(int j=base;j<base+len;++j)
				s+=dp[r][j]*p[i][j];
			dp[r+1][i]=dp[r][i]*s;
		}
	}

	int best=0;
	for(int i=1;i<m;++i)
		if(dp[n][i]>dp[n][best]+1e-12)
			best=i;
	cout<<best+1<<'\n';
	return 0;
}
```
