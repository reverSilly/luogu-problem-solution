# P2373 yyy2015c01 的 IDE 之 Call Stack — 题解

## 一、解题过程：人类思考叙事

**重述题意。** 输入 $n+1$ 行：第一行是调用次数 $n$，后面 $n$ 行每行是一条函数调用信息，形如

```
int fac1(int n);
```

它给出了函数的**返回类型**、**函数名**、以及**参数表**（每个参数都是"类型 + 形参名"）。判定两个函数"是同一个"的规则是：

- 函数名**忽略大小写**；
- 参数的**类型序列**必须逐个相同；
- **形参名**无所谓。

所以 `int fac1(int n)` 与 `int fac1(char n)` 是两个不同的函数（类型不同），而 `int Fac1(int n,int m)` 与 `int FAC1(int x,int y)` 是同一个函数（名字只差大小写，类型都是 `int,int`）。

`main` 可能出现也可能不出现，但无论如何都不计入答案。保证程序里每个函数至少被调用一次。输出不同函数的个数。约束：每行长度 $\le255$，每个函数参数至多 $10$ 个。

**第一反应：这不就是个"去重"题吗。** 把每条调用信息变成一个"身份证号"（规范形），相同身份证号就是同一个函数，最后数一数有几种身份证号即可。难点全部在**怎么发身份证号**。

**怎么设计身份证号？** 直接拿整行字符串比是不行的：`int fac1(int n);` 和 `int fac1(int m);` 整行不同但是同一个函数；`int Fac1(int n,int m);` 和 `int FAC1(int x,int y);` 整行也不同但是同一个函数。必须把"不该影响身份"的东西**擦掉**，把"影响身份"的东西**留住**：

- 擦掉：形参名、返回类型（见下文讨论）、所有空格、行尾的 `;`、函数名的大小写差异。
- 留住：函数名的字母序列（小写化后）、参数类型的**顺序**。

于是规范形定为

$$\text{key}=\texttt{小写函数名}(\text{类型}_1,\text{类型}_2,\dots,\text{类型}_k).$$

**第二步：怎么从一行里把这三样东西切出来？** 盯着格式看，发现它的"标点骨架"极其规整：

```
  int   fac1  (  int  n  ,  char  m  )  ;
  └─┬─┘ └─┬─┘  └──────┬──────┘
  返回类型 函数名      参数表
```

- **第一个 `(` 之前**的部分是"返回类型 + 函数名"。因为返回类型和函数名都是标识符，不可能含括号，所以第一个 `(` 一定出现在函数名之后；把这段按空格切成 token，**最后一个 token 就是函数名**（前面的返回类型顺手丢掉）。
- **第一个 `(` 和它后面第一个 `)` 之间**就是参数表。参数里只有标识符和逗号，不会有括号，所以 `)` 就是参数表的右端。
- **按 `,` 切参数表**，每一段就是一个形参，段内的**第一个 token 是类型**，剩下的（如果有）是形参名。
- `)` 之后的 `;` 根本不用管。

用 `istringstream` 做 token 化，任意多、任意位置的空格都能自动吃掉，比手写下标扫描稳。

**第三步：三个"看起来对但其实会错"的念头。**

1. **"返回类型要不要进 key？"** 如果把返回类型也拼进 key，那么 `int f(int a)` 与 `char f(int a)` 会被认成两个函数。题面把身份规则写得很死："认的不是参数名，而是其类型和忽略大小写的函数名"——只提到名字和参数类型，所以按题意应该**丢掉返回类型**。另一方面，在一份合法的 C++ 程序里，同一个函数不可能有两种返回类型，所以两种写法在合法数据上结果相同；既然规则明确，就按规则丢掉（代码里"取 `(` 前最后一个 token"天然做到了这一点）。
2. **"`main` 怎么排除？"** 因为函数名是忽略大小写比较的，排除也应该在**小写化之后**做，这样 `Main`、`MAIN` 也一并被排除，与规则一致。
3. **"`main()` 没有参数，会不会把空串也当成一个参数？"** 会——如果按 `,` 切分后不检查空段。`()` 之间是空串，必须跳过空段，否则 key 会变成 `main()` 里塞进一个空类型。

