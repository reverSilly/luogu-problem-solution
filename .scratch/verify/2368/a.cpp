// 题目：P2368 EXCEEDED WARNING B
// 结论：x^2 mod 10^9 = 987654321 在 [0,10^9) 上恰有 8 个解，且全是 9 位数：
//       111111111 119357639 380642361 388888889 611111111 619357639 880642361 888888889
//       于是 n<=8 无解（根都 >=1e8）、n=9 有 8 个、n>=10 每块贡献 8*9 个 => 72*10^(n-10)
//       答案是 n-8 位大数，必须用字符串输出
#include<bits/stdc++.h>
using namespace std;

int n;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	if(n<=8)
		cout<<0<<'\n';                 // 位数太少，凑不出 9 位后缀
	else if(n==9)
		cout<<8<<'\n';                 // 模 1e9 恰有 8 个根，且全都是 9 位数
	else
	{
		string s="72";                 // 8 * 9 * 10^(n-10)
		s.append(n-10,'0');
		cout<<s<<'\n';
	}
	return 0;
}
