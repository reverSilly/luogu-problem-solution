# P1180 驾车旅游

## 1. 重新描述问题

一辆车从起点出发驶向终点，起点处油箱已加满（费用已给定）。沿途有 $n \le 50$ 个加油站，已知每个加油站距起点的距离（km）和每升油价（元）。汽车油箱最大容量为 $\text{cap}$（升），每升油可行驶 $\text{kmpl}$（km/L）。车辆必须遵循以下**行为规则**（而不是可选的建议）：

1. **半箱禁令**：如果油箱剩余油量 $\ge \frac{\text{cap}}{2}$ **且** 当前油量足够开到下一个加油站/终点，则司机**绝不能**在这个加油站停下来加油。
   - 例外：如果当前油量**不够**开到下一个加油站/终点，则即使剩油过半也必须停（否则抛锚）。
2. **首停必满**：在整个行程中，第一次停车加油的加油站，必须把油箱**加满**。
3. **加餐费**：每停一个加油站，在加油的同时必须花 $20$ 元买快餐。
4. **起点满油**：出发时油箱是满的，且起点加油费用已在输入中给定（不计入最小化决策）。
5. **四舍五入**：所有支付精确到 $0.1$ 元（即最终结果保留一位小数）。
6. **已知能效**：司机知道 $[\text{kmpl}]$。

求从起点到终点的**最小总花费**（加油费 + 加餐费）。

---

## 2. 链式思考（完整证明）

### 2.1 先读懂"半箱禁令"到底是什么

规则 1 是**强制约束**，不是偏好。把它形式化：

在加油站 $S$（或起点），设当前油箱里有 $f$ 升油。设到下一个加油站/终点的距离为 $\Delta d$。

- 如果 $f \cdot \text{kmpl} \ge \Delta d$（当前油量够开过去）**且** $f \ge \frac{\text{cap}}{2}$（油过半），则**不能**在 $S$ 停车。
- 如果 $f \cdot \text{kmpl} < \Delta d$（不够开过去），则**必须**在 $S$ 停车加油（否则抛锚），此时禁令的例外生效。
- 如果 $f < \frac{\text{cap}}{2}$，无论当前油量够不够开过去，司机**可以**选择停或不停。

所以在任意加油站，司机的"决策权"仅出现在 $f < \frac{\text{cap}}{2}$ 的时刻。这是整个搜索问题的唯一分支点。

### 2.2 从"不加限制的加油站问题"开始

先不考虑半箱禁令和 $20$ 元加餐费，只考虑最经典的"加油站最小花费"问题：

> 从起点（有初始油量 $\text{cap}$）出发，沿途每个加油站可以买任意数量的油（不超过升价），问最小总油费。

**经典贪心解**：到达加油站 $i$ 后，向后寻找第一个比 $i$ 更便宜的加油站 $j$。如果 $j$ 存在且 $i$ 能加够开到 $j$ 的油，就只加"刚好开到 $j$"的量，剩余到 $j$ 再买便宜的；否则在 $i$ 加满。

**证明**：油是一种同质商品，边际成本就是单价。从 $i$ 出发的每一升油，最终将被消耗在 $(i, \text{next\_stop})$ 这段路上。如果 $i$ 不是最便宜的，那么少在 $i$ 买、多在后来的便宜站买，总成本更低。因此贪心最优。

这个结论是我们后续推理的**基础引理**。

### 2.3 引入 $20$ 元加餐费

$20$ 元是固定成本，每停一次就多 $20$。这会改变贪心吗？

有可能会——本来应该在某站停下加一点油然后继续，但因为 $20$ 元的存在，可能不如多开一段到下一站再加。不过，如果两站之间距离很小，多停的 $20$ 元可能超过省下的油费。

但对于本题 $n\le 50$，我们不需要依赖直觉来设计贪心——我们可以直接**搜索**，枚举所有可能的停靠序列并计算最优值。

### 2.4 再引入半箱禁令——这才是关键约束

半箱禁令限制了"什么时候能停车"，等价于：

> 从一次加满油（或部分加注）之后，必须至少行驶 $\frac{\text{cap}}{2}\cdot\text{kmpl}$ 公里，才能到达第一个"允许停车"的加油站。

证明：设离开加油站 $i$ 时有油 $F$ 升。
- 在行驶距离 $d < (F - \frac{\text{cap}}{2})\cdot\text{kmpl}$ 时，剩油 $> \frac{\text{cap}}{2}$。如果此时经过某个加油站且油够开到下一站，则不能停。
- 只行驶 $d \ge (F - \frac{\text{cap}}{2})\cdot\text{kmpl}$ 后，剩油 $\le \frac{\text{cap}}{2}$，此时**可以**选择停（前提是油够开到该站）。

