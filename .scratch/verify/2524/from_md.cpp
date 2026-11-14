// 题目：P2524 Uim的情人节礼物·其之弐
// 思路：康托展开：比 a 小的排列按"第一个不同位 i"分类，
//       第 i 类有 c_i·(n-1-i)! 个（c_i=后缀中比 a[i] 小的个数）
//       排名 = 1 + Σ c_i·(n-1-i)!
//       输入排列无空格 ⇒ 按字符串读入逐位取数字
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{15};

int n,a[mxn],fac[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	cin>>n>>s;
	for(int i=0;i<n;++i)
		a[i]=s[i]-'0';
	fac[0]=1;
	for(int i=1;i<=n;++i)
		fac[i]=fac[i-1]*i;
	int ans=1;	// 康托展开数的是"严格更小"，排名要 +1
	for(int i=0;i<n;++i)
	{
		int c=0;
		for(int j=i+1;j<n;++j)
			if(a[j]<a[i])
				++c;
		ans+=c*fac[n-1-i];
	}
	cout<<ans<<'\n';
	return 0;
}
