# P1745 [CERC2016] Lost Logic

## 1. 重新读题

给定 $n$（$2 \le n \le 50$）个布尔变量 $x_1, \dots, x_n$ 的三个互不相同的真值赋值 $v^1, v^2, v^3 \in \{0, 1\}^n$。

要求构造一个包含不超过 $500$ 条 2-SAT 蕴涵约束（形如 `a -> b`，其中 $a, b \in \{x_i, !x_i\}$）的列表，使得满足所有约束的真值赋值**恰好只有**给定的这三个赋值 $\{v^1, v^2, v^3\}$；若不存在这样的 2-SAT 约束集，输出 `-1`。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：什么样的解集能被 2-SAT 精确刻画？

并不是任意三个布尔向量都能成为某个 2-SAT 问题的解集！

**引理 1（2-SAT 解集的多数函数闭合性定理）**：对于任意三个布尔赋值 $v^1, v^2, v^3 \in \{0, 1\}^n$，定义逐位多数向量（Median Vector）$M \in \{0, 1\}^n$，其中 $M_i = \operatorname{maj}(v_i^1, v_i^2, v_i^3)$（即 $v_i^1, v_i^2, v_i^3$ 中出现至少 $2$ 次的那个值）。
若 $v^1, v^2, v^3$ 均满足某组 2-SAT 约束 $\mathcal{C}$，则多数向量 $M$ **也必定满足 $\mathcal{C}$**！
因此，$\{v^1, v^2, v^3\}$ 能成为某个 2-SAT 公式的精确解集，**当且仅当**多数向量 $M$ 恰好等于 $v^1, v^2, v^3$ 中的某一个！

**证明**：
- **必要性**：任取 $\mathcal{C}$ 中的一条子句 $a \to b$（等价于 $a \lor b$ 型二元子句，只禁止某一个二维赋值，例如禁止 $(a = 1, b = 0)$）。如果在多数向量 $M$ 中有 $a(M) = 1$ 且 $b(M) = 0$，则由多数函数的定义：
  - $a$ 在 $\{v^1, v^2, v^3\}$ 中至少有 $2$ 个赋值为 $1$；
  - $b$ 在 $\{v^1, v^2, v^3\}$ 中至少有 $2$ 个赋值为 $0$。
  由抽屉原理（$2 + 2 > 3$），在三个赋值 $\{v^1, v^2, v^3\}$ 中**至少存在一个赋值**同时满足 $a = 1$ 且 $b = 0$，从而违反了子句 $a \to b$，矛盾！因此 $M$ 必然满足 $\mathcal{C}$ 中的每一条子句。若解集恰好只有 $\{v^1, v^2, v^3\}$，必有 $M \in \{v^1, v^2, v^3\}$。
- **充分性（显式构造）**：不妨通过重排假设 $M = v^1$。对每个变量 $x_i$，考察它在 $(v^1, v^2, v^3)$ 中的取值列向量 $(v_i^1, v_i^2, v_i^3)$。因为 $v_i^1 = \operatorname{maj}(v_i^1, v_i^2, v_i^3)$，不可能出现 $v_i^2 = v_i^3 \ne v_i^1$！
  于是如果把基准值 $v_i^1 = 1$ 的变量 replaced 为其取反文字 $l_i = !x_i$（若 $v_i^1 = 0$ 则取 $l_i = x_i$），则在文字 $l_1, \dots, l_n$ 下，$v^1$ 对应全 $0$，而每个文字 $l_i$ 在 $(v^1, v^2, v^3)$ 下的取值模式只可能有三种：
  1. **模式 $C$**：$(0, 0, 0)$（三个赋值中 $l_i$ 恒为假）$\implies$ 加 $1$ 条自反子句 $l_i \to !l_i$ 强制 $l_i = 0$；
  2. **模式 $A$**：$(0, 1, 0)$（仅在 $v^2$ 中为真）；
  3. **模式 $B$**：$(0, 0, 1)$（仅在 $v^3$ 中为真）。
  （因为 $v^1, v^2, v^3$ 互不相同，模式 $A$ 和模式 $B$ 的文字集合均非空！）
  我们将模式 $A$ 的所有文字连成双向等价链（或单向环 + 逆向环，或者更简单：选第一个文字 $a_0$ 作为代表，对每个其余文字 $a_j$ 加两条子句 $a_0 \to a_j$ 与 $a_j \to a_0$），强制它们同真同假；同理将模式 $B$ 的所有文字与代表 $b_0$ 强制等价；最后在两个代表文字之间加一条互斥子句 $a_0 \to !b_0$（禁止 $a_0 = 1$ 且 $b_0 = 1$）！
  此时，模式 $C$ 全为 $0$，$(a_0, b_0)$ 只允许取 $(0, 0), (1, 0), (0, 1)$ 三种组合，恰好一一对应 $v^1, v^2, v^3$！且总子句数至多为 $2n + 1 \le 101 \le 500$！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用抽屉原理证明任何 2-SAT 解集对三元多数运算 $\operatorname{maj}(v^1, v^2, v^3)$ 封闭，得出有解当且仅当逐位多数向量属于给定三赋值之一（引理 1）。
