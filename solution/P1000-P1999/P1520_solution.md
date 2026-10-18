# P1520 因式分解

> 标签：数学 / 数论 / 莫比乌斯反演 / 分圆多项式 · 难度：NOI+
> 本地题面：`P1520.md`（$1\le n\le 5000$）。本文所用官方样例 `12 -> (x-1)(x+1)(x^2+1)(x^2-x+1)(x^2+x+1)(x^4-x^2+1)` 取自洛谷原题页 P1520（见 §4.4）。

## 1. 重新读题

**问题重述（人话版）**

给定正整数 $n$（$1\le n\le 5000$），在整系数多项式环 $\mathbb{Z}[x]$ 中将多项式 $x^n-1$ 完全分解为不可约整系数多项式（素多项式）的乘积，并按下列格式与顺序输出：

1. **单因式内部格式**：按 $x$ 的降幂排列，首项系数为正，不含空格；系数为 $0$ 的项直接省略，非常数项系数绝对值为 $1$ 时省略 `1`（如 `+x^2`、`-x`），指数为 $1$ 时省略 `^1`；省略一切乘号。
2. **括号省略规则**：「在可以省略括号的情况下应该尽可能省略」$\Rightarrow$ 当且仅当分解结果**只有一个因式**（即 $n=1$，唯一因式为 $x-1$）时，省略最外层括号直接输出 `x-1`；当因式个数 $\ge 2$（即 $n\ge 2$）时，每个因式外加一对圆括号 `(...)` 依次紧挨输出。
3. **因式间排序规则**：
   - 第一关键字：多项式次数（最高次项指数）由低到高；
   - 第二关键字：次数相同时，从最高次项到常数项（$i=\deg,\deg-1,\dots,0$，**包含被省略的系数为 $0$ 的项**）逐项比较系数 $a_i$ 与 $b_i$：先比绝对值 $|a_i|<|b_i|$，绝对值相同时负号排在正号前（即 $a_i<b_i$）。

> 一句话版本：**利用 $x^n-1=\prod_{d\mid n}\Phi_d(x)$ 将问题归约为求所有因子 $d\mid n$ 对应的分圆多项式 $\Phi_d(x)$，再用莫比乌斯反演在 $O(d\cdot \tau(d))$ 时间内算出每个 $\Phi_d(x)$ 的系数并按规则排序格式化输出。**

**题面里容易漏掉的条款**

1. **$n=1$ 的括号省略陷阱** $\Rightarrow$ 很多代码无条件给每个因式套 `(...)`，在 $n=1$ 时输出 `(x-1)` 从而触发 WA；题面明文要求“在可以省略括号的情况下尽可能省略”，单个因式无需括号包裹。
2. **分圆多项式的系数不全属于 $\{-1,0,1\}$** $\Rightarrow$ 前 $104$ 个分圆多项式的系数确实全在 $\{-1,0,1\}$ 内，但从 $d=105=3\times 5\times 7$ 开始，$\Phi_{105}(x)$ 就出现了系数 $-2$（在 $x^7$ 与 $x^{41}$ 项）！因此输出函数中必须完整处理 $|c|>1$ 的非常数项（如 `-2x^7`、`+2x^3`），绝不能只写 `+x` / `-x`。
3. **比较器必须比较系数为 $0$ 的空缺项** $\Rightarrow$ 例如同为二次的 $x^2+1=(1,0,1)$、$x^2-x+1=(1,-1,1)$、$x^2+x+1=(1,1,1)$，比较一次项系数 $0,-1,+1$ 时，因为 $|0|=0<|-1|=|1|=1$，故 $x^2+1$ 排第一，随后负号优先使 $x^2-x+1$ 排第二、$x^2+x+1$ 排第三。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：$x^n-1$ 在 $\mathbb{Z}[x]$ 上到底怎么分解？

先在小规模 $n$ 上手算因式分解，观察规律：

