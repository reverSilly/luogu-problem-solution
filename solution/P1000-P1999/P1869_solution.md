# P1869 愚蠢的组合数

> 难度：普及/提高- · 标签：数学

## 1. 重新读题
### 1.1 问题重述（人话版）

给 $t$ 组 $N,K$，保证 $K\le N$。$C(N,K)$ 是奇数就输出 $1$，否则输出 $0$。不要把组合数真的算出来。

### 1.2 关键约束与规模暗示

题面数据范围是 $1\le N\le 10^5$、$1\le t\le 10^5$（$K$ 为不超过 $N$ 的非负整数）。$t$ 与 $N$ 同阶，不能对每组做 $O(N)$ 的阶乘。Lucas 模 $2$ 退化成按位判断，每组 $O(1)$ 或 $O(\log N)$。

## 2. 思维链（Chain-of-thought）

**引理 1（Lucas，模 $2$）。** 设 $N,K$ 的二进制位是 $N_i,K_i\in\{0,1\}$。则
$$
C(N,K)\equiv \prod_i C(N_i,K_i)\pmod 2.
$$
$C(0,1)=0$，其余 $C(0,0)=C(1,0)=C(1,1)=1$。因此 $C(N,K)$ 为奇数当且仅当每个为 $1$ 的 $K_i$ 都对应 $N_i=1$，即 $(N\,\&\, K)=K$。

证明：Lucas 定理说，对素数 $p$，$C(N,K)\equiv\prod C(N_i,K_i)\pmod p$，其中 $N_i$ 是 $p$ 进制各位，且若某个 $N_i<K_i$ 则该项为 $0$。取 $p=2$，每一位只有 $0/1$，上一段的四个值就是全部情况。二进制下 $K$ 的某个 $1$ 落在 $N$ 的 $0$ 上，等价于按位与丢掉了这个 $1$，即 $N\,\&\,K\neq K$。$\blacksquare$

**引理 2（和杨辉三角一致）。** 模 $2$ 的递推 $C(n,k)\equiv C(n-1,k)+C(n-1,k-1)\pmod 2$，边界 $C(n,0)=C(n,n)=1$，与引理 1 在 $n<2005$ 的表上相同。

证明：Pascal 恒等式在整数上成立，模 $2$ 仍成立。对拍时小的 $N$ 用这张表，大的 $N$ 用 Kummer：2 在 $C(N,K)$ 里的幂次是 $\mathrm{popcount}(K)+\mathrm{popcount}(N-K)-\mathrm{popcount}(N)$，幂次为 $0$ 才是奇数。这与「二进制相加 $K$ 与 $N-K$ 不进位」是同一件事，因此和 $(N\,\&\,K)=K$ 等价，但代码路径不同。另外用 Python 大整数把 $C(N,K)$ 直接乘出来再看个位，核对 $N\le 500$ 的全部询问。$\blacksquare$

$C(N,0)=1$ 是奇数，$N\,\&\,0=0$。样例 $C(1,1)=1$、$C(1,0)=1$、$C(2,1)=2$ 分别输出 $1,1,0$。

## 3. 模式总结（Pattern Summary）

- **parity not the value** — 只问奇偶，不必构造组合数
- **Lucas at prime 2** — 模 $2$ 的 Lucas 只看二进制每一位
- **subset of bits** — $K$ 的每个 $1$ 都必须是 $N$ 的 $1$
- **one bitwise and** — $(N\,\&\,K)==K$ 就是奇数
- **cross-check with Pascal and popcount** — 小表、Kummer 幂次、大整数乘积三条路互证

## 4. 真实解法
### 4.1 符号表（表格：符号 / 类型尺寸 / 含义 / 初值 / 助记）

| 符号 | 类型尺寸 | 含义 | 初值 | 助记 |
| --- | --- | --- | --- | --- |
| `t` | `int` | 组数 | 读入 | $\le 10^5$ |
| `n,k` | `i64` | 组合数上下指标 | 读入 | $K\le N$ |

### 4.2 算法步骤

1. 读 $t$。
2. 每组读 $N,K$，若 `(n&k)==k` 输出 `1`，否则输出 `0`。

### 4.3 复杂度与验证状态（只写你真实跑出来的数字）

每组 $O(1)$，总时间 $O(t)$，空间 $O(1)$。

`g++ -O2 -std=c++17 -Wall -Wextra` 无警告。暴力在 $N<2005$ 时用杨辉三角模 $2$，更大时用 popcount 幂次。检查器还用 Python 整数把 $N\le 500$ 的组合数乘出来核对奇偶。含官方样例和 $C(5,2),C(6,2),C(7,3),C(10,3)$ 等手算点。结果：

- `30 tests OK`
- 最大数据 $t=10^5$、$0\le K\le N\le 10^5$，标程墙钟 `0.053799s`

## 5. 示例代码
### 5.1 标程（`P1869.cpp`）

```cpp
//
// 题目：P1869 愚蠢的组合数
// https://www.luogu.com.cn/problem/P1869
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		i64 n,k;
		cin>>n>>k;
		cout<<(((n&k)==k)?1:0)<<'\n';
	}
	return 0;
}
```
