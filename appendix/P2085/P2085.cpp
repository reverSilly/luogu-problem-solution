// 题目：P2085 最小函数值
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n,m;
i64 A[mxn],B[mxn],C[mxn];
int xs[mxn];

// 每个函数的值随 x 严格递增，用小根堆做 n 路归并
priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<pair<i64,int>>> Q;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		cin>>A[i]>>B[i]>>C[i];
		xs[i]=1;
		Q.push({A[i]+B[i]+C[i],i});
	}
	for(int i=0;i<m;++i)
	{
		auto[val,id]=Q.top();
		Q.pop();
		cout<<val<<(i==m-1?'\n':' ');
		++xs[id];
		Q.push({A[id]*xs[id]*xs[id]+B[id]*xs[id]+C[id],id});
	}
	return 0;
}
