// 题目：P2959 [USACO09OCT] The Leisurely Stroll G
// 满二叉树上求根到叶的最长路径（边数）：f(u)=1+max(f(d1),f(d2))，叶子 f=0
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int p,ch[mxn][2];

int dfs(int u)	// u=-1 表示草地；返回 u 到下方草地的最多小径数
{
	if(u==-1)
		return 0;
	return 1+max(dfs(ch[u][0]),dfs(ch[u][1]));
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	fill_n(&ch[0][0],mxn*2,-1);	// 全局默认 0 是合法结点编号，必须显式置 -1
	cin>>p;
	for(int i=1,c,d1,d2;i<p;++i)
	{
		cin>>c>>d1>>d2;--c;
		ch[c][0]=d1?d1-1:-1;
		ch[c][1]=d2?d2-1:-1;
	}
	cout<<dfs(0)<<'\n';
	return 0;
}
