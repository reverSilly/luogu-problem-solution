# P1275 魔板 —— 题解

> 本文按"学生式发现过程"组织，共 5 部分：① 重读题目 ② 思维链（含全部引理的完整证明） ③ 模式小结 ④ 真实解法（含符号表、编号步骤、复杂度、验证状态） ⑤ 示例代码。

---

## Part 1 · 重读题目（白话版）

有一个 $n$ 行 $m$ 列的面板，每个格子里有一个小灯泡，状态二值：$0$（亮）或 $1$（暗）。给定两个面板状态 $A$ 和 $B$，允许对面板做两种操作：

1. **行翻转**：任选一行，把该行所有灯泡取反（$0\leftrightarrow1$）。
2. **列交换**：任选两列，交换它们的整体位置。

问：能否通过有限次上述操作，把 $A$ 变成 $B$？能输出 `YES`，否则输出 `NO`。

**数据规模与提示：**
- 多组数据，$k\le 5$。
- 每组 $0<n,m\le 100$。
- 标签是"搜索"，但 $n,m\le 100$ 显然**不能**真去搜（状态空间爆炸）。这是提示：要找**不变量 / 结构刻画**，把判定降成可计算的代数条件。
- $n\le 100$ 意味着一列有 $100$ 比特，塞不进 `uint64`（64 位）——这会在 Part 4 影响掩码的表示选择。

**输入的一个易错点：** 第 $x$ 行第 $y$ 列的数字为 $0$ 表示"亮"，为 $1$ 表示"暗"。题目用 $0/1$ 直接编码状态，**不需要**额外取反——直接把读入的 $0/1$ 当作灯泡值即可（取反就是异或 $1$）。

---

## Part 2 · 思维链（发现过程 + 完整证明）

### 第 0 步 · 探索操作的本质（property）

先弄清两种操作各自改变了什么、不改变什么。把面板看成一个 $n\times m$ 的 $0/1$ 矩阵 $a$，下标 $r\in[0,n)$ 是行、$c\in[0,m)$ 是列。

- **行翻转（翻转行 $r$）**：对每个列 $c$，$a[r][c]\leftarrow 1-a[r][c]$。它同时作用于**所有列**的第 $r$ 个比特。
- **列交换（交换列 $p,q$）**：对所有行 $r$，交换 $a[r][p]$ 与 $a[r][q]$。它只是**给列重新排了个顺序**。

一个立刻有用的观察：**行翻转是可以叠加的，而且顺序无关、自逆**。翻两次同一行等于没翻。所以一组行翻转操作完全可以用一个 $n$ 位向量 $s$ 描述：$s[r]=1$ 表示"第 $r$ 行最终被翻了奇数次"。把 $s$ 看作一个 $n$ 比特掩码。于是"任意次行翻转"等价于"对每一列的掩码异或上 $s$"。

> 记号预告：把**一列**编码成一个 $n$ 比特掩码。记第 $c$ 列在面板 $A$ 中的掩码为 $\text{colA}[c]$，在 $B$ 中为 $\text{colB}[c]$。掩码的第 $r$ 位 = 第 $r$ 行第 $c$ 列的灯泡值。
>
> 行翻转（翻集合 $s$）后的效果：$\text{colA}[c]\leftarrow \text{colA}[c]\oplus s$（$\oplus$ 按位异或）。
>
> 列交换的效果：把掩码数组 $\{\text{colA}[c]\}$ **整体置换**（重排顺序）。

### 第 1 步 · 把"存在操作序列"翻译成代数条件（reduce to a cleaner statement）

"能把 $A$ 变成 $B$" 等价于：存在一个行翻转掩码 $s$ 和一个列置换 $\pi$，使得对所有 $(r,j)$：

$$a[r][\pi(j)]\ \oplus\ s[r]\ =\ b[r][j].\tag{$\star$}$$

（先按 $s$ 翻行，再按 $\pi$ 排列，结果逐格等于 $B$。）

