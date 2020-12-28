#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
  
const int N=2e6+5,mod=998244353;
int n,q,a[N];
ll sum[N],s[N],tot;
template <class T> void read(T &x){char c=getchar(); int f=0;while (c<'0'||c>'9') f|=(c=='-'),c=getchar();while (c>='0'&&c<='9') x=(x<<3)+(x<<1)+(c^48),c=getchar();if (f) x=-x;}
  
namespace gen
{
    int s,a,b,p,tp;
    ll lastans=0;
    void init() {cin >>s>>a>>b>>p>>tp;}
    int Rand(){
        s = (s*a%p+(b^(tp*lastans)))%p;
        s = s < 0? -s: s;
        return s;
    }
}
  
namespace RMQ
{
    const int sq = 30;
    int m,st[18][70000],pre[N],suf[N],LG[N];
  
    bool umax(int &x,int y) {
        if(y == -1) return 0;
        else if(x == -1) return x=y, 1;
        else if(a[x] < a[y]) return x=y, 1;
        else return 0;
    }
    void rmain()
    {
        LG[0]=-1; for(int i=1;i<N;++i) LG[i] = LG[i>>1]+1;
 
        memset(st,-1,sizeof(st));
        for(int i=0;i<n;++i) {
            umax(st[0][i/sq],i);
            pre[i] = i;
            if(i%sq != 0) umax(pre[i],pre[i-1]);
        }
        for(int i=n-1;i>=0;--i) {
            suf[i] = i;
            if(i%sq != sq-1) umax(suf[i],suf[i+1]);
        }
        m = (n-1)/sq;
        for(int i=1;i<18;++i) {
            for(int j=0;j+(1<<i)-1<=m;++j) {
                st[i][j] = st[i-1][j];
                umax(st[i][j], st[i-1][j+(1<<(i-1))]);
            }
        }
    }
    int que(int l,int r) {
        int res = -1;
        if(r/sq == l/sq) {
            for(int i=l;i<=r;++i) umax(res,i);
        }
        else {
            int L = l/sq, R = r/sq;
            umax(res, suf[l]);
            umax(res, pre[r]);
            L++; R--;
            if(R < L) return res;
            int x = LG[R-L+1];
            umax(res, st[x][L]);
            umax(res, st[x][R-(1<<x)+1]);
        }
        return res;
    }
}
  
namespace PreSolve
{
    int b[N],st[N],top;
    ll f[2][N],r[N],g[2][N];
  
    void rmain(int op)
    {
        memcpy(b,a,sizeof(a));
        if(op) reverse(b,b+n);
        top = 0;
        for(int i=n-1;i>=0;--i) {
            while(top && b[st[top]] < b[i]) --top;
            r[i] = top? st[top]: n;
            st[++top] = i;
        }
        g[op][n] = 0; for(int i=n-1;i>=0;--i) g[op][i] = 1ll*(r[i]-i)*b[i] + g[op][r[i]];
        f[op][n] = 0; for(int i=n-1;i>=0;--i) f[op][i] = f[op][i+1]+g[op][i];
    }
}
using namespace PreSolve;
  
signed main()
{
    cin >> n >> q;
    for(int i=0;i<n;++i) read(a[i]);
    for(int i=1;i<n;++i) a[i] += a[i-1];
  
    for(int i=0;i<n;++i) {
        s[i] = a[i];
        sum[i] = 1ll*(n-i)*a[i];
        if(i>0) {sum[i]+=sum[i-1];s[i]+=s[i-1];}
    }
  
    gen::init();
    RMQ::rmain();
  
    PreSolve::rmain(0);
    PreSolve::rmain(1);
  
    int l,r,pos;
    ll ans, res = 0;
    while(q --> 0) {
        l = gen::Rand()%n;
        r = gen::Rand()%n;
        if(l > r) swap(l,r);
  
        pos = RMQ::que(l,r);
  
        ans = 0;
        ans += 1ll*(pos-l+1)*(r-pos+1)*a[pos];
        ans += f[0][l] - f[0][pos] - 1ll*(pos-l)*g[0][pos];
        ans += f[1][n-1-r] - f[1][n-1-pos] - 1ll*(r-pos)*g[1][n-1-pos];

        ans -= sum[r];
        tot = s[r];
        if(l>=2) {
            ans += sum[l-2];
            tot -= s[l-2];
        }
  
        ans += tot*(n-r);
        res = ((res+ans)%mod+mod)%mod;
        gen::lastans = ans;
    }
    cout << res <<"\n";
  
    return 0;
}