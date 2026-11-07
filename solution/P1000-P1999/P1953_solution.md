# P1953 易语言

## 1. 重新读题

### 1.1 问题重述（人话版）

老师要把一批测试数据文件重命名。输入格式分两种：

**情况一（第一行两个字符串）**：第一个字符串是「生成后的第一个输入文件名」（例如 `enger0.in`），第二个字符串是「生成后输出文件的扩展名」（例如 `ans`）。含义是：

- 目标输入文件的主名、扩展名、编号位置都从第一个字符串里读出来（`enger0.in` 表示主名 `enger`、扩展名 `in`、编号 `0` 跟在主名后面）；
- 目标输出文件的主名与输入文件相同，扩展名用第二个字符串（于是输出名为 `enger0.ans, enger1.ans, ...`）；
- 编号从第一个字符串里的编号开始，逐个加一。

**情况二（第一行只有一个数字 $s\in\{0,1\}$）**：所有文件的**主名和扩展名保持不变**，只把编号改成从 $s$ 开始连续编号。

接下来若干行（直到输入结束），每行两个字符串：原始输入文件名、原始输出文件名。保证原始文件名里，**编号数字只出现在主名末尾或扩展名末尾**，且文件名其他任何地方不含数字。

要求：按编号从小到大输出 $n$ 行，每行是「新的输入文件名 新的输出文件名」。

### 1.2 关键约束与规模暗示

- 行数 $n\le10^5$：需要 $O(n\log n)$ 的排序，读入要读到 EOF。
- 主名、扩展名长度 $\le10$，最大编号 $\le2\times10^5$：编号能塞进 `int`，字符串操作也很便宜。
- 文件名里的数字**只可能是编号**，且只在主名末尾或扩展名末尾，这是解析能确定下来的关键保证。

---

## 2. 思维链（Chain-of-thought）

### 2.1 第一步：一个文件名里的「编号」在哪

文件名的一般形状是 `主名.扩展名`。编号数字要么是主名的后缀，要么是扩展名的后缀。

**引理 1（解析唯一）**：若文件名除编号外不含其它数字，则「最长的十进制数字后缀」只可能出现在主名或扩展名两者之一，且它就是要找的编号。

**证明**：设文件名分成主名 $M$ 与扩展名 $E$（以最后一个 `.` 分隔）。

- 若 $M$ 有非空数字后缀，由题设「编号只可能在主名末尾或扩展名末尾」以及「其它地方没有数字」，$M$ 的数字后缀就是编号，且 $E$ 中不含数字（否则数字出现在扩展名的非编号位置或第二个编号，均违反题设）。
- 若 $M$ 没有数字后缀，则编号只能在 $E$ 的末尾，取 $E$ 的最长数字后缀即可。

于是「先看主名有无数字后缀，有就归主名，否则归扩展名」的判定是唯一且正确的。$\square$

解析结果用一个四元组表示：

$$
(\text{mainBase},\ \text{extBase},\ \text{idx},\ \text{pos}),
$$

`pos=false` 表示编号贴在主名末尾，`pos=true` 表示贴在扩展名末尾。格式化时按 `pos` 把编号拼回去。

### 2.2 第二步：编号的重新分配

原文件名给出的编号顺序可能是乱的（题面明确提醒）。我们需要把「第 $k$ 小的原编号」（$k$ 从 $0$ 计）映射到「新编号 $s+k$」。

**引理 2（保序重编号）**：把原始文件按原编号升序排序，设排序后第 $k$ 个（$k$ 从 $0$ 计）的原编号为 $i_k$，则令新编号为 $s+k$ 得到的重命名方案，就是把这批文件「从 $s$ 开始连续编号」且保持测试点先后顺序的唯一方案。

**证明**：

- 每个文件的输入、输出名必须配对（同一行给的就是一对），所以重命名不能改变「哪个输入对应哪个输出」。
- 「只改编号、从 $s$ 开始」要求新编号集合为 $\{s,s+1,\dots,s+n-1\}$，且相邻原编号对应的文件在重命名后仍相邻（否则某些测试点会交换顺序，测试数据与答案错位）。
- 排序后依次分配 $s,s+1,\dots$ 恰好满足这些条件，且是由原编号大小关系唯一确定的。$\square$