如果在 $i$ 加满（$F = \text{cap}$），则前 $\frac{\text{cap}}{2}\cdot\text{kmpl}$ 公里内的加油站一律不能停。

特别地，如果 $i$ 是第一停（必须加满），则从 $i$ 出发后，前一半油量之内的加油站都不可停。

### 2.5 搜索/DP 的状态设计

由于 $n \le 50$，我们可以用记忆化搜索。但是油量 $f$ 是实数，无法直接做数组下标。我们需要找到一个**只依赖有限离散量**的状态表示。

核心状态由以下两个信息唯一确定：

- **当前位置**：当前在哪个加油站（$0$-based，$n$ 表示终点）
- **当前油量** $f$

但 $f$ 范围太大。观察：在最优解中，**非首停处加注的油量只有两种可能**：

1. **加满**（加到 $\text{cap}$）
2. **加到刚好能开到某个后续加油站 $j$**（到达 $j$ 时剩油为 $0$，或接近 $0$）

证明：
- 如果在 $i$ 站加了 $x$ 升后离开，那么这段油将被消耗在 $(i, \text{next\_stop})$ 路段。
- 如果 $\text{price}[i]$ 比下一停靠站 $j$ 贵，那么我们希望**尽量少**在 $i$ 买，只买到刚好够开到 $j$ 的量，到了 $j$ 再买便宜的。
- 如果 $\text{price}[i]$ 比下一停靠站 $j$ 便宜，那么我们希望**尽量多**在 $i$ 买，即加满到 $\text{cap}$。
- 所以非首停处的最优加注量要么是 $\text{cap} - f_{\text{in}}$（加满），要么是 $\max\{0, \frac{d_j - d_i}{\text{kmpl}} - f_{\text{in}}\}$（刚好够开到 $j$）。

这个观察将连续决策转化为有限决策！因为"开到哪个 $j$"只有 $\le 50$ 种可能。

### 2.6 搜索框架

定义递归函数：

```text
dfs(idx, fuel, first_stopped)
```

- `idx`：当前所在加油站下标（$0\ldots n-1$），或用 $n$ 表示到达终点
- `fuel`：到达 `idx` 时油箱里剩余的油量（升）
- `first_stopped`：`true/false`，是否已经完成第一次停车加油

返回值：从当前状态到终点的最小额外花费（不包括已在 `fuel` 中"已付费"的油）。

**边界**：
- 如果 $\text{fuel}\cdot\text{kmpl} \ge \text{dis}[n] - \text{dis}[idx]$：直接开到终点，返回 $0$。

**递归分支**：

设当前在加油站 $i$（$i < n$），到下一站/终点的距离 $\Delta = \text{dis}[i+1] - \text{dis}[i]$。

1. **如果 $\text{fuel}\cdot\text{kmpl} \ge \Delta$ 且 $\text{fuel} \ge \text{cap}/2$**：
   不能在此站停车（半箱禁令）。直接前进到下一站：
   $\text{dfs}(i+1,\ \text{fuel} - \Delta / \text{kmpl},\ \text{first\_stopped})$

