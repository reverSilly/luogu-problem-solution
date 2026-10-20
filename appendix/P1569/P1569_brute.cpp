// 暴力基准:枚举所有"切割位置"子集,逐一检查每组和是否 >=0
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};            // 暴力仅跑 n<=20

int n;
i64 a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];

	int best=-1;
	for(int mask=0;mask<(1<<max(0,n-1));++mask)   // mask 的每一位表示此处是否切一刀
	{
		i64 cur=0;
		bool ok=true;
		int cnt=1;
		for(int i=0;i<n;++i)
		{
			cur+=a[i];
			int cut=(i<n-1)&&((mask>>i)&1);
			if(cut)
			{
				if(cur<0)
					ok=false;
				cur=0;
				++cnt;
			}
		}
		if(cur<0)
			ok=false;
		if(ok)
			best=max(best,cnt);
	}

	if(best<0)
		cout<<"Impossible\n";
	else
		cout<<best<<'\n';
	return 0;
}
