//
// 题目：P1527 [国家集训队] 矩阵乘法
// 暴力基准: 每次询问直接把子矩形元素收集进数组 nth_element 求第 k 小
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};

int n,q;
int a[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>q))
		return 0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];

	vector<int>buf;
	for(int qi=0;qi<q;++qi)
	{
		int x1,y1,x2,y2,k;
		cin>>x1>>y1>>x2>>y2>>k;
		--x1;--y1;--x2;--y2;
		buf.clear();
		for(int i=x1;i<=x2;++i)
			for(int j=y1;j<=y2;++j)
				buf.push_back(a[i][j]);
		nth_element(buf.begin(),buf.begin()+k-1,buf.end());
		cout<<buf[k-1]<<'\n';
	}
	return 0;
}
