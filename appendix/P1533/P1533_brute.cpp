//
// 题目：P1533 可怜的狗狗
// 暴力基准: 每次询问直接拷贝子区间 [l, r] 并用 nth_element 求第 k 小
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{300005};

int n,m,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		cin>>a[i];
	vector<int>buf;
	for(int qi=0;qi<m;++qi)
	{
		int l,r,k;
		cin>>l>>r>>k;
		--l;--r;
		buf.assign(a+l,a+r+1);
		nth_element(buf.begin(),buf.begin()+k-1,buf.end());
		cout<<buf[k-1]<<'\n';
	}
	return 0;
}
