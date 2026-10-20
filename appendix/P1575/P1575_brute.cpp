//
// P1575 暴力基准：调度场(shunting-yard)双栈迭代求值 + 状态机式错误检测
// 与递推下降完全不同的实现,用于交叉验证。
#include<bits/stdc++.h>
using namespace std;

int prec(const string&o)
{
	if(o=="not")
		return 3;
	if(o=="and")
		return 2;
	if(o=="or")
		return 1;
	return 0;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	vector<string>tk;
	string s;
	while(cin>>s)
		tk.push_back(s);
	vector<char>val;      // 值栈 0/1
	vector<string>op;     // 算符栈
	bool ok=true;
	bool expect=1;        // 1=期望操作数, 0=期望算符
	for(auto&t:tk)
	{
		if(t=="true"||t=="false")
		{
			if(!expect){ok=false;break;}
			val.push_back(t=="true");
			expect=0;
		}
		else if(t=="not")
		{
			if(!expect){ok=false;break;}
			op.push_back(t);
		}
		else if(t=="and"||t=="or")
		{
			if(expect){ok=false;break;}
			int pr=prec(t);
			while(!op.empty()&&prec(op.back())>=pr)
			{
				string o=op.back();
				op.pop_back();
				if(o=="not")
				{
					if(val.empty()){ok=false;break;}
					val.back()^=1;
				}
				else
				{
					if(val.size()<2){ok=false;break;}
					char y=val.back();val.pop_back();
					char x=val.back();val.pop_back();
					val.push_back(o=="and"?(x&y):(x|y));
				}
			}
			if(!ok)
				break;
			op.push_back(t);
			expect=1;
		}
		else
		{
			ok=false;
			break;
		}
	}
	if(ok&&expect)        // 空串或以算符/not 结尾
		ok=false;
	while(ok&&!op.empty())
	{
		string o=op.back();
		op.pop_back();
		if(o=="not")
		{
			if(val.empty()){ok=false;break;}
			val.back()^=1;
		}
		else
		{
			if(val.size()<2){ok=false;break;}
			char y=val.back();val.pop_back();
			char x=val.back();val.pop_back();
			val.push_back(o=="and"?(x&y):(x|y));
		}
	}
	if(ok&&val.size()!=1)
		ok=false;
	if(ok)
		cout<<(val[0]?"true\n":"false\n");
	else
		cout<<"error\n";
	return 0;
}
