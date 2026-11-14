// 题目：P2636 密码破解者
// 思路：解密 = 各重逆运算按加密逆序复合。
//       栅栏：算每栏长度 cnt[j]=(n-1-j)/L+1 定偏移，把密文段写回 P[g*L+j]。
//       维吉尼亚：表即 c=(m+k)%26，解密 m=(c-k)%26，密钥循环、保大小写。
//       QWE：单表替换，建逆表 qmap 查回，保大小写。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int N,K[mxn],L[mxn];
string S,T[mxn];
string kb="QWERTYUIOPASDFGHJKLZXCVBNM";
int qmap[26];

string raildec(string c,int L)
{
	int n=c.size();
	if(L>n)L=n;				// L>=n 时栅栏是恒等，截到 n 不影响
	string p(n,' ');
	int off=0;
	for(int j=0;j<L;++j)
	{
		int cnt=(n-1-j)/L+1;		// 第 j 栏的字符数
		for(int g=0;g<cnt;++g)
			p[g*L+j]=c[off+g];	// 写回明文位置 g*L+j
		off+=cnt;
	}
	return p;
}

string vigdec(string c,string key)
{
	int n=c.size(),m=key.size();
	string p(n,' ');
	for(int i=0;i<n;++i)
	{
		int sh=tolower(key[i%m])-'a';	// 移位量与密钥大小写无关
		char ch=c[i];
		if(ch>='A'&&ch<='Z')
			p[i]='A'+((ch-'A'-sh)%26+26)%26;
		else if(ch>='a'&&ch<='z')
			p[i]='a'+((ch-'a'-sh)%26+26)%26;
		else
			p[i]=ch;
	}
	return p;
}

string qwedec(string c)
{
	int n=c.size();
	string p(n,' ');
	for(int i=0;i<n;++i)
	{
		char ch=c[i];
		if(ch>='A'&&ch<='Z')
			p[i]='A'+qmap[ch-'A'];
		else if(ch>='a'&&ch<='z')
			p[i]='a'+qmap[ch-'a'];
		else
			p[i]=ch;
	}
	return p;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=0;i<26;++i)
		qmap[kb[i]-'A']=i;		// QWE 逆表
	cin>>N>>S;
	for(int i=0;i<N;++i)
	{
		cin>>K[i];
		if(K[i]==1)cin>>L[i];
		else if(K[i]==2)cin>>T[i];
	}
	for(int i=N-1;i>=0;--i)		// 加密顺序的逆序逐层解密
	{
		if(K[i]==1)S=raildec(S,L[i]);
		else if(K[i]==2)S=vigdec(S,T[i]);
		else S=qwedec(S);
	}
	cout<<S<<'\n';
	return 0;
}
