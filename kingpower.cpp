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
#define db long double
using namespace std;
const int N = 5e5+5;
const db inf = 1e16;
struct voc
{
    voc(db xx=0,db yy=0) {x=xx;y=yy;}
    db x,y;
    voc operator+(const voc &o)
    const {return voc(x+o.x,y+o.y); }
    voc operator-(const voc &o)
    const {return voc(x-o.x,y-o.y); }
    voc operator*(const db &o)
    const {return voc(x*o,y*o); }
}pot[N],c;
 
db dot(voc v1,voc v2) {return v1.x*v2.x+v1.y*v2.y; }
db crs(voc v1,voc v2) {return v1.x*v2.y-v1.y*v2.x; }
 
int n,H;
db cost[N],R,D;
db dis(voc a,voc b) {return (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y); }
voc mid(voc a,voc b) {return voc((a.x+b.x)/2.0,(a.y+b.y)/2.0); }
 
voc out_cir(voc a,voc b,voc c)
{
    voc v1 = b-a;
    voc v2 = c-b;
    v1 = voc(v1.y,-v1.x);
    v2 = voc(v2.y,-v2.x);
    voc s = mid(a,b);
    voc t = mid(b,c);
    db p = crs(t-s,v2)/crs(v1,v2);
    return s+v1*p;
}
 
signed main()
{
    cin >> n >> H >> R >> D;
    for(int i=0;i<=H;++i) cin >> cost[i];
    for(int i=1;i<=n;++i) scanf("%Lf%Lf",&pot[i].x,&pot[i].y);
    random_shuffle(pot+1,pot+n+1);
 
    db r = 0;
    c = voc(0,0);
    for(int i=1;i<=n;++i) {
        if(dis(c,pot[i]) > r) {
            c = pot[i];
            r = 0;
            for(int j=1;j<=i-1;++j) if(dis(c,pot[j]) > r) {
                c = mid(pot[i],pot[j]);
                r = dis(c,pot[j]);
                for(int k=1;k<=j-1;++k) if(dis(c,pot[k]) > r) {
                    c = out_cir(pot[i],pot[j],pot[k]);
                    r = dis(pot[i],c);
                }
            }
        }
    }
 
    r = sqrt(r);
    int ansh;
    db ans = inf, ansr;
    for(int i=0;i<=H && i<=D;++i) {
        db nr = r - sqrt(D*D-i*i);
        if(nr < R) nr = R;
        if(ans > nr*cost[i]) {
            ans = nr*cost[i];
            ansh = i;
            ansr = nr;
        }
    }
 
    printf("%0.10Lf\n%0.10Lf %0.10Lf %d %0.10Lf\n",ans,c.x,c.y,ansh,ansr);
 
    return 0;
}