// 题目：P2816 宋荣子搭积木
// 一列从顶层号 d 的积木需 x>=d。下界：x<=v 的积木只能占每列最上 v+1 层。
// 升序后 C=max ceil((i+1)/(a[i]+1))，轮转发牌构造恰好达到下界 ⇒ 这就是答案。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};

int n,a[mxn],ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	sort(a,a+n);
	for(int i=0;i<n;++i)
		ans=max(ans,(i+a[i]+1)/(a[i]+1));	// ceil((i+1)/(a[i]+1)) 的整数写法
	cout<<ans<<'\n';
	return 0;
}
