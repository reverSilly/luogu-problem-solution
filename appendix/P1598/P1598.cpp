//
// 题目：P1598 [USACO03FEB] 垂直柱状图 Vertical Histogram
// https://www.luogu.com.cn/problem/P1598
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};           // 每行不超过 100 字符 (+5 松弛)

int cnt[26];                     // cnt[c]: 字母 c 在四行中出现的次数
char s[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=0;i<4;++i)
	{
		cin.getline(s,mxn);       // 必须整行读, 行内可能有空格
		for(int j=0;s[j];++j)
			if(s[j]>='A'&&s[j]<='Z')
				++cnt[s[j]-'A'];
	}

	int mx=0;
	for(int i=0;i<26;++i)
		mx=max(mx,cnt[i]);

	for(int k=mx;k>=1;--k)        // 第 k 层, 从最高层往下画
	{
		string row;
		for(int i=0;i<26;++i)
			row+=(cnt[i]>=k?"* ":"  ");
		while(!row.empty()&&row.back()==' ')   // 行末不留多余空格
			row.pop_back();
		cout<<row<<'\n';
	}

	for(int i=0;i<26;++i)         // 最后一行字母表, 用单个空格分隔
	{
		cout<<char('A'+i);
		if(i<25)
			cout<<' ';
	}
	cout<<'\n';
	return 0;
}
