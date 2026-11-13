// 题目：P2397 yyy loves Maths VI (mode)
// 摩尔投票：O(1) 空间流式求出现次数超过一半的数，不开数组
#include<bits/stdc++.h>
using namespace std;

int n,cand,cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,x;i<n;++i)
	{
		cin>>x;
		if(!cnt)
			cand=x,cnt=1;
		else if(x==cand)
			++cnt;
		else
			--cnt;
	}
	cout<<cand<<'\n';
	return 0;
}
