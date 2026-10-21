//
// P1595 朴素解：枚举 n! 个排列, 计数无不动点的排列
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	vector<int>p(n);
	for(int i=0;i<n;++i)
		p[i]=i;
	i64 cnt=0;
	do
	{
		bool ok=true;
		for(int i=0;i<n;++i)
			if(p[i]==i)
			{
				ok=false;
				break;
			}
		if(ok)
			++cnt;
	}while(next_permutation(p.begin(),p.end()));
	cout<<cnt<<'\n';
	return 0;
}
