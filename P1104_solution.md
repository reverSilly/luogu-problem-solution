# P1104 生日 题解

---

## 1. 重新阅读问题

给定 $n$ 名同学，每人有姓名 $s$、出生年 $y$、月 $m$、日 $d$。要求按**年龄从大到小**的顺序输出姓名。

**关键约束**：

- 年龄大 = 出生日期早（数值上更小）。
- 如果两个同学生日完全相同（同岁），则**输入靠后的同学先输出**（即维持输入中的相对顺序，但靠后的更优先）。
- $n<100$，$|s|<20$，年份 $\in[1960,2020]$，日期保证合法。
- 只输出姓名，每行一个。

---

## 2. 思维链与完整证明

### 2.1 排序方向的确定

题目要求"年龄从大到小"排序。年龄大的含义是出生更早，即年月日组成的**三元组更小**。因此：

> **核心等价关系**：按年龄从大到小输出 $\Longleftrightarrow$ 按出生日期从小到大排序。

证明：设两个同学 $A,B$，$A$ 年龄比 $B$ 大 $\iff$ $A$ 出生时间早于 $B$ $\iff$ 比较三元组 $(y,m,d)$，$A$ 的三元组字典序更小。

### 2.2 稳定性问题的处理

题中特殊要求：**年龄相同时，输入靠后的同学先输出**。

这与通常的"稳定排序"正好相反：

- 稳定排序（如 `std::stable_sort`）在相等时保持原始顺序，即**输入靠前的先输出**。
- 这里要求**输入靠后的先输出**，因此我们需要**自定义比较器**，使得当生日相等时，下标更大的视为"更小"。

等价地，排序关键字可定义为：

```
key(i) = (y_i, m_i, d_i, -i)     // -i 使下标大的排在前面
```

然后按照 key 从小到大排序即可。

**为什么不能直接用 `std::stable_sort` 然后反转？** 因为反转后年龄顺序会错乱——年龄不同的人的顺序也会被反转。

**为什么不用 tuple 直接比较？** 可以，但自己写比较器更清晰且能明确处理相等情形。

### 2.3 复杂度分析

- $n\le 100$，$\texttt{std::sort}$ 复杂度 $O(n\log n)$，完全可行。
- 甚至 $O(n^2)$ 的冒泡排序也能通过，但使用 $\texttt{sort}$ 更简洁规范。

### 2.4 数据范围

题目中 $1<n<100$，但习惯性开 $105$ 留有余量。

---

## 3. 模式总结

| 要点 | 说明 |
|------|------|
| **排序方向** | 年龄从大到小 $\iff$ 出生日期从小到大 |
| **排序关键字** | $(y,m,d)$ 字典序，可选 $+(-i)$ 控制稳定性 |
| **特殊稳定性** | 输入靠后的先输出 $\Rightarrow$ 下标大的视为更小 |
| **数据类型** | 年份/月份/日期均为整数，姓名字符串 |
| **算法** | 自定义排序（`sort` + lambda/比较函数） |

这是一个典型的**多关键字排序 + 自定义稳定性**问题，在 OI 入门阶段非常常见，同时也是理解 `sort` 自定义比较器的一个好例子。

---

## 4. 真实解法与符号说明

### 4.1 符号表

| 符号 | 含义 |
|------|------|
| $n$ | 同学人数 |
| $s_i$ | 第 $i$ 个同学的姓名 |
| $(y_i,m_i,d_i)$ | 第 $i$ 个同学的出生年月日 |
| $i$ | 输入顺序下标（从 $0$ 开始） |
| $\prec$ | 排序比较关系：$a\prec b$ 表示 $a$ 应排在 $b$ 前面 |

### 4.2 比较器定义

定义一个严格弱序比较函数 $\text{cmp}(a,b)$：

$$
\text{cmp}(a,b)=\begin{cases}
\text{true} & \text{if } (y_a,m_a,d_a) < (y_b,m_b,d_b) \text{ (字典序)} \\
\text{true} & \text{if } (y_a,m_a,d_a) = (y_b,m_b,d_b) \text{ and } a>b \text{ (下标大的优先)} \\
\text{false} & \text{otherwise}
\end{cases}
$$

### 4.3 算法步骤

1. 读入 $n$。
2. 循环 $i=0..n-1$，读入 $s_i$, $y_i$, $m_i$, $d_i$，同时记录 $i$。
3. 将 $0..n-1$ 的下标数组按照比较器 $\text{cmp}$ 排序。
4. 按排序后的下标输出对应的姓名 $s_i$。

### 4.4 正确性证明

- 如果 $(y_a,m_a,d_a)\ne(y_b,m_b,d_b)$，字典序比较保证了出生早的排在前面，即年龄大的在前。
- 如果生日完全相同，$\text{cmp}(a,b)$ 返回 $a>b$，即下标大的被认为"更小"，因而排在前面。这正好实现了"输入靠后的先输出"。
- 该比较器满足严格弱序的三条公理（反对称性、传递性、不可比性的传递性），可安全用于 `std::sort`。

---

## 5. 示例代码

### 5.1 下标数组法（推荐）

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

string name[mxn];
int y[mxn],m[mxn],d[mxn];
int idx[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>name[i]>>y[i]>>m[i]>>d[i];
		idx[i]=i;
	}

	sort(idx,idx+n,[](int a,int b)
	{
		if(y[a]!=y[b])return y[a]<y[b];
		if(m[a]!=m[b])return m[a]<m[b];
		if(d[a]!=d[b])return d[a]<d[b];
		return a>b;
	});

	for(int i=0;i<n;++i)
	{
		cout<<name[idx[i]]<<'\n';
	}
}
```

### 5.2 结构体法

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

struct Student
{
	string name;
	int y,m,d,id;
} stu[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>stu[i].name>>stu[i].y>>stu[i].m>>stu[i].d;
		stu[i].id=i;
	}

	sort(stu,stu+n,[](const Student& a,const Student& b)
	{
		if(a.y!=b.y)return a.y<b.y;
		if(a.m!=b.m)return a.m<b.m;
		if(a.d!=b.d)return a.d<b.d;
		return a.id>b.id;
	});

	for(int i=0;i<n;++i)
	{
		cout<<stu[i].name<<'\n';
	}
}
```

---

## 补充说明

- **排序方向**：年龄从大到小 $=$ 出生日期从小到大，所以比较器内全部使用 `<`，不要搞反。
- **稳定性**：`std::sort` 本身不稳定，但我们通过 `a>b` 显式处理了相等情况，效果符合题意。
- **没有踩坑**：年份 $\in[1960,2020]$，月份和日期均合法，无需校验。
- 数据范围很小，两种写法在时间上没有任何差别，选自己喜欢的即可。
