//
// 题目：P1583 魔法照片
// https://www.luogu.com.cn/problem/P1583
// 暴力基准:选择排序定名次 + 重复 k 次线性取最大,类号按题面原文 (D-1)%10+1
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};
constexpr int mxe{15};

int n,k;
int e[mxe];
int w[mxn];
int d[mxn];                    // d[i]=内部编号 i 的名次 D_i
bool used[mxn];                // 定名次阶段:是否已排名
bool took[mxn];                // 选人阶段:是否已入选
int f[mxn];                    // f[i]=最终权值

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	for(int i=1;i<=10;++i)
		cin>>e[i];
	for(int i=0;i<n;++i)
		cin>>w[i];
	// 每轮从未排名的人里线性扫出(w 最大,编号最小)的一个,名次 r=1..n
	for(int r=1;r<=n;++r)
	{
		int b{-1};
		for(int i=0;i<n;++i)
			if(!used[i]&&(b<0||w[i]>w[b]||(w[i]==w[b]&&i<b)))
				b=i;
		used[b]=true;
		d[b]=r;
	}
	// 类号按题面原文 (D-1)%10+1,与标程的 r%10+1 相互印证
	for(int i=0;i<n;++i)
		f[i]=w[i]+e[(d[i]-1)%10+1];
	// 重复 k 次:从未入选的人里线性扫出(f 最大,编号最小)的一个
	for(int t=0;t<k;++t)
	{
		int b{-1};
		for(int i=0;i<n;++i)
			if(!took[i]&&(b<0||f[i]>f[b]||(f[i]==f[b]&&i<b)))
				b=i;
		took[b]=true;
		cout<<b+1<<' ';
	}
	cout<<'\n';
	return 0;
}
