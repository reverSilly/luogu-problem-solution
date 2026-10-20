# P1580 yyy loves Easter_Egg I

> 标签：模拟 · 字符串 · 洛谷原创 · 难度：普及-
> 本地题面：`P1580.md`（行数 ≤5×10⁴，每行 ≤10³ 字符，输入文件 ≤4MB；官方样例 2 组）。验证：`appendix/P1580/` 官方样例 2/2 回归 + 9 组边界用例 + 5000 组随机对拍 `std==brute` + 20 组中规模（n≤3000）全部一致；最坏规模 5×10⁴ 行（3.72MB）标程耗时 **0.0064s**。

## 1. 重新读题

**问题重述（人话版）**

输入是一段群聊消息记录：每行一条消息，格式为「消息发送者 + `:` + 消息内容」（官方样例里冒号两边带空格，行尾还可能带空格）。所有人的名字都是 `yyy loves XXX` 的形式（`XXX` 是单个词，如 `OI` / `Maths` / `Chemistry` / `Physics` / `Microelectronic`），内容里保证不含 `:`，所以每行恰好一个 `:`，它就是发送者与内容的分界线。文件以一行空行结尾。

第一句话里，发送者艾特了恰好一个人 P（题面保证只艾特了一个、且不艾特自己）。从第二句起判定"队形"：

- 本句内容里**恰好有一个 `@` 字符**；并且
- 这个 `@` 后面跟的名字，与第一句话艾特的人 P **相同**。

两条都满足，队形延续；否则本句就是**第一次破坏队形**的行。输出分三种结局：

1. **轰炸成功**——被炸的人 P 在队形完好期间作为发送者发过言（"在队形被打破之前出来发言了"），或者破坏队形的正是 P 本人（"就是他打破队形了"）。输出一行 `Successful @P attempt`。
2. **队形被破坏且轰炸失败**——输出三行：`Unsuccessful @P attempt`、第一次破坏的行号（从 1 开始数）、第一次破坏队形的人的 id（完整名字）。
3. **队形从未被破坏且轰炸失败**——输出三行：`Unsuccessful @P attempt`、队形的长度（消息条数）、`Good Queue Shape`。

> 一句话版本：**目标 P 在第一句后就冻结了；之后每句只剩两个自由度——`@` 的个数和 `@` 的对象；最后看 P 有没有赶在队形破裂之前亲自吭声。**

**题面里容易漏掉的条款**（对着 Hint 逐条对账）

1. 「含有且**恰好**含有一次 `@`」数的是 `@` **字符**的个数，不是"@ 了几个不同的人"。因此 `@P @P`（反复艾特同一个人）含两个 `@`，**算破坏队形**——Hint 特意提醒「并不保证后面说话是否会反复艾特同一个人」，踩的就是这个坑。
2. 「@ 的人和上一句话一样」与「若 @ 的人与第一个人不同」两条并存。因为队形一旦保持，前面每句 @ 的都是同一个人，所以"与上一句相同"和"与第一句相同"完全等价（§2.1 引理 1）——实现时只需把 P 记下来，往后每句都与 P 比。
3. 成功条件里的「在队形被打破**之前**」是有窗口的：P 必须赶在第一次破坏（含破坏句本身）之前发言；P 在破坏行之后才说话，轰炸已经失败了（§2.3 引理 3）。
4. 「保证第一个说话的一定在 @ 某人 / 只 @ 了一个人 / 不会艾特自己」⇒ 第一句无需任何校验，直接取它 @ 的人当目标 P；且第一句的发送者必然 ≠ P。
5. 「保证每个人说的话中没有 `:`」⇒ 第一个 `:` 就是分隔符，不会错切到内容里去。
6. 「保证文件结束一定有一行空行」⇒ 读到空行即可停（同时保留 EOF 兜底更稳）。
7. 「保证大家的名字都是 `yyy loves ***` 的格式」⇒ `@` 后面的名字是三个 token（`yyy`、`loves`、`XXX`），名字后面还可以跟任意文字（`+1`、`wo pei fu ni de chu ti xiao lv` 等），取前三个 token 即为被艾特者的名字。
8. 官方样例里冒号两边有空格、行尾有空格 ⇒ 发送者必须剥掉首尾空白再比较 / 输出（样例 1 的期望输出是 `yyy loves Microelectronic`，不带尾随空格）。
9. 「此队形长达 91 行」⇒ 队形的长度按**消息条数**数，不是按参与人数。

