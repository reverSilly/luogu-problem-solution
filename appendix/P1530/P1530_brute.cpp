#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{200005};

int n,d;
int rems[mxn];                    // 余数序列
char digs[mxn];                   // 对应小数位

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>d;
	int ip=n/d,r=n%d;
	string full=to_string(ip)+".";
	if(r==0)
	{
		full+="0";
		cout<<full<<'\n';
		return 0;
	}
	int lim=2*d+10,len=0,cur=r;    // 生成足够多位的数字
	while(len<lim&&cur!=0)
	{
		rems[len]=cur;
		cur*=10;
		digs[len]=char('0'+cur/d);
		cur%=d;
		len++;
	}
	if(cur==0)                    // 有限小数
	{
		for(int i=0;i<len;++i)
			full+=digs[i];
	}
	else                          // 早先出现过的余数即循环节起点(线性扫描)
	{
		int st=-1,per=-1;
		for(int i=0;i<len&&st==-1;++i)
			for(int j=i+1;j<len;++j)
				if(rems[i]==rems[j])
				{
					st=i;per=j-i;break;
				}
		for(int i=0;i<st;++i)
			full+=digs[i];
		full+='(';
		for(int i=st;i<st+per;++i)
			full+=digs[i];
		full+=')';
	}
	for(int i=0;i<(int)full.size();i+=76)
		cout<<full.substr(i,76)<<'\n';
	return 0;
}
