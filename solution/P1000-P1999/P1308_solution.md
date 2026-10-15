# P1308 [NOIP 2011 普及组] 统计单词数 — 题解

## 1. 重述题目

给定两个字符串：
- 第 1 行：单词 `s`，只含字母，长度 $1\le|s|\le 10$；
- 第 2 行：文章 `t`，只含字母和空格，长度 $1\le|t|\le 10^6$。

要求：
1. **完全匹配**：`s` 必须与 `t` 中的**独立单词**完全相同（不区分大小写）。独立单词指被空格或字符串边界夹住的连续字母串。若 `s` 仅是某个单词的一部分，则不算匹配。
2. 输出 `s` 在 `t` 中作为独立单词出现的**次数**和**第一次出现的位置**（首字母在 `t` 中的下标，从 0 开始）。若未出现，输出 `-1`。
3. 空格也占一个字符位置。

---

## 2. 思路链（Chain-of-thought）

### 2.1 探索特殊机制：什么是"独立单词"

先看样例来精确理解"独立单词"这个概念：

- **样例 1**：`s = "To"`, `t = "to o t"`。输出 `2 0`。
  - `t` 被空格切分成 `["to", "o", "t"]`。
  - 不区分大小写后：`["to", "o", "t"]`。
  - `s = "to"` 匹配到 `to`（位置 0），不匹配 `o`，也不匹配 `t`（长度不同）。
  - 所以答案是 1 次，第一次位置 0。但输出是 `2 0`？

  等等，让我重新看样例。题目说输出是"次数和第一次出现的位置"，样例 1 输出 `2 0`。

  让我重新理解。实际上题目样例应该是：
  - `s = "to"`, `t = "to o t"`
  - 切分：`to`(位置 0), `o`(位置 3), `t`(位置 6)
  - `to` 不区分大小写等于 `to` → 匹配，位置 0
  - `o` 长度 1 ≠ 2 → 不匹配
  - `t` 长度 1 ≠ 2 → 不匹配
  - 应该输出 `1 0`。

  看来我对样例的理解有误。题目原文说"参见样例 1"和"参见样例 2"来解释完全匹配和部分的区分，但没有直接给出样例数据。我不猜测样例具体数值，而是严格依据题面定义来分析。

  关键定义：**给定单词必须与文章中的某一独立单词在不区分大小写的情况下完全相同**。

  所以核心机制是：
  1. 将文章 `t` 按空格分割成若干"token"（连续字母串）；
  2. 每个 token 与 `s` 做不区分大小写的完整比较；
  3. 只有**长度相同**且**逐字符不区分大小写相同**的 token 才算匹配。

  这意味着：
  - 长度检查是**必要前置**（快速过滤）；
  - 比较是**逐字符不区分大小写**（`tolower` 后比较）；
  - 位置记录的是 token 在 `t` 中的起始下标（0-based）。

### 2.2 考虑更简单的版本：忽略"独立单词"要求

如果去掉"独立单词"的限制，退化为子串匹配问题——找 `s` 在 `t` 中（忽略大小写）出现的位置和次数。这是经典的 KMP / `string::find` 问题。

但本题**不是**子串匹配。`s = "to"` 不应匹配 `t` 中的 `"toast"`（`to` 是 `toast` 的前缀但不是独立单词）。

区别在于：子串匹配看的是 `t` 中任意位置的连续子串，而本题看的是 `t` 中**由空格分隔的 token**。

### 2.3 经典解法：按空格分割 + 逐 token 比较

既然匹配单元是"空格分隔的 token"，那最直接的做法就是：

**扫描 `t`，提取每个 token 及其起始位置，与 `s` 比较。**

扫描方式：从左到右遍历 `t`，记录：
- `start`：当前 token 的起始下标；
- `len`：当前 token 的长度。

遇到空格或到达末尾时，一个 token 结束，此时：
1. 若 `len == |s|`，逐字符不区分大小写比较 `t[start..start+len-1]` 与 `s`；
2. 若匹配，`cnt++`，若 `first` 尚未记录则 `first = start`。

然后 `start` 重置为当前位置 + 1，`len` 清零。

### 2.4 证明：此方法正确性

**引理 1**：按空格分割 `t` 得到的 token 序列恰好是 `t` 中的所有独立单词。

*证明*：
- 设 `t = c_0 c_1 \cdots c_{n-1}`，其中 $c_i \in \{\text{字母}, \text{空格}\}$。
- 定义 token 的起止：一个 token 是从一个字母开始，到下一个空格或字符串末尾之前的连续字母串。
- 我们证明 token 序列 $\{tok_0, tok_1, \ldots, tok_{k-1}\}$ 恰好是 $t$ 中所有独立单词。
- （充分性）每个 $tok_j$ 是连续字母串，且左右两侧要么是空格要么是字符串边界，因此 $tok_j$ 是独立单词。
- （必要性）设 $w$ 是 $t$ 中的一个独立单词，即 $w$ 是 $t$ 中某个连续字母子串 $c_a c_{a+1} \cdots c_b$（$a \le b$），满足 $a=0$ 或 $c_{a-1} = $ 空格，$b = n-1$ 或 $c_{b+1} = $ 空格。
- 在我们的扫描中，当 $i = a$ 时（$c_a$ 是字母），$start$ 被设为 $a$（因为前一个位置是空格或开头，$start$ 在重置后恰好指向 $a$）。随着 $i$ 推进，$len$ 递增，直到 $i = b$ 时 $len = b - a + 1 = |w|$。当 $i = b+1$（空格或 $i = n$）时，token 被提取，其内容恰好是 $c_a \cdots c_b = w$。
- 因此每个独立单词 $w$ 都对应恰好一个 token。

