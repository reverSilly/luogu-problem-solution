// 题目：P2788 数学1（math1）- 加减算式
// 一遍扫描：数字用 Horner 攒进 num，遇到 +/- 就把 sgn*num 结进 ans 并翻转符号。
// 开头的符号作用在 num=0 上，天然正确；空白字符直接跳过。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 ans,num;
int sgn=1;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(char c;cin.get(c);)
	{
		if(c>='0'&&c<='9')
			num=num*10+(c-'0');
		else if(c=='+'||c=='-')
		{
			ans+=sgn*num;	// 结算上一项
			num=0;
			sgn=c=='+'?1:-1;
		}
	}
	ans+=sgn*num;	// 最后一项没有后继运算符，单独结算
	cout<<ans<<'\n';
	return 0;
}