题面例子 $e0.in,\dots,e9.in \to data1.in,\dots,data10.in$ 也正好是这个保序映射：最小的 $0$ 变 $1$，其次 $1$ 变 $2$，依此类推。

### 2.3 第三步：两种情况分别怎么拼名字

**情况一**：从第一个目标输入文件名解析出 $(\text{tMain},\text{tExt},\text{start},\text{tPos})$。

- 第 $k$ 小的原始文件，新编号 $\text{nid}=\text{start}+k$；
- 新输入名：用 $(\text{tMain},\text{tExt},\text{nid},\text{tPos})$ 拼；
- 新输出名：主名同 $\text{tMain}$，扩展名用第一行第二个 token `oext`，编号位置**沿用** `tPos`。题面示例只展示了「编号跟在主名后」的情形；这里沿用目标输入名的编号位置，是与示例一致的自然解释。

**情况二**：每个原始文件保留自己的主名、扩展名和编号位置，只把编号换成 $\text{nid}$。输入、输出各自独立解析、独立拼接（它们的主名/扩展名可能不同）。

**引理 3（拼接互逆）**：对任意按引理 1 解析出的四元组，先按 `pos` 去掉编号得到 $(\text{mainBase},\text{extBase})$，再用新的 `idx` 拼回，得到的新名字解析后编号恰为该 `idx`、主名/扩展名基底不变。

**证明**：`pos=false` 时新名字是 `mainBase + digits(idx) + "." + extBase`，最右的数字后缀恰为 `digits(idx)`（主名基底不以数字结尾，扩展名无数字），解析回去得到同一基底与 `idx`；`pos=true` 时类似，数字后缀在扩展名末尾，主名无数字后缀，解析先看主名（无数字）再看扩展名，得到同一基底与 `idx`。$\square$

### 2.4 第四步：输出顺序

新编号是随 $k$ 单调递增的，所以「按原编号升序排好」后直接顺序输出，就是「按新编号从小到大输出」。

### 2.5 复杂度

- 解析每行 $O(\text{长度})$，排序 $O(n\log n)$，总时间 $O(n\log n+\text{总字符数})$。
- 空间 $O(n)$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 文件名 = 主名 + 扩展名，编号是唯一一段「末尾数字」，且只会出现在主名末尾或扩展名末尾。
- **reduce to easier problem** — 把每个名字解析成（主名基底、扩展名基底、编号、编号位置）四元组。
- **classic problem solution** — 按原编号排序，第 $k$ 个赋新编号 $s+k$，保序重命名。
- **discuss by category** — 情况一用目标格式模板拼名字；情况二保留每个文件自己的主名/扩展名。
- **explore its property** — 编号随排序位置单调，所以输出顺序就是排序后的顺序。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `filedata` | `struct` | 解析后的文件名：`main,ext,idx,pos,dot` | — | 文件四元组 |
| `main` | `string` | 去掉编号后的主名基底 | 解析 | main |
| `ext` | `string` | 去掉编号后的扩展名基底 | 解析 | extension |
| `idx` | `int` | 原编号 | 解析 | index |
| `pos` | `bool` | `false`：编号在主名末尾；`true`：编号在扩展名末尾 | 解析 | 位置 |
| `dot` | `bool` | 文件名是否含 `.`（防御性处理） | 解析 | 有扩展名 |
| `t1` | `string` | 第一行第一个 token | 读入 | 判断模式 |
| `tgt` | `filedata` | 情况一的目标输入模板 | 解析 | target |
| `oext` | `string` | 情况一的目标输出扩展名 | 读入 | out ext |
| `start` | `int` | 新编号起点 | 读入 | 起点 |
| `fs` | `vector<pair<filedata,filedata>>` | 原始（输入,输出）对 | 读入 | files |
| `ni` | `int` | 当前文件的新编号 | `start+k` | new index |