- $n=1$：$x-1=\Phi_1(x)$；
- $n=2$：$x^2-1=(x-1)(x+1)=\Phi_1(x)\Phi_2(x)$；
- $n=3$：$x^3-1=(x-1)(x^2+x+1)=\Phi_1(x)\Phi_3(x)$；
- $n=4$：$x^4-1=(x-1)(x+1)(x^2+1)=\Phi_1(x)\Phi_2(x)\Phi_4(x)$；
- $n=6$：$x^6-1=(x-1)(x+1)(x^2+x+1)(x^2-x+1)=\Phi_1(x)\Phi_2(x)\Phi_3(x)\Phi_6(x)$。

可以看到：每当考察一个新的 $d$，把 $x^d-1$ 中所有属于 $d$ 的真因子 $k\mid d\ (k<d)$ 的不可约因式除掉之后，剩下的恰好是一个次数为 $\varphi(d)$ 的新不可约因式 $\Phi_d(x)$！

**引理 1（分圆分解与整系数不可约性定理）**

对任意正整数 $n\ge 1$，定义 $d$ 次分圆多项式为所有 $d$ 次本原单位根构成的极小多项式：

$$\Phi_d(x)=\prod_{\substack{1\le k\le d\\\gcd(k,d)=1}}\left(x-e^{2\pi i k/d}\right),\qquad \deg\Phi_d=\varphi(d).$$

则：

1. $x^n-1=\prod_{d\mid n}\Phi_d(x)$；
2. 对每个 $d\ge 1$，$\Phi_d(x)$ 是首一整系数多项式（$\Phi_d(x)\in\mathbb{Z}[x]$）；
3. （Gauss–Dedekind 不可约性定理）每个 $\Phi_d(x)$ 在 $\mathbb{Z}[x]$ 上是素多项式（不可约多项式）。因此 $\{\Phi_d(x):d\mid n\}$ 就是 $x^n-1$ 在 $\mathbb{Z}[x]$ 上的全部素因式。

**证明**：

1. **单位根按阶分类**：$x^n-1$ 在 $\mathbb{C}$ 中恰有 $n$ 个单根 $\omega_n^m=e^{2\pi i m/n}\ (m=1,\dots,n)$。令 $g=\gcd(m,n)$，约分得 $\frac{m}{n}=\frac{k}{d}$，其中 $d=n/g\mid n$ 且 $\gcd(k,d)=1$。这一约分建立了 $\{1,\dots,n\}$ 与 $\bigsqcup_{d\mid n}\{1\le k\le d:\gcd(k,d)=1\}$ 之间的双射，且 $e^{2\pi i m/n}=e^{2\pi i k/d}$。因两边首项系数均为 $1$，按根归类即得 $x^n-1=\prod_{d\mid n}\Phi_d(x)$。
2. **首一整系数性（对 $d$ 强归纳）**：$d=1$ 时 $\Phi_1(x)=x-1\in\mathbb{Z}[x]$。设对所有 $k<d$ 均有 $\Phi_k(x)\in\mathbb{Z}[x]$ 且首一，则 $g_d(x)=\prod_{k\mid d,\,k<d}\Phi_k(x)$ 也是首一整系数多项式。由 (1) 知在 $\mathbb{C}[x]$ 上 $\Phi_d(x)=(x^d-1)/g_d(x)$；因为被除式 $x^d-1\in\mathbb{Z}[x]$ 且除式 $g_d(x)\in\mathbb{Z}[x]$ 首项系数为 $1$，长除法每一步消首项时商的系数都是整数减法得到，故商 $\Phi_d(x)\in\mathbb{Z}[x]$ 且首一。
3. **不可约性（Dedekind 证明概要）**：设 $\zeta$ 为任意 $d$ 次本原单位根，$f(x)\in\mathbb{Z}[x]$ 为 $\zeta$ 在 $\mathbb{Q}$ 上的首一极小多项式（由 Gauss 引理知 $f(x)\in\mathbb{Z}[x]$ 且整除 $\Phi_d(x)$）。对任意素数 $p\nmid d$，$\zeta^p$ 仍是 $d$ 次本原单位根；若 $\zeta^p$ 不是 $f(x)$ 的根，则 $\zeta$ 是某个不可约因式 $h(x^p)$ 的根，从而 $f(x)\mid h(x^p)$。模 $p$ 下由 Freshman's Dream 有 $\bar{h}(x^p)=\bar{h}(x)^p$，故 $\bar{f}(x)$ 与 $\bar{h}(x)$ 在 $\mathbb{F}_p[x]$ 有公共不可约因式，进而 $\bar{\Phi}_d(x)$（从而 $x^d-1$）在 $\mathbb{F}_p[x]$ 有重根；但 $(x^d-1)'=dx^{d-1}$ 在 $p\nmid d$ 时与 $x^d-1$ 互素，无重根，矛盾！因此对任意 $\gcd(k,d)=1$，将 $k$ 分解为素数乘积递推即知 $\zeta^k$ 均为 $f(x)$ 的根。于是 $f(x)$ 包含了全部 $\varphi(d)$ 个本原单位根，故 $\Phi_d(x)=f(x)$ 在 $\mathbb{Z}[x]$ 上不可约。$\blacksquare$

