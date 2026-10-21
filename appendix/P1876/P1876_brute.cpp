//
// 题目：P1876 开灯
// https://www.luogu.com.cn/problem/P1876
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2000005};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	cin>>n;
	int N=(int)n;
	static int cnt[mxn];
	for(int p=1;p<=N;++p)
		for(int i=p;i<=N;i+=p)
			++cnt[i];
	bool fst=true;
	for(int i=1;i<=N;++i)
	{
		if(cnt[i]&1)
		{
			if(!fst)
				cout<<' ';
			fst=false;
			cout<<i;
		}
	}
	cout<<'\n';
	return 0;
}
