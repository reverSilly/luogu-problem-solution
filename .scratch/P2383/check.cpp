#include<bits/stdc++.h>
using namespace std;

int main()
{
	mt19937 rn(1234567);
	int mode=atoi(getenv("MODE")?getenv("MODE"):"0");
	int T=300;
	for(int t=0;t<T;++t)
	{
		int m=(mode==1)?20:(4+rn()%8);		// 小规模 4..11 / 大规模 20
		int mxv=(t%3==0)?4:((t%3==1)?8:2000);
		{
			ofstream fout("in.txt");
			fout<<1<<'\n'<<m;
			for(int i=0;i<m;++i)
				fout<<' '<<(int)(1+rn()%mxv);
			fout<<'\n';
		}
		(void)system("P2383b.exe < in.txt > out1.txt");
		(void)system(mode==1?"dp.exe < in.txt > out2.txt":"brute.exe < in.txt > out2.txt");
		ifstream f1("out1.txt"),f2("out2.txt");
		string a,b;
		f1>>a;
		f2>>b;
		if(a!=b)
		{
			cout<<"MISMATCH t="<<t<<" got="<<a<<" want="<<b<<"  data: ";
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
