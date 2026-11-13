// 题目：P2431 正妹吃月饼
// 子集和 <-> 二进制数，个数 <-> popcount
// 答案 = [A,B] 内最大 popcount；按 A^B 的最高位 p 分两支
#include<bits/stdc++.h>
using namespace std;
using u64=unsigned long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	u64 A,B;
	cin>>A>>B;
	if(A==B)						// 退化：区间只有一个数
	{
		cout<<__builtin_popcountll(A)<<'\n';
		return 0;
	}
	u64 x=A^B;
	int p=63-__builtin_clzll(x);			// A,B 最高的不同位
	int cnt=__builtin_popcountll(A>>(p+1));	// 被锁死的公共高位里 1 的个数
	u64 mask=(1ULL<<(p+1))-1;
	// 第 p 位取 0 稳拿 p 个；取 1 只有在 B 低 p+1 位全 1 时才多拿 1 个
	cout<<cnt+p+((B&mask)==mask)<<'\n';
	return 0;
}
