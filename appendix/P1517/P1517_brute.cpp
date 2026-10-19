// P1517 暴力基准：用字符串大数逐次乘以 P
#include<bits/stdc++.h>
using namespace std;

string mul(string s,int m)            // 十进制字符串 *= 小整数
{
	int carry=0;
	for(int i=s.size()-1;i>=0;--i)
	{
		int cur=(s[i]-'0')*m+carry;
		s[i]=char('0'+cur%10);
		carry=cur/10;
	}
	while(carry)
	{
		s.insert(s.begin(),char('0'+carry%10));
		carry/=10;
	}
	return s;
}

string fmt(string s,int e)            // 按规则插入小数点并去零
{
	if(e==0)
		return s;
	if((int)s.size()<=e)
		s=string(e-s.size(),'0')+s;
	int pos=s.size()-e;
	string ip=s.substr(0,pos),fp=s.substr(pos);
	while(!fp.empty()&&fp.back()=='0')
		fp.pop_back();
	int i=0;
	while(i<(int)ip.size()&&ip[i]=='0')
		++i;
	ip=ip.substr(i);
	if(ip.empty())
		return fp.empty()?"0":"."+fp;
	if(fp.empty())
		return ip;
	return ip+"."+fp;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	int n;
	while(cin>>s>>n)
	{
		int P=0,k=0;
		bool dot=false;
		for(char c:s)
		{
			if(c=='.')
				dot=true;
			else if(c>='0'&&c<='9')
			{
				P=P*10+(c-'0');
				if(dot)
					++k;
			}
		}
		string num="1";
		for(int i=0;i<n;++i)
			num=mul(num,P);
		cout<<fmt(num,k*n)<<'\n';
	}
	return 0;
}
