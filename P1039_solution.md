# P1039 [NOIP 2003 提高组] 侦探推理 — 题解

## 一、发现叙事

### 谜面

> 一场推理游戏。明明（侦探）知道**恰好有 N 个人始终在说谎**，其余人始终说真话。**凶手只有一个**。
>
> 明明逐个询问了每个人，得到了 P 条证词。证词只有五种格式（见题目描述）。其他话忽略。
>
> 问：谁是凶手？

乍看是一个典型的"谁是凶手"逻辑谜题。M 最大 20，N 不超过 M，P 不超过 100。输入规模不大，暗示我们很可能不需要高级算法，**枚举**才是正解。

### 关键观察

我们来看数据范围：**M ≤ 20**。这意味着：
- 凶手候选人最多 20 个。
- 星期几有 **7 种**可能（Monday ~ Sunday）。

如果同时枚举"凶手是谁"和"今天是星期几"，总共只有 **20 × 7 = 140** 种组合。对于每种组合，我们只需要检查它是否与所有证词一致，并统计说谎人数是否恰好为 N。

**这就是本题的核心思路：枚举 + 模拟。**

---

## 二、思维链与完整证明

### 2.1 形式化模型

设：
- 同学集合为 $P = \{p_0, p_1, \dots, p_{M-1}\}$，共 $M$ 人。
- 恰好 $N$ 人始终说谎（每句话都是假话），其余 $M-N$ 人始终说真话（每句话都是真话）。
- 凶手恰好一人，记作 $c \in P$。
- 今天是星期 $d \in \{0,1,\dots,6\}$（分别对应 Monday~Sunday）。

每一条证词 $s$ 是以下五种之一：

| 证词 | 真值条件 |
|------|---------|
| `I am guilty.` | $c = \text{说话者}$ |
| `I am not guilty.` | $c \neq \text{说话者}$ |
| `XXX is guilty.` | $c = \text{XXX}$ |
| `XXX is not guilty.` | $c \neq \text{XXX}$ |
| `Today is XXX.` | $d = \text{XXX}$（星期匹配） |

### 2.2 一致性条件

对于给定的猜测 $(c, d)$，我们检查每条证词的真假：

- 若一条证词为**真**，则说话者**必须**是诚实者（说真话的人）。
- 若一条证词为**假**，则说话者**必须**是撒谎者（说假话的人）。

因此，对每个人 $p_i$，扫描他说的所有证词：

- 如果 $p_i$ 至少说了一句真话**且**至少说了一句假话 → **矛盾**（一个人不能既诚实又撒谎）。
- 如果 $p_i$ 的所有话都为真 → $p_i$ 被强制为诚实者。
- 如果 $p_i$ 的所有话都为假 → $p_i$ 被强制为撒谎者。
- 如果 $p_i$ 没说话（或说的话不属五种格式被忽略）→ $p_i$ 的角色**自由**，可诚实可撒谎。

记：
- $L_{\text{强制}} =$ 被强制为撒谎者的人数
- $T_{\text{强制}} =$ 被强制为诚实者的人数
- $F = M - L_{\text{强制}} - T_{\text{强制}} =$ 自由人数

则存在一种合法的角色分配，当且仅当：

$$
L_{\text{强制}} \le N \le L_{\text{强制}} + F
$$

等价于：

$$
L_{\text{强制}} \le N \le M - T_{\text{强制}}
$$

因为自由的人既可以当撒谎者也可以当诚实者，只要最终的撒谎者总数恰好为 $N$。

**证明：**

必要性：
- 必须有至少 $L_{\text{强制}}$ 个撒谎者，因为这些人已经被证词强制为撒谎者。故 $N \ge L_{\text{强制}}$。
- 必须有至少 $T_{\text{强制}}$ 个诚实者，故 $M - N \ge T_{\text{强制}}$，即 $N \le M - T_{\text{强制}}$。

充分性：
- 当 $L_{\text{强制}} \le N \le M - T_{\text{强制}}$ 时，我们先把强制撒谎者设为撒谎者，强制诚实者设为诚实者。
- 剩余自由人数为 $F = M - L_{\text{强制}} - T_{\text{强制}}$。
- 还需要 $N - L_{\text{强制}}$ 个撒谎者，而 $0 \le N - L_{\text{强制}} \le F$，所以可以从自由人中选出 $N - L_{\text{强制}}$ 人设为撒谎者，其余自由人设为诚实者。$\square$

### 2.3 枚举策略

```
for 凶手 c in 0..M-1:
    for 星期 d in 0..6:
        检查一致性:
            如果一致 → c 是一个可能的凶手
```

最后统计：
- 如果恰有一个 $c$ 是可能的 → 输出他的名字
- 如果多于一个 $c$ 是可能的 → 输出 `Cannot Determine`
- 如果零个 $c$ 是可能的 → 输出 `Impossible`

**注意：** 同一个凶手 $c$ 可能对应多个不同的 $d$ 都一致。我们只关心"是否存在某个 $d$ 使 $(c,d)$ 合法"，而不关心有多少个 $d$。所以用布尔数组 `pos[c]` 标记即可。

### 2.4 复杂度分析

- 枚举 $c$：$O(M)$
- 枚举 $d$：$O(7)$
- 每次检查：扫描 $P$ 条证词 $O(P)$，每人检查 $O(M)$
- 总复杂度：$O(M \times 7 \times P) = O(7MP)$

$M \le 20, P \le 100$，最坏 $7 \times 20 \times 100 = 14000$ 次操作，瞬间出解。

---

## 三、模式总结

本题使用的是 **"枚举 + 模拟"** 模式，特别适合以下场景：

