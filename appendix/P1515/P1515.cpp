// 题目：P1515 旅行
// https://www.luogu.com.cn/problem/P1515
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{45};        // 固定 14 家 + 新增 <=20 (+5 松弛)

int a[mxn];                   // 所有旅馆位置(去重后升序)
i64 f[mxn];                   // f[i]=从起点走到第 i 家旅馆的方案数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int A,B,N;
	cin>>A>>B>>N;
	int base[14]={0,990,1010,1970,2030,2940,3060,3930,4060,4970,5030,5990,6010,7000};
	int m=14;
	for(int i=0;i<14;++i)
		a[i]=base[i];
	for(int i=0,x;i<N;++i)
	{
		cin>>x;
		a[m++]=x;
	}
	sort(a,a+m);
	m=unique(a,a+m)-a;        // 题面保证无重复;去重仅为稳妥
	f[0]=1;                   // 起点(0 km)只有"尚未出发"一种
	for(int i=1;i<m;++i)
		for(int j=0;j<i;++j)
			if(a[i]-a[j]>=A&&a[i]-a[j]<=B)
				f[i]+=f[j];   // 当天恰好从旅馆 j 开到旅馆 i
	cout<<f[m-1]<<'\n';       // a[m-1]=7000,即终点
	return 0;
}
