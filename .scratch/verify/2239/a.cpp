// 题目：P2239 [NOIP 2014 普及组] 螺旋矩阵
// 思路：螺旋矩阵是同心方环；第 k 环左上角为 1+4k(n-k)，再按四条边算环内偏移，O(1)
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 n,i,j;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>i>>j;
	--i;--j;
	i64 k=min({i,j,n-1-i,n-1-j});   // 到四条边距离的最小值 = 层号
	i64 s=n-2*k;                    // 本层边长
	i64 st=1+4*k*(n-k);             // 本层左上角 (k,k) 填的数
	i64 r=i-k,c=j-k;                // 层内相对坐标
	i64 ans;
	if(r==0)                        // 上边：左 -> 右
		ans=st+c;
	else if(c==s-1)                 // 右边：上 -> 下
		ans=st+(s-1)+r;
	else if(r==s-1)                 // 下边：右 -> 左
		ans=st+2*(s-1)+(s-1-c);
	else                            // 左边：下 -> 上
		ans=st+3*(s-1)+(s-1-r);
	cout<<ans<<'\n';
	return 0;
}
