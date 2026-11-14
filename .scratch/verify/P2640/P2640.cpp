#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,k;
bool np[mxn];	// np[i]=true 表示 i 不是素数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	np[0]=np[1]=true;
	for(int i=2;i*i<=n;++i)
		if(!np[i])
			for(int j=i*i;j<=n;j+=i)
				np[j]=true;
	bool found=false;
	for(int p=2;p+k<=n;++p)
		if(!np[p]&&!np[p+k])
		{
			cout<<p<<' '<<p+k<<'\n';
			found=true;
		}
	if(!found)
		cout<<"empty\n";
	return 0;
}