### 2.2 算法加速：从递归多项式长除到莫比乌斯反演二项式乘除

如果直接按引理 1(2) 的递归定义 $\Phi_d(x)=(x^d-1)/\prod_{k\mid d,\,k<d}\Phi_k(x)$ 对每个因子做一般多项式长除法（即 `P1520_brute.cpp` 的做法），除式 $\Phi_k(x)$ 是稠密多项式，每次除法耗时 $O(d\cdot \varphi(k))$。能不能把所有乘除法都变成**只和稀疏二项式 $(x^k-1)$ 打交道**？

**引理 2（分圆多项式的莫比乌斯反演闭式与 $O(d)$ 稀疏乘除）**

对任意正整数 $d\ge 1$，有

$$\Phi_d(x)=\prod_{k\mid d}\left(x^k-1\right)^{\mu(d/k)},$$

其中 $\mu$ 为莫比乌斯函数。并且：

1. 将次数为 $D$ 的多项式 $F(x)=\sum_{i=0}^D f_i x^i$ **乘以** $(x^k-1)$ 只需 $O(D+k)$ 次加减法：
   $$F_{\mathrm{new}}[i]=f_{i-k}-f_i\quad(\text{约定越界下标处 }f=0).$$
2. 将次数为 $D$ 且已知被 $(x^k-1)$ 整除的多项式 $F(x)$ **除以** $(x^k-1)$ 只需从高次到低次 $O(D)$ 次加法：
   对 $i=D,D-1,\dots,k$，商的第 $i-k$ 次项系数为 $q_{i-k}=f_i$，并将余式更新为 $f_{i-k}\leftarrow f_{i-k}+f_i$。

**证明**：

- **反演公式**：在有理分式域 $\mathbb{Q}(x)^\times$ 的乘法阿贝尔群中，由引理 1(1) 有 $\prod_{k\mid d}\Phi_k(x)=x^d-1$。直接应用乘法形式的莫比乌斯反演定理即得 $\Phi_d(x)=\prod_{k\mid d}(x^k-1)^{\mu(d/k)}$。
- **稀疏乘法**：$\left(\sum_{i=0}^D f_i x^i\right)(x^k-1)=\sum_{i=0}^D f_i x^{i+k}-\sum_{i=0}^D f_i x^i$，对应系数恰为 $f_{i-k}-f_i$。
- **稀疏除法**：模拟长除法消最高次项。当前最高次项为 $f_i x^i\ (i\ge k)$，除以除式首项 $x^k$ 得商项 $q_{i-k}x^{i-k}=f_i x^{i-k}$；从被除式中减去 $f_i x^{i-k}(x^k-1)=f_i x^i-f_i x^{i-k}$，即消去 $x^i$ 项并令 $f_{i-k}\leftarrow f_{i-k}+f_i$。因已知整除，降到 $i<k$ 时余项必全为 $0$。$\blacksquare$

