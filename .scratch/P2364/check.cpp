// 验证输出: 三串公共子序列 + 长度等于暴力最优
#include<bits/stdc++.h>
using namespace std;
bool issub(const string&s,const string&t)
{
	int p=0;
	for(char ch:t) if(p<(int)s.size()&&s[p]==ch)++p;
	return p==(int)s.size();
}
int main()
{
	FILE*f1=fopen("in.txt","r"),*f2=fopen("o1.txt","r"),*f3=fopen("o2.txt","r");
	char b1[1000]={0},b2[1000]={0},b3[1000]={0};
	fscanf(f1,"%s %s %s",b1,b2,b3);
	fscanf(f2,"%s",b1+500);
	fscanf(f3,"%s",b2+500);
	string a(b1),b(b2),c(b3),ans(b1+500),exp(b2+500);
	if(ans.size()!=exp.size()){printf("LEN %d vs %d\n",(int)ans.size(),(int)exp.size());return 1;}
	if(!issub(ans,a)||!issub(ans,b)||!issub(ans,c)){puts("NOT SUBSEQ");return 1;}
	return 0;
}
