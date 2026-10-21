//
// 题目：P1801 黑匣子
// https://www.luogu.com.cn/problem/P1801
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{4005};

int m,n;
int a[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<m;++i)
		cin>>a[i];
	vector<int> box;
	int ptr{0};
	for(int i=0;i<n;++i)
	{
		int uu;
		cin>>uu;
		while(ptr<uu)
			box.push_back(a[ptr++]);
		vector<int> tmp{box};
		nth_element(tmp.begin(),tmp.begin()+i,tmp.end());
		cout<<tmp[i]<<'\n';
	}
	return 0;
}
