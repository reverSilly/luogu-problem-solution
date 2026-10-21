//
// P1601 暴力: 以 10^9 为基的大整数(与标程的逐位字符串完全不同)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int BASE{1000000000};

vector<int> readnum(const string&s)
{
	vector<int>a{0};
	for(char c:s)
	{
		i64 carry=c-'0';
		for(size_t i=0;i<a.size();++i)
		{
			i64 cur=(i64)a[i]*10+carry;
			a[i]=int(cur%BASE);
			carry=cur/BASE;
		}
		if(carry)
			a.push_back(int(carry));
	}
	while(a.size()>1&&a.back()==0)
		a.pop_back();
	return a;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s,t;
	cin>>s>>t;
	vector<int>a=readnum(s),b=readnum(t);
	if(a.size()<b.size())
		swap(a,b);
	int carry=0;
	for(size_t i=0;i<a.size();++i)
	{
		i64 cur=(i64)a[i]+(i<b.size()?b[i]:0)+carry;
		a[i]=int(cur%BASE);
		carry=int(cur/BASE);
	}
	if(carry)
		a.push_back(carry);
	printf("%d",a.back());
	for(int i=(int)a.size()-2;i>=0;--i)
		printf("%09d",a[i]);
	printf("\n");
	return 0;
}
