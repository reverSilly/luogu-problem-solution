#include<bits/stdc++.h>
using namespace std;

int w[6]={1,2,3,5,10,20};
int a[6];
bool can[5005];

void dfs(int i,int s)
{
	if(i==6)
	{
		can[s]=true;
		return;
	}
	for(int c=0;c<=a[i];++c)
		dfs(i+1,s+c*w[i]);
}

int main()
{
	for(int i=0;i<6;++i)
		cin>>a[i];
	dfs(0,0);
	int ans=0;
	for(int s=1;s<=5000;++s)
		if(can[s])
			++ans;
	cout<<"Total="<<ans<<'\n';
	return 0;
}
