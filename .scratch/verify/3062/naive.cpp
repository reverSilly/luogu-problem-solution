#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int n,A,B,x[2005];i64 f[2005];
int main(){cin>>n>>A>>B;for(int i=0;i<n;++i)cin>>x[i];sort(x,x+n);
for(int i=1;i<=n;++i){f[i]=1e18;for(int j=0;j<i;++j)f[i]=min(f[i],f[j]+2*A+(i64)B*(x[i-1]-x[j]));}
if(f[n]%2==0)cout<<f[n]/2<<'\n';else cout<<f[n]/2<<".5\n";}
