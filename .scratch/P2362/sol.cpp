// 题目：P2362 围栏木桩
// 最长不下降子序列长度 + 方案数, 按「以 i 结尾」分类计数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int m,n,h[mxn],f[mxn],cnt[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m;
	while(m--)
	{
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>h[i];
		for(int i=0;i<n;++i)
		{
			f[i]=1;cnt[i]=1;   // 只取自己
			for(int j=0;j<i;++j)
				if(h[j]<=h[i])
				{
					if(f[j]+1>f[i])
					{
						f[i]=f[j]+1;
						cnt[i]=cnt[j];
					}
					else if(f[j]+1==f[i])
						cnt[i]+=cnt[j];
				}
		}
		int t=0,c=0;
		for(int i=0;i<n;++i)
			if(f[i]>t)
			{
				t=f[i];
				c=cnt[i];
			}
			else if(f[i]==t)
				c+=cnt[i];
		cout<<t<<' '<<c<<'\n';
	}
	return 0;
}
