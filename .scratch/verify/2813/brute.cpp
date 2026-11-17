// 独立暴力：按定义递归枚举所有"每个防御配一个不同攻击"的全破匹配，取 tot-used 最大；无则 0
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int m,n;
i64 def[10],atk[10],tot,best;
bool used[10];
void dfs(int j,i64 cost)
{
	if(j==m){best=max(best,tot-cost);return;}
	for(int i=0;i<n;++i)
		if(!used[i]&&atk[i]>def[j])
		{
			used[i]=true;
			dfs(j+1,cost+atk[i]);
			used[i]=false;
		}
}
int main()
{
	cin>>m>>n;
	for(int i=0;i<m;++i)cin>>def[i];
	for(int i=0;i<n;++i){cin>>atk[i];tot+=atk[i];}
	best=0;
	dfs(0,0);
	cout<<best<<'\n';
	return 0;
}