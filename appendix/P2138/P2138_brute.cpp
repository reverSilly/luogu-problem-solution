#include<bits/stdc++.h>
using namespace std;

//暴力：把字符串看成图上的点，“距离 1”当边，BFS 求最短路
//点集：字母表取 a、b 中出现过的字符，长度不超过 lim 的所有非空串
string a,b;
vector<string>vs;

int lcs(const string&x,const string&y)
{
	static int dp[16][16];
	int la=x.size(),lb=y.size();
	for(int i=0;i<=la;++i)
		dp[i][0]=0;
	for(int j=0;j<=lb;++j)
		dp[0][j]=0;
	for(int i=1;i<=la;++i)
		for(int j=1;j<=lb;++j)
			dp[i][j]=(x[i-1]==y[j-1])?dp[i-1][j-1]+1:max(dp[i-1][j],dp[i][j-1]);
	return dp[la][lb];
}

//“各自删除不超过其自身长度一半的字符后相等”
bool one(const string&x,const string&y)
{
	return 2*lcs(x,y)>=max((int)x.size(),(int)y.size());
}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>a>>b;

	string alpha;
	for(char c:a)
		if(alpha.find(c)==string::npos)alpha+=c;
	for(char c:b)
		if(alpha.find(c)==string::npos)alpha+=c;

	int lim=2*max((int)a.size(),(int)b.size());
	lim=min(lim,6);
	//枚举所有串
	vector<string>cur{""};
	for(int len=1;len<=lim;++len)
	{
		vector<string>nxt;
		for(auto&s:cur)
			for(char c:alpha)
				nxt.push_back(s+c);
		for(auto&s:nxt)
			cur.push_back(s);
	}
	vs=cur;

	unordered_map<string,int>id;
	for(size_t i=0;i<vs.size();++i)
		id[vs[i]]=(int)i;

	int n=vs.size();
	vector<vector<unsigned char>>lcsv(n,vector<unsigned char>(n));
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			lcsv[i][j]=(unsigned char)lcs(vs[i],vs[j]);

	int s=id[a],t=id[b];
	vector<int>dis(n,-1);
	queue<int>Q;
	dis[s]=0;
	Q.push(s);
	while(!Q.empty())
	{
		int u=Q.front();
		Q.pop();
		if(u==t)break;
		for(int v=0;v<n;++v)
			if(dis[v]==-1&&2*lcsv[u][v]>=max((int)vs[u].size(),(int)vs[v].size()))
			{
				dis[v]=dis[u]+1;
				Q.push(v);
			}
	}
	//a==b 时按题面定义答案为 1
	cout<<max(1,dis[t])<<'\n';
	return 0;
}
