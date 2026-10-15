# P1139 单向双轨道

## 1. 重述问题

车站有入口 A、两个调度站 B 和 C（相当于两个栈，容量无限）、出口 D。n 辆火车（n ≤ 15，编号为小写字母）在 A 处以 a,b,c,... 的顺序等待。所有移动必须从左向右单向：A→B、A→C、B→D、C→D。给定一个目标排列（n 个互异小写字母的字符串），问能否通过栈操作将 A 处的初始序列变换为目标序列。若能，输出**最短**的操作序列（每行 `h, L, R`）；若多解，输出按 `(L,R)` 字典序最小的方案。若不能，输出 `NO`。

## 2. 推理链

### 2.1 问题本质

这是典型的**双栈排序（two-stack sorting）**问题。入口 A 就像一个队列（先进先出），B 和 C 是两个栈（后进先出），D 是输出。

初始状态：
- A 有序列 a,b,c,..., 按顺序出队
- B 空栈
- C 空栈
- D 空输出序列

操作：
1. A→B：A 队首移动到 B 栈顶
2. A→C：A 队首移动到 C 栈顶
3. B→D：B 栈顶移动到 D（追加到输出末尾）
4. C→D：C 栈顶移动到 D

目标：D 的输出序列等于给定的目标字符串。

### 2.2 n ≤ 15 → 状态空间有限

n ≤ 15，每个火车有 4 种位置可能性（A/B/C/D），但约束很强（A 是队列，B/C 是栈，D 是序列）。状态总数是有限的。考虑 BFS 求**最短**操作序列。

### 2.3 状态定义

一个状态由以下信息唯一确定：
- `ai`：A 中还未出队的火车索引（0 到 n）
- B 的内容：从栈底到栈顶的火车列表
- C 的内容：从栈底到栈顶的火车列表
- D 的内容：已输出的火车序列

但因为 n ≤ 15，B 和 C 的内容可以表示为字符串。每个状态可以完整编码。

### 2.4 BFS 搜索

从初始状态开始 BFS：
- 状态：`(ai, B_str, C_str, D_str)`
- 初始：`(0, "", "", "")`
- 目标：`D_str == target`

每步扩展 4 种操作。由于要求**最短**序列，BFS 天然保证。

### 2.5 字典序处理

当有多种最短方案时，按操作 `(L,R)` 的字典序最小输出。操作序列的字典序是指按顺序比较每一行。每一行格式为 `h, L, R`，字典序比较的实质是比较 `(L,R)` 对（因为 h 由 L,R 决定）。

要让最终序列字典序最小，可以在 BFS 中按操作类型的有序顺序扩展，这样第一个搜索到的目标状态就是字典序最小的。

操作的有序性：考虑 `(L,R)` 字典序，即比较 `(L,R)` 对的字符串顺序：A→B, A→C, B→D, C→D。这四个对的字典序从小到大是：
- A→B (L='A',R='B')
- A→C (L='A',R='C')
- B→D (L='B',R='D')
- C→D (L='C',R='D')

按此顺序扩展 BFS，首次到达目标状态的路径即为字典序最小的最短方案。

所以扩展顺序：A→B, A→C, B→D, C→D。

### 2.6 路径记录

每个状态需要记录它的前驱状态和导致该状态的操作。BFS 队列中存储完整的状态和路径。由于 n ≤ 15，状态数不会爆炸（每个火车要么在 A、B、C、D 之一，状态数有一个组合上界但实际远小于搜索空间）。

### 2.7 为什么 BFS 能找到解

四个操作是可逆吗？不完全可逆，但 BFS 在有限状态图上一定可以找到最短路径。如果有解，从初态到目标态存在一条操作序列，BFS 一定能在有限步内找到它。如果 BFS 耗尽所有可达状态仍未达到目标，则输出 NO。

### 2.8 正确性证明概要

**引理 1**：BFS 在无权重图上找到的是最短路径。初始状态到目标状态的每条边权重为 1（一次操作），所以 BFS 找到的操作序列是最短的。

**引理 2**：在 BFS 中，如果按照字典序从小到大扩展邻居，则首次到达目标状态的路径是字典序最小的最短路径。证明：设 BFS 按层（距离）扩展，在同一层内，先遍历字典序小的操作。由于字典序比较是对整个路径序列按位置进行，第一个被访问到的目标状态一定具有最小的字典序。

**引理 3**：状态空间有限。每个火车有有限种可能的位置组合，因此状态总数 ≤ (n+1) × 2^{O(n)}，对 n=15 完全可行。

## 3. 模式总结

