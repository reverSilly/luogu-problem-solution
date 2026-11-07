# P1939 【模板】矩阵加速（数列）

## 1. 重新读题

数列满足

$$a_x=\begin{cases}1,&x\in\{1,2,3\},\\ a_{x-1}+a_{x-3},&x\ge4.\end{cases}$$

$T$ 组询问，每次给定 $n$，求 $a_n\bmod (10^9+7)$。

### 关键约束与规模暗示

- $T\le100$，$n\le2\times10^9$。
- $n$ 高达 $2\times10^9$，线性递推（$60\%$ 数据 $n\le2\times10^7$ 的做法）会超时；必须 $O(\log n)$。
- 递推是**常系数线性递推**，且只依赖前面 $3$ 项 → 固定的 $3\times3$ 转移矩阵 + 快速幂。
- 多组询问，每组独立做矩阵快速幂即可（$100\times27\times31$ 次乘法，极小）。

## 2. 思维链（Chain-of-thought）

### 2.1 先试线性递推，卡在 $n$

直接按定义从 $a_1,a_2,a_3$ 推到 $a_n$，时间 $O(n)$。$n=2\times10^9$ 时约 $2\times10^9$ 次加法，必然超时。瓶颈是"每步只前进一项"。

### 2.2 找"可打包"的状态

注意到 $a_x$ 只由 $a_{x-1}$ 和 $a_{x-3}$ 决定。要算出下一项，需要知道最近的三项。把这三项打包成列向量

$$v_k=\begin{pmatrix}a_k\\a_{k-1}\\a_{k-2}\end{pmatrix}.$$

那么 $v_k$ 与 $v_{k-1}$ 的关系是固定的线性变换：

$$\begin{aligned}
a_k&=a_{k-1}+a_{k-3},\\
a_{k-1}&=a_{k-1},\\
a_{k-2}&=a_{k-2}.
\end{aligned}$$

写成矩阵：

$$v_k=\underbrace{\begin{pmatrix}1&0&1\\1&0&0\\0&1&0\end{pmatrix}}_{M}v_{k-1}.$$

**引理 1（矩阵幂公式）**：对任意 $k\ge3$，

$$v_k=M^{k-3}v_3,\qquad v_3=(a_3,a_2,a_1)^{\mathsf T}=(1,1,1)^{\mathsf T}.$$

**证明**：对 $k$ 归纳。$k=3$ 时 $M^0v_3=v_3$。设 $v_{k-1}=M^{k-4}v_3$，则由 $v_k=Mv_{k-1}$ 得

$$v_k=M\cdot M^{k-4}v_3=M^{k-3}v_3. \qquad\blacksquare$$

于是 $a_n$ 就是 $M^{n-3}v_3$ 的第一个分量。因为 $v_3$ 三个分量都是 $1$，它恰好等于 $M^{n-3}$ 第一行三个元素之和（按模计算）。

### 2.3 矩阵快速幂

**引理 2（快速幂正确）**：设 $M$ 为方阵，`qpow(M,e)` 通过二进制拆分 $e=\sum b_i2^i$，令结果 $R=I$，遇到 $b_i=1$ 就 $R\leftarrow R\cdot M^{2^i}$，最终 $R=M^e$。

**证明**：矩阵乘法满足结合律，故 $M^{e_1+e_2}=M^{e_1}M^{e_2}$。对 $e$ 的二进制位数归纳：处理完低 $i$ 位后，$R$ 等于 $M$ 的"已处理位对应指数"次幂，循环中维护的 `x` 等于 $M^{2^i}$。所有位处理完，$R=M^e$。$\blacksquare$

$3\times3$ 矩阵乘法 $O(3^3)=27$ 次乘加，指数 $\le2\times10^9<2^{31}$，最多 $31$ 轮。

### 2.4 边界

$n\le3$ 时按定义直接输出 $1$；否则用 $M^{n-3}$。注意模数 $10^9+7$，所有乘加用 `long long` 并即时取模。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 递推只依赖最近 $3$ 项，是常系数线性递推。
- **reduce to easier problem** — 把"推进一项"打包成固定矩阵的乘法（状态向量 $v_k$）。
- **classic problem solution** — 矩阵快速幂，$O(\log n)$ 求第 $n$ 项。
- **induction proof** — 归纳证明 $v_k=M^{k-3}v_3$ 与快速幂的正确性。
- **special case** — $n\le3$ 直接输出 $1$。

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `T` | `int` | 询问组数 | 读入 | — |
| `n` | `i64` | 每次询问的下标 | 读入 | 可达 $2\times10^9$ |
| `p` | `constexpr i64` | 模数 $10^9+7$ | 常量 | prime |
| `mat` | 结构体 | $3\times3$ 矩阵，`a[3][3]` | 全 $0$ | matrix |
| `qpow(x,e)` | 函数 | 矩阵快速幂 $x^e$ | — | quick power |
| `M` | `mat` | 转移矩阵 $\begin{pmatrix}1&0&1\\1&0&0\\0&1&0\end{pmatrix}$ | 现场构造 | — |

### 4.2 算法步骤

1. 读入 $T$。
2. 对每组询问读入 $n$：
   - 若 $n\le3$，输出 $1$，继续；
   - 构造 $M$：`M.a[0][0]=1,M.a[0][2]=1,M.a[1][0]=1,M.a[2][1]=1`，其余 $0$；
   - 计算 $Q=M^{n-3}$（矩阵快速幂）；
   - 因为 $v_3=(1,1,1)^{\mathsf T}$，答案 $=Q_{00}+Q_{01}+Q_{02}\pmod p$；
   - 输出答案。

### 4.3 复杂度

- 单次询问 $O(3^3\log n)$，总时间 $O(T\cdot27\log n)$，几乎为常数。
- 空间 $O(1)$（只存几个 $3\times3$ 矩阵）。

## 5. 示例代码

```cpp
//
// 题目：P1939 【模板】矩阵加速（数列）
// https://www.luogu.com.cn/problem/P1939
//
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 p{1'000'000'007};

struct mat
{
	i64 a[3][3]{};
};

mat operator*(mat x,mat y)
{
	mat z;
	for(int i=0;i<3;++i)
		for(int k=0;k<3;++k)
			for(int j=0;j<3;++j)
				z.a[i][j]=(z.a[i][j]+x.a[i][k]*y.a[k][j])%p;
	return z;
}

mat qpow(mat x,i64 b)
{
	mat r;
	for(int i=0;i<3;++i)
		r.a[i][i]=1;
	for(;b;b>>=1,x=x*x)
		if(b&1)
			r=r*x;
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
	{
		i64 n;
		cin>>n;
		if(n<=3)
		{
			cout<<1<<'\n';
			continue;
		}
		mat m;
		m.a[0][0]=1;m.a[0][2]=1;
		m.a[1][0]=1;
		m.a[2][1]=1;
		mat q=qpow(m,n-3);
		// v_3=(a3,a2,a1)^T=(1,1,1)^T，取第一行与它的内积
		i64 ans=(q.a[0][0]+q.a[0][1]+q.a[0][2])%p;
		cout<<ans<<'\n';
	}
	return 0;
}
```
