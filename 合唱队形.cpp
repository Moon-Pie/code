#include <set>
#include <map>
#include <cmath>
#include <queue>
#include <vector>
#include <bitset>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>
#define ll long long
using namespace std;
const int N=1e6;
ll pr_ph[N],pr_mu[N];
ll l,r;
int not_p[N],p[N],tot=0;
void presol()
{
    pr_mu[1] = pr_ph[1] = 1;
    for(int i=2;i<N;++i)
    {
        if(!not_p[i])
        {
            p[++tot] = i;
            pr_mu[i] = -1;
            pr_ph[i] = i-1;
        }
        for(int j=1;j<=tot && p[j]*i<N;++j)
        {
            not_p[p[j]*i] = 1;
            if(i%p[j] == 0)
            {
                pr_mu[i*p[j]] = 0;
                pr_ph[i*p[j]] = pr_ph[i]*p[j];
                break;
            }
            pr_mu[i*p[j]] = -pr_mu[i];
            pr_ph[i*p[j]] = pr_ph[i]*(p[j]-1);
        }
    }
    for(int i=1;i<N;++i) {
        pr_ph[i] += pr_ph[i-1];
        pr_mu[i] += pr_mu[i-1];
    }
}
// phi * 1 = id
 
map<ll,ll> vp,ph;
ll getF(ll x)
{
    if(x < N) return pr_ph[x];
    ll res = (x+1)*x/2;
    for(ll l=2,r;l<=x;l=r+1) {
        r = x/(x/l);
        res -= getF(x/l)*(r-l+1);
    }
    return res;
}
 
// mu * 1 = e
map<ll,ll> vu,mu;
ll getU(ll x)
{
    if(x < N) return pr_mu[x];
    ll res = 1;
    for(ll l=2,r;l<=x;l=r+1) {
        r = x/(x/l);
        res -= getU(x/l)*(r-l+1);
 
    }
    return res;
}
 
signed main()
{
    presol();
    cin >> l >> r;
    printf("%0.2lf\n",1.0*(getF(r+1)+getU(r+1)-getF(l)-getU(l))/2);
    return 0;
}