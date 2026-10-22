# P1770 万能的OIH搜索

## 1. 重新读题

给定两行字符串：第一行为查询关键词 $S_1$，第二行为返回的题目文本 $S_2$。两行均由汉字拼音（英文小写字母串）、空格和半角标点符号组成。

定义“词语”为**中间仅由空格分隔（不含任何标点符号）的连续两个或更多个汉字拼音**。若 $S_2$ 中的某个词语也在 $S_1$ 中作为词语出现过，则需要将其加粗（在前面加 `<b>`、后面加 `</b>`）。标点符号加不加粗均可，但必须满足：
1. **第一优先级**：添加的 `<b>...</b>` 标记对的数量尽可能少；
2. **第二优先级**：每对标记 `<b>` 与 `</b>` 之间的字符数尽可能少。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：二元词组（2-gram）充分性与跨标点合并法则

**引理 1（二元相邻拼音匹配充分性）**：
一个汉字拼音在 $S_2$ 中属于某个与 $S_1$ 匹配的“长度 $\ge 2$ 的词语”，当且仅当它属于某个与 $S_1$ 匹配的**长度恰好为 $2$ 的相邻拼音词组**！

**证明**：
任何长度为 $L \ge 2$ 的连续无标点拼音序列 $w_1 w_2 \dots w_L$，都可以分解为 $L - 1$ 个相邻二元词组 $(w_1, w_2), (w_2, w_3), \dots, (w_{L-1}, w_L)$ 的并集。若整段在 $S_1$ 中出现，则其中的每个二元词组也必在 $S_1$ 中出现；反之，只需提取 $S_1$ 中所有无标点相邻二元词组集合 $\mathcal{P}$，扫描 $S_2$ 中每一对无标点相邻拼音 $(w_i, w_{i+1})$，若 $(w_i, w_{i+1}) \in \mathcal{P}$ 则将第 $i$ 个和第 $i+1$ 个汉字拼音均标记为“需加粗”（`bold[i] = bold[i+1] = true`），即可精确标出所有需加粗的汉字！$\blacksquare$

**引理 2（最小化标签对数且最小化包络长度的贪心构造）**：
设 $S_2$ 中共有 $K$ 个汉字拼音（按出现顺序编号为 $0 \dots K-1$），第 $i$ 个汉字拼音在原串 $S_2$ 中的字符起止区间为 $[L_i, R_i]$。将所有满足 `bold[i] == true` 的极大连续编号区间 $[a, b]$（即 `bold[a..b]` 全为真，且两端外侧为假或越界）分别用一个 `<b>...</b>` 包裹，其中 `<b>` 紧贴在 $L_a$ 之前、`</b>` 紧贴在 $R_b$ 之后。该方案同时达到“标签对数最少”和“每对标签内字符数最少”。

**证明**：
1. 为什么在极大连续加粗汉字区间 $[a, b]$ 内部，即使 $i$ 与 $i+1$ 之间隔着标点符号（例如 `bold[i]` 来自左边的词组，`bold[i+1]` 来自右边的词组），也必须合并进同一个 `<b>...</b>`？因为题面规定“标点符号加不加粗无所谓”，而汉字 $i$ 与 $i+1$ 之间没有任何不该加粗的汉字！把它们连同中间的标点合并成一个 `<b>...</b>` 能使标签对数严格减少 $1$（满足第一优先级）。
2. 反之，若两个加粗汉字之间隔着至少一个 `bold == false` 的汉字，则绝不能合并（否则会错误加粗不匹配的汉字）。因此，极大连续 `bold == true` 的汉字编号区间个数就是理论最少的 `<b>...</b>` 对数！
3. 对每个这样的极大区间 $[a, b]$，将 `<b>` 放在第 $a$ 个汉字的首字母 $L_a$ 处、`</b>` 放在第 $b$ 个汉字的末字母 $R_b$ 处，不包含区间外侧多余的空格或标点，使标签内的字符数达到理论最小（满足第二优先级）。$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将“任意长度 $\ge 2$ 的词语匹配”等价简化为“无标点间隔的相邻二元拼音对（2-gram）匹配”（引理 1）。
- **classic problem solution** — 标出 $S_2$ 中所有匹配的汉字拼音下标后，将极大连续加粗汉字下标段 $[a, b]$ 在原串区间 $[L_a, R_b]$ 处插入 `<b>` 与 `</b>`（引理 2）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `s1, s2` | `string` | 关键词行与题目文本行 | `getline` 读入 | input lines |
| `Tok` | `struct` | `{word, l, r, has_punc_after}` 表示一个汉字拼音及其在原串位置与右侧是否有标点 | 解析得出 | token |
| `pairs` | `set<pair<string,string>>` | $S_1$ 中所有无标点相邻二元词组集合 | 从 $S_1$ 提取 | 2-grams |
| `bold[i]` | `bool` | $S_2$ 中第 $i$ 个汉字拼音是否需要加粗 | 匹配标记 | bold flag |

