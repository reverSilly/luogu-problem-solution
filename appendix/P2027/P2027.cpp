// 题目：P2027 BrainFuck
// 解释器：30000 个 8 位有符号内存槽；指令集外的字符一律当作注释忽略
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{30005};
constexpr int memsz{30000};
constexpr int maxstep{1000000};

char code[mxn];
int mem[memsz];
int cl[mxn],op[mxn];//cl:'[' 匹配的 ']',op:']' 匹配的 '['
int stk[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	//读入全部内容
	string raw,line;
	while(getline(cin,line))
	{
		raw+=line;
		raw+='\n';
	}
	//格式：代码$ 输入流 $，'$' 后的空格不属于输入缓冲区
	size_t p=raw.find('$');
	string part=raw.substr(0,p);
	string rest=raw.substr(p+1);
	size_t q=rest.rfind('$');
	size_t blen=(q>=2)?(q-2):0;//去掉结尾的「空格 $」
	string buf=rest.substr(1,blen);

	//抽出指令字符，其余全部是注释
	int len=0;
	for(char c:part)
	{
		if(c=='+'||c=='-'||c==','||c=='.'||c=='>'||c=='<'||c=='['||c==']')
			code[len++]=c;
	}
	code[len]='\0';
	//预处理括号匹配
	int top=0;
	for(int i=0;i<len;++i)
	{
		if(code[i]=='[')
			stk[top++]=i;
		else if(code[i]==']')
		{
			int j=stk[--top];
			cl[j]=i;
			op[i]=j;
		}
	}
	for(int i=0;i<memsz;++i)
		mem[i]=0;
	int ptr=0,pc=0,ip=0,step=0;
	while(pc<len&&step<maxstep)
	{
		++step;
		char c=code[pc];
		if(c=='+')
		{
			//8 位有符号：127+1 回绕成 -128
			if(++mem[ptr]==128)
				mem[ptr]=-128;
		}
		else if(c=='-')
		{
			//-128-1 回绕成 127
			if(--mem[ptr]==-129)
				mem[ptr]=127;
		}
		else if(c=='>')
		{
			++ptr;
		}
		else if(c=='<')
		{
			--ptr;
		}
		else if(c==',')
		{
			int v=(ip<(int)buf.size())?(unsigned char)buf[ip++]:-1;
			if(v>127)
				v-=256;//按 8 位有符号存放
			mem[ptr]=v;
		}
		else if(c=='.')
		{
			cout.put((char)mem[ptr]);
		}
		else if(c=='[')
		{
			if(mem[ptr]==0)
				pc=cl[pc];//跳过整个循环体
		}
		else if(c==']')
		{
			pc=op[pc]-1;//跳回 '['，重新判断条件
		}
		++pc;
	}
	cout<<'\n';
	return 0;
}
