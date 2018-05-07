struct edge_t {
	int u, v;
	ll cap, cost;
	int id; /* use if you want to recover the edges */
	edge_t (int from, int to, ll cap, ll cost, int id = -1)
		:u(from), v(to), cap(cap), cost(cost), id(id) {}
};

const ll inf = 5e18 + 420;
struct mcmf_t {
	vector<edge_t> edges;
	vector< vector<int> > adj; /* adj[i] <- indices of edges incident on i */

	vector<ll> pot;

	mcmf_t(int N)
		: adj(N) {}
	void add_edge(int u, int v, ll cap, ll cost, int id = -1)
	{
		adj[u].push_back(edges.size());
		edges.push_back({u, v, cap, cost, id});
		adj[v].push_back(edges.size());
		edges.push_back({v, u, 0, -cost, id});
	}

	vector<ll> BellmanFord(int source)
	{
		int N = adj.size();
		vector<ll> dist(N, inf);
		dist[source] = 0;

		for (int iter = 0; iter < N; ++iter) {
			for (auto &e : edges) {
				if (e.cap > 0 && dist[e.v] > dist[e.u] + e.cost) {
					dist[e.v] = dist[e.u] + e.cost;
				}
			}
		}

		return dist;
	}

	/* call this once, it return
	 * < max_flow, cost >
	 * runtime: O(VE + FE*log(E)),
	 * where F is max_flow */
	pair<ll, ll> min_cost_max_flow(int source, int sink)
	{
		pot = BellmanFord(source);
		int N = adj.size();
		typedef pair<ll, int> pqnode_t;

		ll flow = 0;
		ll cost = 0;
		while(flow < inf) {
			vector<ll> d(N, inf);
			vector<int> edge_index(N, -1);
			vector<int> popped(N);
			d[source] = 0;

			priority_queue< pqnode_t, vector<pqnode_t>, greater<pqnode_t> > dfq;
			dfq.push({0, source});

			while(!dfq.empty()) {
				int u = dfq.top().second;
				ll w  = dfq.top().first;
				dfq.pop();
				if (popped[u]) {
					continue;
				}
				popped[u] = 1;
				for (auto &eidx : adj[u]) {
					const auto &e = edges[eidx];
					if (e.cap > 0) {
						ll cand = d[e.u] + (e.cost - pot[e.v] + pot[e.u]);
						if (cand < d[e.v]) {
							d[e.v]          = cand;
							edge_index[e.v] = eidx;
							dfq.push({d[e.v], e.v});
						}
					}
				}
			}

			if (edge_index[sink] == -1) {
				return {flow, cost};
			} else {
				/* update the potentials */
				for (int u = 0; u < N; ++u) {
					if (edge_index[u] != -1) {
						pot[u] += d[u];
					}
				}
				/* first compute the amount of flow we can send */
				ll aug_flow = inf;
				for (int u = sink; u != source; ) {
					const auto &e = edges[edge_index[u]];
					aug_flow = min(aug_flow, e.cap);

					u = e.u;
				}

				/* now update total flow */
				flow += aug_flow;

				/* now update graph edges and also add in cost of aug_flow */
				for (int u = sink; u != source; ) {
					edge_t &fwd = edges[edge_index[u]];
					edge_t &rev = edges[edge_index[u] ^ 1];
					rev.cap += aug_flow;
					fwd.cap -= aug_flow;

					cost += aug_flow * fwd.cost;

					u = fwd.u;
				}
			}

		}

		return {flow, cost};
	}
};
