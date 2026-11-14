// 题目：P2674 《瞿葩的数字游戏》T2-多边形数
// 通项 P(K,n)=n+(K-2)*n*(n-1)/2，对 K 线性 ⇒ 反解 K=2+(2N-2n)/(n(n-1))
// 隐藏规律 P(K,2)=K ⇒ 每个 N>=3 都是 N 边形数；N=1 是所有 K；Poor 仅 N=2
// P(K,n)>=T_n ⇒ n<=sqrt(2N)，枚举 n 即 O(sqrt N)/组
// 输出按题面"中间无空格"：K1K2 拼接 / Poor+N 拼接 / 单个 K 只输出一个
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxk{1420};	// n_max<=1413 (N<=1e6)，合法 K 个数不超过 n_max

int ng,cnt;
i64 kk[mxk];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>ng;
	while(ng--)
	{
		i64 N;
		cin>>N;
		if(N==1)	// P(K,1)=1 对所有 K>=3，最小两个是 3,4
		{
			cout<<"34\n";
			continue;
		}
		cnt=0;
		for(i64 n=2;n*(n+1)/2<=N;++n)	// P(K,n)>=T_n ⇒ n 受三角形数定界
		{
			i64 num=2*N-2*n,den=n*(n-1);
			if(num%den==0)
			{
				i64 K=2+num/den;
				if(K>=3)	// 范围内自动 >=3，防御性保留
					kk[cnt++]=K;
			}
		}
		if(cnt==0)
			cout<<"Poor"<<N<<'\n';	// 仅 N=2 会走到这里
		else
		{
			sort(kk,kk+cnt);
			cout<<kk[0];
			if(cnt>=2)
				cout<<kk[1];
			cout<<'\n';
		}
	}
	return 0;
}
