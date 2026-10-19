// 暴力基准:直接枚举"到底是哪个签收点把时刻卡住"的 O(n^2) 精确公式
// 与二分版相互独立:这里完全不做模拟,只按 (S_i-S_j)/(y_i-x_j) 取最大
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{200005};

int n;
i64 x[mxn],y[mxn],s[mxn],S[mxn];  // S[i]=前 i 段距离和, x[0]=0,S[0]=0

int main()
{
	if(!(cin>>n))
		return 0;
	for(int i=1;i<=n;++i)
		cin>>x[i]>>y[i]>>s[i];

	S[0]=0;x[0]=0;
	for(int i=1;i<=n;++i)
		S[i]=S[i-1]+s[i];

	double best=0;                    // v 越大越容易达标, 取所有下界的最大值
	for(int i=1;i<=n;++i)
		for(int j=0;j<i;++j)          // j 为"上一个卡住时刻"的地点(0 表示出发地)
		{
			double r=(double)(S[i]-S[j])/(double)(y[i]-x[j]);
			if(r>best)
				best=r;
		}
	printf("%.2f\n",best+1e-9);       // 与标程同样加 1e-9, 保证半分位舍入一致
	return 0;
}
