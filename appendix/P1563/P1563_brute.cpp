#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n,m;
int fcg[mxn];
string nm[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>fcg[i]>>nm[i];
	int pos=0;
	for(int i=0;i<m;++i)
	{
		int a,s;cin>>a>>s;
		int dir;                  // +1 逆时针(下标+), -1 顺时针(下标-)
		if(fcg[pos]==0)           // 面朝圈内: 左=顺时针, 右=逆时针
			dir=(a==0)?-1:1;
		else                      // 面朝圈外: 左=逆时针, 右=顺时针
			dir=(a==0)?1:-1;
		for(int k=0;k<s;++k)      // 一步一步走, 便于捕捉取模/方向的偏差
			pos=(pos+dir+n)%n;
	}
	cout<<nm[pos]<<'\n';
	return 0;
}
