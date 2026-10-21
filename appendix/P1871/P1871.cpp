//
// 题目：P1871 对撞机
// https://www.luogu.com.cn/problem/P1871
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int lp[mxn],pr[mxn],pc;
int own[mxn];
bool on[mxn];

void sieve(int n)
{
	for(int i=2;i<=n;++i)
	{
		if(!lp[i])
		{
			lp[i]=i;
			pr[pc++]=i;
		}
		for(int j=0;j<pc&&pr[j]<=lp[i]&&i<=n/pr[j];++j)
			lp[i*pr[j]]=pr[j];
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m;
	cin>>n>>m;
	sieve(n);
	while(m--)
	{
		char op;
		int x;
		cin>>op>>x;
		if(op=='+')
		{
			if(on[x])
			{
				cout<<"Already on\n";
				continue;
			}
			int bad=0,y=x;
			while(y>1)
			{
				int p=lp[y];
				if(own[p])
				{
					bad=own[p];
					break;
				}
				while(y%p==0)
					y/=p;
			}
			if(bad)
			{
				cout<<"Conflict with "<<bad<<'\n';
				continue;
			}
			y=x;
			while(y>1)
			{
				int p=lp[y];
				own[p]=x;
				while(y%p==0)
					y/=p;
			}
			on[x]=true;
			cout<<"Success\n";
		}
		else
		{
			if(!on[x])
			{
				cout<<"Already off\n";
				continue;
			}
			int y=x;
			while(y>1)
			{
				int p=lp[y];
				own[p]=0;
				while(y%p==0)
					y/=p;
			}
			on[x]=false;
			cout<<"Success\n";
		}
	}
	return 0;
}
