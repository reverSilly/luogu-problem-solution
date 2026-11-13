# P1728 [PA 2014] Parking

## 1. 重新读题

在一个高度固定为 $w$（$1 \le w \le 10^9$）、向右无限延伸的矩形停车场中，停放着 $n$ 辆矩形汽车（$1 \le n \le 5 \times 10^4$）。每辆车 $i$ 的初始位置由矩形 $[x_{i,1}, x_{i,2}] \times [y_{i,1}, y_{i,2}]$ 给出，目标位置由矩形 $[x'_{i,1}, x'_{i,2}] \times [y'_{i,1}, y'_{i,2}]$ 给出（输入中两端点大小关系不定，需先取 $\min, \max$ 规范化）。

汽车只能平移、不能旋转，移动过程中不能超出停车场上下边界 $[0, w]$，也不能彼此发生面积重叠。共 $t$ 组测试数据（$t \le 20$），判断每组数据中能否将所有车从初始位置移动到目标位置，输出 `TAK` 或 `NIE`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：什么时候两辆车可以在通道里互相错车（交换左右顺序）？

停车场向右无限延伸，这意味着我们总可以把右侧的车往右拉到无穷远处，在极其宽敞的空地上单独处理任意两辆相邻车的换位问题！

设第 $i$ 辆车的高度（竖直方向跨度）为 $h_i = |y_{i,2} - y_{i,1}|$。
考虑两辆车 $i$ 和 $j$：
- 如果 $h_i + h_j \le w$，我们只需将其中一辆车贴紧下边界 $[0, h_i]$，另一辆车贴紧上边界 $[w - h_j, w]$，它们在竖直方向上完全不重叠，因此可以在水平方向上自由擦肩而过！
- 反之，如果 $h_i + h_j > w$，由抽屉原理，无论这两辆车在 $[0, w]$ 内如何上下平移，它们的竖直投影区间 $[y_i, y_i + h_i]$ 与 $[y_j, y_j + h_j]$ 的长度之和大于 $w$，必然存在严格正长度的重叠！因此在任何时刻，这两辆车的水平投影区间都不能跨越彼此——也就是说，**它们的左右相对顺序在整个移动过程中绝对不能颠倒**！

### 2.2 充要条件证明与树状数组维护逆序对

**引理 1（重排可行性的充要条件）**：记每辆车 $i$ 的初始左边界为 $x_i = \min(x_{i,1}, x_{i,2})$，目标左边界为 $x'_i = \min(x'_{i,1}, x'_{i,2})$，高度为 $h_i = |y_{i,2} - y_{i,1}|$。将所有车从初始布局平移至目标布局是可行的，当且仅当不存在任何一对车 $(i, j)$ 使得
$$(x_i < x_j \text{ 且 } x'_i > x'_j) \quad \text{且} \quad h_i + h_j > w.$$

**证明**：
- **必要性**：初始时两车不重叠，若 $h_i + h_j > w$，则它们的竖直区间必有重叠，故初始水平区间不相交；由 $x_i < x_j$ 知初始时车 $i$ 严格在车 $j$ 左侧。在连续平移过程中，任一时刻两车竖直区间均重叠，水平坐标差 $x_j(t) - x_i(t)$ 是关于时间 $t$ 的连续函数且不能穿过 $0$，故终态必有 $x'_i < x'_j$。若出现 $x'_i > x'_j$ 则必然无解。
- **充分性**：若所有需交换左右相对顺序的车对 $(i, j)$ 均满足 $h_i + h_j \le w$，我们先把所有车向右拉开足夠大的水平间距（按初始 $x_i$ 从右到左依次向右平移，互不阻挡），此时所有车沿 $x$ 轴严格排成一列；随后模拟冒泡排序，每次只需交换相邻两辆目标顺序逆序的车，而该相邻逆序对必满足 $h_i + h_j \le w$，将一车贴顶、一车贴底即可无碰撞交换；排好目标相对顺序后再从左到右依次推入目标位置即可。$\blacksquare$