把 $(\star)$ 按**列**重新解读：固定输出列 $j$，它来自 $A$ 的第 $\pi(j)$ 列。对这一列的所有行 $r$：$\text{colA}[\pi(j)]$ 异或 $s$ 后的整列，必须等于 $\text{colB}[j]$ 整列。也就是说，$\pi$ 只是把"翻完行的 $A$ 的列"重新编号，使其逐列对上 $B$ 的列。**列的编号无关紧要，重要的是列的"多重集"**。

由此得到核心判定条件（**命题 1**）：

> **命题 1.** $A$ 能变成 $B$ $\iff$ 存在 $n$ 位掩码 $s$，使得
> $$\big\{\text{colA}[i]\oplus s:\ i\in[0,m)\big\}\ =\ \big\{\text{colB}[j]:\ j\in[0,m)\big\}\quad(\text{作为多重集}).$$

**证明（完整，双向）：**

- **充分性（$\Leftarrow$）。** 假设存在 $s$ 使上式成立。记翻行后的 $A$ 的列多重集为 $M=\{\text{colA}[i]\oplus s\}$。$M$ 与 $\{\text{colB}[j]\}$ 作为多重集相等，因此**存在**一个列的双射 $\pi$，把 $M$ 中的每个元素一一配到 $\text{colB}$ 的对应列上，即 $\text{colA}[\pi(j)]\oplus s=\text{colB}[j]$（对每个 $j$）。对任意行 $r$，取该等式的第 $r$ 位：$a[r][\pi(j)]\oplus s[r]=b[r][j]$，即 $(\star)$ 成立。于是"先翻行 $s$，再按 $\pi$ 排列"就是合法的操作序列，$A$ 变成 $B$。∎

- **必要性（$\Rightarrow$）。** 假设存在操作序列把 $A$ 变成 $B$。该行序列里行翻转的奇偶效果就是某个掩码 $s$，列交换的净效果就是某个列置换 $\pi$，故 $(\star)$ 成立。对每个 $j$，$\text{colA}[\pi(j)]\oplus s=\text{colB}[j]$。因为 $\pi$ 是 $[0,m)$ 上的双射，$\{\text{colA}[\pi(j)]:j\}$ 恰是 $\{\text{colA}[i]:i\}$ 的重排，故 $\{\text{colA}[i]\oplus s:i\}=\{\text{colB}[j]:j\}$ 作为多重集成立。∎

> 命题 1 把"存在操作序列"（看似要搜索）彻底降成了一个**纯多重集等式**的判定。接下来要解决"存在 $s$"。

### 第 2 步 · 候选掩码 $s$ 的爆炸，以及如何把它砍到 $O(m)$（关键引理）

直接找 $s$ 有 $2^n$ 种可能（$n$ 可达 $100$，$2^{100}$ 天文数字），不可行。需要**结构性地把候选砍小**。

观察多重集等式 $\{\text{colA}[i]\oplus s\}=\{\text{colB}[j]\}$ 的右端：$\text{colB}[0]$ 是右端多重集里的一个**具体元素**。若等式成立，$\text{colB}[0]$ 必须出现在左端 $\{\text{colA}[i]\oplus s\}$ 中。

> **命题 2（候选缩减）.** 设 $s$ 满足命题 1 的多重集等式。则
> $$s\ =\ \text{colA}[i]\oplus\text{colB}[0]\ \ \text{（对某个 } i\in[0,m)\text{）}.$$
> 因此**只需枚举** $s\in\{\text{colA}[c]\oplus\text{colB}[0]:\ c\in[0,m)\}$ 这**至多 $m$ 个**候选。

**证明（完整）。** 由假设，$\text{colB}[0]\in\{\text{colB}[j]:j\}=\{\text{colA}[i]\oplus s:i\}$。所以存在某个 $i\in[0,m)$ 使得 $\text{colA}[i]\oplus s=\text{colB}[0]$，两端异或 $\text{colA}[i]$ 得 $s=\text{colA}[i]\oplus\text{colB}[0]$。该 $i$ 在 $[0,m)$ 内，故 $s$ 属于枚举集合 $\{\text{colA}[c]\oplus\text{colB}[0]:c\in[0,m)\}$。∎