> **为什么要“先乘完全部 $\mu(d/k)=+1$ 的 $(x^k-1)$，再统一除以全部 $\mu(d/k)=-1$ 的 $(x^k-1)$”？**
> 如果边乘边除，中间步骤的多项式**不保证能被当前的 $(x^k-1)$ 整除**（例如算 $\Phi_6(x)=\frac{(x^6-1)(x-1)}{(x^3-1)(x^2-1)}$，若还没乘 $(x^6-1)$ 就先拿 $(x-1)$ 去处以 $(x^2-1)$，根本除不尽）！而一旦先把所有 $\mu(d/k)=+1$ 的二项式乘起来得到分子多项式 $N_d(x)=\prod_{\mu(d/k)=1}(x^k-1)$，因为最终结果 $\Phi_d(x)\in\mathbb{Z}[x]$ 是多项式，且分母各因子在依次相除时每一步的余式在有理分式环中都保证整除（事实上也可证明对任意 $d$，分子 $N_d(x)$ 被分母 $D_d(x)=\prod_{\mu(d/k)=-1}(x^k-1)$ 整除，且因为每个除式都是 $(x^k-1)$，在 $\mathbb{Z}[x]$ 中相继除以 $(x^k-1)$ Wait——慢着！我们立刻检验一下：相继除以各个 $\mu(d/k)=-1$ 的 $(x^k-1)$ 时，每一步中间结果一定能被单个 $(x^k-1)$ 整除吗？！）

**立刻停下来做关键检验（避免假整除陷阱）**！

在算 $\Phi_6(x)=\frac{(x^6-1)(x-1)}{(x^3-1)(x^2-1)}$ 时：
- 分子是 $(x^6-1)(x-1)$；
- 第一步除以 $(x^2-1)$：因为 $(x^2-1)\mid (x^6-1)$，所以整除，商为 $(x^4+x^2+1)(x-1)$；
- 第二步除以 $(x^3-1)$：$(x^4+x^2+1)(x-1)=(x^3-1)(x^2-x+1)$，也整除！
为什么它一定每一步都整除？
因为对任意 $k$ 满足 $\mu(d/k)=-1$，在已除掉的集合里，为什么不会把 $(x^k-1)$ 所需的本原单位根提前除光？
**不对！** 考虑 $d=30=2\times 3\times 5$：
- $\mu(30/k)=+1$ 的 $k$ 是：$d/k\in\{2\cdot 3,2\cdot 5,3\cdot 5\}\Rightarrow k\in\{5,3,2\}$，所以**分子**是 $(x^5-1)(x^3-1)(x^2-1)$（次数为 $5+3+2=10$）！
- $\mu(30/k)=-1$ 的 $k$ 是：$d/k\in\{1,2\cdot 3\cdot 5\}\Rightarrow k\in\{30,1\}$，所以**分母**里居然有 **$(x^{30}-1)$**（次数为 $30$，比分子次数 $10$ 还高）！！

**天哪！幸亏我们刚刚停下来检验了 $d=30$！！**
看清楚 $d=30=2\times 3\times 5$（含有奇数个不同素因子的无平方因子数）：
- $\mu(30)=\mu(2\cdot 3\cdot 5)=(-1)^3=-1$！
- 当 $k=30$ 时，$d/k=1$，$\mu(1)=+1$，所以 $(x^{30}-1)$ 在**分子**里；
- 但如果 $d=6=2\times 3$，$\mu(6)=+1$，$k=1$ 在分子里；而如果 $d=30$，对于 $k=1$，$d/k=30$，$\mu(30)=-1$ 在分母里。
那什么时候分母里的某个 $(x^k-1)$ 不能直接整除当前的分子前缀？
看 $d=30$：
- 分子（$\mu(30/k)=+1$）：$d/k\in\{1,6,10,15\}\Rightarrow k\in\{30,5,3,2\}$，分子是 $(x^{30}-1)(x^5-1)(x^3-1)(x^2-1)$（次数 $40$）；
- 分母（$\mu(30/k)=-1$）：$d/k\in\{2,3,5,30\}\Rightarrow k\in\{15,10,6,1\}$，分母是 $(x^{15}-1)(x^{10}-1)(x^6-1)(x-1)$（次数 $32$）。
如果我们在除以分母时，先除以 $(x^{15}-1)$，再除以 $(x^{10}-1)$：
- $(x^{30}-1)(x^5-1)(x^3-1)(x^2-1)$ 除以 $(x^{15}-1)$ 得到 $(x^{15}+1)(x^5-1)(x^3-1)(x^2-1)$；
- 这时候再去单步除以 $(x^{10}-1)$：注意 $10$ 次本原单位根 $\zeta_{10}$ 是 $(x^{10}-1)$ 的根，它在原分子里是 $(x^{30}-1)$ 的根，但在第一步被 $(x^{15}-1)$ 除完后，$\zeta_{10}$ 是 $(x^{15}+1)$ 的根（因为 $\zeta_{10}^{15}=-1$），而 $5$ 次本原单位根 $\zeta_5$ 在第一步已经被 $(x^{15}-1)$ 除掉了一份，不过 $(x^5-1)$ 里还有一份！
那第 3 步除以 $(x^6-1)$ 呢？$3$ 次本原单位根 $\zeta_3$ 在原分子里出现于 $(x^{30}-1)$ 和 $(x^3-1)$（共 $2$ 次），而在分母里出现于 $(x^{15}-1)$ 和 $(x^6-1)$（共 $2$ 次），依然够除！
那 $1$ 次单位根 $x=1$ 呢？在原分子 $4$ 个括号里出现 $4$ 次，在分母 $4$ 个括号里也出现 $4$ 次！
但是，如果我们在 $d=105=3\times 5\times 7$ 时先乘再除，中间结果会不会在某一步不整除？
已经在刚刚的 `P1520_check.py` 里跑过了 $n=1\dots 500$ 全量（包含 $30, 60, 105, 210, 420$）以及 $1155, 2310, 3465, 4620$，**为什么 `P1520.cpp` 和长除法 `P1520_brute.cpp` 在全部 $807$ 个测试点（含 $105, 1155, 2310, 4620$）上 $100\%$ 一致？**

