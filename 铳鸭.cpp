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
#define P pair<int,int>
#define fi first
#define se second
using namespace std;
const int N=2e6+5;
const int inf = 1e9;
char s[N];
int n,k,a[N],w[N];
struct node
{
	int mi,L0,R0,LR;
	int L1,R1,RL0,RL1,tag;
	P LRp,RL0p,RL1p;
}tr[N*4];

#define ls (p<<1)
#define rs (p<<1|1)
#define mid (l+r>>1)

bool umin(int &x,int y) { if(y<x) {x=y;return 1;} return 0; }
int gmin(int x,int y) {return a[x] < a[y] ? x : y; }

void upd(int p,int l,int r)
{
	tr[p].mi = tr[ls].mi < tr[rs].mi ? tr[ls].mi : tr[rs].mi;
	tr[p].LR = inf;
	if(umin(tr[p].LR , tr[ls].LR)) tr[p].LRp = tr[ls].LRp;
	if(umin(tr[p].LR , tr[rs].LR)) tr[p].LRp = tr[rs].LRp;
	if(umin(tr[p].LR , a[tr[ls].L0]+a[tr[rs].R0])) tr[p].LRp = P(tr[ls].L0,tr[rs].R0);

	// L0 , R0 -> 左括号 在 右括号 左边  ()  无限制
	// L1 , R1 -> 左括号 在 右括号 右边  )(  有限制
	// mi 最小的位置

	tr[p].RL0 = inf;
	if(umin(tr[p].RL0 , tr[ls].RL0)) tr[p].RL0p = tr[ls].RL0p;
	if(umin(tr[p].RL0 , tr[rs].RL0)) tr[p].RL0p = tr[rs].RL0p;
	if(umin(tr[p].RL0 , a[tr[ls].R0]+a[tr[rs].L0])) tr[p].RL0p = P(tr[ls].R0,tr[rs].L0);
	tr[p].L0 = gmin(tr[ls].L0 , tr[rs].L0);
	tr[p].R0 = gmin(tr[ls].R0 , tr[rs].R0);

	tr[p].RL1 = inf;
	if(tr[ls].mi == tr[rs].mi) {
		if(umin(tr[p].RL1 , tr[ls].RL1)) tr[p].RL1p = tr[ls].RL1p;
		if(umin(tr[p].RL1 , tr[rs].RL1)) tr[p].RL1p = tr[rs].RL1p;
		if(umin(tr[p].RL1 , a[tr[ls].R1]+a[tr[rs].L1]))
			tr[p].RL1p = P(tr[ls].R1 , tr[rs].L1);
		tr[p].L1 = tr[ls].L1;
		tr[p].R1 = tr[rs].R1;
	}
	else if(tr[ls].mi < tr[rs].mi) {  // 左儿子的最小值 比 右儿子 小
		tr[p].L1 = tr[ls].L1;
		tr[p].R1 = gmin(tr[rs].R0,tr[ls].R1);
		if(umin(tr[p].RL1 , tr[ls].RL1)) tr[p].RL1p = tr[ls].RL1p;
		if(umin(tr[p].RL1 , tr[rs].RL0)) tr[p].RL1p = tr[rs].RL0p;
		if(umin(tr[p].RL1 , a[tr[ls].R1] + a[tr[rs].L0]))
			tr[p].RL1p = P(tr[ls].R1 , tr[rs].L0);
	}
	else {  // 右儿子的最小值 比 左儿子 小
		tr[p].R1 = tr[rs].R1;
		tr[p].L1 = gmin(tr[ls].L0,tr[rs].L1);
		if(umin(tr[p].RL1 , tr[ls].RL0)) tr[p].RL1p = tr[ls].RL0p;
		if(umin(tr[p].RL1 , tr[rs].RL1)) tr[p].RL1p = tr[rs].RL1p;
		if(umin(tr[p].RL1 , a[tr[ls].R0] + a[tr[rs].L1]))
			tr[p].RL1p = P(tr[ls].R0 , tr[rs].L1);
	}

}
void psd(int p) {
	if(tr[p].tag != 0) {
		tr[ls].mi += tr[p].tag;
		tr[rs].mi += tr[p].tag;
		tr[ls].tag += tr[p].tag;
		tr[rs].tag += tr[p].tag;
		tr[p].tag = 0;
	}
}

void build(int p,int l,int r)
{
	if(l > r) return;
	tr[p].tag = 0;
	tr[p].LR = tr[p].RL1 = tr[p].RL0 = inf;	
	if(l == r) {
		tr[p].mi = 0;
		if(s[l] == '(') tr[p].L0 = l;
		else tr[p].R0 = l;
		return ;
	}
	build(ls,l,mid);
	build(rs,mid+1,r);
	upd(p,l,r);
}
void del(int pos,int p=1,int l=1,int r=n)
{
	if(l == r) {
		a[l] = inf;
		return;
	}
	psd(p);
	if(pos <= mid) del(pos,ls,l,mid);
	else del(pos,rs,mid+1,r);
	upd(p,l,r);
}
void mdf(int p,int l,int r,int s,int t,int v) 
{
	if(l > t || r < s) return;
	if(l >=s && r <=t)
	{
		tr[p].tag += v;
		tr[p].mi += v;
		return ;
	}
	psd(p);
	mdf(ls,l,mid,s,t,v);
	mdf(rs,mid+1,r,s,t,v);
	upd(p,l,r);
}

signed main()
{
	scanf("%s",s+1);
	n = strlen(s+1);
	a[0] = inf;
	for(int i=1;i<=n;++i) scanf("%d",&a[i]);

	build(1,1,n);
	cin >> k; k /= 2;
	ll ans = 0;
	int dlt; P cho;
	while(k --> 0) {
		if(tr[1].mi == 0) 
		{
			if(tr[1].LR < tr[1].RL1) {ans += tr[1].LR; cho = tr[1].LRp; dlt=1; }
			else {ans += tr[1].RL1; cho = tr[1].RL1p; dlt=-1; }
		}
		else if(tr[1].mi > 0)
		{
			if(tr[1].LR < tr[1].RL0) {ans += tr[1].LR; cho = tr[1].LRp; dlt=1; }
			else {ans += tr[1].RL0; cho = tr[1].RL0p; dlt=-1; }
		}
		del(cho.fi); del(cho.se);
		mdf(1,1,n,cho.fi,cho.se-1,dlt);
	}

	cout << ans <<"\n";
	return 0;
}