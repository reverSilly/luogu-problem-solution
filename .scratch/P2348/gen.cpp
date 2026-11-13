#include<bits/stdc++.h>
using namespace std;

int main()
{
	mt19937 rn(chrono::steady_clock::now().time_since_epoch().count());
	int n=rn()%5+1;
	int k=rn()%30+1;
	int m=rn()%6+1;
	int p=rn()%n+1;
	cout<<n<<' '<<k<<' '<<m<<' '<<p<<'\n';
	string suit="CHSD",rank="A23456789TJQK";
	string ty[]={"K","M","P","W","B","A","S","C","G","D","U","T","N","F","E","I","K","L","+","-","LN","QG","GD","HB","CX","WL","QL","ZB","GS","FT","YS","LG","RW","BZ","TJ","SZ"};
	for(int i=0;i<k;++i)
		cout<<suit[rn()%4]<<rank[rn()%13]<<' '<<ty[rn()%36]<<'\n';
	return 0;
}
