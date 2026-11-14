// 题目：P2637 Going Once, Going Twice, Gone! B
// 思路：收入 rev(x)=x*min(n,#{p_i>=x})。相邻出价间销量恒定 ⇒ rev 关于 x 递增
//       ⇒ 最优售价必在某个 p_i。排序后枚举 p_i，lower_bound 算销量，
//       升序 + 严格更大才更新 ⇒ 平局自动取最低售价。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxm{1005};

int n,m,p[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
		cin>>p[i];
	sort(p,p+m);
	i64 best=0;int bp=0;
	for(int i=0;i<m;++i)
	{
		int price=p[i];
		int cnt=m-(lower_bound(p,p+m,price)-p);	// #{p_j>=price}
		int buy=min(n,cnt);
		i64 rev=(i64)price*buy;
		if(rev>best||(rev==best&&price<bp))	// 升序枚举，平局取更低售价
			best=rev,bp=price;
	}
	cout<<bp<<' '<<best<<'\n';
	return 0;
}
