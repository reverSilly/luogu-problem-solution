# P1390 公约数的和

> 标签：数学 / 数论 · 欧拉函数 · 莫比乌斯反演 · 容斥原理 · 整除分块 · 难度：省选/NOI- · 洛谷原创

## 1. 重新读题

**题面原样**：给定 $n$，求 $\sum_{i=1}^{n}\sum_{j=i+1}^{n}\gcd(i,j)$，即 $1\sim n$ 这 $n$ 个数中"每任意两个不同的数"的最大公约数之和。输入一行一个整数 $n$，输出一行一个整数表示答案。

**白话重述**：把 $\binom{n}{2}$ 个无序对 $(i,j)$（$i<j$）的 gcd 全部相加。**只有一组询问**（背景里也只给了一个 $n$）、**不取模**、答案原样输出——所以"会不会爆 `int`"必须由自己算清楚，而不是靠取模躲掉。

**数据范围**

| 量 | 范围 |
|---|---|
| $n$ | $2\le n\le 2\times10^{6}$ |
| 40% 档 | $n\le 2\times10^{3}$ |

分档在暗示什么：$n\le2\times10^3$ 时 $\binom{n}{2}=1999000\approx2\times10^{6}$ 对，逐对调用 `gcd` 就能拿到 40 分；满分档 $\binom{n}{2}\approx2\times10^{12}$ 对，**"枚举数对"这条路必须整体换掉**。背景中"LXL 争取 100 秒出解 / TIBBAR 想 1 秒过"正是这两档的比喻。

**样例情况声明**：题面文件 `D:\Information\My\OI\luogu_problems\P1390.md` 中**没有任何样例输入/输出**（只有题面、数据范围、标签），因此本文所有验证只能手算（按定义暴力枚举数对 + 两条独立公式互验 + 边界自造小例），**无法与官方答案对标**。题面给出的标签（莫比乌斯反演/容斥/整除分块/欧拉函数）是外部提示，本文以题面的求和式为准：下文 §2.7 会给出容斥/莫比乌斯路线并证明它与 §2.5 的欧拉函数主式同值，§2.8 给出整除分块，但**主代码走最直的 $O(n)$ 路线**。

---

## 2. 思维链（Chain-of-thought）

### 2.1 按定义暴力：先算清"枚举对象"的账

最朴素的做法：二重循环 + `std::gcd`。

- $n=2\times10^3$：$\binom{n}{2}=1999000$ 对，每对 $\gcd$ 的欧几里得算法在最坏（相邻斐氏数）情形约 $\log_{\phi}(2\times10^3)\approx16$ 步取模，通常远少于此，估算 $\le 3.2\times10^{7}$ 次基本操作 —— 40 分稳。
- $n=2\times10^6$：$\binom{n}{2}=1999999000000\approx2\times10^{12}$ 对，即使每对只用 1 次操作也要 $2\times10^{12}$，按 $10^8$/秒的常识量级是 $2\times10^{4}$ 秒，**彻底不可行**。

矛盾点很清楚：求和式里 $i,j$ 是"位置"，而我们真正要求的是"值"（gcd 是多少）。**把计数对象从"位置"换成"值"**是这类题的标准转向，下一步就做这个。

**先按 40% 档打一张小表，看看有没有线索**（$n\le2\times10^3$ 的暴力可以直接写：二重循环累加 `std::gcd(i,j)`，约 $2\times10^{6}$ 对）。手算到 $n=8$，把"第 $n$ 行新增加多少"单独列出来：

| $n$ | 第 $n$ 行 $\sum_{i=1}^{n-1}\gcd(i,n)$（逐项） | 行和 | $T(n)$ |
|---|---|---|---|
| 2 | 1 | 1 | 1 |
| 3 | 1,1 | 2 | 3 |
| 4 | 1,2,1 | 4 | 7 |
| 5 | 1,1,1,1 | 4 | 11 |
| 6 | 1,2,3,2,1 | 9 | 20 |
| 7 | 1,1,1,1,1,1 | 6 | 26 |
| 8 | 1,2,1,4,1,2,1 | 12 | 38 |

行和序列 $1,2,4,4,9,6,12$ 看不出闭式递推（$n=6$ 突然 9、$n=7$ 又掉到 6），说明**按行（按位置）推进是死路**——它把"与 $n$ 的因子结构有关"的信息全打散在一行里。但表里已经埋着线索：第 7 行全是 1（7 是素数，$\{i<7\}$ 全与 7 互素），第 8 行的 4 出现在 $\gcd(4,8)$、2 出现在 $\gcd(2,8),\gcd(6,8)$，即"$\gcd(i,n)=d$ 的个数只跟 $n/d$ 有关，等于 $\varphi(n/d)$"（$d\mid n$ 且 $d<n$）。这正好是下面引理 2 的逐行版本，两条路（按值分层 / 按行分层）都会在引理 3 处汇合到欧拉函数。本文选**按值分层**，因为它直接给出定理 1 的一维和式。

### 2.2 按 gcd 的值分层（引理 1）

记 $c_d=\#\{(i,j):1\le i<j\le n,\ \gcd(i,j)=d\}$。

**引理 1（分层计数）** $T(n)=\sum\limits_{i=1}^{n}\sum\limits_{j=i+1}^{n}\gcd(i,j)=\sum\limits_{d=1}^{n}d\,c_d$。

**证明** 设 $S=\{(i,j):1\le i<j\le n\}$，有限集。对每个 $(i,j)\in S$ 令 $v(i,j)=\gcd(i,j)$，则 $1\le v(i,j)\le i<n$ 故值域含于 $\{1,\dots,n\}$。按值把 $S$ 划分为 $\{A_d\}_{d=1}^{n}$，$A_d=\{(i,j)\in S:v(i,j)=d\}$，两两不交且并集为 $S$（划分性：每个元素的值唯一落在某一个 $A_d$）。于是
$$\sum_{(i,j)\in S}v(i,j)=\sum_{d=1}^{n}\sum_{(i,j)\in A_d}d=\sum_{d=1}^{n}d\,|A_d|=\sum_{d=1}^{n}d\,c_d,$$
第一个等号是"有限和按划分重排"（加法结合律/交换律，有限和可任意重排），第二个等号是内层每项都等于常数 $d$、共 $|A_d|$ 项。$\blacksquare$

现在唯一的问题是：**$c_d$ 怎么算快**。

### 2.3 缩比：$\gcd=d$  ⟺  互素对（引理 2）

**引理 2（缩比双射）** 设 $m=\left\lfloor\dfrac nd\right\rfloor$，则 $c_d=C(m)$，其中 $C(m)=\#\{(a,b):1\le a<b\le m,\ \gcd(a,b)=1\}$。

**证明** 构造两个映射并验证互逆（映射记作 $\sigma,\tau$，以免与欧拉函数 $\varphi$ 撞名）。
- $\sigma:B_d\to P_m$，$\sigma(i,j)=(i/d,\ j/d)$，其中 $B_d=\{(i,j):i<j,\ d\mid i,\ d\mid j\}$，$P_m=\{(a,b):1\le a<b\le m\}$。合法性：$d\mid i$ 使 $i/d\in\mathbb{Z}_{\ge1}$；$i<j\Rightarrow i/d<j/d$；$j\le n$ 且 $d\mid j$ 使 $b=j/d\le n/d$，而 $b$ 是整数故 $b\le\lfloor n/d\rfloor=m$。
- $\tau:P_m\to B_d$，$\tau(a,b)=(da,db)$。合法性：$1\le da<db$，且 $db\le dm=d\lfloor n/d\rfloor\le n$，故 $(da,db)$ 仍是 $S$ 里的对且 $d$ 整除两者。
- 互逆：$\tau(\sigma(i,j))=(d\cdot i/d,d\cdot j/d)=(i,j)$；$\sigma(\tau(a,b))=(da/d,db/d)=(a,b)$。两个复合都是恒等，故 $\sigma,\tau$ 互为逆映射、都是双射。
- 保持性：$\gcd(i,j)=d\iff\gcd(i/d,j/d)=1$。$\Rightarrow$：设 $g=\gcd(i/d,j/d)>1$，取素因子 $p\mid g$，则 $i,j$ 都被 $dp$ 整除，故 $\gcd(i,j)\ge dp>d$，矛盾。$\Leftarrow$：若 $\gcd(a,b)=1$，则 $\gcd(da,db)=d\gcd(a,b)=d$（逐素数比较指数：对素数 $q$，$v_q(da)=v_q(d)+v_q(a)$，$v_q(db)=v_q(d)+v_q(b)$，故 $\min$ 为 $v_q(d)+\min(v_q(a),v_q(b))=v_q(d)+0=v_q(d)$，对一切 $q$ 成立即 $\gcd(da,db)=d$）。

