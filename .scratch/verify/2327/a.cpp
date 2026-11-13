// 题目：P2327 [SCOI2005] 扫雷
// 第二列第 i 格的 8 邻域里只有第一列的 i-1,i,i+1 有雷 => a_i = b_{i-1}+b_i+b_{i+1}
// b_{i+1} 的系数是 1 => 由 b_1 唯一确定整列 => 只枚举 b_1 的两种取值
// 中途算出非 0/1 即整条链判死；最后算出的 b_{n+1} 必须为 0
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
		cin>>a[i];
	int ans=0;
	for(int b1=0;b1<2;++b1)
	{
		int prv=0,cur=b1;	//b_0=0（棋盘外无雷），b_1=b1
		bool ok=1;
		for(int i=1;i<=n;++i)
		{
			int nxt=a[i]-prv-cur;	//a_i=b_{i-1}+b_i+b_{i+1}
			if(nxt<0||nxt>1)
			{
				ok=0;
				break;
			}
			prv=cur;
			cur=nxt;
		}
		if(ok&&cur==0)		//最后算出的是 b_{n+1}，必须是 0
			++ans;
	}
	cout<<ans<<'\n';
	return 0;
}