**数据范围与规模感受**

$$\text{行数}\le 5\times10^{4},\qquad \text{每行长度}\le 10^{3},\qquad \text{输入文件}\le 4\mathrm{MB}.$$

总字符数被 4MB 卡死，意味着**一趟线性扫描**就是满分做法：每句只做"找冒号、数 `@`、切三个 token、比字符串"四件本地小事，没有任何跨句状态，连数组都不用开。1s / 125MB 的限制对本解法而言余量在两个数量级以上。

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：队形到底由什么决定

先把"队形"这条规则拆开看。它只问两件事：**本句内容里 `@` 的个数**，和**本句 @ 的人是谁**。发送者是谁、队形已经保持了多少句、句子里还写了什么废话——全都不影响"本句是否维持队形"。这个观察很重要：它意味着判定是**逐句局部**的，不存在需要跨句维护的结构。

但题面这句话「@ 的人和上一句话一样」读起来是要记"上一句 @ 了谁"，而下一句又说「若 @ 的人与第一个人不同，就算队形被打破」——到底跟上一句比还是跟第一句比？这两者其实是一回事：

**引理 1（"与上一句相同" ⟺ "与第一句相同"）**

设第 $2,\dots,i-1$ 句都维持了队形，$P$ 是第一句艾特的人。则对第 $i$ 句而言，「@ 的人与上一句相同」与「@ 的人与第一句相同」等价。

**证明**：先证一个断言：在"前 $i-1$ 句都维持队形"的前提下，第 $2,\dots,i-1$ 句 @ 的人全都是 $P$。对句号归纳：第 1 句 @ 的人就是 $P$（定义）；若第 $j$ 句（$2\le j\le i-1$）维持队形，则它 @ 的人与第 $j-1$ 句相同，由归纳假设第 $j-1$ 句 @ 的是 $P$，故第 $j$ 句 @ 的也是 $P$。断言得证。

于是第 $i$ 句的"上一句"（第 $i-1$ 句）@ 的人就是 $P$，所以

$$\text{第 }i\text{ 句@的人与上一句相同}\iff\text{第 }i\text{ 句@的人是 }P\iff\text{第 }i\text{ 句@的人与第一句相同}.$$

中间一化同时用上了"上一句 @ 的是 $P$"（断言）和"第一句 @ 的是 $P$"（定义）。$\blacksquare$

> 这条引理把"跟上一句比"这个看似要维护历史状态的规则，一步压成"跟一个首句就冻结的常量 $P$ 比"。**目标在第一句读完后就再也不变了**，这是整道题的第一把钥匙。

### 2.2 考虑简化版：先只判"队形破没破"

把"轰炸成败"整个扔掉，先解决子任务：找出第一次破坏队形的行号 $k$ 和破坏者。由引理 1，第 $i$ 句（$i\ge2$）维持队形的条件是一条干净的合取式：

$$C_i=\big(\text{内容里恰好一个 }\mathtt{'@'}\big)\ \wedge\ \big(\text{那个 }\mathtt{'@'}\text{ 后面的人名}=P\big).$$

**引理 2（破坏队形的充要条件）**

第 $i$ 句（$i\ge2$）破坏队形，当且仅当以下三条至少一条成立：

1. 内容里 `@` 的个数为 $0$；
2. 内容里 `@` 的个数 $\ge2$；
3. `@` 的个数恰为 $1$，但 `@` 后面的人名 $\ne P$。

**证明**：队形延续的条件是 $C_i$，破坏即 $\neg C_i$。由德摩根律，

$$\neg C_i=\neg\big(\text{恰好一个 }\mathtt{'@'}\big)\ \vee\ \neg\big(\text{人名}=P\big).$$