最后一步把两个集合对上：记 $X_d=\{(i,j)\in B_d:\gcd(i,j)=d\}$（按定义 $c_d=|X_d|$），$Y_m=\{(a,b)\in P_m:\gcd(a,b)=1\}$（按定义 $C(m)=|Y_m|$）。由保持性，$\sigma(x)\in Y_m$ 对每个 $x\in X_d$ 成立，$\tau(y)\in X_d$ 对每个 $y\in Y_m$ 成立；又 $\sigma,\tau$ 在整体上互逆，故它们限制到 $X_d,Y_m$ 上仍互为逆映射，即 $\sigma|_{X_d}:X_d\to Y_m$ 是双射。两边取元素数得 $c_d=C(m)$。$\blacksquare$

**这一步的价值**：$c_d$ 只依赖 $\lfloor n/d\rfloor$，不依赖 $d$ 的其他性质；而且所有 $d$ 用的是**同一个函数 $C$**。原来的"$n$ 个不同的计数问题"塌缩成"一个函数 $C$ 在 $O(\sqrt n)$ 个点上的取值"。

### 2.4 数互素对：欧拉函数登场（引理 3）

按第二分量 $b$ 逐行拆 $C(m)$：$C(m)=\sum_{b=2}^{m}\#\{a:1\le a<b,\ \gcd(a,b)=1\}$（$b=1$ 那一行是空集，因为不存在 $a<1$）。

**引理 3** 记 $\varphi(b)=\#\{a:1\le a\le b,\ \gcd(a,b)=1\}$（欧拉函数，$\varphi(1)=1$），$\Phi(m)=\sum_{k=1}^{m}\varphi(k)$，则
$$C(m)=\Phi(m)-1 .$$

**证明** 先拆成两部分：对固定的 $b\ge2$，$\varphi(b)$ 统计的是 $1\le a\le b$ 中与 $b$ 互素的 $a$ 的个数。把范围切成 $a<b$ 与 $a=b$：当 $a=b$ 时 $\gcd(b,b)=b\ge2>1$，**这一项本来就不被 $\varphi$ 计入**，故 $\varphi(b)=\#\{a:1\le a<b,\gcd(a,b)=1\}$ 对一切 $b\ge2$ 成立。代回：
$$C(m)=\sum_{b=2}^{m}\varphi(b)=\sum_{b=1}^{m}\varphi(b)-\varphi(1)=\Phi(m)-1,$$
最后一步用了 $\varphi(1)=1$：按定义 $\varphi(1)=\#\{a:1\le a\le1,\gcd(a,1)=1\}=\#\{1\}=1$（$\gcd(1,1)=1$）。之所以要减 1，是因为 $b=1$ 那一行在 $C$ 里贡献 0（没有 $a<1$），却在 $\Phi$ 里被记了 $\varphi(1)=1$——这是"$\varphi$ 的定义含 $a=b$"与"$C$ 要求 $a<b$"之间的唯一缝隙。$\blacksquare$

**自检**：$m=4$。$\varphi(1..4)=1,1,2,2$，$\Phi(4)=6$，公式给 $C(4)=5$。直接数 $[1,4]^2$ 中 $a<b$ 的互素对：$(1,2),(1,3),(1,4),(2,3),(3,4)$ —— $(2,4)$ 因 $\gcd=2$ 不算，共 5 ✓。

### 2.5 主公式（定理 1）与对称形式自检

**定理 1** $T(n)=\sum\limits_{d=1}^{n}d\left(\Phi\!\left(\left\lfloor\dfrac nd\right\rfloor\right)-1\right)$。

**证明** 引理 1 给 $T=\sum_d d\,c_d$，引理 2 给 $c_d=C(\lfloor n/d\rfloor)$，引理 3 给 $C(m)=\Phi(m)-1$，三个等式逐点代入即得。求和上限取 $n$ 是因为 $d>n$ 时 $c_d=0$：$\gcd(i,j)\le i\le n$，故 $d$ 的实际取值被 $\{1,\dots,n\}$ 覆盖，多余项 $\lfloor n/d\rfloor=0$、$\Phi(0)-1$ 无定义——所以代码里 $d$ 只跑到 $n$，且 $n/d\ge1$ 恒成立。$\blacksquare$

**引理 4（对称形式，用于自检）** 设 $f(n)=\sum_{i=1}^{n}\sum_{j=1}^{n}\gcd(i,j)$（全体有序对），则
$$f(n)=\sum_{d=1}^{n}d\left(2\Phi\!\left(\left\lfloor\tfrac nd\right\rfloor\right)-1\right)=\frac{n(n+1)}{2}+2T(n).$$

**证明** 仿引理 1/2：$f=\sum_d d\,w_d$，$w_d=\#\{(i,j)\in[1,n]^2:\gcd(i,j)=d\}$。缩比双射（同引理 2，去掉 $a<b$ 的限制）给 $w_d=\#\{(a,b)\in[1,m]^2:\gcd(a,b)=1\}=M(m)$。把 $[1,m]^2$ 按 $a<b$、$a=b$、$a>b$ 三块划分：对角线上 $\gcd(a,a)=a$，互素当且仅当 $a=1$，故对角块贡献 1 个点 $(1,1)$；$a<b$ 块恰是 $C(m)$；$a>b$ 块由对称映射 $(a,b)\mapsto(b,a)$ 与 $a<b$ 块双射，也是 $C(m)$。故 $M(m)=2C(m)+1=2\Phi(m)-1$（引理 3）。第一式得证。
另一方面，同一集合 $[1,n]^2$ 按 $i<j$ / $i=j$ / $i>j$ 划分，$i=j$ 块的和是 $\sum_{i=1}^{n}i=\frac{n(n+1)}{2}$，$i<j$ 块的和按定义是 $T(n)$，$i>j$ 块因 $\gcd$ 对称也等于 $T(n)$，故 $f=\frac{n(n+1)}2+2T$。$\blacksquare$

**自检 $n=4$**：定理 1 给 $1\cdot(6-1)+2\cdot(2-1)+3\cdot0+4\cdot0=5+2=7$。引理 4 给 $f=1\cdot11+2\cdot3+3\cdot1+4\cdot1=11+6+3+4=24$，且 $\frac{4\cdot5}{2}+2\cdot7=10+14=24$ ✓ 两式互洽。

### 2.6 预处理 $\Phi$：线性筛的正确性（引理 5~8）

定理 1 的外层是 $O(n)$ 次查表，关键是 $\Phi(1..n)$ 能 $O(n)$ 算出来。

**引理 5（$\varphi$ 的基本值）** $\varphi(1)=1$；$p$ 为素数时 $\varphi(p)=p-1$；$k\ge1$ 时 $\varphi(p^{k})=p^{k}-p^{k-1}=p^{k-1}(p-1)$。

