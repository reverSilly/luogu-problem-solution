#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};           // 单座城堡积木数 <=100 (+5 松弛)
constexpr int mxh{10005};         // 高度上界 100*100 (+5 松弛)
constexpr int inf{1145141919};    // 114514 家族哨兵

int n;
int a[mxn];                       // a[j]=当前城堡自下而上第 j 块的棱长(读入即 0-based)
bool f[mxh];                      // f[h]=当前城堡能否抽出高度 h
bool ok[mxh];                     // ok[h]=此前所有城堡都能抽出 h(可达性交集)

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	int lim{inf};                 // 答案上界:各城堡原高度的最小值
	fill_n(&ok[0],mxh,true);      // 交集初值为全集;bool 全局默认 false,必须显式填充
	for(int i=0,x,m,sum;i<n;++i)
	{
		m=0;sum=0;
		while(cin>>x&&x!=-1){a[m++]=x;sum+=x;}
		lim=min(lim,sum);         // 共同高度不可能超过任何一座城堡的原高
		fill_n(&f[0],mxh,false);  // 换城堡必须重置,否则残留上一座的可达状态
		f[0]=true;                // 一块不留/全删,高度 0 必可达
		for(int j=0;j<m;++j)
			for(int h=sum;h>=a[j];--h)
				f[h]=(f[h]||f[h-a[j]]);   // 倒序:保证第 j 块至多用一次
		for(int h=0;h<=sum;++h)
			ok[h]=(ok[h]&&f[h]);
	}
	for(int h=lim;h>=0;--h)
		if(ok[h])
		{
			cout<<h<<'\n';
			return 0;
		}
	cout<<0<<'\n';
	return 0;
}
