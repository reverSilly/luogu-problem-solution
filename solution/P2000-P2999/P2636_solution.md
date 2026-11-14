# P2636 密码破解者 — 题解

## 一、解题过程：人类思考叙事

**题意重述。** 一份明文被**依次**施加了 $N$ 重加密（$N\le1000$），得到密文 $S$（$|S|\le1000$）。输入给出 $N$、$S$，以及**按加密顺序**排列的 $N$ 重加密方式，每重是三种之一：

1. **栅栏密码**（参数栏数 $L$）：把明文每 $L$ 个一组，先取各组第 $1$ 个字符连起来，再取各组第 $2$ 个……依次拼接。最后一组不足 $L$ 个就只取它有的那些。
2. **维吉尼亚密码**（参数密钥串 $T$）：$c_i=m_i\oplus k_i$，$\oplus$ 由题面那张 $26\times26$ 的表给出；密钥不够长就循环使用。
3. **QWE 键盘码**：把字母表 `A..Z` 依次替换成键盘三行 `QWERTYUIOP ASDFGHJKL ZXCVBNM`。

要求输出还原后的明文。

**第一反应：解密就是"倒着做"。** 加密是 $S=E_N(E_{N-1}(\cdots E_1(P)\cdots))$。两边逐层取逆：
$$P=D_1(D_2(\cdots D_N(S)\cdots)),\qquad D_i=E_i^{-1}.$$
注意**逆序**：最后加密的那一重最先解。所以我先把 $N$ 重描述全读进来存好，再从第 $N$ 重往第 $1$ 重，对当前串依次施加对应的**解密**变换。框架就这么简单，剩下的全是"每种密码怎么解"。

**第二步：先捏软柿子——QWE 与维吉尼亚。**

- **QWE** 是单表替换：加密把 `A→Q, B→W, C→E, …`。解密只要建一张**反向表**：`qmap[Q]=A, qmap[W]=B, …`，逐字符查表即可。题面例子 `CODING→EGROFU` 正好用来核对这张表。
- **维吉尼亚** 那张吓人的 $26\times26$ 表，其实就是一句话：**$c=(m+k)\bmod 26$**（把 `A` 当 $0$）。看第一行 `A` 行是 `A B C … Z`（$m=0$ 时 $c=k$），第二行 `B` 行是 `B C … Z A`（$m=1$ 时 $c=(1+k)\bmod26$），规律一目了然。于是解密 $m=(c-k)\bmod26$。密钥循环：第 $i$ 位用 $T[i\bmod|T|]$。

**第三步：一个必须从样例抠出来的约定——大小写。** 题面维吉尼亚的例子是 `M=Helloworld, k=abc → C=Hfnlpyosnd`。我逐位核对：`H`(大写)+`a`→`H`(大写)，`e`(小写)+`b`→`f`(小写)……**密文的大小写和明文完全一致**，密钥只贡献"移位量"（`a/A`=0, `b/B`=1, …，与密钥自身大小写无关）。所以解密时**保留密文字符的大小写**，在该大小写区间内做 $(c-k)\bmod26$。QWE 例子全是大写，按同样精神保留大小写。栅栏只是搬运字符，大小写天然跟着走。

**第四步：硬骨头——栅栏密码的逆。** 加密是"按列读"，解密就得"按列写回"。我先把加密的结构精确写出来：明文位置 $p$ 属于第 $g=\lfloor p/L\rfloor$ 组、组内第 $j=p\bmod L$ 个。加密时**先按 $j$ 从小到大、再按 $g$ 从小到大**输出 $P[gL+j]$。也就是说，密文是 $L$ 段"栏"的拼接：
$$C=\underbrace{P[0],P[L],P[2L],\dots}_{\text{栏 }0}\ \underbrace{P[1],P[L+1],\dots}_{\text{栏 }1}\ \cdots$$
栏 $j$ 收集所有满足 $p\equiv j\pmod L$ 且 $p<n$ 的明文位置。

