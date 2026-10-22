# P1761 [ICPC 2006 Tehran R] 正方形

## 1. 重新读题

有 $n$（$1 \le n \le 50$）个正方形，边长分别为 $s_1, s_2, \dots, s_n$（$1 \le s_i \le 30$）。将它们依次以 $45^\circ$ 角倾斜放入第一象限，使得每个正方形的底顶点落在 $x$ 轴上、整体不超出第一象限（左顶点横坐标 $\ge 0$），且与此前已放入的所有正方形内部不重叠。在此前提下，每个正方形与 $x$ 轴交点的横坐标尽可能小。

问全部放好后，从正上方往下俯视，哪些正方形至少有一段严格正长度的部分没有被更高的正方形遮挡？按编号升序输出。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：乘以 $\sqrt{2}$ 消除一切无理数与浮点数！

一个边长为 $s_i$ 的正方形旋转 $45^\circ$ 后，其水平对角线与竖直对角线的半长均为 $\frac{\sqrt{2}}{2} s_i$。
如果我们把整个坐标系放大 $\sqrt{2}$ 倍，那么第 $i$ 个正方形的**对角线半长就变成了纯整数 $s_i$**！
设放大 $\sqrt{2}$ 倍后，第 $i$ 个正方形的底顶点为 $(x_i, 0)$，则它的四个顶点分别为：
- 底：$(x_i, 0)$，顶：$(x_i, 2 s_i)$，左：$(x_i - s_i, s_i)$，右：$(x_i + s_i, s_i)$。

**引理 1（斜放正方形的最小合法中心坐标公式）**：
在放大 $\sqrt{2}$ 倍的坐标系下，第 $i$ 个正方形不超出 $y$ 轴且不与前 $i-1$ 个正方形重叠的最小底点横坐标为：
$$x_i = \max\Big( s_i,\ \max_{1 \le j < i} \big(x_j + 2 \min(s_i, s_j)\big) \Big).$$

**证明**：
- 不超出第一象限要求左顶点横坐标 $x_i - s_i \ge 0 \iff x_i \ge s_i$。
- 考察两个均以 $45^\circ$ 放置且底点在 $x$ 轴上的正方形 $j$ 与 $i$（$x_j < x_i$）。它们的边界均由斜率为 $\pm 1$ 的线段组成（满足 $|x - x_k| + |y - s_k| \le s_k$）。由曼哈顿球（$\ell_1$ 球）的闵可夫斯基和性质，当 $y = \min(s_i, s_j)$ 时，左正方形 $j$ 的右边界横坐标达到 $x_j + s_j - |y - s_j| = x_j + \min(s_i, s_j)$，而右正方形 $i$ 的左边界横坐标达到 $x_i - s_i + |y - s_i| = x_i - \min(s_i, s_j)$。二者内部不相交当且仅当 $x_i - \min(s_i, s_j) \ge x_j + \min(s_i, s_j) \iff x_i \ge x_j + 2\min(s_i, s_j)$。取所有约束的最大值即得最小合法整数坐标 $x_i$。$\blacksquare$

### 2.2 俯视可见区间的裁剪（附完整证明）

**引理 2（俯视可见区间裁剪法则）**：第 $i$ 个正方形在 $x$ 轴上的初始投影区间为 $[l_i, r_i] = [x_i - s_i,\, x_i + s_i]$。对于任意其它正方形 $j \ne i$：
- 若 $j < i$（在 $i$ 左侧，$x_j < x_i$）且 $s_j > s_i$：在两投影重叠区间内，由于 $x_i - x_j \ge 2 s_i$ 且 $s_j > s_i$，正方形 $j$ 的上表面高度 $2s_j - (x - x_j)$ 在重叠区恒大于 $i$ 的上表面高度 $2s_i - (x_i - x)$，故 $j$ 遮挡了 $i$ 的左半重叠部分，需令 $l_i = \max(l_i,\, x_j + s_j)$；
- 对称地，若 $j > i$（在 $i$ 右侧，$x_j > x_i$）且 $s_j > s_i$：$j$ 遮挡了 $i$ 的右半重叠部分，需令 $r_i = \min(r_i,\, x_j - s_j)$。
裁剪完毕后，正方形 $i$ 从上方可见当且仅当 $l_i < r_i$。

