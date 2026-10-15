# P1023 税收与补贴问题 题解

---

## 一、发现叙事

拿到这道题，你面前摆着一张价格-销量表格，政府已经定好了它期望的售价，但自由市场下的利润分布未必让这个价位成为商家的最优选择——如果其他价位的利润更高，商家自然会偏移过去，政府的期望就落空了。

政府的武器是 **税收与补贴**：对每件商品收取固定金额（税收）或给予固定金额（补贴）。这个固定值是一个**与售价无关的常数**，它整体平移了单位利润曲线。我们的任务是：找出一整数 \(x\)（正数 = 补贴，负数 = 税收），使得在政府的期望价位上，商家的总利润不低于任何其他价位。

初看是一个优化问题——在无数可能的 \(x\) 中寻找最优解。但细细一想，问题发生了质变：

> **给定价位 \(p\) 的总利润函数**
> \[
> f(p) = (p - \text{cost} + x) \cdot s(p)
> \]
> 对任意两个价位 \(p_1, p_2\)，比较 \(f(p_1)\) 与 \(f(p_2)\) 实际上是在考察一个**关于 \(x\) 的一元一次不等式**。

于是，原问题从"寻找最优 \(x\)"退化为"寻找满足一组线性约束的 \(x\)"。这组约束的每个不等式都形如 \(x \cdot A \ge B\)，而 \(A, B\) 完全由已知数据决定。所有约束的交集是一个区间，答案就是这个区间中绝对值最小的整数。

这个"化无限为有限"的视角转换是本题的核心洞察。

---

## 二、推理链（含完整证明）

### 2.1 价格-销量函数的构造

已知若干离散点 \((p_i, s_i)\)，且：
- 相邻点之间销量线性变化；
- 超过最高价位后，每提高 1 元销量减少固定值 \(\text{dec}\)；
- 价格不低于成本；
- 销量均为整数，价格均为整数。

设已知数据为（已排序，含成本点）：
\[
(p_0, s_0),\ (p_1, s_1),\ \ldots,\ (p_{n-1}, s_{n-1})
\]
其中 \(p_0 = \text{cost}\)。

对任意整数价位 \(p\)：

- 若 \(p \le p_0\)（即 \(p = \text{cost}\)），\(s(p) = s_0\)。
- 若 \(p_i < p < p_{i+1}\)，线性插值：
  \[
  s(p) = s_i + \frac{s_{i+1} - s_i}{p_{i+1} - p_i} \cdot (p - p_i)
  \]
- 若 \(p \ge p_{n-1}\)，外推：
  \[
  s(p) = \max\!\big(0,\ s_{n-1} - \text{dec} \cdot (p - p_{n-1})\big)
  \]

所有运算在整数域中进行。

### 2.2 利润比较转化为不等式

记政府预期价位为 \(\text{exp}\)，税收/补贴额为 \(x\)（\(x>0\) 为补贴，\(x<0\) 为税收）。单位利润为：
\[
\text{unit\_profit}(p) = p - \text{cost} + x
\]

总利润：
\[
f(p) = (p - \text{cost} + x) \cdot s(p)
\]

条件：\(\text{exp}\) 处总利润最大，即对任意 \(p \ne \text{exp}\) 且 \(s(p) > 0\) 有：
\[
f(\text{exp}) \ge f(p)
\]

代入展开：
\[
(\text{exp} - \text{cost} + x) \cdot s_{\text{exp}} \ge (p - \text{cost} + x) \cdot s_p
\]

\[
x \cdot s_{\text{exp}} + (\text{exp} - \text{cost}) \cdot s_{\text{exp}} \ge x \cdot s_p + (p - \text{cost}) \cdot s_p
\]

\[
x \cdot (s_{\text{exp}} - s_p) \ge (p - \text{cost}) \cdot s_p - (\text{exp} - \text{cost}) \cdot s_{\text{exp}}
\]

记：
\[
A = s_{\text{exp}} - s_p, \qquad
B = (p - \text{cost}) \cdot s_p - (\text{exp} - \text{cost}) \cdot s_{\text{exp}}
\]

