// 题目：P2394 yyy loves Chemistry I
// 关键引理：round(x/23,8) 只依赖 floor(x*1e9)，且 floor(x*1e9/23)=floor(floor(x*1e9)/23)
// ⇒ 只需要 x 的前 9 位小数，5MB 的尾巴一个字符都不用读
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 ip,fp;
int cnt;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int c;
	while((c=getchar())!=EOF&&c!='.'&&c!='\n'&&c!='\r')
		if(c>='0'&&c<='9')
			ip=ip*10+(c-'0');
	if(c=='.')
		while(cnt<9&&(c=getchar())!=EOF&&c!='\n'&&c!='\r')
			if(c>='0'&&c<='9')
				fp=fp*10+(c-'0'),++cnt;
	while(cnt<9)
		fp*=10,++cnt;		// 小数不足 9 位补 0
	i64 A=ip*1000000000+fp;		// A=floor(x*1e9)，x<=1 ⇒ A<2e9
	i64 q=A/23,d=(q+5)/10;		// q=floor(x/23*1e9)；末位>=5 进位等价于 (q+5)/10
	cout<<d/100000000<<'.'<<setw(8)<<setfill('0')<<d%100000000<<'\n';
	return 0;
}
