# P2580 于是他错误的点名开始了 — 题解

## 一、解题过程：人类思考叙事

**题意重述。** 先给 $n$ 个**互不相同**的学生名字（只含小写字母，长度 $\le50$，$n\le10^4$）。再给 $m$ 个教练报的名字（$m\le10^5$，同样只含小写字母、长度 $\le50$）。对每个报的名字输出一行：

- 不在名单里 → `WRONG`；
- 在名单里且**第一次**被报 → `OK`；
- 在名单里但**之前已经报过** → `REPEAT`。

**第一步：探清这题到底要维护什么。** 输出有三种，背后其实是**两个独立的判定**：(1) 这个名字是不是名单里的合法名字（成员查询）；(2) 如果是，它之前有没有被报过（首次出现标记）。`WRONG` 只看 (1)；`OK`/`REPEAT` 在 (1) 为真的前提下看 (2)。所以核心数据结构要支持"插入名字、查询名字是否存在、查询/更新某名字是否已访问"。这是一个**字典（集合）+ 每元素一个访问标记**的问题。

**第二步：先估暴力，确认必须上数据结构。** 最朴素：每个报的名字去 $n$ 个名单里逐个字符串比较。复杂度 $O(n\cdot m\cdot L)=10^4\times10^5\times50=5\times10^{10}$ 次字符比较，妥妥 TLE。哪怕先把名单排序再二分，也要 $O(m\log n\cdot L)=10^5\times14\times50\approx7\times10^7$，能过但比较常数和字符串拷贝都不小。题目标签写着"字典树 Trie、哈希、STL"，方向明确：用 $O(L)$ 单次操作的字典。

**第三步：在 STL 与 Trie 之间选。** `unordered_set<string>` 配一个"已访问"集合最省事，但题面特意标注"新增一组 Hack 数据"——这类 hack 通常冲着**自然溢出哈希**（如 BKDR 用 `unsigned long long` 不取模）去的，构造大量同哈希串把它卡成 $O(n)$ 链。`std::map`（红黑树）确定性 $O(\log n)$ 不怕 hack，但每次比较要扫字符串。**Trie（字典树）** 是这道题的招牌解法：单次操作严格 $O(L)$、完全确定性、对任何 hack 免疫，而且天然共享前缀省空间。我选 Trie。

**第四步：Trie 怎么设计才不漏判。** 把 $n$ 个名字插进 Trie，在每个名字的**结尾节点**打标记 `ed=true`。查询一个报的名字时沿字符走：

- 中途某字符没有对应儿子 → 这个名字连"某个名单名字的前缀"都不是 → `WRONG`；
- 走完了但结尾节点 `ed=false` → 它只是某个更长名字的**前缀**，本身不是名单名字 → `WRONG`；
- 走完且 `ed=true` → 是合法名字，再看 `vis`：没访问过 → `OK` 并置 `vis=true`；访问过 → `REPEAT`。

**这里是我差点踩的坑**：不能只判"路径走通了"就认为名字正确。比如名单只有 `abc`，报 `ab`——`a`、`b` 节点都存在（因为 `abc` 经过它们），路径走通，但 `ab` 不是名单名字！必须靠 `ed` 标记区分"完整名字"与"仅仅是前缀"。同理报 `abcd` 会在 `c` 之后找不到 `d` 儿子而 `WRONG`。

**第五步：为什么 `vis` 打在节点上就等于打在名字上。** 名单名字互不相同，而 Trie 里"根到某节点的路径"唯一拼出一个字符串。两个不同名字不可能停在同一个节点（否则它们拼出的串相同，矛盾）。所以**每个名字 ↔ 唯一的结尾节点**，在节点上记 `vis` 与在名字上记 `vis` 完全等价。`WRONG` 的查询不碰任何 `vis`，不会污染状态。

