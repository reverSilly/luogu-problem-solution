// 题目：P2548 [AHOI2004] 智能探险车
// 思路：模板按因素拆成 N 个独立列约束。第 j 列填单词要求该列 M 个单词全一致，
//       否则只能 '*'。故逐列求共识：全一致输出该词，否则 '*'，即 '*' 最少的唯一模板。
//       边读边维护"首词 + 是否仍一致"，O(NM) 时间、O(N) 空间。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,m;
string word[mxn];
bool same[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	fill_n(same,n,true);
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			string w;
			cin>>w;
			if(i==0)
				word[j]=w;
			else if(w!=word[j])
				same[j]=false;	// 该列出现分歧，只能 '*'
		}
	for(int j=0;j<n;++j)
	{
		if(j)
			cout<<' ';
		cout<<(same[j]?word[j]:string("*"));
	}
	cout<<'\n';
	return 0;
}
