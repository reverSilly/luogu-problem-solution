#include<bits/stdc++.h>
using namespace std;
int main(int argc,char**argv){
	int seed=atoi(argv[1]);int vmax=atoi(argv[2]);
	mt19937 rn(seed);
	printf("10\n");
	for(int t=0;t<10;++t){
		vector<int>a(20);
		int tot=0;
		for(int i=0;i<20;++i){a[i]=1+rn()%vmax;tot+=a[i];}
		while(tot%4){int i=rn()%20;a[i]++;tot++;}
		int side=tot/4;
		int mx=*max_element(a.begin(),a.end());
		while(mx>side){ /* shrink the max */ int k=max_element(a.begin(),a.end())-a.begin(); a[k]--; tot--; side=tot/4; if(tot%4){a[k]++;tot++;side=tot/4;} mx=*max_element(a.begin(),a.end()); if(mx<=side)break; }
		printf("20");
		for(int i=0;i<20;++i)printf(" %d",a[i]);
		printf("\n");
	}
	return 0;
}
