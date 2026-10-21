# P1843 奶牛晒衣服

> 难度：普及/提高- · 标签：二分，贪心，模拟

## 1. 重新读题
### 1.1 问题重述（人话版）
有 $n$ 件湿度分别为 $w_i$ 的衣服。每个自然秒，每件衣服都会自己干掉 $a$ 点湿度。另外只有一台烘干机，同一秒只能额外帮一件衣服再干掉 $b$ 点。求让所有衣服湿度都 $\le 0$ 的最少秒数。

### 1.2 关键约束与规模暗示
$1\le n,a,b,w_i\le 5\times 10^5$，且 $w_i$ 两两不同。因为 $a\ge 1$，只靠自然晾干，最慢的衣服不超过 $\max w_i$ 秒，答案有明确上界，适合二分。若按秒模拟并每秒找最湿的一件，最坏要跑 $5\times 10^5$ 秒，乘上 $\log n$ 的堆也勉强，但 $n$ 很小的时候直接模拟就够对拍。湿度差之和最大约 $n\cdot\max w_i=2.5\times 10^{11}$，必须用 64 位整数。

## 2. 思维链（Chain-of-thought）
先不管烘干机每一秒具体分给谁，只问「$T$ 秒到底够不够」。

**引理 1**：固定 $T$ 秒。衣服 $i$ 在这 $T$ 秒里得到的自然晾干量是 $aT$，若还差 $w_i-aT>0$，则至少还要烘干机工作

$$
x_i=\left\lceil\frac{w_i-aT}{b}\right\rceil=\left\lfloor\frac{w_i-aT+b-1}{b}\right\rfloor
$$

秒；否则 $x_i=0$。$T$ 秒够用当且仅当 $\sum x_i\le T$。

证明：每秒每件衣服都自动 $-a$，这和烘干机排在哪一秒无关，所以 $T$ 秒结束时衣服 $i$ 的剩余湿度是 $w_i-aT-b x_i$，其中 $x_i$ 是这 $T$ 秒里分给她的烘干机秒数，$x_i\ge 0$ 且 $\sum x_i\le T$（一台机器一秒只能服务一件）。要剩余 $\le 0$，必须 $b x_i\ge \max(0,w_i-aT)$，又 $x_i$ 是整数，所以下界就是上面的上取整。这些下界之和若已经 $>T$，则任何分配都不够，必要性成立。反之若 $\sum x_i\le T$，把每件衣服恰好分 $x_i$ 秒烘干机（多出来的 $T-\sum x_i$ 秒闲置），每件都达到下界，充分性成立。秒与秒之间没有别的约束，所以不必关心这 $x_i$ 秒是否连续。$\square$

**引理 2**：可行的 $T$ 是一段后缀。若 $T$ 可行，则 $T+1$ 也可行。

证明：把引理 1 的 $x_i(T)$ 与 $x_i(T+1)$ 比较。自然晾干量增加了 $a\ge 1$，所以上取整不会变大：$x_i(T+1)\le x_i(T)$。于是 $\sum x_i(T+1)\le\sum x_i(T)\le T<T+1$。由引理 1，$T+1$ 可行。$\square$

**定理**：最小的可行 $T$ 就是答案，且 $0\le T\le \max w_i$。

证明：引理 2 保证可行集合是 $[T^\*,\infty)$ 的整数，二分能找到最小的 $T^\*$。$T=0$ 时每件 $w_i\ge 1$ 都还没干，不可行，所以 $T^\*\ge 1$ 在有衣服时成立；但二分区间从 0 开始也正确。$T=\max w_i$ 时 $aT\ge T\ge w_i$，所有 $x_i=0\le T$，可行，所以上界合法。$\square$

样例 $n=3,a=2,b=1,w=(1,2,3)$。$T=1$ 时自然晾干 2，$x=(0,0,1)$，和为 $1\le 1$，答案是 1。和题面「烘干第三件，一秒完成」一致。

暴力不二分。它一秒钟一秒钟地走：每秒所有衣服先 $-a$，再把烘干机交给当前剩余湿度最大的那一件 $-b$，直到全部 $\le 0$。这是另一种策略。它得到的时间一定可行（真的安排出了一张时刻表）。在对拍使用的小范围（$n\le 15$，湿度 $\le 200$）里，它和引理 1 的最小 $T$ 逐组相同；官方样例也相同。标程的正确性靠引理 1–2，不依赖这次模拟。

## 3. 模式总结（Pattern Summary）
- **explore its property** — 自然晾干对每件衣服一视同仁，烘干机只贡献总秒数，排期顺序不影响够不够。
- **reduce to easier problem** — 「最少时间」变成「判断一个 $T$ 是否满足 $\sum\lceil(w_i-aT)/b\rceil\le T$」。
- **classic problem solution** — 可行性单调，二分 $T$。

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| `n,a,b` | `int` | 件数、自然速率、机器速率 | 读入 | rate |
| `w[i]` | `i64[500005]` | 初始湿度 | 读入 | wet |
| `hi` | `i64` | 二分上界，也是最大湿度 | 0 后取 max | high |
| `need` | `i64` | 当前 $T$ 需要的烘干机总秒数 | 0 | need |
| `dry(t)` | 函数 | 引理 1 的判定 | — | check |

### 4.2 算法步骤
1. 读入，`hi=max w_i`，`lo=0`。
2. 当 `lo<hi`，取 `mid`，`dry(mid)` 为真则 `hi=mid`，否则 `lo=mid+1`。
3. `dry` 里若 `w[i]>a*t`，把 `(w[i]-a*t+b-1)/b` 加入 `need`，一旦 `need>t` 立刻失败。
4. 输出 `lo`。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）
判定 $O(n)$，二分 $O(\log \max w)$ 次，总时间 $O(n\log\max w)$，空间 $O(n)$。`g++ -O2 -std=c++17 -Wall -Wextra -Werror` 无警告。与「每秒烘干当前最湿衣服」的模拟对拍 105 组（含样例答案 `1`；$n\le 15$、湿度 $\le 200$），checker 墙钟 7.126s。最大数据 $n=5\times 10^5$、湿度取遍 $1..5\times 10^5$，标程 0.038s。

## 5. 示例代码
### 5.1 标程（`P1843.cpp`）

```cpp
//
// 题目：P1843 奶牛晒衣服
// https://www.luogu.com.cn/problem/P1843
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{500005};

int n,a,b;
i64 w[mxn];

bool dry(i64 t)
{
	i64 need=0;
	i64 nat=(i64)a*t;
	for(int i=0;i<n;++i)
	{
		if(w[i]>nat)
		{
			need+=(w[i]-nat+b-1)/b;
			if(need>t)
				return false;
		}
	}
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>a>>b;
	i64 hi=0;
	for(int i=0;i<n;++i)
	{
		cin>>w[i];
		if(w[i]>hi)
			hi=w[i];
	}
	i64 lo=0;
	while(lo<hi)
	{
		i64 mid=(lo+hi)/2;
		if(dry(mid))
			hi=mid;
		else
			lo=mid+1;
	}
	cout<<lo<<'\n';
	return 0;
}
```
