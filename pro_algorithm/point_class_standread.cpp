#include"struct.h"
/*
   deal data set:
   head?(y=1/n=0) ,corner?(y1=1:corner,y2=2:stairpoint in,y3=3 stairpoint out/n=0)
   if(head->y)
   {
       read : x,y,z
	   init head
	   read_mode=1
   }
   if(corner->y3)
   {
       read :
   }
*/


class point_algorithm
{
public:
	bool init_class();
	bool init_graph(double x,double y,double z);
	bool GetStoredMap();
	bool GetStoredMap(char *storepath);
	bool StoreMap();
	bool ReadData(FILE *fin);
	layer_af *search(int stair,int stairid,int index);
	layer_af *search(int stairid); //home(stairid==-1)

	bool ConvertMap2D(FILE *fin);

	bool FindPath();
	/*bool init();
	bool GetStoredMap(LPCSTR lpcstr);
	bool StoreMap(LPCSTR lpcstr);

	bool ConvertMap(FILE *fin);//from stored map
 	bool ConvertMap(double x,double y,double z,double height,double foot,double left,double right);//new point_plain
	bool ConvertMap(double x,double y,double z,double height,double foot,double left,double right,int corner);//any point ,corner=0:plain, 1:90cross,2:stairs
	bool spec_judge(double x,double y,double z,special_point *secp);

	bool MakeArrayFromStruct();
	bool FindPath();
	*/
private:
	home *plugin;
	int mapisin;    //未达成1
	int mapids;         //未达成1
	char mapname[100]; //需要交互等待用户输入map的名称
	int bfsarray[100][100][100];
	vector<INTPOINT> path;
	queue<INTPOINT> que;
};
bool point_algorithm::init_class()
{
	mapids=0;
	while(!que.empty()) que.pop();
	mapisin=-1;
}
bool point_algorithm::init_graph(double x,double y,double z)
{
	plugin=new home;
	memset(plugin->a,NULL,sizeof plugin->a);
	memset(plugin->c,NULL,sizeof plugin->c);
	plugin->x=x,plugin->y=y,plugin->z=z;
	plugin->after=NULL;
	plugin->mapid=mapids++;
	return true;
}
bool point_algorithm::GetStoredMap()
{
	FILE *fin=freopen(StoragePath,"r",stdin);
	mapisin=1;
	ReadData(fin);
	fclose(fin);
	delete fin;
	return true;
}
bool point_algorithm::GetStoredMap(char* storepath)
{
	FILE *fin=freopen(storepath,"r",stdin);
	mapisin=0;
	ReadData(fin);
	fclose(fin);
	delete fin;
	return fin;
}
layer_af* point_algorithm::search(int stair,int stairid,int index)
{
	int *tmp=lower_bound(plugin->floor,plugin->floor+index,stair);
	int index=tmp-plugin->floor;
	layer_af *next=plugin->a[index]->after;
	while(next!=NULL)
	{
		if(next->stairid==stairid)
		{
			return next;
		}
	}
	return NULL;
}
layer_af* point_algorithm::search(int stairid)
{
	layer_af *next=plugin->after;
	while(next!=NULL)
	{
		if(next->stairid==stairid)
		{
			return next;
		}
	}
	return NULL;
}
bool point_algorithm::StoreMap()
{
	if(mapisin)
	{
		return true;
	}
	else
	{
		strcat(".txt",mapname);
		FILE* fout=freopen(mapname,"w",stdout);
		//dfs
		cout<<"head id="<<mapids<<" 1 0 "<<plugin->x<<' '<<plugin->y<<' '<<plugin->z<<endl;
		layer_af *next=plugin->after;
		while(next!=NULL)
		{
			layer_af tmp=*next;
			cout<<"headafter "<<tmp.dx<< ' ' <<tmp.dy<< ' ' <<tmp.dz<< ' ' <<tmp.corner<<' '<<tmp.stairid<<endl;
			next=tmp.after;
		}
		cout<<"end of head after"<<endl;
		for(int i=0;plugin->a[i]!=NULL;i++)
		{
			layer_head *lhd=plugin->a[i];
			cout<<"layerhead "<< lhd->x<<' ' <<lhd->y<< ' ' <<lhd->z<<' ' <<lhd->floors<<endl;
			layer_af *next=lhd->after;
			while(next!=NULL)
			{
				layer_af tmp=*next;
				cout<<"layerafter "<<tmp.dx<< ' ' <<tmp.dy <<' '<<tmp.dz<<' '<<tmp.corner<<' '<<tmp.stairid<<endl;
				next=tmp.after;
			}
			cout<<"end of one layer"<<endl;
		}
		cout<<"end of layers"<<endl;
		cout<<"stairs"<<endl;
		for(int i=0;plugin->c[i]!=NULL;i++)
		{
			checkout_head *ch=plugin->c[i];
			cout<<"stair "<<ch->from<<' '<<ch->to<<' '<<ch->dx<< ' '<<ch->dy<<' '<<ch->dz<<' '<<ch->stairid<<endl;
			checkout_after *next=ch->after;
			while(next!=NULL)
			{
				checkout_after tmp=*next;
				cout<<"stairafter "<<tmp.dx<<' '<<tmp.dy<<' '<<tmp.dz<< ' '<<tmp.corner<<endl;
				next=tmp.after;
			}
			cout<<"end of a stair"<<endl;
		}
		cout<<"end of stairs"<<endl;
		cout<<"end of the map"<<endl;
		fclose(fout);
		delete fout;
	}
	return true;
}
bool point_algorithm::ReadData(FILE *fin)
{
	bool head,corner;
	double x,y,z;
	int floor=-1;
	if(!scanf("head id=%d %d %d %lf %lf %lf",&mapids,&head,&corner,&x,&y,&z)) return false;										//home
	else
	{
		plugin=new home;
		plugin->x=x,plugin->y=y,plugin->z=z;
		memset(plugin->a,NULL,sizeof plugin->a);
		int indexlf=-1;
		int indexca=-1;
		int stairid;
		int from=-1,to=-1;
		char str[100];
		memset(str,'\0',sizeof str);
		layer_af **nextlf=&plugin->after;
		checkout_after **nextca=NULL;
		while(scanf("%s",&str))
		{
			if(strcmp(str,"end")==0)
			{
				char waste[105];
				cin.getline(waste,100);
				if(strcmp(waste,"of headafter")==0) nextlf=NULL;
				delete waste;
			}
			else if(strcmp(str,"layerhead")==0)
			{
				cin>>x>>y>>z>>floor;
				plugin->a[++indexlf]=new layer_head(x,y,z,floor,NULL);
				plugin->floor[indexlf]=floor;
				nextlf=&plugin->a[indexlf]->after;
			}
			else if(strcmp(str,"layerafter")==0)
			{
				cin>>x>>y>>z>>corner>>stairid;
				*nextlf=new layer_af(x,y,z,corner,NULL,stairid);
				nextlf=&(*nextlf)->after;
			}
			else if(strcmp(str,"headafter")==0)
			{
				cin>>x>>y>>z>>corner;
				*nextlf=new layer_af(x,y,z,corner,NULL,stairid);
				nextlf=&(*nextlf)->after;
			}
			else if(strcmp(str,"stair")==0)
			{
				cin>>from>>to>>x>>y>>z>>stairid;
				layer_af *f=NULL,*t=NULL;
				
				
			}
		}
	}
}