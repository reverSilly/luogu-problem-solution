//
// 题目：P1887 乘积最大3
// https://www.luogu.com.cn/problem/P1887
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m;
	cin>>n>>m;
	int q=n/m;
	int r=n%m;
	int c=m-r;
	for(int i=0;i<m;++i){
		if(i){
			cout<<' ';
		}
		if(i<c){
			cout<<q;
		}else{
			cout<<q+1;
		}
	}
	cout<<'\n';
	return 0;
}
