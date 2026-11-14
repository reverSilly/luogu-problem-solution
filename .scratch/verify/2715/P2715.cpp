#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string line,s;
	getline(cin,line);
	for(char c:line)
		if(c!=' ')
			s+=c;
	int eq=s.find('=');
	int op=-1;
	char opc=0;
	for(int i=0;i<eq;++i)
		if(s[i]=='+'||s[i]=='-'||s[i]=='*'||s[i]=='/')
		{
			op=i;opc=s[i];break;
		}
	string A=s.substr(0,op),B=s.substr(op+1,eq-op-1),C=s.substr(eq+1);
	bool qa=A=="?",qb=B=="?",qc=C=="?";
	i64 a=0,b=0,c=0;
	if(!qa)a=stoll(A);
	if(!qb)b=stoll(B);
	if(!qc)c=stoll(C);
	i64 num=0,den=1;	// 答案 = num/den（整数情形 den=1）
	if(opc=='+')
		num=qa?c-b:qb?c-a:a+b;
	else if(opc=='-')
		num=qa?c+b:qb?a-c:a-b;
	else if(opc=='*')
	{
		if(qa)num=c,den=b;
		else if(qb)num=c,den=a;
		else num=a*b;
	}
	else
	{
		if(qa)num=c*b;
		else if(qb)num=a,den=c;
		else num=a,den=b;
	}
	// 四舍五入到 2 位小数：cents = floor(100*num/den + 1/2)，全程整数
	i64 cents=(200*num+den)/(2*den);
	cout<<cents/100<<'.'<<setw(2)<<setfill('0')<<cents%100<<'\n';
	return 0;
}