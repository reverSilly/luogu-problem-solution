// 题目：P2021 faebdc玩扑克
// 暴力：用数组直接模拟「把队首移到队尾，再弹出队首」，记录被弹出位置的次序
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005};

int n,len;
int q[mxn];
int ans[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	len=n;
	for(int i=0;i<n;++i)
		q[i]=i;
	for(int v=1;v<=n;++v)
	{
		//把队首移到队尾
		int f=q[0];
		for(int i=0;i<len-1;++i)
			q[i]=q[i+1];
		q[len-1]=f;
		//弹出队首
		int p=q[0];
		for(int i=0;i<len-1;++i)
			q[i]=q[i+1];
		--len;
		ans[p]=v;
	}
	for(int i=0;i<n;++i)
	{
		if(i)
			cout<<' ';
		cout<<ans[i];
	}
	cout<<'\n';
	return 0;
}
