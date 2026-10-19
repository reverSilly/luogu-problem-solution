// 题目：P1551 亲戚
// https://www.luogu.com.cn/problem/P1551
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{5005};          // n,m,p<=5000 (+5 松弛)

int n,m,p;
int f[mxn];                       // 并查集父指针

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>p;
	iota(f,f+n,0);                                // 每人自成一族
	for(int i=0,a,b;i<m;++i)
	{
		cin>>a>>b;--a;--b;
		int ra=getrt(a),rb=getrt(b);
		if(ra!=rb)
			f[ra]=rb;
	}
	for(int i=0,a,b;i<p;++i)
	{
		cin>>a>>b;--a;--b;
		cout<<(getrt(a)==getrt(b)?"Yes\n":"No\n");
	}
	return 0;
}
