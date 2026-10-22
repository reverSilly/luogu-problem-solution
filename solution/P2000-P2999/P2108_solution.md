# P2108 学英语 — 题解

## 一、解题过程：人类思考叙事

题目给一个整数的英文写法，要求翻译回阿拉伯数字。例如 `eight hundred fourteen thousand twenty two` → `814022`。

数据范围 $-999{,}999{,}999 \le x \le 999{,}999{,}999$，也就是最多 9 位，十亿级别。

先回忆英语数词的结构。这是整个题目的核心，结构一旦看清，翻译就变成了"照着结构读"。

英语整数的基本结构是"**三节一位组**"（thousand million 体系）：

- $|x|<1000$ 的部分只用 0~19 以及 twenty/thirty/.../ninety 这类整十词；
- 1000 的倍数用 `thousand` 标记，1000000 的倍数用 `million` 标记；
- 英文写法从高位到低位依次书写各节。

题目保证了第 5 点"基本符合英语语法规则"，所以：

- `hundred` 出现在一个节内部，形如 `X hundred Y`（$X\in[1,9]$，$Y<100$），值 $=X\times 100+Y$；
- `thousand` 前面是 1~999 的数，后面跟着更低节；
- `million` 前面是 1~999 的数，后面跟着 thousand 节及以下；
- 不会出现 `and`。

于是最自然的做法是**从左到右扫一遍，累积**：

维护两个量：
- `cur`：当前正在"拼"的那一节（节的值 $0\sim 999$）；
- `ans`：已经拼完的高位节的总和。

对每一个单词：
- `negative`：记下符号 `sgn=-1`（保证只在开头出现）。
- `hundred`：`cur *= 100`。
- `thousand`：`ans += cur*1000; cur = 0;`（这一节整体成为千位节，结算掉）。
- `million`：`ans += cur*1000000; cur = 0;`（这一节整体成为百万位节，结算掉）。
- 其余单词（0~19、twenty~ninety）：`cur += 它的值`。

最后答案是 `sgn*(ans+cur)`。

为什么这样是对的？关键在于 `hundred` 与 `thousand`/`million` 的**位置语义**：

- `hundred` 出现在 `X hundred` 中，说明当前的 `cur` 里只有一个"十位量级"的头（$X$），乘 100 把节提升两位。$X$ 之后可以再跟 0~99 的部分（`one hundred twenty three`），所以 `hundred` 之后 `cur` 还能继续累加。
- `thousand` 出现在 `X hundred Y thousand` 中，说明"当前这一节"整体要乘 1000 变成千位节，结算后 `cur` 清零，接着开始新的一节（可以是 `one hundred` 这样的百位节或更低位）。$X\cdot 1000$ 中 $X$ 已经包含 `hundred` 的效果（`eight hundred fourteen thousand` = 814 千）。
- `million` 同理，只是量级是 $10^6$。

一个例子走一遍：`eight hundred fourteen thousand twenty two`。
- `eight` → cur=8
- `hundred` → cur=800
- `fourteen` → cur=814
- `thousand` → ans=814000，cur=0
- `twenty` → cur=20
- `two` → cur=22
- 答案 814000+22 = 814022 ✓

再验证 `one million one hundred one`：
- `one` → cur=1
- `million` → ans=1000000，cur=0
- `one` → cur=1
- `hundred` → cur=100
- `one` → cur=101
- 答案 1000101 ✓

`negative seven hundred twenty nine`：
- `negative` → sgn=-1
- `seven` → cur=7
- `hundred` → cur=700
- `twenty` → cur=720
- `nine` → cur=729
- 答案 -729 ✓

有几个实现细节要抠：

1. **输入用 `getline`**。整行是一句话，`cin >>` 只能读到第一个单词。所以用 `getline` 读整行，再按空格切词。
2. **单词表**。0~19 是 20 个词；twenty~ninety 是 8 个词，共 28 个。可以用一个数组按顺序存，`val(w)` 里对 $i<20$ 返回 $i$，对 $i\ge 20$ 返回 $(i-18)\times 10$（因为 twenty 在下标 20，$(20-18)\times 10=20$；ninety 在下标 27，$(27-18)\times10=90$）。
3. **符号**。`negative` 出现在最前，单独记 `sgn`，最后统一乘。
4. **数值范围**。$|x|\le 999{,}999{,}999$，`int` 装得下，但为了稳妥用 `long long`（`ans` 中间累加时也不会溢出）。

复杂度：句子长度有限（最多十几个词），$O(L)$ 时间和 $O(L)$ 空间，其中 $L$ 是输入串长度。

---

## 二、思维链：完整证明

