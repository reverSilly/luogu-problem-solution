#include<bits/stdc++.h>
using namespace std;

int main()
{
	mt19937 rn(2024);
	int T=300;
	for(int t=0;t<T;++t)
	{
		int M=1+rn()%5,N=1+rn()%5;
		int M1=1+rn()%3,M2=1+rn()%3;
		while(M2==M1)
			M2=1+rn()%3;
		if(M*N<2)
			--t;
		else
		{
			vector<vector<int>>g(M,vector<int>(N));
			int sp=(int)(rn()%4);
			for(int i=0;i<M;++i)
				for(int j=0;j<N;++j)
				{
					int r=rn()%10;
					g[i][j]=r<sp?0:(r<sp+2?2:1);
				}
			int s=rn()%(M*N),tt=rn()%(M*N);
			while(tt==s)
				tt=rn()%(M*N);
			g[s/N][s%N]=3;
			g[tt/N][tt%N]=4;
			{
				ofstream fout("in.txt");
				fout<<M<<' '<<N<<' '<<M1<<' '<<M2<<'\n';
				for(int i=0;i<M;++i)
				{
					for(int j=0;j<N;++j)
						fout<<g[i][j]<<' ';
					fout<<'\n';
				}
			}
			(void)system("P2385.exe < in.txt > out1.txt");
			(void)system("brute.exe < in.txt > out2.txt");
			ifstream f1("out1.txt"),f2("out2.txt");
			int a=-2,b=-3;
			f1>>a;
			f2>>b;
			if(a!=b)
			{
				cout<<"MISMATCH t="<<t<<" got="<<a<<" want="<<b<<"\n";
				ifstream fin("in.txt");
				string s;
				while(getline(fin,s))
					cout<<s<<'\n';
				return 1;
			}
		}
	}
	cout<<"all ok\n";
	return 0;
}
