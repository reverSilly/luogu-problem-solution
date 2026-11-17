// 题目：P2957 [USACO09OCT] Barn Echoes G
// 串长 <=80，O(L^2) 枚举长度 + 逐字符比较足够；两个方向都要查
// 谓词不单调（abca/xxabca：k=4 真 k=3 假 k=1 真），不能二分，从大到小扫
#include<bits/stdc++.h>
using namespace std;

string a,b;

bool ovl(const string&x,const string&y,int k)	// x 的长 k 前缀 == y 的长 k 后缀
{
	int m=y.size();
	for(int j=0;j<k;++j)
		if(x[j]!=y[m-k+j])
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;
	int n=a.size(),m=b.size();
	for(int k=min(n,m);k>=1;--k)
		if(ovl(a,b,k)||ovl(b,a,k))
		{
			cout<<k<<'\n';
			return 0;
		}
	cout<<0<<'\n';	// 空串永远是合法重叠，答案至少 0
	return 0;
}
