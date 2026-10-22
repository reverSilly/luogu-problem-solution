# P1707 刷题比赛

## 1. 重新读题

三位同学在第 $1$ 天都做了 $1$ 道题（$a_1 = b_1 = c_1 = 1$），在第 $2$ 天都做了 $3$ 道题（$a_2 = b_2 = c_2 = 3$）。对于任意整数 $k \ge 1$，第 $k+2$ 天三人的刷题量由以下耦合递推式给出：

$$\begin{aligned}
a_{k+2} &= p a_{k+1} + q a_k + b_{k+1} + c_{k+1} + r k^2 + t k + 1, \\
b_{k+2} &= u b_{k+1} + v b_k + a_{k+1} + c_{k+1} + w^k, \\
c_{k+2} &= x c_{k+1} + y c_k + a_{k+1} + b_{k+1} + z^k + k + 2.
\end{aligned}$$

给定正整数 $n, m$（$4 \le n \le 10^{16}$，$2 \le m \le 10^{16}$）及常数 $p, q, r, t, u, v, w, x, y, z \in [1, 100]$，求第 $n$ 天三人的刷题量 $a_n, b_n, c_n$ 对 $m$ 取模后的结果。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：递推式里有哪些非齐次项？

数据范围 $n \le 10^{16}$ 直接排除了 $O(n)$ 逐步递推的可能，唯一的出路是对数复杂度的**矩阵快速幂**。

观察三条递推式的右端，除了与前两项相关的线性项（$a_{k+1}, a_k, b_{k+1}, b_k, c_{k+1}, c_k$）之外，还混入了三类随下标 $k$ 变化的非齐次项：
1. **多项式项**：$k^2, k, 1$（二项式展开可将 $(k+1)^2, k+1, 1$ 表示为 $k^2, k, 1$ 的线性组合）；
2. **底数为 $w$ 的指数项**：$w^k$（下一步变为 $w^{k+1} = w \cdot w^k$）；
3. **底数为 $z$ 的指数项**：$z^k$（下一步变为 $z^{k+1} = z \cdot z^k$）。

这意味着：只要把这 $6$ 个递推状态与 $5$ 个辅助生成项合并成一个 $11$ 维列向量，整个非齐次耦合递推就能完全线性化！

### 2.2 构造状态向量与转移矩阵（附完整证明）

**引理 1（状态向量的闭合线性转移）**：对任意整数 $k \ge 1$，定义 $11$ 维列向量
$$V_k = \begin{bmatrix} a_{k+1} & a_k & b_{k+1} & b_k & c_{k+1} & c_k & k^2 & k & 1 & w^k & z^k \end{bmatrix}^T.$$
则存在一个与 $k$ 无关的 $11 \times 11$ 常数矩阵 $T$，使得对一切 $k \ge 1$ 均有 $V_{k+1} = T \cdot V_k$。

