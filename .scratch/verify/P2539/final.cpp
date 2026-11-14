// 题目：P2539 [AHOI2005] 矿藏编码
// 思路：编码是四叉树先序遍历。深度 d 的块面积 4^(k-d)。
//       dfs 解析：'0' 叶子加整块面积，'1' 加 0，'2' 递归四子求和。
//       答案最大 4^49=2^98 远超 long long ⇒ 全程 __int128，手工逐位输出。
//       只求总和，象限顺序无关（加法交换律）。
#include<bits/stdc++.h>
using namespace std;
using i128=__int128;

constexpr int mxn{205},mxk{55};

int k,pos;
string s;
i128 pw[mxk];	// pw[d]=4^d，深度 d 的块含 4^(k-d) 个格子

i128 dfs(int d)	// 解析深度 d 的一个块，返回其中平地格子数
{
	char c=s[pos++];
	if(c=='0')
		return pw[k-d];	// 整块平地
	if(c=='1')
		return 0;	// 整块山峰
	i128 res=0;
	for(int i=0;i<4;++i)
		res+=dfs(d+1);	// '2'：四份依次编码
	return res;
}

void wr(i128 x)	// __int128 手工输出（答案非负，无需负号）
{
	if(x>9)
		wr(x/10);
	putchar((int)(x%10)+'0');
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>s;
	pw[0]=1;
	for(int i=1;i<=k;++i)
		pw[i]=pw[i-1]*4;
	wr(dfs(0));
	putchar('\n');
	return 0;
}
