#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};	// 2e9 最多 10 位，+5 松弛

int len,dig[mxn];
i64 f[mxn][11][2];	// f[pos][pre][lead]，pre=10 表示仍在前导零

i64 dfs(int pos,int pre,bool lead,bool lim)
{
	if(pos==len)
		return lead?0:1;	// 全前导零代表 0，不是 windy 数
	if(!lim&&f[pos][pre][lead]!=-1)
		return f[pos][pre][lead];
	int up=lim?dig[pos]:9;
	i64 res=0;
	for(int d=0;d<=up;++d)
	{
		bool nlim=lim&&d==dig[pos];
		if(lead&&d==0)
			res+=dfs(pos+1,10,true,nlim);
		else
		{
			if(!lead&&abs(d-pre)<2)
				continue;
			res+=dfs(pos+1,d,false,nlim);
		}
	}
	if(!lim)
		f[pos][pre][lead]=res;
	return res;
}

i64 count(i64 x)
{
	if(x<=0)
		return 0;
	len=0;
	for(i64 t=x;t;t/=10)
		dig[len++]=t%10;
	for(int i=0;i<len/2;++i)
		swap(dig[i],dig[len-1-i]);	// 翻成高位在前
	memset(f,-1,sizeof f);
	return dfs(0,10,true,true);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 a,b;
	cin>>a>>b;
	cout<<count(b)-count(a-1)<<'\n';
	return 0;
}