# P2388 阶乘之乘 — 题解

## 一、解题过程：人类思考叙事

题目：求 $1!\times 2!\times 3!\times\cdots\times n!$ 的**末尾有几个零**，$n\le 10^8$。输出一个整数。题面没有样例，下面所有数值例子都是我自己算的。

**第一步：把"末尾零"翻译成数论语言。** 一个正整数末尾有 $c$ 个零，等价于 $10^c\mid N$ 而 $10^{c+1}\nmid N$。而 $10=2\times 5$，所以 $c=\min(v_2(N),v_5(N))$，其中 $v_p$ 表示质因子 $p$ 的指数。直觉上阶乘里因子 $2$ 远比因子 $5$ 多（每 $2$ 个数就有一个偶数，每 $5$ 个数才有一个 $5$ 的倍数），所以答案应该就是 $v_5(N)$。这个直觉后面要严格证（引理 2），不能默认。

**第二步：朴素做法撞墙。** 对每个 $k!$ 用 Legendre 公式 $v_5(k!)=\sum_{i\ge1}\lfloor k/5^i\rfloor$，再累加：

$$\text{ans}=\sum_{k=1}^{n}\sum_{i\ge1}\left\lfloor\frac{k}{5^i}\right\rfloor.$$

$n=10^8$，内层约 $\log_5 n\approx 11$ 项，总共 $\approx 1.1\times 10^9$ 次除法——太慢。那开个数组存 $v_5(k!)$ 的前缀和？$10^8$ 个 `long long` 是 $800$ MB，直接 MLE。**必须找闭式。**

**第三步：换一个视角看这个乘积。** 与其"逐个阶乘数因子"，不如问：**整数 $j$ 在整个乘积里出现了多少次？** $j$ 是 $k!$ 的因子当且仅当 $j\le k$，所以 $j$ 出现在 $j!, (j+1)!,\dots,n!$ 里，共 $n-j+1$ 次。于是

$$\prod_{k=1}^{n}k!=\prod_{j=1}^{n}j^{\,n-j+1},\qquad v_5=\sum_{j=1}^{n}(n-j+1)\,v_5(j).$$

这一步本身还是 $O(n\log n)$，但它把式子变成了"**一个权重乘一个 indicator 之和**"的形状——这种形状几乎总是可以交换求和次序。

**第四步：把 $v_5(j)$ 也拆成 indicator。** $v_5(j)$ 就是"有多少个 $5^i$ 整除 $j$"：$v_5(j)=\sum_{i\ge1}[5^i\mid j]$。代进去交换求和：

$$v_5=\sum_{i\ge1}\ \sum_{\substack{1\le j\le n\\ 5^i\mid j}}(n-j+1).$$

内层的 $j$ 就是 $q,2q,3q,\dots,aq$（$q=5^i$，$a=\lfloor n/q\rfloor$），而权重 $n-j+1$ 关于 $j$ 是**公差为 $-q$ 的等差数列**！等差数列求和是 $O(1)$ 的。外层 $i$ 只有 $\lfloor\log_5 n\rfloor=11$ 个。整个算法变成 $O(\log n)$。

写出来就是：

$$\text{ans}=\sum_{i\ge1,\ 5^i\le n}\left[a(n+1)-q\cdot\frac{a(a+1)}{2}\right],\quad q=5^i,\ a=\left\lfloor\frac nq\right\rfloor.$$

**第五步：踩坑与修正。**

