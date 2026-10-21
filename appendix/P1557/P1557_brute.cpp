#include<bits/stdc++.h>
using namespace std;

// 使用 __int128 在中小数据下独立验证词法分析与求值逻辑
int main()
{
	string s;
	if(!(cin>>s))
		return 0;
	__int128 total=0;
	int n=s.size(),i=0;
	while(i<n)
	{
		__int128 sgn=1,mult=1;
		if(s[i]=='+'||s[i]=='-')
		{
			char ch=s[i];
			sgn=(ch=='+')?1:-1;
			if(i+1<n&&s[i+1]=='(')
			{
				i+=2;
				mult=0;
				while(i<n&&s[i]!=')')
					mult=mult*10+(s[i++]-'0');
				++i;
			}
			else
			{
				mult=0;
				while(i<n&&s[i]==ch)
				{
					++mult;
					++i;
				}
			}
		}
		__int128 val=0;
		while(i<n&&isdigit(static_cast<unsigned char>(s[i])))
			val=val*10+(s[i++]-'0');
		total+=sgn*mult*val;
	}
	if(total==0)
	{
		cout<<0<<'\n';
		return 0;
	}
	if(total<0)
	{
		cout<<'-';
		total=-total;
	}
	string out;
	while(total>0)
	{
		out.push_back('0'+total%10);
		total/=10;
	}
	reverse(out.begin(),out.end());
	cout<<out<<'\n';
	return 0;
}
