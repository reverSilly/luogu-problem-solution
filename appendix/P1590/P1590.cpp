//
// 题目：P1590 失踪的7
// https://www.luogu.com.cn/problem/P1590
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	cin>>t;
	while(t--)
	{
		cin>>s;
		i64 ans=0;              // 九进制读数即排名
		for(char c:s)
		{
			int d=c-'0';
			if(d>7)
				--d;            // 跳过数字 7, 其后的数码整体前移一位
			ans=ans*9+d;
		}
		cout<<ans<<'\n';
	}
	return 0;
}
