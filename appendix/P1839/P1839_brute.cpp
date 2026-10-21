//
// 题目：P1839 Play with Power 暴力（局面迭代吸引子）
// https://www.luogu.com.cn/problem/P1839
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

struct stt
{
	int a,b;
	int to[2];
	int tc;
	int o;
	int frozen;
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	int t;
	cin>>n>>t;
	auto legal=[&](i64 a,int b)->bool
	{
		if(a<1||b<1)
			return false;
		if(b==1)
			return a<=n;
		if(a==1)
			return true;
		i64 r=1;
		for(int i=0;i<b;++i)
		{
			if(r>n/a)
				return false;
			r*=a;
		}
		return true;
	};
	int maxb=0;
	i64 p2=1;
	while(p2<=n/2)
	{
		p2*=2;
		++maxb;
	}
	vector<pair<int,int>> keys;
	for(int a=2;a<=n;++a)
	{
		for(int b=1;b<=36;++b)
		{
			if(!legal(a,b))
				break;
			keys.push_back({a,b});
		}
	}
	for(int b=1;b<=maxb+1;++b)
		keys.push_back({1,b});
	auto kid=[&](int a,int b)->i64
	{
		return (i64)a*64+b;
	};
	unordered_map<i64,int> id;
	id.reserve(keys.size()*2+8);
	vector<stt> nodes(keys.size());
	for(int i=0;i<(int)keys.size();++i)
	{
		nodes[i].a=keys[i].first;
		nodes[i].b=keys[i].second;
		nodes[i].tc=0;
		nodes[i].o=-1;
		nodes[i].frozen=0;
		nodes[i].to[0]=nodes[i].to[1]=-1;
		id[kid(nodes[i].a,nodes[i].b)]=i;
	}
	for(int i=0;i<(int)nodes.size();++i)
	{
		int a=nodes[i].a,b=nodes[i].b;
		if(a==1&&b==maxb+1)
		{
			nodes[i].o=1;
			nodes[i].frozen=1;
			continue;
		}
		auto add=[&](int na,int nb)
		{
			if(na==1&&nb>maxb+1)
				return;
			if(!legal(na,nb))
				return;
			auto it=id.find(kid(na,nb));
			if(it==id.end())
				return;
			nodes[i].to[nodes[i].tc++]=it->second;
		};
		add(a+1,b);
		add(a,b+1);
		if(nodes[i].tc==0)
		{
			nodes[i].o=0;
			nodes[i].frozen=1;
		}
	}
	bool chg=true;
	while(chg)
	{
		chg=false;
		for(int i=0;i<(int)nodes.size();++i)
		{
			if(nodes[i].frozen)
				continue;
			bool allw=true,hasl=false,hasu=false;
			for(int k=0;k<nodes[i].tc;++k)
			{
				int o=nodes[nodes[i].to[k]].o;
				if(o==-1)
				{
					allw=false;
					hasu=true;
				}
				else if(o==0)
				{
					hasl=true;
					allw=false;
				}
				else if(o==1)
					allw=false;
			}
			if(hasl)
			{
				if(nodes[i].o!=2)
				{
					nodes[i].o=2;
					chg=true;
				}
			}
			else if(!hasu&&allw)
			{
				if(nodes[i].o!=0)
				{
					nodes[i].o=0;
					chg=true;
				}
			}
		}
	}
	while(t--)
	{
		int a,b;
		cin>>a>>b;
		int o;
		if(a==1&&b>maxb)
			o=1;
		else
		{
			auto it=id.find(kid(a,b));
			o=it==id.end()?0:nodes[it->second].o;
			if(o==-1)
				o=1;
		}
		if(o==2)
			cout<<"Masha\n";
		else if(o==0)
			cout<<"Stas\n";
		else
			cout<<"Missing\n";
	}
	return 0;
}
