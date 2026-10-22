# P1787 [入门赛 #22] 非众数 Hard Version

## 1. 重新读题

给定一个长度为 $n$（$1 \le n \le 10^5$）的小写字母字符串 $s$。若某个非空子串 $a$ 中出现次数最多的字符的出现次数 $\le \lfloor |a| / 2 \rfloor$，则称 $a$ 为“非众数串”。求 $s$ 的所有 $\frac{n(n+1)}{2}$ 个非空子串中非众数串的个数。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：正难则反，绝对众数的唯一性（互斥性）

直接统计“所有 $26$ 个字母的出现次数都不超过一半”需要同时约束 $26$ 个变量；但如果**反过来统计“存在某个字母出现次数严格超过一半（$> \lfloor |a|/2 \rfloor$）”的众数子串**：

**引理 1（绝对众数的互斥性与 $\pm 1$ 前缀和判据）**：
1. 在任意非空子串 $s[l \dots r]$ 中，满足出现次数 $\text{cnt}_c(l, r) > \lfloor (r - l + 1) / 2 \rfloor \iff 2 \cdot \text{cnt}_c(l, r) - (r - l + 1) \ge 1$ 的字符 $c \in \{\texttt{'a'}, \dots, \texttt{'z'}\}$ **至多只有一个**；
2. 固定字符 $c$，定义权值序列 $w_i = (s_i == c ? +1 : -1)$ 及其前缀和 $P_i = \sum_{k=1}^i w_k$（$P_0 = 0$）。则子串 $s[l \dots r]$ 以 $c$ 为绝对众数当且仅当
   $$P_r - P_{l-1} \ge 1 \iff P_{l-1} \le P_r - 1.$$

**证明**：
1. 若存在两个不同字符 $c_1 \ne c_2$ 均满足 $2 \text{cnt}_{c_1} \ge L + 1$ 且 $2 \text{cnt}_{c_2} \ge L + 1$，相加得 $2(\text{cnt}_{c_1} + \text{cnt}_{c_2}) \ge 2L + 2 \implies \text{cnt}_{c_1} + \text{cnt}_{c_2} \ge L + 1 > L$，超过子串总长度，矛盾！故 $26$ 个字母的绝对众数事件两两互斥，可直接将各字母的绝对众数子串个数相加，再用总子串数 $\frac{n(n+1)}{2}$ 减去即可！
2. 在 $\pm 1$ 赋值下，$P_r - P_{l-1} = \text{cnt}_c(l, r) - \big((r - l + 1) - \text{cnt}_c(l, r)\big) = 2\text{cnt}_c(l, r) - (r - l + 1) \ge 1$。$\blacksquare$

### 2.2 相邻前缀和差为 $\pm 1$ 的 $\Theta(n)$ 桶维护

更进一步：由于相邻前缀和满足 $|P_i - P_{i-1}| = 1$，当指针从 $i-1$ 移动到 $i$ 时，前缀和 $P$ 只会 $+1$ 或 $-1$，小于 $P_i$ 的历史前缀和个数 `less_cnt` 只需加减单点桶频数 `cnt[...]` 即可在 **$O(1)$ 时间**内更新，连树状数组的 $\log n$ 都省去了，总复杂度严格为 $\Theta(26 n)$！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 正难则反，利用“绝对众数至多一个”的互斥性将问题拆分为 $26$ 个独立的单字母计数问题（引理 1）。
- **classic problem solution** — 对每个字母赋权 $+1/-1$，利用前缀和单步变化 $\pm 1$ 的连续性动态维护 $< P_i$ 的前缀个数，实现 $\Theta(|\Sigma| n)$ 线性求解。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `s` | `string` | 输入字符串（长度 $n$） | 标准输入 | string |
| `cnt[v]` | `int [200005]` | 偏移 $+n$ 后的前缀和频数桶 | 每字母清零 | frequency bucket |
| `bad` | `i64` | 存在绝对众数的子串总数 | `0` | majority substrings |

### 4.2 算法步骤

1. 读入字符串 $s$，令 $n = |s|$，总子串数 `tot = 1LL * n * (n + 1) / 2`。
2. 枚举字母 $c = 0 \dots 25$：
   - 令当前前缀和（带偏移 $n$）`cur = n`，`cnt[cur] = 1`，严格小于 `cur` 的历史前缀和计数 `less_cnt = 0`；
   - 遍历 $i = 0 \dots n-1$：
     - 若 $s[i] - \texttt{'a'} == c$：先令 `less_cnt += cnt[cur]`，再令 `++cur`；
     - 否则：先令 `--cur`，再令 `less_cnt -= cnt[cur]`；
     - 令 `bad += less_cnt`，`++cnt[cur]`；
   - 清理本次访问过的桶区间。
3. 输出 `tot - bad`。

### 4.3 复杂度分析

- **时间复杂度**：$26$ 次线性扫描，时间复杂度严格为 $\Theta(|\Sigma| n)$（$n = 10^5$ 时仅 $2.6 \times 10^6$ 次加减，耗时 $< 10\text{ ms}$）。
- **空间复杂度**：频数桶数组长度 $2n + 5$，空间复杂度为 $\Theta(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1787 [入门赛 #22] 非众数 Hard Version
// https://www.luogu.com.cn/problem/P1787
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

string s;
int cnt[mxn*2];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	int n=s.size();
	i64 tot=1LL*n*(n+1)/2,bad=0;

	for(int c=0;c<26;++c)
	{
		int cur=n,mn=n,mx=n;
		cnt[cur]=1;
		i64 less_cnt=0;
		for(int i=0;i<n;++i)
		{
			if(s[i]-'a'==c)
			{
				less_cnt+=cnt[cur];
				++cur;
			}
			else
			{
				--cur;
				less_cnt-=cnt[cur];
			}
			bad+=less_cnt;
			++cnt[cur];
			mn=min(mn,cur);
			mx=max(mx,cur);
		}
		for(int v=mn;v<=mx;++v)
			cnt[v]=0;
	}

	cout<<tot-bad<<'\n';
	return 0;
}
```