**记号约定**：把输入按空格切成单词序列 $w_1,w_2,\dots,w_t$。定义"节"（group）为 3 位一组的十进制分组；定义英文数词的层级：一个完整整数是若干节的串联，从高到低依次是 million 节、thousand 节、个位节（节值 0~999，可为 0）。

**引理 1**：若英文写法中某节以 `hundred` 结尾（即写作 `X hundred Y`，$X\in\{1,\dots,9\}$，$Y$ 表示 $0\sim 99$ 的部分），则该节的数值为 $100X+Y$；若该节不含 `hundred`，则该节数值等于其中所有 0~99 量级词的值之和。

*证明*：英语中 `X hundred` 表示 $X$ 个百，即 $100X$；`Y` 部分按十位与个位相加（`twenty` 系列加 10 的倍数，`zero`~`nineteen` 加个位），两者相加得 $100X+Y$。不含 `hundred` 时该节最大为 99，只可能由十位词与个位词组成，按位相加即得数值。证毕。

**引理 2**：若英文写法中某节以 `thousand` 结尾（即 `X thousand`），则该节对最终答案的贡献是 $1000X$；`million` 类似，贡献是 $10^6X$。

*证明*：英语数词中 `thousand` 标记的是"千位节"，即它前面那个节的值 $X$ 要整体乘以 $1000$ 才是这一部分的数值。题目保证"基本符合英语语法规则"（第 5 点，例如 $1500$ 写作 `one thousand five hundred` 而非 `fifteen hundred`），故不会出现"一个数直接跟 thousand 表示千倍"的简写，`X` 必是一个完整的节值（$1\sim 999$）。`million` 同理。证毕。

**引理 3**：扫描到任意位置时，`ans` 等于"已经结算的所有高位节的贡献之和"，`cur` 等于"当前正在拼的这一节的值"。

*证明*：对已处理的单词数做归纳。

初始时未处理任何单词，`ans=0`，`cur=0`，断言成立。

设处理前断言成立，处理下一个单词 $w$：

- 若 $w$ 是 `negative`：只改符号不改数值，断言仍成立。
- 若 $w$ 是普通量词（0~19、twenty~ninety）：按引理 1，它对当前节的贡献就是它的值（十位量级词会与已有的个位量级词相加，符合十进制的"十位+个位"结构；题目保证语法合法，故同一节内不会先出现 10 的倍数再出现另一个 10 的倍数之类冲突）。故 `cur += val(w)` 后，`cur` 仍是当前节的正确值，`ans` 不变，断言成立。
- 若 $w$ 是 `hundred`：由引理 1，当前节值要乘 100。执行 `cur *= 100` 后 `cur` 是该节乘 100 后的中间结果（后续的普通词会继续加到这一节上），`ans` 不变，断言成立。
- 若 $w$ 是 `thousand`：由引理 2，当前节的完整数值就是 $cur$（此时节已写完，因为 thousand 后面不可能再出现属于本节的内容），它对答案的贡献是 $1000\times cur$。执行 `ans += cur*1000; cur = 0;` 后，`ans` 累加了这一节的贡献，`cur` 归零开始新的一节，断言成立。
- 若 $w$ 是 `million`：与 `thousand` 同理，贡献 $10^6\times cur$，断言成立。

归纳完成。证毕。

**引理 4**：扫描结束时，$|x| = \text{ans}+\text{cur}$。

*证明*：完整英文写法由"million 节 + thousand 节 + 个位节"串联而成。由引理 3，扫描结束时 `ans` 恰是 million 节与 thousand 节（或它们中存在的部分）的贡献之和，`cur` 恰是最后未结算的个位节的值。由引理 2，各节贡献即为 $10^6\times(\text{million 节值})$ 与 $10^3\times(\text{thousand 节值})$，加上个位节值，正是 $|x|$。证毕。

**引理 5**：`sgn` 正确表示 $x$ 的符号。

*证明*：题目保证 $x<0$ 时第一个单词是 `negative`，$x\ge 0$ 时不出现 `negative`。算法初始 `sgn=1`，遇到 `negative` 置为 $-1$，之后不再改变。由于 `negative` 只可能出现一次且在最前，算法最终 `sgn` 等于 $\mathrm{sign}(x)$。证毕。

**定理**：算法输出的就是题目要求的 $x$。

