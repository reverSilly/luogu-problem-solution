#include<bits/stdc++.h>
using namespace std;
// 开发期校验：翻前缀是对合，图无向，从全1做一次BFS即得每个串的最少步数
int greedy(const string&s)
{
	int n=s.size(),f=0;
	for(int k=n-1;k>=0;--k)
		if(((s[k]-'0')^(f&1))==0)++f;
	return f;
}
int main()
{
	for(int n=1;n<=12;++n)
	{
		int N=1<<n;
		vector<int>dist(N,-1);
		string all(n,'1');
		int start=0; // 全1对应二进制全1
		start=N-1;
		queue<int>Q;Q.push(start);dist[start]=0;
		while(!Q.empty())
		{
			int u=Q.front();Q.pop();
			for(int k=1;k<=n;++k)
			{
				int v=u^((1<<k)-1); // 翻转低k位(前k个硬币)
				if(dist[v]<0){dist[v]=dist[u]+1;Q.push(v);}
			}
		}
		for(int m=0;m<N;++m)
		{
			string s(n,'0');
			for(int i=0;i<n;++i)s[i]=((m>>i)&1)?'1':'0'; // s[i]=bit i，前k个=低k位
			int g=greedy(s);
			if(g!=dist[m]){cout<<"MISMATCH n="<<n<<" s="<<s<<" greedy="<<g<<" bfs="<<dist[m]<<"\n";return 1;}
		}
		cout<<"n="<<n<<" all "<<N<<" strings OK\n";
	}
	cout<<"ALL OK\n";
	return 0;
}