- **坑一：值域。** 我一开始差点用 `int`。粗估答案量级：$\sum_i \frac{n^2}{2\cdot 5^i}\approx\frac{n^2}{8}=1.25\times 10^{15}$，远超 `int` 的 $2.1\times 10^9$。必须 `long long`。实际跑 $n=10^8$ 得 $1249999451562499$，与估算吻合。**注意本题不需要取模、也不需要逆元**：输出的是精确的零的个数，不是模意义下的值；真正的坑是 $64$ 位值域和中间乘积 $q\cdot a(a+1)\approx 2\times10^{15}$。
- **坑二：$/2$ 会不会丢精度？** $a(a+1)$ 是两个连续整数之积，必为偶数，所以 $q\cdot a(a+1)/2$ 是精确整除，写成整数除法没问题（引理 6）。
- **坑三：循环终止。** `for(i64 q=5;q<=n;q*=5)`：$n=10^8$ 时 $q$ 依次取 $5,25,\dots,48828125$，下一次 $244140625>n$ 退出。$q$ 用 `i64` 就不会在自乘时溢出。
- **坑四：$n<5$。** 循环一次都不进，答案 $0$。$n=1$ 时乘积是 $1!=1$，确实没有零 ✓。$n=0$（若数据允许）乘积为空积 $1$，同样输出 $0$，代码天然正确。

**第六步：手算校验。**

- $n=5$：$1\cdot2\cdot6\cdot24\cdot120=34560$，末尾 $1$ 个零。公式：$q=5,a=1$，$1\times6-5\times1\times2/2=6-5=1$ ✓。
- $n=10$：$q=5,a=2$，$2\times11-5\times2\times3/2=22-15=7$；$q=25>10$ 停。答案 $7$ ✓（$k=5..9$ 各贡献 $1$ 个 $5$，$k=10$ 贡献 $2$ 个）。
- $n=25$：$q=5,a=5$：$5\times26-5\times5\times6/2=130-75=55$；$q=25,a=1$：$1\times26-25=1$；合计 $56$ ✓。
- $n=100$：$q=5,a=20$：$2020-1050=970$；$q=25,a=4$：$404-250=154$；合计 $1124$ ✓。

**第七步：不放心，再找两条独立的路交叉验证。**

- **路二**：不重排乘积，直接对 $\sum_{k=1}^n\lfloor k/q\rfloor$ 按 $\lfloor k/q\rfloor$ 的取值分组：值 $0..a-1$ 各出现 $q$ 次、值 $a$ 出现 $b+1$ 次（$n=aq+b$），得 $q\frac{a(a-1)}2+a(b+1)$。这与路一的闭式**代数上恒等**（引理 7），但推导完全不同，可以用来查代数变形错误。
- **路三**：Legendre 的数字和形式 $v_5(k!)=\frac{k-s_5(k)}{4}$（$s_5$ 是五进制数位和），于是 $\text{ans}=\frac{1}{4}\left(\frac{n(n+1)}2-\sum_{k\le n}s_5(k)\right)$。这条式子长得跟前面完全不一样，是极强的独立校验（引理 8）。

三条路在小规模上必须给出同一个数——对拍结果见第四节。

---

## 二、思维链：完整证明

记 $N=\prod_{k=1}^{n}k!$，$v_p(x)$ 为质数 $p$ 在 $x$ 中的指数。

### 引理 1（末尾零的刻画）

正整数 $N$ 的十进制末尾零个数等于 $\min(v_2(N),v_5(N))$。

*证明*：由算术基本定理，$N=2^{a}5^{b}u$，其中 $a=v_2(N)$、$b=v_5(N)$、$\gcd(u,10)=1$。

$10^c\mid N\iff 2^c5^c\mid 2^a5^bu\iff c\le a$ 且 $c\le b$（因为 $2,5$ 是不同质数，且 $u$ 不含因子 $2,5$；由唯一分解，$2^c\mid N\iff c\le a$，$5^c\mid N\iff c\le b$）。故满足 $10^c\mid N$ 的最大整数 $c$ 是 $\min(a,b)$。

再说明"末尾恰有 $c$ 个零"等价于"$10^c\mid N$ 且 $10^{c+1}\nmid N$"：写 $N=10^c\cdot w$，$10\nmid w$。$w$ 的末位数字非零，故 $N$ 的十进制表示是 $w$ 的表示后接 $c$ 个零，末尾零个数恰为 $c$。取 $c=\min(a,b)$ 即得。证毕。

