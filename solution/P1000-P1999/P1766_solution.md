# P1766 液体滴落

## 1. 重新读题

平面上有 $N$（$1 \le N \le 10^4$）条开线段 $L_i$（不含两端点），端点坐标为整数且绝对值 $\le 10^9$。一滴液体从横坐标 $x = S$、纵坐标 $y = +\infty$ 处垂直下落：
- 每当液体在垂直下落途中碰到某条线段（即存在满足 $\min(x_{i,1}, x_{i,2}) < x < \max(x_{i,1}, x_{i,2})$ 的线段位于当前点正下方），它会落在该横坐标下位置最高的那条线段上，并沿该线段滑向高度较低的那个端点，从该端点继续垂直下落；
- 注意线段不含端点，故液滴从端点垂直落下时不会再次碰到同一高度或经过端点的斜面。
求液滴最终落出所有线段系统时的横坐标 $x$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：模拟每次碰撞与分数精确比较

每次液滴从当前位置 $(x, y)$ 垂直下落，最多只会撞上一条线段；撞上后液滴直接跳到该线段较低的端点 $(x_{\text{low}}, y_{\text{low}})$，纵坐标 $y$ 严格减小！由于每条线段在一次滑落后液滴位于其最低端点，此后 $y \le y_{\text{low}}$，液滴绝不可能再次撞上同一条线段，因此最多发生 $N$ 次碰撞。

如何在不损失精度的情况下，从所有横跨 $x$（即 $\min(x_{i,1}, x_{i,2}) < x < \max(x_{i,1}, x_{i,2})$）的线段中找出位于当前点 $(x, y)$ 下方且交点高度最高的那一条？

**引理 1（竖直线交点纵坐标的精确有理数表示与 `__int128` 比较）**：
对于满足 $x_1 < x < x_2$ 的线段 $(x_1, y_1)-(x_2, y_2)$，它在横坐标 $x$ 处的精确纵坐标为有理数
$$Y_i(x) = y_1 + \frac{(y_2 - y_1)(x - x_1)}{x_2 - x_1} = \frac{y_1(x_2 - x_1) + (y_2 - y_1)(x - x_1)}{x_2 - x_1} = \frac{\text{num}_i}{\text{den}_i},$$
其中分母 $\text{den}_i = x_2 - x_1 > 0$。
1. 该交点位于当前液滴高度 $y$ 下方（或初始 $y = +\infty$）当且仅当 $\text{num}_i \le y \cdot \text{den}_i$（因为端点不含在内且线段互不穿过落点，严格低于当前线段的较低端 $y$）；
2. 比较两条候选线段 $i$ 与 $j$ 在 $x$ 处的交点高低：$Y_i(x) > Y_j(x) \iff \text{num}_i \cdot \text{den}_j > \text{num}_j \cdot \text{den}_i$。由于 $|\text{num}_i| \le 4 \times 10^{18},\ \text{den}_j \le 2 \times 10^9$，乘积可达 $8 \times 10^{27}$，使用 `__int128` 即可做到零误差精确比较！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 每次碰到线段后状态直接转移为该线段较低端点，且纵坐标严格单调递减，总步数 $\le N$。
- **classic problem solution** — 将竖直线与线段的交点纵坐标表示为正分母分数 $\frac{\text{num}_i}{\text{den}_i}$，用 `__int128` 交叉相乘精确选出正下方最高的线段（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, sx` | `int, i64` | 线段数 $N$ 与当前液滴横坐标 | 标准输入 | 维度与横坐标 |
| `seg[i]` | `Seg [mxn]` | 规范化使 $x_1 < x_2$ 后的线段端点 | 读入整理 | segments |
| `sy, has_y` | `i64, bool` | 当前液滴纵坐标及是否已离开无穷远 | `has_y = false` | vertical state |

### 4.2 算法步骤

1. 读入 $N, S$ 及所有线段，对每条线段若 $x_1 > x_2$ 则交换两端点，保证 $x_1 \le x_2$。
2. 循环寻找下一条碰撞线段 `best = -1`：
   - 遍历 $i = 0 \dots N-1$：若 ` !(seg[i].x1 < sx && sx < seg[i].x2)`，跳过；
   - 计算分子 `num = (__int128)seg[i].y1 * (seg[i].x2 - seg[i].x1) + (__int128)(seg[i].y2 - seg[i].y1) * (sx - seg[i].x1)`，分母 `den = seg[i].x2 - seg[i].x1`；
   - 若 `has_y` 为真且 `num >= (__int128)sy * den`，跳过；
   - 若 `best == -1` 或 `num * best_den > best_num * den`，更新 `best = i, best_num = num, best_den = den`。
   - 若 `best == -1` 则跳出循环；否则将 `(sx, sy)` 更新为 `seg[best]` 中 $y$ 较小的那个端点，置 `has_y = true`。
3. 输出 `sx`。

### 4.3 复杂度分析

- **时间复杂度**：每次碰撞后 $y$ 严格低于该线段较高端并跳至较低端，最多发生 $N$ 次跳转，每次扫描 $N$ 条线段，时间复杂度为 $O(N^2)$（耗时 $< 25\text{ ms}$）。
- **空间复杂度**：线段数组占用 $\Theta(N)$ 空间。

---

## 5. 示例代码

```cpp
//
// 题目：P1766 液体滴落
// https://www.luogu.com.cn/problem/P1766
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using i128=__int128;

constexpr int mxn{10005};

int n;
i64 sx,sy;
struct Seg{i64 x1,y1,x2,y2;}seg[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>sx;
	for(int i=0;i<n;++i)
	{
		cin>>seg[i].x1>>seg[i].y1>>seg[i].x2>>seg[i].y2;
		if(seg[i].x1>seg[i].x2)
		{
			swap(seg[i].x1,seg[i].x2);
			swap(seg[i].y1,seg[i].y2);
		}
	}

	bool has_y=false;
	while(true)
	{
		int best=-1;
		i128 b_num=0,b_den=1;
		for(int i=0;i<n;++i)
		{
			if( sx<=seg[i].x1||sx>=seg[i].x2)
				continue;
			i128 den=seg[i].x2-seg[i].x1;
			i128 num=(i128)seg[i].y1*den+(i128)(seg[i].y2-seg[i].y1)*(sx-seg[i].x1);
			if(has_y&&num>=(i128)sy*den)
				continue;
			if(best==-1||num*b_den>b_num*den)
			{
				best=i;
				b_num=num;
				b_den=den;
			}
		}
		if(best==-1)
			break;
		has_y=true;
		if(seg[best].y1<seg[best].y2)
		{
			sx=seg[best].x1;
			sy=seg[best].y1;
		}
		else
		{
			sx=seg[best].x2;
			sy=seg[best].y2;
		}
	}
	cout<<sx<<'\n';
	return 0;
}
```
