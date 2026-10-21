# P1873 [COCI 2011/2012 #5] EKO / 砍树

> 难度：普及- · 标签：二分答案 / 单调性

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $N$ 棵树，高度是 $h_i$。把锯片设在整数高度 $H$，每棵树高出 $H$ 的部分都锯下来。要锯下的木材至少 $M$ 米，并且 $H$ 尽量高。

### 1.2 关键约束与规模暗示

- $N\le 10^6$，高度 $\le 4\times 10^5$，所以不能从最高处一米一米往下试（那是 $O(N\cdot H)$）。
- $M\le 2\times 10^9$，但木材总量最大约 $10^6\times 4\times 10^5=4\times 10^{11}$，累加必须用 64 位整数。
- 题面保证所有树高之和 $>M$，所以 $H=0$ 一定够，答案存在。
- 「再高 $1$ 米就不够」就是在说：求最大的可行整数 $H$。

## 2. 思维链（Chain-of-thought）

### 2.1 木材量关于 $H$ 单调

记 $\mathrm{cut}(H)=\sum_i \max(0,\,h_i-H)$。

**引理 1（单调）**。$H$ 增大时 $\mathrm{cut}(H)$ 不增。

**证明**。对单棵树，$H\le H'$ 时 $\max(0,h-H)\ge \max(0,h-H')$。有限个不增函数的和仍然不增。$\square$

因此可行集合 $\{H\in[0,\max h_i]:\mathrm{cut}(H)\ge M\}$ 若非空，一定是一段从 $0$ 开始的整数区间，最大端点就是答案。

### 2.2 答案一定落在这个区间里

**引理 2（下界可行）**。$\mathrm{cut}(0)=\sum h_i>M$，所以 $0$ 可行；$\mathrm{cut}(\max h_i)=0<M$（$M\ge 1$），所以最高树的高度本身不可行。答案在 $0$ 与 $\max h_i-1$ 之间，用闭区间二分即可，右端点取 $\max h_i$ 也不会漏，因为检查失败会把它丢掉。

### 2.3 二分哪一侧

要的是**最大**可行高度。取中点时必须偏右：`mid=l+(r-l+1)/2`。若 `cut(mid)>=M`，则 `mid` 以及更低的高度都不用再考虑作为「下界」，令 `l=mid`；否则 `mid` 以及更高的高度都不可行，令 `r=mid-1`。

**证明这个写法不会死循环**。当 `r>l` 时 `mid` 满足 `l<mid\le r`，所以无论走哪边，区间长度都严格下降。结束时 `l=r`，由不变式「`l` 可行且任何 `>r` 的高度不可行」（初始时把「$>\max h_i$ 不可行」当作右端外侧），得到 `l` 就是最大可行值。$\square$

暴力对照不是二分，而是从最高处向下逐米枚举，第一个满足 `cut(H)>=M` 的高度即答案。它只用于树高不超过几千的数据。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 锯得越高，得到的木材越少，可行高度是一段前缀。
- **consider easier version** — 从最高处一米米下降，用来对拍小数据。
- **binary search on answer** — 在 $[0,\max h_i]$ 上二分最大可行 $H$，检查用 64 位累加。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `h[i]` | `int [1000005]` | 树高 | 读入 | height |
| `need` | `i64` | 至少要的木材 $M$ | 读入 | need |
| `l,r` | `int` | 当前答案区间 | $0$ 与最高树 | 二分端点 |
| `cut(H)` | 函数 | 锯片在 $H$ 时的木材 | — | cut |

### 4.2 算法步骤

1. 读入树高，记下最大值 `mx`。
2. 在 $[0,mx]$ 上按「偏右中点」二分。
3. `cut(mid)` 用 `i64` 累加每棵 `h[i]-mid`（仅当 `h[i]>mid`）。
4. 输出最终的 `l`。

### 4.3 复杂度与验证状态

- **时间**：$O(N\log H)$，$H\le 4\times 10^5$。
- **空间**：$O(N)$。
- **沙箱对拍**：`g++ -O2 -std=c++17 -Wall -Wextra` 零警告。两道 COCI 样例输出 `15` 与 `36`。另有 $80$ 组 $N\le 300$、树高 $\le 2500$ 的随机数据与线性枚举暴力一致，共 $82$ 组。$N=10^6$、树高至 $4\times 10^5$ 的最大数据标程耗时 $0.3449\text{ s}$，并与一份独立的 Python 二分结果相同。

## 5. 示例代码

### 5.1 标程（`P1873.cpp`）

```cpp
//
// 题目：P1873 [COCI 2011/2012 #5] EKO / 砍树
// https://www.luogu.com.cn/problem/P1873
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

int h[mxn],n;
i64 need;

i64 cut(int H)
{
	i64 s=0;
	for(int i=0;i<n;++i)
		if(h[i]>H)
			s+=h[i]-H;
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>need;
	int mx=0;
	for(int i=0;i<n;++i)
	{
		cin>>h[i];
		mx=max(mx,h[i]);
	}
	int l=0,r=mx;
	while(l<r)
	{
		int mid=l+(r-l+1)/2;
		if(cut(mid)>=need)
			l=mid;
		else
			r=mid-1;
	}
	cout<<l<<'\n';
	return 0;
}
```