**引理 2**：一个 token $tok$ 与 $s$ 匹配（不区分大小写的完全匹配）当且仅当 $|tok| = |s|$ 且对每个 $j \in [0, |s|-1]$，$\text{tolower}(tok[j]) = \text{tolower}(s[j])$。

*证明*：
- 这是"不区分大小写的完全匹配"的定义展开：
  - 长度必须相等（否则不可能是同一单词）；
  - 逐字符忽略大小写后必须相同。
- 充分性：若长度相等且逐字符 tolower 后相等，则 $tok$ 和 $s$ 不区分大小写地完全相同，匹配。
- 必要性：若 $tok$ 和 $s$ 不区分大小写地完全相同，则长度必然相等且逐字符 tolower 后相等。$\square$

**引理 3**：算法对每个 token 至多做一次匹配检查，总时间复杂度为 $O(|t| + |s| \cdot |t|/|s|)$，最坏 $O(|t| \cdot |s|)$。

*证明*：
- 扫描 `t` 的每个字符恰好一次，用于维护 `start` 和 `len`，花费 $O(|t|)$。
- 每个 token 的长度为 $len$，与 `s` 比较需要 $O(\min(len, |s|))$ 时间（长度不等时 $O(1)$ 直接跳过，长度相等时 $O(|s|)$）。
- 设 token 序列为 $tok_1, \ldots, tok_k$，则总比较时间为 $\sum_{j=1}^{k} O(\min(|tok_j|, |s|)) \le O(|s| \cdot k)$。
- 最坏情况下每个 token 长度恰好等于 $|s|$（例如 `t = "ab ab ab ..."` 且 $s = "ab"$），此时 $k = |t|/|s|$，总比较时间为 $O(|t|)$。
- 若 token 长度远大于 $|s|$（不可能，因为 token 是连续字母串，长度可以任意大），比较在 $O(|s|)$ 时即可终止。
- 综合：$O(|t| + |s| \cdot k)$，最坏 $O(|t| + |s| \cdot (|t|/|s|)) = O(|t|)$（当所有 token 长度 $= |s|$ 时），或 $O(|t| + |s| \cdot |t|)$ 在极端情况下（每个 token 长度 $\ge |s|$ 时，$k$ 可以小到 1，此时总比较 $O(|s|)$）。

  更精确地：$\sum_{j=1}^{k} |tok_j| = |t| - (\text{空格数}) \le |t|$。对长度 $\le |s|$ 的 token，比较代价 $\le |tok_j|$；对长度 $> |s|$ 的 token，比较代价 $= |s|$。所以总比较代价 $\le \sum_{|tok_j|\le|s|} |tok_j| + |s| \cdot \#\{|tok_j|>|s|\} \le |t| + |s| \cdot k$。由于 $\sum |tok_j| \le |t|$ 且 $k \le |t|$，最坏总代价 $O(|t| + |s| \cdot |t|)$。但 $|s| \le 10$，所以最坏 $O(10 \cdot |t|) = O(|t|)$。

**定理**：算法正确输出匹配次数和第一次出现位置（或未出现时输出 -1）。

*证明*：
- 由引理 1，算法枚举了所有独立单词；
- 由引理 2，算法对每个独立单词正确判定是否与 `s` 匹配；
- 计数 `cnt` 累加所有匹配，正确；
- `first` 记录第一个匹配 token 的 `start`，由于扫描从左到右，`start` 递增，第一个匹配即为最靠左的匹配，正确；
- 若 `cnt = 0`，输出 -1，正确。$\square$

### 2.5 边界情况

1. **`t` 以空格开头或结尾**：例如 `t = "  hello  "`。扫描时空格不增加 `len`，token 提取时 `len` 可能为 0（连续空格），此时跳过。算法自然处理：`len == 0` 时不做比较。
2. **连续多个空格**：同上，`len` 不增长，不产生空 token。
3. **`s` 长度 1**：每个单字母 token 都要比较，正确。
4. **大小写混合**：`tolower` 处理，正确。
5. **`s` 在 `t` 中作为子串出现但不是独立单词**：如 `s = "to"`, `t = "toast"`。`toast` 是一个长度为 5 的 token，长度 ≠ 2，跳过。正确。

### 2.6 算法步骤总结

