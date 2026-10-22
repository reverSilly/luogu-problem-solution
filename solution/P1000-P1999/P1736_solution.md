# P1736 创意吃鱼法

## 1. 重新读题

给定一个 $n \times m$（$1 \le n, m \le 2500$）的 $01$ 矩阵。求最大的整数 $k$，使得存在一个 $k \times k$ 的正方形子矩阵，其**主对角线**（左上到右下）或**副对角线**（右上到左下）上的元素全为 $1$，且该正方形子矩阵内的所有其余位置元素全为 $0$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：正方形递推如何拆解？

以“主对角线全为 $1$、其余全为 $0$”的正方形为例（副对角线完全对称）：
如果以 $(i, j)$ 为右下角能构成一个边长为 $k$ 的合法正方形，那么去掉它的最后一行和最后一列后，剩下的左上角 $(k-1) \times (k-1)$ 子矩阵（以 $(i-1, j-1)$ 为右下角）必然也是一个边长为 $k-1$ 的合法正方形！
反过来，要在以 $(i-1, j-1)$ 为右下角的合法正方形基础上向右下扩展一格，新增的边框部分必须满足：
1. 右下角自身 $a[i][j] = 1$；
2. $(i, j)$ 正上方连续至少有 $k - 1$ 个 $0$；
3. $(i, j)$ 正左方连续至少有 $k - 1$ 个 $0$。

### 2.2 动态规划转移方程（附完整证明）

**引理 1（对角线空心正方形的 $\Theta(nm)$ 递推公式）**：
对每个位置 $(i, j)$，定义：
- $\text{up}[i][j]$：从 $(i-1, j)$ 开始向上连续 $0$ 的个数；
- $\text{lf}[i][j]$：从 $(i, j-1)$ 开始向左连续 $0$ 的个数；
- $\text{rt}[i][j]$：从 $(i, j+1)$ 开始向右连续 $0$ 的个数；
- $dp_1[i][j]$：以 $(i, j)$ 为右下角的主对角线合法正方形最大边长；
- $dp_2[i][j]$：以 $(i, j)$ 为左下角的副对角线合法正方形最大边长。

当 $a[i][j] = 1$ 时，有：
$$dp_1[i][j] = \min\big(\{dp_1[i-1][j-1],\, \text{up}[i][j],\, \text{lf}[i][j]\}\big) + 1,$$
$$dp_2[i][j] = \min\big(\{dp_2[i-1][j+1],\, \text{up}[i][j],\, \text{rt}[i][j]\}\big) + 1;$$
当 $a[i][j] = 0$ 时，$dp_1[i][j] = dp_2[i][j] = 0$。

**证明**：
以 $(i, j)$ 为右下角的 $k \times k$ 子矩阵满足“主对角线全为 $1$、其余全为 $0$”，当且仅当：
1. 其左上 $(k-1) \times (k-1)$ 子矩阵合法（等价于 $k - 1 \le dp_1[i-1][j-1]$）；
2. 第 $j$ 列的上方 $k-1$ 个元素 $a[i-k+1 \dots i-1][j]$ 全为 $0$（等价于 $k - 1 \le \text{up}[i][j]$）；
3. 第 $i$ 行的左方 $k-1$ 个元素 $a[i][j-k+1 \dots j-1]$ 全为 $0$（等价于 $k - 1 \le \text{lf}[i][j]$）；
4. $a[i][j] = 1$。
因此最大合法的 $k - 1$ 恰好等于 $\min(dp_1[i-1][j-1],\, \text{up}[i][j],\, \text{lf}[i][j])$，加 $1$ 即得 $dp_1[i][j]$。副对角线 $dp_2[i][j]$ 同理得证。$\blacksquare$

注意空间优化：$n, m \le 2500$ 时无需开多个 $2500 \times 2500$ 的 `int` 数组，只需按行滚动维护上一行的 `up[j]`, `dp1[j]`, `dp2[j]` 以及当前行的 `lf[j]`, `rt[j]`，内存仅需数 MB！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将 $k \times k$ 对角线正方形分解为 $(k-1)\times(k-1)$ 子正方形与 L 形全 $0$ 边框。
- **classic problem solution** — 维护上方、左方、右方连续 $0$ 的长度，用滚动数组按引理 1 在 $\Theta(nm)$ 时间、$\Theta(m)$ 额外空间内递推主/副对角线最大边长。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 矩阵行数与列数 | 标准输入 | 维度 |
| `row[j]` | `int [mxn]` | 当前行读入的 $0/1$ 元素 | 逐行读入 | current row |
| `up[j]` | `int [mxn]` | 当前格上方连续 $0$ 的个数 | 滚动维护 | up zeros |
| `lf[j], rt[j]` | `int [mxn]` | 当前格左方、右方连续 $0$ 的个数 | 行内正反扫 | left / right zeros |
| `dp1[j], dp2[j]` | `int [mxn]` | 主/副对角线合法正方形边长（滚动） | 全 `0` | DP arrays |

### 4.2 算法步骤

1. 读入 $n, m$。对每一行 $i = 1 \dots n$：
2. 读入当前行 `row[1 .. m]`，从左到右算 `lf[j]`，从右到左算 `rt[j]`。
3. 用临时数组或逆序/正序更新 `dp1[j]` 与 `dp2[j]`：
   - 若 `row[j] == 1`：`ndp1[j] = min({dp1[j-1], up[j], lf[j]}) + 1`，`ndp2[j] = min({dp2[j+1], up[j], rt[j]}) + 1`，并用二者更新 `ans`；随后令 `up[j] = 0`；
   - 若 `row[j] == 0`：`ndp1[j] = ndp2[j] = 0`，`++up[j]`。
4. 将 `ndp1, ndp2` 赋回 `dp1, dp2`。全部行处理完毕后输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：每行扫描常数遍，总时间复杂度为 $\Theta(nm)$。
- **空间复杂度**：仅使用一维滚动数组，空间复杂度为 $\Theta(m)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1736 创意吃鱼法
// https://www.luogu.com.cn/problem/P1736
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2505};

int n,m;
int row_v[mxn],up[mxn],lf[mxn],rt[mxn];
int dp1[mxn],dp2[mxn],ndp1[mxn],ndp2[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	int ans=0;
	for(int i=1;i<=n;++i)
	{
		for(int j=1;j<=m;++j)
		{
			cin>>row_v[j];
			lf[j]=(j>1&&row_v[j-1]==0)?lf[j-1]+1:0;
		}
		for(int j=m;j>=1;--j)
			rt[j]=(j<m&&row_v[j+1]==0)?rt[j+1]+1:0;

		for(int j=1;j<=m;++j)
		{
			if(row_v[j]==1)
			{
				ndp1[j]=min({dp1[j-1],up[j],lf[j]})+1;
				ndp2[j]=min({dp2[j+1],up[j],rt[j]})+1;
				ans=max({ans,ndp1[j],ndp2[j]});
				up[j]=0;
			}
			else
			{
				ndp1[j]=ndp2[j]=0;
				++up[j];
			}
		}
		for(int j=1;j<=m;++j)
		{
			dp1[j]=ndp1[j];
			dp2[j]=ndp2[j];
		}
	}
	cout<<ans<<'\n';
	return 0;
}
```
