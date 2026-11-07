# P1952 火星上的加法运算

## 1. 重新读题

### 1.1 问题重述（人话版）

给定进制 $N$（$2\le N\le36$），以及两个 $N$ 进制数（用字符串给出，长度不超过 $200$）。数字字符只可能是 `0`~`9` 与 `a`~`z`，其中 `a`~`z` 分别代表 $10$~$35$。求它们的和，仍然用 $N$ 进制输出。

### 1.2 关键约束与规模暗示

- 长度只有 $200$，两个数相加结果最多 $201$ 位。
- $N\le36$，一位数字的值不超过 $35$，可以用 `int` 直接存。
- 这是手算竖式加法的直接模拟，不需要高精度库。
- 唯一容易错的地方是：**从最低位开始算**（输入是从高位到低位），以及**最高位的进位**。

---

## 2. 思维链（Chain-of-thought）

### 2.1 第一步：把「$N$ 进制数」翻译成我们熟悉的东西

我们平时写十进制加法时，是从个位开始，逐位相加，产生进位。$N$ 进制完全一样，只是「逢 $N$ 进一」。

先建立字符与数值的映射：

$$
\mathrm{val}(c)=
\begin{cases}
c-\texttt{'0'}, & \texttt{'0'}\le c\le\texttt{'9'},\\
c-\texttt{'a'}+10, & \texttt{'a'}\le c\le\texttt{'z'}.
\end{cases}
$$

反过来，数值 $v\in[0,N)$ 输出为字符 $c(v)$：$v<10$ 输出 `'0'+v`，否则输出 `'a'+v-10`。

**引理 1（进位只有 0 或 1）**：每位相加时，两个数字 $x,y\in[0,N-1]$ 加上来自低位的进位 $c\in\{0,1\}$，有

$$
s=x+y+c\le 2(N-1)+1=2N-1,
$$

所以本位数字 $s\bmod N\in[0,N-1]$，新进位 $\lfloor s/N\rfloor\in\{0,1\}$。

**证明**：由 $s\le2N-1<2N$ 得 $\lfloor s/N\rfloor\le1$；显然 $\ge0$。$\square$

这条引理保证「进位」永远只占一位，不会连锁出两位进位，竖式加法的结构是对的。

### 2.2 第二步：从低位到高位

输入字符串是高位在前。把两个字符串都翻转，下标 $0$ 就是最低位。设翻转后为 $A,B$，长度分别为 $L_A,L_B$，令 $L=\max(L_A,L_B)$。

对 $i=0,\dots,L-1$：越界的那一位视为 $0$，计算

$$
s_i=\mathrm{val}(A_i)+\mathrm{val}(B_i)+\mathrm{carry},
$$

本位为 $s_i\bmod N$，新进位为 $\lfloor s_i/N\rfloor$。循环结束后若进位为 $1$，再补一位 $1$。

**引理 2（逐位正确）**：上述过程得到的第 $i$ 位恰好是两个数之和在 $N$ 进制下的第 $i$ 位。

**证明**（对位归纳）：设两个数分别为 $\sum a_iN^i$、$\sum b_iN^i$。两数之和为 $\sum(a_i+b_i)N^i$。对最低位，$s_0=a_0+b_0$，其 $N$ 进制最低位是 $s_0\bmod N$，向更高位的贡献是 $\lfloor s_0/N\rfloor N$，正好是进位。设前 $i$ 位处理完后，把当前结果写成 $A+B=R_i+N^iQ_i$（$R_i$ 是已产出的低 $i$ 位），断言 $Q_i=\mathrm{carry}_i+\sum_{t\ge i}(a_t+b_t)N^{t-i}$。设 $a_i+b_i+\mathrm{carry}_i=r_i+N\cdot\mathrm{carry}_{i+1}$（$0\le r_i<N$），则 $N^iQ_i=N^i r_i+N^{i+1}Q_{i+1}$，归纳成立。归纳到 $i=L$ 时 $A+B=R_L+N^L\mathrm{carry}_L$，进位为 1 就补最高位 1，否则 $R_L=A+B$；$N$ 进制表示唯一，故每一位都正确。$\square$