前半段"不恰好一个 `@`"按整数计数只有两种形态：$0$ 个（第 1 条）或 $\ge2$ 个（第 2 条）。后半段"人名 $\ne P$"只在能取出人名时才有意义，而名字依附于唯一的那个 `@`，故对应第 3 条。三条互斥且穷尽，合起来恰是 $\neg C_i$。$\blacksquare$

> 这条引理的用途是把实现拆成**三个可以分别写错、也必须分别写对**的检查。Hint 里「并不保证后面说话是否会反复艾特同一个人」埋伏的正是第 2 条——`@P @P` 这种句子，只比较"第一个 @ 后面的人名"会误判为延续（见 §2.4 失败 1）。

### 2.3 加回来：轰炸成功的条件

子任务解决了"破没破、在哪破"，现在把胜负判据加回来。题面的原话是：

> 若这个人在队形被打破之前出来发言了，或者就是他打破队形了，就算（油）炸成功了。

设 $k$ 为第一次破坏队形的行号（若从未破坏，记 $k=N$，$N$ 为消息总条数）。

**引理 3（轰炸成功的充要条件）**

轰炸成功，当且仅当 $P$ 作为**发送者**出现在第 $2,3,\dots,k$ 句中的至少一句里。

**证明**：

- （$\Rightarrow$）题面给的成功途径只有两条：「在队形被打破之前出来发言了」——存在 $j<k$ 使第 $j$ 句的发送者是 $P$；「就是他打破队形了」——第 $k$ 句的发送者是 $P$。两者合并即 $\exists j\in\{2,\dots,k\}$ 使 $\text{sender}_j=P$（下界从 2 起，因为题面保证第一句不艾特自己，即 $\text{sender}_1\ne P$）。从未破坏时 $k=N$，"队形被打破之前"覆盖整段记录，窗口就是 $\{2,\dots,N\}$。
- （$\Leftarrow$）若存在 $j\in\{2,\dots,k\}$ 使 $\text{sender}_j=P$。分两种情况：若 $j<k$，则第 $j$ 句发声时队形尚未被破坏（$k$ 是**第一次**破坏），属于"在队形被打破之前出来发言了"；若 $j=k$，则 $P$ 正是第 $k$ 句——即第一次破坏队形的那一句——的发送者，属于"就是他打破队形了"。两种情况都落在题面的成功定义里。$\blacksquare$

> 两个容易写错的细节，都藏在窗口 $\{2,\dots,k\}$ 的端点上：
>
> - **上界是 $k$ 不是 $N$**：$P$ 在破坏行之后才发言，轰炸已经失败（§2.4 失败 2 的反例）。
> - **$P$ 发言的那一句完全可以不破坏队形**：$P$ @ 了他自己（`@P`），恰好一个 `@`、人名又是 $P$，队形照常延续，但轰炸照样成功（§2.4 失败 3 的反例）。所以"队形延不延续"和"轰炸成不成功"是**两个独立判定**，代码里必须是两条分开的分支。

### 2.4 剖析几个失败思路（都给出具体反例）

**失败 1：只比较"第一个 `@` 后面的人名"，不数 `@` 的个数。**
反例：
```
yyy loves OI : @yyy loves Physics
yyy loves Maths : @yyy loves Physics @yyy loves Physics
```
按失败思路：第 2 句第一个 `@` 后面是 $P$，队形延续，最终输出 `Good Queue Shape`。正确答案：第 2 句含**两个** `@`（引理 2 第 2 条），第一次破坏行号为 2，应输出
```
Unsuccessful @yyy loves Physics attempt
2
yyy loves Maths
```
Hint 里「并不保证后面说话是否会反复艾特同一个人」就是冲这个坑写的。⇒ **必须先数 `@` 再比名字**，引理 2 的三条缺一不可。

