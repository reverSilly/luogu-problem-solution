#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	mt19937 rn(998244353);
	int T=300;
	for(int t=0;t<T;++t)
	{
		int n=2+rn()%9;					// 2..10
		int mxv=(t%3==0)?3:((t%3==1)?20:2000);
		{
			ofstream fout("in.txt");
			fout<<n<<'\n';
			for(int i=0;i<n;++i)
				fout<<(int)(1+rn()%mxv)<<'\n';
		}
		(void)system("P2381.exe < in.txt > out1.txt");
		(void)system("brute.exe < in.txt > out2.txt");
		ifstream f1("out1.txt"),f2("out2.txt");
		i64 a=-1,b=-1;
		f1>>a;
		f2>>b;
		if(a!=b)
		{
			cout<<"MISMATCH t="<<t<<" got="<<a<<" want="<<b<<"\n";
			ifstream fin("in.txt");
			string s;
			while(getline(fin,s))
				cout<<s<<' ';
			cout<<'\n';
			return 1;
		}
	}
	cout<<"all ok\n";
	return 0;
}
