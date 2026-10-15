# P1154 奶牛分厩 题解

---

## Part 1 — 重述问题

有 $N$ 头奶牛（$1\le N\le 5000$），每头有一个互不相同的编号 $S_i$（$1\le S_i\le 10^6$）。
谷仓有 $K$ 个厩，编号 $0\sim K-1$，第 $i$ 头奶牛去 $S_i\bmod K$ 号厩。

要求：**所有奶牛不能共享同一个厩**，也就是所有 $S_i\bmod K$ 必须互不相同。

求满足条件的最小的 $K$（题目保证存在）。

---

## Part 2 — 思维链（发现式推理）

### Step 1 — 把"余数互异"翻译成整除关系

我们要 $S_i\bmod K$ 两两不同。什么时候两个数的余数相同？

$$
S_i\equiv S_j\pmod K \iff K\mid (S_i-S_j).
$$

所以：

$$
\forall i\ne j,\;S_i\bmod K\ne S_j\bmod K
\iff \forall i\ne j,\;K\nmid |S_i-S_j|.
$$

**问题等价于**：找到最小的 $K\ge N$，使得 $K$ **不整除** 任何一对编号的差。

### Step 2 — 换个角度：K 不能是任何差的约数

设所有两两差的集合为

$$
D=\{\,|S_i-S_j| \mid 1\le i<j\le N\,\}.
$$

条件 $K\nmid d$ 对任意 $d\in D$ 成立，等价于说：

> **$K$ 本身以及 $K$ 的所有倍数，都不能出现在 $D$ 中。**

因为如果某个 $d\in D$ 满足 $K\mid d$，那么 $d$ 就是 $K$ 的某个倍数；反之，若 $K$ 的某个倍数 $mK$ 出现在 $D$ 中，则 $K\mid mK$，违反条件。

这是一个重要的**视角翻转**：
- 之前我们在思考 "$K$ 是否整除某个差？"
- 现在我们思考 "差集 $D$ 中是否包含 $K$ 的倍数？"

后者的好处是我们只需要做一次标记所有差，然后对每个候选 $K$ **按倍数跳跃扫描**即可。

### Step 3 — 暴力的局限与优化方向

最朴素的想法：从 $K=N$ 开始往上枚举，每次检查所有 $N^2$ 对是否出现冲突。
每次检查 $O(N^2)$，最坏可能要试到 $10^6$，总复杂度 $O(10^6\cdot N^2)$ 完全不可接受。

那么问题来了：**能不能复用差集的信息，避免每轮重新算冲突？**

#### Step 3a — 预计算差集

$N\le 5000$，$N^2\approx 25\times 10^6$，刚好在可接受范围。
我们可以先把所有 $|S_i - S_j|$ 算出来，记在一个布尔数组里：

```
hasDiff[d] = true  若存在一对 (i,j) 使得 |S_i - S_j| = d
```

$S_i\le 10^6$，所以 $d\le 10^6$，数组大小 $10^6+5$，约 1 MB，完全可以接受。

#### Step 3b — 利用倍数关系快速校验

现在对于候选 $K$，我们不需要遍历所有 $N^2$ 对，只需要检查 $K$ 的倍数：

```python
ok = True
for m in range(K, maxDiff+1, K):
    if hasDiff[m]:
        ok = False
        break
```

这个循环的总代价是多少？
对于每个 $K$，内层循环运行 $\lfloor\frac{maxDiff}{K}\rfloor$ 步。

从 $K=N$ 到 $K\approx maxDiff$ 汇总：

$$
\sum_{K=N}^{maxDiff} \frac{maxDiff}{K}
\approx maxDiff \cdot (H_{maxDiff} - H_{N-1})
\approx maxDiff \cdot \ln\frac{maxDiff}{N}
$$

$maxDiff\le 10^6$，最坏 $N=1$ 时 $10^6\cdot \ln 10^6 \approx 14\times 10^6$ 步。
即使加上预计算 $N^2\approx 25\times 10^6$ 步，总共约 $4\times 10^7$ 次操作，C++ 可以在 1 秒轻松跑完。

### Step 4 — 证明答案一定存在且不大

**Lemma 1（上界）**：取 $K > \max D$ 一定满足条件。

> 证明：若 $K > \max D$，则对任意 $d\in D$ 有 $d < K$，故 $K\nmid d$（因为正数 $d$ 不可能被比它大的数整除）。因此 $K$ 是可行的。
>
> 由于 $\max D \le \max S_i - \min S_i \le 10^6 - 1$，所以 $K$ 最大不超过 $10^6$。

