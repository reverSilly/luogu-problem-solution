# P1193 洛谷团队训练 VS 传统团队训练 题解

## Part 1 — 重述题目（简单版）

有 $N$ 道题、$M$ 个学生、$R$ 次评测记录。两种训练方式：

**传统方式**
- 在教师机装载全部 $N$ 道题：$N \times T_a$。
- 每个出场的学生 $s$ 有两种选择，取较小者：
  - 跑教师机评测：$T_a + T_b \cdot c_s$（其中 $c_s$ 是该学生的评测次数）；
  - 在自己机器装数据评测：$T_a + T_c \cdot c_s$。
- 若 $E=1$（用 Excel 记成绩）：每当某学生某道题的分数**严格高于**该题此前的最高分，额外花 $T_d$ 更新表格（第一次记录该题时，旧记录视为 0 分；分数为 0 时不更新）。

**洛谷方式**
- 上传全部 $N$ 道题：$N \times T_a$。
- 每次评测 $T_c$：$R \times T_c$。
- 无记录成本；按可用度 $A\%$ 折算（去掉小数点）后加上罚时 $H$：

$$\text{洛谷耗时} = \left\lfloor \frac{N \cdot T_a + R \cdot T_c}{A / 100} \right\rfloor + H$$

输出两行耗时，第三行：洛谷严格更短则 `Use Luogu!`，否则 `Forget it...`。

**约束**：$1 \le N, M \le 1000$，$1 \le R < 100000$，题目号/学号 $\le 10^8$，$0 \le \text{成绩} \le 100$，$1 \le A \le 100$。

## Part 2 — 思维链（完整推导）

### 1. 探索机制：这道题到底在算什么

把所有成本拆开看：

- 传统方式 = $N T_a$（教师机装载）+ 每个学生的 min 项 + Excel 更新项；
- 洛谷方式 = $\left\lfloor \dfrac{N T_a + R T_c}{A/100} \right\rfloor + H$。

**关键观察**：洛谷的总评测次数就是 $R$，与学生怎么分布无关，所以洛谷侧是一个**与 $R$ 行记录内容完全无关**的公式。所有"逐记录"的工作只发生在传统方式的两处：

1. 统计每个学生的评测次数 $c_s$；
2. 维护"每道题的历史最高分"，判断是否需要 $T_d$。

因此本题没有任何决策树或搜索结构，纯粹是**统计 + 比较**，难点只在两件事：
- 学号/题目号大（$\le 10^8$）而人数/题数少（$\le 1000$），需要把编号映射到紧凑下标（离散化）；
- 样例说明里"去掉小数点"的精确含义，需要用样例反推验证。

### 2. 考虑更简单的版本：$E=0$ 的子任务

$50\%$ 数据不需要 Excel 记录。此时传统耗时为：

$$\text{传统} = N T_a + \sum_{s \text{ 出场}} \min\!\big(T_a + T_b\, c_s,\; T_a + T_c\, c_s\big)$$

因为 $c_s \ge 1$，所以 $\min$ 项就是 $T_a + \min(T_b, T_c)\, c_s$。更一般地直接逐项算 min 即可（$T_a$ 项保留在式子里，便于与样例对拍）。

这个简化版本已经覆盖了"每个学生二选一"的全部结构，加上 Excel 项只是**逐记录**追加一个判断，结构上不会变复杂。所以先实现 $E=0$ 的版本，再把 Excel 逻辑作为增量加回来。

**Excel 增量的精确语义**（由题面"得分高于表格里的记录"推出）：

- 对每道题 $p$ 维护历史最高分 $h_p$，初始 0；
- 处理到某次评测 $(p, s, \text{sc})$ 时，若 $\text{sc} > h_p$ 则累加 $T_d$ 并更新 $h_p = \text{sc}$，否则不动。

注意两个边界：
- 第一次提交且 $\text{sc} > 0$：$0$ 分基线被超过，要记录；
- $\text{sc} = 0$：永远不超过基线 0，**不**记录（样例中 3 号同学"都是 0 分所以没必要记录"印证了这一点）。

**样例反推**（#1：$N=4, M=5$，记录 (2,2,50),(2,7,80),(3,3,0),(3,5,100),(3,7,50),(5,2,90),(5,5,50),(5,7,40)；$T_a=50,T_b=30,T_c=10,T_d=5,A=93,H=50,E=1$）：

