# P1741 Diamond A&B(2)

## 1. 重新读题

一个边长为 $n$（$1 \le n \le 888$）的大菱形被划分为 $n \times n$ 个单位小菱形，经过逆时针旋转一个锐角并拉直变形为 $n \times n$ 的正方形网格后，输入由 $2n + 1$ 行给出各条网格边是否存在（`1` 表示有边，`0` 表示无边）：
- 偶数行 $0, 2, \dots, 2n$（共 $n+1$ 行）：每行 $n$ 个 $0/1$，表示第 $r$ 条水平网格线上的 $n$ 段水平边 $H[r][0 \dots n-1]$；
- 奇数行 $1, 3, \dots, 2n-1$（共 $n$ 行）：每行 $n+1$ 个 $0/1$，表示第 $r$ 行小方格两侧及中间的 $n+1$ 段竖直边 $V[r][0 \dots n]$。

求网格中有多少个**四条边界完整存在（全为 `1`）且严格内部没有任何残留边（内部水平边与竖直边全为 `0`）**的平行四边形（即变形后的轴对齐子矩形）。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：为什么不需要 $O(n^4)$ 枚举所有子矩形？

在变形后的 $n \times n$ 网格中，一个子矩形 $[r_1, r_2] \times [c_1, c_2]$（$0 \le r_1 < r_2 \le n,\ 0 \le c_1 < c_2 \le n$）合法，当且仅当：
1. **外框全 `1`**：上边 $H[r_1][c_1 \dots c_2-1]$ 全为 `1`，下边 $H[r_2][c_1 \dots c_2-1]$ 全为 `1`，左边 $V[r_1 \dots r_2-1][c_1]$ 全为 `1`，右边 $V[r_1 \dots r_2-1][c_2]$ 全为 `1`；
2. **内部全 `0`**：严格内部的所有水平边 $H[r_1+1 \dots r_2-1][c_1 \dots c_2-1]$ 全为 `0`，且严格内部的所有竖直边 $V[r_1 \dots r_2-1][c_1+1 \dots c_2-1]$ 全为 `0`。

如果暴力枚举 $(r_1, c_1, r_2, c_2)$，状态数是 $O(n^4) \approx 6 \times 10^{11}$，绝对超时。
但请注意第 2 条严苛的约束——**内部没有任何边**！

**引理 1（左上角唯一决定右下角定理）**：对于任意固定的左上角格点 $(r_1, c_1)$（$0 \le r_1, c_1 < n$），以 $(r_1, c_1)$ 为左上角的“内部无边合法子矩形”**至多只有一个**！具体而言：
- 其右边界 $c_2$ 必然是第 $r_1$ 行从列 $c_1 + 1$ 开始向右遇到的**第一条**存在的竖直边（即最小的 $c > c_1$ 使得 $V[r_1][c] = 1$）；
- 其下边界 $r_2$ 必然是第 $c_1$ 列从行 $r_1 + 1$ 开始向下遇到的**第一条**存在的水平边（即最小的 $r > r_1$ 使得 $H[r][c_1] = 1$）。

**证明**：
设 $[r_1, r_2] \times [c_1, c_2]$ 是一个合法子矩形。
- 由右边界全为 `1` 知 $V[r_1][c_2] = 1$；
- 由内部竖直边全为 `0` 知对一切 $c_1 < c < c_2$ 均有 $V[r_1][c] = 0$。
这两条合起来，意味着 $c_2$ 恰为满足 $c > c_1$ 且 $V[r_1][c] = 1$ 的**最小下标**！同理，$r_2$ 恰为满足 $r > r_1$ 且 $H[r][c_1] = 1$ 的**最小下标**。因此一旦 $(r_1, c_1)$ 给定，唯一的候选右下角 $(r_2, c_2)$ 就被唯一锁定！$\blacksquare$

### 2.2 二维前缀和 $O(1)$ 校验候选矩形

由引理 1，我们只需预处理：
1. `nxtV[r][c]`：满足 $c' > c$ 且 $V[r][c'] = 1$ 的最小列号；
2. `nxtH[r][c]`：满足 $r' > r$ 且 $H[r'][c] = 1$ 的最小行号；
3. 水平边数组 $H$ 与竖直边数组 $V$ 的二维前缀和 `sumH` 与 `sumV`。

