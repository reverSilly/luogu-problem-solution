# P1962 斐波那契数列

## 1. 重新读题

### 1.1 问题重述（人话版）

$$F_n=\begin{cases}1,&n\le 2,\\ F_{n-1}+F_{n-2},&n\ge 3,\end{cases}$$
给定 $n$，求 $F_n\bmod (10^9+7)$。

### 1.2 关键约束与规模暗示

- $60\%$ 数据 $1\le n\le 92$：$F_{92}$ 还在 `long long` 范围内，直接线性递推即可。
- $100\%$ 数据 $1\le n<2^{63}$：$n$ 高达约 $9.2\times10^{18}$，$O(n)$ 递推绝对不可行，必须 $O(\log n)$。
- 递推是二阶线性常系数递推，这类问题最标准的 $O(\log n)$ 做法就是**矩阵快速幂**（也可用快速倍增公式，本质相同）。
- 模数 $10^9+7$，矩阵元素相乘约 $10^{18}$，在 `long long` 范围内（$9.22\times10^{18}$），加两项也不会溢出。

---

## 2. 思维链（Chain-of-thought）

### 2.1 为什么线性递推不可行

直接按定义从 $F_1,F_2$ 推到 $F_n$，时间复杂度 $O(n)$。$n$ 可以到 $9\times10^{18}$，即使每秒 $10^9$ 次也无法完成。

但二阶递推的状态可以压缩成二维向量：想求下一个数，只需要知道最近的两个数。于是
$$
\begin{pmatrix}F_{k+1}\\F_k\end{pmatrix}
=
\begin{pmatrix}1&1\\1&0\end{pmatrix}
\begin{pmatrix}F_k\\F_{k-1}\end{pmatrix}.
$$
记
$$
M=\begin{pmatrix}1&1\\1&0\end{pmatrix},
$$
那么从 $(F_2,F_1)=(1,1)$ 出发，$M^{n-1}$ 就能一次“跳”到 $(F_{n+1},F_n)$。求矩阵的幂可以用二进制快速幂，$O(\log n)$。

### 2.2 矩阵幂的封闭形式

**引理 1（$M^k$ 的元素就是斐波那契数）**：约定 $F_0=0,\ F_1=1,\ F_2=1$，则对任意整数 $k\ge 1$，
$$
M^k=\begin{pmatrix}F_{k+1}&F_k\\F_k&F_{k-1}\end{pmatrix}.
$$

**证明**（对 $k$ 归纳）：

- $k=1$：$M^1=\begin{pmatrix}1&1\\1&0\end{pmatrix}=\begin{pmatrix}F_2&F_1\\F_1&F_0\end{pmatrix}$，因为 $F_2=1,F_1=1,F_0=0$，成立。
- 假设对 $k$ 成立，则
$$
M^{k+1}=M^kM
=
\begin{pmatrix}F_{k+1}&F_k\\F_k&F_{k-1}\end{pmatrix}
\begin{pmatrix}1&1\\1&0\end{pmatrix}
=
\begin{pmatrix}F_{k+1}+F_k&F_{k+1}\\F_k+F_{k-1}&F_k\end{pmatrix}
=
\begin{pmatrix}F_{k+2}&F_{k+1}\\F_{k+1}&F_k\end{pmatrix}.
$$
最后一步用了递推式 $F_{k+2}=F_{k+1}+F_k$ 与 $F_{k+1}=F_k+F_{k-1}$。归纳完成。$\square$

**推论**：对 $n\ge 1$，$F_n=(M^{n-1})_{0,0}$。

**证明**：由引理 1 取 $k=n-1\ge 0$。当 $n=1$ 时 $M^0=I$，$(M^0)_{0,0}=1=F_1$；当 $n\ge 2$ 时 $(M^{n-1})_{0,0}=F_n$。$\square$

### 2.3 二进制快速幂为什么正确

**引理 2（快速幂正确性）**：设 $e$ 的二进制为 $\sum b_t2^t$。维护两个矩阵 `res=I` 与 `base=M`，从低位到高位扫描：若 $b_t=1$ 则 `res=res*base`；随后 `base=base*base`。循环结束后 `res=M^e`。

