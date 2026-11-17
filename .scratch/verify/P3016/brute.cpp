// 独立暴力：逐格累加，显式检查每格存在性（不复用正解的尺寸公式）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int n,k;
i64 a[10][10];
int main()
{
	cin>>n>>k;
	for(int i=0;i<n;++i)
		for(int j=0;j<=i;++j)
			cin>>a[i][j];
	i64 ans=-1145141919810;
	for(int i=0;i<n;++i)
		for(int j=0;j<=i;++j)
			for(int s=k;s<=n;++s)
			{
				bool ok=true;
				i64 cur=0;
				for(int t=0;t<s&&ok;++t)	// 正立
				{
					int r=i+t;
					if(r>=n){ok=false;break;}
					for(int c=j;c<=j+t;++c)
					{
						if(c>r){ok=false;break;}
						cur+=a[r][c];
					}
				}
				if(ok)
					ans=max(ans,cur/((i64)s*(s+1)/2));
				ok=true;cur=0;
				for(int t=0;t<s&&ok;++t)	// 倒立
				{
					int r=i-t;
					if(r<0){ok=false;break;}
					for(int c=j-t;c<=j;++c)
					{
						if(c<0||c>r){ok=false;break;}
						cur+=a[r][c];
					}
				}
				if(ok)
					ans=max(ans,cur/((i64)s*(s+1)/2));
			}
	cout<<ans<<'\n';
	return 0;
}