#include<bits/stdc++.h>
using namespace std;
using i64=long long;
mt19937_64 rn(12345);

string term(i64 t)
{
	if(t==0)
		return "x";
	if(t>0)
		return "(x+"+to_string(t)+")";
	return "(x-"+to_string(-t)+")";
}
string expect(i64 a,i64 b)
{
	if(a==b)
	{
		if(a==0)
			return "x^2";
		return term(a)+"^2";
	}
	return term(a)+term(b);
}
string poly(i64 B,i64 C)
{
	string s="x^2";
	if(B!=0)
	{
		if(B==1)
			s+="+x";
		else if(B==-1)
			s+="-x";
		else
			s+=(B>0?"+":"")+to_string(B)+"x";
	}
	if(C!=0)
	{
		if(C>0)
			s+="+"+to_string(C);
		else
			s+=to_string(C);
	}
	return s;
}
string run(string in)
{
	{
		ofstream f("in.txt");
		f<<in<<"\n";
	}
	system("P2378.exe < in.txt > out.txt");
	string out;
	{
		ifstream f("out.txt");
		getline(f,out);
	}
	while(!out.empty()&&(out.back()=='\r'||out.back()=='\n'))
		out.pop_back();
	return out;
}
int main()
{
	int T=600,bad=0;
	for(int tc=0;tc<T;++tc)
	{
		i64 a,b;
		if(tc%7==0)
		{
			a=(i64)(rn()%2001)-1000;
			b=a;
		}
		else
		{
			a=(i64)(rn()%2001)-1000;
			b=(i64)(rn()%2001)-1000;
			if(a<b)
				swap(a,b);
			if(a==b)
				++a;
		}
		i64 B=a+b,C=a*b;
		string in=poly(B,C),out;
		if(tc%3==0)
		{
			string sp;
			for(char c:in)
			{
				if((c=='+'||c=='-')&&sp.size()>1&&rn()%2)
					sp+=' ';
				sp+=c;
			}
			in=sp;
		}
		out=run(in);
		string ex=expect(a,b);
		if(out!=ex)
		{
			++bad;
			cerr<<"MISMATCH in="<<in<<" a="<<a<<" b="<<b<<" out="<<out<<" expected="<<ex<<"\n";
			if(bad>5)
				break;
		}
	}
	cout<<(bad?"BAD":"OK")<<" "<<T<<" tests\n";
	return 0;
}
