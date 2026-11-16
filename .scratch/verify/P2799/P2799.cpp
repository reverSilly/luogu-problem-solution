// 题目：P2799 国王的魔镜
// 镜像结果 T=X+rev(X) 必为偶数长回文，且原像唯一 = T 的前半
// 反复「偶数长且回文 ⇒ 折半」，当前串始终是输入的前缀，只需维护长度 L
#include<bits/stdc++.h>
using namespace std;

string s;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	int L=s.size();
	while(L%2==0)
	{
		for(int i=0;i<L/2;++i)
			if(s[i]!=s[L-1-i])
				goto done;	// 不是回文：不可能再拆
		L/=2;
	}
done:;
	cout<<L<<'\n';
	return 0;
}