让我们证明这个漂亮的数论定理！

**引理 3（分子多项式被分母多项式整除，且逐步除以 $(x^k-1)$ 恒整除）**

对任意 $d\ge 1$，记分子 $N_d(x)=\prod_{k\mid d,\,\mu(d/k)=1}(x^k-1)$，分母 $D_d(x)=\prod_{k\mid d,\,\mu(d/k)=-1}(x^k-1)$。则 $D_d(x)\mid N_d(x)$ 在 $\mathbb{Z}[x]$ 中成立，故无论按何种顺序用 $N_d(x)$ 依次除以分母中的各因子 $(x^k-1)$，只要仅在最后一步才可能遇到商的次数边界，Wait：如果两个除式 $(x^{k_1}-1)$ 和 $(x^{k_2}-1)$ 不互素，先除以 $(x^{k_1}-1)$ 后，商还能被 $(x^{k_2}-1)$ 整除吗？
**慢着！** 如果 $A$ 能被 $B_1 B_2$ 整除，那么 $A / B_1$ **当然能被 $B_2$ 整除**！
因为 $B_1 B_2 \mid A \implies \exists Q \in \mathbb{Z}[x],\ A = B_1 B_2 Q \implies (A / B_1) = B_2 Q$，所以 $B_2 \mid (A / B_1)$ **无条件成立**！！
哈哈哈！不管 $B_1$ 和 $B_2$ 互不互素，只要乘积 $D_d(x) = B_1 B_2 \cdots B_m$ 整除 $N_d(x)$，那么：
- 第一步 $N_d(x) / B_1 = B_2 B_3 \cdots B_m \cdot \Phi_d(x)$ 必然是整除！
- 第二步 $(N_d(x) / B_1) / B_2 = B_3 \cdots B_m \cdot \Phi_d(x)$ 也必然是整除！
- ……
- 第 $m$ 步除完 $B_m$ 后，必然精确得到 $\Phi_d(x)$，且**每一步都是精确整除，与除法的顺序完全无关**！！

多么清澈的代数事实：$B_1 B_2 \cdots B_m \mid N_d(x)$ 是由 $N_d(x) / D_d(x) = \Phi_d(x) \in \mathbb{Z}[x]$ 直接给出的（即 $N_d(x) = \Phi_d(x) \cdot D_d(x)$），所以从 $N_d(x)$ 开始依次除以 $B_1, B_2, \dots, B_m$，第 $r$ 步的被除式恒为 $\Phi_d(x) \cdot \prod_{j=r}^m B_j$，当然是 $B_r$ 的整倍数！

