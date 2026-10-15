# P1109 学生分组 · 题解

---

## 1. Re-read the problem | 重读题目

有 $n$ 组学生，初始每组人数 $a_i$。每次操作可从任意一组中选一个学生，将其移动到任意另一组。目标：使所有组的人数都落在区间 $[L,R]$ 内（$L\le R$）。求最少操作次数，若不可能则输出 $-1$。

**数据范围**：$1\le n\le 50$，人数为整数，$0\le L\le R$。

### 约束重新表述

- 总人数 $S=\sum a_i$ 在操作过程中**不变**（只是移动，不增不减）。
- 最终每组人数 $a_i'$ 必须满足 $L\le a_i'\le R$。
- 每次操作把一人从一个组搬到另一个组，算 1 次。
- 可把学生从任何组搬到任何组，没有相邻限制（这是和 P1031 均分纸牌的关键区别）。

### 可行性条件

显然，若 $S < nL$，即使所有组都只取 $L$ 人也不够分，不可能；
若 $S > nR$，即使所有组都取最大值 $R$ 人也装不下，不可能。
故可行的**必要条件**是

$$
nL\;\le\; S\;\le\; nR
$$

后面会证明这同时也是**充分条件**。

---

## 2. Chain-of-thought with full proofs | 思考链与完整证明

### 2.1 引理：一次操作能同时解决一个"缺"和一个"多"

观察一次操作的本质：把一个学生从组 $i$ 移到组 $j$。这对两个组的影响是：
- 组 $i$ 人数 $-1$，若 $a_i>R$ 则离目标更近一步（"多"的问题缓解）；
- 组 $j$ 人数 $+1$，若 $a_j<L$ 则离目标更近一步（"缺"的问题缓解）。

因此，**若存在一组缺人和一组多人，一次操作可以同时改善两个组**，效率最高。

### 2.2 定义缺额与超额

记

$$
\text{lack}= \sum_{i\;:\;a_i<L} (L-a_i),\qquad
\text{excess}= \sum_{i\;:\;a_i>R} (a_i-R)
$$

- $\text{lack}$：所有低于 $L$ 的组一共还需要**补充**多少人才能达到 $L$。
- $\text{excess}$：所有高于 $R$ 的组一共需要**移走**多少人才能降到 $R$。

### 2.3 下界论证

每次操作至多减少 1 单位的 $\text{lack}$（把一个人送入缺人组），同时至多减少 1 单位的 $\text{excess}$（把一个人从多人组移出）。因此：

- 若 $\text{lack}\ge \text{excess}$，至少需要 $\text{lack}$ 次操作——因为即使每次操作都同时减少 1 单位 lack 和 1 单位 excess，在 excess 归零后仍需单独填补剩余的 lack。
- 若 $\text{excess}\ge \text{lack}$，至少需要 $\text{excess}$ 次操作——对称论证。

故**下界**为 $\max(\text{lack},\,\text{excess})$。

### 2.4 构造性方案达到下界

**情形 A**：$\text{lack}\ge \text{excess}$。

1. 进行 $\text{excess}$ 次操作：每次从某个多余组（$a_i>R$）中移 1 人到某个缺人组（$a_j<L$）。此时所有多余组降至 $R$，缺人组中有一部分升至 $L$。
2. 还需填补 $\text{lack}-\text{excess}$ 个缺额。这些学生从**已经合法**（在 $[L,R]$ 内）的组中取出。由于 $S\ge nL$，这些合法组中超过 $L$ 的"冗余"总数恰好等于 $\text{lack}-\text{excess}$，因此可以全部补齐。
3. 总操作数 $=\text{excess}+(\text{lack}-\text{excess})=\text{lack}= \max(\text{lack},\text{excess})$。

**情形 B**：$\text{excess}\ge \text{lack}$。

