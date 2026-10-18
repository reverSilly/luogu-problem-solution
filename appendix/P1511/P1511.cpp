//
// 题目：P1511 [SDOI2006] 数字串位置 / 超长数字串
// https://www.luogu.com.cn/problem/P1511
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{215};           // |A|<=200 (+15 松弛)
constexpr int inf{1145141919};

string a;
int m;

// 十进制正整数串 +1 (原地或返回新串, 如 "99"->"100")
string incStr(string s)
{
	int i=(int)s.size()-1;
	for(;i>=0&&s[i]=='9';--i)
		s[i]='0';
	if(i>=0)
		++s[i];
	else
		s="1"+s;
	return s;
}

// 十进制正整数串 -1 (调用前提: s 数值 > 10^(|s|-1), 不退位)
string decStr(string s)
{
	int i=(int)s.size()-1;
	for(;i>=0&&s[i]=='0';--i)
		s[i]='9';
	--s[i];
	return s;
}

// 检验从 L 位正整数 x 的第 p 位(0-based)开始往后拼接 x,x+1,x+2,... 能否匹配 a
bool check(const string&x,int p)
{
	if((int)x.size()==0||x[0]=='0')
		return false;
	string cur=x;
	int idx=p;
	for(int i=0;i<m;++i)
	{
		if(idx==(int)cur.size())
		{
			cur=incStr(cur);
			idx=0;
		}
		if(cur[idx++]!=a[i])
			return false;
	}
	return true;
}

// 计算 k(X,p) = L*X - 11...1(L个1) + p + 1 的高精度十进制表示
string calcPos(const string&x,int p)
{
	int L=(int)x.size();
	int d[mxn]{};                 // 低位在 d[0], 先算 L*X + (p+1) - 11...1
	int carry=p+1;
	for(int i=0;i<L;++i)
	{
		int v=(x[L-1-i]-'0')*L+carry-1;   // 同步减去第 i 位的 1
		int q=v/10,r=v%10;
		if(r<0)
			r+=10,--q;
		d[i]=r;
		carry=q;
	}
	int len=L;
	while(carry>0)
	{
		d[len++]=carry%10;
		carry/=10;
	}
	while(len>1&&d[len-1]==0)
		--len;
	string res;
	for(int i=len-1;i>=0;--i)
		res.push_back(char('0'+d[i]));
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>a))
		return 0;
	m=(int)a.size();

	string bestX;
	int bestP=inf;
	for(int L=1;L<=m+1;++L)
	{
		for(int q=1;q<=min(L,m);++q)
		{
			int p=L-q;
			string low=a.substr(0,q);
			vector<string>cands;
			if(p==0)
				cands.push_back(low);
			else if(q==m)
				cands.push_back("1"+string(p-1,'0')+low);
			else
			{
				int r=min(m-q,p);
				string P=a.substr(q,r);
				if(P[0]=='0')
					continue;             // X+1 的最高位不能是 0
				if(low!=string(q,'9'))
					cands.push_back(P+string(p-r,'0')+low);
				else
				{
					string one="1"+string(r-1,'0');
					if(P!=one)
						cands.push_back(decStr(P)+string(p-r,'9')+low);
					else
					{
						if(r<p)
							cands.push_back("1"+string(p-1,'0')+low);
						cands.push_back(string(p,'9')+low);
					}
				}
			}
			for(const string&x:cands)
			{
				if((int)x.size()!=L||x[0]=='0')
					continue;
				if(check(x,p))
				{
					if(bestX.empty()||make_pair(x,p)<make_pair(bestX,bestP))
					{
						bestX=x;
						bestP=p;
					}
				}
			}
		}
		if(!bestX.empty())
			break;                        // L 升序枚举,首个非空层必给出全局最小 (L,X,p)
	}
	cout<<calcPos(bestX,bestP)<<'\n';
	return 0;
}