> 这一步是整道题的"题眼"：把 $2^n$ 的搜索空间，用"锚定一个右端元素 $\text{colB}[0]$"一刀砍到 $m$。锚点选哪一列都行，选 $\text{colB}[0]$ 只是方便（它一定在多重集里）。

**为什么 $m$ 个候选里"逐个排序比对"是正确的完备判据？** 因为命题 2 保证了"若解存在，解必在其中"；而对每一个候选 $s$，命题 1 告诉我们"当且仅当它使两个多重集相等时它才是解"。所以"枚举 $m$ 个候选、逐一判定多重集相等、任一成立即 YES"既是**完备**的（不漏解），又是**正确**的（不误报——只有真解会让多重集相等）。

### 第 3 步 · 判定"两个多重集是否相等"（classic technique：排序 + 比较）

多重集相等 ⇔ 各自排序后逐元素相等。掩码是 $n$ 比特（$n\le100$），选一个能存 $100$ 位、且自带"异或 + 比较 + 可排序"的类型：`std::bitset<128>`（$128\ge100$，且 `bitset` 支持 `^`、`==`、`<`，可直接 `sort`）。

对每个候选 $s$：
1. 构造 $\text{tmp}[i]=\text{colA}[i]\oplus s$（$i\in[0,m)$）。
2. `sort(tmp, tmp+m)`，并与预先排好序的目标 `tar`（= 排序后的 $\{\text{colB}[j]\}$）`equal` 比较。
3. 相等则该 $s$ 是解，输出 `YES` 并停止；否则继续下一候选。

枚举完仍无解，输出 `NO`。

### 第 4 步 · 用最小例子手验逻辑（sanity check，非机器验证）

- **$m=1$（只有一列）**：只能靠行翻转。对任意两列向量 $\text{colA}[0],\text{colB}[0]$，取 $s=\text{colA}[0]\oplus\text{colB}[0]$ 必使 $\text{colA}[0]\oplus s=\text{colB}[0]$，故 $m=1$ **恒为 YES**。直觉正确：只有一列时，逐行取反即可对齐。
- **$n=1$（只有一行），$A=10,\ B=01$**：列掩码（1 位）$A=[1,0],B=[0,1]$。候选 $s=\text{colA}[c]\oplus\text{colB}[0]$。取 $c=0$：$s=1\oplus0=1$，则 $\{1\oplus1,0\oplus1\}=\{0,1\}=\{0,1\}$（$B$ 的多重集）→ **YES**。正确：翻那一行 $10\to01$。
- **$A=10,\ B=11$（$n=1,m=2$）**：$A=[1,0],B=[1,1]$。候选 $s=\text{colA}[c]\oplus\text{colB}[0]$，$c=0$：$s=1\oplus1=0$，$\{1,0\}\ne\{1,1\}$；$c=1$：$s=0\oplus1=1$，$\{0,1\}\ne\{1,1\}$。均不等 → **NO**。正确：单行翻转要么整行全反要么不变，无法把 $10$ 变成 $11$（列交换只能给 $10$ 重排成 $10/01$）。

手验与逻辑一致。

---

## Part 3 · 模式小结（按真实思考顺序）

```
- explore its property        — 行翻转=所有列掩码异或同一个 s（可叠加、自逆）；列交换=列的整体置换（只改变顺序，不改多重集）。
- reduce to easier problem    — "存在操作序列" ⇒ 命题1：∃掩码 s 使 {colA_i ⊕ s} 与 {colB_j} 作为多重集相等。
- classic problem solution    — 命题2 锚点缩减：固定右端元素 colB[0]，把 2^n 候选 s 砍到 ≤ m 个（s = colA_i ⊕ colB[0]）。
- classic problem solution    — 多重集相等 ⇒ 排序 + equal 比较（bitset<128> 天然支持异或/比较/排序）。
- defer reconstruction        — 题目只问能否转化，不需要真的输出操作序列，故省去由 s 与 π 重建操作的步骤。
```

