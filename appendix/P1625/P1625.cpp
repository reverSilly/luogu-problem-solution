//
// 题目：P1625 求和
// https://www.luogu.com.cn/problem/P1625
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int B{1000000000};

struct big
{
	vector<int>d;                       // 小端，base=1e9，空表示 0
	big(){}
	big(int x)
	{
		while(x)
		{
			d.push_back(x%B);
			x/=B;
		}
	}
	bool zero()const
	{
		return d.empty();
	}
};

void norm(big&a)
{
	while(!a.d.empty()&&a.d.back()==0)
		a.d.pop_back();
}

int cmp(const big&a,const big&b)
{
	if(a.d.size()!=b.d.size())
		return a.d.size()<b.d.size()?-1:1;
	for(int i=(int)a.d.size()-1;i>=0;--i)
		if(a.d[i]!=b.d[i])
			return a.d[i]<b.d[i]?-1:1;
	return 0;
}

big add(const big&a,const big&b)
{
	big c;
	c.d.assign(max(a.d.size(),b.d.size())+1,0);
	for(size_t i=0;i<a.d.size();++i)
		c.d[i]+=a.d[i];
	for(size_t i=0;i<b.d.size();++i)
		c.d[i]+=b.d[i];
	for(size_t i=0;i+1<c.d.size();++i)
		if(c.d[i]>=B)
		{
			c.d[i]-=B;
			++c.d[i+1];
		}
	norm(c);
	return c;
}

big sub(const big&a,const big&b)        // 保证 a>=b
{
	big c=a;
	for(size_t i=0;i<b.d.size();++i)
		c.d[i]-=b.d[i];
	for(size_t i=0;i+1<c.d.size();++i)
		if(c.d[i]<0)
		{
			c.d[i]+=B;
			--c.d[i+1];
		}
	norm(c);
	return c;
}

big mul(const big&a,const big&b)
{
	big c;
	if(a.zero()||b.zero())
		return c;
	c.d.assign(a.d.size()+b.d.size()+1,0);
	for(size_t i=0;i<a.d.size();++i)
	{
		if(a.d[i]==0)
			continue;
		i64 carry=0;
		for(size_t j=0;j<b.d.size();++j)
		{
			i64 cur=(i64)c.d[i+j]+(i64)a.d[i]*b.d[j]+carry;
			c.d[i+j]=cur%B;
			carry=cur/B;
		}
		size_t k=i+b.d.size();
		while(carry)
		{
			i64 cur=(i64)c.d[k]+carry;
			c.d[k]=cur%B;
			carry=cur/B;
			++k;
		}
	}
	norm(c);
	return c;
}

big mulSmall(const big&a,int x)
{
	big c;
	if(x==0||a.zero())
		return c;
	c.d.assign(a.d.size()+1,0);
	i64 carry=0;
	for(size_t i=0;i<a.d.size();++i)
	{
		i64 cur=(i64)a.d[i]*x+carry;
		c.d[i]=cur%B;
		carry=cur/B;
	}
	c.d[a.d.size()]=carry;
	norm(c);
	return c;
}

void half(big&a)
{
	i64 rem=0;
	for(int i=(int)a.d.size()-1;i>=0;--i)
	{
		i64 cur=rem*B+a.d[i];
		a.d[i]=cur>>1;
		rem=cur&1;
	}
	norm(a);
}

void shl(big&a,int g)
{
	for(int i=0;i<g;++i)
	{
		i64 carry=0;
		for(size_t j=0;j<a.d.size();++j)
		{
			i64 cur=(i64)a.d[j]*2+carry;
			a.d[j]=cur%B;
			carry=cur/B;
		}
		if(carry)
			a.d.push_back(carry);
	}
}

bool even(const big&a)
{
	return a.d.empty()||(a.d[0]&1)==0;
}

big gcdb(big a,big b)                    // 二进制 gcd
{
	if(a.zero())
		return b;
	if(b.zero())
		return a;
	int g=0;
	while(even(a)&&even(b))
	{
		half(a);
		half(b);
		++g;
	}
	while(!a.zero())
	{
		while(even(a))
			half(a);
		while(even(b))
			half(b);
		if(cmp(a,b)>=0)
			a=sub(a,b);
		else
			b=sub(b,a);
	}
	shl(b,g);
	return b;
}

big divmod(const big&a,const big&b,big&r)   // 返回商，r 为余数
{
	big q;
	if(cmp(a,b)<0)
	{
		r=a;
		return q;
	}
	q.d.assign(a.d.size(),0);
	r=big();
	for(int i=(int)a.d.size()-1;i>=0;--i)
	{
		r.d.insert(r.d.begin(),a.d[i]);
		norm(r);
		int lo=0,hi=B-1,best=0;
		while(lo<=hi)
		{
			int mid=(lo+hi)>>1;
			if(cmp(mulSmall(b,mid),r)<=0)
			{
				best=mid;
				lo=mid+1;
			}
			else
				hi=mid-1;
		}
		q.d[i]=best;
		if(best)
			r=sub(r,mulSmall(b,best));
	}
	norm(q);
	return q;
}

string tostr(const big&a)
{
	if(a.zero())
		return "0";
	string s=to_string(a.d.back());
	char buf[16];
	for(int i=(int)a.d.size()-2;i>=0;--i)
	{
		snprintf(buf,sizeof(buf),"%09d",a.d[i]);
		s+=buf;
	}
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	big A(1);                           // A=(m-1)!
	for(int i=2;i<=m-1;++i)
		A=mulSmall(A,i);
	big Bb(1);                          // Bb=(n+1)(n+2)...(n+m-1)
	for(int k=n+1;k<=n+m-1;++k)
		Bb=mulSmall(Bb,k);
	big X=sub(Bb,A);                    // S=(Q-A)/((m-1)AQ)
	big Y=mulSmall(mul(A,Bb),m-1);
	big g=gcdb(X,Y);
	big r;
	big Xu=divmod(X,g,r);
	big Yu=divmod(Y,g,r);
	cout<<tostr(Xu)<<'\n'<<tostr(Yu)<<'\n';
	return 0;
}
