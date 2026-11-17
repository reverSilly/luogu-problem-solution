// 题目：P3052 [USACO12MAR] Cows in a Skyscraper G
// 状压 DP：dp[S]=(最少趟数, 该趟数下当前电梯最小载重)，字典序取 min。
// 正确性支柱：单步转移 T_c 对字典序单调 ⇒ 同次数下载重小者永不劣。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20},mxs{(1<<18)+5};
constexpr int inf{1145141919};

int n,W,c[mxn],f[mxs],g[mxs];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>W;
	for(int i=0;i<n;++i)
		cin>>c[i];
	int full=(1<<n)-1;
	fill_n(f,full+1,inf);	// 全局默认 0 不是 INF，必须显式填
	fill_n(g,full+1,inf);
	f[0]=1;g[0]=0;	// 空集：第一趟电梯已开着，载重 0
	for(int S=1;S<=full;++S)
		for(int i=0;i<n;++i)
			if(S>>i&1)
			{
				int p=S^(1<<i),nf,ng;
				if(g[p]+c[i]<=W)
					nf=f[p],ng=g[p]+c[i];	// 挤进当前趟
				else
					nf=f[p]+1,ng=c[i];	// 新开一趟
				if(nf<f[S]||(nf==f[S]&&ng<g[S]))
					f[S]=nf,g[S]=ng;
			}
	cout<<f[full]<<'\n';
	return 0;
}