- **问题建模** — 将物理场景抽象为双栈排序模型：A=队列，B/C=栈，D=输出
- **BFS 最短路径** — 无权重图上 BFS 求最少操作次数
- **字典序 BFS** — 按操作 (L,R) 字典序从小到大扩展邻居，保证首次找到的解即字典序最小最短方案
- **状态编码** — 用 (ai, B, C, D) 四元组唯一标识一个状态
- **路径追溯** — 每个状态记录前驱和操作，最终从目标回溯到初态

## 4. 真实解法

### 符号表

| 符号 | 含义 |
|------|------|
| `n` | 火车数量（n ≤ 15） |
| `tar` | 目标排列字符串 |
| `ai` | A 中下一个要出队的火车索引（0-based），ai=n 表示 A 已空 |
| `sb,sc` | 分别表示 B 和 C 栈的内容（栈底到栈顶的字符串） |
| `sd` | 当前已输出到 D 的序列字符串 |
| `st` | 状态四元组 (ai, sb, sc, sd) |
| `pre[v]` | 记录状态 v 的前驱状态 |
| `op[v]` | 记录从 pre[v] 到 v 所执行的操作字符串 |
| `Q` | BFS 队列 |

### 算法步骤

1. 读取 n 和目标字符串 `tar`。
2. BFS 初始化：队列 Q 放入初态 `(0, "", "", "")`，标记初态已访问。
3. 按顺序尝试 4 种扩展：
   - A→B：ai < n 时，取出 `tar[ai]` 的火车（其实是取出 A 中字母），放入 B 栈顶
   - A→C：同上但放入 C
   - B→D：B 非空时，B 栈顶元素出栈进入 D
   - C→D：C 非空时，C 栈顶元素出栈进入 D
4. 对新状态，若未访问过则记录操作和路径并入队。
5. 若某状态 `sd == tar`（且 ai=n, sb 空, sc 空），则回溯输出操作序列。
6. BFS 结束后若未找到，输出 `NO`。

### 复杂度

- 状态数：O((n+1) × 2^{n}) 级别，n=15 时足够小
- 每次扩展 O(1)
- 总复杂度：O(状态数 × 4)

## 5. 示例代码

```cpp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20};

int n;
string tar;

struct State
{
	int ai;
	string sb,sc,sd;
	bool operator<(const State&o)const
	{
		if(ai!=o.ai)return ai<o.ai;
		if(sb!=o.sb)return sb<o.sb;
		if(sc!=o.sc)return sc<o.sc;
		return sd<o.sd;
	}
};

map<State,State>pre;
map<State,string>op;
State goal;
bool found;

void bfs()
{
	queue<State>Q;
	State ini{0,"","",""};
	pre[ini]=ini;
	Q.push(ini);
	while(!Q.empty())
	{
		auto[ai,sb,sc,sd]=Q.front();Q.pop();
		if(ai==n&&sb.empty()&&sc.empty()&&sd==tar)
		{
			found=true;
			goal=State{ai,sb,sc,sd};
			return;
		}
		//A->B
		if(ai<n)
		{
			State ns{ai+1,sb+tar[ai],sc,sd};
			if(!pre.count(ns))
			{
				pre[ns]=State{ai,sb,sc,sd};
				op[ns]=string{tar[ai],',','A',',','B'};
				Q.push(ns);
			}
		}
		//A->C
		if(ai<n)
		{
			State ns{ai+1,sb,sc+tar[ai],sd};
			if(!pre.count(ns))
			{
				pre[ns]=State{ai,sb,sc,sd};
				op[ns]=string{tar[ai],',','A',',','C'};
				Q.push(ns);
			}
		}
		//B->D
		if(!sb.empty())
		{
			char h=sb.back();
			State ns{ai,sb.substr(0,sb.size()-1),sc,sd+h};
			if(!pre.count(ns))
			{
				pre[ns]=State{ai,sb,sc,sd};
				op[ns]=string{h,',','B',',','D'};
				Q.push(ns);
			}
		}
		//C->D
		if(!sc.empty())
		{
			char h=sc.back();
			State ns{ai,sb,sc.substr(0,sc.size()-1),sd+h};
			if(!pre.count(ns))
			{
				pre[ns]=State{ai,sb,sc,sd};
				op[ns]=string{h,',','C',',','D'};
				Q.push(ns);
			}
		}
	}
}

void output(State s)
{
	if(pre[s].ai!=s.ai||pre[s].sb!=s.sb||pre[s].sc!=s.sc||pre[s].sd!=s.sd)
		output(pre[s]);
	if(op.count(s))
		cout<<op[s]<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>tar;
	bfs();
	if(found)
		output(goal);
	else
		cout<<"NO\n";
	return 0;
}
```
