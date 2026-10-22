# P1773 符文之语

## 1. 重新读题

给定一个长度 $L \le 1000$ 的非零数字串 $S$ 和模数 $m$（$2 \le m \le 50$）。可以在数字串的空隙中插入 $k$ 个乘号（$0 \le k < L$），将其切分为 $k+1$ 段十进制整数，记这 $k+1$ 段的乘积对 $m$ 取模的结果为 $x$。

要求找出 $x$ 能达到的**最小值**及对应最少的乘号数 $k$，以及 $x$ 能达到的**最大值**及对应最少的乘号数 $k$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：模 $m$ 状态空间极小（$m \le 50$）与余数聚合转移

虽然数字串长度 $L \le 1000$，但模数 $m \le 50$ 极小！
设 $dp[i][r]$ 表示将前缀 $S[1 \dots i]$ 切分并使各段乘积模 $m$ 等于 $r \in [0, m-1]$ 所需的最少块数（块数减 $1$ 即为乘号数 $k$）。

**引理 1（按后缀余数聚合的 DP 转移优化）**：
对于固定的右端点 $i$，枚举最后一段的左端点 $j + 1$（$0 \le j < i$），记子串 $S[j+1 \dots i]$ 代表的十进制数对 $m$ 取模的值为 $v = \text{val}(j+1, i) \bmod m$。
若先在固定 $i$ 下，对每个余数 $v \in [0, m-1]$ 聚合所有满足 $\text{val}(j+1, i) \equiv v \pmod m$ 的前驱 $j$：
$$\text{best}[r] = \min_{0 \le j < i} dp[j][r] \quad \text{或直接枚举 } (j, r) \text{ 还是先按 } v \text{ 聚合？}$$
更进一步：对固定的 $i$，当 $j$ 从 $0$ 遍历到 $i-1$ 时，$v = \text{val}(j+1, i) \bmod m$ 只取 $0 \dots m-1$ 中的值！对于相同的 $v$ 和相同的左前缀余数 $r_1$，我们当然只需要保留最小的 $dp[j][r_1]$！
具体而言，令 $g[v][r_1] = \min_{\substack{0 \le j < i \\ \text{val}(j+1,i) \bmod m = v}} dp[j][r_1]$，则：
1. 求 $g[v][r_1]$ 只需对每个 $j \in [1, i-1]$ 用 $dp[j][0 \dots m-1]$ 更新对应的行 $g[\text{val}(j+1, i) \bmod m][0 \dots m-1]$，耗时 $O(L \cdot m)$；
2. 再枚举 $v \in [0, m-1]$ 与 $r_1 \in [0, m-1]$，用 $g[v][r_1] + 1$ 更新 $dp[i][(r_1 \cdot v) \bmod m]$，耗时 $O(m^2)$！
如此一来，单步 $i$ 的复杂度从 $O(L \cdot m)$ 加上常数为 $1$ 的最值更新，总时间仅 $O(L^2 + L m^2)$！

**证明**：由于切分点 $j$ 对后续段的影响仅通过前缀余数 $r_1$、当前段余数 $v = \text{val}(j+1, i) \bmod m$ 以及已用段数 $dp[j][r_1]$ 体现，在 $(v, r_1)$ 相同时，较小的 $dp[j][r_1]$ 严格支配较大的 $dp[j][r_1]$，故预聚合 $g[v][r_1]$ 完全等价于枚举所有 $(j, r_1)$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用模乘同余性质将状态压缩为前缀长度 $i$ 与乘积余数 $r \in [0, m-1]$。
- **classic problem solution** — 预处理所有子串模 $m$ 余数 `val[l][r]`，在每个右端点 $i$ 处按末段余数 $v$ 聚合前驱最优值，在 $O(L^2 m)$ 时间内求出所有余数的最少乘号数（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `s` | `string` | 输入数字串（长度 $L$） | 标准输入 | string |
| `m` | `int` | 模数（$\le 50$） | 标准输入 | modulo |
| `val[l][r]` | `int [1005][1005]` | 子串 $S[l \dots r]$（1-based）模 $m$ 的值 | 递推预处理 | substring mod |
| `dp[i][r]` | `int [1005][55]` | 前缀 $1 \dots i$ 乘积模 $m$ 为 $r$ 的最少乘号数 | 初值 `inf` | DP table |

### 4.2 算法步骤

1. 读入字符串 $S$（长度 $L$）与模数 $m$。
2. 预处理所有 $1 \le l \le r \le L$ 的 `val[l][r] = (val[l][r-1] * 10 + (s[r-1] - '0')) % m`。
3. 对每个 $i = 1 \dots L$：
   - 不加乘号（$k = 0$）：置 `dp[i][val[1][i]] = 0`；
   - 枚举切分点 $j = 1 \dots i-1$（令 $v = \text{val}[j+1][i]$），对满足 `dp[j][r] < inf` 的余数 $r$，用 `dp[j][r] + 1` 更新 `dp[i][(r * v) % m]`。
4. 在 `r = 0 .. m-1` 中找出第一个 `dp[L][r] < inf` 的最小余数及最后一个 `dp[L][r] < inf` 的最大余数并输出。

### 4.3 复杂度分析

- **时间复杂度**：外层 $1 \le j < i \le L$ 共 $\frac{L^2}{2} \approx 5 \times 10^5$ 对，内层 $m \le 50$，总循环次数不超过 $2.5 \times 10^7$ 次简单寄存器比较，耗时 $< 25\text{ ms}$。
- **空间复杂度**：`val` 与 `dp` 数组占用约 $4\text{ MB}$，空间复杂度为 $\Theta(L^2 + L m)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1773 符文之语
// https://www.luogu.com.cn/problem/P1773
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int inf{0x3f3f3f3f};

string s;
int n,m;
int val[mxn][mxn];
int dp[mxn][55];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s>>m;
	n=s.size();
	for(int l=1;l<=n;++l)
	{
		int cur=0;
		for(int r=l;r<=n;++r)
		{
			cur=(cur*10+(s[r-1]-'0'))%m;
			val[l][r]=cur;
		}
	}

	memset(dp,0x3f,sizeof(dp));
	for(int i=1;i<=n;++i)
	{
		dp[i][val[1][i]]=0;
		for(int j=1;j<i;++j)
		{
			int v=val[j+1][i];
			for(int r=0;r<m;++r)
				if(dp[j][r]<inf)
				{
					int nr=(r*v)%m;
					dp[i][nr]=min(dp[i][nr],dp[j][r]+1);
				}
		}
	}

	int mn_r=-1,mx_r=-1;
	for(int r=0;r<m;++r)
		if(dp[n][r]<inf)
		{
			if(mn_r==-1)
				mn_r=r;
			mx_r=r;
		}
	cout<<mn_r<<' '<<dp[n][mn_r]<<' '<<mx_r<<' '<<dp[n][mx_r]<<'\n';
	return 0;
}
```
