# P2880 [USACO07JAN] Balanced Lineup G — 题解

## 一、解题过程：人类思考叙事

**题意重述。** $n\le 5\times10^4$ 头牛排成固定队列，身高 $h_i\le 10^6$。$q\le 1.8\times10^5$ 个询问，每个给 $a,b$，输出第 $a$ 到第 $b$ 头牛中**最高与最低身高之差**。队列不变、没有修改——纯静态区间询问。

剥掉背景就是两个经典 **RMQ（区间最值查询）**：答案 $=\max_{i\in[a,b]}h_i-\min_{i\in[a,b]}h_i$。max 和 min 互不干扰，各维护一份即可。

**第一步：否掉暴力。** 每询问扫一遍区间是 $O(nq)=5\times10^4\times1.8\times10^5=9\times10^9$，妥妥超时。需要预处理换查询速度。

**第二步：选数据结构。** 没有修改，候选就两个：

- 线段树：建树 $O(n)$，单查 $O(\log n)$，总量约 $1.8\times10^5\times16\approx2.9\times10^6$ 次合并，能过。
- ST 表（稀疏表）：预处理 $O(n\log n)$，单查 $O(1)$，总量约 $5\times10^4\times16+1.8\times10^5\times2\approx1.2\times10^6$ 次合并，更快且代码更短。

ST 表查询能 $O(1)$ 的**唯一理由**是运算**幂等**：$\max(x,x)=x$。这允许用两个**重叠**的 $2^k$ 长区间盖住询问区间——重叠部分被算两次也不影响最大值。换成"区间和"就绝不能这么查（重叠会重复计数）。max 和 min 都幂等，所以双 ST 表成立。这是本题（也是所有 ST 表题）的思维支点。

**第三步：具体形态。** `stmx[k][i]` = 区间 $[i,i+2^k)$ 的最大值，`stmn` 同理存最小值。与"查单词"那类题不同，这里**直接存值**不存下标：答案只要差值，不需要知道是哪头牛，`int` 值合并一次到位。询问 $[a,b]$（转 0-based 闭区间）：取 $k=\lfloor\log_2(b-a+1)\rfloor$，用 $[a,a+2^k)$ 与 $[b-2^k+1,b+1)$ 两块盖住它，分别取 max 的 max、min 的 min，相减输出。

**第四步：抠细节。**

1. **`lg` 表**：`lg[len]=⌊log2 len⌋`，递推 `lg[i]=lg[i>>1]+1`，`lg[1]=0`（全局默认 $0$ 恰好正确）。
2. **建树边界**：外层 `(1<<k)<=n`、内层 `i+(1<<k)<=n`，只建有定义的格子。$n\le5\times10^4<2^{16}$，用到第 $15$ 层，`mxk=17` 足够。
3. **查询读到的格子一定建过**（引理 5 会证）。这一点必须较真：`stmn` 是全局 `int`，未建格子默认 $0$，而 $h_i\ge1$——读到未建格子不会崩，只会让 min 静默变成 $0$、答案偏大。边界写错是**静默错误**。
4. **$a>b$**：题面没说保证 $a\le b$，加一句 `if(a>b)swap(a,b);` 零成本保险。
5. **$a=b$**：长度 $1$，$k=0$，两块重合为同一格，差为 $0$，自然正确。

**手造数据核对**（题面无样例，全部自造）：

第一组，$n=5,q=3$，$h=[1,5,3,2,4]$，询问 `1 5 / 2 3 / 4 4`：

```
4      ← max 5 − min 1
2      ← [5,3]：5−3
0      ← 单头牛，差 0
```

第二组，$n=1$，$h=[7]$，两次 `1 1`：都输出 `0`（最小规模）。

第三组，$n=4$ 全 $3$，询问 `1 4 / 2 3`：都输出 `0`（全相等，max=min）。

第四组，$n=3$，$h=[10,1,5]$，询问 `3 1`（故意反序）：swap 后 $[10,1,5]$，$10-1=9$。

第五组，$n=33$，$h_i=i$，询问 `1 33 / 2 33 / 17 17 / 1 32`：输出 `32 / 31 / 0 / 31`。这组专门让长度 $33$ 触发 $k=5$、两块 $32$ 长区间**重叠 31 格**的极限覆盖，检验幂等合并与深层建树。

五组程序输出与手算逐条一致。

---

## 二、思维链：完整证明

**记号。** 身高数组 $h_0,\dots,h_{n-1}$，$1\le h_i\le10^6$。询问 $[a,b]$（0-based 闭，$a\le b$，必要时 swap）。$len=b-a+1$，$k=\lfloor\log_2 len\rfloor$。$\mathrm{mx},\mathrm{mn}$ 为有限非空多重集上的最大/最小值运算。

