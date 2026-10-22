// 题目：P2058 [NOIP 2016 普及组] 海港
// 暴力：对每艘船，把窗口内所有乘客的国籍塞进 set，数 set 大小
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};
constexpr int mxk{300005};

int t[mxn],kc[mxn],st[mxn];
int nat[mxk];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	int tot=0;
	for(int i=0;i<n;++i)
	{
		cin>>t[i]>>kc[i];
		st[i]=tot;
		for(int j=0;j<kc[i];++j) cin>>nat[tot++];
	}
	for(int i=0;i<n;++i)
	{
		set<int>s;
		for(int p=0;p<=i;++p)
			if(t[p]>t[i]-86400)
				for(int j=st[p];j<st[p]+kc[p];++j) s.insert(nat[j]);
		cout<<s.size()<<'\n';
	}
	return 0;
}
