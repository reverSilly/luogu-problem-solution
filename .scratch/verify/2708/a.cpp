// 题目：P2708 硬币翻转
// 补虚拟尾 1，答案 = 交界总数 Φ：相邻不同对数 + [末位为 0]
// 每次前缀翻转只翻转切口处一个交界（ΔΦ=±1）⇒ 下界 Φ；
// "翻到最右 0"贪心每步 -1 ⇒ 恰好 Φ 步，最优。无需真的模拟翻转。
#include<bits/stdc++.h>
using namespace std;

string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	int n=s.size();
	int ans=(s[n-1]=='0');	// 末位与虚拟尾 1 的交界
	for(int i=0;i+1<n;++i)
		if(s[i]!=s[i+1])
			++ans;
	cout<<ans<<'\n';
	return 0;
}
