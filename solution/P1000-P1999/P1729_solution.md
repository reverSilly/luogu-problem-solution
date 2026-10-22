# P1729 计算e

## 1. 重新读题

给定正整数 $n$（$n \le 10^4$），利用级数 $e = \sum_{k=0}^\infty \frac{1}{k!}$ 计算自然常数 $e$ 小数点后 $n$ 位。第一行输出 `2.`，从第二行开始输出小数部分，每 $10$ 个数字空一格，每 $50$ 个数字换一行。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：阶乘级数的极速收敛与余项上界

**引理 1（阶乘级数截断余项估计）**：记 $t_k = \frac{1}{k!}$，则递推关系为 $t_0 = 1$、$t_k = \frac{t_{k-1}}{k}$（$k \ge 1$），且第 $K$ 项之后的无穷余项满足
$$R_K = \sum_{k=K+1}^{\infty} \frac{1}{k!} < \frac{1}{K! \cdot K}.$$

**证明**：
将 $k > K$ 的各项提出公因式 $\frac{1}{K!}$：
$$R_K = \frac{1}{K!} \left(\frac{1}{K+1} + \frac{1}{(K+1)(K+2)} + \cdots \right) < \frac{1}{K!} \sum_{j=1}^{\infty} \frac{1}{(K+1)^j} = \frac{1}{K! \cdot K}.$$
当 $K \approx 3250$ 时，$\log_{10}(K!) > 10005$，余项 $R_K < 10^{-10008}$。因此只需用基数 $10^9$ 的压位定点数递推执行 `term /= k` 并累加进 `ans`，直到 `term` 变为全 $0$（约 $3250$ 步），即可在数毫秒内获得精确到小数点后 $10^4$ 位的结果！$\blacksquare$

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用相邻项比值 $t_k = t_{k-1} / k$ 避免单独计算大阶乘，且利用阶乘超指数增长将迭代步数控制在 $\le 3250$ 步（引理 1）。
- **classic problem solution** — 采用基数 $10^9$ 的压位定点高精度数组，反复执行单精度除法与高精度加法直至当前项归零。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 小数点后位数 | 标准输入 | digits |
| `B` | `int` | 压位块数 $n / 9 + 4$ | 计算得出 | blocks |
| `ans, term` | `vector<int>` | 基数 $10^9$ 的定点小数数组 | `ans[0]=2, term[0]=1` | accumulator / term |

### 4.2 算法步骤

1. 读入 $n$，设块数 `B = n / 9 + 4`。
2. 初始化 $k=0,1$ 的和 `ans[0] = 2`，当前项 `term[0] = 1`。
3. 从 $k = 2$ 开始循环：将 `term` 除以 $k$；若结果全为 $0$ 则跳出；否则将 `term` 加到 `ans` 上。
4. 将 `ans[1 .. B-1]` 转为十进制数字串，第一行输出 `2.`，随后按每 $10$ 位一空格、每 $50$ 位一换行输出前 $n$ 位。

### 4.3 复杂度分析

- **时间复杂度**：迭代步数 $K \approx 3250$（满足 $\log_{10}(K!) \approx n$），每步耗时 $O(n/9)$，总运算量约 $3.6 \times 10^6$ 次，耗时 $< 10\text{ ms}$。
- **空间复杂度**：两个长度为 $O(n/9)$ 的数组，空间复杂度为 $O(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1729 计算e
// https://www.luogu.com.cn/problem/P1729
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int BASE{1000000000};

bool div_int(vector<int>&a,int d)
{
	i64 rem=0;
	bool nz=false;
	for(size_t i=0;i<a.size();++i)
	{
		i64 cur=rem*BASE+a[i];
		a[i]=(int)(cur/d);
		rem=cur%d;
		if(a[i])
			nz=true;
	}
	return nz;
}

void add_to(vector<int>&a,const vector<int>&b)
{
	int carry=0;
	for(int i=(int)a.size()-1;i>=0;--i)
	{
		int s=a[i]+b[i]+carry;
		if(s>=BASE){a[i]=s-BASE;carry=1;}
		else{a[i]=s;carry=0;}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	int B=n/9+4;
	vector<int>ans(B,0),term(B,0);
	ans[0]=2;
	term[0]=1;
	for(int k=2;div_int(term,k);++k)
		add_to(ans,term);

	string s;
	s.reserve((B-1)*9);
	for(int i=1;i<B;++i)
	{
		string blk=to_string(ans[i]);
		s+=string(9-blk.size(),'0')+blk;
	}

	cout<<"2.\n";
	for(int i=0;i<n;++i)
	{
		cout<<s[i];
		if((i+1)%50==0)
			cout<<'\n';
		else if((i+1)%10==0)
			cout<<' ';
	}
	if(n%50!=0)
		cout<<'\n';
	return 0;
}
```
