// 题目：P2614 计算器弹琴
// 思路：所有时值都是 1/16 拍的整数倍（最小 32 分附点=3/16）⇒ 以 1/16 拍为单位全程整数。
//       深度 d 基础 16>>d 单位；每个 - 加 16；附点 val*3/2（val 恒偶，无损）。
//       滤空白拼接 → 深度计数器（无需栈）→ 音符贪心 lookahead 吃 - 与 .。
//       秒数 = units/16 拍 × 60/T = units*15/(4T)，整数除法天然向下取整。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n,T;
bool isNote[128];
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(char c:string("+<*/123456789=%CM"))	// 17 个音高符号
		isNote[(unsigned char)c]=true;
	cin>>n>>T;
	string line;
	getline(cin,line);	// 吃掉第一行剩余
	for(int i=0;i<n;++i)
	{
		getline(cin,line);
		for(char c:line)
			if(!isspace((unsigned char)c))
				s+=c;	// 滤空白，跨行音符拼接后连续
	}
	i64 units=0;	// 以 1/16 拍为单位
	int depth=0;
	for(int i=0;i<(int)s.size();++i)
	{
		char c=s[i];
		if(c=='('){++depth;continue;}
		if(c==')'){--depth;continue;}
		if(isNote[(unsigned char)c])
		{
			i64 val=16>>depth;	// 深度 depth 的基础时值
			while(i+1<(int)s.size()&&s[i+1]=='-'){val+=16;++i;}	// 每个 - 加一拍
			if(i+1<(int)s.size()&&s[i+1]=='.'){val=val*3/2;++i;}	// 附点延长 1/2，val 恒偶故精确
			units+=val;
		}
	}
	cout<<units*15/(4*T)<<'\n';	// units/16 拍 × 60/T 秒，向下取整
	return 0;
}
