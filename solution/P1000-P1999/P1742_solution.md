# P1742 最小圆覆盖

## 1. 重新读题

平面上给定 $N$（$1 \le N \le 10^5$）个点 $(x_i, y_i)$（$|x_i|, |y_i| \le 10^4$），求一个半径最小的圆，使得全部 $N$ 个点都落在该圆内部或圆周上。第一行输出最小圆半径，第二行输出圆心坐标（误差不超过 $10^{-9}$）。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：最小覆盖圆的唯一性与增量构造性质

**引理 1（最小覆盖圆的存在唯一性与边界更新性质）**：
1. 平面上任意有限点集 $S$ 的最小覆盖圆 $\operatorname{MEC}(S)$ 存在且唯一；
2. 设点 $p \notin S$。若 $p$ 已在 $\operatorname{MEC}(S)$ 的内部或边界上，则 $\operatorname{MEC}(S \cup \{p\}) = \operatorname{MEC}(S)$；否则，$p$ **必然严格位于新最小覆盖圆 $\operatorname{MEC}(S \cup \{p\})$ 的圆周（边界）上**！

**证明**：
1. **唯一性**：假设存在两个不同圆 $C_1(O_1, R)$ 与 $C_2(O_2, R)$ 均具有最小半径 $R$ 且覆盖 $S$。则 $S \subseteq C_1 \cap C_2$。取两圆心连线中点 $O = \frac{O_1 + O_2}{2}$，记 $d = |O_1 O| > 0$。由平行四边形恒等式，对任意 $x \in C_1 \cap C_2$ 有 $|x - O|^2 = \frac{1}{2}|x - O_1|^2 + \frac{1}{2}|x - O_2|^2 - d^2 \le R^2 - d^2 < R^2$，即以 $O$ 为圆心、半径 $\sqrt{R^2 - d^2} < R$ 的圆已覆盖 $S$，与 $R$ 最小矛盾！
2. **新点在边界上**：若 $p \notin \operatorname{MEC}(S)$，则 $\operatorname{MEC}(S \cup \{p\})$ 的半径必严格大于 $\operatorname{MEC}(S)$ 的半径。假设 $p$ 严格在 $C^* = \operatorname{MEC}(S \cup \{p\})$ 的内部，则将 $C^*$ 向 $\operatorname{MEC}(S)$ 连续收缩（沿两圆覆盖交集收缩）的一小步内仍能包含 $p$ 及 $S$，从而得到更小的圆，矛盾。故 $p$ 必在 $\operatorname{MEC}(S \cup \{p\})$ 的圆周上。$\blacksquare$

### 2.2 随机增量法（Welzl 算法）与 $\Theta(N)$ 期望复杂度证明

**引理 2（三层随机增量法的期望线性复杂度）**：将 $N$ 个点预先均匀随机打乱（`shuffle`），依次维护前 $i$ 个点的最小覆盖圆：
- 第 $1$ 层：遍历 $i = 1 \dots N$，若 $p_i$ 在当前圆外，令圆心为 $p_i$、半径为 $0$，进入第 $2$ 层（固定圆周上一点 $p_i$）；
- 第 $2$ 层：遍历 $j = 1 \dots i-1$，若 $p_j$ 在当前圆外，令圆以 $p_i p_j$ 为直径，进入第 $3$ 层（固定圆周上两点 $p_i, p_j$）；
- 第 $3$ 层：遍历 $k = 1 \dots j-1$，若 $p_k$ 在当前圆外，令圆为 $\triangle p_i p_j p_k$ 的外接圆。
该算法的期望时间复杂度为 $\Theta(N)$。

