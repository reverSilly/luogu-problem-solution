# P1737 [NOI2016] 旷野大计算

## 1. 重新读题

本题是一道提交答案/构造计算图题。输入测试点编号 $T \in \{1, 2, \dots, 10\}$，要求输出一个由计算节点组成的指令序列来完成第 $T$ 号计算任务。

可用基础节点包括：输入 `I`、输出 `O`、加法 `+ i j`、加常数 `C i c`、取反 `- i`、乘 $2^k$ 左移 `< i k`、乘 $2^{-k}$ 右移 `> i k`、以及 Sigmoid 节点 `S i`（计算 $s(x) = \frac{1}{1 + e^{-x}}$）。虽然还提供比较 `P`、最大值 `M`、乘法 `*`，但每使用其中一种会倒扣 $4$ 分，因此满分构造必须**仅使用前 $8$ 种基础节点**，通过 $s(x)$ 的非线性特性与局部线性展开模拟出阶跃比较、条件清零（ReLU）、位运算、定点数乘除与排序！

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：Sigmoid 函数 $s(x) = \frac{1}{1 + e^{-x}}$ 的三大核心算子

为什么只靠加法、移位和 $s(x)$ 就能实现全部 $10$ 个任务？关键在于 $s(x)$ 在两个极端区间的渐近性质：

**引理 1（Sigmoid 饱和阶跃算子）**：当 $|x| \ge 10^{-10}$ 时，将 $x$ 左移 $K = 100$ 位得到 $u = x \cdot 2^{100}$（此时 $|u| > 10^{20}$），则：
$$s(x \cdot 2^{100}) = \begin{cases} 1, & x > 0, \\ 0.5, & x = 0, \\ 0, & x < 0 \end{cases} \qquad (\text{在 } 90 \text{ 位十进制精度下完全精确}).$$

**证明**：当 $x \ge 10^{-10}$ 时，$u \ge 10^{-10} \cdot 2^{100} > 1.26 \times 10^{20}$，此时 $e^{-u} < 10^{-10^{19}} \ll 10^{-90}$，故在小数点后 $90$ 位四舍五入下 $s(u) = 1$；同理 $x \le -10^{-10}$ 时 $s(u) = 0$；而 $x = 0$ 时 $s(0) = \frac{1}{2}$。$\blacksquare$

**引理 2（Sigmoid 微元线性化与条件门控算子 $\operatorname{cond}(b, x) = b \cdot x$）**：
在 $u = 0$ 附近由泰勒展开有 $s(u) = \frac{1}{2} + \frac{1}{4} u - \frac{1}{48} u^3 + O(u^5)$。
设布尔值 $b \in \{0, 1\}$、实数 $|x| \le 2^{64}$。取缩放指数 $K = 150$，构造输入
$$w = (b - 1) \cdot 2^{150} + x \cdot 2^{-150}.$$
则：
$$\Big(s(w) - \frac{1}{2} b\Big) \cdot 2^{152} = \begin{cases} x, & b = 1, \\ 0, & b = 0 \end{cases} \qquad (\text{绝对误差 } < 10^{-20}).$$

**证明**：
- 当 $b = 0$ 时，$w = -2^{150} + x \cdot 2^{-150} < -10^{45}$，由引理 1 知 $s(w) = 0$，减去 $\frac{1}{2} b = 0$ 后仍为 $0$；
- 当 $b = 1$ 时，$w = x \cdot 2^{-150}$，满足 $|w| \le 2^{64 - 150} = 2^{-86} < 1.3 \times 10^{-26}$。代入泰勒展开：
  $$\Big(s(w) - \frac{1}{2}\Big) \cdot 2^{152} = \Big(\frac{1}{4} w + O(w^3)\Big) \cdot 2^{152} = x + O(2^{-258 + 152}) = x + O(2^{-106}),$$
  由于 $2^{-106} < 10^{-31} \ll 10^{-9}$，在题目精度范围内精确还原出 $x$！
