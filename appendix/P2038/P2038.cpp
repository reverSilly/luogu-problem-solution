// 题目：P2038 [NOIP 2014 提高组] 无线网络发射器选址
// 枚举城市内所有 129*129 个路口作为安装点，统计被覆盖的公共场所数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};

int d,n;
int px[mxn],py[mxn];
i64 pk[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>d;
	cin>>n;
	for(int i=0;i<n;++i) cin>>px[i]>>py[i]>>pk[i];

	i64 best=-1;
	int cnt=0;
	for(int x=0;x<=128;++x)
		for(int y=0;y<=128;++y)
		{
			i64 sum=0;
			for(int i=0;i<n;++i)
				if(abs(px[i]-x)<=d && abs(py[i]-y)<=d)
					sum+=pk[i];
			if(sum>best) best=sum,cnt=1;
			else if(sum==best) ++cnt;
		}
	cout<<cnt<<' '<<best<<'\n';
	return 0;
}
