// 题目：P2660 zzc 种田
// 思路：体力=周长和=4*边长和。贪心切最大正方形=辗转相除，边长和=x+y-gcd(x,y)。
//       下界：含短边正方形⇒全高分解+归纳；全部偏小⇒四边计数 S>=2x>=x+y-gcd。
//       故答案=4*(x+y-gcd(x,y))。x,y<=1e16，4*(x+y)<=8e16，i64 安全。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 gcd(i64 a,i64 b){return b?gcd(b,a%b):a;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 x,y;
	cin>>x>>y;
	// 最小边长和 = x+y-gcd(x,y)，体力 = 4 倍边长和
	cout<<4*(x+y-gcd(x,y))<<'\n';
	return 0;
}
