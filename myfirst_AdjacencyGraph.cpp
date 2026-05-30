#include<stdio.h>
#include<iostream>
#include<list>
#include<vector>
using namespace std;
class AdjacencyGraph {
private:
	class Vertex;
	class Edge;
	
	Vertex* find_vertex(int vid);
	Edge* find_edge(int eid);

	bool is_adjacent_to(Vertex*u,Vertex*v);
	list<Edge*>incident_edges(Vertex* v);
	bool erase_edge(Edge* e);

	list<Vertex*>vertices_list;
	list<Edge*>edges_list;
public:
	AdjacencyGraph() = default;
	~AdjacencyGraph();

	bool is_adjacent_to(int vid1, int vid2);
	list<int>incident_edges(int vid);

	bool insert_vertex(int vid);
	bool insert_edge(int vid1, int vid2, int eid);
	bool erase_edge(int eid);
	bool erase_vertex(int vid);
};
class AdjacencyGraph::Vertex {
private:
	friend class AdjacencyGraph;
	int vid;
	list<Edge*>my_incident;
	list<Vertex*>::iterator pos_in_vertices;
public:
	Vertex(int vid)
		:vid(vid){ }
};
class AdjacencyGraph::Edge {
private:
	friend class AdjacencyGraph;
	int eid;
	Vertex* src;
	Vertex* dst;
	list<Edge*>::iterator pos_in_edges;
	list<Edge*>::iterator pos_in_src;
	list<Edge*>::iterator pos_in_dst;
	
public:
	Edge(int eid,Vertex*src,Vertex*dst)
		:eid(eid),src(src),dst(dst){ }
	Vertex*opposite(Vertex*v) {
		if (v == src) {
			return dst;
		}
		else if (v == dst) {
			return src;
		}
		else {
			return nullptr;
		}
	}
};

AdjacencyGraph::Vertex* AdjacencyGraph::find_vertex(int vid) {
	for (Vertex* v : vertices_list) {
		if (v->vid == vid) {
			return v;
		}
	}
	return nullptr;
}
AdjacencyGraph::Edge* AdjacencyGraph::find_edge(int eid) {
	for (Edge* e : edges_list) {
		if (e->eid == eid) {
			return e;
		}
	}
	return nullptr;
}

bool AdjacencyGraph::is_adjacent_to(Vertex* u, Vertex* v) {
	if (u == nullptr || v == nullptr) { return false; }
	if (u->my_incident.size() < v->my_incident.size()) {
		for (Edge* e : u->my_incident) {
			if (e->opposite(u) == v) {
				return true;
				}
			}
	}		
	else {	
		for (Edge* e : v->my_incident) {
			if (e->opposite(v) == u) {
				return true;
			}
		}
	}
	return false;


}
list<AdjacencyGraph::Edge*>AdjacencyGraph::incident_edges(Vertex* v) {
	if (v == nullptr) { return {}; }
	list<Edge*>myedges;
	for (Edge* e : v->my_incident) {
		myedges.push_back(e);
	}
	return myedges;
}
bool AdjacencyGraph::erase_edge(Edge* e) {
	if (e == nullptr) { return false; }/*이중해제 방지로직 한쪽 vertex삭제시 opposite와 함께 공유하고있던 edge삭제되는데 opposite삭제하면 이미삭제한edge삭제 못하게함*/
	Vertex* src = e->src;
	Vertex* dst = e->dst;

	src->my_incident.erase(e->pos_in_src);
	dst->my_incident.erase(e->pos_in_dst);

	edges_list.erase(e->pos_in_edges);
	delete e;
	return true;
}
AdjacencyGraph::~AdjacencyGraph() {
	for (Vertex* v : vertices_list) {
		delete v;
	}
	for (Edge* e : edges_list) {
		delete e;
	}
}

bool AdjacencyGraph::is_adjacent_to(int vid1, int vid2) {
	return is_adjacent_to(find_vertex(vid1), find_vertex(vid2));
}
list<int>AdjacencyGraph::incident_edges(int vid) {
	list<Edge*>myedges = incident_edges(find_vertex(vid));
	list<int>result;
	for (Edge* e : myedges) {
		result.push_back(e->eid);
	}
	return result;
}

bool AdjacencyGraph::insert_vertex(int vid) {
	if (find_vertex(vid) != nullptr) { return false; }
	Vertex* new_vertex = new Vertex(vid);
	vertices_list.push_back(new_vertex);
	new_vertex->pos_in_vertices = --vertices_list.end();
	return true;
}
bool AdjacencyGraph::insert_edge(int vid1, int vid2, int eid) {
	Vertex* src = find_vertex(vid1);
	Vertex* dst = find_vertex(vid2);
	if (find_edge(eid) != nullptr) { return false; }
	if (src == nullptr || dst == nullptr) { return false; }
	if (is_adjacent_to(src, dst)) { return false; }

	Edge* New_edge = new Edge(eid, src, dst);
	edges_list.push_back(New_edge);
	New_edge->pos_in_edges = --edges_list.end();

	src->my_incident.push_back(New_edge);
	New_edge->pos_in_src = --src->my_incident.end();

	dst->my_incident.push_back(New_edge);
	New_edge->pos_in_dst = --dst->my_incident.end();
	return true;

}
bool AdjacencyGraph::erase_edge(int eid) {
	return erase_edge(find_edge(eid));
}
bool AdjacencyGraph::erase_vertex(int vid) {
	Vertex* ver = find_vertex(vid);
	if (ver == nullptr) { return false; }
	list<Edge*>myedges = ver->my_incident;
	for (Edge* e :myedges ) {
		erase_edge(e);
	}
	vertices_list.erase(ver->pos_in_vertices);
	delete ver;
	return true;

}