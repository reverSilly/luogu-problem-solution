#include<bits/stdc++.h>
using namespace std;
mt19937 rn(1);
int main(){int n=1000000;printf("%d\n",n);for(int i=0;i<n;++i)printf("%d ",(int)(rn()%2000000001)-1000000000);puts("");}
