//
// 题目：P1889 [CEOI 1998] 士兵站队
// https://www.luogu.com.cn/problem/P1889
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;

constexpr int mxp{8};

int n;
int xs[mxp],ys[mxp];
int p[mxp];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n;
	for(int i=0;i<n;++i){
		cin>>xs[i]>>ys[i];
		p[i]=i;
	}
	int minx=xs[0],maxx=xs[0],miny=ys[0],maxy=ys[0];
	for(int i=1;i<n;++i){
		minx=min(minx,xs[i]);
		maxx=max(maxx,xs[i]);
		miny=min(miny,ys[i]);
		maxy=max(maxy,ys[i]);
	}
	int best=1145141919;
	do{
		for(int y=miny;y<=maxy;++y){
			for(int s=minx-n;s<=maxx;++s){
				int cost=0;
				bool bad=false;
				for(int i=0;i<n;++i){
					cost+=abs(xs[p[i]]-(s+i));
					cost+=abs(ys[p[i]]-y);
					if(cost>=best){
						bad=true;
						break;
					}
				}
				if(!bad&&cost<best){
					best=cost;
				}
			}
		}
	}while(next_permutation(p,p+n));
	cout<<best<<'\n';
	return 0;
}