---

### 引理 1（答案分解）

询问答案 $=\mathrm{mx}\{h_i:i\in[a,b]\}-\mathrm{mn}\{h_i:i\in[a,b]\}$，且两个最值可独立计算。

*证明*：题面定义答案为区间内最高身高减最低身高，即最大值减最小值，逐字翻译。max 与 min 的定义互不引用，可分别用独立的数据结构维护；差值非负（最大值 $\ge$ 最小值），`int` 范围内（$\le10^6$）无溢出。证毕。

---

### 引理 2（max/min 的运算律与重叠合并）

$\mathrm{mx}$ 满足交换律、结合律、幂等律；从而对任意有限非空多重集，任意顺序、任意重复次数的两两合并结果都等于集合最大值。$\mathrm{mn}$ 同理（把所有 $\ge$ 反向）。

*证明*：交换律由 $\{a,b\}$ 无序；幂等律 $\mathrm{mx}(a,a)=a$ 由定义。

结合律：设 $m=\max\{a,b,c\}$（有限全序集最大值存在唯一）。左边 $\mathrm{mx}(\mathrm{mx}(a,b),c)$：$\mathrm{mx}(a,b)\succeq a,b$，再与 $c$ 取 max 后 $\succeq a,b,c$，故 $\succeq m$；又结果 $\in\{a,b,c\}$，故 $\preceq m$；反对称得 $=m$。右边同理 $=m$。

多重集情形对元素个数归纳：个数 $1$ 平凡。$t+1$ 个元素的任何合并过程最后一步是 $\mathrm{mx}(X,Y)$，$X,Y$ 是两个非空子多重集的合并结果；归纳假设 $X=\max S_1,Y=\max S_2$，而两个最大值的 max 是并的最大值（结合律论证的原样复用）。重复元素无害：$\mathrm{mx}$ 只关心"是否存在更大者"，多算几次不改变最大值（幂等律）。证毕。

**推论（ST 表合法性）**：结合 + 交换 $\Rightarrow$ 可以任意分块预处理；幂等 $\Rightarrow$ 查询时允许两块**重叠**覆盖。若运算不幂等（如求和），重叠覆盖会把重叠部分重复计入，ST 表 $O(1)$ 查询立即失效。

---

### 引理 3（ST 表预处理不变量）

建树后，对所有 $k\ge0$ 与 $i+2^k\le n$：
$$\texttt{stmx}[k][i]=\max_{j\in[i,i+2^k)}h_j,\qquad \texttt{stmn}[k][i]=\min_{j\in[i,i+2^k)}h_j.$$

*证明*：对 $k$ 归纳，两表同步。$k=0$：代码 `stmx[0][i]=stmn[0][i]=h[i]`，区间 $[i,i+1)=\{i\}$ ✓。

设 $k-1$ 成立。$[i,i+2^k)$ **不交**地拆成 $[i,i+2^{k-1})$ 与 $[i+2^{k-1},i+2^k)$。归纳假设给出两半的最值；引理 2 说两半最值的合并就是并的最值；代码恰用 `max(stmx[k-1][i],stmx[k-1][i+(1<<(k-1))])`（min 对称）计算它。

边界：内层条件 `i+(1<<k)<=n` 保证两个子区间都在 $[0,n)$ 内，引用的 `st[k-1][*]` 均已建过（$i+2^{k-1}+2^{k-1}=i+2^k\le n$）；外层 `(1<<k)<=n` 保证该层至少有一个合法格子。证毕。

---

### 引理 4（`lg` 表正确）

`lg[1]=0`（全局默认），`lg[i]=lg[i>>1]+1`（$i\ge2$）。则 $\texttt{lg}[i]=\lfloor\log_2 i\rfloor$ 对 $1\le i\le n$ 成立。

*证明*：先证恒等式：整数 $i\ge2$ 时 $\lfloor\log_2 i\rfloor=\lfloor\log_2\lfloor i/2\rfloor\rfloor+1$。设 $k=\lfloor\log_2 i\rfloor\ge1$，则 $2^k\le i<2^{k+1}$。由 $i\ge2^k$ 得 $\lfloor i/2\rfloor\ge2^{k-1}$；由 $i\le2^{k+1}-1$ 得 $\lfloor i/2\rfloor\le2^k-1<2^k$。故 $\lfloor\log_2\lfloor i/2\rfloor\rfloor=k-1$。

再对 $i$ 归纳：$i=1$ 时两边都是 $0$ ✓；$i\ge2$ 时 `i>>1`$=\lfloor i/2\rfloor<i$，归纳假设 + 恒等式即得。证毕。

