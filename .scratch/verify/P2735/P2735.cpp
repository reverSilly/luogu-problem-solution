// 题目：P2735 [USACO3.4] 网 Electric Fences
// Pick 定理：S=I+B/2-1 ⇒ I=S-B/2+1=(2S-B+2)/2。
// 2S=p*m；边界格点数 B=gcd(n,m)+gcd(|p-n|,m)+p（三条边各自 gcd，顶点不重复计）。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n,m,p;

i64 gcd(i64 a,i64 b){return b?gcd(b,a%b):a;}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>p;
	i64 b=gcd(n,m)+gcd(abs(p-n),m)+p;	// 三条边的格点数（含端点，顶点恰好各算一次）
	i64 ans=(p*m-b+2)/2;	// 2S=p*m 为偶数，B 与 2S 同奇偶，整除精确
	cout<<ans<<'\n';
	return 0;
}