#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n,m;
i64 A[mxn],B[mxn],C[mxn];
vector<i64> vals;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>A[i]>>B[i]>>C[i];
	// 暴力：每个函数取 x=1..m（m 个最小值必然来自 x<=m），全部塞进数组排序
	for(int i=0;i<n;++i)
		for(int x=1;x<=m;++x)
			vals.push_back(A[i]*x*x+B[i]*x+C[i]);
	sort(vals.begin(),vals.end());
	for(int i=0;i<m;++i)
		cout<<vals[i]<<(i==m-1?'\n':' ');
	return 0;
}
