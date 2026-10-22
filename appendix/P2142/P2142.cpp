#include<bits/stdc++.h>
using namespace std;

string a,b;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;
	//去掉前导零，方便比较
	auto trim=[](string&s){
		size_t p=s.find_first_not_of('0');
		if(p==string::npos)
			s="0";
		else
			s=s.substr(p);
	};
	trim(a);
	trim(b);

	bool neg=false;
	if(a.size()<b.size()||(a.size()==b.size()&&a<b))
	{
		swap(a,b);
		neg=true;
	}

	//a>=b，逐位借位
	string res;
	int borrow=0;
	for(int i=(int)a.size()-1,j=(int)b.size()-1;i>=0;--i,--j)
	{
		int x=a[i]-'0'-borrow;
		int y=j>=0?b[j]-'0':0;
		if(x<y)
		{
			x+=10;
			borrow=1;
		}
		else
			borrow=0;
		res.push_back(char('0'+x-y));
	}
	while(res.size()>1&&res.back()=='0')
		res.pop_back();
	reverse(res.begin(),res.end());

	if(neg&&res!="0")
		cout<<'-';
	cout<<res<<'\n';
	return 0;
}