| 题 | 记录序列 | 超过 0 的次数 | $T_d$ 项 |
|---|---|---|---|
| 2 | 50, 80 | 2（50>0, 80>50） | 10 |
| 3 | 0, 100, 50 | 1（100>0） | 5 |
| 5 | 90, 50, 40 | 1（90>0） | 5 |

Excel 合计 $20$，与样例解释 $10+5+5=20$ 一致。

各学生 $c_s$：2 号 3 次、3 号 1 次、5 号 2 次、7 号 3 次。min 项：
- 2 号：$\min(50+90, 50+30) = 80$；
- 3 号：$\min(50+30, 50+10) = 60$；
- 5 号：$\min(50+60, 50+20) = 70$；
- 7 号：$\min(50+90, 50+30) = 80$。

传统合计 $200 + 80+60+70+80 + 20 = 510$。

洛谷侧：$N T_a + R T_c = 200 + 100 = 300$，$\lfloor 300 / 0.93 \rfloor = \lfloor 322.58 \rfloor = 322$，加 $H$ 得 $372$。$372 < 510$，输出 `Use Luogu!`。

这说明"去掉小数点"= **向下取整**（截断）。

### 3. 经典技术：离散化

学号与题目号都在 $10^8$ 量级，不能开 $10^8$ 的数组。但出场学生 $\le M \le 1000$、出场题目 $\le N \le 1000$。

**引理（离散化正确性）**：设出场学生集合 $S' \subseteq S$、出场题目集合 $P' \subseteq P$。存在双射 $\pi: S' \to \{0, \dots, |S'|-1\}$、$\rho: P' \to \{0, \dots, |P'|-1\}$，使得任何只依赖"$s$ 的评测次数"和"题 $p$ 的历史最高分"的统计，在按下标 $\pi(s)$、$\rho(p)$ 维护时结果与按原编号维护完全一致。

**证明**：统计只用到集合 $\{c_s : s \in S'\}$ 与 $\{h_p : p \in P'\}$，每个原编号在统计中只作为一个 key 出现，从不利用其数值大小（比较、算术运算都发生在计数/分数上，编号只作键）。双射将不同键一一映射到不同下标，不产生冲突、不丢失键，故逐键的维护过程（$c$ 自增、$h$ 取 max）在两个表示下逐位相同。$\blacksquare$

实现上用 `unordered_map<int,int>` 建表：读入 $S_i$ / $P_i$ 时顺带查表，没出现过就分配新下标。$M, N \le 1000$，即使链退化也不影响复杂度评估。

### 4. 归约到更简单的问题：逐记录线性扫描

把上面的结论合起来，整个算法退化为一次 $O(R)$ 的线性扫描：

- 第 $i$ 条记录：$c_{\pi(s_i)} \mathrel{+}= 1$；
- 若 $E=1$ 且 $\text{sc}_i > h_{\rho(p_i)}$：$h_{\rho(p_i)} \leftarrow \text{sc}_i$，$excel \mathrel{+}= T_d$。

扫描结束后做一次 $O(M)$ 的后处理求 $\sum_s \min(\cdot)$。没有需要排序、建树或分治的结构。

### 5. 复杂度与取整细节

- 时间：$O(N + M + R)$（哈希表均摊 $O(1)$）；
- 空间：$O(N + M)$；
- 数值：$N T_a \le 10^7$，$R T_c \le 10^8$，$c_s T_b \le 10^6$，所有项在 32 位 int 内；$100 \times (\dots) \le 10^{10}$，故**乘 $100$ 的中间量用 `long long`**，取整前不溢出。

$\lfloor \dfrac{x}{A/100} \rfloor = \left\lfloor \dfrac{100 x}{A} \right\rfloor$：整数除法天然向下取整，与"去掉小数点"一致（$A=100$ 时为 $x$ 本身）。

## Part 3 — 模式总结