**证明**：
在任意横坐标 $x \in [x_i - s_i, x_i + s_i] \cap [x_j - s_j, x_j + s_j]$（不妨设 $j < i$，故 $x_j < x \le x_j + s_j$ 且 $x_i - s_i \le x < x_i$）处：
- 正方形 $j$ 在 $x$ 处的最高点位于其右上边，高度为 $y_j(x) = 2s_j - (x - x_j)$；
- 正方形 $i$ 在 $x$ 处的最高点位于其左上边，高度为 $y_i(x) = 2s_i - (x_i - x)$。
由于两正方形不重叠，有 $(x_i - s_i) + (x_j + s_j)$ 与 $x_i - x_j \ge 2\min(s_i, s_j)$：
- 若 $s_j \le s_i$，则 $x_i - x_j \ge 2 s_j$，此时 $x_j + s_j \le x_i - s_j \le x_i - s_i$（除非单点相切于 $x_j + s_j = x_i - s_i$），二者投影区间内部根本无交！
- 若 $s_j > s_i$，则 $x_i - x_j \ge 2 s_i$，从而对交集内任意 $x$，谁更高只取决于常数项：因在交点处两正方形侧面相贴且 $j$ 的中心更高，$j$ 的右上边严格位于 $i$ 的左上边之上，故重叠区间 $[x_i - s_i, x_j + s_j]$ 完全被 $j$ 遮挡！右半侧 $j > i$ 完全对称。因此，正方形 $i$ 未被任何更高正方形覆盖的部分恰为裁剪后的开区间 $(l_i, r_i)$，其长度 $> 0 \iff l_i < r_i$。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将坐标系放大 $\sqrt{2}$ 倍，使所有中心坐标与投影端点化为纯整数。
- **derive placement formula** — 由 $\ell_1$ 球不交条件推出最小横坐标 $x_i = \max(s_i,\, \max_{j<i}(x_j + 2\min(s_i, s_j)))$（引理 1）。
- **classic problem solution** — 证明只有 $s_j > s_i$ 的正方形才会产生投影重叠并遮挡 $i$，用两侧边界裁剪判定 $l_i < r_i$（引理 2）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 正方形个数 | 每组读入 | count |
| `s[i]` | `int [55]` | 第 $i$ 个正方形边长（缩放后半对角线长） | 标准输入 | side |
| `x[i]` | `int [55]` | 第 $i$ 个正方形底点横坐标 | 按引理 1 递推 | center x |

### 4.2 算法步骤

1. 循环读入 $n$（$n = 0$ 时结束），读入 $s[1 \dots n]$。
2. 依次对 $i = 1 \dots n$ 计算 $x[i] = \max\big(s[i],\, \max_{1 \le j < i}(x[j] + 2 \min(s[i], s[j]))\big)$。
3. 对每个 $i = 1 \dots n$，令 $l = x[i] - s[i],\ r = x[i] + s[i]$：
   - 对 $j = 1 \dots i-1$：若 $s[j] > s[i]$，令 $l = \max(l, x[j] + s[j])$；
   - 对 $j = i+1 \dots n$：若 $s[j] > s[i]$，令 $r = \min(r, x[j] - s[j])$；
   - 若 $l < r$，将 $i$ 加入可见列表并输出。

### 4.3 复杂度分析

- **时间复杂度**：每组数据双重循环，时间复杂度为 $\Theta(n^2)$（$n \le 50$ 时耗时 $< 1\text{ ms}$）。
- **空间复杂度**：一维数组长度 $55$，空间复杂度为 $\Theta(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1761 [ICPC 2006 Tehran R] 正方形
// https://www.luogu.com.cn/problem/P1761
// AC
#include<bits/stdc++.h>
using namespace std;

int n;
int s[55],x[55];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(cin>>n&&n)
	{
		for(int i=1;i<=n;++i)
		{
			cin>>s[i];
			x[i]=s[i];
			for(int j=1;j<i;++j)
				x[i]=max(x[i],x[j]+2*min(s[i],s[j]));
		}
		bool first=true;
		for(int i=1;i<=n;++i)
		{
			int l=x[i]-s[i],r=x[i]+s[i];
			for(int j=1;j<i;++j)
				if(s[j]>s[i])
					l=max(l,x[j]+s[j]);
			for(int j=i+1;j<=n;++j)
				if(s[j]>s[i])
					r=min(r,x[j]-s[j]);
			if(l<r)
			{
				if(!first)
					cout<<' ';
				first=false;
				cout<<i;
			}
		}
		cout<<'\n';
	}
	return 0;
}
```
