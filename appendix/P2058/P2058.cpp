// 题目：P2058 [NOIP 2016 普及组] 海港
// 思路：时间递增，用队列维护 (t_i-86400, t_i] 这段窗口里的船，
//       再用国籍计数数组维护「出现过的不同国籍数」
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};
constexpr int mxk{300005};

int t[mxn],kc[mxn],st[mxn];
int nat[mxk];			// 扁平存储所有乘客的国籍
int cnty[100005];		// cnty[x]: 当前窗口里国籍 x 的人数
int q[mxn];				// 队列存船的编号

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	int tot=0;
	for(int i=0;i<n;++i)
	{
		cin>>t[i]>>kc[i];
		st[i]=tot;
		for(int j=0;j<kc[i];++j) cin>>nat[tot++];
	}
	int head=0,tail=0;
	int diff=0;				// 当前窗口内不同国籍数
	for(int i=0;i<n;++i)
	{
		// 踢掉过期船：窗口左端开，t_p <= t_i-86400 的都不在窗口内
		while(head<tail&&t[q[head]]<=t[i]-86400)
		{
			int s=q[head++];
			for(int j=st[s];j<st[s]+kc[s];++j)
			{
				if(!--cnty[nat[j]]) --diff;
			}
		}
		// 加入当前船
		for(int j=st[i];j<st[i]+kc[i];++j)
		{
			if(!cnty[nat[j]]++) ++diff;
		}
		q[tail++]=i;
		cout<<diff<<'\n';
	}
	return 0;
}