**第六步：空间估算。** 节点数上界 = 所有名字长度之和 $\le n\times50=5\times10^5$（最坏无公共前缀）。每个节点 $26$ 个儿子指针（`int`），$5\times10^5\times26\times4\approx52$ MB，加两个 `bool` 数组约 $1$ MB，总计约 $50$ MB，在常见 $128$ MB 限制内。

**手造数据核对**（题面无样例输入输出）：

第一组，名单 `alice bob carol`，报 `alice bob alice dave bob`：
```
OK       ← alice 首次
OK       ← bob 首次
REPEAT   ← alice 第二次
WRONG    ← dave 不在名单
REPEAT   ← bob 第二次
```

第二组专打"前缀陷阱"，名单只有 `abc`，报 `ab abc abcd a`：
```
WRONG    ← ab 只是前缀，ed=false
OK       ← abc 完整名字
WRONG    ← abcd 走不通（c 之后无 d）
WRONG    ← a 只是前缀
```
两组把三种输出与前缀边界全覆盖，程序输出与手算一致。

---

## 二、思维链：完整证明

**记号。** 名单名字集合 $W=\{w_1,\dots,w_n\}$，互不相同，均为小写字母串、长度 $\le50$。Trie 根为节点 $0$，`ch[u][c]` 为节点 $u$ 沿字符 $c$ 的儿子编号（$0$ 表示空），`ed[u]` 标记 $u$ 是否为某名字结尾，`vis[u]` 标记该名字是否已被报过。插入后总节点数 `tot`。对串 $x$，记 $\mathrm{node}(x)$ 为从根沿 $x$ 各字符走到的节点（若中途断则记为 $\bot$）。

---

### 引理 1（路径存在 ⇔ 是某名单名字的前缀）

对任意查询串 $x$，$\mathrm{node}(x)\ne\bot$ $\iff$ $x$ 是某个 $w_i$ 的前缀（含 $x=w_i$）。且此时 $\mathrm{node}(x)$ 唯一。

*证明*：Trie 的边只在插入名字时创建：插入 $w_i$ 时，对 $w_i$ 的每个前缀 $w_i[0..k)$ 沿字符建/走一条边。故节点 $u\ne0$ 存在 $\iff$ 存在某 $w_i$ 与某 $k\ge1$ 使 $u$ 是 $w_i[0..k)$ 对应的节点；根 $0$ 对应空前缀。

（$\Rightarrow$）若 $\mathrm{node}(x)\ne\bot$，说明从根沿 $x$ 的每条边都存在，由上述每条边都属于某名字的插入路径，故 $x$ 是某 $w_i$ 的前缀。

（$\Leftarrow$）若 $x$ 是 $w_i$ 的前缀，插入 $w_i$ 时已沿 $x$ 建好全部边，故走 $x$ 不会断，$\mathrm{node}(x)\ne\bot$。

唯一性：Trie 中每个节点的儿子按字符区分（`ch[u][c]` 对固定 $u,c$ 至多一个），故从根沿确定串 $x$ 的路径唯一，$\mathrm{node}(x)$ 唯一。证毕。

---

### 引理 2（成员判定：走通且 ed 为真 ⇔ 是名单名字）

查询串 $x$ 是名单名字（$x\in W$）$\iff$ $\mathrm{node}(x)\ne\bot$ 且 `ed[`$\mathrm{node}(x)$`]`$=$`true`。

*证明*：插入阶段对每个 $w_i$ 置 `ed[`$\mathrm{node}(w_i)$`]`$=$`true`，且仅置这些节点（不同名字结尾节点不同，见引理 3）。故 `ed[u]=true` $\iff$ $u=\mathrm{node}(w_i)$ 对某 $i$。

（$\Rightarrow$）若 $x=w_i\in W$，则 $\mathrm{node}(x)=\mathrm{node}(w_i)\ne\bot$（引理 1，$w_i$ 是自身前缀）且 `ed` 为真。

