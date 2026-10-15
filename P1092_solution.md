# P1092 [NOIP 2004 提高组] 虫食算 题解

---

## 一、重新审题

我们有一个 **n 进制** 的加法竖式（n ≤ 26），三个数都是 **n 位**，允许前导零。虫子把数字全部啃光了，只留下了字母——**相同字母代表相同数字，不同字母代表不同数字**。字母取自英文字母表前 n 个大写字母，每个字母至少出现一次。

我们需要给每个字母分配一个 0 ~ n−1 的**互不相同**的数字，使竖式成立。保证**有唯一解**。

**示例**（n = 4）：

```
 BADC
+ CBDA
-------
 DCCC
```

解：A=0, B=1, C=2, D=3。

验证：
```
  1 0 3 2     (BADC)
+ 2 1 3 0     (CBDA)
= 3 2 2 2     (DCCC)
```
从最低位（最右）逐列计算：  
列 3：2 + 0 = 2 → 2, 进位 0  
列 2：3 + 3 = 6 → 2, 进位 1  
列 1：0 + 1 + 1 = 2 → 2, 进位 0  
列 0：1 + 2 = 3 → 3, 进位 0 ✓

---

## 二、思维链——从暴力搜索到高效剪枝

### 2.1 朴素思路

最直接的想法：枚举所有字母到数字的映射。n 个字母对应 n! 种排列，n ≤ 26 时完全不可行。

### 2.2 用 DFS 逐个赋值

我们用深度优先搜索，每次给一个字母分配一个数字。当所有字母都分配完时，验证竖式是否成立。这样最多搜索 n! 个状态，仍然太大。

### 2.3 关键观察——竖式本身提供了约束

竖式 n 位，从**最低位（最右）**向**最高位（最左）**逐列计算。记进位为 carry（只能是 0 或 1，因为两个 n−1 以内的数字相加最大为 2n−2，加上进位最大 2n−1，在 n 进制下进位不可能超过 1）：

```
a[i] + b[i] + carry_in = c[i] + carry_out × n
carry_in, carry_out ∈ {0, 1}
```

**核心剪枝思路**：在 DFS 过程中，只要发现某个**列**已经不可能成立，就可以立即回溯。

### 2.4 赋值顺序——关键优化

从**最右列**开始，逐列向左处理。对每一列，按 **b[i]、a[i]、c[i]** 的顺序将字母加入赋值顺序（跳过已加入的字母）。这样做的原因是：
- 最低位的列没有进位的模糊性（进位为 0），约束最强；
- 随着搜索深入，右边列先被赋值，左边的列可以利用已知的进位信息验证。

### 2.5 剪枝策略——列一致性检查

每次给一个字母赋完值后，检查**所有列**是否仍然有可能成立。

设某一列三个字母为 x, y, z（分别来自加数、加数、和），当前赋值为 vx, vy, vz，-1 表示尚未赋值。

**情况 A：三个字母均已赋值**  
此时必须满足：
```
(vx + vy) % n == vz   OR   (vx + vy + 1) % n == vz
```
因为进位 carry_in 只能是 0 或 1。若两个条件都不满足，则此部分赋值不可能导出合法解，回溯。

**情况 B：恰好两个字母已赋值，一个未赋值**  
记未赋值的字母为 u。u 的可能取值只有 1~2 个（对应 carry_in = 0 或 1 的情况）。如果所有这些可能取值**都已被其他字母占用**（即 used[val] 全部为 true），则无解，回溯。

具体推导：

- 若 x, y 已知，z 未知：  
  z ∈ {(vx+vy) % n, (vx+vy+1) % n}

- 若 x, z 已知，y 未知：  
  y ∈ {(vz−vx+n) % n, (vz−vx−1+n) % n}

- 若 y, z 已知，x 未知：  
  x ∈ {(vz−vy+n) % n, (vz−vy−1+n) % n}

**情况 C：0 或 1 个字母已赋值**  
无法提供有效约束，忽略。

### 2.6 搜索顺序——降序尝试

对每一个待赋值的字母，尝试数字时从 **n−1 到 0 降序**。这利用了竖式高位约束的特性——较大的数字更容易引起进位冲突，从而更快剪枝。

### 2.7 最终验证

当 n 个字母全部赋值完毕后，再做一次完整的竖式验证（逐列计算进位），确保成立。

---

## 三、模式总结

| 维度 | 模式 |
|------|------|
| **问题类型** | 字母数字映射 + 进制加法竖式 |
| **核心算法** | DFS + 剪枝（回溯搜索） |
| **关键剪枝** | 列一致性检查（已知 3 个值或 2 个值时） |
| **赋值顺序** | 最低位到最高位，逐列 b→a→c |
| **搜索顺序** | 数字降序（n−1 → 0） |
| **复杂度** | O(n!) 最坏，剪枝后可通过 n ≤ 26 |

---

## 四、完整解法

### 4.1 算法流程

