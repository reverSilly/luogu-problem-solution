// 题目：P2366 yyy2015c01 的 IDE 之 Watches
// 思路：整行读入 -> 按 '=' 切出左右值 -> 右值按 '+' 切成操作数
//       全数字的是常量，否则查符号表（没赋过值就是 0）
//       输出集合必须单独记录：查表时会往 map 里插入 0，不能拿 map 的键集当答案集
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n;
map<string,i64>val;      // 变量名 -> 当前值；从没出现过的名字查出来就是 0
set<string>asg;          // 被赋值过的变量名，只有这些要输出

bool isNum(const string&s)
{
	if(s.empty())
		return false;
	for(char c:s)
		if(c<'0'||c>'9')
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	string line;
	getline(cin,line);        // 吃掉第一行末尾的换行符
	for(int cnt=0;cnt<n;)
	{
		if(!getline(cin,line))
			break;
		if(!line.empty()&&line.back()=='\r')
			line.pop_back();   // 防御 Windows 换行
		if(line.empty())
			continue;
		++cnt;
		size_t eq=line.find('=');
		string lhs=line.substr(0,eq);
		string rhs=line.substr(eq+1);
		if(!rhs.empty()&&rhs.back()==';')
			rhs.pop_back();
		i64 s=0;
		for(size_t st=0;;)
		{
			size_t pl=rhs.find('+',st);
			string tk=rhs.substr(st,pl==string::npos?string::npos:pl-st);
			s+=isNum(tk)?stoll(tk):val[tk];   // val[tk] 顺手把未赋值变量置 0
			if(pl==string::npos)
				break;
			st=pl+1;
		}
		val[lhs]=s;
		asg.insert(lhs);
	}
	for(const string&s:asg)
		cout<<s<<' '<<val[s]<<'\n';
	return 0;
}