（$\Leftarrow$）若 $\mathrm{node}(x)=u\ne\bot$ 且 `ed[u]=true`，则 $u=\mathrm{node}(w_i)$ 对某 $i$。从根到 $u$ 的路径既拼出 $x$（因 $u=\mathrm{node}(x)$）又拼出 $w_i$（因 $u=\mathrm{node}(w_i)$），而路径拼出的串唯一，故 $x=w_i\in W$。证毕。

**推论（前缀陷阱被正确处理）**：若 $x$ 是某名字的真前缀但 $x\notin W$，则 $\mathrm{node}(x)\ne\bot$ 但 `ed` 为假，判 `WRONG`；若 $x$ 连前缀都不是，$\mathrm{node}(x)=\bot$，判 `WRONG`。两种非法情形都被排除，不会误判 `OK`。

---

### 引理 3（不同名字 ↔ 不同结尾节点，故 vis 打在节点上合法）

$w_i\ne w_j\Rightarrow\mathrm{node}(w_i)\ne\mathrm{node}(w_j)$。因此每个名字对应唯一结尾节点，`vis[u]` 等价于"名字 $u$ 是否已报过"。

*证明*：反证。若 $\mathrm{node}(w_i)=\mathrm{node}(w_j)=u$，则根到 $u$ 的路径同时拼出 $w_i$ 与 $w_j$。但 Trie 中根到任一节点的路径唯一、拼出的串唯一（引理 1 的唯一性），故 $w_i=w_j$，与互不相同矛盾。所以不同名字结尾节点不同。

于是"名字"与"其结尾节点"一一对应，在节点上维护 `vis` 与在名字上维护完全等价：某名字首次被报 $\iff$ 其结尾节点 `vis` 首次由假置真。证毕。

---

### 引理 4（三态输出正确）

对每个查询 $x$，程序输出符合题意：$x\notin W$ → `WRONG`；$x\in W$ 且此前未报过 → `OK`；$x\in W$ 且此前报过 → `REPEAT`。

*证明*：程序逻辑：走 $x$，若断或 `ed` 假 → `WRONG`；否则若 `vis` 假 → 置 `vis` 真、输出 `OK`；否则输出 `REPEAT`。

- 由引理 2，"断或 `ed` 假" $\iff x\notin W$，输出 `WRONG` 正确，且**不修改任何 `vis`**（不影响后续判定）。
- 若 $x\in W$，设其结尾节点 $u$。由引理 3，$u$ 唯一对应 $x$。`vis[u]` 的语义是"$x$ 是否已被报过并输出过 `OK`"。归纳于查询序列：初始所有 `vis` 假（全局默认）。当 $x$ 首次到达合法分支时 `vis[u]` 假 → 输出 `OK` 并置真；此后任何对 $x$ 的查询 `vis[u]` 真 → 输出 `REPEAT`。对别的名字 $y\ne x$，其节点 $v\ne u$，`vis[v]` 不受影响。故每个名字的"首次 OK、之后 REPEAT"独立成立。证毕。

---

### 引理 5（节点数上界与数组尺寸）

插入 $n$ 个长度 $\le50$ 的名字后，`tot`$\le\sum_i|w_i|\le n\times50=5\times10^5$。取 `mxnode=500005` 安全。

*证明*：每插入一个名字 $w_i$，至多新建 $|w_i|$ 个节点（每个字符一个；若前缀已存在则复用，新建数 $\le|w_i|$）。故总节点数 $\le\sum_i|w_i|\le n\times50=10^4\times50=5\times10^5$。根节点 $0$ 不计入 `tot`（`tot` 从 $0$ 起、每新建一个 `++tot`），节点编号 $0..\text{tot}\le500000$。`ch`、`ed`、`vis` 第一维 `mxnode=500005>500000`，不越界。证毕。

---

### 引理 6（复杂度）

时间 $O((n+m)\cdot L)$，$L=50$；空间 $O(n\cdot L\cdot26)$。

