#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int N = 1e5+5;
const ll inf = 1e18;
int n,m,w[N],head[N],pnt[N*2],val[N*2],nxt[N*2],E=0;
void adde(int u,int v,int w) {pnt[++E]=v;nxt[E]=head[u];head[u]=E;val[E]=w;}
namespace PreSolve
{
	int cnt,tp[N],siz[N],son[N],dep[N],far[N],dis[N];
	int up[20][N],dw[20][N],a[N],b[N],st[N],L[N],R[N];
	ll upv[20][N],dwv[20][N];
	void dfs1(int u,int fa)
	{
		for(int i=1;i<20;++i) up[i][u] = up[i-1][up[i-1][u]];
		siz[u] = 1; son[u] = 0;
		L[u] = ++cnt;
		for(int i=head[u];i;i=nxt[i]) {
			int v=pnt[i]; if(v==fa) continue;
			far[v] = u;
			dep[v] = dep[u]+1;
			dis[v] = dis[u]+val[i];

			if(w[v] > w[u]) up[0][v] = u;
			else {
				int t = u;
				for(int i=19;i>=0;--i) if(up[i][t] && w[up[i][t]] >= w[v]) t=up[i][t];
				t = up[0][t];
				up[0][v] = t==u? 0: t;
			}
			if(up[0][v]) upv[0][v] = 1ll*(dis[v]-dis[up[0][v]])*w[v];

			dfs1(v,u);
			siz[u] += siz[v];
			if(!son[u] || siz[v] > siz[son[u]]) son[u]=v;
		}
		R[u] = cnt;
	}
	void dfs2(int u,int toop)
	{
		tp[u] = toop;
		a[++cnt] = u; b[u] = cnt;
		if(son[u]) dfs2(son[u],toop);
		for(int i=head[u];i;i=nxt[i]) {
			int v=pnt[i]; if(v==far[u] || v==son[u]) continue;
			dfs2(v,v); 
		}
	}
	int LCA(int u,int v)
	{
		while(tp[u] != tp[v]) {
			if(dep[tp[u]] < dep[tp[v]]) swap(u,v);
			u = far[tp[u]];
		}
		return dep[u]<dep[v]? u: v;
	}
	void rmain()
	{
		cnt=0; dfs1(1,0);
		cnt=0; dfs2(1,1);

		int toop = 0;
		for(int i=n;i>=1;--i) {  // 找到右边第一个比 它 小的位置
			while(toop && w[st[toop]] > w[a[i]]) --toop;
			dw[0][a[i]] = toop? st[toop]: 0;
			dwv[0][a[i]] = toop? 1ll*w[a[i]]*(dis[st[toop]]-dis[a[i]]): 0;
			st[++toop] = a[i];
		}

		for(int i=1;i<20;++i) for(int u=1;u<=n;++u) {
			dw[i][u] = dw[i-1][dw[i-1][u]];
			upv[i][u] = upv[i-1][u] + upv[i-1][up[i-1][u]];
			dwv[i][u] = dwv[i-1][u] + dwv[i-1][dw[i-1][u]];
		}
	}
}
using namespace PreSolve;

int lst;
ll solve_far(int u,int fa)
{
	lst = u;
	ll res = 0;
	for(int i=19;i>=0;--i) if(up[i][u]) {
		int v = up[i][u];
		if(dep[v] >= dep[fa]) {
			res += upv[i][u];
			lst = u = v;
		}
	}
	res += 1ll*w[lst]*(dis[lst]-dis[fa]);
	return res;
}

ll solve_chi(int u,int chi)
{
	ll res = 0;
	int to,ori,t,s,fd;
	for(;;)
	{
		fd = 0;
		for(int i=19;i>=0;--i) if(dw[i][u] && tp[dw[i][u]] == tp[u]) {
			int to = dw[i][u];
			if(L[to] <= L[chi] && R[to] >= R[chi]) {
				if(w[to] >= w[lst]) {
					res += 1ll*w[lst]*(dis[to]-dis[u]);
					u = to;
				}
				else fd = 1;
			}
		}
		if(fd) {
			res += 1ll*w[lst]*(dis[dw[0][u]] - dis[u]);
			lst = u = dw[0][u];
		}
		for(int i=19;i>=0;--i) if(dw[i][u] && tp[dw[i][u]] == tp[u]) {
			int to = dw[i][u];
			if(L[to] <= L[chi] && R[to] >= R[chi]) {
				res += dwv[i][u];
				lst = u = to;
			}
		}
		if(tp[u] == tp[chi])
		{
			res += 1ll*(dis[chi]-dis[u])*w[lst];
			break;
		}
		else 
		{
			t = chi;
			while(tp[t] != tp[u]) s = tp[t], t = far[tp[t]];
			res += 1ll*w[lst]*(dis[s]-dis[u]);
			u = s;
			if(w[u] < w[lst]) lst = u;
		}
	}
	return res;

}

ll que(int s,int t) 
{
	int lca = LCA(s,t);
	ll A = solve_far(s,lca);
	ll B = solve_chi(lca,t);
	return A+B;
}
signed main()
{
	cin >> n >> m;
	for(int i=1;i<=n;++i) scanf("%d",&w[i]);
	for(int i=2,u,v,l;i<=n;++i) {
		scanf("%d%d%d",&u,&v,&l);
		adde(u,v,l); adde(v,u,l);
	}
	PreSolve::rmain();
	int s,t;
	while(m --> 0)
	{
		scanf("%d%d",&s,&t);
		printf("%lld\n",que(s,t));
	}
	return 0;
}