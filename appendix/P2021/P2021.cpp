// 题目：P2021 faebdc玩扑克
// 用循环双向链表模拟「牌堆」，位置 1..n 组成环
// 每步：把最上方的牌移到牌堆底（等价于队首后移），再把下一张牌取出并输出
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

int n;
int ans[mxn];
int nxt[mxn],prv[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		nxt[i]=(i+1==n)?0:i+1;
		prv[i]=(i==0)?n-1:i-1;
	}
	int cur=0;//cur 指向当前最上方的牌
	for(int v=1;v<=n;++v)
	{
		cur=nxt[cur];//最上方的牌移到底部，新的牌堆顶是原第二张
		ans[cur]=v;//这一张被取出来输出 v
		nxt[prv[cur]]=nxt[cur];//从环上摘掉 cur
		prv[nxt[cur]]=prv[cur];
		cur=nxt[cur];//摘掉之后的新牌堆顶
	}
	for(int i=0;i<n;++i)
	{
		if(i)
			cout<<' ';
		cout<<ans[i];
	}
	cout<<'\n';
	return 0;
}