> **存在少量未知离散变量，且每个变量的取值空间很小，可以直接枚举所有组合，对每种组合模拟验证。**

识别标志：
1. **题目中的未知参数个数少、取值空间小。** 本题：凶手（M 种）× 星期（7 种）。
2. **验证规则明确、可计算。** 本题：每条证词的真假可计算，进而可推出每个人的角色约束。
3. **数据范围极小。** M ≤ 20, P ≤ 100。

枚举的对象不一定是凶手和星期。有时也枚举"谁在说谎"（如本题 N 较小的时候），但本题枚举凶手+星期更直观。

---

## 四、正解推导与符号说明

### 符号表

| 符号 | 含义 |
|------|------|
| $M$ | 总人数 |
| $N$ | 恰好撒谎的人数 |
| $P$ | 证词总数 |
| $nm_i$ | 第 $i$ 个人的名字 |
| $c$ | 枚举的凶手下标 |
| $d$ | 枚举的星期（0~6） |
| $st_i$ | 第 $i$ 个人的角色状态：0=自由, 1=强制撒谎, 2=强制诚实 |
| $pos_c$ | 凶手 $c$ 是否可能（存在某个 $d$ 使组合一致） |

### 算法流程

```
1. 读入 M, N, P。
2. 读入 M 个名字到 nm[]。
3. 读入 P 条证词，每条解析为 (说话者, 证词内容)。
4. 枚举凶手 c:
     枚举星期 d:
         初始化 st[] = {0}
         遍历每条证词:
             找到说话者下标 idx
             计算证词在当前 (c,d) 下的真假
             更新 st[idx]:
                 - 证词为真: 若 st[idx]==1 则矛盾; 否则 st[idx]=2
                 - 证词为假: 若 st[idx]==2 则矛盾; 否则 st[idx]=1
         若无矛盾:
             统计强制撒谎人数 ml 和强制诚实人数 mt
             若 ml ≤ N ≤ M-mt → pos[c]=true
5. 统计 pos[] 中为 true 的人数 cnt:
     若 cnt==0 → Impossible
     若 cnt==1 → 输出该名字
     若 cnt>1  → Cannot Determine
```

---

## 五、C++ 实现

```cpp
#include<bits/stdc++.h>
using namespace std;

int m,n,p;
string nm[20];
string who[100];
string say[100];
string wk[7]={"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
bool pos[20];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n>>p;
	for(int i=0;i<m;++i)
		cin>>nm[i];
	cin.ignore();
	for(int i=0;i<p;++i)
	{
		string ln;
		getline(cin,ln);
		int po=ln.find(": ");
		who[i]=ln.substr(0,po);
		say[i]=ln.substr(po+2);
	}
	for(int cr=0;cr<m;++cr)
	{
		for(int d=0;d<7;++d)
		{
			int st[20]={};
			bool ok=1;
			for(int i=0;i<p&&ok;++i)
			{
				int idx=-1;
				for(int j=0;j<m;++j)
				{
					if(nm[j]==who[i])
					{
						idx=j;
						break;
					}
				}
				string&t=say[i];
				bool tr;
				if(t=="I am guilty.")
					tr=(cr==idx);
				else if(t=="I am not guilty.")
					tr=(cr!=idx);
				else if(t.size()>=15&&t.substr(t.size()-15)==" is not guilty.")
				{
					string x=t.substr(0,t.size()-15);
					int xi=-1;
					for(int j=0;j<m;++j)
					{
						if(nm[j]==x)
						{
							xi=j;
							break;
						}
					}
					tr=(xi!=cr);
				}
				else if(t.size()>=11&&t.substr(t.size()-11)==" is guilty.")
				{
					string x=t.substr(0,t.size()-11);
					int xi=-1;
					for(int j=0;j<m;++j)
					{
						if(nm[j]==x)
						{
							xi=j;
							break;
						}
					}
					tr=(xi==cr);
				}
				else if(t.size()>=9&&t.substr(0,9)=="Today is ")
				{
					string dy=t.substr(9,t.size()-10);
					int di=-1;
					for(int j=0;j<7;++j)
					{
						if(wk[j]==dy)
						{
							di=j;
							break;
						}
					}
					tr=(di==d);
				}
				else
					continue;
				if(tr)
				{
					if(st[idx]==1)
						ok=0;
					else
						st[idx]=2;
				}
				else
				{
					if(st[idx]==2)
						ok=0;
					else
						st[idx]=1;
				}
			}
			if(!ok)
				continue;
			int ml=0,mt=0;
			for(int i=0;i<m;++i)
			{
				if(st[i]==1)
					++ml;
				if(st[i]==2)
					++mt;
			}
			if(ml<=n&&n<=m-mt)
				pos[cr]=1;
		}
	}
	int cnt=0;
	string ans;
	for(int i=0;i<m;++i)
	{
		if(pos[i])
		{
			++cnt;
			ans=nm[i];
		}
	}
	if(cnt==0)
		cout<<"Impossible\n";
	else if(cnt==1)
		cout<<ans<<'\n';
	else
		cout<<"Cannot Determine\n";
	return 0;
}
```

### 代码要点说明

- **名字下标查找**：$M \le 20$，直接线性查找，无需哈希表。
- **证词解析顺序**：先判断 `is not guilty.` 再判断 `is guilty.`，避免 `is not guilty.` 被误匹配到 `is guilty.` 子串。
- **角色冲突检测**：用 `st[]` 标记每人的强制身份。若同一人既有真话又有假话，立即冲突。
- **星期匹配**：从证词中提取 `"Today is "` 后的星期字符串，与 `wk[]` 比较。
- **输出规则**：统计 `pos[]` 中标记的可能凶手数量，按题目要求输出对应结果。
