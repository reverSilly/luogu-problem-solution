#include<bits/stdc++.h>
using namespace std;
using i64=long long;

// 小数据朴素基准:枚举女生的全排列(n<=8)
int P[25][25],Q[25][25];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>P[i][j];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>Q[i][j];
	vector<int> p(n);
	iota(p.begin(),p.end(),0);
	i64 best=-1145141919810LL;
	do
	{
		i64 s=0;
		for(int i=0;i<n;++i)
			s+=(i64)P[i][p[i]]*Q[p[i]][i];   // 男生 i 配女生 p[i]
		if(s>best)
			best=s;
	}while(next_permutation(p.begin(),p.end()));
	cout<<best<<'\n';
	return 0;
}