### 引理 2（本题中 $2$ 不比 $5$ 少）

$v_2(N)\ge v_5(N)$，因此答案 $=v_5(N)$。

*证明*：对每个固定的 $k$ 与每个 $i\ge1$，因 $2^i\le 5^i$，有 $\lfloor k/2^i\rfloor\ge\lfloor k/5^i\rfloor$（分母越小商越大，取整保持不等号）。由 Legendre 公式（引理 3 给出证明）$v_p(k!)=\sum_{i\ge1}\lfloor k/p^i\rfloor$，逐项比较得 $v_2(k!)\ge v_5(k!)$。再对 $k=1..n$ 求和，利用 $v_p(\prod x_i)=\sum v_p(x_i)$（唯一分解的直接推论），得 $v_2(N)\ge v_5(N)$。结合引理 1，$\min=v_5(N)$。证毕。

### 引理 3（Legendre 公式）

对质数 $p$ 与正整数 $k$，$v_p(k!)=\sum_{i\ge1}\left\lfloor\frac{k}{p^i}\right\rfloor$（有限和，$p^i>k$ 后各项为 $0$）。

*证明*：$v_p(k!)=\sum_{j=1}^{k}v_p(j)$。对每个 $j$，$v_p(j)=\#\{i\ge1:p^i\mid j\}$（若 $j=p^e u$，$p\nmid u$，则 $p^i\mid j\iff i\le e$，个数恰为 $e$）。于是

$$v_p(k!)=\sum_{j=1}^{k}\#\{i\ge1:p^i\mid j\}=\#\{(j,i):1\le j\le k,\ i\ge1,\ p^i\mid j\}.$$

按 $i$ 分组计数：对固定 $i$，$[1,k]$ 中 $p^i$ 的倍数有 $\lfloor k/p^i\rfloor$ 个。故上式 $=\sum_{i\ge1}\lfloor k/p^i\rfloor$。证毕。

### 引理 4（乘积重排）

$\displaystyle\prod_{k=1}^{n}k!=\prod_{j=1}^{n}j^{\,n-j+1}$，从而 $v_5(N)=\sum_{j=1}^{n}(n-j+1)v_5(j)$。

*证明*：把左边完全展开成整数因子之积：$\prod_{k=1}^n k!=\prod_{k=1}^n\prod_{j=1}^{k}j$。统计整数 $j$ 在这个双重积中出现的次数：它出现在所有满足 $k\ge j$ 的内层积里，即 $k=j,j+1,\dots,n$，共 $n-j+1$ 次（$1\le j\le n$）。由乘法交换律与结合律，把所有相同因子合并即得 $\prod_{j=1}^n j^{n-j+1}$。两边取 $v_5$，用 $v_5(x^e)=e\,v_5(x)$ 与 $v_5(\prod)=\sum v_5$ 即得右边的和式。证毕。

### 引理 5（指数的 indicator 展开与求和交换）

$\displaystyle v_5(N)=\sum_{i\ge1}\ \sum_{m=1}^{\lfloor n/5^i\rfloor}\bigl(n-m\cdot 5^i+1\bigr)$。

*证明*：由引理 4，$v_5(N)=\sum_{j=1}^n(n-j+1)v_5(j)$。由引理 3 证明中用到的等式 $v_5(j)=\#\{i\ge1:5^i\mid j\}=\sum_{i\ge1}[5^i\mid j]$，代入得

$$v_5(N)=\sum_{j=1}^{n}\sum_{i\ge1}(n-j+1)[5^i\mid j].$$

这是**非负项的有限双重和**（$i>\log_5 n$ 时 indicator 恒 $0$，$j$ 只有 $n$ 个），故可任意交换求和次序（有限和的交换律，无需分析条件）：

$$v_5(N)=\sum_{i\ge1}\sum_{j=1}^{n}(n-j+1)[5^i\mid j]=\sum_{i\ge1}\ \sum_{\substack{1\le j\le n\\5^i\mid j}}(n-j+1).$$