**证明** $\varphi(1)=1$ 见引理 3。素数 $p$：$1\le a\le p$ 中 $\gcd(a,p)>1$ 者必须是 $p$ 的倍数（$p$ 的正因子只有 $1,p$，故 $\gcd(a,p)$ 只能是 $1$ 或 $p$），在 $[1,p]$ 里 $p$ 的倍数只有 $a=p$ 一个，故 $\varphi(p)=p-1$。素数幂：对 $1\le a\le p^{k}$，$\gcd(a,p^{k})>1\iff p\mid a$（$p^{k}$ 的每个大于 1 的因子都被 $p$ 整除，故 $\gcd$ 有 $p$ 这个素因子 $\iff p\mid a$）；$[1,p^{k}]$ 中 $p$ 的倍数恰 $p^{k-1}$ 个（$p,2p,\dots,p^{k}$，一一对应 $1\le q\le p^{k-1}$），故 $\varphi(p^{k})=p^{k}-p^{k-1}=p^{k-1}(p-1)$。$\blacksquare$

**引理 6（积性）** $\gcd(m,n)=1\Rightarrow\varphi(mn)=\varphi(m)\varphi(n)$。

**证明** 先统一记号：令 $U_t=\{r:0\le r<t,\ \gcd(r,t)=1\}$（$\mathbb{Z}/t\mathbb{Z}$ 中的可逆剩余类）。断言 $|U_t|=\varphi(t)$：若 $t\ge2$，映射 $a\mapsto a\bmod t$ 把 $\{a:1\le a\le t,\gcd(a,t)=1\}$ 送到 $U_t$，它是双射——$\gcd(a,t)=\gcd(a\bmod t,t)$（因为 $a=(a\bmod t)+qt$，而 $\gcd(x+qt,t)=\gcd(x,t)$：两边的公因子集合相同），故映射良定义且保持互素；$a=t$ 时 $\gcd(t,t)=t>1$ 不在定义域内，所以像落在 $\{1,\dots,t-1\}$，与 $U_t$ 一致（$0\notin U_t$ 因 $\gcd(0,t)=t>1$），单射由"同余类代表唯一"给出；若 $t=1$，两边都恰 1 个元素（$U_1=\{0\}$，$\gcd(0,1)=1$；$\varphi(1)=1$）。断言成立。

考虑中国剩余映射 $\pi:\mathbb{Z}/mn\mathbb{Z}\to\mathbb{Z}/m\mathbb{Z}\times\mathbb{Z}/n\mathbb{Z}$，$\pi(x)=(x\bmod m,\ x\bmod n)$。由 CRT（$\gcd(m,n)=1$ 时，对任意 $(u,v)$ 存在唯一 $x\bmod mn$ 使 $x\equiv u\ (m),\ x\equiv v\ (n)$），$\pi$ 是环同构，特别地是双射。再证"$x$ 可逆 $\iff$ 两个分量都可逆"：
- $\Rightarrow$：若 $\gcd(x,mn)=1$ 而 $\gcd(x,m)=g>1$，取 $g$ 的素因子 $p$，则 $p\mid m\mid mn$ 且 $p\mid x$，与 $\gcd(x,mn)=1$ 矛盾。$n$ 同理。
- $\Leftarrow$：若 $\gcd(x,m)=\gcd(x,n)=1$ 而 $\gcd(x,mn)>1$，取素因子 $p\mid\gcd(x,mn)$，则 $p\mid x$ 且 $p\mid mn$；由欧几里得引理（$p$ 素数、$p\mid mn$）得 $p\mid m$ 或 $p\mid n$——证明：若 $p\nmid m$，则 $\gcd(p,m)=1$，由 $p\mid mn$ 与"$\gcd(p,m)=1$ 时 $p\mid m\cdot n\Rightarrow p\mid n$"（贝祖定理：存在 $\alpha,\beta$ 使 $\alpha p+\beta m=1$，乘 $n$ 得 $\alpha pn+\beta mn=n$，两项都被 $p$ 整除故 $p\mid n$）；无论哪种都与 $\gcd(x,m)=1$ 或 $\gcd(x,n)=1$ 矛盾（$p\mid x$ 且 $p$ 整除对应模数）。故 $\gcd(x,mn)=1$。
于是 $\pi$ 把 $U_{mn}$ 双射到 $U_m\times U_n$：单射因为 $\pi$ 整体是单射；满射因为任取 $(u,v)\in U_m\times U_n$，CRT 给出 $x\bmod mn$ 使两分量等于 $u,v$，再由上面 $\Leftarrow$ 方向知 $x\in U_{mn}$，即 $(u,v)$ 有原像。两边取元素数：$\varphi(mn)=|U_{mn}|=|U_m\times U_n|=|U_m|\,|U_n|=\varphi(m)\varphi(n)$。$\blacksquare$

**引理 7（线性筛的递推）** 设 $p$ 为素数，$i\ge2$。
- 若 $p\mid i$，则 $\varphi(ip)=p\,\varphi(i)$；
- 若 $\gcd(i,p)=1$，则 $\varphi(ip)=\varphi(i)(p-1)$。

**证明** 第二式：引理 6 的积性直接给 $\varphi(ip)=\varphi(i)\varphi(p)=\varphi(i)(p-1)$（引理 5）。第一式：写 $i=p^{a}u$，$a\ge1$（因 $p\mid i$），$\gcd(u,p)=1$。则 $\varphi(i)=\varphi(p^{a})\varphi(u)$（引理 6，$\gcd(p^a,u)=1$ 因为 $u$ 的所有素因子都 $\ne p$），而 $ip=p^{a+1}u$ 给 $\varphi(ip)=\varphi(p^{a+1})\varphi(u)$。由引理 5，$\varphi(p^{a})=p^{a-1}(p-1)$，$\varphi(p^{a+1})=p^{a}(p-1)=p\cdot\varphi(p^{a})$。故 $\varphi(ip)=p\,\varphi(p^a)\varphi(u)=p\,\varphi(i)$。$\blacksquare$

**引理 8（线性筛覆盖且不重不漏）** 上述递推配合"每个合数 $x$ 恰被其**最小**素因子 $p$ 与 $i=x/p$ 生成一次"，使 $O(n)$ 次操作内求出 $\varphi(2..n)$，从而 $O(1)$ 累加得 $\Phi$。

**证明** 先证明一条不变式：**代码每写一次 `s[i*pri[j]]`，$p=\texttt{pri}[j]$ 都是 $x=ip$ 的最小素因子。** 内层按素数递增枚举，且在第一个满足 $p\mid i$ 的 $p$ 之后立刻 `break`。设 $\ell$ 为 $i$ 的最小素因子。对枚举过程中用到的 $p$：要么 $p<\ell$（此时 $p\nmid i$，走 `s[i]*(p-1)` 分支），要么 $p=\ell$（此时 $p\mid i$，走 `s[i]*p` 分支，随后 `break`）；$p>\ell$ 永远取不到（在 $\ell$ 处已经 `break`）。若 $p<\ell$，则 $p$ 小于 $i$ 的一切素因子，故 $p$ 是 $x=ip$ 的最小素因子；若 $p=\ell$，同样 $p$ 是 $x$ 的最小素因子。不变式成立。

