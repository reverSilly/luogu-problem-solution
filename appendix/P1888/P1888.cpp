//
// 题目：P1888 三角函数
// https://www.luogu.com.cn/problem/P1888
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

int gg(int x,int y)
{
	while(y){
		int t=x%y;
		x=y;
		y=t;
	}
	return x;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int a[3];
	cin>>a[0]>>a[1]>>a[2];
	sort(a,a+3);
	int g=gg(a[0],a[2]);
	cout<<a[0]/g<<'/'<<a[2]/g<<'\n';
	return 0;
}
