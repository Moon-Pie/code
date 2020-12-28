#include <bits/stdc++.h>
using namespace std;
#define ll long long
const int N=2e5+5;
int head[N],nxt[N*2],pnt[N*2],val[N*2],E=0;
int n,m;
void adde(int u,int v,int w) {pnt[++E]=v;nxt[E]=head[u];head[u]=E;val[E]=w; }
 
namespace point_div
{
    int S,root,rfa[N],siz[N],vis[N],inx[20][N],far[20][N],rfr[N],igr[20][N],dep[N];
    ll tofa[20][N];
    struct stuff
    {
        stuff(ll a,int b,int c) {len=a;u=b;fa=c;}
        ll len;
        int u,fa;
        bool operator<(const stuff &o) const {
            return len < o.len;
        }
    };
 
    struct node
    {
        node(int a,ll b) {from=a;dis=b;}
        int from,nxt;
        ll dis;
    };
 
    bool cmp_node(node x,node y) { return x.dis>y.dis; }
    vector<node> c[N];
 
    void find_g(int u,int fa) {
        siz[u]=1;
        int ok=1;
        for(int i=head[u];i;i=nxt[i]) {
            int v = pnt[i]; if(v==fa || vis[v]) continue;
            find_g(v,u);
            siz[u]+=siz[v];
            if(siz[v] > S/2) ok=0;
        }
        if((S-siz[u]) > S/2) ok=0;
        if(ok) root=u;
    }
 
    void get_chains(int u,int fa,ll len,int from,int r) {
        c[r].push_back(node(from,len));
        tofa[dep[r]][u] = len;
        for(int i=head[u];i;i=nxt[i]) {
            int v=pnt[i]; if(fa==v || vis[v]) continue;
            get_chains(v,u,len+val[i],from,r);
        }
    }
 
    void prp(int u,int fa) {
        siz[u]=1;
        for(int i=head[u];i;i=nxt[i]) {
            int v=pnt[i]; if(vis[v] || v==fa) continue;
            prp(v,u);
            siz[u]+=siz[v];
        }
    }
 
    void pdiv(int u) {
        prp(u,0);
        c[u].push_back(node(u,0));
 
        for(int i=head[u];i;i=nxt[i]) {
            int v=pnt[i]; if(vis[v]) continue;
            get_chains(v,u,val[i],v,u);
        }
        sort(c[u].begin(),c[u].end(),cmp_node);
 
        for(int i=c[u].size()-1;i>=0;--i) {
            if(i == c[u].size()-1 || c[u][i].from != c[u][i+1].from) c[u][i].nxt = i+1;
            else c[u][i].nxt = c[u][i+1].nxt;
        }
 
        vis[u]=1;
        for(int i=head[u];i;i=nxt[i]) {
            int v=pnt[i]; if(vis[v]) continue;
            S = siz[v]; root=0;
            find_g(v,u);
            rfa[root] = u;
            rfr[root] = v;
            dep[root] = dep[u]+1;
            pdiv(root);
        }
    }
 
    void solve() {
 
        S=n; root=0;
        dep[root] = 0;
        find_g(1,0);
        pdiv(root);
 
        priority_queue<stuff> Q;
 
        for(int u=1;u<=n;++u) {
            inx[0][u]=0;
            Q.push(stuff(c[u][0].dis,u,0));
            far[0][u] = u; igr[1][u] = rfr[u];
            for(int f=rfa[u],j=1;f;f=rfa[f],++j) {
                far[j][u] = f;
                inx[j][u] = 0;
                igr[j+1][u] = rfr[f];
                while(inx[j][u] < (int)c[f].size() && c[f][inx[j][u]].from == igr[j][u])
                    inx[j][u] = c[f][inx[j][u]].nxt;
                if(inx[j][u] >= (int)c[f].size()) continue;
                Q.push(stuff(tofa[dep[f]][u]+c[f][inx[j][u]].dis,u,j));
            }
        }
 
        for(int i=1;i<=m*2;++i) {
            int u = Q.top().u;
            int f = Q.top().fa;
            ll L = Q.top().len;
            Q.pop();
 
            if(i&1) printf("%lld\n",L);
 
            if(f) {
                int fa = far[f][u];
                inx[f][u]++;
                if(inx[f][u] >= (int)c[fa].size()) continue;
                while(inx[f][u] < (int)c[fa].size() && c[fa][inx[f][u]].from == igr[f][u])
                    inx[f][u] = c[fa][inx[f][u]].nxt;
                Q.push(stuff(tofa[dep[fa]][u]+c[fa][inx[f][u]].dis,u,f));
            }
            else {
                inx[0][u]++;
                if(inx[0][u] >= (int)c[u].size()) continue;
                Q.push(stuff(c[u][inx[0][u]].dis,u,0));
            }
        }
    }
}
 
signed main()
{
    cin >> n >> m;
    for(int i=1,u,v,w;i<n;++i) {
        scanf("%d%d%d",&u,&v,&w);
        adde(u,v,w); adde(v,u,w);
    }
    point_div::solve();
    return 0;
}