// 题目：P2437 蜜蜂路线
// 思路：蜂房 i 只能爬到 i+1 / i+2，答案只依赖 d=n-m，
//       g(d)=g(d-1)+g(d-2)，g(0)=g(1)=1，即 F_{d+1}。
//       F_1000 有 209 位，必须高精度；递推只含加法，用 1e9 压位。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxl{64};
constexpr int base{1'000'000'000};

int m,n,d,len[mxn];
int f[mxn][mxl];//f[i]=距离为 i 时的路线数，base 进制，低位在前

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	d=n-m;
	f[0][0]=1;len[0]=1;//原地不动算一条路线
	f[1][0]=1;len[1]=1;
	for(int i=2;i<=d;++i)
	{
		int l=len[i-1],carry=0;
		for(int j=0;j<l;++j)
		{
			int cur=f[i-1][j]+carry;
			if(j<len[i-2])cur+=f[i-2][j];
			f[i][j]=cur%base;
			carry=cur/base;
		}
		if(carry)f[i][l++]=carry;//最高位进位
		len[i]=l;
	}
	cout<<f[d][len[d]-1];
	for(int j=len[d]-2;j>=0;--j)
		cout<<setw(9)<<setfill('0')<<f[d][j];
	cout<<'\n';
	return 0;
}