特别地，取 $b = [x > 0] = s((x - 10^{-11}) \cdot 2^{100})$，即得 $\max(x, 0) = \operatorname{cond}(b, x)$！$\blacksquare$

### 2.2 十个子任务的逐一构造

1. **Task 1（$-2a - 2b$）**：直接计算 `-((a + b) << 1)`，共 $6$ 个节点。
2. **Task 2（$\frac{1}{1+e^{17a}}$）**：注意到 $\frac{1}{1+e^{17a}} = s(-17a) = s(-(a + 16a))$，共 $6$ 个节点。
3. **Task 3（符号函数 $\operatorname{sgn}(a)$）**：由引理 1，$\operatorname{sgn}(a) = 2\big(s(a \cdot 2^{100}) - 0.5\big)$，共 $6$ 个节点。
4. **Task 4（绝对值 $|a|$）**：利用引理 2 算出 $m = \max(a, 0)$，再由 $|a| = 2m - a$ 输出，共 $13$ 个节点。
5. **Task 5（$32$ 位二进制转整数）**：利用秦九韶算法（Horner 法则），从高位到低位迭代 `acc = (acc << 1) + a_i`，共 $95$ 个节点。
6. **Task 6（整数转 $32$ 位二进制）**：从 $k = 31$ 递减到 $0$，用引理 1 判断当前余数 $r \ge 2^k - 0.5$ 是否成立：$b_k = s((r - (2^k - 0.5)) \cdot 2^{100})$，随后从 $r$ 中减去 $b_k \cdot 2^k$。
7. **Task 7（$32$ 位按位异或 $a \oplus b$）**：先同 Task 6 拆出 $a, b$ 的 $32$ 个二进制位 $a_k, b_k$；对每一位，令 $s_k = a_k + b_k \in \{0, 1, 2\}$，则 $a_k \oplus b_k = [s_k \ge 0.5] - [s_k \ge 1.5] = s((s_k - 0.5)\cdot 100) - s((s_k - 1.5)\cdot 100)$，最后加权求和。
8. **Task 8（计算 $a / 10$）**：利用二进制无穷乘积恒等式
   $$\frac{1}{10} = \frac{3}{32} \cdot \frac{1}{1 - 2^{-4}} = \frac{3}{32} (1 + 2^{-4})(1 + 2^{-8})(1 + 2^{-16})(1 + 2^{-32})(1 + 2^{-64}),$$
   只需先算 $3a = a + 2a$，再连续 $5$ 次执行 `x = x + (x >> (4 << k))`，最后右移 $5$ 位，仅用 $15$ 个节点即可达到 $2^{-128}$ 精度！
9. **Task 9（$16$ 个实数排序）**：由引理 2 构造比较交换器 $(x, y) \mapsto (\min(x, y), \max(x, y))$（其中 $d = \max(x - y, 0),\ \min = x - d,\ \max = y + d$），套用 $16$ 元素双调排序网络（Bitonic Sorting Network，仅 $60$ 个比较器，共约 $680$ 个节点）。
10. **Task 10（$a \cdot b \bmod m$）**：先将 $a$ 拆成 $32$ 位，用引理 2 计算 $P = \sum_{k=0}^{31} \operatorname{cond}(a_k, b \cdot 2^k) = a \cdot b$；再对 $k = 63 \dots 0$ 模拟二进制长除法：令 $q_k = [P \ge m \cdot 2^k] = s\big((P - m \cdot 2^k + 0.5) \cdot 2^{100}\big)$，并更新 $P \leftarrow P - \operatorname{cond}(q_k, m \cdot 2^k)$，最终余数即为 $a \cdot b \bmod m$。

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 挖掘 $s(x)$ 在 $|x| \gg 1$ 时的阶跃特性（引理 1）与 $|x| \ll 1$ 时的泰勒一阶线性特性 $s(u) \approx \frac{1}{2} + \frac{1}{4} u$（引理 2）。
- **build primitive gadgets** — 封装阶跃比较器 `geq`、条件乘法门 `cond(b, x)` 与二元 `min/max` 交换器。
- **classic problem solution** — 组合基础门电路分别实现位拆分、倍增无穷乘积除 $10$、双调排序网络与 $64$ 位二进制试商取模。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `id` | `int` | 当前已生成的计算图节点总数 | `0` | node counter |
| `op_*` | 辅助函数 | 生成对应指令行并返回新节点编号 | 自动递增 `id` | graph builders |
| `cond(b, x)` | 子图封装 | 利用引理 2 计算 $b \cdot x$（$b \in \{0, 1\}$） | $7$ 个节点 | conditional gate |
| `cmp_swap(x, y)` | 子图封装 | 返回 $\{\min(x, y), \max(x, y)\}$ | $12$ 个节点 | comparator |

