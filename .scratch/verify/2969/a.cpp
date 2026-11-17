// 题目：P2969 [USACO09DEC] Music Notes S
// 前缀和严格递增 ⇒ 「s[i]>T」是单调谓词 ⇒ upper_bound 二分
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{50005};

int n,q;
i64 s[mxn];	// s[i]=前 i+1 个音符的总节拍数；音符 i 覆盖 [s[i-1],s[i])，s[-1] 视为 0

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	for(int i=0,b;i<n;++i)
	{
		cin>>b;
		s[i]=b+(i?s[i-1]:0);
	}
	for(int i=0;i<q;++i)
	{
		i64 t;
		cin>>t;
		// 第一个 s[i]>t 的位置就是覆盖时刻 t 的音符（0-based），输出 +1
		cout<<upper_bound(s,s+n,t)-s+1<<'\n';
	}
	return 0;
}
