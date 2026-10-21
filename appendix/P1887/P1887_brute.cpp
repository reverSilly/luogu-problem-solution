//
// 题目：P1887 乘积最大3
// https://www.luogu.com.cn/problem/P1887
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{85};

__int128 f[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n,m;
	cin>>n>>m;
	f[0][0]=1;
	for(int j=1;j<=m;++j){
		for(int s=j;s<=n;++s){
			int up=s-(j-1);
			for(int v=1;v<=up;++v){
				__int128 left=f[j-1][s-v];
				if(left==0){
					continue;
				}
				__int128 prod=left*v;
				if(prod>f[j][s]){
					f[j][s]=prod;
				}
			}
		}
	}
	int s=n,j=m;
	int out[mxn],len=0;
	while(j>0){
		int up=s-(j-1);
		for(int v=1;v<=up;++v){
			__int128 left=f[j-1][s-v];
			if(left==0){
				continue;
			}
			if(left*v==f[j][s]){
				out[len++]=v;
				s-=v;
				--j;
				break;
			}
		}
	}
	for(int i=0;i<len;++i){
		if(i){
			cout<<' ';
		}
		cout<<out[i];
	}
	cout<<'\n';
	return 0;
}
