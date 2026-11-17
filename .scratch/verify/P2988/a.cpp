// 题目：P2988 [USACO10MAR] Test Taking S
// 答 x 个 true 时最坏得分 = |N-x-t|；令 y=N-x，即数轴 [0,N] 上离点集 {t_i} 最远的整点
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,k,t[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	if(k==0)	// 没有任何消息：t 任意，对手取 t=y 使答案为 0
	{
		cout<<0<<'\n';
		return 0;
	}
	for(int i=0;i<k;++i)
		cin>>t[i];
	sort(t,t+k);
	int ans=max(t[0],n-t[k-1]);	// 两端候选 y=0 与 y=N
	for(int i=1;i<k;++i)
		ans=max(ans,(t[i]-t[i-1])/2);	// 相邻间隙中点，下取整
	cout<<ans<<'\n';
	return 0;
}