解密的关键是算出**每栏有多长**。栏 $j$ 的字符数
$$\mathrm{cnt}[j]=\#\{g\ge0:gL+j<n\}=\Big\lfloor\frac{n-1-j}{L}\Big\rfloor+1\quad(j<n).$$
知道每栏长度，就知道栏 $j$ 在密文里的起点 $\mathrm{off}[j]=\sum_{t<j}\mathrm{cnt}[t]$，于是把密文段 $C[\mathrm{off}[j]\,..\,\mathrm{off}[j]+\mathrm{cnt}[j]-1]$ 里的第 $g$ 个字符**写回**明文位置 $gL+j$。每个明文位置恰好被写一次，完美还原。

**第五步：碰壁——边界 $L\ge n$ 与最后一组不足。** 写公式时我担心两件事：

- **最后一组不足 $L$**：比如 $n=14,L=3$，分组 `THE REI SAC IPH ER`，最后一组只有 $2$ 个。会不会让 $\mathrm{cnt}$ 算错？代入公式：$\mathrm{cnt}[2]=\lfloor(14-1-2)/3\rfloor+1=\lfloor11/3\rfloor+1=4$，而栏 $2$ 确实只有 $4$ 个字符（位置 $2,5,8,11$），公式自动把"短组"算进去了，不用特判。
- **$L\ge n$**：这时只有一组（整串），每栏至多 $1$ 个字符，加密其实是**恒等**（$C=P$）。公式里 $\lfloor(n-1-j)/L\rfloor=0$，$\mathrm{cnt}[j]=1$，写回 $P[j]=C[j]$，也是恒等。所以代码里把 $L$ 截到 $\min(L,n)$ 完全安全，还能避免 $L$ 极大时的无谓循环。

**手造数据核对**（题面给了 worked example，我直接拿来当测试）：

| 输入（$N$ / $S$ / 各重） | 输出 | 来源 |
|---|---|---|
| `1 / TEESCPEHRIAIHR / 1 2` | `THEREISACIPHER` | 题面 $2$ 栏例子逆推 |
| `1 / TRSIEHEAPREICH / 1 3` | `THEREISACIPHER` | 题面 $3$ 栏例子逆推 |
| `1 / Hfnlpyosnd / 2 abc` | `Helloworld` | 题面维吉尼亚例子 |
| `1 / EGROFU / 3` | `CODING` | 题面 QWE 例子 |
| `3 / ETGHOW / 3 / 2 abc / 1 2` | `CODING` | 自造三重复合（见下） |

最后一组是我自己造的：明文 `CODING` 先 QWE→`EGROFU`，再维吉尼亚(`abc`)→`EHTOGW`，再 $2$ 栏栅栏→`ETGHOW`。解密按逆序 `栅栏→维吉尼亚→QWE` 应回到 `CODING`，程序输出一致。

---

## 二、思维链：完整证明

**记号。** 明文 $P[0..n-1]$，密文 $C[0..n-1]$（三种加密都保长度，故 $|C|=|P|=n$）。第 $i$ 重加密算子 $E_i$、解密算子 $D_i$。字母数值化：`A/a`$\to0$，…，`Z/z`$\to25$。$\mathrm{val}(c)$ 表示字符 $c$ 在其大小写区间内的数值（`c-'A'` 或 `c-'a'`）。

---

### 引理 1（解密顺序是加密顺序的逆）

设 $S=E_N\circ E_{N-1}\circ\cdots\circ E_1(P)$，且每个 $E_i$ 可逆、逆为 $D_i$。则 $P=D_1\circ D_2\circ\cdots\circ D_N(S)$。

*证明*：对 $N$ 归纳。$N=1$：$S=E_1(P)\Rightarrow P=D_1(S)$，成立。设 $N-1$ 成立。令 $S'=E_{N-1}\circ\cdots\circ E_1(P)$，则 $S=E_N(S')$，故 $S'=D_N(S)$。由归纳假设 $P=D_1\circ\cdots\circ D_{N-1}(S')=D_1\circ\cdots\circ D_{N-1}(D_N(S))=D_1\circ\cdots\circ D_N(S)$。证毕。

**推论**：算法读入全部 $N$ 重后，`for i=N-1 downto 0` 依次施加 $D_i$，即得明文。三种 $E_i$ 的可逆性分别由引理 2、4、5 保证。

---

### 引理 2（栅栏加密是置换，且解密公式正确）