- **不重（唯一性）**：由不变式，生成 $x$ 时 $p=\ell(x)$（$x$ 的最小素因子）且 $i=x/p$ ——两者都由 $x$ 唯一决定，故 $x$ 至多被生成一次。
- **不漏（存在性）**：合数 $x$ 令 $p=\ell(x)$、$i=x/p$，则 $2\le i<x\le n$（因 $p\ge2$），故外层必会跑到 $i$；此时 $p\le\ell(i)$（否则 $\ell(i)<p=\ell(x)$ 且 $\ell(i)\mid i\mid x$，与 $p$ 是 $x$ 的最小素因子矛盾），于是内层枚举到 $p$ 之前不会 `break`；判界也过得去：$ip=x\le n\Rightarrow p\le n/i$，而 $p$ 是整数、整数除法 $\texttt{n/i}=\lfloor n/i\rfloor$（正数情形向零取整即下取整），"整数 $\le$ 实数 $\Rightarrow$ 整数 $\le$ 其下取整"，故 $p\le\texttt{n/i}$ 成立，$p$ 会被取到，$x$ 被生成。素数 $x$ 则由外层 `!vis[x]` 分支直接定值 $\varphi(x)=x-1$。
- **值正确**：生成 $x=ip$ 时 $i<x$（因 $p\ge2$），故 $\varphi(i)$ 已先算出，引理 7 的两条递推恰好覆盖 $p\mid i$ 与 $p\nmid i$ 两种情形，写出的就是 $\varphi(x)$。
- **计数**：合数共 $n-1-\pi(n)$ 个，每个一次赋值；素数 $\pi(n)$ 次定值；内层判界比较次数 $\le$ 赋值次数 $+\,n$（每个 $i$ 至多一次失败退出）$\le2n$。故筛的总操作 $\le3n=6\times10^{6}$，前缀和 $n=2\times10^{6}$ 次加法，累加 $n=2\times10^{6}$ 次乘加，合计 $\le10^{7}$ 次基本操作（细账见 §4.2，那里把内层判界单独列出）。$\blacksquare$

**量级核算**：$n=2\times10^6$ 时，筛 $\le3n=6\times10^{6}$（赋值 $n$ + 判界/循环控制 $\le2n$，见引理 8 计数条）、前缀和 $n=2\times10^{6}$ 次加法、定理 1 的累加 $n=2\times10^{6}$ 次乘加，三段合计 $\le5n=10^{7}$ 次基本操作（更细的账与空间见 §4.2）。

### 2.7 另一条路：容斥 / 莫比乌斯反演（引理 9），以及为什么不走它

标签提示"容斥、莫比乌斯反演"。把引理 3 换成容斥版本：

**引理 9** $2C(m)+1=\sum\limits_{k=1}^{m}\mu(k)\left\lfloor\dfrac mk\right\rfloor^{2}$，即 $C(m)=\frac12\left(\sum_k\mu(k)\lfloor m/k\rfloor^2-1\right)$。

**证明** $[1,m]^2$ 中 $\gcd(a,b)$ 可被 $k$ 整除的对数：$k\mid a$ 有 $\lfloor m/k\rfloor$ 个选择（$a=kq,\ 1\le q\le m/k$，$q$ 整数故 $q\le\lfloor m/k\rfloor$，且不同 $q$ 给出不同 $a$），$k\mid b$ 同理独立，故 $\lfloor m/k\rfloor^2$（"倍数计数=下取整"逐维应用，与引理 2 里的 $\lfloor n/d\rfloor$ 同源）。设 $\gamma(t)=\#\{(a,b)\in[1,m]^2:\gcd(a,b)=t\}$，则 $h(k):=\sum_{t:\,k\mid t}\gamma(t)$ 恰是"$k\mid\gcd(a,b)$"的对数，即 $h(k)=\lfloor m/k\rfloor^2$。
下面把反演步骤补全（不靠"显然"）：对有限和交换求和次序，
$$\sum_{k=1}^{m}\mu(k)h(k)=\sum_{k=1}^{m}\mu(k)\sum_{t:\,k\mid t}\gamma(t)=\sum_{t=1}^{m}\gamma(t)\sum_{k\mid t}\mu(k).$$
内层 $\sum_{k\mid t}\mu(k)$：设 $t$ 有 $r$ 个不同素因子，则 $\mu(k)\ne0$ 的 $k$ 恰是 $\mathrm{rad}(t)$ 的无平方因子因子，共 $\binom rs$ 个含 $s$ 个素因子的，于是 $\sum_{k\mid t}\mu(k)=\sum_{s=0}^{r}\binom rs(-1)^s=(1-1)^{r}$，$r=0$（即 $t=1$）时为 $1$，$r\ge1$ 时为 $0$。故整个和只剩 $t=1$ 一项，等于 $\gamma(1)$。而 $\gcd=1$ 的有序对数 $\gamma(1)=2C(m)+1$（引理 4 的三块划分：对角只有 $(1,1)$，上下两块各 $C(m)$）。整理即得 $\sum_k\mu(k)\lfloor m/k\rfloor^2=2C(m)+1$。$\blacksquare$

**数值核对**：$m=6$，$\mu=1,-1,-1,0,-1,1$，$\lfloor6/k\rfloor^2=36,9,4,1,1,1$，加权和 $=36-9-4+0-1+1=23$，而 $2\Phi(6)-1=2\cdot12-1=23$ ✓（$\Phi(6)=1+1+2+2+4+2=12$）。

**为什么主代码不走它**：两式数值相同（引理 9 与引理 3 给出同一个数），但走莫比乌斯路线要求
$$\sum_{d=1}^{n}d\sum_{k\le n/d}\mu(k)\left\lfloor\frac n{dk}\right\rfloor^{2},$$
把 $(d,k)$ 全部展开是 $\sum_d n/d\approx n\ln n\approx2.9\times10^7$ 项（$n=2\times10^6$：$\ln n=14.51$，$n\ln n=2.90\times10^7$），比引理 3 路线的 $2\times10^6$ 慢一个数量级；而且 $\mu$ 本身也要筛，白多一个数组。**结论**：容斥路线是"同一条定理的另一种写法"，本题单询问、$n\le2\times10^6$，用 $\varphi$ 前缀和更省。

**其余候选路线一并算账**（都是"算同一个数"，只比快慢；避免以后条件反射地选错）：

| 路线 | 式子 | 操作数（$n=2\times10^6$） | 判定 |
|---|---|---|---|
| (a) 定义暴力 | $\sum_{i<j}[\gcd]$ | $\binom n2\approx2\times10^{12}$ | 仅 40% 档（$2\times10^{6}$ 对）可行 |
| (b) 按行 + 枚举因子（差分递推，见 §4.3(6)） | $T=\sum_{k\le n}\left(\sum_{d\mid k}d\varphi(k/d)-k\right)$ | 先筛 $\varphi$（$2\times10^{6}$）再让每个 $d$ 累加到它的所有倍数：$\sum_{d\le n}\frac nd=n\ln n\approx2.9\times10^{7}$ | **可行且很好写**，比主解慢一个数量级（按 $10^8$/秒的常识量级换算是 $0.3$ 秒量级，非实测）；是天然的"参考实现"思路 |
| (c) 莫比乌斯全展开（引理 9） | $\sum_d d\sum_k\mu(k)\lfloor n/(dk)\rfloor^2$ | $\approx2.9\times10^{7}$，且需额外筛 $\mu$ | 同值不划算 |
| (d) 主路线（定理 1 + 引理 8 的线性筛） | $\sum_d d(\Phi(\lfloor n/d\rfloor)-1)$ | 筛 $2\times10^{6}$ + 前缀和 $2\times10^{6}$ + 累加 $2\times10^{6}$ | **最优，$O(n)$** |
| (e) (d) + 引理 10 分块 | 同上，外层变块和 | 累加段 $2\times10^{6}\to2830$，总仍 $O(n)$ | 只在"多次询问/$n$ 极大"时有意义 |
| (f) 只枚举素数 $d$ 或只统计"$d$ 的倍数对数" | — | — | **错**：$\gcd$ 值可以是合数，$n=6$ 时 $\gcd(2,4)=2$、$\gcd(3,6)=3$、$\gcd(4,6)=2$，但 $T(6)=20$ 里的 $d=1$ 层就贡献 11，跳过合数层会漏；且 $\{d\mid\gcd\}$ 的倍数计数 $\lfloor n/d\rfloor^2$ 必须反演才能得到"恰为 $d$" |

### 2.8 整除分块（引理 10）：外层还能再压

