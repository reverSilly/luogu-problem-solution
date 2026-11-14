// 题目：P2699 【数学 1】小浩的幂次运算
// 枚举 w^0,w^1,...；w=1 特判（否则死循环）；乘法前用 cur>r/w 防溢出
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 l,r,w;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>l>>r>>w;
	if(w==1)	// 1 的任何次幂都是 1，枚举循环永不终止
	{
		cout<<(l<=1&&1<=r?"1":"-1")<<'\n';
		return 0;
	}
	bool any=false;
	for(i64 cur=1;cur<=r;)
	{
		if(cur>=l)
		{
			if(any)cout<<' ';
			cout<<cur;
			any=true;
		}
		if(cur>r/w)break;	// 等价于 cur*w>r：下一个幂越界，且再乘会溢出
		cur*=w;
	}
	if(!any)cout<<-1;
	cout<<'\n';
	return 0;
}
