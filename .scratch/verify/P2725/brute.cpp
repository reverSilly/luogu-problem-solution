// P2725 暴力：枚举每种邮票张数 c_i（Σc_i<=k），标记可达面值，找第一个断点
#include<bits/stdc++.h>
using namespace std;
int k,n,a[55];
bool reach[2000005];
int main()
{
	cin>>k>>n;
	int mx=0;
	for(int i=0;i<n;++i){cin>>a[i];mx=max(mx,a[i]);}
	// 递归枚举计数向量
	function<void(int,int,int)>dfs=[&](int idx,int used,int sum){
		if(idx==n){reach[sum]=true;return;}
		for(int c=0;c+used<=k;++c)
			dfs(idx+1,used+c,sum+c*a[idx]);
	};
	dfs(0,0,0);
	int ans=0;
	for(int v=1;v<=k*mx+1;++v)
		if(!reach[v]){ans=v-1;break;}
	cout<<ans<<'\n';
	return 0;
}