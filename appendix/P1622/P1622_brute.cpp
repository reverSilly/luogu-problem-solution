//
// P1622 朴素解：枚举所有释放顺序，逐次模拟发火人数
// 用法：仅用于与 P1622.cpp 对拍，非提交代码（Q <= 7）
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int P,Q;
	cin>>P>>Q;
	vector<int>r(Q);
	for(int i=0;i<Q;++i)
		cin>>r[i];
	sort(r.begin(),r.end());
	int best=INT_MAX;
	do
	{
		vector<char>out(P+2,0);
		int cost=0;
		for(int k:r)
		{
			int l=k,rr=k;
			while(l-1>=1&&!out[l-1])
				--l;
			while(rr+1<=P&&!out[rr+1])
				++rr;
			cost+=(rr-l+1)-1;      // 同块内除自己以外的人都要吃肉
			out[k]=1;
		}
		best=min(best,cost);
	}while(next_permutation(r.begin(),r.end()));
	cout<<(Q?best:0)<<'\n';
	return 0;
}
