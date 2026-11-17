// 题目：P2843 暗杀
// 段内 K 种特性次数全相等 ⇔ 两端点的差向量 v(i)=(c_1-c_K,...,c_{K-1}-c_K) 相等
// 把 N+1 个向量排序分组，组内最远下标差即答案；精确比较，零哈希碰撞
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005},mxk{35};

int n,k,a[mxn],d[mxn][mxk],id[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i)
		cin>>a[i];
	// d[0] 全 0：空前缀是合法左端点，全局默认恰好正确
	for(int i=1;i<=n;++i)
		for(int b=0;b<k;++b)
			d[i][b]=d[i-1][b]+((a[i-1]>>b)&1);
	for(int b=0;b<k-1;++b)
		for(int i=0;i<=n;++i)
			d[i][b]-=d[i][k-1];	// 第 k-1 列保持 c_K，减法互不干扰
	for(int i=0;i<=n;++i)
		id[i]=i;
	sort(id,id+n+1,[](int x,int y){
		return lexicographical_compare(d[x],d[x]+k-1,d[y],d[y]+k-1);
	});
	int ans=0;
	for(int i=0,j;i<=n;i=j)	// 极大相等段分组；k=1 时 0 维向量全等价，自动得 n
	{
		int mn=id[i],mx=id[i];
		for(j=i+1;j<=n&&equal(d[id[i]],d[id[i]]+k-1,d[id[j]]);++j)
		{
			mn=min(mn,id[j]);
			mx=max(mx,id[j]);
		}
		ans=max(ans,mx-mn);
	}
	cout<<ans<<'\n';
	return 0;
}