对称地进行 $\text{lack}$ 次"多余→缺人"操作，再将剩余 $\text{excess}-\text{lack}$ 个多余学生移入合法组中（利用 $S\le nR$ 保证所有合法组有足够空位）。
总操作数 $=\text{lack}+(\text{excess}-\text{lack})=\text{excess}= \max(\text{lack},\text{excess})$。

该构造对 $[L,R]$ 内的组只进行"取走"或"放入"，且由全局可行性保证不会破坏它们的合法性。因此构造有效，且达到下界，故为**最优解**。

### 2.5 实例验证

**样例**：$n=4,\;a=[1,9,2,8],\;L=3,\;R=6$

- $\text{lack}=(3-1)+(3-2)=3$
- $\text{excess}=(9-6)+(8-6)=5$
- $\text{ans}=\max(3,5)=5$

手推过程：
1. 组1→组0：$2,8,2,8$
2. 组1→组0：$3,7,2,8$
3. 组1→组2：$3,6,3,8$
4. 组3→组2：$3,6,4,7$
5. 组3→组2：$3,6,5,6$

所有组均在 $[3,6]$，共 5 步 ✓

---

## 3. Pattern summary | 模式总结

| 维度 | 内容 |
|------|------|
| **问题类型** | 贪心 / 简易调配 |
| **核心观察** | 一次移动同时解决一个"缺"和一个"多" |
| **关键公式** | $\text{lack}=\sum\max(0,L-a_i)$，$\text{excess}=\sum\max(0,a_i-R)$，答案为 ${\max(\text{lack},\text{excess})}$ |
| **可行性** | $nL\le\sum a_i\le nR$ |
| **与均分纸牌** | P1031 有相邻限制，而本题无限制，因此只需统计总量而非扫描前缀 |
| **复杂度** | $O(n)$ 时间，$O(1)$ 额外空间 |

---

## 4. Real solution with notation glossary | 正式解法与符号说明

### 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 组数 |
| $a_i$ | 第 $i$ 组初始人数 |
| $L,R$ | 下界、上界 |
| $S$ | 总人数 $=\sum a_i$ |
| $\text{lack}$ | 总缺额 $=\sum_{i}\max(0,\,L-a_i)$ |
| $\text{excess}$ | 总超额 $=\sum_{i}\max(0,\,a_i-R)$ |
| $\text{ans}$ | 最少操作次数 |

### 算法步骤

```
1. 读入 n, a[0..n-1], L, R.
2. 计算 S = sum(a).
3. 若 S < nL 或 S > nR，输出 -1，结束.
4. 计算 lack 和 excess：
       for i in 0..n-1:
           if a[i] < L: lack += L - a[i]
           if a[i] > R: excess += a[i] - R
5. 输出 max(lack, excess).
```

### 复杂度

- 时间复杂度：$O(n)$，单遍扫描即可完成所有计算。
- 空间复杂度：$O(1)$（仅常数个变量）。

### 正确性要点

- 可行性检查是整个问题的基础，缺之则无解。
- $\text{lack}$ 和 $\text{excess}$ 分别统计了必须进入和必须离开的学生总数。
- 全局可行性 $nL\le S\le nR$ 保证了当 $\text{lack}\neq\text{excess}$ 时，$[L,R]$ 内的组可以提供缓冲，不会从合法变为非法。
- 答案只需 $\max$，不需要考虑更复杂的匹配，这是本问题与一般网络流调配问题的关键简化。

---

## 5. Example code | 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55};
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	int S=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		S+=a[i];
	}
	int L,R;
	cin>>L>>R;

	if(S<n*L||S>n*R)
	{
		cout<<-1<<'\n';
		return 0;
	}

	int lack=0,excess=0;
	for(int i=0;i<n;++i)
	{
		if(a[i]<L)
			lack+=L-a[i];
		if(a[i]>R)
			excess+=a[i]-R;
	}

	cout<<max(lack,excess)<<'\n';
	return 0;
}
```