则有：
\[
x \cdot A \ge B
\]

### 2.3 分类讨论

1. **\(A > 0\)（预期价位的销量更大）**
   \[
   x \ge \frac{B}{A} \quad\Longrightarrow\quad x \ge \left\lceil \frac{B}{A} \right\rceil
   \]
   给出 **下界**（`lb`）。

2. **\(A < 0\)（该价位的销量更大）**
   \[
   x \le \frac{B}{A} \quad\text{（除以负数翻转不等号）}
   \]
   \[
   x \le \left\lfloor \frac{B}{A} \right\rfloor
   \]
   利用恒等式 \(\lfloor B/A \rfloor = -\lceil B/(-A) \rceil\)，可统一用 `ceildiv` 实现：
   \[
   x \le -\left\lceil \frac{B}{-A} \right\rceil
   \]
   给出 **上界**（`ub`）。

3. **\(A = 0\)（销量相等）**
   \[
   0 \ge B
   \]
   若 \(B > 0\)，无论如何无法满足，无解。

### 2.4 区间求交与答案选取

对所有价位 \(p\)（成本价起，至销量降至 0 止）依次处理：
- 计算 \(A, B\)；
- 根据符号更新 \(lb = \max(lb,\ \text{lower})\) 或 \(ub = \min(ub,\ \text{upper})\)；
- 若 \(A = 0 \land B > 0\)，直接输出 `NO SOLUTION`。

处理完毕后，若 \(lb > ub\)，无解；否则在 \([lb,\ ub]\) 中选取绝对值最小的整数：

\[
\text{ans} = \begin{cases}
0, & lb \le 0 \le ub \\
lb, & lb > 0 \\
ub, & ub < 0
\end{cases}
\]

**正确性证明**：区间中任意整数均满足全部约束，因而均能使预期价位取得最大利润。绝对值在 0 处取到最小；若 0 不在区间内，则最接近 0 的端点（正时最小正数 = 左端点；负时最大负数 = 右端点）即为所求。

### 2.5 哨兵值与 ceildiv 实现

由编码规范，下界初始化为极小值 \(-1145141919810\)，上界初始化为极大值 \(1145141919810\)。

`ceildiv(a, b)`（\(b > 0\)）：
\[
\text{ceildiv}(a, b) = \begin{cases}
\lfloor (a + b - 1) / b \rfloor, & a \ge 0 \\
\lfloor a / b \rfloor, & a < 0
\end{cases}
\]
C++ 整数除法向零截断的性质恰好使得负数情况的首部取整等价于向上取整。

---

## 三、模式总结

| 维度 | 特征 |
|---|------|
| **问题类型** | 线性约束满足问题 + 离散优化 |
| **核心技巧** | 将"在某点取最值"转化为两两不等式的交集 |
| **关键观察** | 税收/补贴是常数偏移，代入利润函数后呈线性 |
| **边界处理** | 销量降为 0 后不再产生有效约束；预期价位本身销量为零则直接无解 |
| **整数细节** | 不等式系数均为整数；上下界取整需自制 `ceildiv`，避免浮点误差 |
| **复杂度** | \(O(\text{max\_price} - \text{cost})\)，数据范围 < \(10^5\) |

本题本质上是**参数化的利润最大化问题**，税收/补贴作为可调参数，将最优性条件线性化为一组不等式。这种手法可推广到其他含线性参数的极值比较问题。

---

## 四、正式解法（含符号表）

### 符号说明

| 符号 | 含义 |
|---|------|
| \(\text{exp}\) | 政府预期价位 |
| \(\text{cost}\) | 成本价 |
| \(\text{dec}\) | 最高价位以上每提高 1 元减少的销量 |
| \(p_i,\ s_i\) | 已知价位及其销量（\(p_0 = \text{cost}\)） |
| \(s(p)\) | 价位 \(p\) 处的销量 |
| \(x\) | 税收/补贴金额（正=补贴，负=税收） |
| \(A = s_{\text{exp}} - s_p\) | 销量差 |
| \(B = (p - \text{cost})\cdot s_p - (\text{exp} - \text{cost})\cdot s_{\text{exp}}\) | 利润差常数项 |
| \(lb,\ ub\) | 可行 \(x\) 的下界与上界 |

