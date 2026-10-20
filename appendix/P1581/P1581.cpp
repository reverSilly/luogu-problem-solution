// 题目：P1581 A+B Problem(升级版)
// https://www.luogu.com.cn/problem/P1581
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{11};                      // 加数至多 5 位,和至多 6 位(+5 松弛)
int pr[mxn]={2,3,5,7,11,13,17,19,23,29,31}; // pr[i]=个位起第 i 位的质数基
int a[mxn],b[mxn],c[mxn];                   // a/b=加数,c=和,均按个位起 0-based

// "1,0"→d[0]=个位,d[1]=十位,...;万位可能占两位,必须按 ',' 分词而非逐字符
void parse(const string&t,int*d)
{
	int m=0,cur=0;
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
	for(int i=0;i<m/2;++i)                   // 题面高位在前,翻成个位起
		swap(d[i],d[m-1-i]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string s;
	cin>>s;
	int pos=s.find('+');
	parse(s.substr(0,pos),a);
	parse(s.substr(pos+1),b);
	int carry=0,hi=0;
	for(int i=0;i<7||carry;++i)               // 万位还可能再进一位(基 13),故放到第 6 位
	{
		int t=a[i]+b[i]+carry;
		c[i]=t%pr[i];
		carry=t/pr[i];
		if(c[i])
			hi=i;
	}
	if(hi==0&&!c[0])                         // 和为 0
	{
		cout<<0<<'\n';
		return 0;
	}
	for(int i=hi;i>=0;--i)
		cout<<c[i]<<(i?',':'\n');
	return 0;
}