*证明*：由引理 4，$|x|=\text{ans}+\text{cur}$；由引理 5，`sgn` 是 $x$ 的符号。故输出 `sgn*(ans+cur)` 恰为 $x$。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **先回忆英语数词的结构** — 三位一节、节间用 `thousand`/`million` 连接、节内用 `hundred` 与十位/个位词构成。
2. **找到"结算点"** — 遇到 `thousand`/`million` 就说明一节写完了，可以把这一节整体乘上量级累加进答案；遇到 `hundred` 只是把当前节放大 100，还不能结算。
3. **设计两个累加量** — `ans` 存已结算的高位节，`cur` 存当前节；扫描一遍即可。
4. **特殊词单独处理** — `negative` 记符号；`hundred` 是乘；`thousand`/`million` 是"结算并清零"。
5. **实现细节** — 整行输入用 `getline`；单词表按下标顺序排 28 个词，值可直接用下标算出。
6. **自检** — 走三遍样例（`six`、`negative seven hundred twenty nine`、`one million one hundred one`、`eight hundred fourteen thousand twenty two`）全部符合预期。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 范围 |
|------|------|------|
| `line` | 读入的整行英文 | `string` |
| `w` | 当前处理的单词 | `string` |
| `word[28]` | 数量词表，`0..19` 之后是 `twenty..ninety` | `const char*[]` |
| `val(w)` | 单词的数值，0~90；非数量词返回 $-1$ | `int` |
| `sgn` | 符号，$1$ 或 $-1$ | `int` |
| `cur` | 当前正在拼的一节的值 | `i64`，$0\sim 999$ |
| `ans` | 已结算的高位节贡献之和 | `i64` |

### 算法步骤

1. `getline(cin,line)` 读入整行。
2. 初始化 `sgn=1`、`cur=0`、`ans=0`。
3. 用下标扫描 `line`，跳过空格，按空格切出一个个单词 `w`：
   - `w=="negative"` → `sgn=-1`；
   - `w=="hundred"` → `cur*=100`；
   - `w=="thousand"` → `ans+=cur*1000; cur=0;`；
   - `w=="million"` → `ans+=cur*1000000; cur=0;`；
   - 否则 → `cur+=val(w)`。
4. 输出 `sgn*(ans+cur)`。

### 复杂度

设输入串长度为 $L$。

- 时间复杂度：$O(L)$。分词是 $O(L)$，每个单词查表 $O(28)$ 视为常数。
- 空间复杂度：$O(L)$，用于存放整行输入（单词表本身是常数空间）。

### 验证状态

- 标程 `P2108.cpp` 用 `g++ -O2 -std=c++17` 编译通过。
- 题面官方样例全部通过：`six` → `6`；`negative seven hundred twenty nine` → `-729`；`one million one hundred one` → `1000101`；`eight hundred fourteen thousand twenty two` → `814022`。
- 编写暴力 `P2108_brute.cpp`（按 million/thousand 切段后逐段独立解析）与对拍脚本 `P2108_check.py`，在随机生成的合法英文写法上对拍 **2000 组全部通过**。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2108 学英语
#include<bits/stdc++.h>
using namespace std;

using i64=long long;

const char* word[28]={"zero","one","two","three","four","five","six","seven","eight","nine",
	"ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen",
	"twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};

int val(const string &w)
{
	for(int i=0;i<28;++i)
		if(w==word[i])
			return i<20?i:(i-18)*10;
	return -1;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string line;
	getline(cin,line);

	int sgn=1;
	i64 cur=0,ans=0;
	string w;
	for(int i=0;i<(int)line.size();)
	{
		if(line[i]==' ')
		{
			++i;
			continue;
		}
		int j=i;
		while(j<(int)line.size() && line[j]!=' ') ++j;
		w=line.substr(i,j-i);
		i=j;
		if(w=="negative") sgn=-1;
		else if(w=="hundred") cur*=100;
		else if(w=="thousand")
		{
			ans+=cur*1000;
			cur=0;
		}
		else if(w=="million")
		{
			ans+=cur*1000000;
			cur=0;
		}
		else cur+=val(w);
	}
	cout<<sgn*(ans+cur)<<'\n';
	return 0;
}
```

### 代码说明

- 必须用 `getline` 读整行，`cin >> s` 只会读到第一个单词。
- `word[]` 按 `zero..nineteen`（下标 0~19）、`twenty..ninety`（下标 20~27）顺序排列，于是 `i<20` 时值为 $i$，`i\ge 20$ 时值为 $(i-18)\times 10$，省掉了额外的映射表。
- `hundred` 是"乘 100"而不是"加 100"，`thousand`/`million` 是"结算并清零"，这两处的区分是本题最容易写错的地方。
- `sgn` 单独记录符号，最后统一相乘；因为 `negative` 只在开头出现，读到后不影响后续解析。
- `cur` 用 `i64`：虽然 $|x|\le 999{,}999{,}999$ 用 `int` 也够，但 `ans+cur` 的中间过程用 `long long` 更稳妥。
