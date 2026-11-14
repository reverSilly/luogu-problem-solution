#include<bits/stdc++.h>
using namespace std;

constexpr int mxnode{500005};

int n,m,tot;
int ch[mxnode][26];	// 0 表示空儿子
bool ed[mxnode],vis[mxnode];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		string s;cin>>s;
		int u=0;
		for(char c:s)
		{
			int x=c-'a';
			if(!ch[u][x])ch[u][x]=++tot;
			u=ch[u][x];
		}
		ed[u]=true;
	}
	cin>>m;
	for(int i=0;i<m;++i)
	{
		string s;cin>>s;
		int u=0;
		bool ok=true;
		for(char c:s)
		{
			int x=c-'a';
			if(!ch[u][x]){ok=false;break;}
			u=ch[u][x];
		}
		if(!ok||!ed[u])cout<<"WRONG\n";
		else if(!vis[u]){vis[u]=true;cout<<"OK\n";}
		else cout<<"REPEAT\n";
	}
	return 0;
}