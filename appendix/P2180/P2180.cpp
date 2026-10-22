// 题目：P2180 摆石子
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{30005};

i64 n,m,k;

i64 c2(i64 x){return x*(x-1)/2;}

// R：可用的水平线数，C：可用的竖直线数
// 枚举整块矩形的行数 r，列数 c=k/r，余下 z=k%r 枚石子补成一列（或一行）
i64 calc(i64 R,i64 C)
{
	i64 best=0;
	for(i64 r=1;r<=R;++r)
	{
		i64 c=k/r,z=k%r;
		if(c==0||c>C)
			continue;
		i64 base=c2(r)*c2(c);
		if(base>best)
			best=base;
		if(z==0)
			continue;
		// 余下的 z 枚单独占一列，列高 z<=r，总列数 c+1<=C
		if(z<=r&&c+1<=C)
			best=max(best,base+c*c2(z));
		// 余下的 z 枚单独占一行，行长 z<=c，总行数 r+1<=R
		if(z<=c&&r+1<=R)
			best=max(best,base+r*c2(z));
	}
	return best;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	cout<<max(calc(n,m),calc(m,n))<<'\n';
	return 0;
}
