# P1812 区间运算

## 1. 重新读题

每个「数」是一个闭区间 $[l,r]$。两个区间做运算的结果，是让两个区间中任意取点做该运算所得全部实数构成的集合。需要处理一行或多行中缀表达式，每行不超过 $80$ 个字符，含：

- 括号 `(` `)`；
- 一元负号 `-`（如 `-[-3,5]`）；
- 二元 `+ -`（低级，左结合）；
- 二元 `* /`（高级，左结合）。

优先级从高到低：括号、一元负号、`* /`、`+ -`。输入可能有空格，但空格不出现在 `[]` 内部或负号之后。对每行输出 `[min,max]`（各保留 3 位小数）；若某次除法中除数区间**包含 0**，整行输出 `Division by zero`。

**规模暗示**：每行 $\le80$ 字符，说明表达式很短，可以用「一边读一边递归下降」的 $O(L)$ 解析，不需要任何表达式树预处理；真正的考点是区间运算的端点公式和负零/除零处理。

---

## 2. 思维链（Chain-of-thought）

### 2.1 区间运算到底怎么算

先解决最基本的四则。加、减是显然的：

$$[a,b]+[c,d]=[a+c,\,b+d],\qquad [a,b]-[c,d]=[a-d,\,b-c].$$

（减法的下界要让被减数尽量小、减数尽量大，上界反过来。）

取相反数：$-[a,b]=[-b,-a]$。

乘法、除法不那么显然，但有一个统一的事实：

**引理 1（双线性函数在矩形上的最值在角点取得）**
设 $f(x,y)$ 在矩形 $[a,b]\times[c,d]$ 上对每个变量都单调（或更一般地，固定一个变量后是另一个变量的一次函数），则 $f$ 在矩形上的最小值和最大值都在四个角点 $(a,c),(a,d),(b,c),(b,d)$ 中取得。

**证明**：固定 $y$，$f(\cdot,y)$ 是 $x$ 的一次函数，它在 $x\in[a,b]$ 上的最值在 $x=a$ 或 $x=b$ 取得。于是整个矩形上的最值等于两条竖边 $x=a$、$x=b$ 上的最值中的最值。再看竖边 $x=a$：$f(a,\cdot)$ 是 $y$ 的一次函数，最值在 $y=c$ 或 $y=d$ 取得；$x=b$ 同理。合起来，矩形上的最值必在四个角点之一取得。$\blacksquare$

- 乘法 $f(x,y)=xy$ 对固定 $y$ 是 $x$ 的一次函数，符合引理 1，取四个角点乘积的 $\min/\max$。
- 除法 $f(x,y)=x/y$ 在 $y\ne0$ 的矩形上同理；只要除数区间 $[c,d]$ **不含 0**（即不是 $c\le0\le d$），四个角点商就给出完整的 $\min/\max$。
- 若除数区间含 0，真实结果集无界（甚至可能不是区间），题目规定直接报 `Division by zero`。

**为什么含 0 一定要特判而不是「跳过 0 点算四个角」**：取 $[1,1]\div[-1,1]$，$y\to0^+$ 时商趋于 $+\infty$，$y\to0^-$ 时趋于 $-\infty$，结果不是有限区间，四个角点（$-1,1$）给出的 $[-1,1]$ 完全错误。所以必须整行报错。

### 2.2 解析：递归下降

优先级层次天然对应四个函数：

```
expr  := term (('+'|'-') term)*        // 最低级，左结合
term  := unary (('*'|'/') unary)*      // 次低级，左结合
unary := '-' unary | prim              // 一元负号，右结合
prim  := '(' expr ')' | '[' num ',' num ']'
```

几个容易踩的坑：

1. **`-` 的三种身份**。在 `[...]` 内部它是数字的符号（交给 `strtod` 一并读掉）；在区间外，若它出现在「需要操作数」的位置（行首、`(` 后、另一个运算符后），它是一元负号；否则才是二元减号。递归下降里 `expr` 只在读完一个 `term` 后看 `+/-` 当二元运算符，`unary` 只在最前面看 `-`，两者不会混淆。
2. **空格**。在每次判断当前字符前跳过空白即可；`[]` 内的数字用 `strtod` 解析，天然不会吃到空格。
3. **左结合**。`expr` / `term` 都用 `while` 循环「读一个运算符再读一个操作数」，自然是从左到右累计。
4. **负号优先级高于乘除**：`-unary` 在 `unary` 层，而 `term` 调用的是 `unary`，所以 `-[3,5]*[2,2]` 解析为 $(-[3,5])\times[2,2]$，符合题目给出的优先级。

### 2.3 输出与浮点细节

题目要求保留三位小数。浮点运算可能把数学上恰为 $0$ 的结果算成 $-10^{-17}$，`printf`/`ostream` 会输出 `-0.000`。处理办法：若 $|x|<10^{-9}$，先把 $x$ 改成 $0$，再格式化。这只影响「数值上就是 0」的情形，不会改变正常舍入。

