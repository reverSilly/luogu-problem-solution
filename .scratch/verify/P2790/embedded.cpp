// 题目：P2790 ccj与zrz之积木问题（UVa101 The Blocks Problem）
// pile[i]=第 i 个位置从底到顶的积木；loc[x]=积木 x 现在哪个位置。
// 关键不变量：非空堆 i 的堆底必是 i ⇒ x 不在位置 x 时位置 x 必空 ⇒ 归位就是 push 进空堆。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{30};

int n,loc[mxn];
vector<int>pile[mxn];

void back(int x)	// 把 x 上方的积木全部归位
{
	auto&v=pile[loc[x]];
	while(v.back()!=x)
	{
		int t=v.back();
		v.pop_back();
		pile[t].push_back(t);	// 位置 t 此时必空（堆底不变量）
		loc[t]=t;
	}
}
void carry(int a,int b)	// 把 a 及其上方整摞搬到 b 所在堆顶
{
	int pa=loc[a],pb=loc[b];
	auto&va=pile[pa];
	int pos=find(va.begin(),va.end(),a)-va.begin();
	for(int i=pos;i<(int)va.size();++i)
	{
		loc[va[i]]=pb;
		pile[pb].push_back(va[i]);
	}
	va.resize(pos);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		pile[i].push_back(i);
		loc[i]=i;
	}
	for(string op,md;cin>>op&&op!="quit";)
	{
		int a,b;
		cin>>a>>md>>b;	// 编号本来就是 0-based，不需要 --
		if(loc[a]==loc[b])
			continue;	// 同一堆（含 a==b）：非法指令，忽略
		if(op=="move")
			back(a);
		if(md=="onto")
			back(b);
		if(op=="move")
		{
			pile[loc[a]].pop_back();	// back(a) 后 a 必是堆顶
			loc[a]=loc[b];
			pile[loc[a]].push_back(a);
		}
		else
			carry(a,b);
	}
	for(int i=0;i<n;++i)
	{
		cout<<i<<':';
		for(int v:pile[i])
			cout<<' '<<v;
		cout<<'\n';
	}
	return 0;
}
