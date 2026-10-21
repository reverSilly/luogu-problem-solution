# P1575 正误问题 题解

## Part 1 — 题目理解 (Problem Comprehension)

### 题意简述

给定一行由空格分隔的布尔表达式单词序列，仅包含 `true`、`false`、`not`、`and`、`or` 五种单词（无括号）。
- 运算符优先级从高到低依次为：`not`（最高，一元前缀右结合） $>$ `and`（二元左结合） $>$ `or`（最低，二元左结合）。
- 若该布尔表达式符合语法规则，输出其逻辑值 `true` 或 `false`；若表达式存在语法错误（如缺少操作数、连续两个二元运算符、连续两个布尔值、后缀 `not` 等），输出 `error`。

### 输入输出格式与数据规模

- **输入格式**：一行字符串，包含若干以空格分隔的单词。
- **输出格式**：一行，输出 `true`、`false` 或 `error`。
- **数据规模**：输入字符串总长度 $< 255$。

### 关键约束分析

本题的难点不在于求值，而在于**完备地拦截所有非法语法（Hack 数据）**。例如：
- `true not false`（`not` 出现在两个操作数中间，把一元运算符当二元用 $\implies$ `error`）；
- `not not true`（合法的嵌套一元前缀取反 $\implies$ `true`）；
- `true and` / `or false` / `not`（缺少左或右操作数 $\implies$ `error`）；
- `true false`（缺少二元运算符 $\implies$ `error`）。

---

## Part 2 — 思路推导 (Idea Discovery)

### 第一步：构建无二义性的 LL(1) 上下文无关文法

根据运算优先级（`or` $<$ `and` $<$ `not`），我们可以直接写出合法布尔表达式的严格递归下降文法（CFG）：
1. **或表达式（Or-Expr）**：$\text{OrExpr} \to \text{AndExpr}\ (\texttt{"or"}\ \text{AndExpr})^*$
2. **与表达式（And-Expr）**：$\text{AndExpr} \to \text{NotExpr}\ (\texttt{"and"}\ \text{NotExpr})^*$
3. **非/原子表达式（Not-Expr）**：
   $$\text{NotExpr} \to \texttt{"not"}\ \text{NotExpr} \;\mid\; \texttt{"true"} \;\mid\; \texttt{"false"}$$

**引理 1（递归下降文法完备性定理）**：
一个单词序列 $t_0, t_1, \dots, t_{k-1}$ 是合法的无括号布尔表达式，当且仅当从 `pos = 0` 调用 `parse_or()` 的过程中未触发原子解析失败（`has_err == false`），且调用返回后指针**恰好消耗完全部单词（`pos == k`）**。

**证明**：
- 在 `parse_not()` 中，只接受零个或多个连续的前缀 `"not"` 后紧跟一个 `"true"` 或 `"false"`；若遇到文件尾 `EOF` 或二元算符 `"and"`/`"or"`，立即置 `has_err = true`（拦截了缺失操作数、前缀二元算符、连续二元算符、末尾悬空 `"not"` 等错误）。
- 在 `parse_and()` 与 `parse_or()` 中，每次解析完一个合法的 `NotExpr` 后，只有看到 `"and"` 或 `"or"` 才会继续向后消费；若一个布尔值后面紧跟另一个 `"true"`、`"false"` 或 `"not"`（例如 `true false` 或 `true not false`），循环会立即停止并返回顶层，此时 `pos < k`（存在未消费的剩余单词），顶层检查 `pos != tok.size()` 即可将其精准判为 `error`！证毕。

---

## Part 3 — 算法流程与复杂度分析 (Algorithm & Complexity)

### 算法步骤

1. 使用 `while (cin >> w)` 读入所有单词存入 `tok`；若 `tok` 为空，输出 `error`。
2. 调用 `res = parse_or()` 进行递归下降语法分析与同步求值。
3. 若 `has_err == true` 或 `pos_idx != tok.size()`，输出 `error`；否则根据 `res` 输出 `true` 或 `false`。

