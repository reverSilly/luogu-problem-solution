//
// 题目：P1631 序列合并
// https://www.luogu.com.cn/problem/P1631
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n,a[mxn],b[mxn],idx[mxn];

struct node
{
	int s,i;
	friend bool operator>(node x,node y){return x.s>y.s;}
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		cin>>b[i];

	priority_queue<node,vector<node>,greater<>>Q;
	for(int i=0;i<n;++i)
	{
		idx[i]=0;
		Q.push({a[i]+b[0],i});
	}

	for(int k=0;k<n;++k)
	{
		auto[s,i]=Q.top();Q.pop();
		cout<<s<<(k+1==n?'\n':' ');
		if(++idx[i]<n)
			Q.push({a[i]+b[idx[i]],i});
	}
	return 0;
}