1. 读入 n 和三个字符串 a, b, c；
2. 按"从右到左、逐列 b→a→c"的顺序构建字母的赋值序列 `ord`；
3. 初始化 `lk[26]` 为 -1（未赋值），`used[26]` 为 false；
4. DFS 搜索：
   - 参数 k：当前要赋 `ord[k]` 这个字母；
   - 若 k == n，做最终验证并输出；
   - 对每个未使用的数字 d（从 n−1 到 0）：
     - 赋值 `lk[ord[k]] = d`；
     - 若 `prune()` 通过，递归 `dfs(k+1)`；
     - 撤销赋值；
5. `prune()`：对每一列，按上述剪枝策略检查。

### 4.2 记号说明

| 符号 | 含义 |
|------|------|
| n | 进制数，也是位数和字母种类数 |
| a, b, c | 两个加数与和，字符串长度为 n，下标 0 为最高位 |
| lk[ch] | 字母 ch 对应的数字，−1 表示未赋值 |
| used[d] | 数字 d 是否已被某个字母占用 |
| ord[] | 字母的赋值顺序 |
| carry | 列之间的进位，值 ∈ {0, 1} |

### 4.3 代码实现

```cpp
#include<bits/stdc++.h>
using namespace std;
constexpr int mxn{30};

int n;
char a[mxn],b[mxn],c[mxn];
int lk[mxn];	 // letter->digit, -1=unassigned
bool used[mxn];	 // digit used?
int ord[mxn];	 // assignment order (0..n-1)
int oc;		 // order count

void build_order()
{
	oc=0;
	bool inm[mxn]{false};
	for(int i=n-1;i>=0;--i)
	{
		auto add=[&](char ch)
		{
			int id=ch-'A';
			if(!inm[id])
			{
				inm[id]=true;
				ord[oc++]=id;
			}
		};
		add(b[i]); add(a[i]); add(c[i]);
	}
}

bool prune()
{
	for(int i=n-1;i>=0;--i)
	{
		int va=lk[a[i]-'A'];
		int vb=lk[b[i]-'A'];
		int vc=lk[c[i]-'A'];
		// case A: all three assigned
		if(va!=-1&&vb!=-1&&vc!=-1)
		{
			if((va+vb)%n!=vc&&(va+vb+1)%n!=vc)
				return false;
		}
		// case B: exactly two assigned
		else if(va!=-1&&vb!=-1&&vc==-1)
		{
			int r0=(va+vb)%n;
			int r1=(va+vb+1)%n;
			if(r0==r1&&used[r0])return false;
			if(used[r0]&&used[r1])return false;
		}
		else if(va!=-1&&vb==-1&&vc!=-1)
		{
			int r0=(vc-va+n)%n;
			int r1=(vc-va-1+n)%n;
			if(r0==r1&&used[r0])return false;
			if(used[r0]&&used[r1])return false;
		}
		else if(va==-1&&vb!=-1&&vc!=-1)
		{
			int r0=(vc-vb+n)%n;
			int r1=(vc-vb-1+n)%n;
			if(r0==r1&&used[r0])return false;
			if(used[r0]&&used[r1])return false;
		}
	}
	return true;
}

bool dfs(int k)
{
	if(k==n)
	{
		int carry=0;
		for(int i=n-1;i>=0;--i)
		{
			int s=lk[a[i]-'A']+lk[b[i]-'A']+carry;
			if(s%n!=lk[c[i]-'A'])
				return false;
			carry=s/n;
		}
		return carry==0;
	}
	int cur=ord[k];
	for(int d=n-1;d>=0;--d)
	{
		if(used[d])continue;
		lk[cur]=d;
		used[d]=true;
		if(prune()&&dfs(k+1))
			return true;
		lk[cur]=-1;
		used[d]=false;
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>a>>b>>c;
	fill_n(lk,mxn,-1);
	build_order();
	dfs(0);
	for(int i=0;i<n;++i)
		cout<<lk[i]<<" \n"[i==n-1];
}
```

### 4.4 代码要点说明

- **`build_order()`**：从最低位（i=n−1）开始向左扫描，每列按 b[i]→a[i]→c[i] 的顺序将未加入的字母加入顺序表。这保证最先处理的是约束最强的低位。
- **`prune()`**：检查每一列。三个值全已知时严格验证；两个已知时检查未知字母是否还有可用数字。注意 `r0==r1` 对 n=1 的边界处理（此时两者相同，只需看该值是否已被占用）。
- **`dfs(k)`**：对第 k 个待赋字母，从 n−1 到 0 降序尝试。尝试前检查 `used[d]` 跳过已被占用的数字。
- **最终验证**：逐列模拟加法，track 进位。需检查最终进位为 0（最高位无溢出）。
- **输入**：三个字符串各占一行，用 `cin >> a >> b >> c` 读取即可。

---

## 五、复杂度分析

- **时间复杂度**：最坏 O(n!)，但强剪枝后实际搜索空间很小，对 n ≤ 26 可通过。
- **空间复杂度**：O(n)，仅存储映射数组和顺序表。