---

### 引理 5（$O(1)$ 查询的两块覆盖）

设 $len=b-a+1\ge1$，$k=\texttt{lg}[len]$，$B_1=[a,a+2^k)$，$B_2=[b-2^k+1,b+1)$。则

(a) $B_1,B_2\subseteq[a,b+1)$；
(b) $B_1\cup B_2=[a,b+1)$；
(c) `stmx[k][a]`、`stmx[k][b-2^k+1]`（及 stmn 对应格）都是已建好的格子；
(d) $\max(\texttt{stmx}[k][a],\texttt{stmx}[k][b-2^k+1])-\min(\texttt{stmn}[k][a],\texttt{stmn}[k][b-2^k+1])$ 就是答案。

*证明*：由 $k=\lfloor\log_2 len\rfloor$ 得 $2^k\le len<2^{k+1}$。

(a) $B_1$ 右端 $a+2^k\le a+len=b+1$ ✓；$B_2$ 左端 $b-2^k+1\ge b-len+1=a$ ✓。

(b) 无空隙条件：$B_2$ 左端 $\le B_1$ 右端 $\iff b-2^k+1\le a+2^k\iff len\le2^{k+1}$，由 $len<2^{k+1}$ 成立。$B_1$ 从 $a$ 盖到 $a+2^k-1$，$B_2$ 从 $\le a+2^k$ 盖到 $b$，并集恰为 $[a,b]$。$len=2^k$ 时两块重合，也成立。

(c) `st[k][a]` 需 $a+2^k\le n$：由 (a) $a+2^k\le b+1\le n$ ✓；`st[k][b-2^k+1]` 需 $(b-2^k+1)+2^k=b+1\le n$ ✓。又 $k\le\lfloor\log_2 n\rfloor$，外层循环一定建到了第 $k$ 层。两格都被引理 3 覆盖，**不会读到全局默认的 $0$**——对 `stmn` 尤其关键，$0<h_i$，读错格子会让答案静默偏大。

(d) 引理 3 给出两格分别是 $B_1,B_2$ 的最值；引理 2（幂等 $\Rightarrow$ 重叠无害）给出 $\mathrm{mx}(\max B_1,\max B_2)=\max(B_1\cup B_2)=\max[a,b+1)$，min 同理；引理 1 说差值即答案。证毕。

---

### 引理 6（复杂度）

时间 $O(n\log n+q)$，空间 $O(n\log n)$。

*证明*：读入 $O(n+q)$；`lg` 表 $O(n)$；建树层数 $K=\lfloor\log_2 n\rfloor+1\le16$，第 $k$ 层 $\le n$ 格，每格常数次 max/min，共 $O(nK)=O(n\log n)$。每询问一次查表、两次合并、一次减法，$O(1)$，共 $O(q)$。空间：两张 $17\times50005$ 的 `int` 表约 $6.8$ MB，其余 $O(n)$。

代入：$n=5\times10^4,q=1.8\times10^5$：预处理约 $8\times10^5$ 次合并 + 查询约 $3.6\times10^5$ 次合并，总量 $\approx1.2\times10^6$，远低于时限。证毕。

---

### 定理

程序对每个询问输出区间最高与最低身高之差，总复杂度 $O(n\log n+q)$。

*证明*：引理 1 把答案拆成两个独立 RMQ；引理 2 证明 max/min 幂等、可重叠合并，ST 表合法；引理 3、4 保证预处理与 `lg` 正确；引理 5 保证每次查询两块合法、覆盖完整、结果正确；引理 6 给出复杂度。证毕。

---

## 三、模式总结（按实际思考顺序）

