// 题目：P2815 IPv6地址压缩
// 完全展开地址结构固定：substr(i*5,4) 切 8 组，去前导零（全 0 组留 "0"）。
// 线性扫描选最长、最靠前的连续 "0" 段（严格大于更新），压成一次 ::（Darwin：单个 0 也压）。
#include<bits/stdc++.h>
using namespace std;

string s,g[8];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	for(int i=0;i<8;++i)
	{
		string t=s.substr(i*5,4);
		int j=0;
		while(j<3&&t[j]=='0')
			++j;
		g[i]=t.substr(j);	// j<3 保证全 0 组也保留一个 "0"
	}
	int bl=-1,br=-1,bst=0,cl=-1,cr=-1;
	for(int i=0;i<8;++i)
	{
		if(g[i]=="0")
		{
			if(cl==-1)
				cl=i;
			cr=i;
		}
		else
		{
			if(cl!=-1&&cr-cl+1>bst)	// 严格大于 ⇒ 平局取最靠前
				bst=cr-cl+1,bl=cl,br=cr;
			cl=cr=-1;
		}
	}
	if(cl!=-1&&cr-cl+1>bst)	// 末段可能延伸到第 7 组，循环外结算
		bst=cr-cl+1,bl=cl,br=cr;
	if(bl==-1)	// 无全 0 组：去前导零后正常输出 8 组
	{
		for(int i=0;i<8;++i)
			cout<<(i?":":"")<<g[i];
		cout<<'\n';
		return 0;
	}
	for(int i=0;i<bl;++i)
		cout<<(i?":":"")<<g[i];
	cout<<"::";
	for(int i=br+1;i<8;++i)
		cout<<(i>br+1?":":"")<<g[i];	// 后缀首组前不加冒号，避免 :::
	cout<<'\n';
	return 0;
}
