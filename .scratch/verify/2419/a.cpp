// 题目：P2419 [USACO08JAN] Cow Contest S
// 思路：Floyd 求「强于」关系的传递闭包 f[i][j]。
//       牛 i 的名次确定 ⇔ 它与其余每头牛都可比（f[i][j]||f[j][i]）。
//       证明：名次被夹在 [|D_i|+1, N-|U_i|]，且两端都能构造出相容排名达到，
//             故区间退化为一点 ⇔ |D_i|+|U_i|=N-1。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,m,ans;
bool f[mxn][mxn];	// f[i][j]=1 表示 i 强于 j（直接或间接）

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		f[u][v]=1;			// 胜者在前
	}
	for(int k=0;k<n;++k)		// Floyd 传递闭包
		for(int i=0;i<n;++i)
			if(f[i][k])
				for(int j=0;j<n;++j)
					if(f[k][j])
						f[i][j]=1;
	for(int i=0;i<n;++i)
	{
		int cnt=0;
		for(int j=0;j<n;++j)
			if(j!=i&&(f[i][j]||f[j][i]))
				++cnt;
		if(cnt==n-1)			// 与其余每头牛都可比 ⇒ 排名唯一
			++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
