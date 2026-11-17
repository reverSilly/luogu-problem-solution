// 题目：P2955 [USACO09OCT] Even? Odd? G
// l_j 可达 1e60，任何内置整型都装不下 ⇒ 按字符串读入
// 十进制数 mod 2 只由末位决定：N = d0 + 10*M ⇒ N ≡ d0 (mod 2)
#include<bits/stdc++.h>
using namespace std;

int n;
string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>s;
		cout<<(((s.back()-'0')&1)?"odd\n":"even\n");
	}
	return 0;
}