### 4.2 算法步骤

1. 使用 `getline` 读入两行字符串 $S_1, S_2$（去除末尾可能的 `\r`）。
2. 将字符串解析为拼音 Token 序列：每个连续字母段 `[l, r]` 为一个拼音 Token；检查两个相邻拼音 Token 之间的字符是否全为空格（若含有非空格标点，则置 `has_punc_after = true`）。
3. 将 $S_1$ 中所有满足 `!tok1[i].has_punc_after` 的二元组 `(tok1[i].w, tok1[i+1].w)` 插入集合 `pairs`。
4. 扫描 $S_2$ 的 Token 序列：若 `!tok2[i].has_punc_after` 且 `(tok2[i].w, tok2[i+1].w)` 在 `pairs` 中，置 `bold[i] = bold[i+1] = true`。
5. 找出 `bold` 中所有极大连续为 `true` 的下标段 $[a, b]$，在原串 `s2` 的 `tok2[a].l` 处插入 `<b>`、在 `tok2[b].r` 之后插入 `</b>` 并输出。

### 4.3 复杂度分析

- **时间复杂度**：提取与查询二元组耗时 $O((|S_1| + |S_2|) \log |S_1|)$，耗时 $< 2\text{ ms}$。
- **空间复杂度**：存储至多 $1000$ 个 Token 及二元组集合，空间复杂度为 $O(|S_1| + |S_2|)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1770 万能的OIH搜索
// https://www.luogu.com.cn/problem/P1770
// AC
#include<bits/stdc++.h>
using namespace std;

struct Tok
{
	string w;
	int l,r;
	bool punc_after;
};

vector<Tok>parse(const string&s)
{
	vector<Tok>res;
	int n=s.size(),i=0;
	while(i<n)
	{
		if(isalpha((unsigned char)s[i]))
		{
			int l=i;
			while(i<n&&isalpha((unsigned char)s[i]))
				++i;
			res.push_back({s.substr(l,i-l),l,i-1,false});
		}
		else
			++i;
	}
	for(size_t k=0;k+1<res.size();++k)
	{
		for(int p=res[k].r+1;p<res[k+1].l;++p)
			if(s[p]!=' ')
				res[k].punc_after=true;
	}
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s1,s2;
	getline(cin,s1);
	if(!s1.empty()&&s1.back()=='\r')s1.pop_back();
	getline(cin,s2);
	if(!s2.empty()&&s2.back()=='\r')s2.pop_back();

	auto t1=parse(s1),t2=parse(s2);
	set<pair<string,string>>dict;
	for(size_t i=0;i+1<t1.size();++i)
		if(!t1[i].punc_after)
			dict.insert({t1[i].w,t1[i+1].w});

	int m=t2.size();
	vector<bool>bold(m,false);
	for(int i=0;i+1<m;++i)
		if(!t2[i].punc_after&&dict.count({t2[i].w,t2[i+1].w}))
			bold[i]=bold[i+1]=true;

	vector<int>open_at(s2.size()+1,0),close_at(s2.size()+1,0);
	for(int i=0;i<m;)
	{
		if(!bold[i]){++i;continue;}
		int j=i;
		while(j+1<m&&bold[j+1])
			++j;
		open_at[t2[i].l]=1;
		close_at[t2[j].r]=1;
		i=j+1;
	}

	string out;
	for(size_t p=0;p<s2.size();++p)
	{
		if(open_at[p])
			out+="<b>";
		out.push_back(s2[p]);
		if(close_at[p])
			out+="</b>";
	}
	cout<<out<<'\n';
	return 0;
}
```
