//
// P1589 朴素解：暴搜。离散化每个单位泥点，每次找最左未覆盖点,
// 枚举木板左端点 b in [p-L+1,p], 递归取最小块数
#include<bits/stdc++.h>
using namespace std;

constexpr int mx{40};

int n,L;
bool mud[mx],cov[mx];
int best;

void dfs(int used)
{
	int p=-1;
	for(int i=0;i<mx;++i)
		if(mud[i]&&!cov[i])
		{
			p=i;
			break;
		}
	if(p==-1)
	{
		best=min(best,used);
		return;
	}
	if(used+1>=best)
		return;
	for(int b=p;b>=p-L+1&&b>=0;--b)
	{
		bool ch[mx]{};
		for(int i=b;i<b+L&&i<mx;++i)
		{
			ch[i]=cov[i];
			cov[i]=true;
		}
		dfs(used+1);
		for(int i=b;i<b+L&&i<mx;++i)
			cov[i]=ch[i];
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>L;
	for(int i=0,s,e;i<n;++i)
	{
		cin>>s>>e;
		for(int j=s;j<e;++j)
			mud[j]=true;
	}
	best=1e9;
	dfs(0);
	cout<<best<<'\n';
	return 0;
}
