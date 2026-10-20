//
// P1586 暴力基准：四重循环枚举所有 0<=a<=b<=c<=d 且 a^2+b^2+c^2+d^2<=N 的有序四元组
// (前导 0 表示不足 4 个部件),直接按定义计数,仅用于小规模数据对拍。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{32773};

int cnt[mxn];               // cnt[s]=和为 s 的方案数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int t;
	cin>>t;
	int qs[105],N=0;
	for(int i=0;i<t;++i)
	{
		cin>>qs[i];
		N=max(N,qs[i]);     // 只枚举到查询最大值,一次过算出所有 <=N 的答案
	}
	for(int d=1;d*d<=N;++d)
		for(int c=0;c<=d&&c*c+d*d<=N;++c)
			for(int b=0;b<=c&&b*b+c*c+d*d<=N;++b)
				for(int a=0;a<=b&&a*a+b*b+c*c+d*d<=N;++a)
					++cnt[a*a+b*b+c*c+d*d];
	for(int i=0;i<t;++i)
		cout<<cnt[qs[i]]<<'\n';
	return 0;
}
