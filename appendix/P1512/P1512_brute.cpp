//
// 题目：P1512 [ICPC 2001 Taejon R] 伊甸园日历游戏
// 对照基准: 奇偶不变量 + 9.30/11.30 反常点闭式判定 (与 DAG 逆推全量互验)
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int t;
	if(!(cin>>t))
		return 0;
	for(int i=0,y,m,d;i<t;++i)
	{
		cin>>y>>m>>d;
		bool ok=((m+d)%2==0)||(m==9&&d==30)||(m==11&&d==30);
		cout<<(ok?"YES\n":"NO\n");
	}
	return 0;
}
