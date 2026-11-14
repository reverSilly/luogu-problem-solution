// 题目：P2721 小 Q 的赚钱计划
// 时间轴 0..365：t 表示第 t+1 天清晨（t=365 即年底）。
// 产品占用半开区间 [s,s+b)，s+b 时刻连本带利到账，因子 1+C*b/3650000。
// f[t]=t 时刻最多现金；闲钱向后传，产品向前跳。答案 f[365]。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxd{366};

int n,pre[13];
double f[mxd];
vector<pair<int,int>>g[mxd];	// g[s]：(投资天数 b, 年利率百分数×100 的整数 C)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	constexpr int md[12]{31,28,31,30,31,30,31,31,30,31,30,31};
	for(int i=1;i<=12;++i)
		pre[i]=pre[i-1]+md[i-1];
	cin>>n;
	for(int i=0;i<n;++i)
	{
		string A,C;int b;
		cin>>A>>b>>C;
		int mo=(A[0]-'0')*10+(A[1]-'0'),da=(A[2]-'0')*10+(A[3]-'0');
		int s=pre[mo-1]+da-1;	// 发行日 MMDD → 0-based 时间点
		int c=0,j=0;
		for(;j<(int)C.size()&&C[j]!='.';++j)
			c=c*10+(C[j]-'0');
		c*=100;
		if(j<(int)C.size())	// 小数部分：右补零至 2 位，化成整数百分之一
		{
			++j;
			int fp=0,k=0;
			for(;j<(int)C.size()&&k<2;++j,++k)
				fp=fp*10+(C[j]-'0');
			for(;k<2;++k)
				fp*=10;
			c+=fp;
		}
		g[s].push_back({b,c});
	}
	f[0]=1e5;
	for(int t=0;t<=365;++t)
	{
		if(t)
			f[t]=max(f[t],f[t-1]);	// 闲钱：这一天什么都不买
		for(auto[b,c]:g[t])
			if(t+b<=365)	// 年底后才到期的产品等于把钱锁死，直接忽略
				f[t+b]=max(f[t+b],f[t]*(1+c*b/3650000.0));
	}
	cout<<fixed<<setprecision(2)<<f[365]<<'\n';
	return 0;
}