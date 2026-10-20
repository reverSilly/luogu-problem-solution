//
// 题目：P1583 魔法照片
// https://www.luogu.com.cn/problem/P1583
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};     // n<=20000 (+5 松弛)
constexpr int mxe{15};        // 类别号 1..10 (+5 松弛)

int n,k;
int e[mxe];                   // e[c]=第 c 类的额外权值(c=1..10)
int w[mxn];                   // w[i]=内部编号 i(题面编号 i+1)的初始权值
int f[mxn];                   // f[i]=最终权值 w[i]+e[C_i]
int id[mxn];                  // 名次排列/答案排列用的内部编号

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
	for(int i=0;i<n;++i)
		id[i]=i;
	// 第一次排序:初始权值降序,平局编号小优先(内部编号小=题面编号小)
	sort(id,id+n,[](int a,int b){return w[a]!=w[b]?w[a]>w[b]:a<b;});
	for(int r=0;r<n;++r)       // id[r] 的名次 D=r+1,类别 C=(D-1)%10+1=r%10+1
		f[id[r]]=w[id[r]]+e[r%10+1];
	// 第二次排序:最终权值降序,平局编号小优先(不可对旧序 stable_sort,平局规则不同)
	sort(id,id+n,[](int a,int b){return f[a]!=f[b]?f[a]>f[b]:a<b;});
	for(int i=0;i<k;++i)
		cout<<id[i]+1<<' ';    // 恢复 1-based 编号
	cout<<'\n';
	return 0;
}