定理 1 里被求和项只通过 $\lfloor n/d\rfloor$ 依赖 $d$，故可把相同的 $q=\lfloor n/d\rfloor$ 合并。

**引理 10** 设 $l\ge1$，$q=\lfloor n/l\rfloor\;(\ge1)$，$r=\lfloor n/q\rfloor$。则
(i) $\forall d\in[l,r]:\lfloor n/d\rfloor=q$；(ii) $\lfloor n/(r+1)\rfloor<q$；(iii) 不同 $q$ 的个数 $\le2\lceil\sqrt n\rceil$。

**证明** (i) $d\le r=\lfloor n/q\rfloor$ 且 $q\ge1$，两边乘 $q$ 得 $dq\le q\lfloor n/q\rfloor\le q\cdot(n/q)=n$，故 $q\le n/d$；$q$ 是整数，而"整数 $\le$ 实数 $\Rightarrow$ 整数 $\le$ 该实数的下取整"，于是 $q\le\lfloor n/d\rfloor$。又 $d\ge l\Rightarrow n/d\le n/l\Rightarrow\lfloor n/d\rfloor\le\lfloor n/l\rfloor=q$（下取整保序：$x\le y\Rightarrow\lfloor x\rfloor\le\lfloor y\rfloor$，因为 $\lfloor x\rfloor\le x\le y$ 且 $\lfloor y\rfloor$ 是 $\le y$ 的最大整数，故 $\lfloor x\rfloor\le\lfloor y\rfloor$）。两侧夹住得 $\lfloor n/d\rfloor=q$。
(ii) $r=\lfloor n/q\rfloor\Rightarrow r\le n/q<r+1\Rightarrow n/(r+1)<q\Rightarrow\lfloor n/(r+1)\rfloor\le q-1$（整数 $\le$ 严格小的实数，则 $\le$ 该实数下取整 $\le q-1$）。
(iii) 值 $\lfloor n/d\rfloor$ 分两类：$d\le\sqrt n$ 的 $d$ 至多 $\lceil\sqrt n\rceil$ 个，每个贡献一个值；$d>\sqrt n$ 时 $\lfloor n/d\rfloor\le n/d<\sqrt n$，故值只能是 $0,1,\dots,\lceil\sqrt n\rceil-1$ 中若干个，也至多 $\lceil\sqrt n\rceil$ 个。合计 $\le2\lceil\sqrt n\rceil$。$\blacksquare$

合并后 $T(n)=\sum_{\text{块}[l,r]}\left(\sum_{d=l}^{r}d\right)\left(\Phi(q)-1\right)$，其中 $q=\lfloor n/l\rfloor$（引理 10 (i) 保证整块同值，(ii) 保证块恰到此为止、不重叠不遗漏）。块内等差和 $S=\sum_{d=l}^{r}d$ 的闭式：把和式正序、倒序各写一遍相加，$2S=\sum_{d=l}^{r}(l+r)=(r-l+1)(l+r)$，故 $S=\frac{(l+r)(r-l+1)}2$；该除法不丢余数，因为 $(l+r)-(r-l+1)=2l-1$ 是奇数，说明 $l+r$ 与 $r-l+1$ 一奇一偶，乘积必为偶数。$n=2\times10^6$ 时块数 $\le2\lceil\sqrt{2\times10^6}\rceil=2\times1415=2830$。

**诚实的定位**：筛 $\Phi$ 本身已是 $O(n)=2\times10^6$，分块只把外层 $2\times10^6$ 次乘加压到 $\approx2.8\times10^{3}$ 次，**总复杂度仍是 $O(n)$，不是必需的**；它只有在" $\Phi$ 已经预处理好、需要多次不同 $n$ 的查询，或 $n$ 大到 $\Phi$ 只能用杜教筛亚线性求出"时才有实质意义。本文把它作为互验的第二种累加写法（§5 `calc2`），两者结果必须逐位相同。

### 2.9 溢出预算

严格上界（只用 $\varphi(k)\le k$ 与 $\lfloor n/d\rfloor\le n/d$，不依赖任何渐近公式）：
$$0\le d\left(\Phi(m)-1\right)\le d\,\Phi(m)\le d\cdot\frac{m(m+1)}2\le d\cdot\frac{(n/d)(n/d+1)}2=\frac{n^2/d+n}{2}=\frac{n^2}{2d}+\frac n2 .$$
对 $d=1..n$ 求和（$m=\lfloor n/d\rfloor$，用 $\Phi(m)\le\sum_{k\le m}k=\frac{m(m+1)}2$，$H_n\le\ln n+1$）：
$$T(n)\le\frac{n^2}{2}H_n+\frac{n^2}{2}\le\frac{n^2}{2}(\ln n+2).$$
取 $n=2\times10^6$：$n^2=4\times10^{12}$，$\ln n=\ln2+6\ln10=0.6931+13.8155=14.5087$，得 $T\le2\times10^{12}\times16.5087\approx3.3\times10^{13}$。

| 量 | 上界算式 | 结论 |
|---|---|---|
| $\varphi(i)$ | $\le n=2\times10^{6}$ | `int` 够，但存进 `i64` 数组以承载前缀和 |
| $\Phi(m)\le\frac{m(m+1)}2$ | $m=n$ 时 $\le2\times10^{12}$；真值 $\approx\frac{3}{\pi^2}n^2=1.22\times10^{12}$ | **必须 `i64`**（`int` 上限 $2.147\times10^{9}$，超约 $570$ 倍；答案本身超约 $8600$ 倍） |
| 单项 $d(\Phi(\lfloor n/d\rfloor)-1)$ | $\le\frac{n^2}{2d}+\frac n2\le\frac{n^2}{2}+\frac n2\approx2\times10^{12}$ | `i64` |
| 答案 $T(n)$ | $\le3.3\times10^{13}$；渐近估计 $\approx\frac{3n^2}{\pi^2}(\ln n+\gamma)\approx1.84\times10^{13}$ | `i64`（上限 $9.22\times10^{18}$，余量约 $2.8\times10^{5}$ 倍） |
| 分块块值和 $\frac{(l+r)(r-l+1)}2(\Phi(q)-1)$ | 它是 $T(n)$ 的一个非负子和（引理 10 (i)），故 $\le T(n)\le3.3\times10^{13}$；宽松乘积界 $(l+r)(r-l+1)\le2n\cdot n=8\times10^{12}$ 也需 `i64` | `(i64)(l+r)*(r-l+1)` 先转 `i64` 再乘，否则 $8\times10^{12}>2^{31}$ 直接溢出 |
| 筛内下标 `i*pri[j]` | 判界写作 `pri[j]<=n/i`，乘积只在 $i\cdot p\le n$ 时出现，$\le2\times10^{6}$ | `int` 够；用除法判界避免"先乘再比"的隐性溢出 |

取整方向：$n,d$ 均为正整数，C++ 的 `/` 向零取整，正数情形与下取整 $\lfloor n/d\rfloor$ 完全一致，无需额外补丁；$d\in[1,n]$ 保证除数非零。

---

## 3. 模式总结（Pattern Summary）

