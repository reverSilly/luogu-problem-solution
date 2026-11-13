// 题目：P2436 钦定
// 固定周期 L 时 A 的可行集是区间 [mg, min(L-1,mb-1)]
// 枚举 L<=P+1，每个 L 取左端点生成候选，字典序最小即答案
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int inf{1145141919};

int T,n,m,na,nb,mxa,mxb,mnb,P;
int a[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>T;
	while(T--)
	{
		cin>>n>>m;
		for(int i=0;i<n;++i)cin>>a[i];
		for(int i=0;i<m;++i)cin>>b[i];
		sort(a,a+n);na=unique(a,a+n)-a;
		sort(b,b+m);nb=unique(b,b+m)-b;
		mxa=a[na-1];mxb=b[nb-1];mnb=b[0];
		P=max(mxa,mxb);
		int Lmax=P+1;
		if(mxa>=mnb)Lmax=min(Lmax,mxa-1);	// L>=mxa 时 mg=mxa>=mnb>=mb，必空
		int bestA=inf,bestB=inf;
		for(int L=2;L<=Lmax;++L)
		{
			int mb;
			if(L>=mxb)mb=mnb;		// 蒟蒻全不绕回，r=b
			else
			{
				mb=L+1;
				for(int i=0;i<nb;++i)
				{
					int r=(b[i]-1)%L+1;
					if(r<mb)mb=r;
					if(mb==1)break;	// hi<=0，无解
				}
			}
			if(mb<=1)continue;
			int hi=min(L-1,mb-1),mg;
			if(L>=mxa)mg=mxa;		// 神犇全不绕回，r=a
			else
			{
				mg=0;
				for(int i=0;i<na;++i)
				{
					int r=(a[i]-1)%L+1;
					if(r>mg)mg=r;
					if(mg>hi)break;	// 已超右端点，无解
				}
			}
			if(mg>hi||mg<1)continue;
			int A=mg,B=L-mg;
			if(A<bestA||(A==bestA&&B<bestB)){bestA=A;bestB=B;}
			if(bestA==1&&L>bestA+bestB)break;
			if(L>bestA+bestB&&L>=mxa&&mxa>=bestA)break;
		}
		if(bestA==inf)cout<<"NO\n";
		else cout<<bestA<<' '<<bestB<<'\n';
	}
	return 0;
}
