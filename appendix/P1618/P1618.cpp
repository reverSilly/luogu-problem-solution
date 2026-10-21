//
// 题目：P1618 三连击（升级版）
// https://www.luogu.com.cn/problem/P1618
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

int A,B,C;
bool vis[10];

bool ok(int x)                     // 拆出三个数字，要求恰好用完 1..9 各一次
{
	while(x)
	{
		int d=x%10;
		if(d==0||vis[d])
			return false;
		vis[d]=true;
		x/=10;
	}
	return true;
}

bool chk(int x,int y,int z)
{
	if(x<100||y<100||z<100||x>999||y>999||z>999)
		return false;
	memset(vis,0,sizeof vis);
	return ok(x)&&ok(y)&&ok(z);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>A>>B>>C;
	if(A==0)                       // 比例含 0 时第一个数必为 0，不可能是三位数
	{
		cout<<"No!!!"<<'\n';
		return 0;
	}
	int g=__gcd(__gcd(A,B),C);     // 约分，否则 2:4:6 这类比例会漏解
	A/=g;B/=g;C/=g;
	bool found=false;
	for(int k=1;k*C<=999;++k)
	{
		int x=k*A,y=k*B,z=k*C;
		if(chk(x,y,z))
		{
			cout<<x<<' '<<y<<' '<<z<<'\n';
			found=true;
		}
	}
	if(!found)
		cout<<"No!!!"<<'\n';
	return 0;
}
