//
// 题目：P1602 Sramoc 问题
// https://www.luogu.com.cn/problem/P1602
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};          // m<=1000, 余数状态数 (+5 松弛)

int k,m;
int pre[mxn],pd[mxn];             // 余数 r 的前驱余数 / 末位数字
bool vis[mxn];
int q[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>m;

	int ql=0,qr=0;
	for(int d=1;d<k;++d)          // 首位不能是 0
	{
		int r=d%m;
		if(vis[r])
			continue;
		vis[r]=true;
		pre[r]=-1;                // -1 表示这是首位
		pd[r]=d;
		q[qr++]=r;
	}

	while(ql<qr)
	{
		int r=q[ql++];
		if(r==0)
			break;                // 队首首次出现 0 余数即最短且字典序最小
		for(int d=0;d<k;++d)
		{
			int nr=(r*10+d)%m;
			if(vis[nr])
				continue;
			vis[nr]=true;
			pre[nr]=r;
			pd[nr]=d;
			q[qr++]=nr;
		}
	}

	string ans;
	for(int r=0;r!=-1;r=pre[r])   // 沿前驱回溯出整个数字
		ans+=char('0'+pd[r]);
	reverse(ans.begin(),ans.end());
	cout<<ans<<'\n';
	return 0;
}
