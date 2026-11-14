#include<bits/stdc++.h>
using namespace std;
int main(){
	cin.tie(nullptr);cout.tie(nullptr);ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(12);
	for(int j=1;j<=9;++j){
		long double x=powl(10,j);
		long double g=x*log10l(x);
		long double exact=(long double)j*(long double)x;
		cout<<"j="<<j<<" g="<<(double)g<<" exact="<<(double)exact<<" diff="<<(double)(g-exact)<<"\n";
	}
	return 0;
}