- **explore its property / 先算账** — $\binom n2$ 对：$2\times10^6$ 可暴力、$2\times10^{12}$ 不可，问题定位在"枚举对象是位置"。
- **换计数对象（按值分层）** — 引理 1：$\sum_{\text{对}}\gcd=\sum_d d\cdot c_d$，有限和按划分重排。
- **缩比（齐次性）** — 引理 2：$\gcd(i,j)=d\iff(i,j)=d(a,b),\ \gcd(a,b)=1$，构造显式双射并验互逆；$c_d$ 只依赖 $\lfloor n/d\rfloor$。
- **classic problem solution（逐行拆 + 欧拉函数）** — 引理 3：$C(m)=\Phi(m)-1$，"$\varphi$ 定义含 $a=b$、目标要 $a<b$"的差由 $\varphi(1)=1$ 一处补平。
- **reduce to easier problem** — 定理 1：二重求和 $\to$ 一维 $\sum_d$ + 一次查表，$O(n)$ 完成。
- **对称性自检（独立第二公式）** — 引理 4：$f(n)=\sum d(2\Phi-1)=\frac{n(n+1)}2+2T(n)$，用来交叉校验主式。
- **classic problem solution（线性筛）** — 引理 5~8：积性 + 素数幂 + $p\mid i$/$p\nmid i$ 两条递推 + "最小素因子生成一次"的不重不漏证明。
- **容斥/莫比乌斯对照路线** — 引理 9：$2C(m)+1=\sum_k\mu(k)\lfloor m/k\rfloor^2$；同值但 $n\ln n\approx2.9\times10^7$ 项，本题不划算。
- **整除分块 + 等差块和** — 引理 10：相同 $q=\lfloor n/d\rfloor$ 合并，块数 $\le2\lceil\sqrt n\rceil=2830$；单询问下只是互验写法，不改总复杂度。
- **overflow guard** — $\Phi(n)\approx1.2\times10^{12}$（超 `int` 约 $570$ 倍）、$T(n)\lesssim3.3\times10^{13}$（超 `int` 约 $1.5\times10^{4}$ 倍），前缀和与累加全程 `i64`。

---

## 4. 真实解法（Real Solution）

### 4.1 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|---|---|---|---|
| `n` | 题面上界 | `int`，输入，$2\le n<\texttt{mxn}$ | 唯一输入 |
| `mxn` | $2\times10^{6}+5$ 松弛 | `constexpr int` | 数组尺寸 |
| `s[i]` | **两段语义**：筛完后是 $\Phi(i)=\sum_{k\le i}\varphi(k)$；筛的过程中是 $\varphi(i)$ | `i64` 数组，全局零初始化；`s[1]=1`，`s[0]=0` 作前缀和哨兵 | "s = sigma/前缀和" |
| `pri[j]`,`pcnt` | 已筛出的素数表及其个数 | `int` 数组 / 计数，初值 0 | 线性筛用 |
| `vis[i]` | $i$ 是否被标记为合数 | `bool` 数组，全局 0 | 未标记即素数 |
| `d`,`q` | 枚举的 gcd 值 / $q=\lfloor n/d\rfloor$ | `int` | 定理 1 的两个角色 |
| `l`,`r` | 分块版的块端点 | `int`，`l=1` 起，`r=n/(n/l)` | 引理 10 |
| `ans` | 累加结果 $T(n)$ | `i64`，$=0$ 起 | 答案 |

**下标约定**：本解法**全程 1-based**（`s[i]` 就是数学的 $\Phi(i)$，`d` 就是题面里的 gcd 值）。理由：数论函数定义在正整数上，筛法与 $\lfloor n/d\rfloor$ 都天然 1-based，强行 0-based 反而每处要 $\pm1$、易错。唯一哨兵是 `s[0]=0`（全局零初始化保证），使 `for(i=1;i<=n;++i) s[i]+=s[i-1]` 一步成立。全文（`ini`/`calc`/`calc2`/验证表）均按此约定，无转换点。

### 4.2 算法步骤

1. 读 `n`；`assert(2<=n&&n<mxn)` 文档化输入合法性。
2. `ini()`：置 `s[1]=1`；对 $i=2..n$，未标记则入素数表并 `s[i]=i-1`（引理 5）；内层对每个满足 $p\le n/i$ 的素数 $p$ 标记 `vis[i*p]`，并按 $p\mid i$ 与否分别写 `s[i*p]=s[i]*p` 或 `s[i*p]=s[i]*(p-1)`（引理 7），$p\mid i$ 时 `break`（引理 8）。筛完后 `s[i]+=s[i-1]` 原地把 $\varphi$ 转成 $\Phi$（定义即引理 3 里的 $\Phi$）。
3. `calc()`：`for(d=1;d<=n;++d) ans+=(i64)d*(s[n/d]-1);`（定理 1）。
4. `assert(ans==calc2())`：分块版（引理 10 + 等差块和）与朴素版互验。
5. 输出 `ans`。

**复杂度算式**

- 时间：线性筛 $\le3n=6\times10^{6}$（赋值 $n$ 次 + 内层判界/循环控制 $\le2n$ 次，引理 8 计数条）；前缀和 $n=2\times10^{6}$ 次加法；`calc` $n=2\times10^{6}$ 次"1 除法 + 1 查表 + 1 乘加"；`calc2` $\le2\lceil\sqrt n\rceil=2830$ 块。总计 $\le5n+2830\approx1.0\times10^{7}$ 次基本操作，其中除法 $\le4\times10^{6}$ 次（筛内 $\texttt{n/i}$ 与 `calc` 的 `n/d` 各约 $n$ 次，加上循环条件重复求值）；按 $10^{8}$/秒的量级常识换算约为 $0.1$ 秒量级，远低于 1 秒（**该数字是操作数量的换算，不是实测**，本环境禁止运行）。
- 空间：`s` $8\times2000005\approx16.0$ MB，`pri` $4\times2000005\approx8.0$ MB，`vis` $1\times2000005\approx2.0$ MB，合计 $\approx26$ MB（$\varphi$ 与 $\Phi$ 共用 `s` 省下一个 `int` 数组的 8 MB）。

### 4.3 验证（全部手算，本环境禁止编译/运行）

纪律：**无编译器、无运行**，以下全为纯手算；且题面未给样例，**无法与官方答案对标**，改用"定义暴力 + 两条独立公式互验"支撑结论。

**(0) $n\le6$ 按定义全枚举（$\binom n2$ 个对，逐个写 gcd）**

| $n$ | 全部 $i<j$ 的 $\gcd$ | 暴力 $T(n)$ | 定理 1 手算 |
|---|---|---|---|
| 2 | (1,2)=1 | 1 | $1\cdot(\Phi(2){-}1)=1$ → 1 ✓ |
| 3 | 1,1,1 | 3 | $1\cdot3+2\cdot0=3$ ✓ |
| 4 | 1,1,1,1,2,1 | 7 | $1\cdot5+2\cdot1+0+0=7$ ✓ |
| 5 | 上 6 对 + (1..4,5)=1,1,1,1 | 11 | $1\cdot9+2\cdot1+3\cdot0+4\cdot0+5\cdot0=11$ ✓ |
| 6 | 上 + (1..5,6)=1,2,3,2,1 → +9 | 20 | $1\cdot11+2\cdot3+3\cdot1+0=20$ ✓ |

其中 $\Phi(1..6)=1,2,4,6,10,12$（$\varphi=1,1,2,2,4,2$），$\lfloor n/d\rfloor$ 逐点代入。$n=5$ 行：$d=1$ 时 $\Phi(5)-1=9$，$d=2$ 时 $\lfloor5/2\rfloor=2$、$2\times(\Phi(2)-1)=2$，$d\ge3$ 时 $\lfloor5/d\rfloor=1$、$\Phi(1)-1=0$。

**(1) $n=20$ 逐步复现（两条独立公式对撞）** — 手算 $\varphi(1..20)=1,1,2,2,4,2,6,4,6,4,10,4,12,6,8,8,16,6,18,8$，累加得 $\Phi=1,2,4,6,10,12,18,22,28,32,42,46,58,64,72,80,96,102,120,128$。
- 主式：$d=1..10$ 的 $\lfloor20/d\rfloor=20,10,6,5,4,3,2,2,2,2$，$d\ge11$ 时 $\lfloor20/d\rfloor=1$ 贡献 0；各项 $1\cdot127,\,2\cdot31,\,3\cdot11,\,4\cdot9,\,5\cdot5,\,6\cdot3,\,7\cdot1,\,8,\,9,\,10$，累加 $=127{+}62{+}33{+}36{+}25{+}18{+}7{+}8{+}9{+}10=335$。
- 独立第二公式（引理 4）：$f(20)=1\cdot255+2\cdot63+3\cdot23+4\cdot19+5\cdot11+6\cdot7+7\cdot3+8\cdot3+9\cdot3+10\cdot3+155\cdot1=880$，$T=(880-\frac{20\cdot21}{2})/2=(880-210)/2=335$ ✓ 同值。
- 第三法（按第二分量逐行）：$T(20)=\sum_{k=1}^{20}g(k)-210$，$g=\varphi*\mathrm{id}$（§4.3(6)）给出 $\sum g(k)=545$，$545-210=335$ ✓ 三法一致。