**证明**：
逐分量检验 $V_{k+1} = \begin{bmatrix} a_{k+2} & a_{k+1} & b_{k+2} & b_{k+1} & c_{k+2} & c_{k+1} & (k+1)^2 & k+1 & 1 & w^{k+1} & z^{k+1} \end{bmatrix}^T$ 是否均为 $V_k$ 各分量的常数线性组合：
- 第 $0$ 行（$a_{k+2}$）：$p \cdot a_{k+1} + q \cdot a_k + 1 \cdot b_{k+1} + 1 \cdot c_{k+1} + r \cdot k^2 + t \cdot k + 1 \cdot 1$；
- 第 $1$ 行（$a_{k+1}$）：$1 \cdot a_{k+1}$；
- 第 $2$ 行（$b_{k+2}$）：$1 \cdot a_{k+1} + u \cdot b_{k+1} + v \cdot b_k + 1 \cdot c_{k+1} + 1 \cdot w^k$；
- 第 $3$ 行（$b_{k+1}$）：$1 \cdot b_{k+1}$；
- 第 $4$ 行（$c_{k+2}$）：$1 \cdot a_{k+1} + 1 \cdot b_{k+1} + x \cdot c_{k+1} + y \cdot c_k + 1 \cdot k + 2 \cdot 1 + 1 \cdot z^k$；
- 第 $5$ 行（$c_{k+1}$）：$1 \cdot c_{k+1}$；
- 第 $6$ 行（$(k+1)^2$）：由二项式定理 $(k+1)^2 = 1 \cdot k^2 + 2 \cdot k + 1 \cdot 1$；
- 第 $7$ 行（$k+1$）：$1 \cdot k + 1 \cdot 1$；
- 第 $8$ 行（$1$）：$1 \cdot 1$；
- 第 $9$ 行（$w^{k+1}$）：$w \cdot w^k$；
- 第 $10$ 行（$z^{k+1}$）：$z \cdot z^k$。
以上每一行的系数均为不依赖 $k$ 的常数，因此 $V_{k+1} = T \cdot V_k$ 恒成立。由归纳法，对任意 $n \ge 2$，有
$$V_{n-1} = T^{n-2} V_1,$$
其中 $V_{n-1}$ 的第 $0, 2, 4$ 个分量恰好就是所求的 $a_n, b_n, c_n$。$\blacksquare$

### 2.3 碰壁与防溢出细节：$m \le 10^{16}$ 的乘法陷阱

**失败尝试：直接用 64 位整数 `long long` 计算矩阵乘法中的 `(A[i][k] * B[k][j]) % m`。**

- 反例：当 $m = 10^{16}$ 时，矩阵元素最大可达 $m - 1 \approx 10^{16}$。两个 $10^{16}$ 量级的数相乘高达 $10^{32}$，而 `long long` 的上限仅为 $\approx 9.22 \times 10^{18}$，会发生严重的有符号整数溢出（UB）！
- 解决方案：在 C++（64 位环境）中直接使用 `__int128` 承载乘法中间结果（`__int128` 上限为 $\approx 1.7 \times 10^{38} > 10^{32}$），乘完立即对 $m$ 取模，既简洁又无慢速龟速乘的常数开销。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 拆解耦合递推式中的线性项、多项式项（$k^2, k, 1$）与指数项（$w^k, z^k$）。
- **reduce to linear system** — 将 $6$ 个历史状态与 $5$ 个非齐次生成项打包为 $11$ 维状态向量 $V_k$，使每步转移变为常数矩阵乘法（引理 1）。
- **classic problem solution** — 使用矩阵快速幂在 $O(11^3 \log n)$ 次运算内求出 $T^{n-2} V_1$。
- **failed attempt（64 位乘法溢出）** — 模数 $m \le 10^{16}$ 下直接 `i64 * i64` 溢出 $2^{63}-1$，改用 `__int128` 过渡乘法。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `i64` | 目标天数 $n$ 与模数 $m$ | 标准输入 | 题目参数 |
| `p,q,r,t,u,v,w,x,y,z` | `i64` | 递推方程常数 | 标准输入 | 递推系数 |
| `mat` | `i64 [11][11]` | $11 \times 11$ 转移矩阵结构体 | 默认零初始化 | matrix |
| `T` | `mat` | 单步转移矩阵 $V_{k+1} = T \cdot V_k$ | 按引理 1 填充 | transition |
| `V[11]` | `i64 [11]` | 初始列向量 $V_1$ | `[3,1,3,1,3,1,1,1,1,w,z] % m` | initial state |

### 4.2 算法步骤

