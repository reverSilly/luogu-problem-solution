# P1906 凯撒密码

## 1. 重新读题

### 1.1 问题重述（人话版）

Caesar 用「移位密码」加密一句话：把明文里每个字母换成字母表中它**后面第 $t$ 个**字母（循环，$A\to Z$ 之后回到 $A$），非字母字符（空格、标点、数字等）不变。$t$ 在整份输入里是**固定的**，但题目没有直接告诉你 $t$ 是多少。

输入由若干段组成，每段格式为

```
START
<一行密文>
END
```

整个文件以一行 `ENDOFINPUT` 结束。要求对每段密文输出解密后的明文，并且**全部转成大写**。

### 1.2 关键约束与规模暗示

- 输入文件小于 50 KB，$t$ 固定，数据保证由「字母频率」可以**唯一**确定 $t$。
- 提示直接给出了英文字母频率表：$E$ 出现频率最高（约 $12.68\%$），其次是 $T,A,O,I,N,S,R$。
- 这说明不能用穷举 $t$ + 「看起来像不像英文」的模糊判断；正确做法是**统计密文中出现次数最多的字母，它对应明文里的 $E$**。
- 多段密文共享同一个 $t$，所以频率要在**所有密文合并**后统计，而不是每段各自猜一个 $t$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先把加密方向弄清楚

题目说：明文字母 $c$ 被替换成「它后面的第 $t$ 个字母」。也就是

$$
\text{cipher}=(c+t)\bmod 26 .
$$

那么解密就是

$$
\text{plain}=(\text{cipher}-t)\bmod 26 .
$$

样例 $t=2$：`I Need Soldiers` 加密成 `K Pggf Uqnfkgtu`。验算 `I`$(8)\to$ `K`$(10)$，`N`$(13)\to$ `P`$(15)$，都对。反向 `K`$(10)-2=$`I`$(8)$ 也对。

### 2.2 第一个想法：枚举 $t$，人眼挑「最像英文」的

枚举 $0\le t<26$，解出 26 份候选，再挑一份「看起来最像英文」的。问题是程序无法「看」，而且多个 $t$ 可能都能解出通顺片段，判据不唯一。题目明确提示了频率表，就是让我们用统计判据。

### 2.3 频率法及其正确性

**引理 1**：设输入中所有字母（不分大小写）的总频数中，出现次数最多的字母为 $c^\*$。若该段文本足够「成文」，则 $c^\*$ 就是密文中的 $E$，从而

$$
t=(c^\*-E)\bmod 26 .
$$

**证明思路**：加密是一个**固定的字母置换**：所有字母整体平移 $t$ 位。因此「哪个字母出现最多」这个性质只取决于明文里哪个字母最多，与 $t$ 无关——明文里最多的字母是 $E$，加密后变成 $E+t$，它在密文里仍然最多。于是 $c^\*=E+t$，即 $t=c^\*-E$。题目额外保证「数据保证这一特性可以唯一确定 $t$」，所以用最大频数字母反推即可。$\square$

注意这个论证依赖「频数排序在整体平移下不变」，这是严格成立的：平移是双射，它把每个明文字母的计数原样搬到对应密文字母上。

### 2.4 大小写与输出格式

- 统计频数时统一转成大写（或统一转小写），因为 `A` 和 `a` 是同一个字母。
- 解密后题目要求**全部大写**输出，所以解密时可以直接把每个字母当成大写处理：`(c-'A'-t+26)%26+'A'`。小写字母先转大写再解密，结果一致。
- 非字母字符原样保留（空格、标点）。

### 2.5 读入细节

按行读入。看到 `START` 就再读一行作为密文，然后读掉一行 `END`；看到 `ENDOFINPUT` 结束。要注意 Windows 换行可能留下 `\r`，需要剥掉，否则字符串比较 `==\"START\"` 会失败。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 移位加密是字母表的整体平移，字母频数排序不变。
- **classic problem solution** — 统计密文字母频率，最大频数字母对应明文 $E$，反推出 $t$。
- **reduce to easier problem** — $t$ 已知后，解密就是逐字符减去 $t$ 再转大写。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `txt` | `vector<string>` | 保存所有密文行 | 空 | texts |
| `cnt[26]` | `int [26]` | 各字母（不分大小写）出现次数 | 0 | count |
| `best` | `int` | 出现次数最多的字母编号（$0=A$） | 0 | best letter |
| `t` | `int` | 推出来的移位量 | 由 `best` 算出 | shift |
| `line` | `string` | 读入缓冲 | — | 行 |

### 4.2 算法步骤

1. 逐行读入：
   - 若整行等于 `ENDOFINPUT`，结束读入；
   - 若整行等于 `START`，读下一行存进 `txt`，再读一行（`END`）丢弃；
   - 其他行忽略（容错）。
2. 遍历 `txt` 中所有字符，若是字母则 `++cnt[toupper(c)-'A']`。
3. 找 `cnt` 的最大值位置 `best`；计算 $t=(best-4+26)\bmod 26$（`E` 的编号是 $4$）。
4. 对每行密文逐字符解密：字母（大写化后）做 `(c-'A'-t+26)%26+'A'`，非字母不变；输出该行。

### 4.3 复杂度分析

- **时间复杂度**：$O(\text{输入总长度})$。
- **空间复杂度**：$O(\text{输入总长度})$（保存所有密文行）。

---

## 5. 示例代码

```cpp
//
// 题目：P1906 凯撒密码
// https://www.luogu.com.cn/problem/P1906
#include<bits/stdc++.h>
using namespace std;

int cnt[26];
vector<string> txt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string line;
	while(getline(cin,line))
	{
		if(!line.empty()&&line.back()=='\r')
			line.pop_back();
		if(line=="ENDOFINPUT")
			break;
		if(line=="START")
		{
			getline(cin,line);
			if(!line.empty()&&line.back()=='\r')
				line.pop_back();
			txt.push_back(line);
			getline(cin,line);
			if(!line.empty()&&line.back()=='\r')
				line.pop_back();
		}
	}

	for(const string&s:txt)
		for(char c:s)
			if(isalpha((unsigned char)c))
				++cnt[toupper((unsigned char)c)-'A'];

	int best{0};
	for(int i=1;i<26;++i)
		if(cnt[i]>cnt[best])
			best=i;

	int t=((best-('E'-'A'))%26+26)%26;

	for(const string&s:txt)
	{
		string o;
		for(char c:s)
		{
			if(c>='A'&&c<='Z')
				o+=(char)('A'+(c-'A'-t+26)%26);
			else if(c>='a'&&c<='z')
				o+=(char)('A'+(c-'a'-t+26)%26);
			else
				o+=c;
		}
		cout<<o<<'\n';
	}
	return 0;
}
```
