# P1314 聪明的质监员

> 标签：数学 / 二分 / 前缀和 · 难度：普及/提高-

## 1. 重新读题

**问题重述（简化版）**

$n$ 个矿石排成一排，第 $j$ 个重量 $w_j$，价值 $v_j$。给定 $m$ 个区间 $[l_i,r_i]$ 和标准值 $s$。

选一个阈值 $W$，对每个区间 $[l_i,r_i]$：

- **计数** $c_i(W)=\#\{j\in[l_i,r_i]\mid w_j\ge W\}$
- **加权和** $d_i(W)=\sum_{j\in[l_i,r_i]}[w_j\ge W]\,v_j$

区间检验值 $y_i(W)=c_i(W)\cdot d_i(W)$，总检验结果 $y(W)=\sum_{i=1}^{m}y_i(W)$。

求 $\min_W |s-y(W)|$。

**约束与子任务（提示复杂度）**

| 数据范围 | 规模 |
|---|---|
| 10% | $n,m\le 10$ |
| 30% | $n,m\le 500$ |
| 50% | $n,m\le 5000$ |
| 70% | $n,m\le 10000$ |
| 100% | $n,m\le 2\times10^5$，$0<w_i,v_i\le10^6$，$0<s\le10^{12}$ |

子任务阶梯式增长，说明需要"每次数线性 + 外层对数次调用"才能撑住 100%。

**值域**：$w_j\in[1,10^6]$，$W$ 是整数。$W\in[1,10^6]$ 时部分矿石激活；$W\ge10^6+1$ 时全失活，$y=0$。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索特殊机制 —— $y(W)$ 对 $W$ 的依赖结构

**固定区间，分析单个 $y_i(W)$**：

$$y_i(W)=c_i(W)\cdot d_i(W),\quad c_i(W)=\sum_{j=l_i}^{r_i}\mathbb{1}[w_j\ge W],\quad d_i(W)=\sum_{j=l_i}^{r_i}\mathbb{1}[w_j\ge W]v_j$$

$w_j\ge W$ 等价于 $W\le w_j$。当 $W$ **增大**时，一些矿石从激活变失活：

- 每失活一个矿石 $j$：$c_i$ 减 1（计数 -1），$d_i$ 减 $v_j$（$v_j>0$，加权和减正数）。
- 故 $W$ 增大时，$c_i$ 与 $d_i$ 都只可能不变或减小。
- 又 $c_i\ge0,\ d_i\ge0$，两个非负因子各不增，乘积不增。

**引理 1（区间级单调性）**

对任意区间 $[l_i,r_i]$，$y_i(W)$ 关于 $W$ 非增：$W_1<W_2\Rightarrow y_i(W_1)\ge y_i(W_2)$。

**证明**：设 $W_1<W_2$，令 $S_1=\{j\in[l_i,r_i]:w_j\ge W_1\}$，$S_2=\{j\in[l_i,r_i]:w_j\ge W_2\}$。

- 若 $j\in S_2$，则 $w_j\ge W_2>W_1$，故 $j\in S_1$，即 $S_2\subseteq S_1$。
- 故 $|S_2|\le|S_1|$，即 $c_i(W_2)\le c_i(W_1)$。
- 由 $v_j>0$ 且 $S_2\subseteq S_1$：$\sum_{j\in S_2}v_j\le\sum_{j\in S_1}v_j$，即 $d_i(W_2)\le d_i(W_1)$。
- 两非负因子各不增，乘积不增：$y_i(W_2)=c_i(W_2)d_i(W_2)\le c_i(W_1)d_i(W_1)=y_i(W_1)$。$\blacksquare$

**引理 2（全局单调性）**

$y(W)=\sum_i y_i(W)$ 关于 $W$ 非增。

**证明**：有限个非增函数之和仍非增，由引理 1 逐项相加即得。$\blacksquare$

> **机制小结**：$W$ 单调地"关掉"矿石，每个区间贡献非增，故全局 $y(W)$ 非增。这是整道题的根基。

