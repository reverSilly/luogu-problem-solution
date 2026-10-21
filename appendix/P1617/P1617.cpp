//
// 题目：P1617 爱与愁的一千个伤心的理由
// https://www.luogu.com.cn/problem/P1617
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

const char*dig[]{"zero","one","two","three","four","five","six","seven","eight","nine"};
const char*tee[]{"ten","eleven","twelve","thirteen","fourteen","fifteen","sixteen","seventeen","eighteen","nineteen"};
const char*ten[]{"","","twenty","thirty","forty","fifty","sixty","seventy","eighty","ninety"};

string w2(int x)                   // 1..99 的英文写法，21..99 用连字符
{
	if(x<10)
		return dig[x];
	if(x<20)
		return tee[x-10];
	if(x%10==0)
		return ten[x/10];
	return string(ten[x/10])+"-"+dig[x%10];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n;
	cin>>n;
	string ans;
	int r=n;
	if(r>=1000)
	{
		ans=string(dig[r/1000])+" thousand";
		r%=1000;
	}
	if(r>=100)
	{
		if(!ans.empty())
			ans+=" ";
		ans+=string(dig[r/100])+" hundred";
		r%=100;
	}
	if(r>0)
	{
		// 末两位不超过 10 才加 and，其余情况只留空格
		if(!ans.empty())
			ans+=(r<=10?" and ":" ");
		ans+=w2(r);
	}
	if(ans.empty())
		ans="zero";
	cout<<ans<<'\n';
	return 0;
}
