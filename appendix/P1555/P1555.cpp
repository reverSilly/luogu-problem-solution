//
// 题目：P1555 [USACO11NOV] 尴尬的数字 Awkward Digits B
// https://www.luogu.com.cn/problem/P1555
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

string tob(i64 v)                  // v 的 2 进制标准表示(无前导 0)
{
	if(v==0)
		return "0";
	string s;
	while(v){s+=char('0'+v%2);v/=2;}
	reverse(s.begin(),s.end());
	return s;
}
string tot(i64 v)                  // v 的 3 进制标准表示(无前导 0)
{
	if(v==0)
		return "0";
	string s;
	while(v){s+=char('0'+v%3);v/=3;}
	reverse(s.begin(),s.end());
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string b,t;
	cin>>b>>t;
	int Lb=b.size(),Lt=t.size();
	i64 ans=0;
	for(int i=0;i<Lb;++i)          // 枚举"哪一位二进制写错了", 共 <=30 种
	{
		string nb=b;
		nb[i]=(nb[i]=='0'?'1':'0');
		i64 v=0;
		for(char c:nb)
			v=v*2+(c-'0');
		string sb=tob(v),st=tot(v);
		if((int)sb.size()==Lb&&(int)st.size()==Lt)   // 位数不变(Bessie 不会增删位)
		{
			int d1=0,d2=0;
			for(int j=0;j<Lb;++j)
				d1+=(sb[j]!=b[j]);
			for(int j=0;j<Lt;++j)
				d2+=(st[j]!=t[j]);
			if(d1==1&&d2==1)       // 两串各恰好错一位
				ans=v;
		}
	}
	cout<<ans<<'\n';
	return 0;
}
