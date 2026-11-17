// 题目：P3009 [USACO11JAN] Profits S
// 最大非空连续子段和：Kadane，cur=max(x,cur+x)，ans 取全程最大
// 全负数据答案为最大单元素（负数），ans 哨兵必须显式设负无穷
#include<bits/stdc++.h>
using namespace std;

int n,ans{-1145141919},cur;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,x;i<n;++i)
	{
		cin>>x;
		cur=max(x,cur+x);	// cur：以第 i 天结尾的最大非空子段和
		ans=max(ans,cur);
	}
	cout<<ans<<'\n';
	return 0;
}