1. 读入 `s` 和 `t`。
2. 对 `t` 的每个字符预计算 `tolower` 版本（或直接比较时用 `tolower`）。
3. 从左到右扫描 `t`：
   - 维护 `start`（当前 token 起始下标）和 `len`（当前 token 长度）。
   - 字母：若 `len == 0` 则 `start = i`；`len++`。
   - 空格或末尾：若 `len == |s|`，逐字符比较 `t[start..]` 与 `s`（tolower）。匹配则 `cnt++`，若 `first == -1` 则 `first = start`。重置 `len = 0`。
4. 输出结果。

---

## 3. 模式总结（Pattern Summary）

```
- explore its property — "独立单词"= 空格分隔的 token；匹配 = 长度相等 + 逐字符 tolower 相同
- consider easier version — 去掉独立单词限制退化为子串匹配（KMP），但本题不需要
- classic problem solution — 扫描字符串提取 token + 逐 token 与 s 比较（string matching on tokens）
- reduce to easier problem — 每个 token 的比较退化为 O(|s|) 的逐字符 tolower 比较
- boundary cases — 前导/尾随空格、连续空格、len=0 的假 token 自然跳过
```

---

## 4. 正式解法（Real Solution）

### 4.1 符号表

| 符号 | 含义 | 初始化 |
|------|------|--------|
| `s` | 给定单词，只含字母 | `cin >> s` |
| `t` | 文章，含字母和空格 | `getline(cin, t)`（在读 `s` 后） |
| `ns` | `s` 的长度 | `s.size()` |
| `nt` | `t` 的长度 | `t.size()` |
| `start` | 当前 token 在 `t` 中的起始下标 | `0` |
| `len` | 当前 token 的长度 | `0` |
| `cnt` | 匹配次数 | `0` |
| `first` | 第一次匹配的起始位置 | `-1` |

**助记**：`start`/`len` 描述当前正在累积的 token；`cnt`/`first` 是最终输出。

### 4.2 算法步骤

1. 读入 `s`（用 `cin >> s`），读入 `t`（用 `getline`，注意 `cin >> s` 后先 `cin.ignore()` 吃掉换行符）。
2. 设 `ns = s.size()`。
3. 遍历 `i = 0` 到 `nt`：
   - 若 `i < nt` 且 `t[i]` 是字母：
     - 若 `len == 0`，设 `start = i`。
     - `len++`。
   - 否则（`i == nt` 或 `t[i]` 是空格）：
     - 若 `len == ns`：
       - 对 `j = 0` 到 `ns-1`：若 `tolower(t[start+j]) != tolower(s[j])`，标记不匹配。
       - 若匹配：`cnt++`；若 `first == -1`，`first = start`。
     - `len = 0`。
4. 若 `cnt == 0`，输出 `-1`；否则输出 `cnt` 和 `first`。

### 4.3 复杂度

- **时间**：$O(|t| + |s| \cdot k)$，其中 $k$ 为 token 数。由于 $|s| \le 10$，最坏 $O(10 \cdot |t|) = O(|t|)$。
- **空间**：$O(|t| + |s|)$（存储两个字符串）。

### 4.4 验证状态

- 手工验证样例场景（前导空格、连续空格、大小写混合、部分匹配不算）均正确。
- 未运行编译测试（按要求不编译）。

---

## 5. 示例代码（C++）

```cpp
#include<bits/stdc++.h>
using namespace std;

string s,t;

bool matchToken(int start,int ns)
{
	for(int j=0;j<ns;++j)
		if(tolower(t[start+j])!=tolower(s[j]))
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	cin.ignore();
	getline(cin,t);

	int ns=s.size();
	int nt=t.size();
	int cnt=0,first=-1;
	int start=0,len=0;

	for(int i=0;i<=nt;++i)
	{
		if(i<nt&&isalpha((unsigned char)t[i]))
		{
			if(len==0)
				start=i;
			++len;
		}
		else
		{
			if(len==ns)
			{
				if(matchToken(start,ns))
				{
					++cnt;
					if(first==-1)
						first=start;
				}
			}
			len=0;
		}
	}

	if(cnt==0)
		cout<<-1;
	else
		cout<<cnt<<' '<<first;
	cout<<'\n';
	return 0;
}
```

### 代码说明

- `cin>>s; cin.ignore(); getline(cin,t);`：`s` 不含空格所以用 `>>` 读；`cin.ignore()` 吃掉 `s` 后的换行符，然后 `getline` 读整行（含空格）作为 `t`。
- 扫描循环 `i` 从 `0` 到 `nt`（含 `nt`），在 `i == nt` 时触发最后一个 token 的结算（因为最后一个 token 后面没有空格）。
- `isalpha((unsigned char)t[i])`：判断是否为字母（`unsigned char` 防止负值 UB）。
- `matchToken`：长度已由调用者保证相等（`len == ns`），只需逐字符 tolower 比较。
- `tolower` 参数应传 `unsigned char` 或 `EOF` 以避免 UB，但此处字符均为 ASCII 字母，安全。
- 时间复杂度 $O(|t|)$（$|s| \le 10$ 为常数），空间 $O(|t|)$。
