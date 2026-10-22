# P1797 【模板】Stern-Brocot 树

## 1. 重新读题

维护正既约分数的 Stern-Brocot 树（根节点为 $\frac{1}{1}$，每个节点 $\frac{a+c}{b+d}$ 由其左界祖先 $\frac{a}{b}$ 与右界祖先 $\frac{c}{d}$ 取法雷中位数生成，初始开区间为 $(\frac{0}{1}, \frac{1}{0})$），处理 $T \le 10^5$ 次询问，支持五种操作：
1. `ENCODE_PATH p q`：输出从根 $\frac{1}{1}$ 到 $\frac{p}{q}$ 的游程编码路径；
2. `DECODE_PATH k c1 n1 ...`：输出游程路径到达的分数；
3. `LCA a b c d`：求两个分数的最近公共祖先；
4. `ANCESTOR k a b`：求 $\frac{a}{b}$ 到根路径上深度为 $k$ 的祖先（根深度为 $0$，不存在输出 `-1`）；
5. `RANGE p q`：求 $\frac{p}{q}$ 子树代表法雷开区间的下确界与上确界。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：法雷区间矩阵与辗转相除法（欧几里得算法）的同构

在 Stern-Brocot 树中，每个节点都唯一对应一个开区间 $\big(\frac{a}{b}, \frac{c}{d}\big)$（满足 $b c - a d = 1$），该节点自身的分数即为中位数 $\frac{a+c}{b+d}$：
- 向左走一步（`L`）相当于将右端点换为中位数：$\big(\frac{a}{b}, \frac{a+c}{b+d}\big)$；连续向左走 $n$ 步相当于将右端点变为 $\frac{n a + c}{n b + d}$！
- 向右走一步（`R`）相当于将左端点换为中位数：$\big(\frac{a+c}{b+d}, \frac{c}{d}\big)$；连续向右走 $n$ 步相当于将左端点变为 $\frac{a + n c}{b + n d}$！

**引理 1（辗转相除求路径游程编码定理）**：
从根 $\frac{1}{1}$ 走向既约分数 $\frac{p}{q}$ 的过程，与对 $(p, q)$ 做辗转相除法完全对偶：
- 当 $p < q$ 时，向左走的连续步数恰为 $n = \lfloor (q - 1) / p \rfloor$，走完后剩余比值转化为 $(p,\ q - n p)$；
- 当 $p > q$ 时，向右走的连续步数恰为 $n = \lfloor (p - 1) / q \rfloor$，走完后剩余比值转化为 $(p - n q,\ q)$；
- 当 $p = q = 1$ 时到达目标节点。
由此不仅能在 $O(\log \max(p, q))$ 时间内完成 `ENCODE_PATH`，而且：
- `DECODE_PATH` 与 `RANGE` 只需从初始区间 $\big(\frac{0}{1}, \frac{1}{0}\big)$ 按游程步数做线性组合即可直接得到区间端点 $(a, b), (c, d)$ 及节点值 $(a+c, b+d)$；
- `LCA` 只需编码出两分数的路径，取其**最长公共游程前缀**再解码；
- `ANCESTOR` 只需编码出路径，若总步数 $< k$ 则输出 `-1`，否则截取前 $k$ 步解码！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将 Stern-Brocot 树节点与法雷边界对 $\big(\frac{a}{b}, \frac{c}{d}\big)$ 双射，连续 $n$ 步 `L`/`R` 对应边界向量的 $n$ 倍累加（引理 1）。
- **classic problem solution** — 用 `encode(p, q)`（辗转相除）与 `decode(path)`（边界线性组合）两个核心原语统一实现全部 $5$ 个子任务。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `Path` | `vector<pair<char,i64>>` | 游程编码路径 `{(dir, count), ...}` | `encode` 生成 | run-length path |
| `encode(p, q)` | 函数 | 用整除算出 $\frac{p}{q}$ 的游程路径 | $O(\log \max(p,q))$ | Euclidean encode |
| `decode_range(path)` | 函数 | 返回路径终点的区间 `(a, b, c, d)`（节点值为 `a+c, b+d`） | 初始 `(0,1,1,0)` | matrix decode |

### 4.2 算法步骤