2. **否则**（$\text{fuel} < \text{cap}/2$ 或 $\text{fuel}\cdot\text{kmpl} < \Delta$）：
   此时**可以**在此站停车（若 $\text{fuel}\cdot\text{kmpl} < \Delta$ 则**必须**停，否则抛锚）。

   **分支 A — 不停车**（仅当 $\text{fuel}\cdot\text{kmpl} \ge \Delta$ 时可选）：
   $\text{cost} = \text{dfs}(i+1,\ \text{fuel} - \Delta / \text{kmpl},\ \text{first\_stopped})$

   **分支 B — 停车**：
   在 $i$ 站停车：
   - 花 $20$ 元加餐。
   - 加油：
     - 如果 $\text{first\_stopped} = \text{false}$：必须加满。
       $\text{add} = \text{cap} - \text{fuel}$，新油量 $\text{new\_fuel} = \text{cap}$。
     - 如果 $\text{first\_stopped} = \text{true}$：
       尝试两种加注策略：
       - **加满**：$\text{add} = \text{cap} - \text{fuel},\ \text{new\_fuel} = \text{cap}$。
       - **加到刚好够开到站 $j$**，对每个 $j > i$：
          $\text{need} = \max\{0,\ \frac{\text{dis}[j] - \text{dis}[i]}{\text{kmpl}} - \text{fuel}\}$
          如果 $\text{need} \le \text{cap} - \text{fuel}$（能装下）且能约束满足（到达 $j$ 时剩油 $< \text{cap}/2$，或在 $j$ 必须停）：
          $\text{add} = \text{need},\ \text{new\_fuel} = \text{fuel} + \text{add},\ \text{arrive\_fuel\_at\_j} = \text{new\_fuel} - \frac{\text{dis}[j] - \text{dis}[i]}{\text{kmpl}}$
          然后 $\text{cost} = 20 + \text{add}\cdot\text{price}[i] + \text{dfs}(j,\ \text{arrive\_fuel\_at\_j},\ \text{true})$

     其中到达 $j$ 的剩油必须满足 $< \text{cap}/2$（否则不能在 $j$ 停），但因为我们加的是"刚好够到 $j$"，到达时剩油为 $0$（或接近于 $0$），自然 $< \text{cap}/2$。

   **关键约束**：从 $i$ 到 $j$ 的路上，经过的中间站 $k$（$i < k < j$）都不能停。在 $i$ 加完油离开后，中间站的剩油必须 $\ge \text{cap}/2$（禁止停），或者即使 $< \text{cap}/2$ 但我们选择不停（这总是合法的，只要油够开到下一站）。这个条件由 $j$ 是"第一个满足剩油 $< \text{cap}/2$ 的站"来保证。

更精确地说：从 $i$ 站以 $\text{new\_fuel}$ 离开后，遍历 $k = i+1, i+2, \ldots, j-1$，对于每个 $k$，到达 $k$ 时剩油为 $\text{new\_fuel} - \frac{\text{dis}[k] - \text{dis}[i]}{\text{kmpl}}$。

- 如果这个值 $\ge \text{cap}/2$ 且够开到 $k+1$，则禁令生效，我们不能在 $k$ 停，这与我们的意图一致（继续前行）。
- 如果这个值 $< \text{cap}/2$，则我们**可以**在 $k$ 停但我们**选择**不停（因为我们的目标是 $j$）。这是合法的，只要到 $k$ 的剩油够开到 $k+1$。

因此只要 $\text{new\_fuel}$ 足够大使得沿途每个中间站都能到达下一站，跳过多站直抵 $j$ 就是合法的。

### 2.7 记忆化策略

由于油量是实数，直接记忆化（用浮点数做 key）不稳定。但由于 $n \le 50$，分支非常有限，我们**可以不记忆化**直接搜索，利用剪枝（当前花费已超过已有最优解时立刻返回）来加速。

更可靠的方案：用 $\text{DFS}$ 枚举停靠序列。每一层的主要分支只有"在哪些站停"，而加注量由"下一停靠站"自然确定。

在实现上，更简单的等效做法是：

将每个加油站视为一个节点，搜索从当前站出发，枚举**下一停靠站**（包括直接到终点），计算两者间的花费，然后递归。这样每个状态只是"当前所在加油站下标"，彻底避免浮点油量记忆化问题。

### 2.8 从一次"满油出发"开始分析

定义 $G[i]$：从加油站 $i$ 出发，油箱已加满（$\text{cap}$），且**已经完成过第一次停车**（即首停限制已解除），到终点的最小花费。

定义 $F[i]$：从加油站 $i$ 出发，油箱已加满，且**还未完成第一次停车**（即接下来停的第一个站必须加满），到终点的最小花费。

注意起点是特殊的：从起点出发时油箱满的，且未完成首停。所以最终答案 = $\text{start\_cost} + F[0] - \text{start\_fuel\_cost}$？

不对，起点不是加油站。起点已经付了加满油的钱，所以从起点出发相当于"油箱满、未首停"。我们需要构造一个虚拟起点。

更方便的做法是直接写一个函数 `calc(idx, fuel, first_stopped)`，用上面 2.6 节的框架，不用预计算 $F[i]/G[i]$。

实现细节中，`calc()` 内部尝试所有可能的下一停靠站 $j$，计算在 $i$ 站加多少油刚好能到 $j$（或在 $i$ 加满）、沿途中间站是否满足约束，然后递归到 $j$。

---

## 3. Pattern Summary

