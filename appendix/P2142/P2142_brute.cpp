#include<bits/stdc++.h>
using namespace std;

//暴力：以 10^9 为进制分块存储，做高精度减法（与标程的逐位借位实现独立）
constexpr long long base{1000000000LL};

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	string a,b;
	cin>>a>>b;

	auto tochunk=[](const string&s)
	{
		vector<long long>v;
		for(int i=(int)s.size();i>0;i-=9)
		{
			int l=max(0,i-9);
			long long x=0;
			for(int j=l;j<i;++j)
				x=x*10+(s[j]-'0');
			v.push_back(x);
		}
		while(v.size()>1&&v.back()==0)
			v.pop_back();
		return v;
	};

	vector<long long>x=tochunk(a),y=tochunk(b);
	bool neg=x.size()<y.size()||(x.size()==y.size()&&a<b);
	if(neg)swap(x,y);
	size_t n=max(x.size(),y.size());
	x.resize(n,0);
	y.resize(n,0);
	long long borrow=0;
	for(size_t i=0;i<n;++i)
	{
		long long d=x[i]-y[i]-borrow;
		if(d<0)
		{
			d+=base;
			borrow=1;
		}
		else
			borrow=0;
		x[i]=d;
	}
	while(x.size()>1&&x.back()==0)
		x.pop_back();
	string res=to_string(x.back());
	for(int i=(int)x.size()-2;i>=0;--i)
	{
		string t=to_string(x[i]);
		res+=string(9-t.size(),'0')+t;
	}
	if(neg&&res!="0")
		cout<<'-';
	cout<<res<<'\n';
	return 0;
}