另外，`Division by zero` 一旦出现，整行结果作废；解析函数里置一个全局 `bad` 标志并让上层直接返回即可（剩余字符不用再解析，答案已经确定）。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 四则运算都是「双线性函数在矩形上取最值」，最值必在角点（引理 1）。
- **classic problem solution** — 乘法/除法枚举四个端点组合；加法/减法直接算端点。
- **handle special case** — 除数区间含 0 时结果无界，整行输出 `Division by zero`。
- **classic problem solution** — 按优先级写四个递归下降函数，左结合用循环实现。
- **numeric detail** — 输出前把 $|x|<10^{-9}$ 归零，避免 `-0.000`。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `itv` | `struct{double lo,hi;}` | 一个闭区间 | — | interval |
| `s` | `string` | 当前行 | 每行 `getline` | — |
| `pos` | `int` | 当前解析位置 | `0` | cursor |
| `len` | `int` | 当前行长度 | `s.size()` | — |
| `bad` | `bool` | 本行是否出现除零 | `false` | error flag |
| `eps` | `constexpr double` | 负零判定阈值 | `1e-9` | — |

### 4.2 算法步骤

1. 读入一行到 `s`；若全为空白则跳过（不输出）。
2. 置 `pos=0`、`bad=false`，调用 `pexpr()` 得到结果区间。
3. 若 `bad`：输出 `Division by zero`；否则把区间端点按 3 位小数输出成 `[lo,hi]`。
4. 重复直到读不到下一行。

各函数职责：

- `pprim()`：跳过空白；遇 `(` 递归 `pexpr()` 并吃掉 `)`；否则吃 `[`，用 `strtod` 读两个数并吃 `,`、`]`。
- `punary()`：若当前是 `-`，吃掉后递归 `punary()`，返回 `[-hi,-lo]`；否则 `pprim()`。
- `pterm()`：先 `punary()`，循环读 `*` / `/` 与右操作数，按引理 1 合并；除法先判除零。
- `pexpr()`：先 `pterm()`，循环读 `+` / `-` 与右操作数合并。

### 4.3 复杂度

- 时间：每行每个字符至多被扫描常数次，$O(L)$，$L\le80$。
- 空间：递归深度不超过括号/负号层数，$O(L)$；无额外大数组。

---

## 5. 示例代码

```cpp
//
// 题目：P1812 区间运算
// https://www.luogu.com.cn/problem/P1812
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr double eps{1e-9};

struct itv{double lo,hi;};

string s;
int pos,len;
bool bad;

void skip()
{
	while(pos<len&&isspace((unsigned char)s[pos]))
		++pos;
}

itv pexpr();

itv pprim()
{
	skip();
	if(pos<len&&s[pos]=='(')
	{
		++pos;
		itv r=pexpr();
		skip();
		++pos;          // ')'
		return r;
	}
	++pos;              // '['
	skip();
	char*end;
	double a=strtod(s.c_str()+pos,&end);
	pos=int(end-s.c_str());
	skip();
	++pos;              // ','
	skip();
	double b=strtod(s.c_str()+pos,&end);
	pos=int(end-s.c_str());
	skip();
	++pos;              // ']'
	return {a,b};
}

itv punary()
{
	skip();
	if(pos<len&&s[pos]=='-')
	{
		++pos;
		itv r=punary();
		return {-r.hi,-r.lo};
	}
	return pprim();
}

itv pterm()
{
	itv r=punary();
	for(;;)
	{
		skip();
		if(pos>=len||(s[pos]!='*'&&s[pos]!='/'))
			break;
		char op=s[pos++];
		itv b=punary();
		if(op=='*')
		{
			double v[4]={r.lo*b.lo,r.lo*b.hi,r.hi*b.lo,r.hi*b.hi};
			r={*min_element(v,v+4),*max_element(v,v+4)};
		}
		else
		{
			if(b.lo<=0&&b.hi>=0)      // 除数含 0
			{
				bad=true;
				return {0,0};
			}
			double v[4]={r.lo/b.lo,r.lo/b.hi,r.hi/b.lo,r.hi/b.hi};
			r={*min_element(v,v+4),*max_element(v,v+4)};
		}
	}
	return r;
}

itv pexpr()
{
	itv r=pterm();
	for(;;)
	{
		skip();
		if(pos>=len||(s[pos]!='+'&&s[pos]!='-'))
			break;
		char op=s[pos++];
		itv b=pterm();
		if(op=='+')
			r={r.lo+b.lo,r.hi+b.hi};
		else
			r={r.lo-b.hi,r.hi-b.lo};
	}
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cout<<fixed<<setprecision(3);
	while(getline(cin,s))
	{
		bool any=false;
		for(char c:s)
			if(!isspace((unsigned char)c))
				any=true;
		if(!any)
			continue;
		pos=0;len=int(s.size());bad=false;
		itv r=pexpr();
		if(bad)
		{
			cout<<"Division by zero\n";
			continue;
		}
		double lo=r.lo,hi=r.hi;
		if(fabs(lo)<eps)
			lo=0;
		if(fabs(hi)<eps)
			hi=0;
		cout<<'['<<lo<<','<<hi<<"]\n";
	}
	return 0;
}
```
