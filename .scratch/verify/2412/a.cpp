// 题目：P2412 查单词
// 思路：忽略大小写 ⇒ 每个单词存一份全小写键 low[]，原串 s[] 留着输出。
//       「键大优先，键同则靠后优先」打包成二元组 (low[i],i) 的字典序 ⇒ 一个全序 max。
//       max 幂等 ⇒ ST 表可以用两块重叠区间 O(1) 回答询问。
//       st[k][i] 存下标而不是字符串，省内存省拷贝。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005},mxk{20};

int n,m,lg[mxn],st[mxk][mxn];
string s[mxn],low[mxn];

int btr(int a,int b)	// 忽略大小写字典序大者优先；相同则靠后（下标大）者优先
{
	if(low[a]!=low[b])
		return low[a]>low[b]?a:b;
	return a>b?a:b;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		cin>>s[i];
		low[i]=s[i];
		for(char&c:low[i])
			if(c>='A'&&c<='Z')
				c+=32;			// low 只用于比较，s 保留原样用于输出
	}
	for(int i=2;i<=n;++i)
		lg[i]=lg[i>>1]+1;
	for(int i=0;i<n;++i)
		st[0][i]=i;
	for(int k=1;(1<<k)<=n;++k)
		for(int i=0;i+(1<<k)<=n;++i)
			st[k][i]=btr(st[k-1][i],st[k-1][i+(1<<(k-1))]);
	for(int q=0,x,y;q<m;++q)
	{
		cin>>x>>y;--x;--y;
		if(x>y)
			swap(x,y);
		int k=lg[y-x+1];
		cout<<s[btr(st[k][x],st[k][y-(1<<k)+1])]<<'\n';
	}
	return 0;
}
