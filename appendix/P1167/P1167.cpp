#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{5005};
int n,a[mxn];
int mon[13]{0,31,28,31,30,31,30,31,31,30,31,30,31};

int leap(int y)
{
	return(y%4==0&&y%100!=0)||y%400==0;
}

i64 toMin(int y,int m,int d,int h,int mi)
{
	i64 days=365LL*y+y/4-y/100+y/400+1;
	for(int i=1;i<m;++i)
	{
		days+=mon[i];
		if(i==2&&leap(y))
			++days;
	}
	days+=d-1;
	return days*1440+h*60+mi;
}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int y,m,d,h,mi;
	char c;
	cin>>y>>c>>m>>c>>d>>c>>h>>c>>mi;
	i64 s=toMin(y,m,d,h,mi);
	cin>>y>>c>>m>>c>>d>>c>>h>>c>>mi;
	i64 e=toMin(y,m,d,h,mi);
	i64 t=e-s;
	if(t<0)
		t=0;
	sort(a,a+n);
	int ans=0;
	for(int i=0;i<n;++i)
	{
		if(t<a[i])
			break;
		t-=a[i];
		++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