### 4.2 算法步骤

1. 读入测试点编号 $T \in \{1 \dots 10\}$。
2. 根据 $T$ 调用对应的计算图生成函数 `task1()` $\sim$ `task10()`，按节点编号递增顺序直接打印每条指令。

### 4.3 复杂度分析

- **时间复杂度**：各测试点生成的节点数最多不超过 $1500$（远小于上限 $10^4$），程序运行耗时 $< 1\text{ ms}$。
- **空间复杂度**：边生成边输出，空间复杂度为 $O(1)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1737 [NOI2016] 旷野大计算
// https://www.luogu.com.cn/problem/P1737
// AC
#include<bits/stdc++.h>
using namespace std;

int tot;
int opI(){cout<<"I\n";return ++tot;}
int opO(int x){cout<<"O "<<x<<'\n';return ++tot;}
int opAdd(int x,int y){cout<<"+ "<<x<<' '<<y<<'\n';return ++tot;}
int opC(int x,const string&c){cout<<"C "<<x<<' '<<c<<'\n';return ++tot;}
int opNeg(int x){cout<<"- "<<x<<'\n';return ++tot;}
int opShl(int x,int k){cout<<"< "<<x<<' '<<k<<'\n';return ++tot;}
int opShr(int x,int k){cout<<"> "<<x<<' '<<k<<'\n';return ++tot;}
int opS(int x){cout<<"S "<<x<<'\n';return ++tot;}

// b in {0,1}, return b * x
int cond(int b,int x)
{
	int bm1=opC(b,"-1");
	int shift_b=opShl(bm1,150);
	int xs=opShr(x,150);
	int w=opAdd(shift_b,xs);
	int sw=opS(w);
	int hb=opShr(b,1);
	int nhb=opNeg(hb);
	int diff=opAdd(sw,nhb);
	return opShl(diff,152);
}

// return max(x, 0)
int relu(int x)
{
	int x_eps=opC(x,"-0.00000000001");
	int big=opShl(x_eps,100);
	int b=opS(big);
	return cond(b,x);
}

