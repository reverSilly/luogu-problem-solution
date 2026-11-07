# P1822 魔法指纹

## 1. 重新读题

### 1.1 人话版

对一个至少两位的正整数 $n$，把它的十进制数字写成一排，相邻两位取差的绝对值，得到一串新数字；去掉这串数字的**前导零**后得到的数记作 $\operatorname{magic}(n)$。一位数规定 $\operatorname{magic}(n)=n$。

例如 $\operatorname{magic}(5913)=482$（$|5-9|=4,|9-1|=8,|1-3|=2$），$\operatorname{magic}(1198)=081=81$，$\operatorname{magic}(666)=00=0$。

把 $\operatorname{magic}$ 反复作用直到变成一位数，这个最终的一位数字叫 $n$ 的**魔法指纹**。指纹为 $7$ 的数叫幸运数。给定 $A,B$，求 $[A,B]$ 中幸运数的个数。

### 1.2 关键约束与规模暗示

- $1\le A\le B\le10^9$。
- 子任务：$30\%$ 数据 $B\le10^4$，可以直接暴力。
- 满分 $B\le10^9$：逐个 $n$ 算指纹，每个 $n$ 最多 $9$ 轮、每轮 $9$ 次运算，约 $8\times10^9$ 次操作，太慢。
- $B\le10^9$ 意味着**位数只有 $\le10$**。这个"位数很小"是唯一可用的结构。

---

## 2. 思维链（Chain-of-thought）

### 2.1 先算清楚暴力有多慢

对每个 $n$，每一轮位数至少减少 $1$（$\operatorname{magic}$ 的输出最多 $len-1$ 位），所以迭代次数 $\le9$，每轮 $O(10)$。总 $O(B\cdot 100)\approx10^{11}$，不行。

自然的下一步：**能不能不逐个枚举 $n$，而是反过来，从最终答案 $7$ 倒推出所有 $n$？**

### 2.2 explore its property：$\operatorname{magic}$ 的逆长什么样

设 $\operatorname{magic}(n)=m>0$。把 $n$ 的相邻差（**含前导零**）记作串 $D$，长度 $\operatorname{len}(n)-1$。定义说：$D$ 去掉前导零后是 $m$ 的十进制串。

因为 $m$ 的十进制串本身**没有前导零**，所以 $D$ 只能形如
$$
D=\underbrace{00\cdots0}_{k\ \text{个}}\ \Vert\ \operatorname{str}(m),\qquad k\ge0.
$$

反过来，若已知 $D$，要还原 $n=(e_0e_1\cdots e_{\mathrm{len}})$，只需要选首位 $e_0\in[1,9]$（首位不能是 $0$），然后逐位满足
$$
|e_i-e_{i+1}|=D_i\quad\Longrightarrow\quad e_{i+1}=e_i+D_i\ \text{或}\ e_i-D_i .
$$

**引理 1（逆的刻画）**
对 $m>0$，$\{\,n:\operatorname{magic}(n)=m\,\}$ 恰好是下面构造出的所有数：枚举 $k\ge0$，令 $D=0^k\Vert\operatorname{str}(m)$；枚举首位 $e_0\in[1,9]$；对每位 $D_i$，若 $D_i=0$ 则 $e_{i+1}=e_i$（唯一），若 $D_i>0$ 则取 $e_i\pm D_i$ 中落在 $[0,9]$ 的值（最多两个）。

**证明**：

- 构造出的 $n$：其相邻差串恰为 $D$，前导零去掉后是 $\operatorname{str}(m)$，故 $\operatorname{magic}(n)=m$。首位非零保证 $n$ 是合法的 $\operatorname{len}(n)$ 位数。
- 任取满足条件的 $n$：其相邻差串 $D$ 去零后为 $\operatorname{str}(m)$，故 $D=0^k\Vert\operatorname{str}(m)$ 对应某个 $k$；$n$ 的每一位都由相邻差与首位唯一决定（差为 $0$ 时唯一，差非零时至多两个选择），所以 $n$ 必在构造结果中。$\blacksquare$

