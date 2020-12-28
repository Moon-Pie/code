#include <set>
#include <map>
#include <cmath>
#include <queue>
#include <vector>
#include <cstdio>
#include <bitset>
#include <cstring>
#include <iostream>
#include <algorithm>
#include<bits/stdc++.h>
#define line cerr<<"--------------------\n"
#define see(x) cerr<<x<<" "
#define seeln(x) cerr<<x<<endl
#define out(x) cerr<<#x<<" = "<<x<<" "
#define outln(x) cerr<<#x<<" = "<<x<<endl
#define outarr(x,l,r) cerr<<#x"["<<l<<"-"<<r<<"] = "; for (int _i=l;_i<=r;++_i) cerr<<x[_i]<<" ";cerr<<endl;
 
using namespace std;
const int N=2005;
int c[N],v[N],n,m,L;
bitset<N> dp[N],t[N],ans,s,tmp;
vector<bitset<N>> a[N],b[N];
signed main()
{
    cin >> n >> L;
    for(int i=1;i<=n;++i) cin >> c[i] >> v[i];
    cin >> m;
    for(int i=1,t;i<=m;++i) cin>>t, s.set(t);
 
    dp[0].set(0);
    for(int i=1;i<=n;++i)
    {
        // dp[i][j] -> 体积为i，种类数为j，可行
        for(int j=0;j<v[i];++j)
        {
            b[j].resize(0);
            a[j].resize(0);
        }
 
        for(int x,j=0;j<=L;++j)
        {
            x = j%v[i];
            a[x].push_back(dp[j]<<1);
        }
 
        for(int sz,x=0;x<v[i];++x) {
            sz = a[x].size();
            b[x].resize(sz);
            for(int j=sz-1;j>=0;--j) {
                b[x][j] = a[x][j];
                if(j%c[i]!=c[i]-1 && j!=sz-1) b[x][j] |= b[x][j+1];
            }
            for(int j=0;j<sz;++j) {
                if(j%c[i]!=0) a[x][j] |= a[x][j-1];
            }
        }
        for(int x,y,j=L;j>=1;--j) {
            x = j%v[i]; y = j/v[i]-1;
            if(y >= 0) {
                dp[j] |= a[x][y];
            }
            if(y-c[i]+1>=0) {
                dp[j] |= b[x][y-c[i]+1];
            }
 
        }
    }
 
    for(int i=1;i<=L;++i) {
        tmp = (s&dp[i]);
        if(tmp.count()) {
            cout<<i<<" ";
        }
    }
 
    cout<<"\n";
    return 0;
}