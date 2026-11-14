// 题目：P2729 [USACO3.2] 饲料调配 Feed Ratios
// 思路：枚举三种饲料份数 m,n,p ∈ [0,99]，算出混合后大麦/燕麦/小麦总量 A,B,C。
//       (A,B,C) 与目标 (x,y,z) 成整数倍 ⟺ 令 T=x+y+z, S=A+B+C，T|S 且 k=S/T 满足 A=kx,B=ky,C=kz。
//       取 m+n+p 最小者输出 m n p k；无解输出 NONE。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxb{100},inf{1145141919};	// 份数枚举 0..99

int x,y,z,a[3],b[3],c[3];
int ans{inf},bm,bn,bp,bk;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>x>>y>>z;
	for(int i=0;i<3;++i)
		cin>>a[i]>>b[i]>>c[i];
	int T=x+y+z;	// 题面保证目标行不全 0 ⇒ T>=1
	for(int m=0;m<mxb;++m)
		for(int n=0;n<mxb;++n)
			for(int p=0;p<mxb;++p)
			{
				int A=m*a[0]+n*a[1]+p*a[2];
				int B=m*b[0]+n*b[1]+p*b[2];
				int C=m*c[0]+n*c[1]+p*c[2];
				int S=A+B+C;
				if(S==0||S%T!=0)	// S=0 即 m=n=p=0，不产出目标饲料
					continue;
				int k=S/T;
				if(A==k*x&&B==k*y&&C==k*z&&m+n+p<ans)
				{
					ans=m+n+p;
					bm=m;bn=n;bp=p;bk=k;
				}
			}
	if(ans==inf)
		cout<<"NONE\n";
	else
		cout<<bm<<' '<<bn<<' '<<bp<<' '<<bk<<'\n';
	return 0;
}