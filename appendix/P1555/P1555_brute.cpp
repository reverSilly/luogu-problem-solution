//
// 题目：P1555 [USACO11NOV] 尴尬的数字 Awkward Digits B ——朴素暴力基准
// https://www.luogu.com.cn/problem/P1555
// 结构独立于标程: 直接枚举候选 N 的全部可能值(v < 2^Lb), 逐个比对
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

string tob(i64 v)
{
	if(v==0)
		return "0";
	string s;
	while(v){s+=char('0'+v%2);v/=2;}
	reverse(s.begin(),s.end());
	return s;
}
string tot(i64 v)
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
	for(i64 v=0;v<(1LL<<Lb);++v)   // 只在小规模(Lb<=20)下使用
	{
		string sb=tob(v),st=tot(v);
		if((int)sb.size()!=Lb||(int)st.size()!=Lt)
			continue;
		int d1=0,d2=0;
		for(int j=0;j<Lb;++j)
			d1+=(sb[j]!=b[j]);
		for(int j=0;j<Lt;++j)
			d2+=(st[j]!=t[j]);
		if(d1==1&&d2==1)
			ans=v;
	}
	cout<<ans<<'\n';
	return 0;
}