**(2) 边界最小值 $n=2$**：$T=1$（只有一个对 $(1,2)$）。代码路径：`s[1]=1`，筛 $i=2$（未标记→`pri[0]=2`,`s[2]=1`；内层 $2\le n/2=1$ 不成立，不标记），前缀和 `s[1]=1,s[2]=2`；`calc`: $d=1$ 加 $1\cdot(s[2]-1)=1$，$d=2$ 加 $2\cdot(s[1]-1)=0$ → `ans=1` ✓。`calc2`：块 $[1,1]$（$q=2$）得 $\frac{(1+1)\cdot1}{2}\times(\Phi(2)-1)=1\times1=1$；块 $[2,2]$（$q=1$）得 $\frac{(2+2)\cdot1}{2}\times0=0$ → 合计 1 ✓。

**(3) 边界 $n=3$ 与 $n=1$（题面 $n\ge2$，仅作鲁棒性检查）**：$n=3$：$d=1\to\Phi(3)-1=3$，$d=2,3\to q=1$ 贡献 0 → 3，与暴力一致 ✓。$n=1$（不合法输入）：`ans=0`（空和，$d=1$ 项 $\Phi(1)-1=0$），与"$\binom12=0$ 个对"一致，说明 $d>n/2$ 自动零贡献是结构性的、不是巧合。

**(4) 自造例 $n=8$**：$\Phi(8)=22,\Phi(4)=6,\Phi(2)=2$。$d=1\to1\cdot21$；$d=2\to2\cdot5=10$；$d=3,4\to q=2$、$3+4=7$；$d\ge5\to0$。总 $=21+10+7=38$。逐行暴力核对：$T(7)=26$，第 8 行的对 $\gcd(1..7,8)=1,2,1,4,1,2,1$ 和为 12，$26+12=38$ ✓（$T(7)=26$ 由 $T(6)=20$ 加第 7 行 $1,1,1,1,1,1$ 共 6 得）。

**(5) 分块版 `calc2` 在 $n=12$ 上逐步跟踪**（验证引理 10 的块结构与等差块和）：
$\Phi(12)=46,\Phi(6)=12,\Phi(4)=6,\Phi(3)=4,\Phi(2)=2,\Phi(1)=1$。
块：$[1,1]$($q{=}12$)、$[2,2]$($q{=}6$)、$[3,3]$($q{=}4$)、$[4,4]$($q{=}3$)、$[5,6]$($q{=}2$)、$[7,12]$($q{=}1$)。
块值：$1\cdot45=45$；$2\cdot11=22$；$3\cdot5=15$；$4\cdot3=12$；$\frac{(5+6)\cdot2}{2}\cdot1=11$；$\frac{(7+12)\cdot6}{2}\cdot0=0$。合计 $105$。
朴素版逐项 $d(\Phi(\lfloor12/d\rfloor)-1)$：$45,22,15,12,5,6,0\ldots$ 合计 $105$ ✓ 两版一致。
再用第三法（§4.3(6) 的逐行差分）复核：$\sum_{k=1}^{12}g(k)=1+3+5+8+9+15+13+20+21+27+21+40$，逐项累计 $1,4,9,17,26,41,54,74,95,122,143,183$，故 $\sum g(k)=183$；$T=183-\frac{12\cdot13}{2}=183-78=105$ ✓ 三法一致。

**(6) 第三条独立路线（差分递推，用来做全量互验的思路依据）** 记 $g(k)=\sum_{i=1}^{k}\gcd(i,k)=\sum_{d\mid k}d\,\varphi(k/d)=(\mathrm{id}*\varphi)(k)$，则 $T(n)=\sum_{k=2}^{n}\left(g(k)-k\right)$。
**证明**：把第 $k$ 行按 $d=\gcd(i,k)$ 分层。设 $b=k/d$，则 $\gcd(i,k)=d\iff d\mid k$ 且 $i=da$、$1\le a\le b$、$\gcd(a,b)=1$（必要性：$d\mid k$，写 $i=da$，若 $\gcd(a,b)=t>1$ 则 $dt\mid i,dt\mid k$ 与 $\gcd(i,k)=d$ 矛盾，且 $1\le i\le k\Rightarrow a\le b$；充分性：$\gcd(da,db)=d\gcd(a,b)=d$，逐素数比较指数同引理 2）。故固定 $d$ 时 $a$ 的个数是 $\varphi(k/d)$；这里 $\varphi$ 的定义取"$1\le a\le b$"（含端点），与"$1\le i\le k$"（含 $i=k$）完全对齐：端点 $a=b$ 被计入当且仅当 $\gcd(b,b)=b=1$，即 $b=1$（也就是 $d=k$、$i=k$ 那一项，其 $\gcd$ 恰为 $k$），所以**不需要**引理 3 那样的 $-1$ 修正。对 $d\mid k$ 求和得 $g(k)=\sum_{d\mid k}d\,\varphi(k/d)$。最后第 $k$ 行在 $T$ 里只取 $i<k$，去掉对角 $\gcd(k,k)=k$，逐行相加即 $T(n)=\sum_{k=2}^{n}(g(k)-k)$（$k=1$ 时 $g(1)-1=0$，写 $\sum_{k=1}^{n}$ 也一样）。$\blacksquare$
数值（素数幂用 $g(p^{a})=p^{a}+a\,p^{a-1}(p-1)$——即引理 5 的 $\varphi$ 公式代回卷积，再由引理 6 的积性拼接）：$g(1..20)=1,3,5,8,9,15,13,20,21,27,21,40,25,39,45,48,33,63,37,72$，累计 545（§4.3(1) 已用），$n=6$ 时累计 41、$T(6)=41-21=20$ ✓ 与表 (0) 一致。

**(7) 引理 3 的独立复核（$C$ 直接数、不走 $\Phi$）**：$C(5)=\Phi(5)-1=9$。直接数 $1\le a<b\le5$ 且互素：$b=2$:(1)1；$b=3$:(1,2)2；$b=4$:(1,3)2；$b=5$:(1,2,3,4)4 → $1+2+2+4=9$ ✓。$C(6)=\Phi(6)-1=11$，再加 $b=6$ 行的 $(1,5)$ 2 个 → $9+2=11$ ✓。

**(8) 引理 9（容斥式）复核**：$m=4$：$1\cdot16-1\cdot4-1\cdot1+0\cdot1=11=2\Phi(4)-1$ ✓；$m=6$：$36-9-4-1+1=23=2\Phi(6)-1$ ✓。与主式同值，故 §2.7 的"慢一个数量级"是纯性能取舍。

**(9) `ini()` 线性筛的逐步跟踪（$n=10$，验证代码路径而非只验公式）**：记每一步写入的 `s[x]`（此时仍是 $\varphi$）。

