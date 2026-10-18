//
// 题目：P1537 [MCERC 1999] 弹珠
// https://www.luogu.com.cn/problem/P1537
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxs{60005};         // 最大半和 6*20000/2 = 60000 (+5 松弛)

int n[7];
bool dp[mxs];
int used[mxs];                    // 当前面值已使用的弹珠个数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int tc=0;
	while(cin>>n[1]>>n[2]>>n[3]>>n[4]>>n[5]>>n[6])
	{
		int sum=0;
		for(int i=1;i<=6;++i)
			sum+=i*n[i];
		if(sum==0)
			break;
		++tc;
		cout<<"Collection #"<<tc<<":\n";
		if(sum&1)
		{
			cout<<"Can't be divided.\n\n";
			continue;
		}
		int target=sum>>1;
		memset(dp,0,sizeof(bool)*(target+1));
		dp[0]=true;
		for(int i=1;i<=6;++i)
		{
			if(n[i]==0)
				continue;
			memset(used,0,sizeof(int)*(target+1));
			for(int w=i;w<=target;++w)
				if(!dp[w]&&dp[w-i]&&used[w-i]<n[i])
				{
					dp[w]=true;
					used[w]=used[w-i]+1;
				}
		}
		if(dp[target])
			cout<<"Can be divided.\n\n";
		else
			cout<<"Can't be divided.\n\n";
	}
	return 0;
}
