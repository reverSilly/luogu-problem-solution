// 题目：P2999 [USACO10NOV] Chocolate Milk S
// A<B ⇒ 编号即拓扑序：正序递推 cs（到达 v 的源数），逆序递推 ct（v 可达的汇数）
// 合法 ⟺ cs[v]==S 且 ct[v]==T 且 v 非源；路径唯一保证求和不重不漏
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,cs[mxn],ct[mxn];
vector<int>graph[mxn],rg[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		rg[v].push_back(u);
	}
	int S=0,T=0;
	for(int i=0;i<n;++i)
		if(rg[i].empty())
			cs[i]=1,++S;
		else
			for(int u:rg[i])
				cs[i]+=cs[u];	// 各前驱的源集合不交（路径唯一）
	for(int i=n-1;i>=0;--i)
		if(graph[i].empty())
			ct[i]=1,++T;
		else
			for(int v:graph[i])
				ct[i]+=ct[v];
	for(int i=0;i<n;++i)
		if(!rg[i].empty()&&cs[i]==S&&ct[i]==T)
			cout<<i+1<<'\n';
	return 0;
}