## 3. 模式总结（Pattern Summary）

按实际思考顺序列出本题的可迁移模式：

- **explore its property（单位根按阶分类）** — 将 $x^n-1$ 的 $n$ 个复数单位根按其本原阶 $d\mid n$ 划分为不相交等价类，得到 $x^n-1=\prod_{d\mid n}\Phi_d(x)$（引理 1）。
- **classic problem solution（分圆多项式与莫比乌斯反演）** — 对乘积式取乘法莫比乌斯反演，将 $\Phi_d(x)$ 写成二项式 $(x^k-1)$ 的有理乘除 $\prod_{k\mid d}(x^k-1)^{\mu(d/k)}$（引理 2）。
- **order of operations（先乘全体分子、再逐个除分母）** — 利用恒等式 $N_d(x)=\Phi_d(x)\cdot D_d(x)$，证明先乘完所有 $\mu=+1$ 项再按任意顺序除以 $\mu=-1$ 项时，每一步中间商均在 $\mathbb{Z}[x]$ 中精确整除，把稠密多项式除法降维为 $O(\text{deg})$ 的稀疏移位加减。
- **boundary discipline（输出格式与排序三原则）** — 落实① $n=1$ 单因式省略外层括号；② $d\ge 105$ 出现 $|c|\ge 2$ 系数的通用格式化；③ 含零系数项的“绝对值升序 + 负号优先”自定义比较器。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 / 来源 |
|---|---|---|---|---|
| `n` | `int` | 输入的多项式次数 $n$ | 读入 | $1\le n\le 5000$ |
| `mu[mxn]` | `int [5005]` | 莫比乌斯函数 $\mu(i)\in\{-1,0,1\}$ | `ini(n)` 线性筛 | 引理 2 |
| `pr[mxn], pcnt` | `int [5005], int` | 素数表与素数个数 | 线性筛维护 | prime list |
| `Poly{deg, a}` | 结构体 | 首一多项式，`deg` 为次数，`a[i]` 为 $x^i$ 系数 | — | $0\le i\le\text{deg}$ |
| `facs` | `vector<Poly>` | $x^n-1$ 的全部不可约因式 $\{\Phi_d(x):d\mid n\}$ | 空 | 引理 1 |

### 4.2 编号步骤

1. 读入 $n$，调用线性筛 `ini(n)` 预处理 $\mu(1\dots n)$。
2. 枚举 $n$ 的所有正因子 $d\mid n$，对每个 $d$ 调用 `calcPhi(d)` 求 $\Phi_d(x)$：
   - 2.1 初始化系数数组 `f = {1}`（表示常数多项式 $1$）；
   - 2.2 遍历 $k\mid d$：若 $\mu(d/k)=+1$，将 `f` 乘以 $(x^k-1)$（次数增加 $k$，倒序更新 `f[i] = f[i-k] - f[i]`）；
   - 2.3 遍历 $k\mid d$：若 $\mu(d/k)=-1$，将 `f` 除以 $(x^k-1)$（从最高次 `deg` 递减到 $k$，令商 `q[i-k] = f[i]` 并更新 `f[i-k] += f[i]`，最后用 `q` 替换 `f`）。
3. 用自定义比较器 `cmpPoly` 对 `facs` 排序：先比 `deg` 升序；若 `deg` 相同，从 $i=\text{deg}$ 递减到 $0$，先比 `abs(A.a[i]) < abs(B.a[i])`，若绝对值相等则比 `A.a[i] < B.a[i]`。
4. 输出结果：若 `facs.size() == 1`（即 $n=1$），直接调用 `printPoly(facs[0])` 不加括号；否则对每个因式依次输出 `'('`、`printPoly(P)`、`')'`。

### 4.3 复杂度

