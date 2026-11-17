// 题目：P2947 [USACO09MAR] Look Up S
// 单调栈：栈内身高自底向顶非严格递减；新牛严格高于栈顶时，栈顶就找到了最近仰望对象
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,h[mxn],ans[mxn],stk[mxn],top;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	fill_n(ans,n,-1);	// -1=无仰望对象；输出时 +1 恰好变 0
	for(int i=0;i<n;++i)
	{
		cin>>h[i];
		while(top&&h[stk[top-1]]<h[i])	// 严格小于才弹：等高不算仰望，不能弹
			ans[stk[--top]]=i;
		stk[top++]=i;
	}
	for(int i=0;i<n;++i)
		cout<<ans[i]+1<<'\n';
	return 0;
}
