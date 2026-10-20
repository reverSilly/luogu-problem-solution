//
// 题目：P1580 yyy loves Easter_Egg I
// https://www.luogu.com.cn/problem/P1580
// 本地验证:官方样例 2/2 回归 + 5000 组随机对拍 std==brute + 4MB 最坏规模计时(见 P1580_check.py)
#include<bits/stdc++.h>
using namespace std;

string tgt;                    // 被轰炸的目标:第一句话艾特的人(形如 "yyy loves XXX")

string trim(const string&s)    // 剥首尾空白:样例里 "yyy loves OI : ..." 的发送者带尾随空格,输出前必须剥掉
{
	int l=0,r=(int)s.size()-1;
	while(l<=r&&isspace((unsigned char)s[l]))++l;
	while(r>=l&&isspace((unsigned char)s[r]))--r;
	return s.substr(l,r-l+1);
}

// 取 s[at]=='@' 之后被艾特者的名字:名字格式为 "yyy loves XXX",按空白切出前 3 个 token
string atname(const string&s,int at)
{
	string res;
	for(int i=at+1,c=0;i<(int)s.size()&&c<3;)
	{
		while(i<(int)s.size()&&s[i]==' ')++i;
		if(i>=(int)s.size())break;
		int j=i;
		while(j<(int)s.size()&&s[j]!=' ')++j;
		if(c)res+=' ';
		res+=s.substr(i,j-i);
		++c;
		i=j;
	}
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	string line;
	int n=0;                    // 已读入的消息条数(=队形长度候选)
	while(getline(cin,line))
	{
		while(!line.empty()&&line.back()=='\r')line.pop_back();   // 防 CRLF 行尾
		if(line.empty())break;  // 题面保证:文件以一行空行结尾
		++n;
		int c=line.find(':');
		string sdr=trim(line.substr(0,c));        // 发送者(内容保证无 ':',第一个 ':' 即分隔符)
		string ctx=line.substr(c+1);              // 消息内容
		if(n==1)
		{
			tgt=atname(ctx,(int)ctx.find('@'));  // 题面保证第一句恰一个 @ 且不艾特自己
			continue;
		}
		if(sdr==tgt)             // 被炸的人亲自发言(队形仍完好)→轰炸成功
		{
			cout<<"Successful @"<<tgt<<" attempt\n";
			return 0;
		}
		size_t at=ctx.find('@');
		bool brk=(at==string::npos)||(ctx.find('@',at+1)!=string::npos)||(atname(ctx,(int)at)!=tgt);
		if(brk)                  // 本句第一次破坏队形:出行号与破坏者 id
		{
			cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<n<<'\n'<<sdr<<'\n';
			return 0;
		}
	}
	cout<<"Unsuccessful @"<<tgt<<" attempt\n"<<n<<"\nGood Queue Shape\n";
	return 0;
}
