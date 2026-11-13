#include<bits/stdc++.h>
using namespace std;
using i64=long long;

string sgnTerm(i64 coef,int deg)		// 生成 "+5x"、"-3" 这样的项（coef 可以为 0）
{
	string s=coef<0?"-":"+";
	i64 v=coef<0?-coef:coef;
	if(deg==0)
		return s+to_string(v);
	if(v==1)
		return s+(deg==1?"x":"x^2");
	return s+to_string(v)+(deg==1?"x":"x^2");
}

int main()
{
	mt19937 rn(20240607);
	int bad=0;
	for(int t=0;t<400;++t)
	{
		i64 p=(i64)(rn()%401)-200,q=(i64)(rn()%401)-200;
		int style=t%4;
		i64 b=p+q,c=p*q;
		string in="x^2";
		if(style==0||style==2)			// 三项全写，允许 +0x / +0
			in+=sgnTerm(b,1)+sgnTerm(c,0);
		else							// 省略系数为 0 的项
		{
			if(b!=0)
				in+=sgnTerm(b,1);
			if(c!=0)
				in+=sgnTerm(c,0);
		}
		if(style==3)
		{
			string sp;
			for(char ch:in)
			{
				if(ch=='+'||ch=='-')
					sp+=' ';
				sp+=ch;
			}
			in=sp;
		}
		ofstream("in.txt")<<in<<'\n';
		(void)system("P2378.exe < in.txt > out.txt");
		ifstream fin("out.txt");
		string out;
		getline(fin,out);
		vector<i64>got;
		bool ok=true;
		size_t i=0;
		auto readnum=[&](i64&v)->bool
		{
			int sg=1;
			if(i<out.size()&&(out[i]=='+'||out[i]=='-'))
			{
				if(out[i]=='-')
					sg=-1;
				++i;
			}
			if(i>=out.size()||!isdigit((unsigned char)out[i]))
				return false;
			i64 r=0;
			while(i<out.size()&&isdigit((unsigned char)out[i]))
			{
				if(r>1000000000000LL)
					return false;
				r=r*10+(out[i]-'0');
				++i;
			}
			v=sg*r;
			return true;
		};
		while(i<out.size()&&ok)
		{
			if(out[i]=='(')
			{
				++i;
				if(i>=out.size()||out[i]!='x')
				{
					ok=false;
					break;
				}
				++i;
				i64 k;
				if(!readnum(k))
				{
					ok=false;
					break;
				}
				if(i>=out.size()||out[i]!=')')
				{
					ok=false;
					break;
				}
				++i;
				got.push_back(k);
			}
			else if(out[i]=='x')
			{
				++i;
				got.push_back(0);
			}
			else
			{
				ok=false;
				break;
			}
			if(i<out.size()&&out[i]=='^')		// ^2
			{
				++i;
				if(i>=out.size()||out[i]!='2')
				{
					ok=false;
					break;
				}
				++i;
				i64 last=got.back();
				got.push_back(last);
			}
		}
		vector<i64>want={p,q};
		sort(want.begin(),want.end());
		vector<i64>srt=got;
		sort(srt.begin(),srt.end());
		bool ordered=true;
		for(size_t k=1;k<got.size();++k)
			if(got[k-1]<got[k])
				ordered=false;
		if(!ok||srt!=want||!ordered)
		{
			++bad;
			cout<<"FAIL input="<<in<<" output="<<out<<"\n";
		}
	}
	cout<<"bad="<<bad<<'\n';
	return 0;
}
