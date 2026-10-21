//
// 题目：P1827 [USACO3.4] 美国血统 American Heritage
// https://www.luogu.com.cn/problem/P1827
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

string ino,pre;
int pos[128],lc[128],rc[128];

int build(int il,int ir,int pl)
{
	if(il>ir)
		return -1;
	int root=(unsigned char)pre[pl];
	int k=pos[root];
	int llen=k-il;
	lc[root]=build(il,k-1,pl+1);
	rc[root]=build(k+1,ir,pl+1+llen);
	return root;
}

void post(int u)
{
	if(u<0)
		return;
	post(lc[u]);
	post(rc[u]);
	cout<<(char)u;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>ino>>pre;
	int n=(int)ino.size();
	fill_n(lc,128,-1);
	fill_n(rc,128,-1);
	for(int i=0;i<n;++i)
		pos[(unsigned char)ino[i]]=i;
	int root=build(0,n-1,0);
	post(root);
	cout<<'\n';
	return 0;
}
