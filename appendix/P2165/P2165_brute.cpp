#include<bits/stdc++.h>
using namespace std;

//暴力：枚举所有点对组合，判断四点能否围成矩形
//圆内接四边形是矩形当且仅当两条对角线都是直径
constexpr int mxn{25};

int n,len[mxn],pos[mxn],sum;

bool antipodal(int x,int y)
{
	return 2*(pos[y]-pos[x])==sum;
}

int main()
{
	cin.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	sum=0;
	for(int i=0;i<n;++i)
	{
		cin>>len[i];
		sum+=len[i];
	}
	pos[0]=0;
	for(int i=1;i<n;++i)
		pos[i]=pos[i-1]+len[i-1];

	int ans=0;
	for(int a=0;a<n;++a)
		for(int b=a+1;b<n;++b)
			for(int c=b+1;c<n;++c)
				for(int d=c+1;d<n;++d)
				{
					//按圆周顺序 a<b<c<d，对边成对：(a,c) 与 (b,d)
					if(antipodal(a,c)&&antipodal(b,d))
						++ans;
				}
	cout<<ans<<'\n';
	return 0;
}
