// 题目：P2669 [NOIP 2015 普及组] 金币
// 思路：第 n 组连续 n 天、每天 n 枚 ⇒ 按组求和，第 n 组贡献 n*min(n,k-day)
//       组数约 sqrt(2k)，O(sqrt k)；ans<=1.41e6，int 足够
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int k,ans=0;
	cin>>k;
	for(int n=1,day=0;day<k;++n)
	{
		int t=min(n,k-day);	// 第 n 组实际发放 t 天（末组可能被截断）
		ans+=n*t;
		day+=t;
	}
	cout<<ans<<'\n';
	return 0;
}
