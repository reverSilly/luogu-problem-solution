//
// P1580 暴力基准:把全部消息存下来,先找出第一处破队形的行号,再单独扫"被炸的人是否发过言"
// 与标程(边读边判、提前退出)是两条独立代码路径,小规模下输出必须逐字节一致。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};      // 行数 <=5e4 (+5 松弛)

string ln[mxn];                // 原始消息行(不含结尾空行)
string sdr[mxn];               // 每句发送者(剥首尾空白后)
int nat[mxn];                  // 每句内容里 '@' 的个数(>=2 统一记 2)
string nm[mxn];                // 恰有一个 '@' 时被艾特者的名字

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string line;
	int n=0;
	while(getline(cin,line))
	{
		while(!line.empty()&&line.back()=='\r')line.pop_back();
		if(line.empty())break;
		ln[n++]=line;
	}
	// 第一遍:逐句手工解析(不用 string::find/trim,全部下标循环独立实现一遍)
	for(int i=0;i<n;++i)
	{
		int c=-1;
		for(int j=0;j<(int)ln[i].size();++j)
			if(ln[i][j]==':')
			{
				c=j;
				break;
			}
		int a=0,b=(c<0?(int)ln[i].size():c)-1;   // 发送者=第一个 ':' 之前的那段
		while(a<=b&&isspace((unsigned char)ln[i][a]))++a;
		while(b>=a&&isspace((unsigned char)ln[i][b]))--b;
		sdr[i]=ln[i].substr(a,b-a+1);
		int at=-1,cnt=0;
		for(int j=c+1;j<(int)ln[i].size();++j)
			if(ln[i][j]=='@')
			{
				++cnt;
				if(at<0)at=j;
			}
		nat[i]=min(cnt,2);
		if(cnt==1)              // 恰一个 @:按空白切前 3 个 token 当名字
		{
			string res;
			for(int j=at+1,t=0;j<(int)ln[i].size()&&t<3;)
			{
				while(j<(int)ln[i].size()&&ln[i][j]==' ')++j;
				if(j>=(int)ln[i].size())break;
				int k=j;
				while(k<(int)ln[i].size()&&ln[i][k]!=' ')++k;
				if(t)res+=' ';
				res+=ln[i].substr(j,k-j);
				++t;
				j=k;
			}
			nm[i]=res;
		}
	}
	string tgt=nm[0];           // 题面保证第一句恰艾特一人
	int k=0;                    // 第一次破队形的行号(1-based),0 表示从未破
	for(int i=1;i<n;++i)
		if(nat[i]!=1||nm[i]!=tgt)
		{
			k=i+1;
			break;
		}
	bool win=false;            // 被炸的人是否在队形完好期间(第 2..k 句)发过言
	for(int i=1,last=(k?k:n);i<last;++i)
		if(sdr[i]==tgt)
		{
			win=true;
			break;
		}
	if(win)
		cout<<"Successful @"<<tgt<<" attempt\n";
	else if(k)
		cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<k<<'\n'<<sdr[k-1]<<'\n';
	else
		cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<n<<"\nGood Queue Shape\n";
	return 0;
}
