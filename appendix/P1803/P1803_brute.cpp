//
// 题目：P1803 凌乱的yyy / 线段覆盖
// https://www.luogu.com.cn/problem/P1803
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int n;
int L[mxn],R[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>L[i]>>R[i];
	int ans{0};
	int lim{1<<n};
	for(int mask=0;mask<lim;++mask)
	{
		int id[25];
		int cnt{0};
		for(int i=0;i<n;++i)
			if(mask>>i&1)
				id[cnt++]=i;
		for(int i=0;i<cnt;++i)
			for(int j=i+1;j<cnt;++j)
				if(R[id[j]]<R[id[i]])
					swap(id[i],id[j]);
		bool ok{true};
		int cur{-1};
		for(int i=0;i<cnt;++i)
		{
			if(L[id[i]]<cur)
			{
				ok=false;
				break;
			}
			cur=R[id[i]];
		}
		if(ok)
			ans=max(ans,cnt);
	}
	cout<<ans<<'\n';
	return 0;
}