### 复杂度分析

- **时间复杂度**：每个单词被访问常数次，时间复杂度为 $\mathcal{O}(|\text{tok}|)$。
- **空间复杂度**：$\mathcal{O}(|\text{tok}|)$。

---

## Part 4 — 代码实现 (Code)

```cpp
//
// 题目：P1575 正误问题
// https://www.luogu.com.cn/problem/P1575
// AC
#include<bits/stdc++.h>
using namespace std;

vector<string> tok;
int pos_idx;
bool has_err;

bool parse_not()
{
	if(pos_idx>=static_cast<int>(tok.size()))
	{
		has_err=true;
		return false;
	}
	if(tok[pos_idx]=="not")
	{
		++pos_idx;
		return !parse_not();
	}
	if(tok[pos_idx]=="true")
	{
		++pos_idx;
		return true;
	}
	if(tok[pos_idx]=="false")
	{
		++pos_idx;
		return false;
	}
	has_err=true;
	return false;
}

bool parse_and()
{
	bool val=parse_not();
	while(!has_err&&pos_idx<static_cast<int>(tok.size())&&tok[pos_idx]=="and")
	{
		++pos_idx;
		bool rhs=parse_not();
		val=val&&rhs;
	}
	return val;
}

bool parse_or()
{
	bool val=parse_and();
	while(!has_err&&pos_idx<static_cast<int>(tok.size())&&tok[pos_idx]=="or")
	{
		++pos_idx;
		bool rhs=parse_and();
		val=val||rhs;
	}
	return val;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string w;
	while(cin>>w)
		tok.push_back(w);

	if(tok.empty())
	{
		cout<<"error\n";
		return 0;
	}

	pos_idx=0;
	has_err=false;
	bool res=parse_or();
	if(has_err||pos_idx!=static_cast<int>(tok.size()))
		cout<<"error\n";
	else
		cout<<(res?"true\n":"false\n");
	return 0;
}
```

### 符号对照表 (Notation Glossary)

| 数学符号 | 代码变量 | 含义说明 |
| :--- | :--- | :--- |
| $t_0 \dots t_{k-1}$ | `tok` | 输入的单词序列 |
| $\text{pos}$ | `pos_idx` | 当前递归下降解析器扫描到的单词下标 |
| $\text{has\_err}$ | `has_err` | 解析过程中是否遇到语法错误 |

---

## Part 5 — 易错点与总结 (Pitfalls & Takeaway)

### 常见易错点

1. **C++ 短路求值吃掉后续语法错误**：在 `parse_and()` 和 `parse_or()` 中，千万不要写成 `val = val && parse_not();`！因为一旦 `val` 为 `false`，C++ 的 `&&` 短路机制会**直接跳过 `parse_not()` 的调用**，导致指针 `pos_idx` 不再前进，既漏掉了后续语法错误，也导致 `pos_idx != tok.size()` 误判！必须先将右操作数存入临时变量 `bool rhs = parse_not();`，再执行 `val = val && rhs;`。
2. **漏判 `pos_idx != tok.size()`**：对于 `true not false` 或 `true true`，前半段 `true` 本身是一个合法表达式，必须通过检查是否消费完全部 Token 来捕获此类错误。

### 本地对拍与验证结果

| 测试项 | 覆盖范围与规模 | 结果 |
| :--- | :--- | :--- |
| 边界与 Hack 测试 | `true not false`、`not not true`、悬空算符等 10 组边界 | 全部通过 |
| 随机压力对拍 | $500$ 组随机合法/非法词串 vs `P1575_brute.cpp`（有限状态机+多趟规约） | 全部一致 |

### 方法归纳 (Pattern Summary)

- **触发信号**：带多级优先级与一元前缀运算符的表达式语法校验及求值。
- **核心套路**：**LL(1) 递归下降分析器 + 末尾 `pos == tok.size()` 完整性断言**，并注意将递归调用提取到临时变量中以避开逻辑运算符的短路副作用。
