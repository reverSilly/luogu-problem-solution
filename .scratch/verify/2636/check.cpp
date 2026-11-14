#include<bits/stdc++.h>
using namespace std;

string kb="QWERTYUIOPASDFGHJKLZXCVBNM";
int qmap[26];

// ---- 正解的解密函数 ----
string raildec(string c,int L)
{
	int n=c.size();if(L>n)L=n;string p(n,' ');int off=0;
	for(int j=0;j<L;++j){int cnt=(n-1-j)/L+1;for(int g=0;g<cnt;++g)p[g*L+j]=c[off+g];off+=cnt;}
	return p;
}
string vigdec(string c,string key)
{
	int n=c.size(),m=key.size();string p(n,' ');
	for(int i=0;i<n;++i){int sh=tolower(key[i%m])-'a';char ch=c[i];
		if(ch>='A'&&ch<='Z')p[i]='A'+((ch-'A'-sh)%26+26)%26;
		else if(ch>='a'&&ch<='z')p[i]='a'+((ch-'a'-sh)%26+26)%26;else p[i]=ch;}
	return p;
}
string qwedec(string c)
{
	int n=c.size();string p(n,' ');
	for(int i=0;i<n;++i){char ch=c[i];
		if(ch>='A'&&ch<='Z')p[i]='A'+qmap[ch-'A'];
		else if(ch>='a'&&ch<='z')p[i]='a'+qmap[ch-'a'];else p[i]=ch;}
	return p;
}

// ---- 独立的加密函数（按题面定义正向实现）----
string railenc(string p,int L)
{
	int n=p.size();string c;
	for(int j=0;j<L;++j)
		for(int g=0;g*L+j<n;++g)
			c+=p[g*L+j];
	return c;
}
string vigenc(string p,string key)
{
	int n=p.size(),m=key.size();string c(n,' ');
	for(int i=0;i<n;++i){int sh=tolower(key[i%m])-'a';char ch=p[i];
		if(ch>='A'&&ch<='Z')c[i]='A'+(ch-'A'+sh)%26;
		else if(ch>='a'&&ch<='z')c[i]='a'+(ch-'a'+sh)%26;else c[i]=ch;}
	return c;
}
string qweenc(string p)
{
	string c(p.size(),' ');
	for(int i=0;i<(int)p.size();++i){char ch=p[i];
		if(ch>='A'&&ch<='Z')c[i]=kb[ch-'A'];
		else if(ch>='a'&&ch<='z')c[i]=tolower(kb[ch-'a']);else c[i]=ch;}
	return c;
}

int main()
{
	for(int i=0;i<26;++i)qmap[kb[i]-'A']=i;
	mt19937 rn(999);
	int bad=0;
	for(int t=0;t<20000;++t)
	{
		int n=rn()%40;string p;
		for(int i=0;i<n;++i){char c='a'+rn()%26;if(rn()%2)c=toupper(c);p+=c;}
		int L=1+rn()%20;
		string key;int kl=1+rn()%8;for(int i=0;i<kl;++i){char c='a'+rn()%26;if(rn()%2)c=toupper(c);key+=c;}
		// 单层往返
		if(raildec(railenc(p,L),L)!=p){++bad;if(bad<=5)cerr<<"rail bad L="<<L<<" p="<<p<<"\n";}
		if(vigdec(vigenc(p,key),key)!=p){++bad;if(bad<=5)cerr<<"vig bad key="<<key<<" p="<<p<<"\n";}
		if(qwedec(qweenc(p))!=p){++bad;if(bad<=5)cerr<<"qwe bad p="<<p<<"\n";}
		// 三层复合往返：加密顺序 rail->vig->qwe，解密逆序
		string c=qweenc(vigenc(railenc(p,L),key));
		string back=raildec(vigdec(qwedec(c),key),L);
		if(back!=p){++bad;if(bad<=5)cerr<<"compose bad\n";}
	}
	cerr<<"roundtrip mismatch="<<bad<<"\n";
	return 0;
}