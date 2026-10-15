# P1236 算24点

## 1. 重新读题

给你 4 个 1～9 之间的自然数，用 `+` `-` `*` `/` 和括号把它们组合成等于 24 的算式。约束：

- **所有中间结果必须是正整数**（除法必须整除，且中间结果 > 0）
- 如果能算出 24，输出 3 行运算步骤；否则输出 `No answer!`
- 输出规则：每行是 `a op b = result`，两个操作数大的在前
- 第 1 行：两个原始输入 → 中间结果
- 第 2 行：中间结果 + 一个原始输入（或两个中间结果）→ 中间结果
- 第 3 行：中间结果 + 原始输入（或两个中间结果）→ 24
- 多重解输出任意一种即可

## 2. 链式推理

### 2.1 先搞清楚搜索空间有多大

4 个数能组成一棵有 4 个叶子的二叉运算树。我需要数出有多少种不同的"运算序列"：

- 选哪两个数先算：$\binom{4}{2} = 6$ 种
- 运算符：4 种（`+` `-` `*` `/`）
- 剩下 3 个"数"（2 个原始数 + 1 个中间结果），再选两个组合：$\binom{3}{2} = 3$ 种
- 运算符：4 种
- 最后 2 个"数"组合：1 种选法
- 运算符：4 种

总计：$6 \times 4 \times 3 \times 4 \times 1 \times 4 = 1152$ 种组合。

这个量级极小，**直接暴力搜索即可**，不需要任何剪枝或优化。

### 2.2 搜索怎么建：状态表示

搜索过程中，我维护一个"当前可用数的列表"。初始时是 4 个原始数。每一步：

1. 从列表中选两个数
2. 用某个运算符运算
3. 把结果放回列表（替换掉选出的那两个数）
4. 列表大小减 1

当列表只剩 1 个数且等于 24 时，找到解。

**关键点**：选两个数时，减法和除法**不满足交换律**，所以 (a-b) 和 (b-a) 是不同的，需要都试。加法和乘法满足交换律，选 (a,b) 和 (b,a) 是一样的，只需试一次。

**正整数约束**：
- 加法：a+b > 0 自动满足（a,b ≥ 1）
- 减法：a-b > 0 才合法，即 a > b
- 乘法：a*b > 0 自动满足
- 除法：a%b == 0 且 a/b > 0 才合法，即 a 能被 b 整除

### 2.3 记录运算步骤

搜索是回溯的，我维护一个"步骤数组"：

- 每次做一步运算时，把 (操作数1, 操作数2, 运算符, 结果) 存入 `step[depth]`
- 回溯时不需要显式恢复（下一步会覆盖）
- 找到解时，`step[0..2]` 就是 3 步运算

### 2.4 完整搜索算法

```
dfs(cur, cnt, depth):
    如果 cnt == 1:
        如果 cur[0] == 24:
            返回 true
        返回 false
    
    枚举 i < j (i,j ∈ [0,cnt)):
        构造新列表 newcur = cur 去掉 i,j 两个元素
        枚举运算符:
            计算 result，检查合法性
            如果合法:
                记录 step[depth] = (cur[i], op, cur[j], result)
                把 result 加入 newcur
                如果 dfs(newcur, cnt-1, depth+1):
                    返回 true
    返回 false
```

### 2.5 证明正确性

**命题**：上述 DFS 能正确判断是否有解，且能找到解。

**证明**：

**完备性（有解则能找到）**：

任何合法的 24 点解式都对应一棵有 4 个叶子的二叉运算树，树内部恰好 3 个节点（对应 3 次运算）。设这 3 个内部节点从下到上为 $n_1, n_2, n_3$（$n_3$ 是根）。

DFS 的搜索过程等价于：从 4 个叶子开始，每次选两个"子树"合并为一棵更大的子树（即做一步运算），直到只剩一棵树。

DFS 第 1 步从 4 个叶子中选两个。解树中 $n_1$ 的两个孩子都是叶子，一定在 4 个叶子中，所以 DFS 第 1 步**一定能选到** $n_1$ 的两个孩子。选到后，枚举所有运算符时包含 $n_1$ 的运算符，得到的值与解树中 $n_1$ 的值相同（解树保证中间结果合法）。

