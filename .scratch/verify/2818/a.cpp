// 题目：P2818 天使的起誓
// 第 m 个盒子 = ((m-1) mod n)+1，等价于 r=m mod n 后输出 (r?r:n)
// m 最多 1001 位 ⇒ 字符串读入；只要余数 ⇒ 逐位 Horner 取模，不需要高精度除法
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int n;
string m;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	i64 r=0;
	for(char ch:m)
		if(ch>='0'&&ch<='9')
			r=(r*10+ch-'0')%n;	// 不变量 0<=r<n；中间量 <=10(n-1)+9<1e9
	cout<<(r?r:n)<<'\n';		// 余数 0 = 恰好整圈，落在 n 号盒而非 0 号盒
	return 0;
}