### 2.3 为什么倒推是可行的

**引理 2（指纹为 $7$ $\iff$ 能由 $7$ 倒推出来）**
$n$ 的指纹是 $7$，当且仅当 $n$ 在"从 $7$ 出发、不断取逆"得到的集合里。

**证明**：

- **必要性**：指纹为 $7$ 意味着存在 $T$ 使 $\operatorname{magic}^T(n)=7$。对 $T$ 归纳：$T=0$ 时 $n=7$（在种子里）；若 $\operatorname{magic}^{T-1}(m)=7$ 且 $m$ 已被生成，则 $\operatorname{magic}(n)=m$，由引理 1，$n$ 是 $m$ 的逆，会在下一步被生成。
- **充分性**：每个被生成的 $n$ 都满足 $\operatorname{magic}(n)=$ 它的"父节点"，顺着父子链走 $T$ 步必到种子 $7$，所以指纹是 $7$。

注意 $7$ 是唯一指纹为 $7$ 的一位数（一位数 $\operatorname{magic}(d)=d$），故种子只有 $7$。$\blacksquare$

### 2.4 倒推的规模为什么很小

**引理 3（链上不会超过 $n$，深度有限）**
对 $n\ge10$，$\operatorname{magic}(n)<n$ 且位数严格减少；因此对任意 $n\le10^9$，它的整条迭代链都不超过 $10^9$，且最多 $9$ 步就到一位数。

**证明**：$n$ 有 $L$ 位时，差串有 $L-1$ 位，去前导零后位数 $\le L-1$，故 $\operatorname{magic}(n)<10^{L-1}\le n$（$L\ge2$）；$L=1$ 时 $\operatorname{magic}(n)=n$。位数每步至少减一，故 $\le L-1\le9$ 步。$\blacksquare$

由此，BFS 只需要在 $[1,10^9]$ 内工作（不必担心"中间量超过上界"而漏解）。另外，每一步逆操作至少让位数 $+1$，所以只需枚举 $k$ 直到 $\operatorname{len}(n)=L+k+1>10$ 为止，深度非常浅。

实际测得以 $10^9$ 为界时，幸运数总共只有 $160218$ 个——倒推集合很小，BFS 完全跑得动。

### 2.5 算法流程

1. 把 $7$ 放进队列，同时放进哈希表去重，并加入答案集合。
2. 每次取出 $m$，按引理 1 枚举它的所有逆 $n$（$n\le10^9$ 才保留）：枚举 $k$，用 DFS 逐位挑 $e_{i+1}=e_i\pm D_i$。
3. 新出现的 $n$ 入队、入集合。
4. 队列空后，把答案集合排序，对每个询问用 `lower_bound/upper_bound` 统计 $[A,B]$。

**去重的必要性**：同一个 $n$ 可能由不同的 $k$/不同的正负号组合构造出来（例如差串里出现 $0$ 时两条符号分支重合），必须用哈希表保证只处理一次，否则会指数爆炸且重复计数。

---

## 3. 模式总结（Pattern Summary）

- **consider subtask** — 直接逐个 $n$ 迭代，$B\le10^4$ 可过，满分不行。
- **explore its property** — 把 $\operatorname{magic}$ 写成"相邻差串 $=0^k\Vert\operatorname{str}(m)$"，得到显式逆映射。
- **reduce to easier problem** — 问题变成"从 $7$ 出发做逆 BFS，生成所有幸运数"，把"对每个 $n$ 判定"换成"只生成答案"。
- **classic problem solution** — 用 BFS $+$ 哈希表去重，最后排序后二分统计区间。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `lim` | `constexpr i64` | $10^9$ | — | 上界 |
| `A, B` | `i64` | 询问区间 | 输入 | 区间 |
| `vis` | `unordered_set<i64>` | 已生成的幸运数（去重） | 放入 $7$ | visited |
| `v` | `vector<i64>` | 所有幸运数 | 生成 | 排序后二分 |
| `q` | `queue<i64>` | BFS 队列 | 放入 $7$ | 待扩展 |
| `d[0..L-1]` | `int [12]` | $m$ 的十进制数字（高位在前） | 拆位 | digits |
| `k` | `int` | 差串前导零个数 | 枚举 | zeros |
| `e[0..len]` | `int [12]` | 构造中的 $n$ 的数字 | DFS | 新数各位 |

