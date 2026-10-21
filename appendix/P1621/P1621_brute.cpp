//
// P1621 朴素解：反复扫描所有点对，合并共享 >=p 质因子的两个数
// 用法：仅用于与 P1621.cpp 对拍，非提交代码
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int a,b,p,ans;
int f[mxn];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

bool hasbig(int g)                 // g 是否存在 >=p 的质因子
{
	if(g<2)
		return false;
	for(int d=2;1LL*d*d<=g;++d)
		if(g%d==0)
		{
			while(g%d==0)
				g/=d;
			if(d>=p)
				return true;
		}
	if(g>1&&g>=p)
		return true;
	return false;
}

int main()
{
	cin>>a>>b>>p;
	for(int i=a;i<=b;++i)
		f[i]=i;
	bool changed=true;
	while(changed)
	{
		changed=false;
		for(int i=a;i<=b;++i)
			for(int j=i+1;j<=b;++j)
			{
				if(getrt(i)==getrt(j))
					continue;
				if(hasbig(__gcd(i,j)))
				{
					f[getrt(i)]=getrt(j);
					changed=true;
				}
			}
	}
	for(int i=a;i<=b;++i)
		if(getrt(i)==i)
			++ans;
	cout<<ans<<'\n';
	return 0;
}
