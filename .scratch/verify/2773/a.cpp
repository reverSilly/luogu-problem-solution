// 题目：P2773 漂亮字串
// 思路：约束只限制"极长同字母连续段" ⇒ 串 = O 块与 X 块交替。
//       设 kO 个 O 块、kX 个 X 块，则 |kO-kX|<=1、kO<=CountO、kX<=CountX，
//       长度 <= min(CountO,kO*maxO)+min(CountX,kX*maxX)，两项对块数单调不减，
//       在逐分量最大的可行点 kO=min(CountO,CountX+1),kX=min(CountX,CountO+1) 取最优。
//       答案 = min(CountO,(CountX+1)*maxO)+min(CountX,(CountO+1)*maxX)。
//       max=0 的字母一个也放不了，先把对应 Count 清零。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(i64 a,b,p,q;cin>>a>>b>>p>>q;)
	{
		if(p==0)
			a=0;			// maxO=0：任何只含 O 的子串长度>=1 都违规
		if(q==0)
			b=0;
		cout<<min(a,(b+1)*p)+min(b,(a+1)*q)<<'\n';
	}
	return 0;
}
