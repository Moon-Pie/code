#include <bits/stdc++.h>
#define ll long long
#define int long long
#define P pair<ll,ll>
#define fi first
#define se second
using namespace std;
const int mod=1e9+7,N=1e5+5;
const ll moh=1019260817,bas=134243;
int qpow(int x,int y,ll p) {
    int res=1;
    for(int i=0;(1ll<<i)<=y;++i,x=1ll*x*x%p) if(y&(1ll<<i)) res=1ll*res*x%p;
    return res;
}
int n,m,far[N],deg[N],onc[N];
ll hac[N],res;
vector<int> cir,g[N];
P c[N];
  
namespace topu
{
    int in[N];
    void rmain()
    {
        memset(onc,0,sizeof(onc));
        queue<int> Q;
        memcpy(in,deg,sizeof(deg));
        for(int i=1;i<=n;++i) if(!in[i]) Q.push(i);
        while(!Q.empty()) {
            int u = Q.front(); Q.pop();
            if(--in[far[u]] == 0) Q.push(far[u]);
        }
        for(int i=1;i<=n;++i) if(in[i]) {
            for(;!onc[i];i=far[i]) onc[i]=1,cir.push_back(i);
            break;
        }
    }
}
  
namespace on_tree
{
    int siz[N],dep[N],sta;
    ll hash[N],dp[N],po[N],inv[N],pi[N];
    bool cmp(int x,int y) {return hash[x] < hash[y];}
    void init() {
        po[0]=1; for(int i=1;i<=n;++i) po[i] = po[i-1]*i%mod;
        pi[0]=bas; for(int i=1;i<=n;++i) pi[i] = pi[i-1]*bas%moh;
        inv[n] = qpow(po[n],mod-2,mod);
        for(int i=n-1;i>=0;--i) inv[i]=inv[i+1]*(i+1)%mod;
    }
    void dfs(int u,int fa)
    {
        siz[u] = 1;
        for(int i=0;i<(int)g[u].size();++i) {
            int v=g[u][i]; if(onc[v] || v==fa) continue;
            dep[v] = dep[u]+1;
            dfs(v,u);
            siz[u] += siz[v];
        }
        sort(g[u].begin(),g[u].end(),cmp);
  
        hash[u] = dep[u]%moh;
        dp[u] = m;
        int cnt=1,lst=-1;
        for(int i=0;i<(int)g[u].size();++i) {
            int v=g[u][i]; if(onc[v]) continue;
            hash[u] = (hash[u]*pi[siz[v]]+hash[v])%moh;
            if(lst > 0) {
                if(siz[v]!=siz[lst] || hash[v] != hash[lst]) {
                    for(int j=0;j<cnt;++j) dp[u] = dp[u]*(dp[lst]+j)%mod;
                    dp[u] = dp[u]*inv[cnt]%mod;
                    cnt=1;
                }
                else ++cnt;
            }
            lst = g[u][i];
        }
  
        if(lst < 0) return ;
        for(int j=0;j<cnt;++j) dp[u] = dp[u]*(dp[lst]+j)%mod;
        dp[u] = dp[u]*inv[cnt]%mod;
    }
    P rmain(int u) {
        dep[u]=1; sta=u;
        dfs(u,0);
        return P(dp[u],hash[u]);
    }
}
  
int gcd(int x,int y) {return !y?x:gcd(y,x%y);}
  
signed main()
{
    cin >> n >> m;
    for(int i=1;i<=n;++i)
    {
        scanf("%lld",&far[i]);
        deg[far[i]]++;
        g[far[i]].push_back(i);
    }
  
    on_tree::init();
    topu::rmain();
    int sz = cir.size();
    for(int i=0;i<sz;++i) {
        c[i] = on_tree::rmain(cir[i]);
    }
  
    int mp;
    for(int i=1;i<=sz;++i) if(sz%i == 0) {
        int ok=1;
        for(int j=0;j<sz;++j) if(c[j].se != c[(j+i)%sz].se) ok=0;
        if(ok) {mp=i;break;}
    }
  
    ll Mul = 1;
    for(int i=0;i<mp;++i) Mul = Mul*c[i].fi%mod;
 
    ll ans = 0;
    for(int i=0;i<sz/mp;++i) {
        int g=gcd(i,sz/mp);
        ans = (ans+qpow(Mul,g,mod))%mod;
    }
  
    cout << ans*qpow(sz/mp,mod-2,mod)%mod <<"\n";
  
    return 0;
}