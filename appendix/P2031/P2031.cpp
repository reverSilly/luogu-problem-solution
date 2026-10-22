// 题目：P2031 脑力达人之分割字串
// 思路：先标出字典词在主串中的所有出现位置，再做 DP
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305};

string s,wd;
int n,N;
// minEnd[l]: 使 s[l..r] 含有某个字典词的最小 r（1-based，闭区间），无解为 inf
int minEnd[mxn];
// f[i]: s[1..i] 最多分成几段，-1 表示无法分割
int f[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	cin>>N;
	n=s.size();
	const int inf=1145141919;
	for(int l=1;l<=n+1;++l) minEnd[l]=inf;
	for(int k=0;k<N;++k)
	{
		cin>>wd;
		int m=wd.size();
		for(int i=1;i+m-1<=n;++i)
		{
			bool same=true;
			for(int j=0;j<m;++j)
				if(s[i-1+j]!=wd[j]){same=false;break;}
			if(same) minEnd[i]=min(minEnd[i],i+m-1);
		}
	}
	// minEnd[l] = 所有起点 >= l 的词出现位置中，最小的结束位置
	for(int l=n-1;l>=1;--l) minEnd[l]=min(minEnd[l],minEnd[l+1]);

	f[0]=0;
	for(int i=1;i<=n;++i)
	{
		f[i]=-1;
		for(int j=1;j<=i;++j)
			if(f[j-1]>=0 && minEnd[j]<=i)
				f[i]=max(f[i],f[j-1]+1);
	}
	if(f[n]<0) cout<<0<<'\n';
	else cout<<f[n]<<'\n';
	return 0;
}
