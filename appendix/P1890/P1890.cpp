//
// 题目：P1890 gcd 区间
// https://www.luogu.com.cn/problem/P1890
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int mxlg{12};

int n,m;
int a[mxn];
int st[mxlg][mxn];
int lg2[mxn];

int gg(int x,int y)
{
	while(y){
		int t=x%y;
		x=y;
		y=t;
	}
	return x;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=1;i<=n;++i){
		cin>>a[i];
		st[0][i]=a[i];
	}
	lg2[1]=0;
	for(int i=2;i<=n;++i){
		lg2[i]=lg2[i>>1]+1;
	}
	for(int j=1;j<=lg2[n];++j){
		int span=1<<(j-1);
		int lim=n-(1<<j)+1;
		for(int i=1;i<=lim;++i){
			st[j][i]=gg(st[j-1][i],st[j-1][i+span]);
		}
	}
	while(m--){
		int l,r;
		cin>>l>>r;
		int len=r-l+1;
		int j=lg2[len];
		int ans=gg(st[j][l],st[j][r-(1<<j)+1]);
		cout<<ans<<'\n';
	}
	return 0;
}
