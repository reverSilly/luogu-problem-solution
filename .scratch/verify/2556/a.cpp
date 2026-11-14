// 题目：P2556 [AHOI2002] 黑白图像压缩
// 字节展开成像素（高位在前），再按颜色做游程编码：每段输出 (颜色<<7)|长度
// 题目保证每段长度 < 128，恰好塞进低 7 位
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{80005};

int n,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n/8;++i)
	{
		int b;
		cin>>b;
		for(int j=7;j>=0;--j)
			a[i*8+(7-j)]=(b>>j)&1;	// 高位对应前面的像素
	}
	bool first=true;
	for(int i=0;i<n;)
	{
		int j=i;
		while(j<n&&a[j]==a[i])
			++j;
		if(!first)
			cout<<' ';
		first=false;
		cout<<((a[i]<<7)|(j-i));	// 颜色占最高位，长度占低 7 位
		i=j;
	}
	cout<<'\n';
	return 0;
}