*证明*：插入每个名字走 $|w_i|\le L$ 步，每步 $O(1)$（数组访问 + 可能新建节点），共 $O(nL)$。查询每个名字走 $\le L$ 步，每步 $O(1)$，共 $O(mL)$。合计 $O((n+m)L)=(10^4+10^5)\times50=5.5\times10^6$ 步，远低于时限。

空间：`ch` 为 `int[mxnode][26]`$\approx5\times10^5\times26\times4\approx52$ MB，`ed`、`vis` 各 `bool[mxnode]`$\approx0.5$ MB，合计约 $50$ MB，即 $O(nL\cdot26)$。证毕。

**确定性**：Trie 每步是数组下标访问，无哈希、无随机、无平衡树旋转，最坏复杂度即上述上界，对"同哈希碰撞"类 hack 完全免疫。

---

### 定理

程序对每个报的名字正确输出 `OK`/`WRONG`/`REPEAT`，时间 $O((n+m)L)$、空间约 $50$ MB，且不受哈希 hack 影响。

*证明*：引理 1 保证路径存在性等价于前缀关系；引理 2 保证成员判定（含前缀陷阱）正确；引理 3 保证节点与名字一一对应、`vis` 语义合法；引理 4 保证三态输出与首次/重复逻辑正确；引理 5 保证数组不越界；引理 6 给出复杂度与确定性。证毕。

---

## 三、模式总结（按实际思考顺序）

- **explore its property** — 三态输出 = 两个独立判定：成员查询（是否名单名字）+ 首次出现标记（是否已报过）。
- **consider easier version** — 暴力逐个比较 $O(nmL)=5\times10^{10}$ TLE；排序二分 $O(mL\log n)$ 能过但常数大 ⇒ 需要 $O(L)$ 字典。
- **reduce to easier problem** — 归约为"集合 + 每元素访问标记"；在 STL/Trie 间选 Trie：确定性 $O(L)$、共享前缀、对 anti-hash hack 免疫。
- **classic problem solution** — 字典树：插入打 `ed` 结尾标记，查询沿字符走，`ed`+`vis` 双标志决定 OK/WRONG/REPEAT。
- **explore its property** — 走通 ≠ 是名字：真前缀也走通但 `ed=false`，必须靠 `ed` 区分（前缀陷阱）。
- **discuss by category** — 名字互不相同 ⇒ 结尾节点互不相同 ⇒ `vis` 打在节点上等价于打在名字上；WRONG 不碰 vis；节点上界 $nL$ 定数组尺寸。

---

## 四、解法说明

### 符号表

| 符号 | 含义 | 类型 / 初值 | 记忆点 |
|---|---|---|---|
| `n` | 名单人数 $\le10^4$ | `int`，读入 | 名字互不相同 |
| `m` | 报名字个数 $\le10^5$ | `int`，读入 | 逐个判定 |
| `mxnode` | 节点上界 $n\times50+5$ | `constexpr int{500005}` | 最坏无公共前缀 |
| `ch[u][c]` | 节点 $u$ 沿字符 $c$ 的儿子编号 | `int[mxnode][26]`，全局默认 $0$ | **$0$ 表示空儿子**，根也是 $0$ |
| `tot` | 已新建节点数 | `int`，初值 $0$ | 新节点 `++tot`，编号 $1..tot$ |
| `ed[u]` | $u$ 是否某名单名字结尾 | `bool[mxnode]`，默认 `false` | 插入时置真；区分名字 vs 前缀 |
| `vis[u]` | 该名字是否已报过 | `bool[mxnode]`，默认 `false` | 首次 OK 时置真 |
| `u` | 当前走到的节点 | `int`，每次从根 $0$ 起 | 查询/插入的游标 |
| `ok` | 查询路径是否走通 | `bool`，初值 `true` | 断则置假 |

### 算法步骤