对固定 $i$，记 $q=5^i$。$[1,n]$ 中 $q$ 的倍数恰为 $q,2q,\dots,aq$，其中 $a=\lfloor n/q\rfloor$（因为 $mq\le n\iff m\le n/q\iff m\le\lfloor n/q\rfloor$）。令 $j=mq$ 即得结论。当 $q>n$ 时 $a=0$，内层和为空，贡献 $0$，所以外层只需枚举 $5^i\le n$。证毕。

### 引理 6（等差数列闭式与整除性）

设 $q\ge1$，$a=\lfloor n/q\rfloor$，则
$$\sum_{m=1}^{a}(n-mq+1)=a(n+1)-q\cdot\frac{a(a+1)}{2},$$
且 $q\cdot a(a+1)/2$ 是整数（整数除法精确）。

*证明*：把和式拆成三部分：
$$\sum_{m=1}^{a}(n+1)-q\sum_{m=1}^{a}m=a(n+1)-q\cdot\frac{a(a+1)}2,$$
其中用了自然数求和公式 $\sum_{m=1}^a m=\frac{a(a+1)}2$（该公式本身可由首尾配对归纳证明：$a$ 偶时配成 $a/2$ 对、每对和 $a+1$；$a$ 奇时配成 $(a-1)/2$ 对再加中间项 $(a+1)/2$，两种情形都等于 $a(a+1)/2$）。

整除性：$a$ 与 $a+1$ 是连续整数，必有一个偶数，故 $2\mid a(a+1)$，$\frac{a(a+1)}2\in\mathbb Z$，再乘整数 $q$ 仍是整数。因此在 C++ 中写 `q*a*(a+1)/2` 不会因截断而丢值——**但必须先乘后除**，若写成 `q*(a/2)*(a+1)` 则 $a$ 为奇数时出错。证毕。

### 引理 7（第二条推导：按 $\lfloor k/q\rfloor$ 分组，与引理 6 恒等）

$\displaystyle\sum_{k=1}^{n}\left\lfloor\frac kq\right\rfloor=a(n+1)-q\frac{a(a+1)}2$，其中 $a=\lfloor n/q\rfloor$；且若写 $n=aq+b$（$0\le b<q$），该值也等于 $q\frac{a(a-1)}2+a(b+1)$。

*证明*：先证第一个等号。用"按层计数"：对非负整数 $y$，$y=\sum_{j\ge1}[y\ge j]$。故
$$\sum_{k=1}^n\left\lfloor\frac kq\right\rfloor=\sum_{k=1}^n\sum_{j\ge1}\left[\left\lfloor\frac kq\right\rfloor\ge j\right]=\sum_{j\ge1}\#\{k\le n:\lfloor k/q\rfloor\ge j\}.$$
而 $\lfloor k/q\rfloor\ge j\iff k\ge jq$，故该计数为 $\max(0,n-jq+1)$；非零当且仅当 $j\le a$。于是和为 $\sum_{j=1}^{a}(n-jq+1)$，由引理 6 等于 $a(n+1)-q\frac{a(a+1)}2$。

再证第二个等号（直接分组）：$\lfloor k/q\rfloor$ 在 $k\in[1,n]$ 上的取值分布为——值 $0$ 出现 $q-1$ 次（$k=1..q-1$），值 $j$（$1\le j\le a-1$）出现 $q$ 次（$k=jq..jq+q-1$），值 $a$ 出现 $b+1$ 次（$k=aq..aq+b$）。故
$$\sum_{k=1}^n\left\lfloor\frac kq\right\rfloor=q\sum_{j=1}^{a-1}j+a(b+1)=q\frac{a(a-1)}2+a(b+1).$$
两式恒等可直接代数验证：把 $n=aq+b$ 代入第一式，
$$a(aq+b+1)-q\frac{a(a+1)}2=a^2q+ab+a-\frac{a^2q+aq}2=\frac{a^2q-aq}2+ab+a=q\frac{a(a-1)}2+a(b+1).\ \checkmark$$

