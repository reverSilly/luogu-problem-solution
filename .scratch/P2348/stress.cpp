#include<bits/stdc++.h>
using namespace std;


typedef pair<string,string> card;

void shuffleFast(vector<card>&A)
{
	int k=A.size(),h=k/2;
	vector<card>B(k);
	for(int d=0;d<k;++d)
	{
		int src=(d&1)?d/2:h+d/2;
		B[d]=A[src];
	}
	A=B;
}

// 独立实现：按题面文字「下半叠、上半叠交替」重建牌堆
void shuffleBrute(vector<card>&A)
{
	int k=A.size(),h=k/2;
	vector<card>B;
	B.reserve(k);
	for(int i=0;i<=h;++i)
	{
		int lo=h+i;
		if(lo<k)
			B.push_back(A[lo]);
		if(i<h)
			B.push_back(A[i]);
	}
	A=B;
}

string outFast(int n,int m,int p,vector<card>A)
{
	if((int)A.size()<4*n)
		return "Error:cards not enough\n";
	for(int t=0;t<m;++t)
		shuffleFast(A);
	string s;
	for(int t=0;t<4;++t)
	{
		int i=(p-1)+t*n;
		s+=A[i].first+' '+A[i].second+'\n';
	}
	return s;
}

string outBrute(int n,int m,int p,vector<card>A)
{
	if((int)A.size()<4*n)
		return "Error:cards not enough\n";
	for(int t=0;t<m;++t)
		shuffleBrute(A);
	string s;
	for(int t=0;t<4;++t)
	{
		int i=(p-1)+t*n;
		s+=A[i].first+' '+A[i].second+'\n';
	}
	return s;
}

int main()
{
	mt19937 rn(20241022);
	const string suit="CHSD",point="A23456789TJQK";
	const string ty[8]={"K","M","P","W","LN","QG","E","L"};

	// 第一部分：置换本身的一致性
	for(int t=1;t<=500;++t)
	{
		int k=rn()%40+1;
		vector<card>A(k);
		for(int i=0;i<k;++i)
			A[i]={to_string(i),"x"};
		vector<card>X=A,Y=A;
		int m=rn()%6+1;
		for(int q=0;q<m;++q)
		{
			shuffleFast(X);
			shuffleBrute(Y);
		}
		if(X!=Y)
		{
			cerr<<"shuffle mismatch at test "<<t<<" k="<<k<<'\n';
			return 1;
		}
	}

	// 第二部分：整体发牌结果
	for(int t=1;t<=2000;++t)
	{
		int n=rn()%6+1;
		int k=rn()%32+1;
		int m=rn()%12+1;
		int p=rn()%n+1;
		vector<card>A(k);
		for(int i=0;i<k;++i)
		{
			string f;
			f+=suit[rn()%4];
			f+=point[rn()%13];
			A[i]={f,ty[rn()%8]};
		}
		string r1=outFast(n,m,p,A),r2=outBrute(n,m,p,A);
		if(r1!=r2)
		{
			cerr<<"mismatch at test "<<t<<": n="<<n<<" k="<<k<<" m="<<m<<" p="<<p<<'\n';
			cerr<<"fast:\n"<<r1<<"brute:\n"<<r2;
			return 1;
		}
	}
	cout<<"all tests passed\n";
	return 0;
}
