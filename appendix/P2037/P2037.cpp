// 题目：P2037 [ECNA 1999] 电话号码
// 把号码统一成"前3位-后4位"的标准形式，再用 map 统计出现次数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

char mp[26];	// 字母 -> 数字，'Q' 与 'Z' 不出现
map<string,int> cnt;

void init()
{
	// 注意：7 对应 P,R,S；9 对应 W,X,Y（题面无 Q、Z）
	const string g[8]={"ABC","DEF","GHI","JKL","MNO","PRS","TUV","WXY"};
	for(int i=0;i<8;++i)
		for(char c:g[i]) mp[c-'A']=char('2'+i);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	init();
	int N;
	cin>>N;
	for(int i=0;i<N;++i)
	{
		string raw;
		cin>>raw;
		string dig;
		for(char c:raw)
		{
			if(c=='-') continue;			// 连字符可忽略
			if(c>='0'&&c<='9') dig+=c;
			else dig+=mp[c-'A'];
		}
		// 标准形式：七位数字，中间用连字符分成前三个和后四个
		string key=dig.substr(0,3)+"-"+dig.substr(3);
		cnt[key]++;
	}
	bool any=false;
	for(auto &p:cnt)
		if(p.second>1)
		{
			cout<<p.first<<' '<<p.second<<'\n';
			any=true;
		}
	if(!any) cout<<"No duplicates.\n";
	return 0;
}
