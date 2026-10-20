#include<bits/stdc++.h>
using namespace std;

// 小数据朴素基准:直接维护颜色数组,区间暴力涂色 / 区间暴力数颜色
int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int L,T,O;
	cin>>L>>T>>O;
	vector<int> col(L,1);          // 初始全为 1 号色
	for(int i=0;i<O;++i)
	{
		char ch;
		cin>>ch;
		int a,b,c;
		if(ch=='C')
		{
			cin>>a>>b>>c;
			--a;--b;
			if(a>b)
				swap(a,b);
			for(int j=a;j<=b;++j)
				col[j]=c;
		}
		else
		{
			cin>>a>>b;
			--a;--b;
			if(a>b)
				swap(a,b);
			bool has[35]={};
			int cnt=0;
			for(int j=a;j<=b;++j)
				if(!has[col[j]])
				{
					has[col[j]]=1;
					++cnt;
				}
			cout<<cnt<<'\n';
		}
	}
	return 0;
}
