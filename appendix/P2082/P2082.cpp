// 题目：P2082 区间覆盖（加强版）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

struct seg
{
	i64 l,r;
};

int n;
seg a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i].l>>a[i].r;
	sort(a,a+n,[](seg x,seg y){
		if(x.l!=y.l)
			return x.l<y.l;
		return x.r<y.r;
	});

	// 整数点口径：合并块 [cl,cr] 贡献 cr-cl+1；坐标与答案都要 long long
	i64 ans=0,cl=a[0].l,cr=a[0].r;
	for(int i=1;i<n;++i)
	{
		if(a[i].l<=cr+1)
		{
			cr=max(cr,a[i].r);
		}
		else
		{
			ans+=cr-cl+1;
			cl=a[i].l;
			cr=a[i].r;
		}
	}
	ans+=cr-cl+1;
	cout<<ans<<'\n';
	return 0;
}
