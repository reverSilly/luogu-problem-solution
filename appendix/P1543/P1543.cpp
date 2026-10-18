//
// 题目：P1543 [POI 2004] SZP
// https://www.luogu.com.cn/problem/P1543
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};       // n<=10^6 (+5 松弛), 严格控制内存 <= 13MB

int n;
int a[mxn],deg[mxn],q[mxn];
bitset<mxn> vis;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		--a[i];
		++deg[a[i]];
	}

	int ql=0,qr=0;
	for(int i=0;i<n;++i)
		if(deg[i]==0)
			q[qr++]=i;

	int ans=0;
	while(ql<qr)
	{
		int v=q[ql++];
		if(vis[v])
			continue;
		vis[v]=1;
		int u=a[v];
		if(!vis[u])
		{
			vis[u]=1;
			++ans;
			int p=a[u];
			if(--deg[p]==0&&!vis[p])
				q[qr++]=p;
		}
	}

	// 剩余未访问节点必构成若干互不相交的纯环
	for(int i=0;i<n;++i)
		if(!vis[i])
		{
			int len=0,cur=i;
			while(!vis[cur])
			{
				vis[cur]=1;
				cur=a[cur];
				++len;
			}
			ans+=len/2;
		}

	cout<<ans<<'\n';
	return 0;
}
