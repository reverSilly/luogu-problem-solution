# P1125 [NOIP 2008 提高组] 笨小猴 — 完整题解

---

## 1. 重新阅读题目

给定一个由小写字母构成的单词（长度 $<100$），定义：

- $\text{maxn}$ — 单词中出现次数**最多**的字母的出现次数；
- $\text{minn}$ — 单词中出现次数**最少**的字母的出现次数；

要求：若 $\text{maxn} - \text{minn}$ 是质数，则输出 `Lucky Word` 并输出该差值；否则输出 `No Answer` 并输出 $0$。

**关键细节**：
- 只考虑**出现在单词中**的字母。未出现的字母不参与统计 minn。
- 质数定义为大于 $1$ 的自然数，且只有 $1$ 和自身两个正因数。
- 若差值为 $0$ 或 $1$，显然不是质数。

**样例验证**：
- `error`：r 出现 3 次，e/o 各出现 1 次 → $\text{maxn}=3,\;\text{minn}=1$ → $3-1=2$ 是质数 → Lucky Word。
- `olympic`：每个字母出现 1 次 → $\text{maxn}=1,\;\text{minn}=1$ → $1-1=0$ 非质数 → No Answer。

---

## 2. 思维链与完整证明

### 2.1 暴力思路

长度 $<100$，字母种类只有 26 种。这根本不需要任何优化：

1. 用一个长度为 26 的数组统计每个字母出现次数；
2. 扫描数组，找出非零的最大值和最小值；
3. 计算差值，判断是否为质数。

### 2.2 正确性证明

**引理 1**：统计方式完备。  
每个位置上的字符属于 $26$ 个字母之一。对每个位置 `s[i]`，执行 `++cnt[s[i]-'a']` 即可得到每个字母的精确频次。因为输入保证只有小写字母，无需额外校验。

**引理 2**：minn 的定义排除未出现字母。  
题目明确定义 minn 是"出现次数最少的字母的出现次数"，未出现字母的出现次数为 $0$，但若将其纳入，则 minn 恒为 $0$，导致差值 $\text{maxn}-0=\text{maxn}$，与题意矛盾。因此只能遍历出现过（`cnt[i] > 0`）的字母。

**引理 3**：质数判定的正确性。  
对于 $d = \text{maxn} - \text{minn}$：
- 若 $d < 2$，直接判为非质数；
- 若 $d \ge 2$，试除 $[2,\sqrt{d}]$：若存在因子则非质数，否则是质数。

由于 $d \le 100$，$\sqrt{d} \le 10$，试除次数极少，无需预处理素数表。

**引理 4**：算法复杂度。  
统计频次 $O(|S|)$，求最值 $O(26)$，质数判定 $O(\sqrt{d})$。总复杂度 $O(|S|)$，对于 $|S| < 100$ 极其充裕。

---

## 3. 模式总结

| 维度 | 内容 |
|------|------|
| 问题类型 | 模拟 + 质数判定 |
| 核心技巧 | 桶计数（字母→频次） |
| 易错点 | minn 只考虑出现过的字母；质数定义中 $0$ 和 $1$ 不是质数 |
| 适用场景 | 任何需要对有限种类元素做频次统计后判定的问题 |

本题是 NOIP 提高组的第一题，考察基本功：
- 字符偏移索引（`ch - 'a'`）的熟练使用；
- 边界条件的周全考虑（minn 的选取范围、质数的定义）；
- 格式化输出的规范。

这是一个典型的"桶计数 + 条件判定"模型，在 OI 普及-难度题目中非常常见。

---

## 4. 正式题解与符号说明

### 4.1 符号表

| 符号 | 含义 |
|------|------|
| $S$ | 输入的单词字符串 |
| $n$ | $|S|$，单词长度 |
| $\Sigma$ | 英文字母集，$|\Sigma|=26$ |
| $c_i$ | 字母 $i$ 在 $S$ 中出现的次数 |
| $M$ | $\max\{c_i \mid c_i>0\}$，即 $\text{maxn}$ |
| $m$ | $\min\{c_i \mid c_i>0\}$，即 $\text{minn}$ |
| $d$ | $M - m$ |
| $\mathbb{P}$ | 质数集 |

### 4.2 算法流程

**Step 1 — 初始化**  
声明数组 `cnt[26]`，初值为 $0$。

**Step 2 — 频次统计**  
```
for ch in S:
    cnt[ch - 'a'] += 1
```

**Step 3 — 求最值**  
```
M ← 0, m ← +∞
for i in 0..25:
    if cnt[i] > 0:
        M ← max(M, cnt[i])
        m ← min(m, cnt[i])
```

**Step 4 — 质数判定**  
```
d ← M - m
函数 is_prime(x):
    if x < 2: return False
    for i in 2..sqrt(x):
        if x % i == 0: return False
    return True
```

**Step 5 — 输出**  
若 `is_prime(d)` 为真，输出 `"Lucky Word"` 并换行输出 $d$；  
否则输出 `"No Answer"` 并换行输出 $0$。

### 4.3 正确性

由引理 1–4，该算法对任何合法输入均能在 $O(n)$ 时间内给出正确答案，且不会出现未定义行为。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};

char s[mxn];
int cnt[26];

bool is_prime(int x)
{
	if (x<2) return 0;
	for (int i=2; i*i<=x; ++i)
		if (x%i==0) return 0;
	return 1;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	for (int i=0; s[i]; ++i) ++cnt[s[i]-'a'];

	int mx=0,mn=114514;
	for (int i=0; i<26; ++i)
		if (cnt[i])
		{
			if (cnt[i]>mx) mx=cnt[i];
			if (cnt[i]<mn) mn=cnt[i];
		}

	int d=mx-mn;
	if (is_prime(d))
		cout<<"Lucky Word\n"<<d<<'\n';
	else
		cout<<"No Answer\n0\n";

	return 0;
}
```

### 代码说明

- 使用 `s[i]` 作为终止条件，等价于遍历到 `'\0'`。
- `mn` 初始化为 `114514`（一个足够大的哨兵值），保证第一个有效频次一定能更新它。
- 质数判定采用 $i \times i \le x$ 而非 `sqrt(x)`，避免浮点误差。
- 输出严格遵循题目要求的两行格式。
- 全部采用 Allman 风格大括号，全局 `using namespace std;`，单文件结构。
