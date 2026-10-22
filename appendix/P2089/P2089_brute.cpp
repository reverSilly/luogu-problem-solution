#include<bits/stdc++.h>
using namespace std;

int n;
int cur[11];
vector<array<int,10>> sol;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	// 暴力：三进制枚举 0..3^10-1，把第 j 位看成第 j 种配料的克数-1
	for(int i=0;i<59049;++i)
	{
		int t=i,sum=0;
		for(int j=9;j>=0;--j)
		{
			cur[j]=t%3+1;
			t/=3;
			sum+=cur[j];
		}
		if(sum==n)
			sol.push_back({cur[0],cur[1],cur[2],cur[3],cur[4],cur[5],cur[6],cur[7],cur[8],cur[9]});
	}
	// 三进制高位在前，枚举顺序已经是字典序
	cout<<sol.size()<<'\n';
	for(auto&s:sol)
	{
		for(int j=0;j<10;++j)
			cout<<s[j]<<(j==9?'\n':' ');
	}
	return 0;
}
