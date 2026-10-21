//
// 题目：P1615 西游记公司
// https://www.luogu.com.cn/problem/P1615
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

string s1,s2;
i64 n;

int parse(const string&s)          // "hh:mm:ss" -> 当天第几秒
{
	int h=0,mi=0,se=0;
	sscanf(s.c_str(),"%d:%d:%d",&h,&mi,&se);
	return h*3600+mi*60+se;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s1>>s2>>n;
	cout<<(i64)(parse(s2)-parse(s1))*n<<'\n';
	return 0;
}
