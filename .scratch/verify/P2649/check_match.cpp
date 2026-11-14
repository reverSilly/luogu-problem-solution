#include<bits/stdc++.h>
using namespace std;
// 在进程内对拍：贪心 vs Kuhn 最大二分匹配（独立验证贪心最优性）
int m,n,a[60],b[1200],vis[1200];
int greedy()
{
	int cnt=0;
	for(int i=1;i<=n*m;++i)if(!vis[i])b[cnt++]=i;
	sort(a,a+n);sort(b,b+cnt);
	int j=0,beat=0;
	for(int i=0;i<n;++i){while(j<cnt&&b[j]<=a[i])++j;if(j<cnt){++beat;++j;}}
	return n-beat;
}
// Kuhn: 左部 John 的牌, 右部补集牌, 边 b>a
vector<int>g[60];
int mr[1200];bool used[1200];
bool aug(int u){for(int v:g[u]){if(used[v])continue;used[v]=true;if(mr[v]<0||aug(mr[v])){mr[v]=u;return true;}}return false;}
int kuhn()
{
	int cnt=0;for(int i=1;i<=n*m;++i)if(!vis[i])b[cnt++]=i;
	for(int i=0;i<n;++i){g[i].clear();for(int j=0;j<cnt;++j)if(b[j]>a[i])g[i].push_back(j);}
	fill(mr,mr+cnt,-1);
	int match=0;
	for(int i=0;i<n;++i){fill(used,used+cnt,false);if(aug(i))++match;}
	return n-match;
}
int main()
{
	mt19937 rn(12345);
	int mismatch=0;
	for(int t=0;t<20000;++t)
	{
		m=2+rn()%5;n=1+rn()%8;
		fill(vis,vis+n*m+1,0);
		// 随机选 n 张不重复的牌给 John
		vector<int>pool(n*m);iota(pool.begin(),pool.end(),1);
		shuffle(pool.begin(),pool.end(),rn);
		for(int i=0;i<n;++i){a[i]=pool[i];vis[a[i]]=1;}
		int g=greedy(),k=kuhn();
		if(g!=k){++mismatch;cout<<"MISMATCH m="<<m<<" n="<<n<<" greedy="<<g<<" kuhn="<<k<<" John:";for(int i=0;i<n;++i)cout<<' '<<a[i];cout<<'\n';if(mismatch>5)break;}
	}
	cout<<"done 20000 cases, mismatch="<<mismatch<<'\n';
	return 0;
}