**这条引理的意义**：由引理 3，$v_5(N)=\sum_{k=1}^n v_5(k!)=\sum_{i\ge1}\sum_{k=1}^n\lfloor k/5^i\rfloor$，与本引理结合得到**与引理 4~6 完全相同**的闭式，但推导路径不经过"乘积重排"。两条独立路径给出同一公式，是公式正确性的强证据。证毕。

### 引理 8（第三条推导：Legendre 的数字和形式）

$v_5(k!)=\dfrac{k-s_5(k)}{4}$，其中 $s_5(k)$ 是 $k$ 的五进制数位和。从而
$$v_5(N)=\frac14\left(\frac{n(n+1)}2-\sum_{k=1}^{n}s_5(k)\right).$$

*证明*：设 $k=\sum_{j\ge0}d_j5^j$（$0\le d_j\le4$）。则 $\lfloor k/5^i\rfloor=\sum_{j\ge i}d_j5^{j-i}$（丢掉低 $i$ 位）。由引理 3，
$$v_5(k!)=\sum_{i\ge1}\sum_{j\ge i}d_j5^{j-i}=\sum_{j\ge1}d_j\sum_{i=1}^{j}5^{j-i}=\sum_{j\ge1}d_j\cdot\frac{5^j-1}{4}=\frac{\sum_j d_j5^j-\sum_j d_j}{4}=\frac{k-s_5(k)}4,$$
其中用了等比数列求和 $\sum_{i=1}^{j}5^{j-i}=5^{j-1}+\dots+5^0=\frac{5^j-1}{4}$，以及 $j=0$ 项贡献 $0$。

对 $k=1..n$ 求和：$\sum_k v_5(k!)=\frac14\left(\sum_k k-\sum_k s_5(k)\right)=\frac14\left(\frac{n(n+1)}2-\sum_{k\le n}s_5(k)\right)$。证毕。

（这条式子要变成 $O(\log n)$ 算法还需对 $\sum s_5(k)$ 做数位 DP；本题不必实现它，但它是**形式完全不同的第三条独立校验**，对拍时用它逐 $k$ 暴力求数位和即可。）

### 引理 9（项数、非负性与溢出界）

设 $n\le 10^8$。则：
1. 外层循环至多 $\lfloor\log_5 n\rfloor\le 11$ 项；
2. 每项 $T_i=a(n+1)-q\frac{a(a+1)}2\ge0$；
3. 总和 $\le \frac{n(n+1)}4<2.51\times10^{15}$；
4. 中间量 $a(n+1)$ 与 $q\cdot a(a+1)$ 均 $<2.1\times10^{15}$。
故 `long long`（上界 $9.22\times10^{18}$）全程安全，而 `int`（上界 $2.1\times10^9$）必然溢出。

*证明*：
1. $5^{11}=48828125\le10^8<244140625=5^{12}$，故满足 $5^i\le n$ 的 $i$ 至多 $11$ 个。
2. 由引理 5，$T_i=\sum_{m=1}^{a}(n-mq+1)$，而 $mq\le aq\le n$ 故每项 $\ge1>0$，$T_i\ge0$。
3. 由 2 的表达式放缩：$T_i\le\sum_{m=1}^{a}(n+1)=a(n+1)\le\frac nq(n+1)$。求和：
$$\sum_i T_i\le n(n+1)\sum_{i\ge1}5^{-i}=n(n+1)\cdot\frac{1/5}{1-1/5}=\frac{n(n+1)}4.$$
代 $n=10^8$：$\frac{10^8(10^8+1)}4=2.500000025\times10^{15}<2.51\times10^{15}$。
4. $a\le n/q\le n/5=2\times10^7$，故 $a(n+1)\le2\times10^7\times(10^8+1)=2.00000002\times10^{15}$。又 $qa\le n$，故 $q\cdot a(a+1)=(qa)(a+1)\le n(a+1)\le10^8\times(2\times10^7+1)=2.0000001\times10^{15}$。两者都远小于 $9.22\times10^{18}$。

