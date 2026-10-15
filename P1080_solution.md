# P1080 [NOIP 2012 提高组] 国王游戏 — 完整题解

---

## 一、重述题意（Re-read the problem）

国王与 $n$ 位大臣排成一列，国王固定站在队首。国王与每位大臣左右手上各写一个正整数。对于大臣 $i$（从 $1$ 到 $n$ 编号），他获得的**金币数**定义为：

$$
\text{coin}_i = \left\lfloor \frac{\prod_{j=0}^{i-1} a_j}{b_i} \right\rfloor
$$

其中 $a_0,b_0$ 为国王手上的数，$a_j,b_j$ 为第 $j$ 位大臣手上的数。换言之，每个大臣的金币 = **他前面所有人（包括国王）左手数的乘积 ÷ 他自己的右手数，向下取整**。

国王可以任意排列大臣的顺序（国王位置不动），目标是**最小化所有大臣金币中的最大值**，即：

$$
\min_{\text{permutation } \pi}\;\max_{1\le i\le n}\;\text{coin}_{\pi(i)}
$$

输出这个最小值。

**数据范围**：$n\le 1000$，$0<a,b<10000$。答案可能远超 $10^9$，需要高精度。

---

## 二、推理链与完整证明（Chain-of-thought with full proofs）

### 2.1 直觉与切入方向

一个朴素的想法是：左手数越大的大臣应该尽量往后排（因为其左手数会乘在后面大臣的分子里），右手数越大的大臣应该尽量往前排（因为其右手数作为分母越小），但是这两个因素会相互制约。我们自然猜想最优顺序是某个**关于 $(a,b)$ 的二元比较函数**决定的。

我们尝试用**邻项交换法**（Exchange Argument）来严格推导。

### 2.2 邻项交换分析

考虑任意一种排列。假设存在**相邻**两位大臣 $i$ 和 $i+1$（设他们分别为 $P=(a_i,b_i)$ 与 $Q=(a_{i+1},b_{i+1})$），我们分析交换他们是否能使最大值不增。

记 $S$ 为排在 $P$ 之前的所有人（包括国王）左手数的乘积。那么在交换之前，$P$ 和 $Q$ 贡献的两个金币为：

$$
\begin{aligned}
x_P &= \left\lfloor \frac{S}{b_i} \right\rfloor \\[2pt]
x_Q &= \left\lfloor \frac{S\cdot a_i}{b_{i+1}} \right\rfloor
\end{aligned}
$$

交换 $P,Q$ 之后，$Q$ 在前 $P$ 在后，贡献变为：

$$
\begin{aligned}
x_Q' &= \left\lfloor \frac{S}{b_{i+1}} \right\rfloor \\[2pt]
x_P' &= \left\lfloor \frac{S\cdot a_{i+1}}{b_i} \right\rfloor
\end{aligned}
$$

由于向下取整只影响数值大小而**不影响比较关系**（我们后面会回到这一点），我们先比较去掉取整后的上界：

$$
\begin{aligned}
M_{\text{before}} &= \max\!\left(\frac{S}{b_i},\; \frac{S\cdot a_i}{b_{i+1}}\right) \\[2pt]
M_{\text{after}} &= \max\!\left(\frac{S}{b_{i+1}},\; \frac{S\cdot a_{i+1}}{b_i}\right)
\end{aligned}
$$

我们希望找出条件，使得 $M_{\text{before}} \le M_{\text{after}}$ 恒成立，从而**不交换更优**（即 $P$ 排在 $Q$ 前是局部最优的）。

### 2.3 化简比较条件

由于 $S>0$，两边同时除以 $S$ 不改变大小关系，等价于比较：

$$
\max\!\left(\frac1{b_i},\; \frac{a_i}{b_{i+1}}\right) \quad\text{vs.}\quad 
\max\!\left(\frac1{b_{i+1}},\; \frac{a_{i+1}}{b_i}\right)
$$

