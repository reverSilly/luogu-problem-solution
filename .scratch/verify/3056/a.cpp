// 题目：P3056 [USACO12NOV] Clumsy Cows S
// 扫描：cnt==0 遇 ')' 强制反转（ans++,cnt++）；结束后余额必偶，再翻 cnt/2 个靠右 '('。
// 答案 = ceil(未匹配右括号/2) + ceil(未匹配左括号/2)，下界由"最深坑+总数配平"双约束给出。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

char s[mxn];
int ans,cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	for(int i=0;s[i];++i)
	{
		if(s[i]=='(')
			++cnt;
		else if(cnt)
			--cnt;
		else
			++ans,++cnt;	// 裸右括号：翻成 '('，一次反转对前缀和 +2
	}
	cout<<ans+cnt/2<<'\n';	// n 偶 ⇒ cnt 必偶，整除安全
	return 0;
}