**第四个念头：为什么"数不同调用"就等于"数不同函数"？** 因为题面保证了每个函数至少被调用一次。若某个函数一次都没被调用，它就不会出现在输入里，我们也就数不到它——这条保证正是"数调用签名"能等于"数函数"的原因。

**手造几组小数据走一遍**（题面没有给出样例数据，这里自己造）：

```
2
int fac1(int n);
int fac1(char n);
```
类型不同 ⇒ 2 个函数，输出 `2`。

```
2
int Fac1(int n,int m);
int FAC1(int x,int y);
```
小写化后名字都是 `fac1`，类型都是 `(int,int)` ⇒ 输出 `1`。

```
4
int f(int a);
int F(char b);
int f(int c,int d);
int f(int ,int  );
```
keys 分别是 `f(int)`、`f(char)`、`f(int,int)`、`f(int,int)` ⇒ 输出 `3`（第 3、4 行合流，且第 4 行形参连名字都没有也能解析）。

```
3
int main();
int f(int a);
int MAIN();
```
两个 `main` 一律不计 ⇒ 输出 `1`。

**实现上的坑：**

- `cin>>n` 之后必须 `getline` 一次把第一行剩余部分（换行）吃掉，否则第一次 `getline` 读到空行。
- 若评测数据用 `\r\n` 换行，`getline` 会把 `\r` 留在行尾。本例中 `\r` 出现在 `)` 之后，不影响解析，但稳妥起见行尾的 `\r`/空格应当剥掉。
- 行尾的 `;` 落在 `)` 之后，解析参数表时自然不会被读到。

---

## 二、思维链：完整证明

**记号**：对一行调用信息 $\ell$，记 $\mathrm{name}(\ell)$ 为其函数名（原样大小写），$\mathrm{types}(\ell)=(t_1,\dots,t_k)$ 为其参数类型序列（按出现顺序），$k\ge0$。记 $\mathrm{low}(s)$ 为 $s$ 全部转小写。

**引理 1（身份是等价关系，"数不同函数"就是数等价类）**：按题面规则定义 $\ell_1\approx\ell_2$ 当且仅当 $\mathrm{low}(\mathrm{name}(\ell_1))=\mathrm{low}(\mathrm{name}(\ell_2))$ 且 $\mathrm{types}(\ell_1)=\mathrm{types}(\ell_2)$（长度相同且逐项相同）。则 $\approx$ 是等价关系，且答案等于 $\{\ell_1,\dots,\ell_n\}$ 在该关系下的等价类个数（去掉 `main` 的那些）。

*证明*：自反、对称、传递性直接来自"字符串相等"与"序列相等"这三条性质：例如传递性，若 $\mathrm{low}(n_1)=\mathrm{low}(n_2)$、$\mathrm{low}(n_2)=\mathrm{low}(n_3)$，则 $\mathrm{low}(n_1)=\mathrm{low}(n_3)$；类型序列同理。所以 $\approx$ 把 $n$ 行分成若干互不相交的类。题面所说的"不完全相同的函数"正是"不属于同一个 $\approx$ 类"，因此不同函数的个数 $=$ 类的个数。又因为每个函数至少被调用一次，每个函数恰好对应出现过的某个类，故类的个数就是答案。证毕。

**引理 2（规范形与原判定等价）**：令
$$\mathrm{key}(\ell)=\mathrm{low}(\mathrm{name}(\ell))\,\Vert\,\texttt{(}\,\Vert\,t_1\Vert\texttt{,}\Vert\cdots\Vert t_k\,\Vert\,\texttt{)} .$$
则 $\mathrm{key}(\ell_1)=\mathrm{key}(\ell_2)\iff\ell_1\approx\ell_2$。

*证明*：由引理 1，$\ell_1\approx\ell_2$ 等价于"$\mathrm{low}(\mathrm{name})$ 相同且类型序列相同"。
（$\Leftarrow$）若两项都相同，则拼出的字符串逐字符相同（拼接结果只由这两项决定，分隔符是固定的）。
（$\Rightarrow$）设两个 key 相同。由于题面规定参数类型只能是 `int` 或 `char`，其中不含 `(`、`)`、`,` 三个分隔符，故字符串中 `(` 的位置只可能来自固定插入的那一个分隔符：从 key 里按第一个 `(` 切开，左边必然是 $\mathrm{low}(\mathrm{name})$，于是两边的 $\mathrm{low}(\mathrm{name})$ 相同。右边按 `,` 切分（空串切成 0 段）得到类型序列，两边的序列也相同（因为切分位置完全由分隔符决定，而分隔符不会出现在类型内部）。证毕。
*为什么强调"类型里没有逗号"*：若类型本身可能含 `,`（比如模板参数），拼接就不再是单射，key 相同的两个函数可能被判成同一个。这是"用字符串拼接做规范形"必须检查的地方。

