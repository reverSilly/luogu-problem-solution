//
// 题目：P1801 黑匣子
// https://www.luogu.com.cn/problem/P1801
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int m,n;
int a[mxn],u[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<m;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		cin>>u[i];

	priority_queue<int> lo;
	priority_queue<int,vector<int>,greater<int>> hi;
	int ptr{0};
	for(int i=1;i<=n;++i)
	{
		while(ptr<u[i-1])
		{
			int x{a[ptr++]};
			if(lo.empty()||x<=lo.top())
				lo.push(x);
			else
				hi.push(x);
		}
		while((int)lo.size()>i)
		{
			hi.push(lo.top());
			lo.pop();
		}
		while((int)lo.size()<i)
		{
			lo.push(hi.top());
			hi.pop();
		}
		cout<<lo.top()<<'\n';
	}
	return 0;
}