### 2.2 简化问题 —— 单调阶梯函数的"最近点"在何处

**引理 3（$y$ 是阶梯函数）**

$y(W)$ 只取决于集合 $\{j:w_j\ge W\}$，该集合仅在 $W$ 跨越某个 $w_j$ 时改变，故 $y(W)$ 是阶梯函数，跳变点有限（至多 $n$ 个 $w_j$）。

**证明**：固定 $W$，$[w_j\ge W]$ 是 $w_j$ 与 $W$ 的比较结果。若 $W$ 从 $a$ 变到 $b$（$a<b$）且开区间 $(a,b)$ 内无 $w_j$，则对所有 $j$，$[w_j\ge a]=[w_j\ge b]$，$y(a)=y(b)$。跳变仅发生在 $W$ 跨过某个 $w_j$。$\blacksquare$

**引理 4（最优点在交点两侧）**

设 $K=\max\{W\in[1,10^6+1]:y(W)\ge s\}$，若不存在则 $K=0$。则使 $|s-y(W)|$ 最小的 $W\in\{K,K+1\}\cap[1,10^6+1]$（去掉越界项）。

**证明**：由引理 2，$y$ 非增，集合 $\{W:y(W)\ge s\}$ 是前缀 $[1,K]$（$K$ 可能为 0 即空集）。

- **$W\in[1,K]$ 段**：$y(W)\ge s$，$|s-y(W)|=y(W)-s$。$y$ 非增 $\Rightarrow$ $y(W)-s$ 非增 $\Rightarrow$ 该段 $|s-y|$ 最小在 $W=K$。
- **$W\in[K+1,10^6+1]$ 段**：$y(W)<s$，$|s-y(W)|=s-y(W)$。$y$ 非增 $\Rightarrow$ $s-y(W)$ 非降 $\Rightarrow$ 该段 $|s-y|$ 最小在 $W=K+1$。
- 综合：全局最小在 $\{K,K+1\}$（若 $K=0$ 则第一段为空，候选仅 $W=1$）。$\blacksquare$

> 引理 4 严格处理了平台情况（$y$ 可能有常数段），不漏解。

**推论**：二分定位 $K$，再算 $|s-y(K)|$ 与 $|s-y(K+1)|$ 取小即可。二分 $\log_2(10^6+1)\approx20$ 次。

### 2.3 经典问题识别 —— 单次 $y(W)$ 求值

**引理 5（单次求值 $O(n+m)$）**

给定 $W$，可在 $O(n+m)$ 内算出 $y(W)$。

**证明**：

- **$O(n)$ 建前缀**：设 $A[i]=\sum_{j<i}\mathbb{1}[w_j\ge W]$（前 $i-1$ 个激活数），$B[i]=\sum_{j<i}\mathbb{1}[w_j\ge W]v_j$（前 $i-1$ 个激活价值和），$A[0]=B[0]=0$。递推：
$$A[j]=A[j-1]+\mathbb{1}[w_j\ge W],\quad B[j]=B[j-1]+\mathbb{1}[w_j\ge W]\,v_j,\quad j=1..n$$
一次线性扫描 $O(n)$。

- **$O(m)$ 查区间**：对区间 $[l_i,r_i]$，
$$c_i=A[r_i]-A[l_i-1],\quad d_i=B[r_i]-B[l_i-1],\quad y_i=c_i\cdot d_i$$
累加得 $y$，$O(m)$。合计 $O(n+m)$。$\blacksquare$

**总复杂度**：$O((n+m)\log V)$，$V=10^6$，$\log_2 V=20$。$n+m\le4\times10^5$，总 $\approx8\times10^6$ 次运算。

### 2.4 边界与溢出

