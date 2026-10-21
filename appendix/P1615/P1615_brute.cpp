//
// P1615 朴素解：逐秒推进时钟做模拟
// 用法：仅用于与 P1615.cpp 对拍，非提交代码
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	int h1,m1,s1,h2,m2,s2;
	i64 n;
	if(scanf("%d:%d:%d",&h1,&m1,&s1)!=3)
		return 0;
	if(scanf("%d:%d:%d",&h2,&m2,&s2)!=3)
		return 0;
	cin>>n;
	i64 cnt=0;
	while(!(h1==h2&&m1==m2&&s1==s2))
	{
		++cnt;
		++s1;
		if(s1==60)
		{
			s1=0;
			++m1;
			if(m1==60)
			{
				m1=0;
				++h1;
			}
		}
	}
	cout<<cnt*n<<'\n';
	return 0;
}
