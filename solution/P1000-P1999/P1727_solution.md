# P1727 计算π

## 1. 重新读题

给定正整数 $n$（$n \le 10^4$），计算圆周率 $\pi$ 小数点后 $n$ 位。第一行输出 `3.`，从第二行开始输出小数部分，每 $10$ 个数字空一格，每 $50$ 个数字换一行。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：收敛极快的梅钦公式（Machin's Formula）

如果用经典的莱布尼茨级数 $\frac{\pi}{4} = \sum_{k=0}^\infty \frac{(-1)^k}{2k+1}$，要想精确到小数点后 $10^4$ 位需要 $10^{10000}$ 项，完全不可行。我们需要**几何级数速度收敛**的反正切展开公式！

**引理 1（梅钦公式与泰勒级数展开）**：
$$\pi = 16 \arctan \frac{1}{5} - 4 \arctan \frac{1}{239} = \sum_{k=0}^{\infty} (-1)^k \frac{16}{(2k+1)\cdot 5^{2k+1}} - \sum_{k=0}^{\infty} (-1)^k \frac{4}{(2k+1)\cdot 239^{2k+1}}.$$

**证明**：
设 $\alpha = \arctan \frac{1}{5}$。由正切倍角公式：
$$\tan 2\alpha = \frac{2 \tan\alpha}{1 - \tan^2\alpha} = \frac{2/5}{1 - 1/25} = \frac{5}{12}, \qquad \tan 4\alpha = \frac{2 \tan 2\alpha}{1 - \tan^2 2\alpha} = \frac{10/12}{1 - 25/144} = \frac{120}{119}.$$
再由正切差角公式：
$$\tan\Big(4\alpha - \frac{\pi}{4}\Big) = \frac{\tan 4\alpha - 1}{1 + \tan 4\alpha} = \frac{120/119 - 1}{1 + 120/119} = \frac{1}{239}.$$
因 $\alpha \in (0, \pi/4)$ 且 $\tan 4\alpha = \frac{120}{119} \approx 1$，有 $4\alpha - \frac{\pi}{4} = \arctan \frac{1}{239}$，移项乘以 $4$ 即得 $\pi = 16\arctan\frac{1}{5} - 4\arctan\frac{1}{239}$。将 $\arctan x = \sum_{k=0}^\infty \frac{(-1)^k x^{2k+1}}{2k+1}$（$|x| \le 1$）代入即得右端级数。$\blacksquare$

### 2.2 迭代项数与定点数精度分析

在引理 1 的两个交错级数中：
- 第一个级数的第 $k$ 项绝对值 $< \frac{16}{5^{2k+1}}$，每增加 $1$ 项缩小至少 $25 = 10^{1.3979}$ 倍。要达到 $10^{-10005}$ 精度，只需约 $\frac{10005}{2 \log_{10} 5} \approx 7160$ 项；
- 第二个级数的第 $k$ 项每增加 $1$ 项缩小至少 $239^2 = 57121$ 倍，只需约 $2100$ 项即可归零！

