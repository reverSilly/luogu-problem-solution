//
// 题目：P1577 切绳子 —— 暴力基准(精确有理数枚举,仅供小规模对拍)
// 思路:最优解 x* 必为断点 a_i/t;枚举全部候选,纯整数运算判可行,取最大
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{10005};

int n,k;
i64 a[mxn];

// 字符串精确解析长度 -> 整数分(不经过浮点,与标程的读入互相独立)
i64 parse(const string&s)
{
	i64 v=0;int fd=0;bool f=false;
	for(char c:s)
	{
		if(c=='.'){f=true;continue;}
		v=v*10+(c-'0');
		if(f)++fd;
	}
	if(!f)v*=100;
	else while(fd++<2)v*=10;
	return v;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		a[i]=parse(s);
	}
	i64 bn=0,bd=1;              // 当前最大可行解 x*=bn/bd
	for(int i=0;i<n;++i)
		for(i64 t=1;t<=k;++t)    // 断点参数 t<=k(最优断点的 t 不超过 k)
		{
			i64 cnt=0;
			for(int j=0;j<n;++j)
			{
				cnt+=a[j]*t/a[i];   // floor(a_j/(a_i/t)),纯整数除法,精确
				if(cnt>=k)break;
			}
			if(cnt>=k&&a[i]*bd>bn*t)   // a[i]/t>bn/bd,叉乘比较
			{
				bn=a[i];bd=t;
			}
		}
	i64 ans=bn/bd;              // floor(x*):x* 本就是以"分"为单位,截断到整数分
	cout<<ans/100<<'.';
	if(ans%100<10)
		cout<<'0';
	cout<<ans%100<<'\n';
	return 0;
}