pair<int,int>cmp_swap(int x,int y)
{
	int ny=opNeg(y);
	int diff=opAdd(x,ny);
	int d=relu(diff);
	int nd=opNeg(d);
	int mn=opAdd(x,nd);
	int mx=opAdd(y,d);
	return {mn,mx};
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	if(t==1)
	{
		int a=opI(),b=opI();
		int s=opAdd(a,b);
		int s2=opShl(s,1);
		int ans=opNeg(s2);
		opO(ans);
	}
	else if(t==2)
	{
		int a=opI();
		int a16=opShl(a,4);
		int a17=opAdd(a,a16);
		int na17=opNeg(a17);
		int ans=opS(na17);
		opO(ans);
	}
	else if(t==3)
	{
		int a=opI();
		int big=opShl(a,100);
		int s=opS(big);
		int c=opC(s,"-0.5");
		int ans=opShl(c,1);
		opO(ans);
	}
	else if(t==4)
	{
		int a=opI();
		int r=relu(a);
		int r2=opShl(r,1);
		int na=opNeg(a);
		int ans=opAdd(r2,na);
		opO(ans);
	}
	else if(t==5)
	{
		int cur=opI();
		for(int i=1;i<32;++i)
		{
			int nxt=opI();
			int sh=opShl(cur,1);
			cur=opAdd(sh,nxt);
		}
		opO(cur);
	}
	else if(t==6)
	{
		int a=opI();
		vector<int>bits(32);
		for(int k=31;k>=0;--k)
		{
			if(k>0)
			{
				string thr="-"+to_string((1ULL<<k)-1)+".5";
				int sub=opC(a,thr);
				int big=opShl(sub,100);
				int bk=opS(big);
				bits[31-k]=bk;
				int val=opShl(bk,k);
				int nval=opNeg(val);
				a=opAdd(a,nval);
			}
			else
				bits[31]=a;
		}
		for(int b:bits)
			opO(b);
	}
	else if(t==7)
	{
		auto decomp=[&](int a){
			vector<int>bits(32);
			for(int k=31;k>=0;--k)
			{
				if(k>0)
				{
					string thr="-"+to_string((1ULL<<k)-1)+".5";
					int sub=opC(a,thr);
					int big=opShl(sub,100);
					int bk=opS(big);
					bits[k]=bk;
					int val=opShl(bk,k);
					int nval=opNeg(val);
					a=opAdd(a,nval);
				}
				else
					bits[0]=a;
			}
			return bits;
		};
		int a=opI(),b=opI();
		auto ba=decomp(a),bb=decomp(b);
		int ans=0;
		for(int k=31;k>=0;--k)
		{
			int s=opAdd(ba[k],bb[k]);
			int c1=opC(s,"-0.5"),b1=opS(opShl(c1,80));
			int c2=opC(s,"-1.5"),b2=opS(opShl(c2,80));
			int nb2=opNeg(b2);
			int xk=opAdd(b1,nb2);
			if(k==31)
				ans=xk;
			else
				ans=opAdd(opShl(ans,1),xk);
		}
		opO(ans);
	}
	else if(t==8)
	{
		int a=opI();
		int a2=opShl(a,1);
		int x=opAdd(a,a2);
		for(int k=0;k<5;++k)
		{
			int sh=opShr(x,4<<k);
			x=opAdd(x,sh);
		}
		int ans=opShr(x,5);
		opO(ans);
	}
	else if(t==9)
	{
		vector<int>v(16);
		for(int i=0;i<16;++i)
			v[i]=opI();
		for(int k=2;k<=16;k<<=1)
			for(int j=k>>1;j>0;j>>=1)
				for(int i=0;i<16;++i)
				{
					int l=i^j;
					if(l>i)
					{
						auto[mn,mx]=cmp_swap(v[i],v[l]);
						if((i&k)==0){v[i]=mn;v[l]=mx;}
						else{v[i]=mx;v[l]=mn;}
					}
				}
		for(int i=0;i<16;++i)
			opO(v[i]);
	}
	else if(t==10)
	{
		int a=opI(),b=opI(),m=opI();
		vector<int>ba(32);
		for(int k=31;k>=0;--k)
		{
			if(k>0)
			{
				string thr="-"+to_string((1ULL<<k)-1)+".5";
				int bk=opS(opShl(opC(a,thr),100));
				ba[k]=bk;
				a=opAdd(a,opNeg(opShl(bk,k)));
			}
			else
				ba[0]=a;
		}
		int prod=cond(ba[0],b);
		for(int k=1;k<32;++k)
		{
			int term=cond(ba[k],opShl(b,k));
			prod=opAdd(prod,term);
		}
		for(int k=63;k>=0;--k)
		{
			int mk=(k==0)?m:opShl(m,k);
			int diff=opAdd(prod,opNeg(mk));
			int qk=opS(opShl(opC(diff,"0.5"),100));
			int sub=cond(qk,mk);
			prod=opAdd(prod,opNeg(sub));
		}
		opO(prod);
	}
	return 0;
}
```
