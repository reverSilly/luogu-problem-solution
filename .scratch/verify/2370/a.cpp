// 题目：P2370 yyy2015c01 的 U 盘
// 把文件按大小升序依次塞进 0/1 背包：塞完第 i 个后 dp[S] 就是「接口大小 = a[i].w」时的最大价值
// 第一个满足 dp[S]>=p 的文件大小就是答案，全程不满足就是 No Solution!
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,p,S;
struct{int w,v;}a[mxn];
int dp[mxn];   //dp[j]=容量 j 以内能取到的最大价值

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>p>>S;
	for(int i=0;i<n;++i)
		cin>>a[i].w>>a[i].v;
	sort(a,a+n,[](auto x,auto y){return x.w<y.w;});
	int ans{1145141919};       //114514 家族哨兵：表示「没找到」
	for(int i=0;i<n;++i)
	{
		for(int j=S;j>=a[i].w;--j)
			dp[j]=max(dp[j],dp[j-a[i].w]+a[i].v);
		//此时 dp 恰好是「接口大小 = a[i].w」时的 0/1 背包
		if(dp[S]>=p)
		{
			ans=a[i].w;
			break;
		}
	}
	if(ans==1145141919)
		cout<<"No Solution!\n";
	else
		cout<<ans<<'\n';
	return 0;
}