| $i$ | 是否被 `vis` 标记 | 素数分支 | 内层 $j$：$p=\texttt{pri}[j]$，条件 $p\le n/i$ | 写入 |
|---|---|---|---|---|
| 2 | 否 → 新素数 | `pri[0]=2`,`s[2]=1` | $j{=}0$:$2\le5$ ✓，$2\%2{=}0$ → 标记 4，`s[4]=s[2]*2=2`，`break` | $\varphi(2){=}1,\varphi(4){=}2$ |
| 3 | 否 → 新素数 | `pri[1]=3`,`s[3]=2` | $j{=}0$:$2\le3$ ✓，$3\%2{\neq}0$ → 标记 6，`s[6]=2*1=2`；$j{=}1$:$3\le3$ ✓，$3\%3{=}0$ → 标记 9，`s[9]=2*3=6`，`break` | $\varphi(3){=}2,\varphi(6){=}2,\varphi(9){=}6$ |
| 4 | 是 | — | $j{=}0$:$2\le2$ ✓，$4\%2{=}0$ → 标记 8，`s[8]=s[4]*2=4`，`break` | $\varphi(8){=}4$ |
| 5 | 否 → 新素数 | `pri[2]=5`,`s[5]=4` | $j{=}0$:$2\le2$ ✓，$5\%2{\neq}0$ → 标记 10，`s[10]=4*1=4`；$j{=}1$:$3\le2$ ✗ 退出 | $\varphi(5){=}4,\varphi(10){=}4$ |
| 6 | 是 | — | $j{=}0$:$2\le1$ ✗ 退出（12 已超 $n$） | — |
| 7 | 否 → 新素数 | `pri[3]=7`,`s[7]=6` | $j{=}0$:$2\le1$ ✗ 退出 | $\varphi(7){=}6$ |
| 8,9,10 | 是 | — | $j{=}0$:$2\le1$ ✗ 退出 | — |

得到的 $\varphi(1..10)=1,1,2,2,4,2,6,4,6,4$ 与真值逐个相符（$\varphi(6)=2$：$1,5$；$\varphi(8)=4$：$1,3,5,7$；$\varphi(10)=4$：$1,3,7,9$ ✓）。注意三点：①`s[1]=1` 是手工给的，筛的过程碰不到它；②每个合数恰好被写一次（4,6,8,9,10 各一次），与引理 8 一致；③`break` 在 $i=2,3,4$ 都触发过，保证 12 之类的数不会被 $6\times2$ 与 $4\times3$ 双重生成。前缀和后 $s[1..10]=1,2,4,6,10,12,18,22,28,32$，与 §4.3(0)(5) 用的 $\Phi$ 表一致 ✓。

**(10) 时间/空间自检算式**：基本操作 $\le1.0\times10^{7}$、除法 $\le4\times10^{6}$、内存 $\approx26$ MB（§4.2）。本环境禁止编译/运行，**未做任何实测耗时、未记录任何得分**。

### 4.4 不确定点

- **无官方样例可对标**（题面文件不含样例输入输出）。本文用"定义暴力全枚举 $n\le6$ + $n=8,12,20$ 三法互验（主式 / 对称式 / 差分式）+ 边界 $n=2,3$ + 分块版逐步跟踪"支撑结论，全部一致。**未做对拍、未记录得分或实测耗时**。
- 题面标签含"莫比乌斯反演/整除分块/O2优化"，是外部提示；题面本身只要求那个求和式。本文证明这些路线与主式同值（引理 9、引理 10），并说明本题单询问、$n\le2\times10^{6}$ 时 $O(n)$ 线性筛 + $O(n)$ 累加已足够，**分块不影响总复杂度**，仅作为互验写法保留。若某评测版本改成"多组询问"或 $n$ 更大（如 $10^{10}$），则必须用引理 10 的分块 + 杜教筛求 $\Phi$，本文未涉及后者。
- $\varphi(1)=1$ 与 $C(m)=\Phi(m)-1$ 的 $-1$ 是最容易写错的一处（漏掉则每项多算 $d$，总量 $\frac{n(n+1)}{2}$ 的偏移），§4.3(2)(3) 的 $n=1,2$ 边界检查专门盯这一处；主代码里体现为 `s[n/d]-1`。
- `s[]` 一物两用（先存 $\varphi$ 再原地前缀和）依赖"前缀和阶段只往后读 `s[i-1]`、且 `s[0]=0`"这一条不变量，代码注释已标注。

---

## 5. 示例代码（C++）

```cpp
// P1390 公约数的和
// 求 sum_{1<=i<j<=n} gcd(i,j) = sum_{d=1}^{n} d * (Phi(n/d) - 1)
// https://www.luogu.com.cn/problem/P1390
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2000005};    // n<=2e6，+5 松弛

int n;
i64 s[mxn];                    // 筛的过程中存 phi(i)，之后原地前缀和成 Phi(i)；s[0]=0 是哨兵
int pri[mxn],pcnt;
bool vis[mxn];
i64 ans;

void ini()
{
	s[1]=1;                    // phi(1)=1，既不是质数也不是合数，单独给
	for(int i=2;i<=n;++i)
	{
		if(!vis[i])
		{
			pri[pcnt++]=i;
			s[i]=i-1;          // phi(p)=p-1
		}
		for(int j=0;j<pcnt&&pri[j]<=n/i;++j)   // 用除法判界，绝不让 i*pri[j] 越界
		{
			vis[i*pri[j]]=true;
			if(i%pri[j]==0)
			{
				s[i*pri[j]]=s[i]*pri[j];        // p|i => phi(ip)=p*phi(i)
				break;                            // 只由最小质因子生成一次
			}
			else
				s[i*pri[j]]=s[i]*(pri[j]-1);    // (i,p)=1 => phi(ip)=phi(i)*(p-1)
		}
	}
	for(int i=1;i<=n;++i)
		s[i]+=s[i-1];          // 原地转成 Phi，省掉第二个数组
}

i64 calc()                     // 主累加，O(n)
{
	i64 res=0;
	for(int d=1;d<=n;++d)
		res+=(i64)d*(s[n/d]-1);    // -1 是关键：phi(1)=1 那行没有 a<b
	return res;
}

i64 calc2()                    // 整除分块版，与 calc 同值（互验用）
{
	i64 res=0;
	for(int l=1,r,q;l<=n;l=r+1)
	{
		q=n/l;r=n/q;
		res+=(i64)(l+r)*(r-l+1)/2*(s[q]-1);    // 块内 d 的等差和 * (Phi(q)-1)
	}
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	assert(2<=n&&n<mxn);
	ini();
	ans=calc();
	assert(ans==calc2());      // 朴素累加与分块累加必须逐位相同
	// cerr<<ans<<' '<<calc2()<<'\n';
	cout<<ans<<'\n';
	return 0;
}
```

### 代码说明

- **`s[]` 两段语义**：`ini()` 前半段是 $\varphi$、末段 `s[i]+=s[i-1]` 之后是 $\Phi$。全局静态数组零初始化天然给出 `s[0]=0`，前缀和循环从 1 开始即可，无需 `fill_n`（本解法没有需要 INF 的数组）。
- **`-1` 与引理 3 一一对应**，漏写就会得到 $\sum_d d\,\Phi(\lfloor n/d\rfloor)$，比正解多 $\frac{n(n+1)}{2}$（即对角线之和）——正是引理 4 里的 $f(n)$ 与 $2T(n)+\text{diag}$ 的关系，可用来自检。
- **`pri[j]<=n/i`**：把"乘了会不会越界"改成除法判定，避免 `i*pri[j]` 在判定时先溢出；数组下标 `i*pri[j]` 只在 $\le n$ 时求值。
- **`i64` 一致性**：$\Phi(2\times10^6)\approx1.2\times10^{12}$、答案 $\lesssim3.3\times10^{13}$（§2.9），全程 `i64`；`(i64)d*(...)` 与 `(i64)(l+r)*(r-l+1)` 两处显式提升，防止 `int` 中间量溢出。
- **`calc2` 只在 `assert` 里跑**：多花 $O(\sqrt n)$ 时间买一个独立实现的交叉校验，不改变总复杂度。
- 1-based 数论数组（§4.1 说明），`'\n'`、Tab 缩进、运算符无空格、小写电报式函数名 `ini/calc/calc2` 均按规范。