### 4.2 算法步骤

1. 读入第一个 token `t1`。
   - 若 `t1=="0"` 或 `t1=="1"`：`mode2=true`，`start=t1[0]-'0'`；
   - 否则：`mode2=false`，再读一个 token 作为 `oext`，`tgt=parse(t1)`，`start=tgt.idx`。
2. 用 `while(cin>>a>>b)` 读入所有原始文件名对，解析后存入 `fs`。
3. 按 `fs[k].first.idx` 升序排序。
4. 对每个 $k=0\dots n-1$，令 `ni=start+k`：
   - 情况二：输出 `fmt(原输入, ni)` 与 `fmt(原输出, ni)`；
   - 情况一：输出 `fmt(tgt, ni)` 与 `fmt(输出模板, ni)`，其中输出模板主名为 `tgt.main`、扩展名为 `oext`、编号位置为 `tgt.pos`。
5. 每行两个名字，换行。

### 4.3 复杂度

- 时间：$O(n\log n+\text{总字符数})$。
- 空间：$O(\text{总字符数})$。

---

## 5. 示例代码

```cpp
//
// 题目：P1953 易语言
// https://www.luogu.com.cn/problem/P1953
#include<bits/stdc++.h>
using namespace std;

struct filedata
{
	string main,ext;
	int idx{};
	bool pos{};		// false: 编号在主名末尾；true: 编号在扩展名末尾
	bool dot{};
};

filedata parse(const string&s)
{
	filedata f;
	size_t d=s.rfind('.');
	if(d==string::npos)
	{
		f.dot=false;
		f.main=s;
		f.ext="";
	}
	else
	{
		f.dot=true;
		f.main=s.substr(0,d);
		f.ext=s.substr(d+1);
	}
	int k=(int)f.main.size();
	while(k>0&&isdigit((unsigned char)f.main[k-1]))
		--k;
	if(k<(int)f.main.size())	// 编号在主名末尾
	{
		f.idx=stoi(f.main.substr(k));
		f.main=f.main.substr(0,k);
		f.pos=false;
	}
	else						// 编号在扩展名末尾
	{
		int q=(int)f.ext.size();
		while(q>0&&isdigit((unsigned char)f.ext[q-1]))
			--q;
		if(q<(int)f.ext.size())
			f.idx=stoi(f.ext.substr(q));
		else
			f.idx=0;
		f.ext=f.ext.substr(0,q);
		f.pos=true;
	}
	return f;
}

string fmt(const filedata&f,int idx)
{
	string s;
	if(!f.pos)
		s=f.main+to_string(idx);
	else
		s=f.main;
	if(f.dot)
	{
		s+='.';
		if(f.pos)
			s+=f.ext+to_string(idx);
		else
			s+=f.ext;
	}
	else if(f.pos)
		s+=to_string(idx);
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string t1;
	if(!(cin>>t1))
		return 0;

	bool mode2{false};
	filedata tgt;
	string oext;
	int start{0};
	if(t1=="0"||t1=="1")
	{
		mode2=true;
		start=t1[0]-'0';
	}
	else
	{
		cin>>oext;
		tgt=parse(t1);
		start=tgt.idx;
	}

	vector<pair<filedata,filedata>> fs;
	string a,b;
	while(cin>>a>>b)
		fs.push_back({parse(a),parse(b)});
	sort(fs.begin(),fs.end(),[](const auto&x,const auto&y){
		return x.first.idx<y.first.idx;
	});

	for(int k=0;k<(int)fs.size();++k)
	{
		int ni=start+k;
		if(mode2)
			cout<<fmt(fs[k].first,ni)<<' '<<fmt(fs[k].second,ni)<<'\n';
		else
		{
			filedata out;
			out.main=tgt.main;
			out.ext=oext;
			out.pos=tgt.pos;
			out.dot=true;
			cout<<fmt(tgt,ni)<<' '<<fmt(out,ni)<<'\n';
		}
	}
	return 0;
}
```