**引理 3（解析器确实算出了 $\mathrm{name}$ 与 $\mathrm{types}$）**：设某行符合题面承诺的格式，即形如
$$\texttt{[ret]}\ \texttt{name}\ \texttt{(}\ \texttt{t}_1\ \texttt{p}_1\ \texttt{,}\ \cdots\ \texttt{,}\ \texttt{t}_k\ \texttt{p}_k\ \texttt{)}\ \texttt{;}$$
（返回类型可缺省，$k\ge0$，各 token 之间可有任意多空格）。则代码得到的 `name` 与类型序列就是 $\mathrm{name}(\ell)$ 与 $\mathrm{types}(\ell)$。

*证明*：分四步。

(1) **第一个 `(` 的位置**：返回类型与函数名都是标识符，标识符不含 `(`，因此行内第一个 `(` 必然出现在函数名结束之后；于是 `substr(0,lp)` 即"返回类型（可能没有）+ 函数名"这两段，可能夹带首尾空格。

(2) **最后一个 token 是函数名**：由 (1)，`head` 的 token 序列是 $[\texttt{ret},\texttt{name}]$ 或 $[\texttt{name}]$（`istringstream` 按空白切分，多空格被折叠）。两种情况下最后一个 token 都是 `name`，故 `tk.back()` 正确；`tk` 非空亦由此保证（函数名必然存在）。注意这里"丢掉返回类型"不是随手为之，而是由格式结构保证的。

(3) **`)` 的位置**：参数表内部只有标识符与逗号，没有括号，所以 `lp` 之后第一个 `)` 就是参数表的右端点 `rp`；`substr(lp+1,rp-lp-1)` 恰为参数表内容，行尾的 `;`（在 `rp` 之后）不被读取。

(4) **按 `,` 切出的第 $i$ 段**：由格式，第 $i$ 个形参是 $\texttt{t}_i\ \texttt{p}_i$，切成 token 后为 $[\texttt{t}_i,\texttt{p}_i]$，其第一个 token 就是类型 $\texttt{t}_i$；若形参省略了名字则为 $[\texttt{t}_i]$，第一个 token 仍是类型。空串（$k=0$ 时）切成 0 个 token，代码用 `if(!pt.empty())` 跳过，因此不会产生多余的"空类型"。于是收集到的序列逐项等于 $\mathrm{types}(\ell)$，并保持了原本的先后顺序。证毕。

**引理 4（`set` 的大小就是答案）**：把每一行（除 `main` 外）的 key 插入 `std::set<std::string>`，最终集合大小等于答案。

*证明*：`std::set` 的语义是"有序、元素互异"——插入一个已存在的元素不改变大小。由引理 3，插入的 key 恰为 $\mathrm{key}(\ell)$；由引理 2，$\mathrm{key}$ 相等 $\iff$ 同一等价类。所以集合的元素与等价类一一对应，大小 $=$ 类数 $=$ 答案（引理 1）。证毕。

**引理 5（`main` 的排除是正确的）**：代码在 `name` 小写化之后与 `"main"` 比较并跳过，排除的正是所有"函数名为 `main`（忽略大小写）"的行，且不影响其它行。

*证明*：由题面"`int main()` 可能出现，也可能不出现；无论它有没有出现，都不能将其算入函数中"，函数名为 `main` 的行一律不计。代码在 $\mathrm{low}(\mathrm{name})$ 上判断，因此 `Main`、`MAIN` 也被排除——这与引理 1 中"名字忽略大小写"的身份规则一致（在该规则下 `MAIN` 与 `main` 本就是同一个函数，必须一并不计）。而其它行的 key 从未经过这个分支，插入逻辑不受影响，故集合内容只少了 `main` 对应的类。证毕。

**定理**：程序输出的整数等于题面所求的"不完全相同的函数"个数。