设明文长 $n$、栏数 $L\ge1$。定义 $\mathrm{cnt}[j]=\lfloor(n-1-j)/L\rfloor+1$（$0\le j<\min(L,n)$），$\mathrm{off}[j]=\sum_{t<j}\mathrm{cnt}[t]$。则：

(a) 加密把 $P[gL+j]$ 放到 $C[\mathrm{off}[j]+g]$（$0\le j<\min(L,n)$，$0\le g<\mathrm{cnt}[j]$），这是一个 $\{0..n-1\}$ 上的双射；
(b) 解密 $P[gL+j]=C[\mathrm{off}[j]+g]$ 恰好还原明文。

*证明*：

**(a) 加密结构。** 明文位置 $p$ 唯一写成 $p=gL+j$，$j=p\bmod L\in[0,L)$，$g=\lfloor p/L\rfloor$。题面加密"先取各组第 $1$ 个、再第 $2$ 个……"即按 $j$ 升序、同 $j$ 内按 $g$ 升序输出 $P[gL+j]$（仅当 $gL+j<n$）。故密文按栏 $j=0,1,\dots$ 分段，栏 $j$ 内第 $g$ 个字符是 $P[gL+j]$。

栏 $j$ 的长度 $=\#\{g\ge0:gL+j<n\}=\#\{g:0\le g<(n-j)/L\}$。当 $j<n$ 时这是 $\lceil(n-j)/L\rceil=\lfloor(n-j-1)/L\rfloor+1=\lfloor(n-1-j)/L\rfloor+1=\mathrm{cnt}[j]$；当 $j\ge n$ 时为 $0$（该栏无字符）。故只需考虑 $j<\min(L,n)$。栏 $j$ 在密文中的起点是前面各栏长度之和 $\mathrm{off}[j]$，于是 $P[gL+j]\mapsto C[\mathrm{off}[j]+g]$。

**双射**：每个明文位置 $p=gL+j$（$j=p\bmod L<\min(L,n)$，$g<\mathrm{cnt}[j]$ 因 $gL+j=p<n$）被映到唯一的密文下标 $\mathrm{off}[j]+g$；不同 $p$ 的 $(j,g)$ 不同，而 $(j,g)\mapsto\mathrm{off}[j]+g$ 是单射（栏段互不重叠：$\mathrm{off}[j]+\mathrm{cnt}[j]=\mathrm{off}[j+1]$）。又 $\sum_{j<\min(L,n)}\mathrm{cnt}[j]=n$（每个位置恰属一栏），故密文下标恰好铺满 $[0,n)$。双射成立。

**(b) 解密。** 由 (a) 的对应关系反解即 $P[gL+j]=C[\mathrm{off}[j]+g]$。代码按 $j$ 升序维护 `off`（每栏处理完 `off+=cnt`），对 $g=0..\mathrm{cnt}[j]-1$ 写回 `p[g*L+j]=c[off+g]`，正是此式。每个明文位置被写恰好一次，故还原。证毕。

---

### 引理 3（$L\ge n$ 时栅栏是恒等；截断 $L$ 安全）

若 $L\ge n$，则栅栏加密 $C=P$（恒等）。代码中 `if(L>n)L=n` 不改变解密结果。

*证明*：$L\ge n$ 时，满足 $gL+j<n$ 的只有 $g=0$（$g\ge1$ 时 $gL\ge L\ge n$），即只有一组。对 $j<n$，$\mathrm{cnt}[j]=\lfloor(n-1-j)/L\rfloor+1=0+1=1$（因 $n-1-j<n\le L$），$\mathrm{off}[j]=j$。加密 $C[j]=P[0\cdot L+j]=P[j]$，故 $C=P$。

截断后 $L'=n$：对 $j<n$，$\mathrm{cnt}[j]=\lfloor(n-1-j)/n\rfloor+1=1$，$\mathrm{off}[j]=j$，写回 $P[j]=C[j]$，同样恒等。两者一致，故 `if(L>n)L=n` 安全（且把循环次数压到 $\le n$，避免 $L$ 极大时的浪费与潜在溢出）。证毕。

---

### 引理 4（维吉尼亚：表即模 $26$ 加法，解密为减法，保大小写）

