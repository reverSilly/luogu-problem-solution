#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int inf{1145141919};

int n,m,k;
//a[i]：第 i 个教徒的宗教
int a[mxn];
//cnt[]：当前区间内各宗教出现次数
int cnt[mxn];
//dp[i]：前 i 人分组后危险值总和的最小值
int dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	for(int i=1;i<=n;++i)
		cin>>a[i];

	//第一问：贪心，每组尽量长（不同宗教数不超过 k）
	int grp=0,typ=0;
	for(int i=1;i<=n;++i)
	{
		if(cnt[a[i]]==0)
		{
			if(typ==k)
			{
				//当前组已满，另开一组
				++grp;
				for(int j=1;j<=m;++j)
					cnt[j]=0;
				typ=0;
			}
			++typ;
		}
		++cnt[a[i]];
	}
	++grp;

	//第二问：dp[i] = min(dp[j-1] + 区间 [j,i] 的不同宗教数)
	fill_n(dp,mxn,inf);
	dp[0]=0;
	for(int i=1;i<=n;++i)
	{
		for(int j=1;j<=m;++j)
			cnt[j]=0;
		int typ=0;
		for(int j=i;j>=1;--j)
		{
			if(cnt[a[j]]==0)
				++typ;
			++cnt[a[j]];
			if(typ>k)
				break;
			dp[i]=min(dp[i],dp[j-1]+typ);
		}
	}

	cout<<grp<<'\n';
	cout<<dp[n]<<'\n';
	return 0;
}
