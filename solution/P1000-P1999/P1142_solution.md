# P1142 轰炸

## 1. 重述问题

平面上有 n 个点（n ≤ 700，坐标整数，无重复点）。飞机沿直线飞行一次，问一条直线最多能覆盖多少个点（即求共线点数的最大值）。

## 2. 推理链

### 2.1 问题本质

求一条直线能穿过的最大点数。等价于：在所有可能的无三点共线假设下寻找共线点数最多的直线。n ≤ 700，O(n³) 不可行（检查每条点对直线会超时），需要 O(n² log n) 或 O(n²) 算法。

### 2.2 核心观察

一条直线由两个点唯一确定。但枚举每条点对直线是 O(n³) 的（n² 条直线 × n 个点验证）。反之，固定一个点作为原点，统计与其他点的方向：如果多个点与原点方向相同，则它们共线。

**引理 1（固定原点法）**：设直线 L 经过点 P，则 L 上所有其他点相对于 P 的方向相同。因此，对每个检测的 P，统计与其他点的方向频率，最大频率 + 1（P 自身）就是以 P 为端点的直线上的最大点数。

**证明**：如果 Q 和 R 都在过 P 的直线 L 上，则向量 PQ 与向量 PR 共线（方向相同或相反）。反之，如果 PQ 与 PR 方向相同，则 P、Q、R 共线。因此同方向即共线。 □

### 2.3 方向归一化

直接比较浮点斜率会受精度影响。更好的方法是用方向向量的最简整数表示：

- 对于两点 (x₁,y₁) 和 (x₂,y₂)，向量 (dx,dy) = (x₂-x₁, y₂-y₁)
- 计算 g = gcd(|dx|, |dy|)
- 归一化：dx' = dx/g, dy' = dy/g
- 为处理相反方向（dx,dy）和 (-dx,-dy）表示同一条直线，规定符号约定：令 dx' 为正，若 dx'=0 则 dy' 为正

这样每个方向有唯一的整数对表示，可放入 map 或 unordered_map 计数。

### 2.4 算法步骤

对每个点 i：
1. 创建 map 统计方向频率
2. 对每个点 j ≠ i：
   - 计算 dx = x[j]-x[i], dy = y[j]-y[i]
   - 归一化方向
   - map[dir]++
3. 取 map 中最大值 + 1 更新全局答案

复杂度：O(n² log C)，其中 log C 是 gcd 的复杂度。n=700 时，约 49 万次操作，完全可行。

### 2.5 正确性证明

定义 f(P) 为过点 P 且共线点数最多的直线上的点数。答案 = maxₚ f(P)。

对任意一条最优直线 L，设它经过点集 S（|S| = ans）。任取 P ∈ S，在固定 P 的迭代中，S\{P} 中所有点相对于 P 的方向相同，因此 map 中该方向计数 ≥ |S|-1 = ans-1，加 1 后 ≥ ans。因此 maxₚ f(P) ≥ ans。显然 ans 不超过全局最优，所以 maxₚ f(P) = ans。

算法正确。 □

## 3. 模式总结

- **固定原点枚举** — 每次固定一个点作为原点，统计其他点的方向
- **方向归一化** — 用 (dx/gcd, dy/gcd) 整数对精确表示方向，避免浮点误差
- **符号规范化** — 约定 dx>0 或 (dx=0 且 dy>0) 使相反方向合并
- **频率统计** — 用哈希表/有序表计数，取最大值 + 1

## 4. 真实解法

### 符号表

| 符号 | 含义 |
|------|------|
| `n` | 点数 |
| `x[i], y[i]` | 第 i 个点的坐标（0-based） |
| `g` | gcd(|dx|, |dy|) |
| `dx, dy` | 两点间的方向向量 |
| `mp` | map<pair<int,int>,int> 统计每个归一化方向的出现次数 |

### 算法步骤

1. 读入 n 和所有点坐标
2. 对每个点 i（0 ≤ i < n）：
   - 清空 mp
   - 对每个 j ≠ i：
     - dx = x[j]-x[i], dy = y[j]-y[i]
     - 若 dx<0 或 (dx==0 且 dy<0)：dx=-dx, dy=-dy（规范符号）
     - g = gcd(|dx|, |dy|)
     - dx /= g, dy /= g
     - mp[{dx,dy}]++
   - 遍历 mp 取最大值，用 ans = max(ans, maxCnt+1) 更新答案
3. 输出 ans

### 复杂度

O(n² log C)，C 为坐标范围。

注意：n=1 时答案为 1。

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{705};

int n,x[mxn],y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];

	int ans=1;
	for(int i=0;i<n;++i)
	{
		map<pair<int,int>,int>mp;
		for(int j=0;j<n;++j)
		{
			if(i==j)continue;
			int dx=x[j]-x[i];
			int dy=y[j]-y[i];
			if(dx<0||(dx==0&&dy<0))
			{
				dx=-dx;
				dy=-dy;
			}
			int g=gcd(abs(dx),abs(dy));
			dx/=g;
			dy/=g;
			mp[{dx,dy}]++;
		}
		for(auto&[_,c]:mp)
			ans=max(ans,c+1);
	}

	cout<<ans<<'\n';
	return 0;
}
```
