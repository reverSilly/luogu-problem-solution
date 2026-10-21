//
// 题目：P1881 [USACO12FEB] 绳子对折 B
// https://www.luogu.com.cn/problem/P1881
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};

bool kn[mxn];
int xs[105],n,L;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>L;
	L*=2;
	for(int i=0;i<n;++i)
	{
		int x;
		cin>>x;
		x*=2;
		xs[i]=x;
		kn[x]=true;
	}
	int ans=0;
	for(int p=1;p<L;++p)
	{
		bool ok=true;
		for(int i=0;i<n;++i)
		{
			int y=p*2-xs[i];
			if(y<0||y>L)
				continue;
			if(!kn[y])
			{
				ok=false;
				break;
			}
		}
		ans+=ok;
	}
	cout<<ans<<'\n';
	return 0;
}