- **值域**：$W\in[1,10^6]$ 正常算；$W=10^6+1$ 时全失活，$y=0$。二分范围 $[1,10^6+1]$。
- **溢出**：
  - $c_i\le n\le2\times10^5$，$d_i\le\sum v_j\le2\times10^5\times10^6=2\times10^{11}$。
  - $y_i=c_i\cdot d_i\le2\times10^5\times2\times10^{11}=4\times10^{16}$。
  - $y=\sum_{i=1}^{m}y_i\le2\times10^5\times4\times10^{16}=8\times10^{21}$。
  - $8\times10^{21}>2^{63}-1\approx9.2\times10^{18}$，**必须用 `__int128`** 存 $y$ 与累加器。
  - $B$ 前缀和 $\le2\times10^{11}$，`i64` 够；$A$ 计数 $\le2\times10^5$，`int` 够。
  - 答案 $|s-y|\le s+y\le10^{12}+8\times10^{21}$，也用 `__int128`，输出需 `__int128→字符` 函数。

### 2.5 完整算法骨架

```
read n, m, s
for j in 1..n: read w[j], v[j]
for i in 1..m: read l[i], r[i]

function calc(W):                       // 返回 y(W), __int128
    if W > maxW: return 0               // 全失活
    A[0]=0; B[0]=0
    for j in 1..n:
        A[j]=A[j-1]+(w[j]>=W)
        B[j]=B[j-1]+(w[j]>=W ? v[j] : 0)
    y = 0
    for i in 1..m:
        c = A[r[i]] - A[l[i]-1]
        d = B[r[i]] - B[l[i]-1]
        y += c * d                       // __int128
    return y

// 二分找最大 W 使 y(W)>=s
K = 0; lo = 1; hi = 10^6+1
while lo <= hi:
    mid = (lo+hi)/2
    if calc(mid) >= s: K = mid; lo = mid+1
    else:               hi = mid-1

// 候选 {K, K+1} 去重、过滤到 [1, 10^6+1]
ans = +inf
for W in {K, K+1} with 1<=W<=10^6+1:
    ans = min(ans, |s - calc(W)|)
print ans
```

---

## 3. 模式总结（Pattern Summary）

按**实际思考顺序**：

- **explore its property** — 把 "$w_j\ge W$" 看成阈值开关：$W$ 单调地"关掉"矿石，每个区间 $c_i\cdot d_i$（两非负因子各不增）非增，故 $y(W)$ 全局非增（引理 1+2）。
- **consider easier version** — $y$ 是阶梯函数，$|s-y|$ 先降后升，最优点在 $y$ 跨越 $s$ 的交点两侧相邻两点（引理 3+4）。
- **classic problem solution** — 二分答案：$\log_2 V\approx20$ 次求值定位 $K=\max\{W:y(W)\ge s\}$，候选 $\{K,K+1\}$（引理 4）。
- **reduce to easier problem** — 单次 $y(W)$：区间计数 + 区间加权和 = 前缀和差，$O(1)$ 每区间（引理 5）。
- **classic problem solution** — 前缀和 $A$（计数）/ $B$（价值），$O(n)$ 建 + $O(m)$ 查，单次 $O(n+m)$。
- **overflow guard** — $y$ 最大 $\approx8\times10^{21}>2^{63}$，`__int128` 累加、比较、输出；$B$ 前缀和用 `i64`，$A$ 用 `int`。

---

## 4. 真实解法（Real Solution）

### 4.1 符号表（Notation Glossary）

