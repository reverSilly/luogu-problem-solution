// 题目：P3021 [USACO11MAR] Bovine Bridge Battle S
// 合法四牛组 <=> 两对点共享中点；按坐标和（2 倍中点）排序分组，每组 k 对贡献 C(k,2)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};

int n,x[mxn],y[mxn];
struct node{int sx,sy;}p[mxn*mxn/2];	// sx,sy ∈ [-2e9,2e9]，int 装得下
i64 ans;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	int cnt=0;
	for(int i=0;i<n;++i)
		for(int j=i+1;j<n;++j)
			p[cnt++]={x[i]+x[j],y[i]+y[j]};
	sort(p,p+cnt,[](node a,node b){return a.sx!=b.sx?a.sx<b.sx:a.sy<b.sy;});
	for(int i=0,j;i<cnt;i=j)
	{
		j=i;
		while(j<cnt&&p[j].sx==p[i].sx&&p[j].sy==p[i].sy)
			++j;
		i64 k=j-i;
		ans+=k*(k-1)/2;		// 同中点的两对拼成一组，且不重不漏
	}
	cout<<ans<<'\n';
	return 0;
}