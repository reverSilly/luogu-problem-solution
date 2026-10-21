//
// P1603 暴力: 枚举所有排列, 取拼接后数值最小者
#include<bits/stdc++.h>
using namespace std;

string W[27]={"zero","one","two","three","four","five","six","seven","eight","nine",
	"ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen",
	"eighteen","nineteen","twenty","a","both","another","first","second","third"};
int V[27]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,1,2,1,1,2,3};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string line;
	getline(cin,line);
	stringstream ss(line);
	string t;
	vector<string>blk;
	while(ss>>t)                                   // 按空格切
	{
		while(!t.empty()&&!isalpha((unsigned char)t.back()))
			t.pop_back();                          // 去掉句号
		for(char&c:t)
			c=tolower((unsigned char)c);
		for(int i=0;i<27;++i)
			if(t==W[i])
			{
				int v=V[i]*V[i]%100;
				blk.push_back(string(1,char('0'+v/10))+char('0'+v%10));
				break;
			}
	}
	if(blk.empty())
	{
		cout<<0<<'\n';
		return 0;
	}

	sort(blk.begin(),blk.end());
	string best;
	bool first=true;
	do
	{
		string s;
		for(auto&x:blk)
			s+=x;
		size_t p=0;
		while(p<s.size()&&s[p]=='0')
			++p;
		string c=p==s.size()?"0":s.substr(p);
		if(first||c.size()<best.size()||(c.size()==best.size()&&c<best))
		{
			first=false;
			best=c;
		}
	}while(next_permutation(blk.begin(),blk.end()));

	cout<<best<<'\n';
	return 0;
}
