# P1957 口算练习题

## 1. 重新读题

### 1.1 问题重述（人话版）

有 $i$ 道口算题，要按题目给出的格式加工成「完整算式 + 算式长度」。每行输入有两种形态：

- **三个数据**：`操作符 运算数1 运算数2`，操作符 `a`/`b`/`c` 分别表示加、减、乘；
- **两个数据**：`运算数1 运算数2`，运算类型**沿用上一行**（即沿用上一个出现的操作符）。

对每道题输出两行：

1. 完整算式，形如 `5+8=13`（无空格）；
2. 该算式的总长度（字符个数，`=` 和结果都算）。

$0<i\le50$，第一个算式一定有操作符，运算数是非负整数且小于 $10000$。

### 1.2 关键约束与规模暗示

- $i\le50$、运算数 $<10^4$：规模极小，纯模拟，不需要任何算法。
- 运算数非负、小于 $10000$，所以输入里不会出现负号；但**减法结果可能为负**，此时结果字符串会多一个 `-`，长度必须按真实字符串算。
- 乘法结果最大约 $10^8$，`int` 够用；用 `long long` 只是习惯性保险。
- 真正的难点不在计算，而在**输入解析**：同一行可能是 $2$ 个或 $3$ 个 token，所以不能用「固定读三个数」的方式。

---

## 2. 思维链（Chain-of-thought）

### 2.1 为什么必须按行读

设输入某一段是
```
a 5 8
2 4
b 9 3
```
第二行只有两个 token。如果一直用 `cin>>x>>y>>z`，读第二行时会把第三行的 `b` 当成 $z$ 读掉，而且 `b` 不是整数，流会直接失效；即使类型侥幸对得上，行边界也彻底乱了。

结论：**先用 `getline` 读整行，再用 `stringstream` 把这一行切成 token 列表**。token 数是 $3$ 还是 $2$ 就一目了然。

### 2.2 操作符怎么「沿用」

用一个循环外的变量 `op` 记住当前操作符（初值可以是任意值，因为题面保证第一行有三个数据，第一次一定被覆盖）。每行：

- token 数 $=3$：第一项是 `a`/`b`/`c`，更新 `op`；后两项是运算数；
- token 数 $=2$：两项都是运算数，`op` 不变。

这里有个小坑：题面说的「与上一题的运算类型相同」，指的是**上一个出现过的操作符**，而不是「上一个 token 列表里恰好有操作符的那一项」。由于三个数据的行一定带操作符，而两个数据的行不带，所以「上一次更新过的 `op`」正是正确答案。

### 2.3 完整算式与长度

对每道题，按操作符计算
$$
\text{res}=\begin{cases}
a+b,&\texttt a\\
a-b,&\texttt b\\
a\times b,&\texttt c
\end{cases}
$$
然后拼字符串
$$
\text{out}=\operatorname{to\_string}(a)+\text{sym}+\operatorname{to\_string}(b)+\texttt{=}+\operatorname{to\_string}(\text{res}),
$$
其中 `sym` 是 `+`、`-`、`*`。

**引理**：题目要求的「算式总长度」恰好等于字符串 `out` 的字符数。

**证明**：题面要求的输出就是完整算式本身（如 `5+8=13`），且没有空格；它的长度按字符计，运算符占 $1$ 个字符、`=` 占 $1$ 个、每个数字的每一位占 $1$ 个字符。`to_string` 十进制表示没有前导零（非负输入）且负结果带一个 `-`，与「算式中实际写出的内容」逐字符一致。因此直接取 `out.size()` 就是答案。$\square$

这个引理顺带解决了一个易错点：**不能用「输入数字的位数 + 结果的位数 + 2」这种公式硬算**，因为负结果多一个符号位，多位数进位也会改变位数。直接建串最稳。

### 2.4 逐项检查边界

- 结果为 $0$：`to_string(0)="0"`，长度 $1$，如 `0+0=0` 长度 $5$。
- 结果为负：`3-8=-5`，长度 $6$（`3`,`-`,`8`,`=`,`-`,`5`）。
- 乘出大数：`12*12=144`，长度 $9$。
- 输入含 $0$：`0+0=0` 没问题，`to_string` 不产生 `+0` 之类。
- 读入第一行后要 `cin.ignore()` 吃掉行末换行，否则第一次 `getline` 会读到空行。

### 2.5 复杂度

每行只做常数次字符串操作，总时间 $O(\text{输入总长})$，空间 $O(1)$（除输出缓冲）。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 每行 token 数不定，但第一个 token 是否为字母可区分两种格式。
- **classic problem solution** — 整行读入 + `stringstream` 切 token，避免格式串错位。
- **state persistence** — 用循环外变量保存上一次的操作符。
- **construct the answer** — 直接拼出算式字符串，长度取其 `size()`；不要用位数公式硬算（负数、进位都会出错）。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 题目行数 | 读入 | 题面的 $i$ |
| `op` | `char` | 当前运算类型 `a`/`b`/`c` | 任意（首行必覆盖） | operator |
| `line` | `string` | 当前整行 | 每次 `getline` | — |
| `t` | `vector<string>` | 当前行的 token 列表 | 每行清空 | tokens |
| `a,b` | `i64` | 两个运算数 | 读入 | — |
| `sym` | `char` | 实际运算符 `+`/`-`/`*` | 每次按 `op` 定 | symbol |
| `res` | `i64` | 运算结果 | 计算 | result |
| `out` | `string` | 完整算式 | 拼装 | output |

### 4.2 算法步骤

1. 读入 `n`，`cin.ignore()` 吃掉换行。
2. 令 `op='a'`（占位）。
3. 循环 `n` 次：
   1. `getline` 读一整行；用 `stringstream` 切出 `t`；
   2. 若 `t.size()==3`：`op=t[0][0]`，`a=stoll(t[1])`，`b=stoll(t[2])`；
   3. 否则：`a=stoll(t[0])`，`b=stoll(t[1])`；
   4. 按 `op` 计算 `res` 并确定 `sym`；
   5. 拼 `out = to_string(a)+sym+to_string(b)+"="+to_string(res)`；
   6. 输出 `out` 和 `out.size()`，各占一行。
4. 结束。

### 4.3 复杂度分析

- 时间 $O(\sum \text{行长度})$，本题上限极小（$n\le50$）。
- 空间 $O(1)$ 额外（字符串长度也不超过约 $40$）。

---

## 5. 示例代码

```cpp
//
// 题目：P1957 口算练习题
// https://www.luogu.com.cn/problem/P1957
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	cin.ignore();
	char op{'a'};
	for(int i=0;i<n;++i)
	{
		string line,s;
		getline(cin,line);
		stringstream ss(line);
		vector<string>t;
		while(ss>>s)
			t.push_back(s);
		i64 a,b;
		if(t.size()==3)
		{
			op=t[0][0];
			a=stoll(t[1]);
			b=stoll(t[2]);
		}
		else
		{
			a=stoll(t[0]);
			b=stoll(t[1]);
		}
		i64 res{0};
		char sym;
		if(op=='a')
		{
			res=a+b;
			sym='+';
		}
		else if(op=='b')
		{
			res=a-b;
			sym='-';
		}
		else
		{
			res=a*b;
			sym='*';
		}
		string out=to_string(a)+sym+to_string(b)+"="+to_string(res);
		cout<<out<<'\n'<<out.size()<<'\n';
	}
	return 0;
}
```
