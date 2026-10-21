//
// 题目：P1597 语句解析
// https://www.luogu.com.cn/problem/P1597
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

string s,t;
int val[3];                     // val[0..2] 对应 a,b,c, 未赋值为 0

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	while(getline(cin,t))
		s+=t;                   // 语句可能跨行, 先去换行拼成一串

	for(int i=0;i+3<(int)s.size();++i)
		if(s[i]>='a'&&s[i]<='c'&&s[i+1]==':'&&s[i+2]=='=')
		{
			char x=s[i+3];      // 右值: 一位数字或变量
			val[s[i]-'a']=x>='0'&&x<='9'?x-'0':val[x-'a'];
			i+=3;               // 跳过 ":=" 与右值
		}
	cout<<val[0]<<' '<<val[1]<<' '<<val[2]<<'\n';
	return 0;
}