1. 读入 $n, m$ 及 $10$ 个正整数常数。
2. 按引理 1 构造 $11 \times 11$ 转移矩阵 $T$（所有系数先对 $m$ 取模）。
3. 用二进制矩阵快速幂计算 $P = T^{n-2} \pmod m$，矩阵乘法内部使用 `(__int128)A.a[i][k] * B.a[k][j] % m` 防溢出。
4. 构造初始向量 $V_1 = [3, 1, 3, 1, 3, 1, 1, 1, 1, w \bmod m, z \bmod m]^T$。
5. 将 $P$ 的第 $0, 2, 4$ 行分别与 $V_1$ 做内积并对 $m$ 取模，按格式输出 `nodgd`、`Ciocio`、`Nicole` 的结果。

### 4.3 复杂度分析

- **时间复杂度**：矩阵维度 $D = 11$，快速幂共进行 $O(\log n)$ 次矩阵乘法，总时间复杂度为 $O(D^3 \log n) \approx 1331 \times 54 \approx 7.2 \times 10^4$ 次基本运算，耗时 $< 1\text{ ms}$。
- **空间复杂度**：常数级 $11 \times 11$ 矩阵，空间复杂度为 $O(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1707 刷题比赛
// https://www.luogu.com.cn/problem/P1707
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n,m;
i64 p,q,r,t,u,v,w,x,y,z;

struct mat
{
	i64 a[11][11]{};
};

mat mul(const mat&A,const mat&B)
{
	mat C{};
	for(int i=0;i<11;++i)
		for(int k=0;k<11;++k)
			if(A.a[i][k])
				for(int j=0;j<11;++j)
					if(B.a[k][j])
						C.a[i][j]=(C.a[i][j]+(__int128)A.a[i][k]*B.a[k][j])%m;
	return C;
}

mat qpow(mat A,i64 b)
{
	mat R{};
	for(int i=0;i<11;++i)
		R.a[i][i]=1;
	for(;b;b>>=1,A=mul(A,A))
		if(b&1)
			R=mul(R,A);
	return R;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	cin>>p>>q>>r>>t;
	cin>>u>>v>>w;
	cin>>x>>y>>z;

	if(n==1)
	{
		cout<<"nodgd "<<1%m<<"\nCiocio "<<1%m<<"\nNicole "<<1%m<<'\n';
		return 0;
	}
	if(n==2)
	{
		cout<<"nodgd "<<3%m<<"\nCiocio "<<3%m<<"\nNicole "<<3%m<<'\n';
		return 0;
	}

	// V_k = [a_{k+1}, a_k, b_{k+1}, b_k, c_{k+1}, c_k, k^2, k, 1, w^k, z^k]^T
	mat T{};
	T.a[0][0]=p%m; T.a[0][1]=q%m; T.a[0][2]=1; T.a[0][4]=1;
	T.a[0][6]=r%m; T.a[0][7]=t%m; T.a[0][8]=1;
	T.a[1][0]=1;

	T.a[2][0]=1; T.a[2][2]=u%m; T.a[2][3]=v%m; T.a[2][4]=1; T.a[2][9]=1;
	T.a[3][2]=1;

	T.a[4][0]=1; T.a[4][2]=1; T.a[4][4]=x%m; T.a[4][5]=y%m;
	T.a[4][7]=1; T.a[4][8]=2%m; T.a[4][10]=1;
	T.a[5][4]=1;

	T.a[6][6]=1; T.a[6][7]=2%m; T.a[6][8]=1;
	T.a[7][7]=1; T.a[7][8]=1;
	T.a[8][8]=1;
	T.a[9][9]=w%m;
	T.a[10][10]=z%m;

	mat P=qpow(T,n-2);
	i64 V[11]={3%m,1%m,3%m,1%m,3%m,1%m,1%m,1%m,1%m,w%m,z%m};
	auto get_row=[&](int row){
		i64 s=0;
		for(int j=0;j<11;++j)
			s=(s+(__int128)P.a[row][j]*V[j])%m;
		return s;
	};
	cout<<"nodgd "<<get_row(0)<<'\n';
	cout<<"Ciocio "<<get_row(2)<<'\n';
	cout<<"Nicole "<<get_row(4)<<'\n';
	return 0;
}
```