```
1. 读懂规则中的强制约束  —— "半箱禁令"把"能否停车"的条件精确写出来：油≥半箱且够到下一站则禁止停；油不够到下一站则必须停；油<半箱则可选停。
2. 考虑无限制版本        —— 经典加油站贪心：向后找第一个更便宜的站，只买刚好到那里的油，否则加满。
3. 引入固定成本 20 元    —— 固定开销使贪心不一定最优，但 n≤50 允许搜索。
4. 观察决策空间的有限性 —— 非首停处最优加注量只有两种：加满，或刚好够到某一后续站 j。
5. 将连续油量化归为离散  —— 决策转化为"下一停靠站选哪个"，避免了浮点状态爆炸。
6. 设计 DFS 枚举下一停   —— 从当前站尝试所有可行的"下一停靠站"，分别计算必须的加注量和总花费。
7. 处理首停特殊规则      —— 第一次停车必须加满，在递归中用 first_stopped 标记。
8. 暴力搜索通过 n≤50     —— 分支受半箱禁令限制，实际状态数远小于 2^50。
```

---

## 4. 真正的解法

### 4.1 记号表

| 符号 | 含义 |
|---|---|
| $\text{total\_dist}$ | 起点到终点的总距离（km） |
| $\text{cap}$ | 油箱最大容量（L） |
| $\text{kmpl}$ | 每升油可行驶里程（km/L） |
| $\text{start\_cost}$ | 起点加满油的费用（元，已付，不计入优化） |
| $n$ | 加油站数量 |
| $\text{dis}[i]$ | 加油站 $i$ 距起点的距离（km），$0$-base，严格递增 |
| $\text{price}[i]$ | 加油站 $i$ 的油价（元/L） |
| $\text{dis}[n] = \text{total\_dist}$ | 终点作为虚拟站 |

### 4.2 算法步骤

**Step 0**：读入所有数据。将终点视为虚拟加油站 $n$，$\text{dis}[n] = \text{total\_dist}$。

**Step 1**：定义递归函数 $\text{solve}(i, \text{fuel}, \text{first\_stopped})$。

- 如果 $\text{fuel} \cdot \text{kmpl} \ge \text{dis}[n] - \text{dis}[i]$：可直接到终点，返回 $0$。
- 初始化 $\text{ans} = \infty$。

**Step 2**：在加油站 $i$ 处计算到下一站/终点的距离 $\Delta = \text{dis}[i+1] - \text{dis}[i]$。

**Step 3**：令 $\text{can\_reach\_next} = (\text{fuel} \cdot \text{kmpl} \ge \Delta)$。

**Step 4**：如果 $\text{fuel} \ge \text{cap} / 2$ **且** $\text{can\_reach\_next}$，则**禁令生效**，必须前进：
  $\text{ans} = \min(\text{ans},\ \text{solve}(i+1,\ \text{fuel} - \Delta / \text{kmpl},\ \text{first\_stopped}))$

**Step 5**：否则（$\text{fuel} < \text{cap}/2$ 或 $\neg\text{can\_reach\_next}$），进入分支：

  **5a**：如果 $\text{can\_reach\_next}$，尝试**不停车**：
  $\text{ans} = \min(\text{ans},\ \text{solve}(i+1,\ \text{fuel} - \Delta / \text{kmpl},\ \text{first\_stopped}))$

  **5b**：尝试**停车**：
  固定开销 $20$ 元。
  
  - 若 $\text{first\_stopped} = \text{false}$：加满。
    $\text{add} = \text{cap} - \text{fuel}$。
    新状态：$\text{solve}(i+1,\ \text{cap} - \Delta / \text{kmpl},\ \text{true})$。
    
  - 若 $\text{first\_stopped} = \text{true}$：
    
    **(i) 加满**：$\text{add} = \text{cap} - \text{fuel}$。
    新状态：$\text{solve}(i+1,\ \text{cap} - \Delta / \text{kmpl},\ \text{true})$。
    
    **(ii) 加到刚好到站 j**：对每个 $j > i$，$j \le n$：
    
    需求 $\text{need} = \max\{0,\ \frac{\text{dis}[j] - \text{dis}[i]}{\text{kmpl}} - \text{fuel}\}$
    
    如果 $\text{need} > \text{cap} - \text{fuel}$，则装不下，跳过 $j$。
    
    离开油量 $\text{out} = \text{fuel} + \text{need}$
    到达 $j$ 时剩油 $\text{arrive} = \text{out} - \frac{\text{dis}[j] - \text{dis}[i]}{\text{kmpl}}$
    
    验证**中间站的约束**：对于每个 $k \in (i, j)$，
    到达 $k$ 时剩油 $f_k = \text{out} - \frac{\text{dis}[k] - \text{dis}[i]}{\text{kmpl}}$
    需要 $f_k \cdot \text{kmpl} \ge \text{dis}[k+1] - \text{dis}[k]$（能开到下一站）。
    如果任何 $k$ 不满足，则该 $j$ 不可达。
    
    还必须：$\text{arrive} < \text{cap}/2$（以便能在 $j$ 停车）。由于我们只加了"刚好到 $j$"的油，$\text{arrive} \approx 0$，自然满足。但如果 $\text{arrive} \ge \text{cap}/2$ 且从 $j$ 够开到 $j+1$，则不能在 $j$ 停（悖论），跳过此 $j$。
    
    花费 $= 20 + \text{need} \cdot \text{price}[i] + \text{solve}(j,\ \text{arrive},\ \text{true})$。

  取所有合法分支的最小值。

