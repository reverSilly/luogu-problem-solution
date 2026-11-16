// 题目：P2781 传教
// n 高达 1e9 但 m 只有 1e3 ⇒ 根本不用维护人群数组。
// 每个询问的答案 = 对它之前的每次传教操作，累加 k × 两区间交的长度。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxm{1005};

int n,m,cnt;
struct{int l,r,k;}op[mxm];	// 只存传教操作，0-based 闭区间

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,t,l,r,k;i<m;++i)
	{
		cin>>t;
		if(t==1)
		{
			cin>>l>>r>>k;--l;--r;
			op[cnt++]={l,r,k};
		}
		else
		{
			cin>>l>>r;--l;--r;
			i64 ans=0;
			for(int j=0;j<cnt;++j)
			{
				int lo=max(l,op[j].l),hi=min(r,op[j].r);
				if(lo<=hi)
					ans+=(i64)op[j].k*(hi-lo+1);	// k*长度可达 1e10，必须 i64
			}
			cout<<ans<<'\n';
		}
	}
	return 0;
}