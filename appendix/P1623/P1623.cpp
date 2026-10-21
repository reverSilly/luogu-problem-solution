//
// 题目：P1623 [CEOI 2007] 树的匹配 Treasury
// https://www.luogu.com.cn/problem/P1623
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int B{1000000000};

struct big
{
	vector<int>d;                       // 小端存储，base=1e9，空表示 0
	big(){}
	big(int x)
	{
		while(x)
		{
			d.push_back(x%B);
			x/=B;
		}
	}
};

void norm(big&a)
{
	while(!a.d.empty()&&a.d.back()==0)
		a.d.pop_back();
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

big mul(const big&a,const big&b)
{
	big c;
	if(a.d.empty()||b.d.empty())
		return c;
	c.d.assign(a.d.size()+b.d.size()+1,0);
	for(size_t i=0;i<a.d.size();++i)
	{
		if(a.d[i]==0)
			continue;
		long long carry=0;
		for(size_t j=0;j<b.d.size();++j)
		{
			long long cur=(long long)c.d[i+j]+(long long)a.d[i]*b.d[j]+carry;
			c.d[i+j]=cur%B;
			carry=cur/B;
		}
		size_t k=i+b.d.size();
		while(carry)
		{
			long long cur=(long long)c.d[k]+carry;
			c.d[k]=cur%B;
			carry=cur/B;
			++k;
		}
	}
	norm(c);
	return c;
}

string tostr(const big&a)
{
	if(a.d.empty())
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

int n,rt;
int f0[mxn],f1[mxn];                    // f0:u 不连儿子 f1:u 连一个儿子
big g0[mxn],g1[mxn];
bool hasfa[mxn];
vector<int>ch[mxn];

void dfs(int u)
{
	int k=ch[u].size();
	for(int v:ch[u])
		dfs(v);
	vector<int>bv(k);
	vector<big>bc(k);
	int base=0;
	for(int i=0;i<k;++i)
	{
		int v=ch[u][i];
		if(f0[v]>f1[v])
		{
			bv[i]=f0[v];
			bc[i]=g0[v];
		}
		else if(f1[v]>f0[v])
		{
			bv[i]=f1[v];
			bc[i]=g1[v];
		}
		else
		{
			bv[i]=f0[v];
			bc[i]=add(g0[v],g1[v]);
		}
		base+=bv[i];
	}
	f0[u]=base;
	big pg(1);
	for(int i=0;i<k;++i)
		pg=mul(pg,bc[i]);
	g0[u]=pg;
	if(k==0)
	{
		f1[u]=-1;
		g1[u]=big();
		return;
	}
	vector<big>pre(k+1),suf(k+1);
	pre[0]=big(1);
	for(int i=0;i<k;++i)
		pre[i+1]=mul(pre[i],bc[i]);
	suf[k]=big(1);
	for(int i=k-1;i>=0;--i)
		suf[i]=mul(suf[i+1],bc[i]);
	int mxf=-1;
	big tot;
	for(int i=0;i<k;++i)
	{
		int v=ch[u][i];
		int cand=base-bv[i]+f0[v]+1;
		big ways=mul(g0[v],mul(pre[i],suf[i+1]));
		if(cand>mxf)
		{
			mxf=cand;
			tot=ways;
		}
		else if(cand==mxf)
			tot=add(tot,ways);
	}
	f1[u]=mxf;
	g1[u]=tot;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		int u,m;
		cin>>u>>m;--u;
		for(int j=0,v;j<m;++j)
		{
			cin>>v;--v;
			ch[u].push_back(v);
			hasfa[v]=true;
		}
	}
	rt=0;
	while(hasfa[rt])
		++rt;
	dfs(rt);

	int mxf;
	big ans;
	if(f0[rt]>f1[rt])
	{
		mxf=f0[rt];
		ans=g0[rt];
	}
	else if(f1[rt]>f0[rt])
	{
		mxf=f1[rt];
		ans=g1[rt];
	}
	else
	{
		mxf=f0[rt];
		ans=add(g0[rt],g1[rt]);
	}
	cout<<mxf<<'\n'<<tostr(ans)<<'\n';
	return 0;
}
// sample: 1 / 1 0  ->  0 / 1
