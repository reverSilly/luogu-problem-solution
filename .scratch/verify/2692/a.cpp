// 题目：P2692 覆盖
// 男生扫整行、女生扫整列 ⇒ Clean(i,j) = rowcov(i) || colcov(j)，二维塌成两个一维
// 数补集最省事：未打扫格子 = 未覆盖行 × 未覆盖列 = (N-R)*(M-C)
// R/C 用差分+前缀和求，判据 s>0（重叠只算一次），全程 O(N+M+B+G)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{5005};	// 差分要写到 y+1，+5 松弛

int n,m,b,g,dr[mxn],dc[mxn];	// dr=行差分（男生），dc=列差分（女生）

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>b>>g;
	for(int i=0,x,y;i<b;++i)
	{
		cin>>x>>y;--x;--y;
		++dr[x];--dr[y+1];
	}
	for(int i=0,x,y;i<g;++i)
	{
		cin>>x>>y;--x;--y;
		++dc[x];--dc[y+1];
	}
	int R=0,C=0,s=0;
	for(int i=0;i<n;++i)
	{
		s+=dr[i];
		if(s>0)	// s 是"人数"，可能 >1；一行只算一次
			++R;
	}
	s=0;	// 扫列前必须重置游标
	for(int i=0;i<m;++i)
	{
		s+=dc[i];
		if(s>0)
			++C;
	}
	cout<<(i64)n*m-(i64)(n-R)*(m-C)<<'\n';
	return 0;
}