- **normalize by median** — 以多数赋值 $v^{\text{med}}$ 为全 $0$ 基准定义文字 $l_i$，将所有变量归入 $(0,0,0)$、$(0,1,0)$、$(0,0,1)$ 三种等价类。
- **classic problem solution** — 固定常量类、将同类变量与代表元双向等价绑定、并对两个非常量代表元添加互斥蕴涵 $a_0 \to !b_0$，用 $\le 2n$ 条子句完成构造。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 变量个数 | 标准输入 | 维度 |
| `v[3][i]` | `int [3][55]` | 三个赋值在第 $i$ 个变量上的 $0/1$ 值 | 标准输入 | truth table |
| `med` | `int` | 等于逐位多数向量的那一行下标（$0,1,2$ 之一） | 查找确定 | median row |
| `lit(i, neg)` | 函数 | 返回变量 $i$ 在基准行取 `0` 时的文字（或其反文字） | 字符串构造 | literal string |
| `ans` | `vector<string>` | 构造出的蕴涵子句列表 | 空 | clauses |

### 4.2 算法步骤

1. 读入 $n$ 及 $3 \times n$ 的布尔矩阵 `v`。
2. 计算逐位多数向量 $M_i = (v[0][i] + v[1][i] + v[2][i] \ge 2)$，检查是否存在某一行 `med` 与 $M$ 完全相等；若不存在，输出 `-1` 并结束。
3. 将第 `med` 行交换至 `v[0]`。对每个变量 $i = 1 \dots n$，定义文字 $l_i$ 为：当 `v[0][i] == 0` 时为 `x_i`，当 `v[0][i] == 1` 时为 `!x_i`。
4. 按 `(v[1][i] != v[0][i], v[2][i] != v[0][i])` 将变量分为三组：
   - `(0, 0)` 组：加入子句 `l_i -> !l_i`；
   - `(1, 0)` 组（记为 `grpA`）：对每个 $j > 0$，加入 `l_{grpA[0]} -> l_{grpA[j]}` 与 `l_{grpA[j]} -> l_{grpA[0]}`；
   - `(0, 1)` 组（记为 `grpB`）：对每个 $j > 0$，加入 `l_{grpB[0]} -> l_{grpB[j]}` 与 `l_{grpB[j]} -> l_{grpB[0]}`。
5. 加入互斥子句 `l_{grpA[0]} -> !l_{grpB[0]}`，输出子句总数及每条子句。

### 4.3 复杂度分析

- **时间复杂度**：扫描并构造不超过 $2n$ 条子句，时间复杂度为 $\Theta(n)$。
- **空间复杂度**：存储 $\le 2n$ 条短字符串，空间复杂度为 $\Theta(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1745 [CERC2016] Lost Logic
// https://www.luogu.com.cn/problem/P1745
// AC
#include<bits/stdc++.h>
using namespace std;

int n;
int v[3][55],maj_v[55];

string get_lit(int i,bool flip)
{
	bool is_neg=(v[0][i]==1)^flip;
	return (is_neg?"!x":"x")+to_string(i);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int r=0;r<3;++r)
		for(int i=1;i<=n;++i)
			cin>>v[r][i];

	for(int i=1;i<=n;++i)
		maj_v[i]=(v[0][i]+v[1][i]+v[2][i]>=2)?1:0;

	int med=-1;
	for(int r=0;r<3;++r)
	{
		bool same=true;
		for(int i=1;i<=n;++i)
			if(v[r][i]!=maj_v[i])
				same=false;
		if(same)
			med=r;
	}
	if(med==-1)
	{
		cout<<"-1\n";
		return 0;
	}
	if(med!=0)
		for(int i=1;i<=n;++i)
			swap(v[0][i],v[med][i]);

	vector<int>grpC,grpA,grpB;
	for(int i=1;i<=n;++i)
	{
		int d1=v[1][i]^v[0][i],d2=v[2][i]^v[0][i];
		if(d1==0&&d2==0)grpC.push_back(i);
		else if(d1==1&&d2==0)grpA.push_back(i);
		else grpB.push_back(i);
	}

	vector<string>ans;
	for(int i:grpC)
		ans.push_back(get_lit(i,false)+" -> "+get_lit(i,true));
	for(size_t j=1;j<grpA.size();++j)
	{
		ans.push_back(get_lit(grpA[0],false)+" -> "+get_lit(grpA[j],false));
		ans.push_back(get_lit(grpA[j],false)+" -> "+get_lit(grpA[0],false));
	}
	for(size_t j=1;j<grpB.size();++j)
	{
		ans.push_back(get_lit(grpB[0],false)+" -> "+get_lit(grpB[j],false));
		ans.push_back(get_lit(grpB[j],false)+" -> "+get_lit(grpB[0],false));
	}
	ans.push_back(get_lit(grpA[0],false)+" -> "+get_lit(grpB[0],true));

	cout<<ans.size()<<'\n';
	for(const auto&s:ans)
		cout<<s<<'\n';
	return 0;
}
```
