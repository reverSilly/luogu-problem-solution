//
// 题目：P1883 【模板】三分 / 函数 / Error Curves
// https://www.luogu.com.cn/problem/P1883
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using f80=long double;

constexpr int mxn{10005};

int n,a[mxn],b[mxn],c[mxn];

f80 fmax(f80 x)
{
	f80 mx=-1e100L;
	for(int i=0;i<n;++i)
	{
		f80 y=(f80)a[i]*x*x+(f80)b[i]*x+c[i];
		if(y>mx)
			mx=y;
	}
	return mx;
}

void push_x(vector<f80>& cand,f80 x)
{
	if(x<0)
		x=0;
	if(x>1000)
		x=1000;
	cand.push_back(x);
}

void roots(vector<f80>& cand,f80 A,f80 B,f80 C)
{
	if(fabsl(A)<1e-18L)
	{
		if(fabsl(B)<1e-18L)
			return;
		push_x(cand,-C/B);
		return;
	}
	f80 disc=B*B-4*A*C;
	if(disc<-1e-8L)
		return;
	if(disc<0)
		disc=0;
	f80 sd=sqrtl(disc);
	push_x(cand,(-B-sd)/(2*A));
	push_x(cand,(-B+sd)/(2*A));
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(4);
	int T;
	cin>>T;
	while(T--)
	{
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>a[i]>>b[i]>>c[i];
		vector<f80> cand;
		cand.push_back(0);
		cand.push_back(1000);
		for(int i=0;i<n;++i)
		{
			if(a[i]>0)
			{
				f80 vx=-(f80)b[i]/(2*(f80)a[i]);
				push_x(cand,vx);
			}
		}
		for(int i=0;i<n;++i)
		{
			for(int j=i+1;j<n;++j)
			{
				f80 A=(f80)a[i]-a[j];
				f80 B=(f80)b[i]-b[j];
				f80 C=(f80)c[i]-c[j];
				roots(cand,A,B,C);
			}
		}
		f80 ans=fmax(0);
		for(f80 x:cand)
			ans=min(ans,fmax(x));
		if(ans>-1e-12L&&ans<1e-12L)
			ans=0;
		cout<<(double)ans<<'\n';
	}
	return 0;
}