**关键观察**：对于任意正数，$\frac1{b_i} \le \frac{a_{i+1}}{b_i}$（因为 $a_{i+1}\ge1$），同理 $\frac1{b_{i+1}} \le \frac{a_i}{b_{i+1}}$。因此左边两个数中的较大者其实是 $\frac{a_i}{b_{i+1}}$，右边两个数中的较大者其实是 $\frac{a_{i+1}}{b_i}$。

为什么？因为 $a\ge1$，所以 $\frac{a_{i+1}}{b_i} \ge \frac1{b_i}$，$\frac{a_i}{b_{i+1}} \ge \frac1{b_{i+1}}$。所以：

$$
\begin{aligned}
M_{\text{before}} &\sim \frac{S\cdot a_i}{b_{i+1}} \\[2pt]
M_{\text{after}} &\sim \frac{S\cdot a_{i+1}}{b_i}
\end{aligned}
$$

于是不交换优于交换的条件简化为：

$$
\frac{S\cdot a_i}{b_{i+1}} \le \frac{S\cdot a_{i+1}}{b_i}
$$

两边消去 $S>0$，交叉相乘得：

$$
a_i \cdot b_i \le a_{i+1} \cdot b_{i+1}
$$

### 2.4 严谨处理向下取整

我们需要确认：当 **去掉取整** 后得到的大小关系，在 **有取整** 时是否仍然保持。具体来说，若 $S\cdot a_i / b_{i+1} \le S\cdot a_{i+1} / b_i$，则：

$$
\left\lfloor\frac{S\cdot a_i}{b_{i+1}}\right\rfloor \le 
\frac{S\cdot a_i}{b_{i+1}} \le 
\frac{S\cdot a_{i+1}}{b_i}
$$

但并不能直接推出 $\left\lfloor S\cdot a_i / b_{i+1} \right\rfloor \le \left\lfloor S\cdot a_{i+1} / b_i \right\rfloor$。不过对于本题的**邻项交换论证**，我们只需要证：若 $a_i b_i \le a_{i+1}b_{i+1}$，那么**必然存在一个最优解中 $P$ 排在 $Q$ 前面**，即交换不会使最大值变得更小。更严格的做法是：

设 $\text{val}(i)$ 表示该大臣在排列中的金币。如果 $a_i b_i > a_{i+1} b_{i+1}$，我们交换 $P,Q$，则：
- $x_Q' \le x_P$（因为 $S/b_{i+1} \le S \cdot a_i / b_{i+1}$ 即 $x_Q' \le x_Q$，而 $x_Q$ 是原来的值之一）
- $x_P' \le x_Q$（因为 $S\cdot a_{i+1} / b_i \le S \cdot a_i / b_{i+1}$ 等价于 $a_{i+1} b_{i+1} \le a_i b_i$）

所以交换后两个数分别不超过交换前两个数，从而 $\max(x_P',x_Q') \le \max(x_P,x_Q)$，整体最大值不会增加。至于其他大臣的金币只依赖于 $S$ 而不依赖 $P,Q$ 的顺序（因为 $S$ 是乘积，乘法交换律），所以不受影响。

因此，**只要存在逆序的相邻对 $(a_i,b_i)$ 与 $(a_{i+1},b_{i+1})$ 满足 $a_i b_i > a_{i+1} b_{i+1}$，交换它们不会使答案变差**。不断进行这样的交换，最终整个序列必然按 $a\cdot b$ 非降排列。

### 2.5 结论：贪心排序

最优排列方案：将所有大臣按 **左手数 × 右手数** 的值从小到大排序。

排序后，遍历计算每个大臣的金币，取最大值输出。由于中间乘积可能极大（$n=1000$，$a<10000$，乘积可达 $10000^{1000}$ 级别），必须使用**高精度**。

---

## 三、模式总结（Pattern summary）

| 要点 | 说明 |
|------|------|
| **核心技巧** | **邻项交换法**（Exchange Argument），通过比较相邻两元素的前后顺序对目标函数的影响，推导出排序键 |
| **排序键** | $a\times b$ 升序 |
| **目标函数** | 最小化最大值（minimax） |
| **高精度需求** | 中间乘积可达 $10^{4000}$ 量级，超出 64 位整数范围 |
| **取整处理** | 向下取整不影响邻项交换的比较，因为交换只让两个数中的较大者减小或不变 |

