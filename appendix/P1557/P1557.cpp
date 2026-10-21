//
// 题目：P1557 Kruscal的加法
// https://www.luogu.com.cn/problem/P1557
// AC
#include<bits/stdc++.h>
using namespace std;

// 非负高精度整数封装
struct BigUInt
{
	vector<int> d;                // 低位在前 (base 10)
	BigUInt(long long v=0)
	{
		if(v==0)
			d.push_back(0);
		while(v>0)
		{
			d.push_back(v%10);
			v/=10;
		}
	}
	void trim()
	{
		while(d.size()>1&&d.back()==0)
			d.pop_back();
	}
	bool is_zero()const{return d.size()==1&&d[0]==0;}
};

int cmp_abs(const BigUInt& a,const BigUInt& b)
{
	if(a.d.size()!=b.d.size())
		return a.d.size()<b.d.size()?-1:1;
	for(int i=static_cast<int>(a.d.size())-1;i>=0;--i)
		if(a.d[i]!=b.d[i])
			return a.d[i]<b.d[i]?-1:1;
	return 0;
}

BigUInt add_abs(const BigUInt& a,const BigUInt& b)
{
	BigUInt c;
	c.d.clear();
	int n=max(a.d.size(),b.d.size()),carry=0;
	for(int i=0;i<n||carry;++i)
	{
		int s=carry;
		if(i<static_cast<int>(a.d.size())) s+=a.d[i];
		if(i<static_cast<int>(b.d.size())) s+=b.d[i];
		c.d.push_back(s%10);
		carry=s/10;
	}
	if(c.d.empty()) c.d.push_back(0);
	c.trim();
	return c;
}

// 要求 a >= b
BigUInt sub_abs(const BigUInt& a,const BigUInt& b)
{
	BigUInt c;
	c.d.clear();
	int borrow=0;
	for(size_t i=0;i<a.d.size();++i)
	{
		int sub=a.d[i]-borrow-(i<b.d.size()?b.d[i]:0);
		if(sub<0)
		{
			sub+=10;
			borrow=1;
		}
		else
			borrow=0;
		c.d.push_back(sub);
	}
	c.trim();
	return c;
}

BigUInt mul_abs(const BigUInt& a,const BigUInt& b)
{
	BigUInt c;
	c.d.assign(a.d.size()+b.d.size(),0);
	for(size_t i=0;i<a.d.size();++i)
	{
		int carry=0;
		for(size_t j=0;j<b.d.size()||carry;++j)
		{
			long long cur=c.d[i+j]+carry+1LL*a.d[i]*(j<b.d.size()?b.d[j]:0);
			c.d[i+j]=cur%10;
			carry=cur/10;
		}
	}
	c.trim();
	return c;
}

BigUInt from_str(const string& s)
{
	BigUInt res;
	res.d.clear();
	for(int i=static_cast<int>(s.size())-1;i>=0;--i)
		res.d.push_back(s[i]-'0');
	if(res.d.empty())
		res.d.push_back(0);
	res.trim();
	return res;
}

struct SignedBig
{
	int sgn{1};                   // +1 或 -1 (0 统一为 +1)
	BigUInt mag{0};
	void normalize()
	{
		mag.trim();
		if(mag.is_zero())
			sgn=1;
	}
	void add_term(int term_sgn,const BigUInt& term_mag)
	{
		if(term_mag.is_zero())
			return;
		if(sgn==term_sgn)
			mag=add_abs(mag,term_mag);
		else
		{
			int c=cmp_abs(mag,term_mag);
			if(c>=0)
				mag=sub_abs(mag,term_mag);
			else
			{
				mag=sub_abs(term_mag,mag);
				sgn=term_sgn;
			}
		}
		normalize();
	}
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	if(!(cin>>s))
		return 0;

	SignedBig total;
	int n=s.size(),i=0;
	while(i<n)
	{
		int sgn=1;
		BigUInt mult(1);
		if(s[i]=='+'||s[i]=='-')
		{
			char ch=s[i];
			sgn=(ch=='+')?1:-1;
			if(i+1<n&&s[i+1]=='(')
			{
				i+=2;             // 跳过符号和 '('
				int st=i;
				while(i<n&&s[i]!=')')
					++i;
				mult=from_str(s.substr(st,i-st));
				if(i<n&&s[i]==')')
					++i;
			}
			else
			{
				int cnt=0;
				while(i<n&&s[i]==ch)
				{
					++cnt;
					++i;
				}
				mult=BigUInt(cnt);
			}
		}
		int st=i;
		while(i<n&&isdigit(static_cast<unsigned char>(s[i])))
			++i;
		BigUInt val=from_str(s.substr(st,i-st));
		BigUInt prod=mul_abs(mult,val);
		total.add_term(sgn,prod);
	}

	if(total.sgn==-1&&!total.mag.is_zero())
		cout<<'-';
	for(int j=static_cast<int>(total.mag.d.size())-1;j>=0;--j)
		cout<<total.mag.d[j];
	cout<<'\n';
	return 0;
}
