#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 独立基准：用前缀和直接展开 mood_i = (sum_{k<=i}(a_k+b_k)) - 8*i
// 与标程的"滚动累加"是不同的代码路径，但数学等价。
int main()
{
	int n;
	cin>>n;
	vector<i64> pre(n+1,0);
	for(int i=1,a,b;i<=n;++i)
	{
		cin>>a>>b;
		pre[i]=pre[i-1]+a+b;
	}
	i64 ans=0;
	for(int i=1;i<=n;++i)
		ans+=pre[i]-8LL*i;
	cout<<ans<<'\n';
	return 0;
}
