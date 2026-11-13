#include<bits/stdc++.h>
using namespace std;
int main()
{
	static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	uniform_real_distribution<double>U(-0.5,0.5);
	int mode=rand()%3;
	double a=U(rng),b=U(rng),c=U(rng),d=U(rng);
	if(mode==1){ // 边界点
		double v[4]={a,b,c,d};
		int wh=rand()%4;
		v[wh]=(rand()%2)?0.5:-0.5;
		a=v[0];b=v[1];c=v[2];d=v[3];
	}
	if(mode==2){ // 全取两位小数
		a=round(a*100)/100;b=round(b*100)/100;c=round(c*100)/100;d=round(d*100)/100;
	}
	printf("%.2f %.2f %.2f %.2f\n",a,b,c,d);
	return 0;
}