如何高效检验引理 1 的条件？
先将所有车按初始横坐标 $x_i$ 升序排序，得到每辆车的初始排名 $\text{pos}[i] \in \{1, \dots, n\}$。
随后按目标横坐标 $x'_i$ 从小到大依次遍历每辆车 $i$：此时所有**排在 $i$ 之前被遍历到**的车 $j$ 都满足 $x'_j < x'_i$。我们只需在这些已遍历的车 $j$ 中，查询满足 $\text{pos}[j] > \text{pos}[i]$（即初始在 $i$ 右侧、目标在 $i$ 左侧）的那些车的**最大高度** $\max h_j$，检验是否有 $\max h_j + h_i > w$，然后再把 $(\text{pos}[i], h_i)$ 插入维护后缀最大值的树状数组中！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用停车场向右无限延伸的性质消去纵向拥堵，证明两车能否交换水平相对顺序仅取决于高度和是否 $\le w$（引理 1）。
- **reduce to 2D dominance query** — 将可行性转化为：是否存在逆序对 $(\text{pos}[j] > \text{pos}[i],\, x'_j < x'_i)$ 满足 $h_i + h_j > w$。
- **classic problem solution** — 按初始坐标排序赋排名，再按目标坐标顺序扫描，用维护后缀最大值的树状数组在 $O(n \log n)$ 时间内完成判定。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, w` | `int` | 车辆数与停车场宽度 | 每组读入 | 维度 |
| `x1[i], x2[i], h[i]` | `int [mxn]` | 车 $i$ 的初始左边界、目标左边界、高度 | 读入计算 | car attributes |
| `ord1, ord2` | `int [mxn]` | 按初始坐标、目标坐标排序的车辆编号数组 | `iota` 后排序 | sorted orders |
| `pos[i]` | `int [mxn]` | 车 $i$ 在初始排序中的排名（$1 \dots n$） | `pos[ord1[r]] = r+1` | initial rank |
| `bit[idx]` | `int [mxn]` | 树状数组（反转下标维护后缀最大高度） | 全 `0` | Fenwick tree |

### 4.2 算法步骤

1. 对每组数据读入 $n, w$；读入 $n$ 个初始矩形，记录 $x_1[i] = \min(xa, xb)$，$h[i] = |ya - yb|$；再读入 $n$ 个目标矩形，记录 $x_2[i] = \min(xa, xb)$。
2. 将车辆按 $x_1$ 升序排得 `ord1`，记录每辆车的初始排名 `pos[ord1[r]] = r + 1`（$r = 0 \dots n-1$）。
3. 将车辆按 $x_2$ 升序排得 `ord2`，清空长度为 $n$ 的前缀最大值树状数组 `bit`。
4. 按 `ord2` 顺序依次取出车辆 $u$：
   - 为查询初始排名 $> \text{pos}[u]$ 的最大高度，将排名映射为反转下标 `rev = n - pos[u] + 1`；
   - 在 `bit` 中查询前缀 $[1, \text{rev} - 1]$ 的最大高度 `mx`；若 `mx + h[u] > w`，标记失败并跳出；
   - 在 `bit` 的位置 `rev` 处用 `h[u]` 更新前缀最大值。
5. 若全程合法输出 `TAK`，否则输出 `NIE`。

### 4.3 复杂度分析

- **时间复杂度**：每组数据两次排序与 $n$ 次树状数组操作，时间复杂度为 $O(n \log n)$；$t$ 组总时间复杂度为 $O(t n \log n)$。
- **空间复杂度**：线性全局数组复用，空间复杂度为 $O(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1728 [PA 2014] Parking
// https://www.luogu.com.cn/problem/P1728
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int n,w;
int x1_pos[mxn],x2_pos[mxn],h[mxn];
int ord1[mxn],ord2[mxn],pos[mxn];
int bit[mxn];

void upd(int i,int v)
{
	for(;i<=n;i+=i&-i)
		bit[i]=max(bit[i],v);
}
int qry(int i)
{
	int res=0;
	for(;i>0;i-=i&-i)
		res=max(res,bit[i]);
	return res;
}

void solve()
{
	cin>>n>>w;
	for(int i=0,xa,ya,xb,yb;i<n;++i)
	{
		cin>>xa>>ya>>xb>>yb;
		x1_pos[i]=min(xa,xb);
		h[i]=abs(ya-yb);
	}
	for(int i=0,xa,ya,xb,yb;i<n;++i)
	{
		cin>>xa>>ya>>xb>>yb;
		x2_pos[i]=min(xa,xb);
	}
	iota(ord1,ord1+n,0);
	iota(ord2,ord2+n,0);
	sort(ord1,ord1+n,[](int a,int b){return x1_pos[a]<x1_pos[b];});
	sort(ord2,ord2+n,[](int a,int b){return x2_pos[a]<x2_pos[b];});
	for(int i=0;i<n;++i)
		pos[ord1[i]]=i+1;

	fill_n(bit+1,n,0);
	bool ok=true;
	for(int i=0;i<n;++i)
	{
		int u=ord2[i];
		int rev=n-pos[u]+1;
		if(qry(rev-1)+h[u]>w)
		{
			ok=false;
			break;
		}
		upd(rev,h[u]);
	}
	cout<<(ok?"TAK\n":"NIE\n");
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
		solve();
	return 0;
}
```
