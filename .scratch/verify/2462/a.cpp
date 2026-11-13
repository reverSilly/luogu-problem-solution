// 题目：P2462 [SDOI2007] 游戏
// 接龙规则等价于：后词字母计数向量 = 前词计数向量 + 恰一个字母
// 把单词压成 26 维签名（每维一个字节），同签名（重复词/异位词）只留一个代表
// 按长度分层 DP：f[u]=以 u 结尾的最长接龙；前驱 = 签名减去一个字母，哈希查找
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005},mxc{26},mxl{105};

int tot,f[mxn],prv[mxn];
string wrd[mxn],key[mxn];
unordered_map<string,int>id;
vector<int>bylen[mxl];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	string s;
	while(cin>>s)
	{
		if(s.find_first_of("0123456789")!=string::npos)
			continue;   // 题面只说有 n 行单词；若数据首行给了数字 n 则跳过（单词保证全小写）
		string k(mxc,'\0');
		for(char ch:s)
			++k[ch-'a'];
		if(id.count(k))
			continue;   // 同签名接龙能力完全相同，留一个代表即可
		int u=tot++;
		id[k]=u;
		wrd[u]=s;
		key[u]=k;
		f[u]=1;
		prv[u]=-1;
		bylen[s.size()].push_back(u);
	}
	int best=0;
	for(int l=1;l<mxl;++l)
		for(int u:bylen[l])
		{
			for(int c=0;c<mxc;++c)
			{
				if(key[u][c]==0)
					continue;
				string k=key[u];
				--k[c];   // 前驱签名：第 c 种字母少一个，长度恰减 1
				auto it=id.find(k);
				if(it==id.end())
					continue;
				int v=it->second;
				if(f[v]+1>f[u])
				{
					f[u]=f[v]+1;
					prv[u]=v;
				}
			}
			if(f[u]>f[best])
				best=u;
		}
	vector<int>path;
	for(int u=best;u!=-1;u=prv[u])
		path.push_back(u);
	reverse(path.begin(),path.end());
	cout<<path.size()<<'\n';
	for(int u:path)
		cout<<wrd[u]<<'\n';
	return 0;
}