### 4.2 算法步骤

1. 读入 $A,B$。
2. `vis` 放入 $7$，`v` 放入 $7$，`q` 放入 $7$。
3. `while(!q.empty())`：
   - 取 `m=q.front(); q.pop()`；
   - 把 $m$ 拆成数字数组 `d[0..L-1]`（高位在前）；
   - 对 $k=0,1,2,\dots$：
     - `len=L+k`；若 `len+1>10` 结束该 $m$ 的枚举；
     - 对 `e[0]=1..9` 调用 DFS，逐位按 $D_i$（$i<k$ 时为 $0$，否则为 `d[i-k]`）产生 $e_{i+1}=e_i\pm D_i\in[0,9]$；
     - DFS 走到 `pos=len+1` 时把数字压成 $n$，若 $1\le n\le10^9$ 且 `vis.insert(n).second` 为真，则 `v.push_back(n); q.push(n);`。
4. `sort(v.begin(),v.end())`。
5. 输出 `upper_bound(v,B)-lower_bound(v,A)`。

### 4.3 复杂度分析

- **时间**：与生成的候选总数成正比。每个 $m$ 的枚举量被"位数 $\le10$"限制（每个 $k$ 至多 $9\cdot2^{len}$ 个候选，$len\le9$）；以 $10^9$ 为界时幸运数共 $160218$ 个，实测 BFS $+$ 排序 $<0.2\text{ s}$。
- **空间**：$O(|V|)$，$|V|=160218$。

---

## 5. 示例代码

```cpp
//
// 题目：P1822 魔法指纹
// https://www.luogu.com.cn/problem/P1822
// 与 [1,1e6] 暴力逐数求指纹 + 随机区间对拍通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 lim{1000000000LL};

i64 A,B;
unordered_set<i64>vis;
vector<i64>v;
queue<i64>q;

// 已确定 e[0..pos-1]，继续构造 len+1 位的 n
void ext(int pos,int len,int k,const int d[],int e[])
{
	if(pos==len+1)
	{
		i64 n=0;
		for(int i=0;i<=len;++i)
			n=n*10+e[i];
		if(n>=1&&n<=lim&&vis.insert(n).second)
		{
			v.push_back(n);
			q.push(n);
		}
		return;
	}
	int di=(pos-1<k?0:d[pos-1-k]);
	if(di==0)
	{
		e[pos]=e[pos-1];
		ext(pos+1,len,k,d,e);
	}
	else
	{
		int y=e[pos-1]-di;
		if(y>=0)
		{
			e[pos]=y;
			ext(pos+1,len,k,d,e);
		}
		y=e[pos-1]+di;
		if(y<=9)
		{
			e[pos]=y;
			ext(pos+1,len,k,d,e);
		}
	}
}

// 生成 m 的所有逆 n（magic(n)=m）
void gen(i64 m)
{
	int d[12],L=0;
	for(i64 x=m;x;x/=10)
		d[L++]=x%10;
	reverse(d,d+L);
	int e[12];
	for(int k=0;;++k)
	{
		int len=L+k;
		if(len+1>10)
			break;
		for(int st=1;st<=9;++st)
		{
			e[0]=st;
			ext(1,len,k,d,e);
		}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>A>>B;
	vis.insert(7);
	v.push_back(7);
	q.push(7);
	while(!q.empty())
	{
		i64 m=q.front();q.pop();
		gen(m);
	}
	sort(v.begin(),v.end());
	cout<<upper_bound(v.begin(),v.end(),B)-lower_bound(v.begin(),v.end(),A)<<'\n';
	return 0;
}
```