实测 $n=10^8$ 的答案为 $1249999451562499\approx1.25\times10^{15}$，落在界内。证毕。

### 引理 10（复杂度）

时间 $O(\log_5 n)$，空间 $O(1)$。

*证明*：循环次数为满足 $5^i\le n$ 的 $i$ 的个数 $=\lfloor\log_5 n\rfloor$，每次做常数次 $64$ 位乘除。只用 `n`、`ans`、`q`、`a` 四个标量，无数组。证毕。

### 定理（算法正确性）

程序输出的值等于 $N=\prod_{k=1}^n k!$ 的末尾零个数。

*证明*：引理 1 把末尾零化为 $\min(v_2,v_5)$；引理 2 证明该 $\min$ 就是 $v_5(N)$；引理 4、5、6 把 $v_5(N)$ 化为程序里那个 $O(\log n)$ 的和式（引理 7、8 从两条独立路径得到同一结果，交叉印证）；引理 9 保证 $64$ 位整数下每一步运算精确无溢出；引理 6 保证整数除法精确。故程序输出即答案。证毕。

---

## 三、模式总结（按实际思考顺序）

- **explore its property** — 末尾零 $=10$ 的因子个数 $=\min(v_2,v_5)$，把"字符串现象"翻译成质因子指数。
- **reduce to easier problem** — 证明逐项 $\lfloor k/2^i\rfloor\ge\lfloor k/5^i\rfloor$ ⇒ $v_2\ge v_5$，只需数 $5$。
- **consider easier version** — 单个阶乘的 Legendre 公式 $v_5(k!)=\sum_i\lfloor k/5^i\rfloor$；直接累加是 $O(n\log n)\approx10^9$，前缀和数组 $800$ MB，两条朴素路都堵死。
- **classic problem solution** — 乘积重排：$\prod_{k\le n}k!=\prod_{j\le n}j^{n-j+1}$（$j$ 出现在 $n-j+1$ 个阶乘里）。
- **explore its property** — $v_5(j)=\sum_i[5^i\mid j]$，把指数写成 indicator 之和，为交换求和铺路。
- **classic problem solution** — 交换求和次序 + 倍数上的等差数列求和 ⇒ 每个 $5^i$ 贡献 $a(n+1)-q\frac{a(a+1)}2$，总复杂度 $O(\log_5 n)$。
- **consider easier version** — 第二条路：对 $\sum_k\lfloor k/q\rfloor$ 按商值分组，得 $q\frac{a(a-1)}2+a(b+1)$，与主公式代数恒等，用于查变形错误。
- **explore its property** — 第三条路：$v_5(k!)=\frac{k-s_5(k)}4$，形式完全不同的独立校验。
- **discuss by category** — 值域与边界：`i64`（答案 $\approx1.25\times10^{15}$）、先乘后除、`q*=5` 终止、$n<5$ 输出 $0$；本题**不取模、不用逆元**。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|------|------|-------------|--------|
| `n` | 输入的 $n$（$\le10^8$） | `i64`，读入 | 用 `i64` 是为了后面乘法不溢出 |
| `ans` | $v_5(N)$，即末尾零个数 | `i64`，全局默认 $0$ | 累加器，**不取模** |
| `q` | 当前枚举的 $5^i$ | `i64`，循环变量，初值 $5$ | 每轮 `q*=5`，`q>n` 退出 |
| `a` | $\lfloor n/q\rfloor$，$[1,n]$ 中 $q$ 的倍数个数 | `i64`，每轮重算 | 等差数列的项数 |
| `a*(n+1)` | 等差数列"常数部分"之和 | 中间量 | $\sum_{m=1}^a(n+1)$ |
| `q*a*(a+1)/2` | 等差数列"递减部分"之和 | 中间量 | $q\sum_{m=1}^a m$，**先乘后除** |

