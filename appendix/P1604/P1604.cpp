//
// 题目：P1604 B进制星球
// https://www.luogu.com.cn/problem/P1604
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005};          // n,m<=2000 (+5 松弛)

int B;
string a,b,sum_;

int val(char c)
{
	if(c>='0'&&c<='9')
		return c-'0';
	return c-'A'+10;
}

char dig(int v)
{
	if(v<10)
		return char('0'+v);
	return char('A'+v-10);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>B>>a>>b;

	int i=a.size()-1,j=b.size()-1,carry=0;
	while(i>=0||j>=0||carry)         // 低位在前逐位相加
	{
		int x=i>=0?val(a[i--]):0;
		int y=j>=0?val(b[j--]):0;
		int t=x+y+carry;
		sum_+=dig(t%B);
		carry=t/B;
	}
	reverse(sum_.begin(),sum_.end());

	size_t p=0;                      // 去掉前导零, 至少保留一位
	while(p+1<sum_.size()&&sum_[p]=='0')
		++p;
	cout<<sum_.substr(p)<<'\n';
	return 0;
}
