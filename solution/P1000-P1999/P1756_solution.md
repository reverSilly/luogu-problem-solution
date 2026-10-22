# P1756 [NOI2009] 描边

## 1. 重新读题

平面上给定 $n$ 个点和其中的 $m$ 条重点线段，用半径为 $r$ 的圆形毛笔沿每条线段从一端描向另一端（每条线段被描出的区域是一个由长为 $L$、宽为 $2r$ 的矩形与两端半径为 $r$ 的半圆拼成的**胶囊体 / Stadium**）。求这 $m$ 个胶囊体并集的总面积。

---

## 2. 思维链（Chain-of-thought）

### 2.1 探索性质：点到线段距离 $\le r$ 的扫描线截面与自适应辛普森积分

第 $k$ 条线段 $A_k B_k$ 被半径 $r$ 的圆描出的胶囊体，其数学定义为平面上到线段 $A_k B_k$ 的欧氏距离 $\le r$ 的点集：
$$C_k = \big\{ P \in \mathbb{R}^2 \;\big|\; \operatorname{dist}(P, A_k B_k) \le r \big\}.$$
求多个复杂几何图形并集的面积，最通用且高精度的利器是**水平截面长度函数 $f(x)$ + 自适应辛普森积分（Adaptive Simpson's Rule）**！

**引理 1（竖直扫描线与胶囊体的截面区间公式）**：
固定横坐标 $x$，竖直线 $X = x$ 与胶囊体 $C_k$（端点 $A=(x_1,y_1), B=(x_2,y_2)$，不妨设 $x_1 \le x_2$）的交集是一个闭区间（或空集）：
1. 直线 $X = x$ 与两端点圆盘 $\operatorname{dist}(P, A) \le r$、$\operatorname{dist}(P, B) \le r$ 的交集分别为：
   - 当 $|x - x_1| \le r$ 时，$\big[y_1 - \sqrt{r^2 - (x-x_1)^2},\ y_1 + \sqrt{r^2 - (x-x_1)^2}\big]$；
   - 当 $|x - x_2| \le r$ 时，$\big[y_2 - \sqrt{r^2 - (x-x_2)^2},\ y_2 + \sqrt{r^2 - (x-x_2)^2}\big]$。
2. 当 $x_1 < x_2$ 时，设线段倾角为 $\theta$（$\Delta x = x_2 - x_1, \Delta y = y_2 - y_1, L = \sqrt{\Delta x^2 + \Delta y^2}$），中间矩形条带由两条平行线和两条垂直端线围成，将这 $4$ 个半平面约束限制在竖直线 $X = x$ 上，直接取交集即得矩形条带在 $X = x$ 上的截面区间 $[l_{\text{rect}}, r_{\text{rect}}]$。
由于圆盘与矩形条带都是凸集且它们的并集在任意竖直线上连通，胶囊体 $C_k$ 在 $X = x$ 上的截面就是上述（至多 $3$ 个）非空区间的**极小左端点到极大右端点**构成的单个闭区间！

**证明**：胶囊体 $C_k$ 是凸集（线段与圆盘的闵可夫斯基和），任何直线与凸集的交集必为单个连通闭区间（或空集）。又因为 $C_k = D(A, r) \cup D(B, r) \cup \text{Rect}(A, B, r)$，故 $X = x$ 与 $C_k$ 的交集等于它与三部分交集的并集；既然并集连通，其左端点必为各部分左端点的最小值，右端点必为各部分右端点的最大值。$\blacksquare$

### 2.2 分段自适应辛普森积分

有了单点截面长度函数 $f(x)$（求出所有胶囊体在 $X=x$ 上的区间后，按左端点排序合并求并集长度），我们将所有特征横坐标（$x_i \pm r$）排序切分为若干子区间，在每个子区间上运行自适应辛普森积分：
$$S(l, r) = \frac{r - l}{6}\big(f(l) + 4 f(\tfrac{l+r}{2}) + f(r)\big),$$
当 $|S(l, m) + S(m, r) - S(l, r)| \le 15 \varepsilon$ 时收敛返回，即可精确求出并集面积！

---

## 3. 模式总结（Pattern Summary）

- **explore its property** — 将毛笔描边识别为线段与圆盘的闵可夫斯基和（凸胶囊体），利用凸性将单条胶囊体在 $X=x$ 处的截面化为圆盘截面与矩形条带截面的外包区间（引理 1）。
- **classic problem solution** — 用区间合并计算截面总长 $f(x)$，按特征横坐标分段运行自适应辛普森积分。

---

## 4. 解法说明（The Real Solution）

### 4.1 符号表

| 符号 | 类型 / 尺寸 | 含义 | 初值 / 来源 | 助记 |
| :--- | :--- | :--- | :--- | :--- |
| `n, m` | `int` | 点数与重点线段数 | 标准输入 | 维度 |
| `rad` | `double` | 毛笔半径 $r$ | 标准输入 | radius |
| `seg[i]` | `Capsule` | 预处理后的线段几何参数 | 读入构建 | capsules |
| `f(x)` | 函数 | 横坐标 $x$ 处所有胶囊体截面的并集总长度 | 排序合并 | cross-section |

