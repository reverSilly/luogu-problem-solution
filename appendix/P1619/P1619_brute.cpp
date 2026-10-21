//
// P1619 朴素解：逐个整数试除判素、试除分解
// 用法：仅用于与 P1619.cpp 对拍，非提交代码
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int lim{40000000};

bool isp(int x)
{
	if(x<2)
		return false;
	for(i64 d=2;d*d<=x;++d)
		if(x%d==0)
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string all((istreambuf_iterator<char>(cin)),istreambuf_iterator<char>());
	vector<string>line;
	string cur;
	for(char c:all)
		if(c=='\n')
		{
			line.push_back(cur);
			cur.clear();
		}
		else
			cur+=c;
	if(!cur.empty())
		line.push_back(cur);

	vector<int>num;
	for(auto&s:line)
	{
		bool has=false;
		i64 v=0;
		for(char c:s)
			if(isdigit((unsigned char)c))
			{
				has=true;
				v=v*10+(c-'0');
				if(v>lim)
					v=lim+1;
			}
		if(!has)
			break;
		num.push_back((int)v);
	}

	for(int i=0;i<(int)num.size();++i)
	{
		int n=num[i];
		if(i)
			cout<<'\n';
		cout<<"Enter the number=\n";
		if(n<2)
		{
			cout<<"Prime? No!\n";
			continue;
		}
		if(n>lim)
		{
			cout<<"Prime? No!\nThe number is too large!\n";
			continue;
		}
		if(isp(n))
		{
			cout<<"Prime? Yes!\n";
			continue;
		}
		cout<<"Prime? No!\n"<<n<<'=';
		int x=n;
		bool first=true;
		for(i64 d=2;d*d<=x;++d)
		{
			if(x%d)
				continue;
			int e=0;
			while(x%d==0)
			{
				x/=d;
				++e;
			}
			if(!first)
				cout<<'*';
			first=false;
			cout<<d;
			if(e>1)
				cout<<'^'<<e;
		}
		if(x>1)
		{
			if(!first)
				cout<<'*';
			cout<<x;
		}
		cout<<'\n';
	}
	return 0;
}
