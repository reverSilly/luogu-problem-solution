// 题目：P1581 A+B Problem(升级版) —— 暴力基准(十进制中转,小数据绝对正确)
// https://www.luogu.com.cn/problem/P1581
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};
i64 pr[mxn]={2,3,5,7,11,13,17,19,23,29,31,37,41,43,47};
i64 d[mxn];                    // 分词后的各位(高位在前)
int e[mxn];                    // 答案各位(个位在前)

// 混合进制串→十进制:Horner,从最高位起,每往右一位乘以下一位的质数基
i64 val(const string&t)
{
	int m=0;
	i64 cur=0;
	for(char ch:t)
	{
		if(ch==',')
		{
			d[m++]=cur;
			cur=0;
		}
		else
			cur=cur*10+(ch-'0');
	}
	d[m++]=cur;
	i64 v=0;
	for(int i=0;i<m;++i)
		v=v*pr[m-1-i]+d[i];
	return v;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	cin>>s;
	int pos=s.find('+');
	i64 x=val(s.substr(0,pos))+val(s.substr(pos+1));
	int m=0;
	do                              // 逐位除质数基,个位起;do-while 保证 x=0 也输出 "0"
	{
		e[m]=x%pr[m];
		x/=pr[m];
		++m;
	}while(x);
	for(int i=m-1;i>=0;--i)
		cout<<e[i]<<(i?',':'\n');
	return 0;
}