| 符号 | 含义 | 类型 / 初始化 | 记忆点 |
|---|---|---|---|
| `n` | 矿石数 | `int`，输入 | $\le2\times10^5$ |
| `m` | 区间数 | `int`，输入 | $\le2\times10^5$ |
| `s` | 标准值 | `i64`，输入 | $\le10^{12}$ |
| `w[i]` | 第 $i$ 矿石重量 | `int w[mxn]`，1-based | $\in[1,10^6]$ |
| `v[i]` | 第 $i$ 矿石价值 | `i64 v[mxn]`，1-based | $\in[1,10^6]$ |
| `L[i]` | 第 $i$ 区间左端点 | `int L[mxn]`，1-based | $1\le L\le R\le n$ |
| `R[i]` | 第 $i$ 区间右端点 | `int R[mxn]`，1-based | |
| `A[i]` | 前缀计数 $\sum_{j<i}\mathbb{1}[w_j\ge W]$ | `int A[mxn]`，`A[0]=0` | 前 $i-1$ 激活数 |
| `B[i]` | 前缀价值和 $\sum_{j<i}\mathbb{1}[w_j\ge W]v_j$ | `i64 B[mxn]`，`B[0]=0` | 前 $i-1$ 激活价值和 |
| `K` | 最大 $W$ 使 $y(W)\ge s$（0 表示不存在） | `int`，二分结果 | 交点左端 |
| `ans` | $\min|s-y|$ | `__int128` | 最终答案 |
| `maxW` | 矿石重量上界 | `constexpr int {1000000}` | $W$ 有效上界 |

> **下标约定**：内部 1-based（与题目一致，前缀和天然 1-based 不易错）。

### 4.2 算法步骤（Numbered Steps）

1. **读入**：$n,m,s$；$n$ 组 $(w_i,v_i)\to w[1..n],v[1..n]$；$m$ 组 $(l_i,r_i)\to L[1..m],R[1..m]$。
2. **`calc(W)`**：返回 $y(W)$（`__int128`）。
   - 若 $W>maxW$ 直接返回 0（全失活）。
   - $O(n)$ 建前缀：$A[0]=B[0]=0$，$j=1..n$：$A[j]=A[j-1]+\mathbb{1}[w_j\ge W]$，$B[j]=B[j-1]+\mathbb{1}[w_j\ge W]\,v_j$。
   - $O(m)$ 累加：$y=0$，$i=1..m$：$c=A[R_i]-A[L_i-1]$，$d=B[R_i]-B[L_i-1]$，$y\mathrel{+}=(\texttt{\_\_int128})c\cdot d$。
   - 返回 $y$。
3. **二分定位** $K=\max\{W\in[1,maxW+1]:y(W)\ge s\}$（不存在 $K=0$）：
   - `K=0; lo=1; hi=maxW+1;`
   - `while(lo<=hi)`：`mid=(lo+hi)/2;` 若 `calc(mid)>=(i64)s` 则 `K=mid; lo=mid+1;` 否则 `hi=mid-1;`。
4. **枚举候选**：对 $W\in\{K,K+1\}\cap[1,maxW+1]$（去重），`ans=min(ans,|s-calc(W)|)`。
   - $K=0$ 时 $\{K,K+1\}=\{0,1\}$，过滤后只取 $W=1$。
   - $K=maxW+1$ 时 $\{K,K+1\}=\{maxW+1,maxW+2\}$，过滤后只取 $W=maxW+1$（$y=0$）。
5. **输出** `ans`（`__int128`，用 `pr128` 函数）。

**复杂度**：
- 时间：$O((n+m)\log V)$，$\log V=\lceil\log_2(10^6+1)\rceil=20$，总 $\approx(n+m)\times22\le4\times10^5\times22=8.8\times10^6$。
- 空间：$O(n+m)$（`w,v,A,B,L,R` 全局数组）。

### 4.3 验证状态

- **样例说明验证**：题目说明 $W=4$ 时三区间检验值 $20,5,0$，$y=25$，$\min|s-y|=10$（故 $s=15$ 或 $35$）。算法在 $W=4$ 时 `calc(4)` 应返回 25；候选集含 $W=4$ 附近点，$\min$ 取得 10。
- **单调性自检**：手工 $n=3,m=1$，$w=[3,5,2],v=[10,20,30]$，$[l,r]=[1,3]$，验证 $y(W)$ 随 $W$ 增大非增。
- **溢出自检**：$y$ 累加 `__int128`，中间 $c\cdot d$ 强转 `(__int128)c*d`，比较 `calc(mid)>=(i64)s` 自动提升。
- **复杂度自检**：$\approx8.8\times10^6$ 次运算，远低于典型 1s 时限（$\sim10^8$ 次/s）。