**Lemma 2（不下界）**：$K$ 必须 $\ge N$。

> 证明：$K$ 个厩最多容纳 $K$ 头互不冲突的奶牛，所以必须 $K\ge N$。

综合：我们只需在 $[N,\; \max D+1]$ 范围内枚举 $K$，**一定**能找到解。

### Step 5 — 举两个例子验证

**例 1**：$S=\{1,2,3\}$，$N=3$
- 差集 $D=\{1,2\}$
- $K=3$：倍数 $3,6,9,\dots$，均不在 $D$ 中 → 可行，输出 $3$

**例 2**：$S=\{1,4,7,10\}$，$N=4$
- 差集 $D=\{3,6,9,3,6,3\}=\{3,6,9\}$
- $K=4$：倍数 $4,8,12,\dots$ → 不在 $D$ 中 → 可行，输出 $4$

**例 3**：$S=\{2,4,6,8\}$，$N=4$
- 差集 $D=\{2,4,6,2,4,2\}=\{2,4,6\}$
- $K=4$：倍数 $4,8,\dots$，$4$ 在 $D$ 中 → 不可行
- $K=5$：倍数 $5,10,\dots$，均不在 → 可行，输出 $5$

---

## Part 3 — 模式总结

| 步骤 | 模式 | 一句话 |
|------|------|--------|
| 1 | 探索问题的数学本质 | 余数互异 $\iff$ 差不被整除 |
| 2 | 翻转视角 | 由"K 是否整除差"变为"差集中是否含 K 的倍数" |
| 3 | 考虑暴力与瓶颈 | 每轮 O(N²) 不可取，需要预计算 |
| 4 | 经典技巧 — 预计算 | N² 可接受，一次算所有差，标记到布尔数组 |
| 5 | 经典技巧 — 倍数扫描 | 用调和级数复杂度检验每个候选 K |
| 6 | 边界推导 | 上界 maxDiff+1，下界 N，保证有限步内找到解 |

---

## Part 4 — 真正的解法

### 符号表

| 符号 | 含义 | 备注 |
|------|------|------|
| $n$ | 奶牛数量 | 输入 |
| $s_i$ | 第 $i$ 头奶牛的编号 | 输入，$1\le s_i\le 10^6$ |
| $\mathit{maxs}$ | $\max s_i$ | 用于限定数组大小和循环上界 |
| $\mathit{hasDiff}[d]$ | 是否存在一对 $(i,j)$ 使得 $\|s_i-s_j\|=d$ | 布尔数组，下标 $0\sim\mathit{maxs}$ |
| $k$ | 当前正在检验的厩数 | 候选答案，从 $n$ 开始递增 |

### 算法步骤

1. **读入** $n$ 和 $s[0\dots n-1]$，同时记录 $\mathit{maxs}$。

2. **标记差集**：双重循环 $0\le i<j<n$，计算 $d = |s_i-s_j|$，设 $\mathit{hasDiff}[d]=\mathrm{true}$。

3. **枚举候选**：$k$ 从 $n$ 开始递增：
   - 遍历 $m = k,\;2k,\;3k,\;\dots,\;\mathit{maxs}$：
     - 若 $\mathit{hasDiff}[m]=\mathrm{true}$，则 $k$ 不可行，中断内层循环。
   - 若内层循环没有中断，则 $k$ 为答案，输出后结束。

4. **正确性保证**：由 Part 2 的 Lemma 1，$k$ 最多试到 $\mathit{maxs}+1$，必定找到解。

### 复杂度

- **时间**：$O(N^2 + \mathit{maxs}\cdot \log\mathit{maxs})$，约 $4\times 10^7$ 次操作。
- **空间**：$O(\mathit{maxs})$，约 1 MB。

---

## Part 5 — 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};
constexpr int mxv{1000005};

int n,s[mxn],maxs;
bool hasDiff[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>s[i];
		if(s[i]>maxs)
			maxs=s[i];
	}

	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			hasDiff[abs(s[i]-s[j])]=true;

	for(int k=n;;++k)
	{
		int m;
		for(m=k;m<=maxs;m+=k)
			if(hasDiff[m])
				goto bad;
		cout<<k<<'\n';
		return 0;
		bad:;
	}
}
```
