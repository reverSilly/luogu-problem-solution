# P1780 [ICPC 2005 Tokyo R] 染色的立方体

## 1. 重新读题

给定 $n$（$1 \le n \le 4$）个立方体，每个立方体由 $6$ 个面的颜色字符串给出（面编号 $0 \dots 5$ 满足对面配对为 $(0, 5), (1, 4), (2, 3)$）。两个立方体视为相同，当且仅当通过三维空间刚体旋转后它们对应的 $6$ 个面颜色完全一致。求最少需要重涂多少个面，才能使全部 $n$ 个立方体完全相同。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：立方体旋转群 $S_4$ 的 $24$ 种姿态与众数贪心

一个立方体在三维空间中共有 $6 \times 4 = 24$ 种合法的刚体旋转姿态，由两个基础 $90^\circ$ 旋转生成：
1. **俯仰旋转（Pitch）**：顶面 $\to$ 前面 $\to$ 底面 $\to$ 后面 $\to$ 顶面，即置换 $(0 \to 1 \to 5 \to 4 \to 0)$，左右面 $2, 3$ 不动；
2. **偏航旋转（Yaw）**：前面 $\to$ 右面 $\to$ 后面 $\to$ 左面 $\to$ 前面，即置换 $(1 \to 2 \to 4 \to 3 \to 1)$，顶底面 $0, 5$ 不动。

**引理 1（固定姿态下的逐面众数贪心）**：
固定第 $0$ 个立方体的姿态不变，为其余 $n - 1$ 个立方体分别选定旋转姿态 $r_1, \dots, r_{n-1} \in \{0, \dots, 23\}$ 后，为了使 $n$ 个立方体的 $6$ 个对应面颜色全部相同，第 $f$ 号面（$0 \le f < 6$）的最少重涂次数等于 $n - \max_{c} \text{cnt}_f(c)$，其中 $\max_c \text{cnt}_f(c)$ 是该面在 $n$ 个立方体中出现次数最多的颜色频数。

**证明**：旋转姿态固定后，$6$ 个面的重涂决策彼此完全独立；在第 $f$ 号面上，若最终统一涂成颜色 $c$，则原本就是颜色 $c$ 的立方体无需重涂，其余 $n - \text{cnt}_f(c)$ 个立方体各需重涂 $1$ 次。取频数最大的颜色 $c$ 即可最小化该面的重涂次数。$\blacksquare$

由于 $n \le 4$，枚举其余 $n-1$ 个立方体的旋转姿态最多只需 $24^3 = 13824$ 次，每次统计 $6$ 个面的最大频数，耗时不到 $1\text{ ms}$！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 用两个基础 $90^\circ$ 旋转生成立方体旋转群的全部 $24$ 种置换。
- **classic problem solution** — 固定第 $0$ 个立方体姿态，枚举其余 $n-1$ 个立方体的 $24^{n-1}$ 种姿态，对每个面独立取众数颜色（引理 1）。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `rot[24][6]` | `int [24][6]` | 立方体的 $24$ 种刚体旋转置换表 | 预生成 | 3D rotations |
| `n` | `int` | 立方体个数（$\le 4$） | 每组读入 | count |
| `col[i][f]` | `int [4][6]` | 离散化后的第 $i$ 个立方体第 $f$ 面颜色编号 | 读入映射 | face colors |

### 4.2 算法步骤

1. 预生成 $24$ 种旋转置换 `rot[0..23][0..5]`。
2. 循环读入 $n$（$n = 0$ 结束），将颜色字符串离散化为整数编号 `col[i][0..5]`。
3. 固定第 $0$ 个立方体旋转为 `0`，用 DFS 枚举第 $1 \dots n-1$ 个立方体的旋转编号：在叶子处对每个面 $f = 0 \dots 5$ 统计 $n$ 个立方体的最大同色频数 `mx`，将 $n - \text{mx}$ 累加并更新全局最小值 `ans`。

### 4.3 复杂度分析

- **时间复杂度**：每组数据至多 $24^{n-1} \le 13824$ 个状态，每个状态 $O(6n)$，耗时 $< 2\text{ ms}$。
- **空间复杂度**：常数级数组，空间复杂度为 $\Theta(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1780 [ICPC 2005 Tokyo R] 染色的立方体
// https://www.luogu.com.cn/problem/P1780
// AC
#include<bits/stdc++.h>
using namespace std;

int rot[24][6];
int n,col[4][6],cur_r[4],ans;

void init_rot()
{
	auto pitch=[](array<int,6>a)->array<int,6>{
		return {a[4],a[0],a[2],a[3],a[5],a[1]};
	};
	auto yaw=[](array<int,6>a)->array<int,6>{
		return {a[0],a[3],a[1],a[4],a[2],a[5]};
	};
	set<array<int,6>>vis;
	queue<array<int,6>>Q;
	array<int,6>st={0,1,2,3,4,5};
	vis.insert(st);
	Q.push(st);
	while(!Q.empty())
	{
		auto u=Q.front();Q.pop();
		for(auto v:{pitch(u),yaw(u)})
			if(!vis.count(v))
			{
				vis.insert(v);
				Q.push(v);
			}
	}
	int idx=0;
	for(const auto&a:vis)
	{
		for(int j=0;j<6;++j)
			rot[idx][j]=a[j];
		++idx;
	}
}

void dfs(int dep)
{
	if(dep==n)
	{
		int cost=0;
		for(int f=0;f<6;++f)
		{
			int mx=0;
			for(int i=0;i<n;++i)
			{
				int c=col[i][rot[cur_r[i]][f]],cnt=0;
				for(int j=0;j<n;++j)
					if(col[j][rot[cur_r[j]][f]]==c)
						++cnt;
				mx=max(mx,cnt);
			}
			cost+=n-mx;
		}
		ans=min(ans,cost);
		return;
	}
	for(int r=0;r<24;++r)
	{
		cur_r[dep]=r;
		dfs(dep+1);
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	init_rot();
	while(cin>>n&&n)
	{
		map<string,int>mp;
		int cid=0;
		for(int i=0;i<n;++i)
			for(int j=0;j<6;++j)
			{
				string s;
				cin>>s;
				if(!mp.count(s))
					mp[s]=cid++;
				col[i][j]=mp[s];
			}
		ans=24;
		cur_r[0]=0;
		dfs(1);
		cout<<ans<<'\n';
	}
	return 0;
}
```