归纳地，假设 DFS 前 $k-1$ 步精确复现了 $n_1, \ldots, n_{k-1}$ 的合并过程，那么第 $k$ 步时，当前列表中包含 $n_k$ 的两个孩子对应的值。DFS 第 $k$ 步枚举所有两两组合，必然包含 $n_k$ 的两个孩子，且枚举所有运算符时包含 $n_k$ 的运算符。因此 DFS 第 $k$ 步能复现 $n_k$。

最终 DFS 第 3 步复现根节点 $n_3$，其值 = 24，DFS 返回 true。$\square$

**正确性（找到的一定是合法解）**：

DFS 每次运算都检查合法性（结果正整数、除法整除），且只使用 `+` `-` `*` `/` 四种运算。当列表剩 1 个数且等于 24 时，`step[0..2]` 中的 3 步运算构成一个合法的运算序列，最终结果 = 24。由于每步都是对两个已有的数（原始输入或前一步结果）做合法运算，这 3 步组成的算式一定合法。$\square$

**无解判断正确**：

DFS 穷举了所有可能的 3 步运算序列（最多 1152 种），如果没有任何一种最终得到 24，则无解。由完备性证明，任何合法解都会被 DFS 找到，所以无解时 DFS 确实返回 false。$\square$

### 2.6 输出格式的细节

"如果两个操作数有大小的话则先输出大的" —— 每行 `a op b = result` 中，如果 $a \neq b$，则 $\max(a,b)$ 在前。

分析各运算符：
- **加法**：$a+b = b+a$，交换不改变结果。需要保证输出时大的在前。
- **乘法**：$a \times b = b \times a$，同上。
- **减法**：由于正整数约束，被减数 > 减数，所以 step_a（被减数）天然比 step_b（减数）大，不需要交换。
- **除法**：由于正整数约束，被除数 > 除数（整除且商 > 0），所以 step_a（被除数）天然比 step_b（除数）大，不需要交换。

**结论**：输出时统一做 `if(a < b) swap(a,b)` 即可。对减法和除法，a 已经是大的不会交换；对加法和乘法，会正确交换使大的在前。

### 2.7 验证样例

输入：`1 2 3 7`

DFS 过程：
1. 列表 = [1, 2, 3, 7]
2. 选 1 和 2，`+` → 3。新列表 = [3, 3, 7]
3. 选 3（结果）和 7，`*` → 21。新列表 = [3, 21]
4. 选 3（原始）和 21，`+` → 24。新列表 = [24]
5. 24 == 24，找到解！

步骤：
- step[0] = (1, +, 2, 3)
- step[1] = (3, *, 7, 21)
- step[2] = (3, +, 21, 24)

输出（每行大的在前）：
```
1+2=3
7*3=21
21+3=24
```

## 3. 模式总结

- **探索性质** — 4 个数的 24 点问题，搜索空间仅 1152 种组合，暴力即可
- **考虑简化版本** — 抽象为"从 4 个数逐步两两合并到 1 个数"的过程
- **经典问题解法** — DFS 回溯搜索，状态 = 当前数的列表，转移 = 选两个数做运算
- **正整数约束** — 每步运算后检查结果为正整数，除法检查整除
- **记录与回溯** — 步骤数组记录运算序列，DFS 回溯时覆盖
- **输出格式** — 每行两个操作数大的在前，统一 swap 即可

## 4. 真正解法

### 符号表

| 符号 | 含义 | 初始化 |
|------|------|--------|
| `cur[4]` | 当前可用数的列表 | 读入的 4 个原始数 |
| `cnt` | 当前列表大小 | 4 |
| `depth` | 当前搜索深度（0~3） | 0 |
| `step_a[3]` | 第 depth 步的第一个操作数 | — |
| `step_op[3]` | 第 depth 步的运算符 | — |
| `step_b[3]` | 第 depth 步的第二个操作数 | — |
| `step_r[3]` | 第 depth 步的运算结果 | — |
| `found` | 是否找到解 | false |

