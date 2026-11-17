// 题目：P2911 [USACO08OCT] Bovine Bones G
// 三个骰子面数 <=20,20,40，组合总数 <=16000，直接枚举计数。
// 和的范围 3..S1+S2+S3；升序扫描 + 严格大于 ⇒ 平局取最小和。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxs{85};	// 20+20+40+5

int s1,s2,s3,cnt[mxs],ans,mx;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s1>>s2>>s3;
	for(int a=1;a<=s1;++a)
		for(int b=1;b<=s2;++b)
			for(int c=1;c<=s3;++c)
				++cnt[a+b+c];
	for(int s=3;s<=s1+s2+s3;++s)
		if(cnt[s]>mx)	// 严格 > ：平局保留更小的 s
			mx=cnt[s],ans=s;
	cout<<ans<<'\n';
	return 0;
}
