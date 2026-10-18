//
// 题目：P1511 [SDOI2006] 数字串位置 / 超长数字串
// 暴力基准: 直接按定义将 1,2,3,... 顺序拼接为字符串 S, 查找首次出现位置(1-based)
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string a;
	if(!(cin>>a))
		return 0;
	string s;
	s.reserve(2000000);
	for(int i=1;i<=300000;++i)
	{
		s+=to_string(i);
		if(s.size()>=a.size()+20&&(i%2000==0||i<=200))
		{
			auto pos=s.find(a);
			if(pos!=string::npos)
			{
				cout<<pos+1<<'\n';
				return 0;
			}
		}
	}
	auto pos=s.find(a);
	cout<<pos+1<<'\n';
	return 0;
}
