#include<bits/stdc++.h>
using namespace std;

// 位掩码 DP（完全搜索，用来验证 m=20 规模下 DFS 剪枝的正确性）
// ok[mask]：mask 中的木棒能排列成「若干个完整的 side 组 + 一个前缀组」
constexpr int mxm{1<<20};

int m,a[20],side;
bool ok[mxm];
int sm[mxm];

int main()
{
	int n;
	cin>>n;
	while(n--)
	{
		cin>>m;
		int tot=0,mx=0;
		for(int i=0;i<m;++i)
		{
			cin>>a[i];
			tot+=a[i];
			mx=max(mx,a[i]);
		}
		if(tot%4!=0||mx>tot/4)
		{
			cout<<"no\n";
			continue;
		}
		side=tot/4;
		int full=(1<<m)-1;
		fill_n(ok,full+1,false);
		sm[0]=0;
		ok[0]=true;
		for(int mask=0;mask<=full;++mask)
		{
			if(mask)
				sm[mask]=sm[mask&(mask-1)]+a[__builtin_ctz(mask)];
			if(!ok[mask])
				continue;
			int cur=sm[mask]%side;
			for(int i=0;i<m;++i)
				if(!(mask>>i&1)&&cur+a[i]<=side)
					ok[mask|1<<i]=true;
		}
		cout<<(ok[full]?"yes\n":"no\n");
	}
	return 0;
}