### 2.3 第三步：边界情况

1. **两数长度不同**：短的高位补 0，这不会改变数值。代码用 `i<(int)A.size()` 判断。
2. **最高位进位**：例如十进制 $999+1=1000$，长度从 3 变 4，必须补一位。
3. **前导零**：输入形如 `000999`，我们按位加会得到前导零；输出前把高位多余的 `0` 去掉，但至少保留一位（和为 $0$ 时输出 `0`）。
4. **结果恰好为 0**：例如 `0 + 0`，去掉前导零后剩空串，要输出 `0`。
5. **$N=36$**：最大数字 `z` 值为 $35=N-1$，合法；输出时 $35$ 仍映射回 `z`。

### 2.4 复杂度

- 时间 $O(L)$，$L\le201$。
- 空间 $O(L)$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — $N$ 进制就是逢 $N$ 进一，字符与数值按 0-9、a-z 映射。
- **consider easier version** — 退化成十进制竖式加法，逻辑完全一致。
- **classic problem solution** — 翻转字符串，从低位到高位模拟进位。
- **explore its property** — 进位只能是 0 或 1，故结构简单；注意最高位补位与前导零。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `N` | `int` | 进制 | 读入 | $2\le N\le36$ |
| `A,B` | `string` | 两个加数（读入后翻转，低位在前） | 读入 | addend |
| `len` | `int` | $\max(\lvert A\rvert,\lvert B\rvert)$ | 计算 | 长度 |
| `res` | `string` | 结果，低位在前，最后翻转 | 空 | result |
| `carry` | `int` | 进位，只取 $0/1$ | 0 | 进位 |
| `s` | `int` | 当前位和 $x+y+carry$ | 每轮 | sum |

### 4.2 算法步骤

1. 读入 $N,A,B$。
2. `reverse(A.begin(),A.end()); reverse(B.begin(),B.end());`
3. 令 `len=max(|A|,|B|)`，`carry=0`。
4. 对 $i=0\dots len-1$：
   1. `x = (i<|A| ? val(A[i]) : 0)`，`y = (i<|B| ? val(B[i]) : 0)`。
   2. `s=x+y+carry`；`res.push_back(ch(s%N))`；`carry=s/N`。
5. 若 `carry` 非零，`res.push_back(ch(carry))`。
6. 去掉 `res` 末尾（即高位）多余的 `'0'`，至少保留一位。
7. 翻转 `res` 输出。

### 4.3 复杂度

- 时间 $O(L)$，空间 $O(L)$，$L\le201$。

---

## 5. 示例代码

```cpp
//
// 题目：P1952 火星上的加法运算
// https://www.luogu.com.cn/problem/P1952
#include<bits/stdc++.h>
using namespace std;

int N;
string A,B;

int val(char c)
{
	if(c>='0'&&c<='9')
		return c-'0';
	return c-'a'+10;
}

char ch(int v)
{
	if(v<10)
		return '0'+v;
	return 'a'+v-10;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>N>>A>>B;
	reverse(A.begin(),A.end());
	reverse(B.begin(),B.end());
	int len=max((int)A.size(),(int)B.size());
	string res;
	int carry{0};
	for(int i=0;i<len;++i)
	{
		int x=i<(int)A.size()?val(A[i]):0;
		int y=i<(int)B.size()?val(B[i]):0;
		int s=x+y+carry;
		res.push_back(ch(s%N));
		carry=s/N;
	}
	if(carry)
		res.push_back(ch(carry));
	while(res.size()>1&&res.back()=='0')	// 去掉结果高位的多余 0
		res.pop_back();
	reverse(res.begin(),res.end());
	cout<<res<<'\n';
	return 0;
}
```