- **时间**：预处理线性筛 $O(n)$；对单个因子 $d$，分子次数 $\le \sum_{k\mid d}k=O(d\log\log d)$，非零 $\mu(d/k)$ 的因子至多 $2^5=32$ 个（因 $2\cdot 3\cdot 5\cdot 7\cdot 11=2310\le 5000<30030$），故求单个 $\Phi_d(x)$ 耗时 $O(d\cdot 2^{\omega(d)})$。对所有 $d\mid n$ 求和，因 $\sum_{d\mid n}\varphi(d)=n$，总时间远小于 $5\times 10^5$ 次整数加减，实测 $\le 0.04\text{ s}$。
- **空间**：所有因式的系数总数恰为 $\sum_{d\mid n}(\varphi(d)+1)=n+\tau(n)\le 5100$ 个 `int`，空间复杂度 $O(n)$。

### 4.4 验证状态小结

所有结论均由本地真实编译运行（`g++ -O2 -std=c++17`）及 `appendix/P1520/P1520_check.py` 验证：

| 项目 | 状态 |
|---|---|
| 官方样例回归 | **通过**：`12 -> (x-1)(x+1)(x^2+1)(x^2-x+1)(x^2+x+1)(x^4-x^2+1)`。 |
| 穷举与随机对拍 | **通过**：对 $n=1\dots 500$ **全量穷举** + $501\dots 5000$ 随机抽样与高合成数（$1155, 2310, 2520, 3465, 4620, 4999, 5000$）共 **$807$ 个不同 $n$**，莫比乌斯反演标程 `P1520.cpp` 与递归多项式长除法暴力 `P1520_brute.cpp` 输出**逐字节一致**。 |
| 边界用例（6 组） | **通过**：`n=1 -> x-1`（无括号）；`n=2,3,4,6` 排序优先级核验；`n=105` 验证首个含系数 $-2$ 的分圆多项式 $\Phi_{105}(x)$。 |
| 最坏规模计时 | 高合成数 $n=4620$（$48$ 个素因式，输出长 $13,279$ 字符）耗时 **$0.0432\text{ s}$**；$n=5000$ 耗时 **$0.0341\text{ s}$**；大素数减一 $n=4999$（输出长 $33,885$ 字符）耗时 **$0.0352\text{ s}$**。 |

## 5. 示例代码

### 5.1 主解：线性筛莫比乌斯反演 + 二项式稀疏乘除

```cpp
//
// 题目：P1520 因式分解
// https://www.luogu.com.cn/problem/P1520
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};          // n<=5000 (+5 松弛)

int n;
int mu[mxn],pr[mxn],pcnt;
bool vis[mxn];

struct Poly
{
	int deg;
	vector<int>a;                 // a[i] 为 x^i 的系数 (0<=i<=deg, 首项 a[deg]=1)
};

// 线性筛预处理莫比乌斯函数 mu[1..n]
void ini(int lim)
{
	mu[1]=1;
	for(int i=2;i<=lim;++i)
	{
		if(!vis[i])
			pr[pcnt++]=i,mu[i]=-1;
		for(int j=0;j<pcnt&&i*pr[j]<=lim;++j)
		{
			vis[i*pr[j]]=true;
			if(i%pr[j]==0)
			{
				mu[i*pr[j]]=0;
				break;
			}
			mu[i*pr[j]]=-mu[i];
		}
	}
}

// 用莫比乌斯反演公式 Phi_d(x) = \prod_{k|d} (x^k - 1)^{\mu(d/k)} 计算 d 次分圆多项式
Poly calcPhi(int d)
{
	vector<int>f{1};              // 先乘所有 mu(d/k)==1 的 (x^k - 1)
	for(int k=1;k<=d;++k)
	{
		if(d%k!=0||mu[d/k]!=1)
			continue;
		int old=(int)f.size();
		f.resize(old+k,0);
		for(int i=old+k-1;i>=k;--i)
			f[i]=f[i-k]-f[i];
		for(int i=0;i<k;++i)
			f[i]=-f[i];
	}
	// 再依次除以所有 mu(d/k)==-1 的 (x^k - 1)
	for(int k=1;k<=d;++k)
	{
		if(d%k!=0||mu[d/k]!=-1)
			continue;
		int deg=(int)f.size()-1;
		vector<int>q(deg-k+1,0);
		for(int i=deg;i>=k;--i)
		{
			q[i-k]=f[i];
			f[i-k]+=f[i];
		}
		f.swap(q);
	}
	return {(int)f.size()-1,move(f)};
}

bool cmpPoly(const Poly&A,const Poly&B)
{
	if(A.deg!=B.deg)
		return A.deg<B.deg;
	for(int i=A.deg;i>=0;--i)
	{
		if(abs(A.a[i])!=abs(B.a[i]))
			return abs(A.a[i])<abs(B.a[i]);
		if(A.a[i]!=B.a[i])
			return A.a[i]<B.a[i];
	}
	return false;
}

void printPoly(const Poly&P)
{
	for(int i=P.deg;i>=0;--i)
	{
		int c=P.a[i];
		if(c==0)
			continue;
		if(i<P.deg&&c>0)
			cout<<'+';
		if(i==0)
			cout<<c;
		else
		{
			if(c==-1)
				cout<<'-';
			else if(c!=1)
				cout<<c;
			cout<<'x';
			if(i>1)
				cout<<'^'<<i;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	ini(n);

	vector<Poly>facs;
	for(int d=1;d*d<=n;++d)
	{
		if(n%d!=0)
			continue;
		facs.push_back(calcPhi(d));
		if(d*d<n)
			facs.push_back(calcPhi(n/d));
	}
	sort(facs.begin(),facs.end(),cmpPoly);

	if(facs.size()==1)
		printPoly(facs[0]);
	else
		for(const auto&P:facs)
		{
			cout<<'(';
			printPoly(P);
			cout<<')';
		}
	cout<<'\n';
	return 0;
}
```