---

## 5. 示例代码（C++）

```cpp
// P1314 [NOIP 2011 提高组] 聪明的质监员
// 二分 + 前缀和
// https://www.luogu.com.cn/problem/P1314
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{200005};
constexpr int maxW{1000000};   // w 上界

int n,m;
i64 s;
int w[mxn];
i64 v[mxn];
int L[mxn],R[mxn];
int A[mxn];
i64 B[mxn];

// 输出 __int128
void pr128(__int128 x)
{
	if(x<0)
	{
		putchar('-');
		x=-x;
	}
	if(x>=10)
		pr128(x/10);
	putchar(x%10+'0');
}

// 给定阈值 W，算检验结果 y(W)
__int128 calc(int W)
{
	if(W>maxW)
		return 0;              // 全失活
	A[0]=0;B[0]=0;
	for(int j=1;j<=n;++j)
	{
		int act=(w[j]>=W);
		A[j]=A[j-1]+act;
		B[j]=B[j-1]+(act?v[j]:0);
	}
	__int128 y=0;
	for(int i=1;i<=m;++i)
	{
		int c=A[R[i]]-A[L[i]-1];
		i64 d=B[R[i]]-B[L[i]-1];
		y+=(__int128)c*d;
	}
	return y;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>s;
	for(int i=1;i<=n;++i)
		cin>>w[i]>>v[i];
	for(int i=1;i<=m;++i)
		cin>>L[i]>>R[i];

	// 二分找最大 W 使 y(W)>=s（W 范围 [1, maxW+1]）
	int K=0,lo=1,hi=maxW+1;
	while(lo<=hi)
	{
		int mid=(lo+hi)/2;
		if(calc(mid)>=(i64)s)
		{
			K=mid;
			lo=mid+1;
		}
		else
			hi=mid-1;
	}

	// 候选 {K, K+1} 过滤到 [1, maxW+1]，去重
	__int128 ans=1145141919810;
	int cand[2]={K,K+1};
	sort(cand,cand+2);
	int last=-1;
	for(int idx=0;idx<2;++idx)
	{
		int W=cand[idx];
		if(W<1||W>maxW+1)
			continue;
		if(W==last)
			continue;
		last=W;
		__int128 yv=calc(W);
		__int128 diff=yv-( __int128)s;
		if(diff<0)
			diff=-diff;
		if(diff<ans)
			ans=diff;
	}

	pr128(ans);
	putchar('\n');
	return 0;
}
```

### 代码说明

- **全局静态数组**：`w,v,A,B,L,R` 全部全局，1-based（与题目一致，前缀和天然 1-based，避免 0-based 转换的边界错误）。
- **`__int128`**：`y` 累加器与 `ans` 用 `__int128`（$y$ 最大 $\approx8\times10^{21}>2^{63}$）；`B` 前缀和用 `i64`（$\le2\times10^{11}$）；`A` 计数用 `int`（$\le2\times10^5$）。
- **`pr128`**：`__int128` 递归数位输出。
- **二分**：找最大 $W\in[1,maxW+1]$ 使 $y(W)\ge s$，$K=0$ 表示不存在（$y$ 全程 $<s$）。
- **候选枚举**：`{K,K+1}` 排序去重，过滤到 $[1,maxW+1]$。$K=0$ 时只取 $W=1$；$K=maxW+1$ 时只取 $W=maxW+1$（$y=0$）。
- **稀疏注释**：仅在关键不变量（溢出、全失活特判）处标注。

> **复杂度复核**：每次 `calc` $O(n+m)$，二分 $\lceil\log_2(10^6+1)\rceil=20$ 次 + 候选至多 2 次，总 $\approx22\times(n+m)\le22\times4\times10^5=8.8\times10^6$，远低于时限。
