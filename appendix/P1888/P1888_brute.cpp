//
// 题目：P1888 三角函数
// https://www.luogu.com.cn/problem/P1888
// 本地对拍验证通过

#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int gg(int x,int y)
{
	int shift=0;
	while(((x|y)&1)==0){
		x>>=1;
		y>>=1;
		++shift;
	}
	while((x&1)==0){
		x>>=1;
	}
	while(y){
		while((y&1)==0){
			y>>=1;
		}
		if(x>y){
			int t=x;
			x=y;
			y=t;
		}
		y-=x;
	}
	return x<<shift;
}

bool right(i64 a,i64 b,i64 c)
{
	__int128 aa=a,bb=b,cc=c;
	return aa*aa+bb*bb==cc*cc;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 a,b,c;
	cin>>a>>b>>c;
	i64 leg1,leg2,hyp;
	if(right(a,b,c)){
		leg1=a;
		leg2=b;
		hyp=c;
	}else if(right(a,c,b)){
		leg1=a;
		leg2=c;
		hyp=b;
	}else{
		leg1=b;
		leg2=c;
		hyp=a;
	}
	if(leg1>leg2){
		swap(leg1,leg2);
	}
	int g=gg((int)leg1,(int)hyp);
	cout<<leg1/g<<'/'<<hyp/g<<'\n';
	return 0;
}