### 4.2 算法步骤

1. 读入 $n$ 个点坐标、$m$ 条线段及毛笔半径 `rad`。
2. 实现截面函数 `f(x)`：对每条线段合并两端圆盘与中间倾斜矩形在 $X = x$ 处的 $y$ 区间，将所有非空区间排序合并累加长度。
3. 将横坐标范围按均匀网格/特征点切分，对每段调用自适应辛普森积分 `asr(l, r, eps, simpson(l, r))` 并累加输出。

### 4.3 复杂度分析

- **时间复杂度**：单次计算 $f(x)$ 耗时 $O(m \log m)$，自适应辛普森积分在平滑段快速收敛，仅在交点附近加密采样。
- **空间复杂度**：仅存储 $n$ 个点和 $m$ 条线段，空间复杂度为 $O(n + m)$。

---

## 5. 示例代码

```cpp
//
// 题目：P1756 [NOI2009] 描边
// https://www.luogu.com.cn/problem/P1756
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr double eps{1e-7};

int n,m;
double rad;
struct pt{double x,y;}p[1005];
struct seg_t{pt a,b;double dx,dy,len;}segs[5005];

double f(double x)
{
	static vector<pair<double,double>>itvs;
	itvs.clear();
	for(int i=0;i<m;++i)
	{
		const auto&s=segs[i];
		if(x<min(s.a.x,s.b.x)-rad||x>max(s.a.x,s.b.x)+rad)
			continue;
		double ylo=1e18,yhi=-1e18;
		auto add_disk=[&](pt c){
			double d=fabs(x-c.x);
			if(d<=rad)
			{
				double h=sqrt(max(0.0,rad*rad-d*d));
				ylo=min(ylo,c.y-h);
				yhi=max(yhi,c.y+h);
			}
		};
		add_disk(s.a);
		add_disk(s.b);
		if(s.len>1e-11)
		{
			// 矩形满足：0 <= (P - A) . (B - A) <= len^2 且 |(P - A) x (B - A)| <= rad * len
			double rlo=-1e18,rhi=1e18;
			auto clip=[&](double A_coef,double B_bound){
				// A_coef * y <= B_bound
				if(fabs(A_coef)<1e-12)
				{
					if(B_bound<-1e-9){rlo=1;rhi=0;}
				}
				else if(A_coef>0)
					rhi=min(rhi,B_bound/A_coef);
				else
					rlo=max(rlo,B_bound/A_coef);
			};
			double X=x-s.a.x;
			clip(s.dy,s.len*s.len-X*s.dx);
			clip(-s.dy,X*s.dx);
			clip(s.dx,rad*s.len+X*s.dy);
			clip(-s.dx,rad*s.len-X*s.dy);
			if(rlo<=rhi)
			{
				ylo=min(ylo,s.a.y+rlo);
				yhi=max(yhi,s.a.y+rhi);
			}
		}
		if(ylo<=yhi)
			itvs.push_back({ylo,yhi});
	}
	if(itvs.empty())
		return 0.0;
	sort(itvs.begin(),itvs.end());
	double tot=0,cl=itvs[0].first,cr=itvs[0].second;
	for(size_t i=1;i<itvs.size();++i)
	{
		if(itvs[i].first<=cr)
			cr=max(cr,itvs[i].second);
		else
		{
			tot+=cr-cl;
			cl=itvs[i].first;
			cr=itvs[i].second;
		}
	}
	tot+=cr-cl;
	return tot;
}

double simpson(double l,double r)
{
	double mid=(l+r)*0.5;
	return (r-l)*(f(l)+4.0*f(mid)+f(r))/6.0;
}

double asr(double l,double r,double e,double A,int dep)
{
	double mid=(l+r)*0.5;
	double L=simpson(l,mid),R=simpson(mid,r);
	if(dep<=0&&fabs(L+R-A)<=15.0*e)
		return L+R+(L+R-A)/15.0;
	return asr(l,mid,e*0.5,L,dep-1)+asr(mid,r,e*0.5,R,dep-1);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=1;i<=n;++i)
		cin>>p[i].x>>p[i].y;
	cin>>m;
	double xlo=1e18,xhi=-1e18;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;
		segs[i].a=p[u];
		segs[i].b=p[v];
		segs[i].dx=p[v].x-p[u].x;
		segs[i].dy=p[v].y-p[u].y;
		segs[i].len=hypot(segs[i].dx,segs[i].dy);
		xlo=min({xlo,p[u].x,p[v].x});
		xhi=max({xhi,p[u].x,p[v].x});
	}
	cin>>rad;
	xlo-=rad;
	xhi+=rad;

	double ans=0;
	int steps=200;
	double step=(xhi-xlo)/steps;
	for(int i=0;i<steps;++i)
	{
		double l=xlo+i*step,r=l+step;
		ans+=asr(l,r,eps/steps,simpson(l,r),2);
	}
	cout<<fixed<<setprecision(6)<<ans<<'\n';
	return 0;
}
```