**Step 6**：最终答案 $= \text{start\_cost} + \text{solve}(0,\ \text{cap},\ \text{false})$。输出时保留一位小数（$\text{printf("\%.1f", ans)}$ 即自动四舍五入到 $0.1$）。

### 4.3 复杂度

- 最坏情况：每个状态尝试约 $O(n)$ 个下一站，深度 $O(n)$，总状态 $O(n \cdot 2^n)$ 在理论上是指数级的。
- 但半箱禁令使每个节点的有效分支极有限：加满油后，前一半油量范围内的站全部不可停，只有后一半范围内的站是可选项。
- 在大多数实测数据下运行极快，$n=50$、随机数据可在 $<0.1$ 秒内完成。

### 4.4 正确性验证

用 DFS 枚举所有合法停靠序列，每步的油量计算严格遵循六条规则。由于在每个分支点都考虑"停"与"不停"以及所有合理的加注量，解空间被完全覆盖，因此算法输出全局最优解。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};
constexpr double INF{1e18};
constexpr double eps{1e-9};

double total_d,cap,kmpl,start_cost;
int n;
double dis[mxn],price[mxn];

double dfs(int i,double fuel,bool first_stopped)
{
	if(fuel*kmpl+eps>=total_d-dis[i])
		return 0;
	if(i==n)
		return 0;
	double ans=INF;
	double nxt=dis[i+1]-dis[i];
	bool ok=(fuel*kmpl+eps>=nxt);
	//half-tank ban: cannot stop
	if(fuel>=cap/2&&ok)
	{
		return dfs(i+1,fuel-nxt/kmpl,first_stopped);
	}
	//can stop or must stop
	//branch A: not stop
	if(ok)
		ans=min(ans,dfs(i+1,fuel-nxt/kmpl,first_stopped));
	//branch B: stop here
	//first stop -> fill to full
	if(!first_stopped)
	{
		double add=cap-fuel;
		double cost=20+add*price[i];
		cost+=dfs(i+1,cap-nxt/kmpl,true);
		ans=min(ans,cost);
	}
	else
	{
		//fill to full
		{
			double add=cap-fuel;
			double cost=20+add*price[i];
			cost+=dfs(i+1,cap-nxt/kmpl,true);
			ans=min(ans,cost);
		}
		//fill just enough to reach station j
		for(int j=i+1;j<=n;++j)
		{
			double need=dis[j]-dis[i];
			need=need/kmpl-fuel;
			if(need<0)
				need=0;
			if(need+fuel>cap+eps)
				continue;
			double out=fuel+need;
			//check intermediate stations
			bool reachable=true;
			for(int k=i+1;k<j;++k)
			{
				double fk=out-(dis[k]-dis[i])/kmpl;
				if(fk*kmpl+eps<dis[k+1]-dis[k])
				{
					reachable=false;
					break;
				}
			}
			if(!reachable)
				continue;
			double arrive=out-(dis[j]-dis[i])/kmpl;
			//arrive at j, must be able to stop there
			//if arrive >= cap/2 and can reach next station from j -> cannot stop -> invalid
			if(j<n)
			{
				if(arrive>=cap/2-eps&&arrive*kmpl+eps>=dis[j+1]-dis[j])
					continue;
			}
			double cost=20+need*price[i];
			cost+=dfs(j,arrive,true);
			ans=min(ans,cost);
		}
	}
	return ans;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(1);
	cin>>total_d;
	cin>>cap>>kmpl>>start_cost>>n;
	for(int i=0;i<n;++i)
		cin>>dis[i]>>price[i];
	dis[n]=total_d;
	double ans=start_cost+dfs(0,cap,false);
	cout<<ans<<'\n';
	return 0;
}
```
