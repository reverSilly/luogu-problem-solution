// 题目：P2186 小Z的栈函数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

struct Op
{
	int id;
	i64 val;
};

Op ops[mxn];
int opcnt;
char buf[64];

// 操作编号
enum
{
	OP_NUM,OP_POP,OP_INV,OP_DUP,OP_SWP,
	OP_ADD,OP_SUB,OP_MUL,OP_DIV,OP_MOD,OP_END,OP_BAD
};

i64 stk[mxn];
int top;

// 执行一次询问，返回 true 表示机器故障（应输出 ERROR）
bool run(i64 x)
{
	top=0;
	if(llabs(x)>1000000000LL)
		return true;
	stk[top++]=x;
	for(int i=0;i<opcnt;++i)
	{
		int id=ops[i].id;
		if(id==OP_END)
			break;
		if(id==OP_BAD)
			return true;
		if(id==OP_NUM)
		{
			i64 v=ops[i].val;
			if(llabs(v)>1000000000LL)
				return true;
			stk[top++]=v;
		}
		else if(id==OP_POP)
		{
			if(top==0)
				return true;
			--top;
		}
		else if(id==OP_INV)
		{
			if(top==0)
				return true;
			stk[top-1]=-stk[top-1];
			if(llabs(stk[top-1])>1000000000LL)
				return true;
		}
		else if(id==OP_DUP)
		{
			if(top==0)
				return true;
			stk[top++]=stk[top-1];
		}
		else if(id==OP_SWP)
		{
			if(top<2)
				return true;
			swap(stk[top-1],stk[top-2]);
		}
		else
		{
			// ADD/SUB/MUL/DIV/MOD：先弹栈顶 x1，再弹次栈顶 x2，结果为 x2 op x1
			if(top<2)
				return true;
			i64 x1=stk[--top];
			i64 x2=stk[--top];
			i64 res;
			if(id==OP_ADD)
				res=x2+x1;
			else if(id==OP_SUB)
				res=x2-x1;
			else if(id==OP_MUL)
				res=x2*x1;
			else if(id==OP_DIV)
			{
				if(x1==0)
					return true;
				res=x2/x1;
			}
			else
			{
				if(x1==0)
					return true;
				res=x2%x1;
			}
			if(llabs(res)>1000000000LL)
				return true;
			stk[top++]=res;
		}
	}
	// 程序结束时栈中必须恰好剩下一个元素
	if(top!=1)
		return true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	// 读入操作序列，直到 END
	while(cin>>buf)
	{
		string o(buf);
		if(o=="END")
		{
			ops[opcnt].id=OP_END;
			++opcnt;
			break;
		}
		else if(o=="NUM")
		{
			ops[opcnt].id=OP_NUM;
			if(!(cin>>ops[opcnt].val))
				ops[opcnt].id=OP_BAD;
		}
		else if(o=="POP")
			ops[opcnt].id=OP_POP;
		else if(o=="INV")
			ops[opcnt].id=OP_INV;
		else if(o=="DUP")
			ops[opcnt].id=OP_DUP;
		else if(o=="SWP")
			ops[opcnt].id=OP_SWP;
		else if(o=="ADD")
			ops[opcnt].id=OP_ADD;
		else if(o=="SUB")
			ops[opcnt].id=OP_SUB;
		else if(o=="MUL")
			ops[opcnt].id=OP_MUL;
		else if(o=="DIV")
			ops[opcnt].id=OP_DIV;
		else if(o=="MOD")
			ops[opcnt].id=OP_MOD;
		else
		{
			// 非法操作名
			ops[opcnt].id=OP_BAD;
		}
		++opcnt;
	}
	int n;
	cin>>n;
	for(int i=0;i<n;++i)
	{
		i64 x;
		cin>>x;
		if(run(x))
			cout<<"ERROR\n";
		else
			cout<<stk[0]<<'\n';
	}
	return 0;
}
