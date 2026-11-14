// 题目：P2646 数数zzy
// 思路：统计子序列(非子串) zzy 的个数 = 递增下标三元组 i<j<k 且 s_i=s_j='z',s_k='y'。
//       定长模式子序列计数 DP：维护 cz/czz/czzy = 已扫前缀里 z/zz/zzy 的出现数。
//       遇 'y'：czzy+=czz；遇 'z'：先 czz+=cz(旧值) 再 ++cz；其它跳过。
//       答案可达 2^63-1，全程 i64。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

char s[mxn];
i64 cz,czz,czzy;	// 已扫前缀中 "z" / "zz" / "zzy" 子序列的个数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	for(int i=0;s[i];++i)
	{
		if(s[i]=='y')
			czzy+=czz;	// 每个已有 zz 接上这个 y 成一个 zzy
		else if(s[i]=='z')
		{
			czz+=cz;	// 先用旧的 cz 与新 z 配对（i<j，不含自己）
			++cz;		// 再把这个 z 计入单字符前缀
		}
	}
	cout<<czzy<<'\n';
	return 0;
}
