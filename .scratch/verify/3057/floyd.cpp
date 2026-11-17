#include<bits/stdc++.h>
using namespace std;
using i64=long long;
int n,A,B;char g[35][35];i64 d[905][905];
int main(){cin>>n>>A>>B;for(int i=0;i<n;++i)cin>>g[i];
int V=n*n;for(int i=0;i<V;++i)for(int j=0;j<V;++j)d[i][j]=i==j?0:1e18;
for(int r=0;r<n;++r)for(int c=0;c<n;++c){int u=r*n+c;
if(r+1<n){int v=u+n,w=g[r+1][c]==g[r][c]?A:B;d[u][v]=d[v][u]=w;}
if(c+1<n){int v=u+1,w=g[r][c+1]==g[r][c]?A:B;d[u][v]=d[v][u]=w;}}
for(int k=0;k<V;++k)for(int i=0;i<V;++i)for(int j=0;j<V;++j)d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
i64 ans=0;for(int i=0;i<V;++i)for(int j=0;j<V;++j)ans=max(ans,d[i][j]);cout<<ans<<'\n';}