**证明**（循环不变式）：在准备处理第 $t$ 位时，`base=M^{2^t}`，`res` 等于 $M$ 的“已处理位的贡献”次幂，即 `res=M^s`，其中 $s=\sum_{j<t}b_j2^j$。

- 初始 $t=0$：`base=M^{1}=M^{2^0}`，`s=0`，`res=I=M^0`，成立。
- 若第 $t$ 位为 $1$，则新贡献 $s'=s+2^t$，`res*base=M^s\cdot M^{2^t}=M^{s+2^t}`；为 $0$ 时不变。随后 `base` 平方变成 $M^{2^{t+1}}$。
- 处理完所有位后 $s=e$，故 `res=M^e`。$\square$

矩阵乘法满足结合律，因此平方与相乘的次序无关，上式成立。

### 2.4 实现与溢出

- 只用 $2\times2$ 矩阵；乘法的第三重循环里 $(x\cdot y)$ 最大约 $(10^9)^2=10^{18}$，加上一个不超过 $10^9+6$ 的已有值仍 $<9.22\times10^{18}$，`long long` 安全。
- $n=1$ 时指数 $n-1=0$，快速幂不进入循环，`res=I`，输出 $(I)_{0,0}=1$，正确。
- 读入用 `long long`（$n<2^{63}$，恰好适合有符号 64 位）。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 二阶线性递推只需保存最近两项，可写成二维状态向量。
- **classic problem solution** — 构造转移矩阵 $M=\begin{pmatrix}1&1\\1&0\end{pmatrix}$，答案为 $(M^{n-1})_{0,0}$。
- **prove by induction** — 归纳证明 $M^k$ 的四个元素恰为 $F_{k+1},F_k,F_k,F_{k-1}$。
- **binary exponentiation** — $O(\log n)$ 求矩阵幂，循环不变式保证正确。

---

## 4. 真实解法（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `i64` | 要求的项数 | 读入 | $<2^{63}$ |
| `p` | `constexpr i64` | 模数 $10^9+7$ | — | prime |
| `M` / `base` | `mat` | 转移矩阵 $\begin{pmatrix}1&1\\1&0\end{pmatrix}$ | 构造 | matrix |
| `res` | `mat` | 当前的幂 $M^{s}$ | 单位阵 $I$ | result |
| `e` | `i64` | 剩余指数，初始 $n-1$ | $n-1$ | exponent |

### 4.2 算法步骤

1. 读入 `n`。
2. 若 `n=1`，输出 `1`（快速幂自然也能处理，指数为 $0$）。
3. 构造 `base=M`，`res=I`，`e=n-1`。
4. 当 `e>0`：若 `e` 为奇数则 `res=res*base`；`base=base*base`；`e>>=1`。
5. 输出 `res.a[0][0] mod p`。

### 4.3 复杂度分析

- 时间：$O(\log n)$ 次 $2\times2$ 矩阵乘法，每次常数次运算；$n<2^{63}$ 时最多约 $63$ 轮。
- 空间：$O(1)$。

---

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int p{1'000'000'007};

struct mat
{
	i64 a[2][2];
};

mat mul(mat x,mat y)
{
	mat z{};
	for(int i=0;i<2;++i)
		for(int k=0;k<2;++k)
			if(x.a[i][k])
				for(int j=0;j<2;++j)
					z.a[i][j]=(z.a[i][j]+x.a[i][k]*y.a[k][j])%p;
	return z;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 n;
	cin>>n;
	mat base{};
	base.a[0][0]=1;base.a[0][1]=1;base.a[1][0]=1;
	mat res{};
	res.a[0][0]=1;res.a[1][1]=1;
	for(i64 e=n-1;e>0;e>>=1)
	{
		if(e&1)
			res=mul(res,base);
		base=mul(base,base);
	}
	cout<<res.a[0][0]%p<<'\n';
	return 0;
}
```