1. I/O 加速三行。
2. 读 `n`；对每个名字：从根 $u=0$ 沿字符走，`x=c-'a'`，若 `ch[u][x]==0` 则 `ch[u][x]=++tot`，`u=ch[u][x]`；走完置 `ed[u]=true`。
3. 读 `m`；对每个报的名字：从根 $u=0$ 沿字符走，若某步 `ch[u][x]==0` 则 `ok=false` 跳出；否则 `u=ch[u][x]`。
4. 判定：`!ok || !ed[u]` → `WRONG`；否则 `!vis[u]` → 置 `vis[u]=true`、输出 `OK`；否则输出 `REPEAT`。

### 复杂度

- 时间：$O((n+m)L)$，$L=50$，约 $5.5\times10^6$ 步。
- 空间：$O(nL\cdot26)\approx50$ MB（`ch` 约 $52$ MB + 两个 `bool` 数组）。
- 验证状态：`g++ -std=c++20 -O2` 编译通过；题面无样例，手造两组——名单 `alice/bob/carol` 报 `alice bob alice dave bob` 得 `OK OK REPEAT WRONG REPEAT`；名单仅 `abc` 报 `ab abc abcd a` 得 `WRONG OK WRONG WRONG`，覆盖三态输出与"真前缀/超长串"边界，与手算一致。

---

## 五、示例代码（C++，Louis Li 风格）

```cpp
// 题目：P2580 于是他错误的点名开始了
// 思路：名单插进字典树，结尾打 ed 标记；查询沿字符走。
//       走不通或 ed=false ⇒ WRONG（ed 区分"完整名字"与"仅前缀"）；
//       ed=true 时看 vis：首次 OK 并置 vis，之后 REPEAT。
//       名字互不相同 ⇒ 结尾节点互不相同 ⇒ vis 打在节点上等价于打在名字上。
//       Trie 确定性 O(L)，对 anti-hash hack 免疫。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxnode{500005};	// n*50+5，最坏无公共前缀

int n,m,tot;
int ch[mxnode][26];	// 0 表示空儿子（根也是 0）
bool ed[mxnode],vis[mxnode];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		string s;cin>>s;
		int u=0;
		for(char c:s)
		{
			int x=c-'a';
			if(!ch[u][x])ch[u][x]=++tot;
			u=ch[u][x];
		}
		ed[u]=true;	// 名字结尾
	}
	cin>>m;
	for(int i=0;i<m;++i)
	{
		string s;cin>>s;
		int u=0;
		bool ok=true;
		for(char c:s)
		{
			int x=c-'a';
			if(!ch[u][x]){ok=false;break;}
			u=ch[u][x];
		}
		if(!ok||!ed[u])cout<<"WRONG\n";	// 走不通 / 只是前缀
		else if(!vis[u]){vis[u]=true;cout<<"OK\n";}
		else cout<<"REPEAT\n";
	}
	return 0;
}
```

### 代码说明

- **`ed` 不可省**：只判路径走通会把"名单名字的真前缀"误判为正确（如名单 `abc`、报 `ab`）。`ed[u]` 标记完整名字结尾，是区分名字与前缀的唯一依据（引理 2）。
- **`ch` 用 $0$ 表示空**：根节点也是 $0$，新建节点从 $1$ 起（`++tot`），故 `ch[u][x]==0` 即"无此儿子"，与根编号不冲突（根不会被当作儿子引用）。全局 `int` 默认 $0$ 恰好表示空，无需额外初始化。
- **`vis` 打在节点上**：名字互不相同 ⇒ 结尾节点互不相同（引理 3），故节点级 `vis` 等价于名字级；`WRONG` 分支不碰 `vis`，不污染状态。
- **数组尺寸**：`mxnode=500005` 覆盖最坏 $n\times50=5\times10^5$ 个节点（引理 5）；`ch` 约 $52$ MB，在 $128$ MB 限制内。
- **确定性**：全程数组下标访问，无哈希，最坏 $O((n+m)L)$，对题面提到的 hack 数据免疫。