// 暴力：x<=26 时 x^x 可用 __int128 精确计算，数数字位数
#include<bits/stdc++.h>
using namespace std;
using i128=__int128;

int digits(i128 v){int d=0;while(v>0){v/=10;++d;}return d;}

int main()
{
	int n;cin>>n;
	for(int x=1;x<=26;++x)
	{
		i128 p=1;
		for(int k=0;k<x;++k)p*=x;
		if(digits(p)>=n){cout<<x<<'\n';return 0;}
	}
	cout<<-1<<'\n';	// n 太大，超出暴力范围
	return 0;
}