### 算法步骤

1. **读入数据**：预期价位、成本与成本价销量、已知价位-销量对（至 `-1 -1`）、衰减量。
2. **建立完整价格-销量函数**：合并成本点，排序去重，实现 `calc_sales(p)`。
3. **检查预期价位有效性**：若 \(s(\text{exp}) \le 0\)，输出 `NO SOLUTION`。
4. **遍历所有可能价位**（从成本到销量降为 0 处）：
   - 计算 \(A, B\)；
   - \(A > 0\)：\(lb \gets \max(lb,\ \text{ceildiv}(B, A))\)；
   - \(A < 0\)：\(ub \gets \min(ub,\ -\text{ceildiv}(B, -A))\)；
   - \(A = 0 \land B > 0\)：无解。
5. **判断与输出**：
   - \(lb > ub\)：`NO SOLUTION`；
   - 否则按绝对值最小原则输出。

---

## 五、C++ 代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 ceildiv(i64 a,i64 b)
{
	if(a>=0)
		return (a+b-1)/b;
	return a/b;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 exp;cin>>exp;
	i64 cost,scost;cin>>cost>>scost;

	vector<pair<i64,i64>>tmp;
	tmp.push_back({cost,scost});
	while(true)
	{
		i64 p,s;cin>>p>>s;
		if(p==-1&&s==-1)
			break;
		tmp.push_back({p,s});
	}
	i64 dec;cin>>dec;

	sort(tmp.begin(),tmp.end());
	vector<pair<i64,i64>>pts;
	for(auto&pr:tmp)
		if(pts.empty()||pr.first!=pts.back().first)
			pts.push_back(pr);

	i64 n=pts.size();
	i64 last_p=pts[n-1].first;
	i64 last_s=pts[n-1].second;

	auto calc=[&](i64 p)->i64
	{
		if(p<cost)
			return 0;
		if(p<=pts[0].first)
		{
			if(pts[0].first==cost)
				return pts[0].second;
			i64 p1=cost,s1=scost;
			i64 p2=pts[0].first,s2=pts[0].second;
			return s1+(s2-s1)*(p-p1)/(p2-p1);
		}
		if(p>=last_p)
			return max(0ll,last_s-dec*(p-last_p));
		for(i64 i=0;i<n-1;++i)
		{
			if(p>=pts[i].first&&p<pts[i+1].first)
			{
				i64 p1=pts[i].first,s1=pts[i].second;
				i64 p2=pts[i+1].first,s2=pts[i+1].second;
				return s1+(s2-s1)*(p-p1)/(p2-p1);
			}
		}
		return 0;
	};

	i64 sexp=calc(exp);
	if(sexp<=0)
	{
		cout<<"NO SOLUTION\n";
		return 0;
	}

	i64 lb=-1145141919810,ub=1145141919810;
	i64 mxp=last_p+(dec>0?last_s/dec+5:100000);

	for(i64 p=cost;p<=mxp;++p)
	{
		i64 sp=calc(p);
		if(sp<=0)
		{
			if(dec>0&&p>=last_p)
				break;
			continue;
		}
		if(p==exp)
			continue;

		i64 A=sexp-sp;
		i64 B=sp*(p-cost)-sexp*(exp-cost);

		if(A>0)
			lb=max(lb,ceildiv(B,A));
		else if(A<0)
			ub=min(ub,-ceildiv(B,-A));
		else if(B>0)
		{
			cout<<"NO SOLUTION\n";
			return 0;
		}
	}

	if(lb>ub)
	{
		cout<<"NO SOLUTION\n";
		return 0;
	}

	i64 ans;
	if(lb<=0&&ub>=0)
		ans=0;
	else if(ub<0)
		ans=ub;
	else
		ans=lb;

	cout<<ans<<'\n';
	return 0;
}
```
