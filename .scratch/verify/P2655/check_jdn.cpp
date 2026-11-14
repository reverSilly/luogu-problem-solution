#include<bits/stdc++.h>
using namespace std;
using i64=long long;
// 独立儒略日(JDN)历法 vs 逐月跳，完全不共享闰年/月长逻辑
bool leap(int y){return (y%4==0&&y%100!=0)||y%400==0;}
int mdays(int y,int mo){static int d[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};if(mo==2&&leap(y))return 29;return d[mo];}
struct dt{int Y,M,D,h,m,s;};
dt monthjump(dt o,int N)
{
	i64 S=(1LL<<(N-1))-1;
	i64 tot=o.h*3600LL+o.m*60+o.s+S;
	int days=tot/86400;i64 rem=tot%86400;
	dt r=o;r.h=rem/3600;r.m=rem%3600/60;r.s=rem%60;
	while(days>0){int dim=mdays(r.Y,r.M);if(r.D+days<=dim){r.D+=days;days=0;}else{days-=dim-r.D+1;r.D=1;++r.M;if(r.M==13){r.M=1;++r.Y;}}}
	return r;
}
int jdn(int y,int m,int d){int a=(14-m)/12,yy=y+4800-a,mm=m+12*a-3;return d+(153*mm+2)/5+365*yy+yy/4-yy/100+yy/400-32045;}
void fromjdn(int j,int&y,int&m,int&d){int a=j+32044,b=(4*a+3)/146097,c=a-146097*b/4,dd=(4*c+3)/1461,e=c-1461*dd/4,mm=(5*e+2)/153;d=e-(153*mm+2)/5+1;m=mm+3-12*(mm/10);y=100*b+dd-4800+mm/10;}
dt jdnmethod(dt o,int N)
{
	i64 S=(1LL<<(N-1))-1;
	i64 tot=o.h*3600LL+o.m*60+o.s+S;
	int days=tot/86400;i64 rem=tot%86400;
	dt r=o;r.h=rem/3600;r.m=rem%3600/60;r.s=rem%60;
	int j=jdn(o.Y,o.M,o.D)+days;
	fromjdn(j,r.Y,r.M,r.D);
	return r;
}
bool eq(dt a,dt b){return a.Y==b.Y&&a.M==b.M&&a.D==b.D&&a.h==b.h&&a.m==b.m&&a.s==b.s;}
int main()
{
	mt19937 rn(4242);
	int mismatch=0;
	for(int t=0;t<50000;++t)
	{
		int N=1+rn()%32;
		dt o;o.Y=rn()%10001;o.M=1+rn()%12;o.D=1+rn()%mdays(o.Y,o.M);o.h=rn()%24;o.m=rn()%60;o.s=rn()%60;
		dt a=monthjump(o,N),b=jdnmethod(o,N);
		if(!eq(a,b)){++mismatch;cout<<"MISMATCH N="<<N<<" o="<<o.Y<<' '<<o.M<<' '<<o.D<<"\n jump "<<a.Y<<' '<<a.M<<' '<<a.D<<"\n jdn  "<<b.Y<<' '<<b.M<<' '<<b.D<<"\n";if(mismatch>5)break;}
	}
	cout<<"done 50000 JDN cases, mismatch="<<mismatch<<'\n';
	return 0;
}