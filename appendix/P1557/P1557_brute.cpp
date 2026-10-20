#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 小数据朴素基准:同样的语法(净符号数*数值),但用 long long 直接算,不用高精度。
// 仅用于小规模对拍,数值位数很少时不会溢出。
int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	cin>>s;
	int n=(int)s.size(),i=0;
	i64 ans=0;
	while(i<n)
	{
		i64 coeff=1,sg=1;
		if(s[i]=='+'||s[i]=='-')
		{
			int j=i,plus=0,minus=0;
			while(j<n&&(s[j]=='+'||s[j]=='-'))
			{
				if(s[j]=='+')
					++plus;
				else
					++minus;
				++j;
			}
			if(j==i+1&&j<n&&s[j]=='(')
			{
				sg=(s[i]=='-')?-1:1;
				int k=j+1;i64 c=0;
				while(k<n&&isdigit((unsigned char)s[k]))
				{
					c=c*10+(s[k]-'0');
					++k;
				}
				if(k<n&&s[k]==')')
					++k;
				coeff=c;
				i=k;
			}
			else
			{
				int net=plus-minus;
				if(net==0)
					coeff=0;
				else
				{
					sg=(net<0)?-1:1;
					coeff=abs(net);
				}
				i=j;
			}
		}
		i64 v=0;
		while(i<n&&isdigit((unsigned char)s[i]))
		{
			v=v*10+(s[i]-'0');
			++i;
		}
		ans+=sg*coeff*v;
	}
	cout<<ans<<'\n';
	return 0;
}
