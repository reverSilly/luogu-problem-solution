#include<bits/stdc++.h>
using namespace std;

// 使用双栈法(算符优先)独立验证布尔表达式语法与求值
int main()
{
	vector<string> t;
	string s;
	while(cin>>s)
		t.push_back(s);
	if(t.empty())
	{
		cout<<"error\n";
		return 0;
	}
	// 1. 有限状态机校验语法合法性
	bool expect_operand=true;
	for(const string& w:t)
	{
		if(expect_operand)
		{
			if(w=="not")
				continue;
			else if(w=="true"||w=="false")
				expect_operand=false;
			else
			{
				cout<<"error\n";
				return 0;
			}
		}
		else
		{
			if(w=="and"||w=="or")
				expect_operand=true;
			else
			{
				cout<<"error\n";
				return 0;
			}
		}
	}
	if(expect_operand)
	{
		cout<<"error\n";
		return 0;
	}

	// 2. 先消除右结合的一元运算 not
	vector<string> t2;
	for(int i=static_cast<int>(t.size())-1;i>=0;--i)
	{
		if(t[i]=="not")
		{
			t2.back()=(t2.back()=="true"?"false":"true");
		}
		else
			t2.push_back(t[i]);
	}
	reverse(t2.begin(),t2.end());

	// 3. 消除二元运算 and
	vector<string> t3;
	for(size_t i=0;i<t2.size();++i)
	{
		if(t2[i]=="and")
		{
			bool lhs=(t3.back()=="true");
			bool rhs=(t2[++i]=="true");
			t3.back()=(lhs&&rhs)?"true":"false";
		}
		else
			t3.push_back(t2[i]);
	}

	// 4. 消除二元运算 or
	bool ans=(t3[0]=="true");
	for(size_t i=1;i<t3.size();i+=2)
	{
		bool rhs=(t3[i+1]=="true");
		ans=ans||rhs;
	}
	cout<<(ans?"true\n":"false\n");
	return 0;
}
