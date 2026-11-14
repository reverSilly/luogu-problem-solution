#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1000005};

char s[mxn];
i64 cz,czz,czzy;	// 已扫前缀中 "z" / "zz" / "zzy" 子序列的个数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>s;
	for(int i=0;s[i];++i)
	{
		if(s[i]=='y')
			czzy+=czz;
		else if(s[i]=='z')
		{
			czz+=cz;	// 先用旧的 cz 与新 z 配对，再自增 cz
			++cz;
		}
	}
	cout<<czzy<<'\n';
	return 0;
}