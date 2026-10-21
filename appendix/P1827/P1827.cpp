//
// 题目：P1827 [USACO3.4] 美国血统 American Heritage
// https://www.luogu.com.cn/problem/P1827
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

string ino,pre;
int pos[128];

void dfs(int il,int ir,int pl)
{
	if(il>ir)
		return;
	int k=pos[(unsigned char)pre[pl]];
	int llen=k-il;
	dfs(il,k-1,pl+1);
	dfs(k+1,ir,pl+1+llen);
	cout<<pre[pl];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>ino>>pre;
	int n=(int)ino.size();
	for(int i=0;i<n;++i)
		pos[(unsigned char)ino[i]]=i;
	dfs(0,n-1,0);
	cout<<'\n';
	return 0;
}
