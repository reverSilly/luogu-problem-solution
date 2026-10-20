//
// 题目：P1540 [NOIP 2010 提高组] 机器翻译
// https://www.luogu.com.cn/problem/P1540
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};          // N<=1000 (+5 松弛)
constexpr int mxv{1005};          // 单词编号 <=1000 (+5 松弛)

int m,n;
int q[mxn];                       // 内存队列:按进入顺序存单词编号(仅用 [head,tail))
int head=0,tail=0;                // 队头/队尾:tail-head 即当前内存中的单词数
bool inmem[mxv];                  // inmem[v]=单词 v 是否仍在内存中

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	int ans=0;
	for(int i=0,x;i<n;++i)
	{
		cin>>x;
		if(inmem[x])              // 命中:内存中已有,不查词典
			continue;
		++ans;                    // 未命中:查一次外存词典
		if(tail-head==m)          // 内存已满:清空最早进入的那个单词
		{
			inmem[q[head]]=false;
			++head;
		}
		q[tail++]=x;              // 新词入内存
		inmem[x]=true;
	}
	cout<<ans<<'\n';
	return 0;
}
