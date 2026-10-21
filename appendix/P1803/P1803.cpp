//
// 题目：P1803 凌乱的yyy / 线段覆盖
// https://www.luogu.com.cn/problem/P1803
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1000005};

struct seg
{
	int l,r;
};

int n;
seg a[mxn];

bool byend(seg x,seg y)
{
	return x.r<y.r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].l>>a[i].r;
	sort(a,a+n,byend);
	int cur{-1};
	int ans{0};
	for(int i=0;i<n;++i)
	{
		if(a[i].l>=cur)
		{
			++ans;
			cur=a[i].r;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