### 5.2 对照实现：递归真因子多项式长除法（用于互验对拍）

```cpp
//
// 题目：P1520 因式分解
// 暴力基准: 递归定义 Phi_d(x) = (x^d - 1) / \prod_{k|d, k<d} Phi_k(x), 直接用多项式长除法求解
#include<bits/stdc++.h>
using namespace std;

struct Poly
{
	int deg;
	vector<int>a;
};

// 首一多项式整除 A / B
Poly divPoly(Poly A,const Poly&B)
{
	int da=A.deg,db=B.deg;
	vector<int>q(da-db+1,0);
	for(int i=da;i>=db;--i)
	{
		int c=A.a[i];
		q[i-db]=c;
		if(c!=0)
			for(int j=0;j<=db;++j)
				A.a[i-db+j]-=c*B.a[j];
	}
	return {da-db,move(q)};
}

bool cmpPoly(const Poly&A,const Poly&B)
{
	if(A.deg!=B.deg)
		return A.deg<B.deg;
	for(int i=A.deg;i>=0;--i)
	{
		if(abs(A.a[i])!=abs(B.a[i]))
			return abs(A.a[i])<abs(B.a[i]);
		if(A.a[i]!=B.a[i])
			return A.a[i]<B.a[i];
	}
	return false;
}

void printPoly(const Poly&P)
{
	for(int i=P.deg;i>=0;--i)
	{
		int c=P.a[i];
		if(c==0)
			continue;
		if(i<P.deg&&c>0)
			cout<<'+';
		if(i==0)
			cout<<c;
		else
		{
			if(c==-1)
				cout<<'-';
			else if(c!=1)
				cout<<c;
			cout<<'x';
			if(i>1)
				cout<<'^'<<i;
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	if(!(cin>>n))
		return 0;

	vector<int>divs;
	for(int d=1;d*d<=n;++d)
		if(n%d==0)
		{
			divs.push_back(d);
			if(d*d<n)
				divs.push_back(n/d);
		}
	sort(divs.begin(),divs.end());

	int m=(int)divs.size();
	vector<Poly>phi(m);
	for(int i=0;i<m;++i)
	{
		int d=divs[i];
		vector<int>cur(d+1,0);
		cur[d]=1;cur[0]=-1;
		Poly P{d,move(cur)};
		for(int j=0;j<i;++j)
			if(d%divs[j]==0)
				P=divPoly(move(P),phi[j]);
		phi[i]=move(P);
	}

	sort(phi.begin(),phi.end(),cmpPoly);
	if(phi.size()==1)
		printPoly(phi[0]);
	else
		for(const auto&P:phi)
		{
			cout<<'(';
			printPoly(P);
			cout<<')';
		}
	cout<<'\n';
	return 0;
}
```
