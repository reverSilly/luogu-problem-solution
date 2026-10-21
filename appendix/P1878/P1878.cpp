//
// 题目：P1878 舞蹈课
// https://www.luogu.com.cn/problem/P1878
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{200005};

int a[mxn],L[mxn],R[mxn],n;
char g[mxn];
set<pair<int,int>> st;
pair<int,int> outp[mxn];
int kout;

void del_edge(int i)
{
	int j=R[i];
	if(i<1||i>n||j<1||j>n)
		return;
	if(g[i]==g[j])
		return;
	int d=a[i]-a[j];
	if(d<0)
		d=-d;
	st.erase({d,i});
}

void add_edge(int i)
{
	int j=R[i];
	if(i<1||i>n||j<1||j>n)
		return;
	if(g[i]==g[j])
		return;
	int d=a[i]-a[j];
	if(d<0)
		d=-d;
	st.insert({d,i});
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	string s;
	cin>>s;
	for(int i=1;i<=n;++i)
	{
		g[i]=s[i-1];
		cin>>a[i];
		L[i]=i-1;
		R[i]=i+1;
	}
	R[0]=1;
	L[n+1]=n;
	for(int i=1;i<n;++i)
		add_edge(i);
	while(!st.empty())
	{
		int i=st.begin()->second;
		st.erase(st.begin());
		int j=R[i];
		int li=L[i],rj=R[j];
		del_edge(li);
		del_edge(i);
		del_edge(j);
		R[li]=rj;
		L[rj]=li;
		add_edge(li);
		int x=i,y=j;
		if(x>y)
			swap(x,y);
		outp[kout++]={x,y};
	}
	cout<<kout<<'\n';
	for(int i=0;i<kout;++i)
		cout<<outp[i].first<<' '<<outp[i].second<<'\n';
	return 0;
}
