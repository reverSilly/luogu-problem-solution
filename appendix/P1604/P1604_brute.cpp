//
// P1604 暴力: 把 B 进制数转成 10^9 基大整数相加, 再除 B 取余还原
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int BASE{1000000000};

int B;

int dv(char c)
{
	return c>='0'&&c<='9'?c-'0':c-'A'+10;
}

vector<int> readnum(const string&s)
{
	vector<int>a{0};
	for(char c:s)
	{
		i64 carry=dv(c);
		for(size_t i=0;i<a.size();++i)
		{
			i64 cur=(i64)a[i]*B+carry;
			a[i]=int(cur%BASE);
			carry=cur/BASE;
		}
		while(carry)
		{
			a.push_back(int(carry%BASE));
			carry/=BASE;
		}
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
	cin>>B>>s>>t;
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

	vector<int>dig;
	while(!(a.size()==1&&a[0]==0))          // 反复除以 B, 余数即低位
	{
		i64 rem=0;
		for(int i=(int)a.size()-1;i>=0;--i)
		{
			i64 cur=rem*BASE+a[i];
			a[i]=int(cur/B);
			rem=cur%B;
		}
		dig.push_back(int(rem));
		while(a.size()>1&&a.back()==0)
			a.pop_back();
	}
	if(dig.empty())
		dig.push_back(0);
	for(int i=(int)dig.size()-1;i>=0;--i)
		cout<<(dig[i]<10?char('0'+dig[i]):char('A'+dig[i]-10));
	cout<<'\n';
	return 0;
}