*证明*：由引理 3 每个非 `main` 行被正确地规范化，由引理 2 规范形与身份判定等价，由引理 4 集合大小等于类数，由引理 5 `main` 被正确剔除，由引理 1 类数等于所求个数。证毕。

**复杂度**：设总输入长度为 $L$、行数为 $n$。每行做一次线性解析（`getline`、`find`、按 `,` 切分、两次 token 化），代价与该行长度成正比；每次插入 `set<string>` 的代价是 $O(\log n)$ 次字符串比较，单次比较最坏 $O(255)$。总时间 $O(L\log n)$，空间 $O(L)$（存下所有不同的 key）。在题面规模下远低于时限。

---

## 三、模式总结（按实际思考顺序）

- explore its property — "同一个函数"被题面定义成一条等价关系：名字忽略大小写 + 参数类型序列相同，形参名无关
- reduce to easier problem — 把"两两比较"换成"给每行发一个规范形 key"，去重变成集合大小问题
- discuss by category — 把一行按标点骨架拆成三段：`(` 之前（返回类型 + 函数名）、括号之间（参数表）、`)` 之后（`;`，丢弃）
- explore its property（第二次） — 标点结构的两个"唯一性"：第一个 `(` 必在函数名之后（标识符不含括号）⇒ 最后一个 token 是函数名；参数表内无括号 ⇒ 第一个 `)` 就是右端
- classic problem solution — `std::set<std::string>` 去重，输出 `size()`
- prove the promise's role — "每个函数至少调用一次"才使得"数不同调用签名"等于"数函数个数"
- handle boundaries — 空参数表、`main`（小写化后再判）、`cin>>n` 后要 `getline` 吞换行、行尾 `\r`、行尾 `;`
- verify the canonical form's injectivity — 类型只可能是 `int`/`char`，不含 `(` `)` `,`，所以拼接是单射（否则 key 相同推不出身份相同）

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 | 初值 / 记忆点 |
|------|------|------|----------------|
| $n$ | 调用行数 | `int` | 输入 |
| `line` | 当前读入的一整行 | `string` | 每轮 `getline` 覆盖，行尾空白被剥掉 |
| `S` | 所有不同 key 的集合 | `set<string>` | 全局，空；最终 `size()` 即答案 |
| `lp` | 行内第一个 `(` 的下标 | `size_t` | `find('(')` |
| `rp` | 参数表右括号下标 | `size_t` | `find(')',lp)`；找不到时为 `npos` |
| `head` | `(` 之前的子串（返回类型 + 函数名） | `string` | 只用来取名字 |
| `body` | 括号之间的子串（参数表） | `string` | 按 `,` 切成若干形参 |
| `name` | 小写化后的函数名 | `string` | `head` 的最后一个 token 再 `low` |
| `key` | 规范形 `name(类型,类型,...)` | `string` | 插入 `S` |
| `cur` | 正在累积的一个形参片段 | `string` | 遇 `,` 或到达末尾时结算 |
| `tok(t)` / `low(t)` | 按空白切 token / 转小写 | 函数 | 都返回新的 `string`/`vector` |

### 算法步骤

1. 读入 $n$，然后 `getline` 一次，吃掉第一行行尾残留。
2. 循环 $n$ 次：`getline` 读一整行；剥掉行尾的 `\r`、空格、制表符。
3. 求 `lp=find('(')`、`rp=find(')',lp)`；`head=substr(0,lp)`，`body=` 括号之间的内容。
4. 对 `head` 做 token 化，取 `tk.back()` 并小写化得到 `name`；若 `name=="main"` 直接跳过本行。
5. 把 `body` 按 `,` 切段，每段 token 化后取第一个 token（有的话）小写化，作为该参数的类型，依次拼进 `key`（段间补 `,`）。
6. 拼出 `key = name + "(" + 类型串 + ")"`，插入 `S`。
7. 输出 `S.size()`。

### 复杂度

- 时间：$O(L\log n)$，$L$ 为输入总长度（每行线性解析 + 一次 `set` 插入的 $\log$ 次比较）。
- 空间：$O(L)$（存下所有不同的 key）。

### 状态的一句话验证