**失败 2：扫完整个文件再判断"$P$ 有没有发言"，窗口取成了 $\{2,\dots,N\}$。**
反例：
```
yyy loves OI : @yyy loves Physics
yyy loves Maths : @yyy loves Physics
yyy loves Chemistry : ni men wu liao me
yyy loves Physics : ni men gou le
```
按失败思路：$P$ 在第 4 句发言了 → `Successful @yyy loves Physics attempt`。正确答案：队形在第 3 句就破了（零个 `@`），$P$ 说得太晚，应输出
```
Unsuccessful @yyy loves Physics attempt
3
yyy loves Chemistry
```
⇒ 成功窗口的上界是 $k$（引理 3），$P$ 的发言必须赶在第一次破坏**之前或恰好那一刻**。

**失败 3：把"队形延不延续"和"轰炸成不成功"搅成一个判定。**
两种典型搅法都死在同一个反例上：
```
yyy loves OI : @yyy loves Physics
yyy loves Maths : @yyy loves Physics
yyy loves Physics : @yyy loves Physics
yyy loves OI : @yyy loves Physics
```
第 3 句 $P$ @ 了他自己：恰好一个 `@`、人名 $=P$，队形**延续**；同时 $P$ **发言了**，轰炸**成功**。若代码认为"$P$ 一发言队形就算破"（搅法 A），会错误输出 `Unsuccessful @yyy loves Physics attempt / 3 / yyy loves Physics`；若代码认为"只有队形破了才谈得上成败"（搅法 B），会错误输出 `Unsuccessful ... Good Queue Shape`。正确答案只有一个：`Successful @yyy loves Physics attempt`。⇒ 引理 2 与引理 3 是两个独立条件，必须分两条分支判。

**失败 4：用 `cin>>s` 按单词读入。**
反例（内容里出现了别人名字的句式）：
```
yyy loves OI : @yyy loves Physics
yyy loves Maths : @yyy loves Physics yyy loves Chemistry
yyy loves OI : @yyy loves Physics
```
按单词读时"yyy"不再只出现在发送者位置，第 2 句的**内容**里也有，于是"一句一行"的边界被悄悄打乱：行数、发送者、`@` 的归属全部可能错位。本例正确答案是队形从未破坏、长度 3：
```
Unsuccessful @yyy loves Physics attempt
3
Good Queue Shape
```
社区里确有按单词读的 AC 代码，但它们额外依赖"每行必定以 `yyy loves` 开头"这种脆弱假设，且对行内空格、行尾空格、结尾空行全都不设防。⇒ **必须整行 `getline`**，行的边界只能由换行符定义。

**失败 5：发送者不剥首尾空白。**
反例就是官方样例 1 的第 4 行 `yyy loves Microelectronic : ni men wu liao me `（冒号前有空格）。直接 `substr` 到 `:` 会得到带尾随空格的 `"yyy loves Microelectronic "`，输出与官方答案 `yyy loves Microelectronic` 逐字节不符。⇒ 发送者输出 / 比较前必须 `trim`。

**失败 6：行尾 `\r` 不处理。**
反例：把官方样例 1 改成 CRLF 换行，`getline` 读到的每行末尾都挂着 `\r`，名字 token 变成 `"Physics\r"`，于是第 2 句就被误判成"@ 了别人"，输出错误的行号 2。评测机的输入文件历史上确实出现过 `\r\n` 换行。⇒ 读入后先剥掉行尾 `\r` 再解析。

### 2.5 归约到经典技术：一趟线性扫描

把引理 1—3 合起来看，整个判定只需要三个东西：冻结的目标 $P$、消息计数器 $n$、当前行。没有任何跨句状态，于是标准技术就是最朴素的**逐行模拟 + 提前退出**：

- 边读边判，读到空行停；
- 一旦某句满足引理 2 的破坏条件，立刻输出行号与破坏者并退出；
- 一旦某句的发送者就是 $P$，立刻输出 `Successful` 并退出（此刻必然还没破坏过——否则早就退出了——所以流式写法天然满足引理 3 的窗口 $\{2,\dots,k\}$）；
- 只有"从未破坏且 $P$ 从不发言"才会扫完整个文件，此时输出队形长度（= 消息条数）与 `Good Queue Shape`。

**复杂度**：每句 $O(\text{句长})$，总计 $O(\text{总输入长度})\le O(4\mathrm{MB})$ 个字符操作；额外空间 $O(1)$（标程流式处理，连存行的数组都不需要）。实测最坏规模（$5\times10^4$ 行、3.72MB、全部合法队形）耗时 0.0064s。