**同类问题特征**：涉及排列顺序、代价与前面元素的乘积/和有关的 minimax 问题，优先考虑邻项交换法。

---

## 四、正式解法与符号表（Real solution with notation glossary）

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 大臣人数 |
| $a_0,b_0$ | 国王左手、右手上的数 |
| $a_i,b_i$ | 第 $i$ 位大臣左手、右手上的数（$1\le i\le n$） |
| $\text{coin}_i$ | 第 $i$ 位大臣获得的金币数 |
| $\text{pref}_i$ | 前 $i$ 个人（包括国王到第 $i-1$ 位大臣）左手数的乘积 |
| $S$ | 邻项交换中，两个相邻大臣之前所有左手数的乘积 |

### 算法步骤

1. **读入** $n$，以及国王的 $(a_0,b_0)$。
2. **读入** $n$ 个大臣的 $(a_i,b_i)$ 存入数组。
3. **排序**：按 $a_i \times b_i$ 从小到大排序。
4. **初始化**高精度变量 `prod = a0`。
5. **遍历**每位大臣（按排序后的顺序）：
   - 计算 `ans = max(ans, prod / b_i)`（高精度除法）。
   - 更新 `prod = prod * a_i`（高精度乘法）。
6. **输出** `ans`。

### 复杂度分析

- **时间复杂度**：$O(n \log n + n \cdot L)$，其中 $L$ 为高精度位数（约 $4000$）。
- **空间复杂度**：$O(L)$。

---

## 五、示例代码（Example code）

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

struct Node
{
	int a,b;
} p[mxn];

int n;

// ———— 高精度模板 ————
struct BigInteger
{
	vector<int> d;
	BigInteger(i64 x=0)
	{
		do
		{
			d.push_back(x%10);
			x/=10;
		}
		while(x);
	}
	BigInteger operator*(int x) const
	{
		BigInteger res(*this);
		int carry=0;
		for(int i=0;i<(int)res.d.size();i++)
		{
			carry+=res.d[i]*x;
			res.d[i]=carry%10;
			carry/=10;
		}
		while(carry)
		{
			res.d.push_back(carry%10);
			carry/=10;
		}
		return res;
	}
	BigInteger operator/(int x) const  // 整除
	{
		BigInteger res;
		res.d.clear();
		int rem=0;
		for(int i=(int)d.size()-1;i>=0;i--)
		{
			rem=rem*10+d[i];
			res.d.push_back(rem/x);
			rem%=x;
		}
		reverse(res.d.begin(),res.d.end());
		while((int)res.d.size()>1&&res.d.back()==0)
			res.d.pop_back();
		return res;
	}
	bool operator<(const BigInteger &o) const
	{
		if(d.size()!=o.d.size())
			return d.size()<o.d.size();
		for(int i=(int)d.size()-1;i>=0;i--)
			if(d[i]!=o.d[i])
				return d[i]<o.d[i];
		return false;
	}
	void print() const
	{
		for(int i=(int)d.size()-1;i>=0;i--)
			cout<<d[i];
	}
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	int ka,kb;
	cin>>ka>>kb;  // 国王
	for(int i=0;i<n;i++)
		cin>>p[i].a>>p[i].b;

	sort(p,p+n,[](const Node &x,const Node &y)
	{
		return x.a*x.b<y.a*y.b;
	});

	BigInteger prod(ka),ans(0);

	for(int i=0;i<n;i++)
	{
		BigInteger cur=prod/p[i].b;
		if(ans<cur)
			ans=cur;
		prod=prod*p[i].a;
	}

	ans.print();
	cout<<'\n';

	return 0;
}
```

### 代码说明

- **排序**：lambda 按 $a\times b$ 升序。
- **高精度**：使用 `vector<int>` 低位存个位，实现乘法（乘一位整数）和除法（除一位整数）以及比较。
- **输入处理**：国王单独读入，大臣读入数组后排序。
- **答案更新**：每次计算当前大臣预期金币 `prod / b`，取最大值。
- **输出**：`ans.print()` 输出高精度数。
