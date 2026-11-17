// 题目：P3015 [USACO11FEB] Best Parenthesis S
// 得分 = 每个空括号对 () 贡献 2^(深度-1) 之和；一遍扫描深度 + 预处理 2 的幂
// 模数 12345678910 超 int，全程 i64；不用栈，天然避开"取模后得 0 与空串标记混淆"的坑
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr i64 p{12345678910};

int n,a[mxn],dep;
i64 pw[mxn],ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	pw[0]=1;
	for(int i=1;i<=n;++i)
		pw[i]=pw[i-1]*2%p;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
	{
		if(a[i]==0)
		{
			++dep;
			if(i+1<n&&a[i+1]==1)	// 紧邻的 () 是叶子，贡献 2^(dep-1)
				ans=(ans+pw[dep-1])%p;
		}
		else
			--dep;
	}
	cout<<ans<<'\n';
	return 0;
}