### 2.6 越界与状态体检（写代码前的最后一次停顿）

| 量 | 范围 | 上界 | `int` 够？ |
|---|---|---|---|
| 行数 $N$ | 题面给定 | $5\times10^4$ | 是 |
| 行长 | 题面给定 | $10^3$ | 是 |
| 行号 / 队形长度 | $[1,N]$ | $5\times10^4$ | 是 |
| `@` 个数 | $[0,10^3]$ | $10^3$ | 是 |

全程只有字符串操作，没有任何算术，不存在溢出。真正要防的是四类**静默错误**（不崩溃、只是答案错，最难查）：

1. `@` 的个数漏数（引理 2 第 2 条，`@P @P` 形态）；
2. 成功窗口上界取成 $N$ 而不是 $k$（引理 3）；
3. 发送者尾随空格没剥（样例 1 直接给出反例）；
4. 行尾 `\r` 没剥（CRLF 数据下每句都误判）。

另外确认一个语义点：**队形长度 = 消息条数**（不含结尾空行）。Hint 说"此队形长达 91 行"——按行数、不按参与人数（91 句里只有 3 个轰炸者）；若理解成"去重人数"会输出 3，与题意相悖。

## 3. 模式总结（Pattern Summary）

- **explore its property** — 队形只取决于"本句 `@` 的个数 + `@` 的人"，与发送者、历史句全部无关（§2.1）。
- **reduce to easier problem** — 用等关系的传递性把"与上一句相同"归约成"与首句冻结的常量 $P$ 相同"（引理 1）。
- **consider easier version** — 先剥掉胜负、只判"破没破、在哪破"，得到破坏的三条充要条件（引理 2）。
- **consider easier version（加回增量）** — 再把成功条件加回来，发现它是独立的窗口判定 $\{2,\dots,k\}$（引理 3）。
- **separate two verdicts** — "队形延续"与"轰炸成功"必须分两条分支判，$P$ 合法发言时二者同时成立（§2.4 失败 3）。
- **classic problem solution** — 无跨句状态 ⇒ 逐行 `getline` 线性模拟 + 提前退出，$O(\text{总输入长度})$（§2.5）。
- **boundary-case hardening** — 数 `@` 字符而非比人名 / 窗口上界是 $k$ 非 $N$ / 发送者 `trim` / 行尾 `\r`（§2.6 四条静默错误）。
- **verify against an independent brute** — "先求 $k$ 再扫窗口"的两相位暴力与流式标程对拍，5000 组随机全一致（§4.5）。

## 4. 真实解法

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 | 助记 / 来源 |
|---|---|---|---|---|
| `tgt` | `string`（全局） | 被轰炸的目标 $P$ = 第一句 @ 的人 | 读入第一句后写入，此后不再变 | target，引理 1 的冻结常量 |
| `line` | `string`（`main` 局部） | 当前读入的一行（已剥行尾 `\r`） | 每行覆盖 | 原始消息行 |
| `n` | `int` | 已读入的消息条数；同时充当 1-based 行号与队形长度 | `0`，每句 `++n` | 行号 / 队形长度 |
| `c` | `int`（局部） | 本行第一个 `:` 的下标 | `line.find(':')` | colon，内容保证无 `:` |
| `sdr` | `string`（局部） | 本句发送者（`trim` 后） | `trim(line.substr(0,c))` | sender |
| `ctx` | `string`（局部） | 本句消息内容 | `line.substr(c+1)` | context |
| `at` | `size_t`（局部） | 本句内容里第一个 `@` 的下标 | `ctx.find('@')` | at |
| `brk` | `bool`（局部） | 本句是否破坏队形（引理 2 三条合一） | — | break |
| `trim` | 函数 | 剥掉字符串首尾空白 | — | 样例 1 的尾随空格陷阱 |
| `atname` | 函数 | 取 `@` 之后前 3 个 token 作为被艾特者名字 | — | 名字格式 `yyy loves XXX` |

### 4.2 编号步骤