---

## Part 4 · 真实解法（符号表 + 编号步骤 + 复杂度 + 验证状态）

### 符号表（每个符号都定义清楚）

| 符号 | 含义 / 初始化 / 记忆 |
|---|---|
| `n, m` | 行数 / 列数，运行时读入，$0<n,m\le100$。 |
| `a[i][j]` | 临时二维缓冲，存当前读入面板的第 $i$ 行第 $j$ 列的灯泡值（$0$ 或 $1$）。读第一块面板填一次、构建 `colA`，读第二块再填一次、构建 `colB`。 |
| `colA[c]` | `bitset<128>`。**第 $c$ 列**在面板 $A$ 中的掩码：第 $r$ 位 $=a[r][c]$。每轮测试先 `reset()` 清零，再按行读入时 `set(r)` 置位。 |
| `colB[c]` | `bitset<128>`。第 $c$ 列在面板 $B$ 中的掩码，构建方式同 `colA`。 |
| `tar[c]` | `bitset<128>`。目标多重集：`tar[c]=colB[c]` 拷贝，随后 `sort` 一次。作为比对基准，整个测试只排序一次。 |
| `tmp[c]` | `bitset<128>`。对当前候选 $s$，`tmp[i]=colA[i]^s` 后 `sort`，再与 `tar` 比较。每个候选复用。 |
| `s` | 当前枚举的"行翻转掩码"候选，$s=\text{colA}[c]\oplus\text{colB}[0]$。 |
| `ok` | `bool`，一旦发现满足命题 1 的 $s$ 即置 `true` 并停止枚举。 |

**记忆口诀**：列当掩码（column-as-mask）；翻行 = 全列异或 $s$（flip = xor all）；锚定 $\text{colB}[0]$ 砍候选（anchor to cut candidates）；排序比多重集（sort to compare multiset）。

**表示选择说明**：$n\le100>64$，`uint64` 放不下，故用 `bitset<128>`（$128\ge100$）。`bitset` 自带 `^`（异或）、`==`、`<`，可直接参与 `std::sort` 与 `std::equal`，无需手写比较器。

### 算法步骤（编号，引用上表符号）

对每一组数据：

1. 读入 $n,m$。
2. 读 $n\times m$ 个数填入 `a[i][j]`（面板 $A$）。
3. 构建 `colA`：对每列 $c$，`colA[c].reset()`；对每个 $r$，若 $a[r][c]=1$ 则 `colA[c].set(r)`。
4. 再读 $n\times m$ 个数覆盖 `a[i][j]`（面板 $B$），同法构建 `colB`（先 `reset` 再 `set`）。
5. 拷贝 `tar[c]=colB[c]`（$c\in[0,m)$），`sort(tar,tar+m)`。
6. 置 `ok=false`。枚举候选列 $c=0..m-1$（若已 `ok` 则停）：
   - 令 $s=\text{colA}[c]\oplus\text{colB}[0]$（锚点用**未排序**的 `colB[0]`）。
   - 对每个 $i$：`tmp[i]=colA[i]^s`；`sort(tmp,tmp+m)`。
   - 若 `equal(tar,tar+m,tmp)`：`ok=true`，跳出。
7. 输出 `ok ? "YES" : "NO"`（每行一个，`'\n'`）。

### 复杂度

- 每组：读入/建列 $O(nm)$；`tar` 排序一次 $O(m\log m)$；枚举 $\le m$ 个候选，每个排序 $O(m\log m)$ + 比较 $O(m)$ → 合计 $O(m^2\log m)$。
- 主导项 $O(m^2\log m)$，$m\le100$ 时 $\approx 10^4\times7$，再乘 $k\le5$，可忽略不计。
- 空间 $O(nm + m\cdot\lceil n/8\rceil)$，由全局数组决定，常数级。

