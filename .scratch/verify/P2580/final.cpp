// 题目：P2580 于是他错误的点名开始了
// 思路：名单插进字典树，结尾打 ed 标记；查询沿字符走。
//       走不通或 ed=false ⇒ WRONG（ed 区分"完整名字"与"仅前缀"）；
//       ed=true 时看 vis：首次 OK 并置 vis，之后 REPEAT。
//       名字互不相同 ⇒ 结尾节点互不相同 ⇒ vis 打在节点上等价于打在名字上。
//       Trie 确定性 O(L)，对 anti-hash hack 免疫。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxnode{500005};	// n*50+5，最坏无公共前缀

int n,m,tot;
int ch[mxnode][26];	// 0 表示空儿子（根也是 0）
bool ed[mxnode],vis[mxnode];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		string s;cin>>s;
		int u=0;
		for(char c:s)
		{
			int x=c-'a';
			if(!ch[u][x])ch[u][x]=++tot;
			u=ch[u][x];
		}
		ed[u]=true;	// 名字结尾
	}
	cin>>m;
	for(int i=0;i<m;++i)
	{
		string s;cin>>s;
		int u=0;
		bool ok=true;
		for(char c:s)
		{
			int x=c-'a';
			if(!ch[u][x]){ok=false;break;}
			u=ch[u][x];
		}
		if(!ok||!ed[u])cout<<"WRONG\n";	// 走不通 / 只是前缀
		else if(!vis[u]){vis[u]=true;cout<<"OK\n";}
		else cout<<"REPEAT\n";
	}
	return 0;
}