`key` 是引理 2 意义下的**单射规范形**，`set` 因此把"等价类"数成了"元素个数"；`main` 的判断放在小写化之后，与"名字忽略大小写"的身份规则保持同一口径。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2373 yyy2015c01 的 IDE 之 Call Stack
// 把每行规范化成 key = 小写函数名(参数类型1,参数类型2,...)
// 形参名、返回类型、空格全都丢掉；main 一律跳过；set 去重后输出大小
#include<bits/stdc++.h>
using namespace std;

set<string> S;

vector<string> tok(const string&t)
{
	vector<string>r;
	istringstream in(t);
	string w;
	while(in>>w)
		r.push_back(w);
	return r;
}
string low(string t)
{
	for(char&c:t)
		c=(char)tolower(c);
	return t;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	string line;
	getline(cin,line);            //吃掉第一行行尾
	for(int i=0;i<n;++i)
	{
		getline(cin,line);
		while(!line.empty()&&(line.back()=='\r'||line.back()==' '||line.back()=='\t'))
			line.pop_back();
		size_t lp=line.find('(');
		size_t rp=lp==string::npos?string::npos:line.find(')',lp);
		string head=lp==string::npos?line:line.substr(0,lp);
		string body=(lp==string::npos||rp==string::npos)?"":line.substr(lp+1,rp-lp-1);
		auto tk=tok(head);
		if(tk.empty())
			continue;               //空行，不是调用
		string name=low(tk.back()); //返回类型丢弃，函数名取 '(' 前最后一个 token
		if(name=="main")
			continue;               //main 不计入
		string key=name+'(';
		string cur;
		bool first=true;
		for(size_t k=0;k<=body.size();++k)
		{
			if(k==body.size()||body[k]==',')
			{
				auto pt=tok(cur);   //一个形参：pt[0] 是类型，后面是形参名
				if(!pt.empty())
				{
					if(!first)
						key+=',';
					key+=low(pt[0]);
					first=false;
				}
				cur.clear();
			}
			else
				cur+=body[k];
		}
		key+=')';
		S.insert(key);
	}
	cout<<S.size()<<'\n';
	return 0;
}
```

### 代码说明

- `tok` 用 `istringstream` 切 token，天然吃掉任意多空格，比手写下标扫描少踩坑。
- `name=low(tk.back())`：一旦 `(` 前有多个 token，只有最后一个是函数名，返回类型被"顺手"丢掉——这正是引理 3(2)。
- 循环写到 `k<=body.size()`，让"最后一个形参"和"末尾的空段"共用同一段结算代码；`if(!pt.empty())` 挡住空参数表与多余的逗号。
- 参数类型也做了 `low`：题面只给了 `int`/`char` 两种类型，正常情况下大小写一致，这一步是无害的防御；真正的关键在函数名。
- `main` 判断放在小写化之后，所以 `Main` 也会被排除（引理 5）。若数据中存在名字就是 `MAIN` 的普通函数，按题面的"忽略大小写"规则它本就等于 `main`，排除是自洽的。
- 如果某行没有 `(` 或 `)`（格式外的情况），`head`/`body` 走兜底分支，`key` 退化成"整行小写后的名字加空括号"，不会越界。

### 验证状态

- 编译：`/d/mingw64/bin/g++.exe -std=c++20 -O2` 通过，无错误。
- 题面未给出样例数据，自造数据验证：`2 / int fac1(int n); / int fac1(char n);` → `2`；`2 / int Fac1(int n,int m); / int FAC1(int x,int y);` → `1`；`4 / int f(int a); / int F(char b); / int f(int c,int d); / int f(int ,int  );` → `3`；`3 / int main(); / int f(int a); / int MAIN();` → `1`；`3 / fac1(int n); / int fac1 ( int m ) ; / int FAC1(char  k);`（缺返回类型、括号前后有空格）→ `2`；全部是 `int main();` 时 → `0`；`\r\n` 换行的数据 → 结果不变。
- 对拍：`.scratch/P2373/` 中写了一版随机数据生成器，它在造数据时自己维护"不同 (小写名, 类型表)"的集合，直接把该集合大小作为标准答案（与解析过程完全独立），与标程在 400 组随机数据（$n\le12$，名字随机大小写、参数 0~3 个、随机多空格、随机插入 `int main()`）上逐一比对，全部一致；对拍目录已删除。