1. I/O 加速三连（`cin.tie` / `cout.tie` / `sync_with_stdio`，顺序固定）；`n=0`。
2. `getline` 循环：先剥掉行尾所有 `\r`；若行为空则 `break`（题面保证的文件结尾，EOF 亦可自然退出循环）；否则 `++n`。
3. 取第一个 `:`，切成发送者 `sdr`（`trim`）与内容 `ctx`。
4. 若 `n==1`：取 `ctx` 中第一个 `@` 之后的 `atname` 写入 `tgt`（题面保证第一句恰一个 `@` 且不艾特自己，无需校验），`continue`。
5. 若 `sdr==tgt`：输出 `Successful @tgt attempt` 并 `return 0`。（此刻必然还未破坏过——否则第 7 步早已返回——故流式写法天然落在引理 3 的窗口内。）
6. 计算 `brk`：`ctx` 里没有 `@`，或 `@` 不少于两个，或唯一 `@` 的 `atname != tgt`（引理 2）。
7. 若 `brk`：输出 `Unsuccessful @tgt attempt`、`n`（1-based 行号）、`sdr`，`return 0`。
8. 循环自然结束（从未破坏且 $P$ 从未发言）：输出 `Unsuccessful @tgt attempt`、`n`（队形长度 = 消息条数）、`Good Queue Shape`。

### 4.3 复杂度

- 时间：每句 $O(\text{句长})$，总计 $O(\text{总输入长度})\le O(4\,\mathrm{MB})$；实测最坏规模 $5\times10^4$ 行 / 3.72MB → **0.0064s**（限制 1s）。
- 空间：标程 $O(1)$ 额外空间（流式、提前退出）；暴力版 $O(\text{总输入长度})$ 存全部行。

### 4.4 关键不变量（值得写 `assert` 的地方）

1. 进入第 $i$ 句循环体时，前 $i-1$ 句都维持了队形（否则早已 `return`）⇒ "上一句 @ 的人" $=$ "第一句 @ 的人" $=$ `tgt`（引理 1）。
2. 第 5 步能到达 ⇒ 队形从未破过 ⇒ 该句发送者 $=P$ 必落在窗口 $\{2,\dots,k\}$ 内（引理 3）。
3. 第 7 步输出的 `n` 是第一次破坏句的 1-based 序号，`sdr` 是该句 trim 后的发送者。
4. 第 8 步输出的队形长度 = 读入的消息条数（不含结尾空行）。

### 4.5 验证状态小结

| 项目 | 状态 |
|---|---|
| 官方样例回归 | **通过**：样例 1/2 标程与暴力均逐字节命中官方输出 |
| 边界用例 | **通过**：9/9——单句、目标合法发言（@ 自己）、目标破坏后才发言、目标即破坏者、重复艾特同一人、91 行长队形、CRLF 行尾、无结尾空行、名字粘连 |
| 随机对拍 | **通过**：5000 组随机数据全部 `std==brute`（名字池 5 基础 + 0~3 个路人；0/1/2/3 个 `@`；同名 / 异名；尾随空格；队形保持概率 0.2~0.98；$n\le60$） |
| 中规模对拍 | **通过**：20 组 $n\le3000$ 全部一致 |
| 最坏规模计时 | $5\times10^4$ 行、3.72MB、全部合法队形且目标不发言 → 输出队形长度 50000，标程耗时 **0.0064s** |
| 残余风险 | ① 若官方数据出现"名字后缀含空格"（违反 `yyy loves ***` 单词惯例），3-token 取名会偏差——按题面保证不会；② "队形长度"按消息条数计，依据是 Hint「此队形长达 91 行」与社区 AC 实现的一致行为；③ CRLF 已防御性剥离，LF 数据不受影响 |

## 5. 示例代码

> 与 `appendix/P1580/P1580.cpp` **逐字节一致**。

