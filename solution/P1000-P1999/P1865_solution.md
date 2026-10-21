# P1865 A % B Problem

> 难度：普及/提高- · 标签：搜索, 二分, 洛谷原创, 素数判断, 前缀和, 线性筛法

## 1. 重新读题
### 1.1 问题重述（人话版）

先给定询问次数 $n$ 和右端上限 $m$。每次询问区间 $[l,r]$。若 $l$ 和 $r$ 都落在 $[1,m]$ 里，输出这个闭区间里的素数个数；否则输出 `Crossing the line`。

### 1.2 关键约束与规模暗示

$1\le n\le 1000$，$1\le m\le 10^6$，$-10^9\le l\le r\le 10^9$。$m$ 到 $10^6$，要先筛再前缀和，$O(m)$ 预处理、$O(1)$ 询问。$l,r$ 必须用 64 位读入。$1$ 不是素数。越界要先判断，不能拿负数或大于 $m$ 的下标去查前缀和。

## 2. 思维链（Chain-of-thought）

**引理 1（判定）。** 当且仅当 $l<1$ 或 $r>m$ 时输出 `Crossing the line`。

证明：题面要求 $l,r\in[1,m]$ 才计数。已知 $l\le r$，所以这两个条件覆盖了「至少有一个端点不在 $[1,m]$」：$r<1$ 时必有 $l\le r<1$；$l>m$ 时必有 $r\ge l>m$。$\blacksquare$

**引理 2（线性筛）。** 每个合数 $x\le m$ 被它的最小质因子恰好筛一次：从小到大扫 $i$，若 $i$ 还没被标记则 $i$ 是素数；再用已找到的素数 $p$ 标记 $p\cdot i$，并且在 $p$ 整除 $i$ 时停下。

证明：若 $x$ 是合数，设 $p$ 是 $x$ 的最小质因子，$i=x/p$，则 $p\le i$（否则 $x$ 有更小的因子写法矛盾于 $p$ 最小），筛到 $i$ 时 $p$ 已经在素数表里，且更小的素数不整除 $i$（否则 $p$ 不是 $x$ 的最小质因子），所以循环会到达 $p$ 并标记 $x$，随后因为 $p\mid i$ 而停止，不会再用更大的素数标记 $x$。素数没有质因子可写，不会被标记。$1$ 单独标成合数。$\blacksquare$

**引理 3（前缀和）。** 令 $s_i$ 为 $[1,i]$ 中的素数个数，则 $[l,r]$ 的答案是 $s_r-s_{l-1}$，$s_0=0$。

证明：$s$ 的定义就是素数指示函数的前缀和，闭区间差分就是这段的个数。$l\ge 1$ 时 $l-1\ge 0$。$\blacksquare$

## 3. 模式总结（Pattern Summary）

- **reject out of range first** — $l<1$ 或 $r>m$ 直接输出那句英文
- **linear sieve up to m** — 每个合数被最小质因子标记一次
- **prefix of prime flags** — 区间个数变成两次前缀相减
- **one is not prime** — 把 $1$ 标成非素数
- **read endpoints as 64-bit** — 端点到 $10^9$，先判越界再转成下标

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| `n,m` | `int` | 询问数、筛的上限 | 读入 | |
| `vis[1000005]` | `bool` | 合数标记 | 全局 `false`，`vis[1]=true` | 真表示不是素数 |
| `pr[1000005]` | `int` | 素数表 | `pc=0` | 线性筛 |
| `sf[1000005]` | `int` | 素数前缀和 | $0$ | $s_i$ |
| `l,r` | `i64` | 询问端点 | 读入 | 可能是负或很大 |

### 4.2 算法步骤

1. 读 $n,m$，做线性筛，顺手做前缀和。
2. 对每次询问读 $l,r$。越界输出 `Crossing the line`，否则输出 `sf[r]-sf[l-1]`。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

预处理 $O(m)$，每次询问 $O(1)$，空间 $O(m)$。

`g++ -O2 -std=c++17 -Wall -Wextra` 无警告。暴力对 $[l,r]$ 里每个数做试除，只在 $m\le 1500$ 时对拍。含官方样例（`2` 与 `Crossing the line`）、$m=1$、$[1,1]$、负数左端、右端超过 $m$。结果：

- `35 tests OK`
- 最大数据 $m=10^6$、$n=1000$，标程墙钟 `0.066503s`

## 5. 示例代码
### 5.1 标程（`P1865.cpp`）

```cpp
//
// 题目：P1865 A % B Problem
// https://www.luogu.com.cn/problem/P1865
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

int n,m;
int pr[mxn],pc;
bool vis[mxn];
int sf[mxn];

void sieve()
{
	vis[1]=true;
	for(int i=2;i<=m;++i)
	{
		if(!vis[i])
			pr[pc++]=i;
		for(int j=0;j<pc;++j)
		{
			i64 v=1ll*pr[j]*i;
			if(v>m)
				break;
			vis[v]=true;
			if(i%pr[j]==0)
				break;
		}
	}
	for(int i=1;i<=m;++i)
		sf[i]=sf[i-1]+(!vis[i]?1:0);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	sieve();
	for(int i=0;i<n;++i)
	{
		i64 l,r;
		cin>>l>>r;
		if(l<1||r>m)
			cout<<"Crossing the line\n";
		else
			cout<<sf[(int)r]-sf[(int)l-1]<<'\n';
	}
	return 0;
}
```