更妙的是，设 $u_k = \frac{16}{5^{2k+1}}$，则 $u_0 = \frac{16}{5}$，且 $u_{k} = \frac{u_{k-1}}{25}$；设 $v_k = \frac{4}{239^{2k+1}}$，则 $v_0 = \frac{4}{239}$，且 $v_k = \frac{v_{k-1}}{57121}$。所有运算都只是**高精度定点数除以单精度整数**（除以 $25$、$57121$ 或 $2k+1$）以及高精度加减法！采用 $10^9$ 压位（每个 `int` 存 $9$ 位十进制数），数组长度仅约 $1120$，总运算量约 $10^7$ 次，耗时 $< 30\text{ ms}$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 利用梅钦公式 $\pi = 16\arctan(1/5) - 4\arctan(1/239)$ 将 $\pi$ 的计算转化为两个指数级收敛的交错幂级数（引理 1）。
- **classic problem solution** — 采用基数 $10^9$ 的压位定点高精度数组，递推维护幂项 $u_k = u_{k-1}/25$ 与 $v_k = v_{k-1}/57121$，将每项除以 $(2k+1)$ 后累加进答案。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n` | `int` | 要求保留的小数位数 | 标准输入 | digits |
| `B` | `int` | 压位定点数块数 $\lceil n/9 \rceil + 3$ | 计算得出 | blocks |
| `ans, p1, p2, tmp` | `vector<int>` | 基数 $10^9$ 的定点小数数组 | 长度 `B` | big fixed-point |

### 4.2 算法步骤

1. 读入 $n$，设块数 `B = n / 9 + 4`。
2. 初始化 `p1 = 16 / 5`（即 `p1[0] = 3, p1[1] = 200000000`），`p2 = 4` 并执行 `div_int(p2, 239)`。
3. 循环 $k = 0, 1, 2, \dots$ 直至 `p1` 全为 `0`：
   - 令 `tmp = p1`，执行 `div_int(tmp, 2 * k + 1)`；若 $k$ 为偶数则 `add(ans, tmp)`，否则 `sub(ans, tmp)`；
   - 若 `p2` 非零，令 `tmp = p2`，执行 `div_int(tmp, 2 * k + 1)`；若 $k$ 为偶数则 `sub(ans, tmp)`，否则 `add(ans, tmp)`；并更新 `div_int(p2, 57121)`；
   - 更新 `div_int(p1, 25)`。
4. 将 `ans` 的小数块展开为字符串，第一行打印 `3.`，随后按每 $10$ 位一空格、每 $50$ 位一换行的格式输出前 $n$ 位。

### 4.3 复杂度分析

- **时间复杂度**：外层迭代约 $\frac{n}{2 \log_{10} 5} \approx 0.715 n$ 次，每次做长度为 $\frac{n}{9}$ 的单精度除法与加减，总基本运算次数约为 $0.16 n^2$（$n = 10^4$ 时约 $1.6 \times 10^7$ 次），耗时约 $20\text{ ms}$。
- **空间复杂度**：$4$ 个长度为 $O(n/9)$ 的 `vector<int>`，空间复杂度为 $O(n)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1727 计算π
// https://www.luogu.com.cn/problem/P1727
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int BASE{1000000000};

void div_int(const vector<int>&a,int d,vector<int>&out,bool&nonzero)
{
	i64 rem=0;
	nonzero=false;
	for(size_t i=0;i<a.size();++i)
	{
		i64 cur=rem*BASE+a[i];
		out[i]=(int)(cur/d);
		rem=cur%d;
		if(out[i])
			nonzero=true;
	}
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

void sub_from(vector<int>&a,const vector<int>&b)
{
	int borrow=0;
	for(int i=(int)a.size()-1;i>=0;--i)
	{
		int s=a[i]-b[i]-borrow;
		if(s<0){a[i]=s+BASE;borrow=1;}
		else{a[i]=s;borrow=0;}
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
	vector<int>ans(B,0),p1(B,0),p2(B,0),tmp(B,0);
	p1[0]=16;
	bool nz1=true,nz2=true;
	div_int(p1,5,p1,nz1);
	p2[0]=4;
	div_int(p2,239,p2,nz2);

	for(int k=0;nz1;++k)
	{
		bool dummy=false;
		div_int(p1,2*k+1,tmp,dummy);
		if(k&1)sub_from(ans,tmp);
		else add_to(ans,tmp);

		if(nz2)
		{
			div_int(p2,2*k+1,tmp,dummy);
			if(k&1)add_to(ans,tmp);
			else sub_from(ans,tmp);
			div_int(p2,57121,p2,nz2);
		}
		div_int(p1,25,p1,nz1);
	}

	string s;
	s.reserve((B-1)*9);
	for(int i=1;i<B;++i)
	{
		string blk=to_string(ans[i]);
		s+=string(9-blk.size(),'0')+blk;
	}

	cout<<"3.\n";
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
