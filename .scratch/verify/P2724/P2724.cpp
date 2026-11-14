// 题目：P2724 [IOI 1998 / USACO3.1] 联系 Contact
// B≤12 ⇒ 不同序列至多 8190 个；对每个长度 L 用滚动窗口 O(|s|) 统计出现次数（可重叠）。
// 按 (频率降, 长度升, 数值升) 排序取前 n，同频率分组、每行 6 个输出。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxl{13},mxv{4096};

int A,B,n,cnt[mxl][mxv],tot;
string s;
struct node{int c,L,v;}all[8200];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>A>>B>>n;
	string t;
	while(cin>>t)
		s+=t;	// 逐行读入并按顺序拼接
	int len=s.size();
	for(int L=A;L<=B;++L)
	{
		int v=0,mask=(1<<L)-1;
		for(int i=0;i<len;++i)
		{
			v=((v<<1)|(s[i]-'0'))&mask;	// 滚动保留末 L 位
			if(i>=L-1)
				++cnt[L][v];
		}
	}
	for(int L=A;L<=B;++L)
		for(int v=0;v<(1<<L);++v)
			if(cnt[L][v])
				all[tot++]={cnt[L][v],L,v};
	sort(all,all+tot,[](node a,node b){
		if(a.c!=b.c)return a.c>b.c;
		if(a.L!=b.L)return a.L<b.L;
		return a.v<b.v;
	});
	int take=min(n,tot);
	for(int i=0;i<take;)
	{
		int j=i;
		while(j<take&&all[j].c==all[i].c)
			++j;
		cout<<all[i].c<<'\n';
		for(int k=i;k<j;++k)
		{
			if((k-i)%6)
				cout<<' ';
			for(int b=all[k].L-1;b>=0;--b)
				cout<<((all[k].v>>b)&1);
			if((k-i)%6==5||k==j-1)
				cout<<'\n';
		}
		i=j;
	}
	return 0;
}