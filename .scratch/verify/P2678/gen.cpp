// P2678 轻量验证：随机小数据 + 枚举全部 2^N 种移走方案的暴力
// 用法：gen.exe <seed>  → 生成 test.in / exp.out
#include<bits/stdc++.h>
using namespace std;

int main(int argc,char**argv)
{
	mt19937 rn(argc>1?atoi(argv[1]):12345);
	int n=rn()%9;			// 0..8
	int L=n+1+rn()%25;		// 保证 (0,L) 里放得下 n 个互不相同的整数
	int m=rn()%(n+1);
	vector<int>pos;
	for(int x=1;x<L;++x)
		pos.push_back(x);
	shuffle(pos.begin(),pos.end(),rn);
	pos.resize(n);
	sort(pos.begin(),pos.end());

	FILE*fi=fopen("test.in","w");
	fprintf(fi,"%d %d %d\n",L,n,m);
	for(int x:pos)
		fprintf(fi,"%d\n",x);
	fclose(fi);

	// 暴力：枚举移走哪些岩石（不超过 m 块），算剩余序列的最小间距，取最大
	int best=0;
	for(int mask=0;mask<(1<<n);++mask)
	{
		if(__builtin_popcount((unsigned)mask)>m)
			continue;
		vector<int>keep{0};
		for(int i=0;i<n;++i)
			if(!(mask>>i&1))
				keep.push_back(pos[i]);
		keep.push_back(L);
		int mn=INT_MAX;
		for(int i=1;i<(int)keep.size();++i)
			mn=min(mn,keep[i]-keep[i-1]);
		best=max(best,mn);
	}
	FILE*fo=fopen("exp.out","w");
	fprintf(fo,"%d\n",best);
	fclose(fo);
	return 0;
}