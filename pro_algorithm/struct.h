#include"define,include.h"
using namespace std;

struct home
{
	double x,y,z;
	int mapid;
	layer_af *after;
	layer_head *a[100];
	checkout_head *c[100];
	int floor[100];
};

struct layer_head
{
	double x,y,z;
	int floors;
	layer_af *after;
	layer_head(double x,double y,double z,int floors,layer_af *after):x(x),y(y),z(z),floors(floors),after(after){}
};

struct layer_af
{
	double dx,dy,dz;
	int corner;
	int stairid;
	layer_af *after;
	layer_af(double dx,double dy,double dz,int corner,layer_af *after,int stairid=-2):dx(dx),dy(dy),dz(dz),corner(corner),after(after),stairid(stairid){}
};

struct checkout_head
{
	int from,to;//floor:
	layer_af *outcome,*income;
	int stairid;
	double dx,dy,dz;
	checkout_after *after;
	checkout_head(int from,int to,double dx,double dy,double dz,int stairid,layer_af *income,layer_af *outcome,checkout_after *after=NULL):from(from),to(to),dx(dx),dy(dy),dz(dz),stairid(stairid),income(income),outcome(outcome),after(after){}
};

struct checkout_after
{
	double dx,dy,dz;
	int corner;
	checkout_after *after;
	checkout_after(int dx,int dy,int dz,int corner,checkout_after *after):dx(dx),dy(dy),dz(dz),corner(corner),after(after){}
};

struct INTPOINT
{
	short x,y,z;
	INTPOINT(){}
	INTPOINT(int x,int y,int z) :x(x),y(y),z(z){}
};//bfs
