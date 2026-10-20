//
// 题目：P1538 迎春舞会之数字舞蹈
// https://www.luogu.com.cn/problem/P1538
// AC
#include<bits/stdc++.h>
using namespace std;

// 七段管编码：0=左上竖,1=上横,2=右上竖,3=中横,4=左下竖,5=下横,6=右下竖
// '-'=横段亮,'|'=竖段亮,' '=该段不亮。
const char seg[10][8]={
	"|-| |-|",   // 0
	"  |   |",   // 1
	" -|-|- ",   // 2
	" -|- -|",   // 3
	"| |-  |",   // 4
	"|- - -|",   // 5
	"|- -|-|",   // 6
	" -|   |",   // 7
	"|-|-|-|",   // 8
	"|-|- -|"    // 9
};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int k;
	string s;
	cin>>k>>s;
	int h{2*k+2};               // 最后一行的行号
	for(int r=0;r<=h;++r)
	{
		bool horz=(r==0||r==k+1||r==h);     // 横段行:上/中/下
		for(char c:s)
		{
			int d=c-'0';
			if(horz)
			{
				char ch=(r==0)?seg[d][1]:(r==k+1?seg[d][3]:seg[d][5]);
				cout<<' ';
				for(int j=0;j<k;++j)
					cout<<ch;               // k 个横段字符
				cout<<"  ";                 // 右边界空列 + 1 个数字间空格
			}
			else
			{
				char l=(r<=k)?seg[d][0]:seg[d][4];    // 上行取左上,下行取左下
				char rr=(r<=k)?seg[d][2]:seg[d][6];   // 上行取右上,下行取右下
				cout<<l;
				for(int j=0;j<k;++j)
					cout<<' ';
				cout<<rr<<' ';
			}
		}
		cout<<'\n';
	}
	return 0;
}