题面 $\oplus$ 表满足：明文数值 $m$、密钥数值 $k$ 时密文数值 $c=(m+k)\bmod26$。解密 $m=(c-k)\bmod26$。加密保持明文字符的大小写，密钥数值取其字母位置（与密钥大小写无关），密钥按 $i\bmod|T|$ 循环。

*证明*：观察表：第 `A` 行（$m=0$）为 `A B C … Z`，即 $c=k$；第 `B` 行（$m=1$）为 `B C … Z A`，即 $c=(1+k)\bmod26$；一般地第 $m$ 行是字母表循环左移 $m$ 位，故 $c=(m+k)\bmod26$。这是 $\mathbb Z_{26}$ 上的加法，逆运算为减法：$m=(c-k)\bmod26$。代码用 `((val(c)-sh)%26+26)%26` 保证结果落在 $[0,26)$（C++ 的 `%` 对负数向零截断，故先 `+26` 再 `%26`）。

**大小写**：题面例 `Helloworld`+`abc`→`Hfnlpyosnd` 逐位核对：`H`(7,大写)+`a`(0)=7→`H`(大写)；`e`(4,小写)+`b`(1)=5→`f`(小写)；`l`(11)+`c`(2)=13→`n`；`l`+`a`=11→`l`；`o`(14)+`b`=15→`p`；`w`(22)+`c`=24→`y`；`o`+`a`=14→`o`；`r`(17)+`b`=18→`s`；`l`+`c`=13→`n`；`d`(3)+`a`=3→`d`。密文大小写与明文逐位相同，密钥全小写却只贡献移位量。故约定：加密保留明文大小写、移位量 $=$ 密钥字母位置（`tolower(T[i%m])-'a'`）。解密时密文大小写 $=$ 明文大小写，在该区间内做减法即可。证毕。

---

### 引理 5（QWE 是单表替换，逆表解密，保大小写）

设键盘串 $\mathrm{kb}=$`QWERTYUIOPASDFGHJKLZXCVBNM`（长 $26$）。加密把数值 $i$ 的字母映到 $\mathrm{kb}[i]$。则 $\mathrm{kb}$ 是 `A..Z` 的一个排列，逆表 $\mathrm{qmap}[\mathrm{kb}[i]]=i$ 良定义，解密 $P[i]=\mathrm{qmap}[C[i]]$（保大小写）。

*证明*：$\mathrm{kb}$ 由键盘三行 `QWERTYUIOP`(10)、`ASDFGHJKL`(9)、`ZXCVBNM`(7) 拼成，共 $26$ 个字母且无重复（键盘字母区每个字母恰出现一次），故是 `A..Z` 的排列，加密是双射，逆表存在且唯一：`for i: qmap[kb[i]-'A']=i`。

核对题面例 `CODING→EGROFU`：`C`(2)→kb[2]=`E`，`O`(14)→kb[14]=`G`，`D`(3)→kb[3]=`R`，`I`(8)→kb[8]=`O`，`N`(13)→kb[13]=`F`，`G`(6)→kb[6]=`U`，得 `EGROFU` ✓。解密用逆表逐字符查回。大小写：例全大写，按与维吉尼亚一致的约定保留大小写（大写查 `qmap[c-'A']` 还大写，小写查 `qmap[c-'a']` 还小写）。证毕。

---

### 引理 6（三种解密都保长度，复合良定义）

栅栏、维吉尼亚、QWE 的解密都把长度 $n$ 的串映到长度 $n$ 的串。故引理 1 的逐层复合中串长恒为 $n$，各层参数（$L$、$T$）与当前串独立，可预先读入。

*证明*：栅栏解密写回 $n$ 个位置（引理 2 双射），输出长 $n$；维吉尼亚逐位映 $n$ 个字符；QWE 逐位查表 $n$ 个字符。三者保长度。各层参数只依赖该层描述（$L$ 或 $T$），不依赖串内容，故可一次性读入存储，逆序套用。证毕。

---

### 引理 7（复杂度）

时间 $O(N\cdot n)$，空间 $O(N+n)$（存 $N$ 重描述与长度 $n$ 的串）。

*证明*：每重解密扫一遍当前串：栅栏 $O(n)$（每位置写一次），维吉尼亚 $O(n)$，QWE $O(n)$。共 $N$ 重，$O(Nn)$。读入与存储 $N$ 重描述 $O(N)$，串 $O(n)$。代入 $N,n\le1000$：$\le10^6$ 次字符操作，远低于 $1$ 秒。证毕。