### 算法步骤

1. 读入 4 个数存入 `cur[0..3]`
2. 调用 `dfs(4, 0)`
3. `dfs(cnt, depth)`:
   - 若 `cnt == 1`：若 `cur[0] == 24`，置 `found = true`，返回 true；否则返回 false
   - 枚举 $i < j$（$i, j \in [0, cnt)$）：
     - 构造 `newcur[]`：`cur[]` 去掉 `i, j` 两个元素
     - 枚举运算（共 8 种可能：`+` `*` 各 1 种，`-` `/` 各 2 种方向）：
       - 计算 result，检查合法性（正整数、整除）
       - 记录 `step_a[depth], step_op[depth], step_b[depth], step_r[depth]`
       - 把 result 加入 `newcur`
       - 若 `dfs(cnt-1, depth+1)` 返回 true，返回 true
   - 返回 false
4. 若 found：按 `step[0..2]` 顺序输出 3 行，每行 `if(a<b) swap(a,b)` 后输出 `a op b = r`
5. 否则输出 `No answer!`

### 复杂度

- 时间：O(1)（常数 = 1152 种组合 × O(1) 检查）
- 空间：O(1)（固定大小的数组）

### 验证状态

- 样例 `1 2 3 7` → 输出 `1+2=3`, `7*3=21`, `21+3=24` ✓
- 所有 1~9 四数组合共 $9^4 = 6561$ 种，均可在 O(1) 内判定

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

int cur[4],step_a[3],step_b[3],step_r[3];
char step_op[3];
bool found=false;

bool dfs(int cnt,int depth)
{
	if(cnt==1)
	{
		if(cur[0]==24)
		{
			found=true;
			return true;
		}
		return false;
	}
	for(int i=0;i<cnt;++i)
		for(int j=i+1;j<cnt;++j)
		{
			int x=cur[i],y=cur[j];
			int newcur[4],nc=0;
			for(int k=0;k<cnt;++k)
				if(k!=i&&k!=j)
					newcur[nc++]=cur[k];
			
			int r;
			
			r=x+y;
			step_a[depth]=x;step_op[depth]='+';step_b[depth]=y;step_r[depth]=r;
			newcur[nc++]=r;
			if(dfs(nc,depth+1))
				return true;
			
			if(x>y)
			{
				r=x-y;
				step_a[depth]=x;step_op[depth]='-';step_b[depth]=y;step_r[depth]=r;
				newcur[nc++]=r;
				if(dfs(nc,depth+1))
					return true;
			}
			if(y>x)
			{
				r=y-x;
				step_a[depth]=y;step_op[depth]='-';step_b[depth]=x;step_r[depth]=r;
				newcur[nc++]=r;
				if(dfs(nc,depth+1))
					return true;
			}
			
			r=x*y;
			step_a[depth]=x;step_op[depth]='*';step_b[depth]=y;step_r[depth]=r;
			newcur[nc++]=r;
			if(dfs(nc,depth+1))
				return true;
			
			if(y!=0&&x%y==0)
			{
				r=x/y;
				step_a[depth]=x;step_op[depth]='/';step_b[depth]=y;step_r[depth]=r;
				newcur[nc++]=r;
				if(dfs(nc,depth+1))
					return true;
			}
			if(x!=0&&y%x==0)
			{
				r=y/x;
				step_a[depth]=y;step_op[depth]='/';step_b[depth]=x;step_r[depth]=r;
				newcur[nc++]=r;
				if(dfs(nc,depth+1))
					return true;
			}
		}
	return false;
}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	for(int i=0;i<4;++i)
		cin>>cur[i];
	dfs(4,0);
	if(!found)
		cout<<"No answer!\n";
	else
	{
		for(int i=0;i<3;++i)
		{
			int a=step_a[i],b=step_b[i];
			if(a<b)
				swap(a,b);
			cout<<a<<step_op[i]<<b<<'='<<step_r[i]<<'\n';
		}
	}
	return 0;
}
```