```cpp
//
// 题目：P1580 yyy loves Easter_Egg I
// https://www.luogu.com.cn/problem/P1580
// 本地验证:官方样例 2/2 回归 + 5000 组随机对拍 std==brute + 4MB 最坏规模计时(见 P1580_check.py)
#include<bits/stdc++.h>
using namespace std;

string tgt;                    // 被轰炸的目标:第一句话艾特的人(形如 "yyy loves XXX")

string trim(const string&s)    // 剥首尾空白:样例里 "yyy loves OI : ..." 的发送者带尾随空格,输出前必须剥掉
{
	int l=0,r=(int)s.size()-1;
	while(l<=r&&isspace((unsigned char)s[l]))++l;
	while(r>=l&&isspace((unsigned char)s[r]))--r;
	return s.substr(l,r-l+1);
}

// 取 s[at]=='@' 之后被艾特者的名字:名字格式为 "yyy loves XXX",按空白切出前 3 个 token
string atname(const string&s,int at)
{
	string res;
	for(int i=at+1,c=0;i<(int)s.size()&&c<3;)
	{
		while(i<(int)s.size()&&s[i]==' ')++i;
		if(i>=(int)s.size())break;
		int j=i;
		while(j<(int)s.size()&&s[j]!=' ')++j;
		if(c)res+=' ';
		res+=s.substr(i,j-i);
		++c;
		i=j;
	}
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string line;
	int n=0;                    // 已读入的消息条数(=队形长度候选)
	while(getline(cin,line))
	{
		while(!line.empty()&&line.back()=='\r')line.pop_back();   // 防 CRLF 行尾
		if(line.empty())break;  // 题面保证:文件以一行空行结尾
		++n;
		int c=line.find(':');
		string sdr=trim(line.substr(0,c));        // 发送者(内容保证无 ':',第一个 ':' 即分隔符)
		string ctx=line.substr(c+1);              // 消息内容
		if(n==1)
		{
			tgt=atname(ctx,(int)ctx.find('@'));  // 题面保证第一句恰一个 @ 且不艾特自己
			continue;
		}
		if(sdr==tgt)             // 被炸的人亲自发言(队形仍完好)→轰炸成功
		{
			cout<<"Successful @"<<tgt<<" attempt\n";
			return 0;
		}
		size_t at=ctx.find('@');
		bool brk=(at==string::npos)||(ctx.find('@',at+1)!=string::npos)||(atname(ctx,(int)at)!=tgt);
		if(brk)                  // 本句第一次破坏队形:出行号与破坏者 id
		{
			cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<n<<'\n'<<sdr<<'\n';
			return 0;
		}
	}
	cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<n<<"\nGood Queue Shape\n";
	return 0;
}
```

**代码结构对照**：`tgt` 是引理 1 的冻结常量；`trim` / `atname` 两个助手分别对应 §2.6 的静默错误 3 与 Hint 第 7 条（名字格式）；`brk` 一行就是引理 2 的三条合一（`at==npos` → 零个 `@`；再 `find` 到第二个 `@` → 不少于两个；`atname!=tgt` → @ 了别人）；第 5 步的 `sdr==tgt` 判定必须放在破坏判定**之前**，这样"目标正是破坏者"才会判成功（引理 3 的 $j=k$ 端点）。

### 5.2 对照实现：两相位暴力基准（仅用于对拍）

> 与 `appendix/P1580/P1580_brute.cpp` **逐字节一致**。