1. 实现 `encode(p, q)`：当 $p \ne q$ 时，若 $p < q$ 则令 $n = (q - 1) / p$，记录 `('L', n)`，$q \gets q - n p$；若 $p > q$ 则令 $n = (p - 1) / q$，记录 `('R', n)`，$p \gets p - n q$。
2. 实现 `decode_range(path)`：初始化 `a = 0, b = 1, c = 1, d = 0`；遍历每段 `(dir, n)`：若 `dir == 'L'` 则 `c += n * a, d += n * b`；否则 `a += n * c, b += n * d`；返回 `{a, b, c, d}`（对应节点分数为 `{a + c, b + d}`）。
3. 对五种操作分别调用 `encode` 与 `decode_range` 输出结果。

### 4.3 复杂度分析

- **时间复杂度**：单次辗转相除段数不超过 $O(\log \max(p, q)) \le 90$，总时间复杂度为 $O(T \log(\max(p, q)))$（耗时约 $80\text{ ms}$）。
- **空间复杂度**：每次查询的游程向量长度 $\le 90$，空间复杂度为 $O(\log(\max(p, q)))$。

---

## 5. 示例代码

```cpp
//
// 题目：P1797 【模板】Stern-Brocot 树
// https://www.luogu.com.cn/problem/P1797
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

using Path=vector<pair<char,i64>>;

Path encode(i64 p,i64 q)
{
	Path res;
	while(p!=q)
	{
		if(p<q)
		{
			i64 k=(q-1)/p;
			res.push_back({'L',k});
			q-=k*p;
		}
		else
		{
			i64 k=(p-1)/q;
			res.push_back({'R',k});
			p-=k*q;
		}
	}
	return res;
}

array<i64,4>decode_range(const Path&path)
{
	i64 a=0,b=1,c=1,d=0;
	for(auto[dir,k]:path)
	{
		if(dir=='L')
		{
			c+=k*a;
			d+=k*b;
		}
		else
		{
			a+=k*c;
			b+=k*d;
		}
	}
	return {a,b,c,d};
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
	{
		string op;
		cin>>op;
		if(op=="ENCODE_PATH")
		{
			i64 p,q;
			cin>>p>>q;
			auto path=encode(p,q);
			cout<<path.size();
			for(auto[dir,k]:path)
				cout<<' '<<dir<<' '<<k;
			cout<<'\n';
		}
		else if(op=="DECODE_PATH")
		{
			int k;
			cin>>k;
			Path path(k);
			for(int i=0;i<k;++i)
				cin>>path[i].first>>path[i].second;
			auto[a,b,c,d]=decode_range(path);
			cout<<a+c<<' '<<b+d<<'\n';
		}
		else if(op=="LCA")
		{
			i64 a,b,c,d;
			cin>>a>>b>>c>>d;
			auto p1=encode(a,b),p2=encode(c,d);
			Path lca;
			for(size_t i=0;i<p1.size()&&i<p2.size();++i)
			{
				if(p1[i].first!=p2[i].first)
					break;
				i64 mn=min(p1[i].second,p2[i].second);
				lca.push_back({p1[i].first,mn});
				if(p1[i].second!=p2[i].second)
					break;
			}
			auto[ra,rb,rc,rd]=decode_range(lca);
			cout<<ra+rc<<' '<<rb+rd<<'\n';
		}
		else if(op=="ANCESTOR")
		{
			i64 k,a,b;
			cin>>k>>a>>b;
			auto path=encode(a,b);
			i64 tot=0;
			for(auto[dir,cnt]:path)
				tot+=cnt;
			if(k>tot)
			{
				cout<<"-1\n";
				continue;
			}
			Path pref;
			for(auto[dir,cnt]:path)
			{
				if(k<=0)
					break;
				i64 take=min(k,cnt);
				pref.push_back({dir,take});
				k-=take;
			}
			auto[ra,rb,rc,rd]=decode_range(pref);
			cout<<ra+rc<<' '<<rb+rd<<'\n';
		}
		else if(op=="RANGE")
		{
			i64 p,q;
			cin>>p>>q;
			auto[a,b,c,d]=decode_range(encode(p,q));
			cout<<a<<' '<<b<<' '<<c<<' '<<d<<'\n';
		}
	}
	return 0;
}
```
