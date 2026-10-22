#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};

int n,m;
//a[]：还没被吃掉的果子的编号，自下而上排列
int a[mxn];
//sz：剩余果子个数；p：松鼠当前所在位置（a 中的下标，0-based）
int sz,p;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		a[i]=i+1;
	sz=n;
	p=0;
	int ans=0;
	for(int i=1;i<=m;++i)
	{
		int k=i*i*i%5+1;
		int q=p+k;
		if(q>=sz)
		{
			//跳出最前面：回到最下面的果子，重做第 i 跳
			p=0;
			q=k;
		}
		ans=a[q];
		for(int j=q;j+1<sz;++j)
			a[j]=a[j+1];
		--sz;
		p=q;
	}
	cout<<ans<<'\n';
	return 0;
}
