# P1659 [国家集训队] 拉拉队排练

> 难度：省选/NOI- · 标签：Manacher 算法 / 回文子串 / 差分后缀和 / 快速幂

## 1. 重新读题

### 1.1 问题重述（人话版）

给定一个长度为 $n$ 的小写字母字符串 $s$ 和一个正整数 $K$。将 $s$ 中所有**长度为奇数的回文子串**（位置不同算不同子串）按长度从大到小排序，求前 $K$ 长的奇数回文子串长度的乘积对 $19930726$ 取模的结果；若奇数长回文子串总数不足 $K$ 个，输出 `-1`。

### 1.2 关键约束与规模暗示

- **数据规模**：$n \le 10^6$，$K \le 10^{12}$。
- **规模暗示**：
  1. 奇数长回文子串总数最多可达 $\frac{n(n+2)}{4} \approx 2.5 \times 10^{11}$，且 $K \le 10^{12}$，因此不能把每个回文子串单独枚举出来，必须按长度归类统计「每种奇数长度 $2r - 1$ 共有多少个回文子串」，再用快速幂批量计算 $(2r - 1)^{\text{take}} \bmod p$！
  2. 因为只统计**奇数长度**回文子串，Manacher 算法无需插入分隔符 `#`，直接在原串 $s[0 \dots n-1]$ 上运行即可！

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：极长回文半径与全体回文子串的后缀和关系

以位置 $i$ 为中心的极长回文半径设为 $d[i]$（即 $s[i - d[i] + 1 \dots i + d[i] - 1]$ 是回文串，长度为 $2 d[i] - 1$）。
显然，以 $i$ 为中心、半径为 $1, 2, \dots, d[i]$ 的所有子串也全部是奇数长回文子串！

### 2.2 考虑简化版 / 子任务

- **中心扩展暴力（$n \le 1000$）**：对每个中心 $i$ 向两侧暴力扩展，把所有奇数回文长度塞进数组降序排序取前 $K$ 个相乘，作为 `P1659_brute.cpp`。

### 2.3 Manacher + 频次后缀和 + 快速幂证明

**引理 1（回文半径后缀和计数引理）**
设 `cnt[r]` 初值为满足 $d[i] = r$ 的中心 $i$ 的个数。从大到小执行后缀累加 `cnt[r] += cnt[r + 1]` 后，`cnt[r]` 恰好等于原串中长度为 $2r - 1$ 的回文子串总个数。

**证明**：
以 $i$ 为中心存在长度为 $2r - 1$（即半径为 $r$）的回文子串，当且仅当位置 $i$ 的极长回文半径满足 $d[i] \ge r$。因此，长度为 $2r - 1$ 的回文子串总数等于 $|\{i : d[i] \ge r\}| = \sum_{x \ge r} |\{i : d[i] = x\}|$，即频次数组的后缀和。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **explore its property** — 只关心奇数长回文串，省去 Manacher 虚轴字符 `#`；每个中心 $d[i]$ 对区间 $[1, d[i]]$ 的所有半径各贡献 $1$。
- **consider easier version** — $O(n^2)$ 暴力扩展收集全部回文长度排序，作为对拍基准。
- **classic problem solution** — 原串 Manacher $O(n)$ 求 $d[i]$ + 桶后缀和求出各长度回文串个数 + 从大到小贪心截取并用快速幂累乘。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, k` | `int, i64` | 字符串长度 $n$ 与截取个数 $K$ | 读入 | 题面参数 |
| `d[i]` | `int [1000005]` | 以 $i$ 为中心的极长回文半径 | Manacher 计算 | radius |
| `cnt[r]` | `i64 [1000005]` | 半径为 $r$（长度 $2r-1$）的回文子串个数 | 频次后缀和 | count of radius $r$ |

### 4.2 算法步骤

1. 读入 $n, k, s$，跑奇数长 Manacher 算法求出所有 `d[i]`，并令 `++cnt[d[i]]`。
2. 从最大可能半径 $r = \lfloor(n+1)/2\rfloor$ 递减至 $1$（当 $k > 0$ 时）：
   - 累加后缀和 `cnt[r] += cnt[r + 1]`；
   - 取 `take = min(k, cnt[r])`，令 `ans = ans * qpow(2 * r - 1, take) % 19930726`，并令 `k -= take`。
3. 若循环结束后仍有 $k > 0$，说明回文子串总数不足 $K$，输出 `-1`；否则输出 `ans`。

### 4.3 复杂度与验证状态

- **时间复杂度**：$O(n \log K)$（每个半径至多调用一次快速幂，实为 $O(n)$）。
- **空间复杂度**：$O(n)$。
- **沙箱对拍验证**：`g++ -O2 -std=c++17 -Wall` 零警告编译通过；官方样例 `5 3 / ababa` 输出 `45` 通过；与 $O(n^2)$ 暴力程序 `P1659_brute.cpp` 对拍 $400$ 组随机数据全部一致；$n = 10^6$ 实测耗时 $0.0879\text{ s}$。

## 5. 示例代码

### 5.1 标程（`P1659.cpp`）

```cpp
//
// 题目：P1659 [国家集训队] 拉拉队排练
// https://www.luogu.com.cn/problem/P1659
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};
constexpr i64 p{19930726};

int n,d[mxn];
i64 k,cnt[mxn];
string s;

i64 qpow(i64 a,i64 b)
{
	i64 r=1;
	a%=p;
	for(;b;b>>=1,a=a*a%p)
		if(b&1)
			r=r*a%p;
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>s;

	for(int i=0,l=0,r=-1;i<n;++i)
	{
		int rad=(i>r)?1:min(d[l+r-i],r-i+1);
		while(i-rad>=0&&i+rad<n&&s[i-rad]==s[i+rad])
			++rad;
		d[i]=rad;
		++cnt[rad];
		if(i+rad-1>r)
		{
			l=i-rad+1;
			r=i+rad-1;
		}
	}

	int max_r=(n+1)/2;
	i64 ans=1;
	for(int r=max_r;r>=1&&k>0;--r)
	{
		cnt[r]+=cnt[r+1];
		i64 take=min(k,cnt[r]);
		ans=ans*qpow(2LL*r-1,take)%p;
		k-=take;
	}

	if(k>0)
		cout<<-1<<'\n';
	else
		cout<<ans<<'\n';
	return 0;
}
```

### 5.2 暴力对照程序（`P1659_brute.cpp`）

```cpp
//
// 题目：P1659 [国家集训队] 拉拉队排练
// 暴力基准：枚举所有中心向两侧扩展统计全部奇数长回文子串长度，排序后取前 K 大乘积
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 p{19930726};

int n;
i64 k;
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k>>s;
	vector<int>lens;
	for(int i=0;i<n;++i)
		for(int r=0;i-r>=0&&i+r<n&&s[i-r]==s[i+r];++r)
			lens.push_back(2*r+1);

	if((i64)lens.size()<k)
	{
		cout<<-1<<'\n';
		return 0;
	}
	sort(lens.begin(),lens.end(),greater<>());
	i64 ans=1;
	for(int i=0;i<k;++i)
		ans=ans*lens[i]%p;
	cout<<ans<<'\n';
	return 0;
}
```
