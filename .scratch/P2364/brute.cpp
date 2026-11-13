// 枚举 a 的全部子序列, 检查是否为 b,c 的子序列, 取最长
#include<bits/stdc++.h>
using namespace std;
string a,b,c;
int best=0;string bs;
bool issub(const string&s,const string&t)
{
	int p=0;
	for(char ch:t)
		if(p<(int)s.size()&&s[p]==ch)++p;
	return p==(int)s.size();
}
int main()
{
	cin>>a>>b>>c;
	int n=a.size();
	for(int S=0;S<(1<<n);++S)
	{
		string s;
		for(int i=0;i<n;++i)if(S>>i&1)s+=a[i];
		if((int)s.size()<best)continue;
		if(issub(s,b)&&issub(s,c))
		{
			if((int)s.size()>best){best=s.size();bs=s;}
		}
	}
	cout<<bs<<'\n';
	return 0;
}