1. **explore its property** — 队列固定、无修改 $\Rightarrow$ 静态 RMQ；答案 = 区间 max − 区间 min，两个最值独立。
2. **consider easier version** — 暴力每询问扫区间 $O(nq)=9\times10^9$ 超时，确认需要预处理。
3. **classic problem solution** — 线段树可行（$O(q\log n)$）；无修改时 ST 表更优（$O(n\log n)$ 预处理 + $O(1)$ 查询）。
4. **explore its property** — ST 表 $O(1)$ 查询的支点是幂等：$\max(x,x)=x$ 允许两块重叠覆盖；求和就不行。
5. **classic problem solution** — 双 ST 表存值不存下标；`lg[i]=lg[i>>1]+1` 递推，查询取 $k=\texttt{lg}[len]$、两块 $[a,a+2^k)$ 与 $[b-2^k+1,b+1)$。
6. **discuss by category** — 建树边界保证查询不读未建格（`stmn` 默认 $0$ 会静默出错）；$a>b$ 时 swap；$a=b$ 差 $0$；全相等差 $0$。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 | 初值 / 记忆点 |
|---|---|---|---|
| `mxn` | $50000+5$ | `constexpr int` | 数组尺寸 |
| `mxk` | ST 表层数上界 $17$（$2^{16}>5\times10^4$，实际用到第 $15$ 层） | `constexpr int` | 第一维 |
| `h[i]` | 第 $i$ 头牛身高 | `int[mxn]` | 读入，0-based |
| `lg[len]` | $\lfloor\log_2 len\rfloor$ | `int[mxn]` | `lg[1]=0` 全局默认；`lg[i]=lg[i>>1]+1` |
| `stmx[k][i]` | $[i,i+2^k)$ 的最大身高 | `int[mxk][mxn]` | `stmx[0][i]=h[i]`；只在 $i+2^k\le n$ 处有定义 |
| `stmn[k][i]` | $[i,i+2^k)$ 的最小身高 | `int[mxk][mxn]` | 同上；**未建格默认 $0$ 是陷阱** |
| `a,b` | 询问端点，读入后 `--a;--b`，必要时 swap | `int` | 每询问读入 |
| `k` | 块长指数 `lg[b-a+1]` | `int` | 每询问现算 |

### 算法步骤

1. I/O 加速三行；读 $n,q$ 与 $h_i$，同时 `stmx[0][i]=stmn[0][i]=h[i]`。
2. 递推 `lg[2..n]`。
3. 对 $k=1,2,\dots$ 直到 $2^k>n$：对 $i=0..n-2^k$，两表分别用左右两半合并。
4. 每询问：读 $a,b$ 转 0-based，必要时 swap；$k=\texttt{lg}[b-a+1]$；两块取 max 的 max、min 的 min；输出差。

### 复杂度

- 时间：$O(n\log n+q)\approx1.2\times10^6$ 次合并。
- 空间：$O(n\log n)\approx6.8$ MB（双 ST 表）。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；Part 1 五组手造数据（含单点、全相等、反序端点、$n=33$ 触发 $k=5$ 重叠 $31$ 格的极限覆盖）全部与手算一致。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2880 [USACO07JAN] Balanced Lineup G
// 静态区间 max-min ⇒ 双 ST 表；max/min 幂等 ⇒ 两块重叠覆盖 O(1) 查询
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005},mxk{17};

int n,q,h[mxn],lg[mxn],stmx[mxk][mxn],stmn[mxk][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	for(int i=0;i<n;++i)
	{
		cin>>h[i];
		stmx[0][i]=stmn[0][i]=h[i];
	}
	for(int i=2;i<=n;++i)
		lg[i]=lg[i>>1]+1;
	for(int k=1;(1<<k)<=n;++k)
		for(int i=0;i+(1<<k)<=n;++i)
		{
			stmx[k][i]=max(stmx[k-1][i],stmx[k-1][i+(1<<(k-1))]);
			stmn[k][i]=min(stmn[k-1][i],stmn[k-1][i+(1<<(k-1))]);
		}
	for(int i=0,a,b;i<q;++i)
	{
		cin>>a>>b;--a;--b;
		if(a>b)
			swap(a,b);
		int k=lg[b-a+1];
		int mx=max(stmx[k][a],stmx[k][b-(1<<k)+1]);
		int mn=min(stmn[k][a],stmn[k][b-(1<<k)+1]);
		cout<<mx-mn<<'\n';
	}
	return 0;
}
```

### 代码说明

- **两张表存值不存下标**：答案只要差值，不需要定位是哪头牛；`int` 合并比结构体/下标间接访问更快更省。
- 幂等是 ST 表的命门：查询两块 $[a,a+2^k)$ 与 $[b-2^k+1,b+1)$ 最多重叠 $2^k-1$ 格，max/min 重复计入无害（引理 2）；若本题改成区间和就必须换线段树/前缀和。
- 建树边界 `i+(1<<k)<=n` 与外层 `(1<<k)<=n` 保证只建有定义的格子；引理 5(c) 保证查询读的两格一定建过。**`stmn` 未建格默认 $0<h_i$，读错不会崩只会让答案静默偏大**，这两处边界必须写对。
- `lg[1]=0` 依赖全局零初始化，恰好正确；询问长度 $\ge1$，`lg[0]` 不会被用到。
- `mxk{17}` 是松弛：$n\le5\times10^4<2^{16}$，实际最高第 $15$ 层。
- `a>b` 时 swap 是零成本保险；$a=b$ 时 $k=0$、两块同格、差 $0$，无需特判。
- 全程 0-based：读入 `--a;--b`，答案是数值不需要加回下标。