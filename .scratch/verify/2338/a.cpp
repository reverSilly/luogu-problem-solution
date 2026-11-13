#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n,nt,nd;
int T[mxn],D[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0,val;i<n;++i)
	{
		char op;
		cin>>op>>val;
		if(op=='T')
			T[nt++]=val;
		else
			D[nd++]=val;
	}
	sort(T,T+nt);
	sort(D,D+nd);
	long double t=0,x=0;
	int cnt=0,i=0,j=0;
	while(x<1000)
	{
		// 下一个事件：要么是到下一个 D 点，要么是到终点
		long double arr=j<nd?t+(D[j]-x)*(cnt+1):t+(1000-x)*(cnt+1);
		if(i<nt&&(long double)T[i]<=arr)
		{
			x+=(T[i]-t)/(cnt+1);
			t=T[i];
			++i;++cnt;
		}
		else if(j<nd)
		{
			t=arr;
			x=D[j];
			++j;++cnt;
		}
		else
		{
			t=arr;
			x=1000;
		}
	}
	cout<<(i64)(t+0.5L)<<'\n';
	return 0;
}
