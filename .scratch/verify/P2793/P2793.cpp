// 题目：P2793 Facer的工厂
// 贪心：每秒开头把能塞的前缀钢管全塞进去（等待长度<=h），然后机器加工 k。
// 逐秒模拟会到 1e14 秒，改成事件式快进：塞不进下一根时，
// 直接跳过 d=ceil((w-(h-a[i]))/k) 秒到"恰好能塞入"的那一秒；全部塞完则快进 ceil(w/k) 秒收尾。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n;
i64 h,k,a[mxn],w,t;

i64 ceildiv(i64 x,i64 y){return (x+y-1)/y;}	// 要求 x>=0,y>0

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>h>>k;
	for(int i=0;i<n;++i)
		cin>>a[i];
	int i=0;
	while(i<n||w>0)
	{
		while(i<n&&w+a[i]<=h)	// 本秒开头：按顺序塞到塞不下为止
			w+=a[i++];
		if(i==n&&w==0)
			break;
		i64 d=i<n?ceildiv(w-(h-a[i]),k):ceildiv(w,k);
		// i<n 时必有 w>h-a[i]>=0，故 d>=1；h-a[i]>=0 保证 d<=ceil(w/k)，w 不会减成负数以下
		t+=d;
		w=max(0ll,w-d*k);
	}
	cout<<t<<'\n';
	return 0;
}