随后枚举每个左上角 $(r_1, c_1)$，在 $O(1)$ 时间内取出唯一候选 $(r_2, c_2) = (\text{nxtH}[r_1][c_1],\, \text{nxtV}[r_1][c_1])$，再用二维前缀和在 $O(1)$ 时间内检查四条边界是否全为 `1`、内部水平边与竖直边之和是否为 `0` 即可！总时间复杂度直接从 $O(n^4)$ 降至 $\Theta(n^2)$！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用“平行四边形内部无边”推出左上角单元格 $(r_1, c_1)$ 右侧第一条竖边与下方第一条横边唯一锁定了右下角 $(r_2, c_2)$（引理 1）。
- **classic problem solution** — 预处理右侧最近竖边 `nxtV`、下方最近横边 `nxtH` 及二维前缀和 `sumH, sumV`，对每个左上角 $O(1)$ 检验唯一候选矩形，总复杂度 $\Theta(n^2)$。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 菱形网格边长 | 标准输入 | 维度 |
| `H[r][c]` | `int [mxn][mxn]` | 水平边 $(0 \le r \le n,\ 0 \le c < n)$ | 偶数行读入 | horizontal edges |
| `V[r][c]` | `int [mxn][mxn]` | 竖直边 $(0 \le r < n,\ 0 \le c \le n)$ | 奇数行读入 | vertical edges |
| `nxtH[r][c]` | `int [mxn][mxn]` | $(r, c)$ 下方第一条水平边的行号 | 倒序递推 | next horizontal |
| `nxtV[r][c]` | `int [mxn][mxn]` | $(r, c)$ 右侧第一条竖直边的列号 | 倒序递推 | next vertical |
| `sH, sV` | `int [mxn][mxn]` | $H$ 与 $V$ 的二维前缀和 | 预处理 | 2D prefix sums |

### 4.2 算法步骤

1. 读入 $n$，交替读入 $n+1$ 行水平边 `H[r][0 .. n-1]` 与 $n$ 行竖直边 `V[r][0 .. n]`。
2. 构建 `H` 与 `V` 的二维前缀和 `sH`、`sV`，并从后往前递推求出 `nxtH[r][c]` 与 `nxtV[r][c]`。
3. 枚举左上角 $0 \le r_1 < n,\ 0 \le c_1 < n$：
   - 若 `H[r1][c1] == 0` 或 `V[r1][c1] == 0`，跳过；
   - 令 `r2 = nxtH[r1][c1], c2 = nxtV[r1][c1]`；若 `r2 > n` 或 `c2 > n`，跳过；
   - 用 `rectH` 与 `rectV` 检查：上边和 $= c_2 - c_1$、下边和 $= c_2 - c_1$、左边和 $= r_2 - r_1$、右边和 $= r_2 - r_1$、内部水平边和 $= 0$、内部竖直边和 $= 0$；若全部满足则 `++ans`。
4. 输出 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：二维前缀和预处理与枚举检验均为 $\Theta(n^2)$（$n \le 888$ 时约 $8 \times 10^5$ 次 $O(1)$ 操作，耗时 $< 15\text{ ms}$）。
- **空间复杂度**：若干个 $900 \times 900$ 的整型数组，空间复杂度为 $\Theta(n^2)$（约 $19\text{ MB}$）。

---

## 5. 示例代码

```cpp
//
// 题目：P1741 Diamond A&B(2)
// https://www.luogu.com.cn/problem/P1741
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{905};

int n;
int H[mxn][mxn],V[mxn][mxn];
int nxtH[mxn][mxn],nxtV[mxn][mxn];
int sH[mxn][mxn],sV[mxn][mxn];

int rectH(int r1,int r2,int c1,int c2)
{
	if(r1>r2||c1>c2)
		return 0;
	return sH[r2+1][c2+1]-sH[r1][c2+1]-sH[r2+1][c1]+sH[r1][c1];
}
int rectV(int r1,int r2,int c1,int c2)
{
	if(r1>r2||c1>c2)
		return 0;
	return sV[r2+1][c2+1]-sV[r1][c2+1]-sV[r2+1][c1]+sV[r1][c1];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<=2*n;++i)
	{
		if(i%2==0)
		{
			int r=i/2;
			for(int c=0;c<n;++c)
				cin>>H[r][c];
		}
		else
		{
			int r=i/2;
			for(int c=0;c<=n;++c)
				cin>>V[r][c];
		}
	}

	for(int r=0;r<=n;++r)
		for(int c=0;c<n;++c)
			sH[r+1][c+1]=sH[r][c+1]+sH[r+1][c]-sH[r][c]+H[r][c];

	for(int r=0;r<n;++r)
		for(int c=0;c<=n;++c)
			sV[r+1][c+1]=sV[r][c+1]+sV[r+1][c]-sV[r][c]+V[r][c];

	for(int c=0;c<n;++c)
	{
		int last=n+1;
		for(int r=n;r>=0;--r)
		{
			nxtH[r][c]=last;
			if(H[r][c])
				last=r;
		}
	}
	for(int r=0;r<n;++r)
	{
		int last=n+1;
		for(int c=n;c>=0;--c)
		{
			nxtV[r][c]=last;
			if(V[r][c])
				last=c;
		}
	}

	int ans=0;
	for(int r1=0;r1<n;++r1)
		for(int c1=0;c1<n;++c1)
		{
			if(!H[r1][c1]||!V[r1][c1])
				continue;
			int r2=nxtH[r1][c1],c2=nxtV[r1][c1];
			if(r2>n||c2>n)
				continue;
			int w=c2-c1,h=r2-r1;
			if(rectH(r1,r1,c1,c2-1)==w&&
			   rectH(r2,r2,c1,c2-1)==w&&
			   rectV(r1,r2-1,c1,c1)==h&&
			   rectV(r1,r2-1,c2,c2)==h&&
			   rectH(r1+1,r2-1,c1,c2-1)==0&&
			   rectV(r1,r2-1,c1+1,c2-1)==0)
				++ans;
		}

	cout<<ans<<'\n';
	return 0;
}
```
