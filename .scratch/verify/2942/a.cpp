// 题目：P2942 [USACO09MAR] Moon Mooing G
// closure of {c} under f1,f2 = 两条单调流的归并（丑数式双指针）
// f_i(x)>=x 且严格递增；候选可能 >=2^63，用 __int128 算，答案本身 <2^63 用 i64 存
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
using i128=__int128;

constexpr int mxn{4000005};
constexpr i128 inf=(i128)1<<100;	// 候选哨兵：远大于 20*2^63

int n,a1,b1,d1,a2,b2,d2;
i64 c,s[mxn];

i128 f1(i64 x){return (i128)a1*x/d1+b1;}
i128 f2(i64 x){return (i128)a2*x/d2+b2;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>c>>n;
	cin>>a1>>b1>>d1>>a2>>b2>>d2;
	s[0]=c;
	int p1=0,p2=0;
	for(int i=0;i<n;++i)
	{
		if(i)
		{
			// 不变量：p1/p2 指向各自流中第一个 >s[i-1] 的候选；越界则暂记 inf
			i128 x=p1<i?f1(s[p1]):inf,y=p2<i?f2(s[p2]):inf;
			s[i]=(i64)min(x,y);	// 题目保证前 n 个时长都 <2^63
		}
		// 推进指针越过所有 <=s[i] 的候选（含 f(x)=x 的不动点与两流撞值）
		while(p1<=i&&f1(s[p1])<=s[i])++p1;
		while(p2<=i&&f2(s[p2])<=s[i])++p2;
	}
	cout<<s[n-1]<<'\n';
	return 0;
}
