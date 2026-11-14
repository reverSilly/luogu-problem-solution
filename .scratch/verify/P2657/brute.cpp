// P2657 暴力：逐个检查 [a,b] 内每个数是否 windy
#include<bits/stdc++.h>
using namespace std;
using i64=long long;
bool windy(i64 x)
{
	int last=-1;
	// 从低位到高位检查相邻差
	while(x)
	{
		int d=x%10;
		if(last!=-1&&abs(d-last)<2)
			return false;
		last=d;
		x/=10;
	}
	return true;
}
int main()
{
	i64 a,b;cin>>a>>b;
	i64 c=0;
	for(i64 n=a;n<=b;++n)
		if(windy(n))++c;
	cout<<c<<'\n';
	return 0;
}