### 验证状态

- **未编译 / 未运行**（依本任务要求：仅写题解，不编译、不运行、不查编译器）。
- 逻辑正确性由 **命题 1（充要，双向证明）+ 命题 2（候选完备缩减）+ 排序判多重集** 共同保证，且已用 §Part2 第 4 步的 3 个手算小例（$m=1$ 恒 YES；$10\to01$ YES；$10\to11$ NO）核对方向无误。
- 待在有编译器环境时可补：随机 $n,m\le 8$ 与 BFS 暴力（状态 = 行翻转掩码 + 列置换，或直接用本判据的等价小状态枚举）对拍，以及 $n,m=100$ 的性能点。此处按指令留白。

---

## Part 5 · 示例代码（C++，遵循编码风格规范）

```cpp
#include<bits/stdc++.h>
using namespace std;

// https://www.luogu.com.cn/problem/P1275
// 列当掩码：翻行=全列异或s；锚定colB[0]把候选砍到<=m；排序判多重集

constexpr int mxn{105};            // n,m<=100 +5 松弛

int n,m;
int a[mxn][mxn];                    // 当前读入面板的行缓冲（复用两次）
bitset<128>colA[mxn],colB[mxn];    // 第c列掩码：bit r = a[r][c]
bitset<128>tar[mxn],tmp[mxn];      // tar=排序后的B列多重集；tmp=当前候选排序后

int main()
{
    cin.tie(nullptr);
    cout.tie(nullptr);
    ios::sync_with_stdio(false);
    int k;
    cin>>k;
    while(k--)
    {
        cin>>n>>m;
        for(int i=0;i<n;++i)
            for(int j=0;j<m;++j)
                cin>>a[i][j];
        for(int c=0;c<m;++c)
            colA[c].reset(),colB[c].reset();
        // 按列取掩码：bit r = a[r][c]
        for(int i=0;i<n;++i)
            for(int j=0;j<m;++j)
                if(a[i][j])
                    colA[j].set(i);
        for(int i=0;i<n;++i)
            for(int j=0;j<m;++j)
                cin>>a[i][j];
        for(int i=0;i<n;++i)
            for(int j=0;j<m;++j)
                if(a[i][j])
                    colB[j].set(i);
        // 目标多重集只排一次
        for(int c=0;c<m;++c)
            tar[c]=colB[c];
        sort(tar,tar+m);
        bool ok=false;
        // 命题2：s=colA[c]^colB[0] 覆盖所有可行解
        for(int c=0;c<m&&!ok;++c)
        {
            auto s=colA[c]^colB[0];
            for(int i=0;i<m;++i)
                tmp[i]=colA[i]^s;
            sort(tmp,tmp+m);
            if(equal(tar,tar+m,tmp))
                ok=true;
        }
        cout<<(ok?"YES":"NO")<<'\n';
    }
    return 0;
}
```

**代码要点对照风格规范：**
- `#include<bits/stdc++.h>` 单一头文件 + `using namespace std;`。
- 全局静态数组 + `constexpr int mxn{105}`（+5 松弛）；哨兵无关（本题无"无穷"，用 `reset()` 清零即可，避开了"全局 int 默认 0 被误当可达"的坑——这里 0 是合法掩码值，故用显式 `reset()` 而非依赖默认值语义混淆）。
- `cin.tie(nullptr); cout.tie(nullptr); ios::sync_with_stdio(false);` 固定顺序；换行用 `'\n'`。
- 内部 0-based；三目输出字符串 `ok?"YES":"NO"`。
- 命名电报式：`colA/colB`（列掩码）、`tar`（目标）、`tmp`（临时）、`ok`（可行标志）。
- Tab 缩进、运算符不加空格、`for` 头紧凑。
- 注释稀疏，只标关键不变量（列掩码含义、锚点候选、目标只排一次）。
