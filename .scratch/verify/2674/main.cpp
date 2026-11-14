#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxk{1420};

int ng,cnt;
i64 kk[mxk];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>ng;
	while(ng--)
	{
		i64 N;
		cin>>N;
		if(N==1)	// 1 是每个 K≥3 的第 1 个多边形数
		{
			cout<<"34\n";
			continue;
		}
		cnt=0;
		for(i64 n=2;n*(n+1)/2<=N;++n)	// P(K,n)≥T_n ⇒ n 受三角形数限制
		{
			i64 num=2*N-2*n,den=n*(n-1);
			if(num%den==0)
			{
				i64 K=2+num/den;
				if(K>=3)
					kk[cnt++]=K;
			}
		}
		if(cnt==0)
			cout<<"Poor"<<N<<'\n';
		else
		{
			sort(kk,kk+cnt);
			cout<<kk[0];
			if(cnt>=2)
				cout<<kk[1];
			cout<<'\n';
		}
	}
	return 0;
}