// 题目：P2814 家谱
// 儿子→父亲是内向森林，最早祖先=树根。用并查集父亲指针+路径压缩求根。
// 关键：森林结构由输入给定，必须 f[son]=father 直接认父亲，禁止按秩合并交换方向。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005};

int f[mxn],cur,cnt;
string nam[mxn];
map<string,int>id;

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);	// 路径压缩：不改变根
}
int getid(const string&s)
{
	auto it=id.find(s);
	if(it!=id.end())
		return it->second;
	nam[cnt]=s;
	f[cnt]=cnt;			// 新人自成一树
	return id[s]=cnt++;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	while(cin>>s&&s!="$")
	{
		string t=s.substr(1);
		if(s[0]=='#')
			cur=getid(t);
		else if(s[0]=='+')
			f[getid(t)]=cur;	// 直接认父亲，根永远是辈分最高者
		else
		{
			int u=getid(t);
			cout<<t<<' '<<nam[getrt(u)]<<'\n';
		}
	}
	return 0;
}