（本题无数组，故不需要 `mxn`；也不需要 `inf` 哨兵。）

### 算法步骤

1. I/O 加速：`cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);`。
2. 读入 $n$（`i64`）。
3. 令 `ans=0`。对 `q=5; q<=n; q*=5` 循环：
   1. `a=n/q`；
   2. `ans += a*(n+1) - q*a*(a+1)/2`（引理 5 + 引理 6）。
4. 输出 `ans` 与 `'\n'`。
5. $n<5$ 时循环不执行，直接输出 $0$；$n=0$ 同理。

### 复杂度

- 时间：$O(\log_5 n)$，$n=10^8$ 时仅 $11$ 轮。
- 空间：$O(1)$（四个标量）。
- 值域：答案 $\le 2.5\times10^{15}$，中间量 $\le2.1\times10^{15}$，`i64` 安全（引理 9）。

### 验证状态

`g++ -std=c++20 -O2` 编译通过；题面无样例，改为**四路交叉对拍 3601 组**（$n=0..3000$ 穷举 $3001$ 组 + $300$ 组 $n\le5\times10^4$ 随机 + $300$ 组 $n\le10^8$ 随机），闭式与"逐 $k$ Legendre 暴力取 $\min(v_2,v_5)$"、"$\sum(n-j+1)v_5(j)$ 暴力"、"分组闭式 $q\frac{a(a-1)}2+a(b+1)$"、"数字和式 $\frac14(\frac{n(n+1)}2-\sum s_5(k))$"全部一致，$0$ mismatch；另用最终可执行文件端到端复核 $16$ 个 $n$ 值，并手算核对 $n=5\to1$、$n=10\to7$、$n=25\to56$、$n=100\to1124$、$n=10^8\to1249999451562499$。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2388 阶乘之乘
// 思路：末尾零 = min(v2,v5) = v5（因为 v2>=v5）
//       ∏ k! = ∏ j^{n-j+1}，v5(j)=Σ_i [5^i|j]
//       交换求和 ⇒ 每个 q=5^i 贡献 Σ_{m=1}^{n/q} (n-mq+1)
//       等差数列求和 ⇒ a*(n+1) - q*a*(a+1)/2，O(log_5 n)
//       答案约 1.25e15，必须 i64；本题不取模
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n,ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(i64 q=5;q<=n;q*=5)
	{
		i64 a=n/q;
		ans+=a*(n+1)-q*a*(a+1)/2;   // 先乘后除：a*(a+1) 必为偶数
	}
	cout<<ans<<'\n';
	return 0;
}
```

### 代码说明

- **全程 `i64`**：`n` 本身虽然只有 $10^8$，但 `a*(n+1)` 与 `q*a*(a+1)` 都到 $2\times10^{15}$ 量级，用 `int` 会在第一轮（$q=5$）就溢出。
- **`q*=5` 写在 `for` 的第三段**：`q` 是 `i64`，最大到 $5^{12}=244140625$ 就因 `q<=n` 失败而退出，不会无限自乘溢出。
- **`q*a*(a+1)/2` 的运算顺序**：C++ 从左到右结合，先算 `q*a*(a+1)` 再除 $2$。由引理 6，$a(a+1)$ 是偶数，整除精确；若改成 `q*(a/2)*(a+1)` 就会在 $a$ 为奇数时少算。
- **没有数组、没有 `mxn`、没有 `inf`**：本题是纯数学闭式，$O(1)$ 空间，不需要任何哨兵或预处理。
- **不取模、不求逆元**：输出是精确的零的个数（可达 $10^{15}$ 量级），任何取模都会破坏答案。
- `n<5`（含 $n=0,1$）时循环体一次都不执行，`ans` 保持全局默认 $0$，正是正确答案。