- explore its property — 洛谷耗时只依赖 $N, R, A, H$，与学生分布无关；传统耗时 = 装载 + 逐学生 min + 逐记录 Excel 更新
- consider easier version — $E=0$ 子任务先落地，Excel 项作为"超过历史最高分才 +$T_d$"的增量补上
- classic problem solution — 离散化：把 $\le 10^8$ 的学号/题号映射到 $\le 1000$ 的紧凑下标
- reduce to easier problem — 归约为一次 $O(R)$ 线性扫描 + 一次 $O(M)$ 后处理求 min 和
- classic problem solution — 整数除法语义：$\lfloor 100x/A \rfloor$ 精确实现"除以可用度并去掉小数点"

## Part 4 — 实际解法（符号表 + 算法）

### 符号表

| 符号 | 含义 | 初始化 |
|---|---|---|
| `n, m, r` | 题目数、学生数、评测数 | 读入 |
| `ta, tb, tc, td, H` | 五种耗时参数 | 读入 |
| `A` | 可用度百分数（1..100） | 读入 |
| `E` | 是否用 Excel 记录（0/1） | 读入 |
| `sid[i]` | 第 $i$ 个学生的学号（原编号） | 读入 |
| `pid[j]` | 第 $j$ 道题的题号（原编号） | 读入 |
| `sidid` | 学号 → 紧凑下标 的哈希表 | 建表时填充 |
| `pidid` | 题号 → 紧凑下标 的哈希表 | 建表时填充 |
| `cnt[i]` | 学生 $i$ 的评测次数 $c_s$ | 0（全局数组默认） |
| `best[j]` | 题 $j$ 的历史最高分 $h_p$ | 0（全局数组默认，正好是"无记录视为 0 分"的语义） |
| `excel` | Excel 更新总耗时 | 0 |
| `tra` | 传统总耗时 | $n \cdot ta$ |
| `lgo` | 洛谷总耗时（含罚时） | 最后算 |

记忆法：`cnt`=count 次数，`best`=best 分数；两个哈希表都是"原编号→下标"。

### 算法步骤

1. 读 $n, m$；读 $n$ 个题号建 `pidid`；读 $m$ 个学号建 `sidid`。
2. 读 $ta, tb, tc, td, A, H, E$；读 $r$。
3. 初始化 `tra = n * ta`。
4. 逐条读 $r$ 条评测记录 $(p, s, \text{sc})$：
   - `cnt[sidid[s]]++`；
   - 若 $E=1$ 且 $\text{sc} > \text{best[pidid[p]]}$：`best[pidid[p]] = sc; excel += td;`
5. 扫描 $m$ 个学生：`tra += min(ta + tb*cnt[i], ta + tc*cnt[i])`。
6. `tra += excel`。
7. `lgo = (100LL * (n*ta + r*tc)) / A + H`。
8. 输出 `tra`、`lgo` 两行；若 `lgo < tra` 输出 `Use Luogu!`，否则 `Forget it...`。

**复杂度**：$O(N + M + R)$ 时间，$O(N + M)$ 空间。

**验证状态**：按样例解释手算对拍 —— 传统 $510$、洛谷 $372$、结论 `Use Luogu!`，与样例输出一致（未做随机对拍，题目要求不运行程序）。

## Part 5 — 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

int n,m,r;
int ta,tb,tc,td,H,A,E;
int pid[mxn],sid[mxn];
unordered_map<int,int>sidid,pidid;
int cnt[mxn],best[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		cin>>pid[i];
		pidid[pid[i]]=i;
	}
	for(int i=0;i<m;++i)
	{
		cin>>sid[i];
		sidid[sid[i]]=i;
	}
	cin>>ta>>tb>>tc>>td>>A>>H>>E;
	cin>>r;

	i64 tra=1LL*n*ta,excel=0;
	for(int i=0,pr,sr,sc;i<r;++i)
	{
		cin>>pr>>sr>>sc;
		cnt[sidid[sr]]++;
		if(E&&sc>best[pidid[pr]])
		{
			best[pidid[pr]]=sc;
			excel+=td;
		}
	}
	for(int i=0;i<m;++i)
		tra+=min(ta+tb*cnt[i],ta+tc*cnt[i]);
	tra+=excel;
	i64 lgo=100LL*(1LL*n*ta+1LL*r*tc)/A+H;
	cout<<tra<<'\n'<<lgo<<'\n'<<(lgo<tra?"Use Luogu!\n":"Forget it...\n");
	return 0;
}
```