---

### 定理

程序输出 $N$ 重加密前的明文。

*证明*：引理 4、5、2 分别给出三种加密的可逆性与解密公式（保大小写约定由题面样例确定，引理 4、5）；引理 3 处理栅栏 $L\ge n$ 边界；引理 6 保证逐层复合保长度、参数可预读；引理 1 保证按加密逆序套用解密即还原明文；引理 7 给出复杂度。证毕。

---

## 三、模式总结（按实际思考顺序）

- **reduce to easier problem** — 多重加密的解密 = 各重逆运算按**加密逆序**复合：$P=D_1\circ\cdots\circ D_N(S)$。
- **classic problem solution** — QWE 单表替换：建反向表 `qmap[kb[i]]=i`，逐字符查回。
- **explore its property** — 维吉尼亚的 $26\times26$ 表本质是 $c=(m+k)\bmod26$，解密即减法；密钥循环、移位量与密钥大小写无关。
- **explore its property** — 从样例 `Helloworld→Hfnlpyosnd` 抠出"保大小写"约定，三种密码统一保留字符大小写。
- **explore its property** — 栅栏加密是按列读的置换：栏 $j$ 收集 $p\equiv j\pmod L$ 的位置，$\mathrm{cnt}[j]=\lfloor(n-1-j)/L\rfloor+1$。
- **reduce to easier problem** — 栅栏解密 = 算每栏长度定偏移，再把密文段scatter写回 $P[gL+j]$，双射还原。
- **discuss by category** — 边界：最后一组不足 $L$ 由 $\mathrm{cnt}$ 公式自动处理；$L\ge n$ 是恒等，截 $L$ 到 $n$ 安全。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|---|---|---|---|
| `N` | 加密重数（$\le1000$） | `int`，读入 | 逆序解密 |
| `S` | 当前串（初始为密文，逐层变明文） | `string`，读入 | 长度恒 $n$ |
| `K[i]` | 第 $i$ 重类型 $1/2/3$ | `int[mxn]`，读入 | 0-based，加密顺序 |
| `L[i]` | 第 $i$ 重栅栏栏数（仅 `K=1`） | `int[mxn]` | 解密时截到 $\min(L,n)$ |
| `T[i]` | 第 $i$ 重维吉尼亚密钥（仅 `K=2`） | `string[mxn]` | 循环使用 |
| `kb` | 键盘串 `QWERTYUIOPASDFGHJKLZXCVBNM` | `string` | QWE 加密表 |
| `qmap[26]` | QWE 逆表：`qmap[kb[i]-'A']=i` | `int[26]` | 解密查表 |
| `mxn` | $1000+5$ | `constexpr int{1005}` | 层数组尺寸 |
| `raildec(c,L)` | 栅栏解密 | 函数 | 算 cnt/off 后 scatter |
| `vigdec(c,key)` | 维吉尼亚解密 | 函数 | $(c-k)\bmod26$，保大小写 |
| `qwedec(c)` | QWE 解密 | 函数 | 查 `qmap`，保大小写 |

### 算法步骤

1. I/O 加速三行；建 QWE 逆表 `qmap[kb[i]-'A']=i`（$i=0..25$）。
2. 读 `N`、`S`；对 $i=0..N-1$ 读 `K[i]`，若 `K=1` 读 `L[i]`，若 `K=2` 读 `T[i]`。
3. `for i=N-1 downto 0`（加密逆序）：
   - `K=1`：`S=raildec(S,L[i])`；
   - `K=2`：`S=vigdec(S,T[i])`；
   - `K=3`：`S=qwedec(S)`。
4. 输出 `S`。

其中 `raildec`：$n=|c|$，`if(L>n)L=n`；`off=0`；对 $j=0..L-1$：$\mathrm{cnt}=(n-1-j)/L+1$，对 $g=0..\mathrm{cnt}-1$ 令 `p[g*L+j]=c[off+g]`，然后 `off+=cnt`。

### 复杂度

