#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20};

int n,ans;
int board[mxn][mxn];              // 1 表示该格可放
int col[mxn],d1[2*mxn],d2[2*mxn];

void dfs(int r)
{
	if(r==n)
	{
		++ans;
		return;
	}
	for(int c=0;c<n;++c)
	{
		if(!board[r][c])
			continue;
		if(col[c]||d1[r+c]||d2[r-c+n])
			continue;
		col[c]=d1[r+c]=d2[r-c+n]=1;
		dfs(r+1);
		col[c]=d1[r+c]=d2[r-c+n]=0;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		for(int j=0;j<n;++j)
			board[i][j]=(s[j]=='*');
	}
	dfs(0);
	cout<<ans<<'\n';
	return 0;
}