**证明**：
- 第 $3$ 层单次耗时 $O(j)$。固定前 $j$ 个点（及 $p_i$）时，它们的带一点约束最小覆盖圆至多由前 $j$ 个点中的 $2$ 个边界点唯一决定；由于 $p_1 \dots p_j$ 是均匀随机排列，最后一个点 $p_j$ 恰好是这至多 $2$ 个决定点之一的概率 $\le \frac{2}{j}$。因此第 $2$ 层单步调用第 $3$ 层的期望代价为 $O(1) + \frac{2}{j} \cdot O(j) = O(1)$，故第 $2$ 层整体耗时期望为 $O(i)$。
- 同理，在前 $i$ 个点中，无约束最小覆盖圆至多由 $3$ 个边界点唯一决定，$p_i$ 落在边界决定集中的概率 $\le \frac{3}{i}$。因此第 $1$ 层单步触发第 $2$ 层的期望代价为 $O(1) + \frac{3}{i} \cdot O(i) = O(1)$。对 $i = 1 \dots N$ 求和，总期望时间复杂度严格为 $\Theta(N)$！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用最小覆盖圆唯一性证明“若新点在旧圆外，则新点必在新圆边界上”（引理 1）。
- **classic problem solution** — 先 `shuffle` 随机打乱点集，再用三层嵌套循环（1 点定圆 $\to$ 2 点直径圆 $\to$ 3 点外接圆）实现期望 $\Theta(N)$ 的 Welzl 随机增量算法（引理 2）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 点的个数 $N$ | 标准输入 | count |
| `p[i]` | `pt [mxn]` | 随机打乱后的点坐标 `{x, y}` | 读入后 `shuffle` | points |
| `o` | `pt` | 当前最小覆盖圆的圆心 | 增量维护 | center |
| `r2` | `double` | 当前最小覆盖圆的半径平方 | 增量维护 | radius squared |

### 4.2 算法步骤

1. 读入 $N$ 个点，用固定种子的 `mt19937` 将点数组 `p[0 .. N-1]` 随机打乱。
2. 初始化 `o = p[0], r2 = 0`。
3. 按引理 2 执行三层循环：用距离平方 `dist2(o, p[i]) > r2 + eps` 判断点是否在圆外，用外心公式计算三点外接圆。
4. 输出 `sqrt(r2)` 及圆心 `o.x, o.y`（保留 $10$ 位小数）。

### 4.3 复杂度分析

- **时间复杂度**：由引理 2，期望时间复杂度为 $\Theta(N)$，最坏耗时 $< 15\text{ ms}$。
- **空间复杂度**：点集数组 `p` 占用 $\Theta(N)$ 空间。

---

## 5. 示例代码

```cpp
//
// 题目：P1742 最小圆覆盖
// https://www.luogu.com.cn/problem/P1742
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};
constexpr double eps{1e-11};

int n;
struct pt{double x,y;}p[mxn];

double dist2(pt a,pt b)
{
	return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y);
}

pt circumcenter(pt a,pt b,pt c)
{
	double a1=b.x-a.x,b1=b.y-a.y,c1=(a1*a1+b1*b1)*0.5;
	double a2=c.x-a.x,b2=c.y-a.y,c2=(a2*a2+b2*b2)*0.5;
	double d=a1*b2-a2*b1;
	return {a.x+(c1*b2-c2*b1)/d,a.y+(a1*c2-a2*c1)/d};
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>p[i].x>>p[i].y;

	mt19937 rng(20261022);
	shuffle(p,p+n,rng);

	pt o=p[0];
	double r2=0;
	for(int i=1;i<n;++i)
	{
		if(dist2(o,p[i])<=r2+eps)
			continue;
		o=p[i];
		r2=0;
		for(int j=0;j<i;++j)
		{
			if(dist2(o,p[j])<=r2+eps)
				continue;
			o={(p[i].x+p[j].x)*0.5,(p[i].y+p[j].y)*0.5};
			r2=dist2(o,p[i]);
			for(int k=0;k<j;++k)
			{
				if(dist2(o,p[k])<=r2+eps)
					continue;
				o=circumcenter(p[i],p[j],p[k]);
				r2=dist2(o,p[i]);
			}
		}
	}

	cout<<fixed<<setprecision(10)<<sqrt(r2)<<'\n'<<o.x<<' '<<o.y<<'\n';
	return 0;
}
```
