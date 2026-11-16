// 题目：P2715 ccj与zrz之在回家的路上
// getline 整行 → 删全部空白 → 定位运算符与 = → 切三段
// 4 运算 × 3 未知位置 = 12 种逆运算；所有值 <=1e9 < 2^53，
// double 整数精确、一次除法误差 ~1e-7 << 0.005，setprecision(2) 即可
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

string s,t;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	getline(cin,s);
	for(char c:s)
		if(c!=' '&&c!='\t'&&c!='\r')
			t+=c;	// 删空白不破坏结构：数字非负，字符集互不冲突
	int op=-1,eq=-1;
	for(int i=0;i<(int)t.size();++i)
	{
		if(t[i]=='=')
			eq=i;
		else if(t[i]=='+'||t[i]=='-'||t[i]=='*'||t[i]=='/')
			op=i;
	}
	string A=t.substr(0,op),B=t.substr(op+1,eq-op-1),C=t.substr(eq+1);
	char o=t[op];
	double ans=0;
	if(A=="?")
	{
		i64 b=stoll(B),c=stoll(C);
		if(o=='+')ans=c-b;
		else if(o=='-')ans=b+c;
		else if(o=='*')ans=(double)c/b;	// 保证 b!=0（答案存在唯一）
		else ans=(double)b*c;
	}
	else if(B=="?")
	{
		i64 a=stoll(A),c=stoll(C);
		if(o=='+')ans=c-a;
		else if(o=='-')ans=a-c;
		else if(o=='*')ans=(double)c/a;	// 保证 a!=0
		else ans=(double)a/c;	// 保证 c!=0
	}
	else
	{
		i64 a=stoll(A),b=stoll(B);
		if(o=='+')ans=a+b;
		else if(o=='-')ans=a-b;
		else if(o=='*')ans=a*b;	// 等式约束 a*b=c<=1e9，不会到 1e18
		else ans=(double)a/b;
	}
	cout<<fixed<<setprecision(2)<<ans<<'\n';
	return 0;
}