// 题目：P2799 国王的魔镜
// 镜像 M(X)=X+rev(X) 必为偶数长回文，且原像唯一 = 前半段
// ⇒ 逆向折半链：偶数长且回文就折半，否则停；答案 = 最终长度
// 折半永远取前缀 ⇒ 只需在原串上维护长度 L，总比较次数几何级数 O(n)
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