```cpp
//
// P1580 暴力基准:把全部消息存下来,先找出第一处破队形的行号,再单独扫"被炸的人是否发过言"
// 与标程(边读边判、提前退出)是两条独立代码路径,小规模下输出必须逐字节一致。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};      // 行数 <=5e4 (+5 松弛)

string ln[mxn];                // 原始消息行(不含结尾空行)
string sdr[mxn];               // 每句发送者(剥首尾空白后)
int nat[mxn];                  // 每句内容里 '@' 的个数(>=2 统一记 2)
string nm[mxn];                // 恰有一个 '@' 时被艾特者的名字

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string line;
	int n=0;
	while(getline(cin,line))
	{
		while(!line.empty()&&line.back()=='\r')line.pop_back();
		if(line.empty())break;
		ln[n++]=line;
	}
	// 第一遍:逐句手工解析(不用 string::find/trim,全部下标循环独立实现一遍)
	for(int i=0;i<n;++i)
	{
		int c=-1;
		for(int j=0;j<(int)ln[i].size();++j)
			if(ln[i][j]==':')
			{
				c=j;
				break;
			}
		int a=0,b=(c<0?(int)ln[i].size():c)-1;   // 发送者=第一个 ':' 之前的那段
		while(a<=b&&isspace((unsigned char)ln[i][a]))++a;
		while(b>=a&&isspace((unsigned char)ln[i][b]))--b;
		sdr[i]=ln[i].substr(a,b-a+1);
		int at=-1,cnt=0;
		for(int j=c+1;j<(int)ln[i].size();++j)
			if(ln[i][j]=='@')
			{
				++cnt;
				if(at<0)at=j;
			}
		nat[i]=min(cnt,2);
		if(cnt==1)              // 恰一个 @:按空白切前 3 个 token 当名字
		{
			string res;
			for(int j=at+1,t=0;j<(int)ln[i].size()&&t<3;)
			{
				while(j<(int)ln[i].size()&&ln[i][j]==' ')++j;
				if(j>=(int)ln[i].size())break;
				int k=j;
				while(k<(int)ln[i].size()&&ln[i][k]!=' ')++k;
				if(t)res+=' ';
				res+=ln[i].substr(j,k-j);
				++t;
				j=k;
			}
			nm[i]=res;
		}
	}
	string tgt=nm[0];           // 题面保证第一句恰艾特一人
	int k=0;                    // 第一次破队形的行号(1-based),0 表示从未破
	for(int i=1;i<n;++i)
		if(nat[i]!=1||nm[i]!=tgt)
		{
			k=i+1;
			break;
		}
	bool win=false;            // 被炸的人是否在队形完好期间(第 2..k 句)发过言
	for(int i=1,last=(k?k:n);i<last;++i)
		if(sdr[i]==tgt)
		{
			win=true;
			break;
		}
	if(win)
		cout<<"Successful @"<<tgt<<" attempt\n";
	else if(k)
		cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<k<<'\n'<<sdr[k-1]<<'\n';
	else
		cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<n<<"\nGood Queue Shape\n";
	return 0;
}
```

两版输出必须逐字节相同（已用 5000 组随机 + 20 组中规模确认）。暴力版的意义在于它走了**另一条代码路径**：先把全部消息存下来，第一遍手工解析出每句的发送者 / `@` 个数 / 被艾特者，第二遍先求第一次破坏的行号 $k$，第三遍才单独扫窗口 $\{2,\dots,k\}$ 判断目标是否发言；而标程是流式边读边判、提前退出。行号端元（$k$ 的归属、窗口上下界）、发送者 trim、`@` 计数这些地方只要有一边写错，对拍立刻暴露——实际开发中它确实抓到了暴力版"发送者忘截到冒号之前"的一处 bug。

### 5.3 易错点清单

- **不数 `@` 的个数** ⇒ `@P @P` 被误判为延续（引理 2 第 2 条，Hint「反复艾特同一个人」的伏笔）。
- **成功窗口取 $\{2,\dots,N\}$** ⇒ 目标在破坏行之后发言被误判为成功（引理 3）。
- **把两个判定搅成一个** ⇒ 目标合法发言（@ 自己）时队形延续且应成功，会被错输出成 `Unsuccessful`（§2.4 失败 3）。
- **用 `cin>>` 按单词读** ⇒ 内容里出现 `yyy loves ...` 句式时行的边界被打乱（§2.4 失败 4）。
- **发送者不 `trim`** ⇒ 输出多一个尾随空格，与官方样例逐字节不符。
- **行尾 `\r` 不剥** ⇒ CRLF 数据下名字变成 `Physics\r`，每句误判破坏。
- **队形长度数成参与人数** ⇒ 91 行的队形会输出 3；长度 = 消息条数（Hint「长达 91 行」）。
- **读到空行不停 / 不判 EOF** ⇒ 题面保证有空行结尾，读到即停；同时保留 EOF 兜底。
- **`cout<<...<<endl;`** 违反本项目风格指南，一律用 `'\n'`。