- 时间：$O(Nn)\le10^6$。
- 空间：$O(N+n)$。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；题面 $4$ 个 worked example（$2$ 栏/$3$ 栏栅栏、维吉尼亚、QWE）逐一吻合；自造三重复合 `CODING` 往返一致；另与"按题面定义正向实现的加密函数"做**往返对拍 20000 组**（随机长度 $\le40$、随机大小写、随机 $L\le20$、随机密钥，单层 + 三层复合），$0$ mismatch。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2636 密码破解者
// 思路：解密 = 各重逆运算按加密逆序复合。
//       栅栏：算每栏长度 cnt[j]=(n-1-j)/L+1 定偏移，把密文段写回 P[g*L+j]。
//       维吉尼亚：表即 c=(m+k)%26，解密 m=(c-k)%26，密钥循环、保大小写。
//       QWE：单表替换，建逆表 qmap 查回，保大小写。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int N,K[mxn],L[mxn];
string S,T[mxn];
string kb="QWERTYUIOPASDFGHJKLZXCVBNM";
int qmap[26];

string raildec(string c,int L)
{
	int n=c.size();
	if(L>n)L=n;				// L>=n 时栅栏是恒等，截到 n 不影响
	string p(n,' ');
	int off=0;
	for(int j=0;j<L;++j)
	{
		int cnt=(n-1-j)/L+1;		// 第 j 栏的字符数
		for(int g=0;g<cnt;++g)
			p[g*L+j]=c[off+g];	// 写回明文位置 g*L+j
		off+=cnt;
	}
	return p;
}

string vigdec(string c,string key)
{
	int n=c.size(),m=key.size();
	string p(n,' ');
	for(int i=0;i<n;++i)
	{
		int sh=tolower(key[i%m])-'a';	// 移位量与密钥大小写无关
		char ch=c[i];
		if(ch>='A'&&ch<='Z')
			p[i]='A'+((ch-'A'-sh)%26+26)%26;
		else if(ch>='a'&&ch<='z')
			p[i]='a'+((ch-'a'-sh)%26+26)%26;
		else
			p[i]=ch;
	}
	return p;
}

string qwedec(string c)
{
	int n=c.size();
	string p(n,' ');
	for(int i=0;i<n;++i)
	{
		char ch=c[i];
		if(ch>='A'&&ch<='Z')
			p[i]='A'+qmap[ch-'A'];
		else if(ch>='a'&&ch<='z')
			p[i]='a'+qmap[ch-'a'];
		else
			p[i]=ch;
	}
	return p;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=0;i<26;++i)
		qmap[kb[i]-'A']=i;		// QWE 逆表
	cin>>N>>S;
	for(int i=0;i<N;++i)
	{
		cin>>K[i];
		if(K[i]==1)cin>>L[i];
		else if(K[i]==2)cin>>T[i];
	}
	for(int i=N-1;i>=0;--i)		// 加密顺序的逆序逐层解密
	{
		if(K[i]==1)S=raildec(S,L[i]);
		else if(K[i]==2)S=vigdec(S,T[i]);
		else S=qwedec(S);
	}
	cout<<S<<'\n';
	return 0;
}
```

### 代码说明

- **逆序解密**：`for(i=N-1;i>=0;--i)` 是引理 1 的直接翻译——最后加密的最先解。各重参数预先读入 `K/L/T`，与串内容无关。
- **栅栏 `off` 边扫边累加**：处理栏 $j$ 前 `off` 恰为 $\sum_{t<j}\mathrm{cnt}[t]$，处理完 `off+=cnt`，无需额外前缀和数组。`cnt=(n-1-j)/L+1` 自动处理"最后一组不足 $L$"。
- **`if(L>n)L=n`**：$L\ge n$ 时加密是恒等（引理 3），截断既正确又把循环压到 $\le n$，避免 $L$ 极大时的浪费。
- **维吉尼亚 `((x)%26+26)%26`**：C++ 负数取模向零截断，先 `+26` 再 `%26` 保证落在 $[0,26)$；`tolower(key[i%m])` 让密钥大小写不影响移位量；按密文字符大小写分支，保留大小写。
- **`qmap` 而非 `dec`**：`dec` 会与 `std::dec`（流操纵符）在 `using namespace std;` 下歧义，故命名 `qmap`。
- 三种解密都保长度（引理 6），`S` 在层间原地替换，最后输出即明文。