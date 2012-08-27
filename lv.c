#include "m8.h"
uint8_t L[100][80];
typedef const uint8_t bcode;
enum{B,W,Ag,Dg,Bm,Wm,Bd,Wd,Sg,Wg,Wsa,Wsb,Bsh,Ite,Bsa,Bsb,FG};
static void wal(int x,int y,int t){L[x][y]=128|t;}
static void wup(int x,int y,int t){L[x][y]=64|t;}
static void til(int x,int y,int t){L[x][y]=t;}
static void diff(bcode*a,int alen){
	int t=a[0];
	for(int i=1;;i++){
		next:;
		int x=a[i],y=a[i+1];
		i+=2;
		do{
			int xlen=a[i]&15,ylen=((a[i]>>4)&7),ysgn=(a[i]&128?-1:1);
			for(int j=0;j<xlen;j++)
				L[x+j][y]=t;
			for(int j=1;j<=ylen;j++)
				L[x+xlen-1][y+j*ysgn]=t;
			x+=xlen;
			y+=ylen*ysgn;
			if(++i==alen)return;
			if(!ylen)goto next;
		}while(a[i]);
	}
}
static void plus(int x,int y,int m){
	wal(x,y,Wd);
	if(!(m&1))wal(x+1,y,Wd);
	if(!(m&2))wal(x-1,y,Wd);
	if(!(m&4))wal(x,y+1,Wd);
	if(!(m&8))wal(x,y-1,Wd);
}
static void range(int t,int x,int y,int dir,int len){
	if(dir)
		memset(L[x]+y,t,len);
	else for(int i=0;i<len;i++)
		L[x+i][y]=t;
}
#define D(xx,yy) ((yy<0?128:0)|(yy>0?yy:-yy)<<4|xx)
#define w 128|
#define u 192|
#define f 16|
void genL(){
	for(int x=0;x<11;x++)wal(x,6,B);
	static bcode topsteps[]={13,15,16,17,22,25,27,29,31,33,35,39,42,44,46,49,53,56,60,64,66,69};
	for(int i=0;i<sizeof(topsteps);i++)
		wal(topsteps[i],6,B);
	static bcode botsteps[]={19,22,25,28,30,32,36,38,40};
	for(int i=0;i<sizeof(botsteps);i++)
		wal(botsteps[i],43,B);
	for(int y=43;y<=64;y++){
		wal(11,y,Wm);
		wal(17,y,Wm);
	}
	for(int x=1;x<=10;x++)
		wal(x,64,Wm);
	for(int y=5;y<=69;y++){
		wal(81,y,B);
		wal(99,y,B);
	}
	for(int y=8;y<=38;y++)
		wal(0,y,Bm);
	for(int y=39;y<=68;y++)
		wal(0,y,Wm);
	for(int x=5;x<=17;x++){
		wal(x,27,FG|Bm);
		wal(x,31,FG|Bm);
		if(x>=7&&x<=16)wal(x,38,Bm);
	}
	for(int y=27;y<=38;y++){
		wal(7,y,Bm);
		if(y<=35)wal(17,y,Bm);
	}
	for(int x=18;x<=46;x++)
		wal(x,35,W);
	for(int x=17;x<=46;x++)
		wal(x,38,x&1?Bsa:Bsb);
	for(int x=18;x<=41;x++)
		for(int y=36;y<=37;y++)
			til(x,y,(x^y)&1?Wsb:FG|Wsa);
	for(int x=18;x<=26;x++)
		for(int y=28-(x-18)/3;y<=31;y++)wal(x,y,Bd);
	static const int8_t catrump[]={2,3,1,2,1,4};
	int catrumpx=27;
	for(int i=0;i<sizeof(catrump);i++){
		for(int j=0;j<catrump[i];j++){
			catrumpx++;
			for(int y=26+i;y<=31;y++)
				wal(catrumpx,y,Bd);
		}
	}
	range(Bd,27,26,1,6);
	for(int x=18;x<=45;x++)
		for(int y=32;y<=34;y++)
			if(y>=33||x<=39)
				til(x,y,Wd);
	range(w Bd,40,32,0,3);
	range(w B,83,25,0,16);
	range(w Wm,11,68,0,8);
	range(w Wsa,3,69,0,9);
	for(int x=2;x<=10;x++){
		if(x!=1)wal(x,46,Wm);
		if(x!=6)wal(x,48,Wm);
		if(x!=10)wal(x,55,Wm);
		if(x!=6&&x!=10)wal(x,57,Wm);
	}
	for(int x=2;x<=15;x+=3){
		wal(x,71,W);
		wal(x,76,W);
	}
	for(int x=0;x<6;x++){
		wal(x*3,73,W);
		wal(x*3+1,74,W);
		wal(x*3,79,W);
		wal(x*3+1,79,W);
	}
	range(w Bsa,19,73,0,6);
	range(w Wsa,19,68,0,4);
	range(w Wsa,26,68,1,12);
	til(12,27,Bm);
	til(12,31,Bm);
	static bcode smz[]={128|Bd,35,26,D(4,1),D(4,1),D(2,1),D(3,1),D(2,1),D(4,-1),D(4,4),D(4,1),D(4,1),D(2,1),D(5,-4),0,
		55,36,D(2,1),D(1,1),D(2,3),D(3,1),D(1,1),D(3,0),
		61,39,D(6,1),D(4,0),
		41,24,D(4,-1),D(3,1),D(3,2),D(4,-1),D(3,-1),D(1,-1),D(3,1),D(2,1),D(5,-3),D(2,-1),D(3,6),0,
		60,30,D(3,1),D(6,-1),D(1,-1),D(2,-1),D(4,-3),D(4,3),0,
		75,36,D(1,-2),D(1,-2),D(1,-3),D(1,-1),0,
		77,40,D(2,-3),D(1,-4)};
	diff(smz,sizeof(smz));
	for(int x=59;x<=63;x++)
		wal(x,45,Wd);
	for(int x=0;x<4;x++)
		for(int y=0;y<4;y++){
			int xx=51+x*4,yy=49+y*4,m=0;
			if(x<3&&y<3)wal(xx,yy,Wd);
			else(!y)m=8;
			else(x==3&&y==3)m=4;
			if(!x){
				m|=2;
				if(!y)m=10;
			}
			plus(xx-2,yy-2,m);
		}
	plus(68,56,0);
	for(int i=0;i<3;i++){
		wal(68+i,54,Wd);
		wal(70,56+i,Wd);
		wal(68-i,58,Wd);
		wal(66,56-i,Wd);
	}
	static bcode mz[]={128|Wd,50,60,D(0,2),D(1,3),D(2,1),D(1,3),D(3,-1),D(2,-1),0,
		53,61,D(2,1),D(1,3),D(2,1),D(3,-1),D(8,1),D(4,-1),D(2,-1),D(2,-4),D(1,-3),0,
		58,63,D(0,-2),D(2,1),D(2,1),D(7,-1),D(3,-1),D(2,-2),0,
		62,60,D(3,1),D(1,0),
		64,52,D(0,-2),D(3,1),D(1,0),
		72,56,D(3,1),D(2,0),
		70,51,D(3,-2),D(3,0),
		73,47,D(2,-1),D(1,0),
		71,45,D(4,-2),D(4,0),
		63,47,D(4,-1),D(3,-3),D(3,0),
		72,50,D(1,-1),D(3,0),
		77,47,D(1,-2),D(1,0),
		51,45,D(3,-2),D(5,0),
		70,51,D(3,1),D(1,1),D(3,-2),D(1,-1),D(1,-3)};
	diff(mz,sizeof(mz));
	for(int i=0;i<7;i++)
		range(w B,84+i*2,62+(i&1),1,2);
	for(int i=0;i<6;i++)
		wal(82+i*2,37+i,B);
	for(int i=0;i<5;i++)
		wal(83+i,57-i,B);
	for(int i=0;i<4;i++)
		wal(98-(i&1),38+i*2,B);
	for(int x=82;x<=97;x++){
		wal(x,68,B);
		if(x&1)wal(x,67,B);
	}
	range(w B,93,51,0,3);
	range(w B,94,49,0,3);
	range(w B,94,53,0,3);
	range(w B,78,69,0,3);
	range(w B,83,65,0,16);
	range(w B,80,73,0,19);
	range(w B,79,77,0,20);
	range(w B,84,33,1,2);
	range(w B,92,59,1,2);
	range(w B,94,58,1,2);
	range(w B,82,71,1,2);
	range(w B,84,71,1,2);
	range(w B,86,69,1,3);
	range(w B,92,69,1,3);
	range(w B,97,49,1,5);
	range(w B,78,70,1,8);
	range(w B,99,70,1,8);
	range(w W,79,75,0,19);
	for(int x=80;x<=98;x++)
		til(x,76,((x^1)&1)<<4|W);
	static bcode obs[]={128|B,86,46,D(3,1),D(2,1),D(2,-1),D(2,-1),D(4,0),
		82,59,D(3,2),D(12,-2),D(1,0),
		87,59,D(4,-2),D(8,0),
		82,30,D(5,1),D(2,1),D(1,0),
		86,28,D(3,1),D(3,0),
		83,35,D(12,-1),D(1,-3),0,
		82,43,D(15,-5),0,
		82,51,D(11,4),D(5,0),
		94,70,D(2,1),D(1,1),0,
		91,32,D(3,-3),D(2,-3)};
	diff(obs,sizeof(obs));
	range(Ite,61,64,0,8);
	static bcode tiles[]={
		18,3,B,
		72,5,B,
		75,3,B,
		79,4,B,
		79,5,B,
		80,5,B,
		44,44,B,
		8,28,Wm,
		9,28,Wm,
		10,28,Wm,
		8,29,Wm,
		9,29,W,
		10,29,Wm,
		8,30,Wm,
		9,30,Wm,
		10,30,Wm,
		11,28,Bm,
		12,28,w f Bm,
		13,28,Bm,
		11,29,Bm,
		12,29,w f Bm,
		13,29,Bm,
		11,30,Bm,
		12,30,w f Bm,
		13,30,Bm,
		14,28,Wm,
		15,28,Wm,
		16,28,Wm,
		14,29,Wm,
		15,29,W,
		16,29,Wm,
		14,30,Wm,
		15,30,Wm,
		16,30,Wm,
		7,25,Bm,
		7,24,Bm,
		7,23,Bm,
		8,23,Bm,
		9,24,Bm,
		10,25,Bm,
		11,25,Bm,
		12,25,w f Bm,
		13,25,Bm,
		14,25,Bm,
		15,24,Bm,
		16,23,Bm,
		17,23,Bm,
		17,24,Bm,
		17,25,Bm,
		//Original home
		5,17,w Ite,
		5,16,B,
		5,17,w Ite,
		5,16,B,
		5,15,w f Bsh,
		6,14,w Ite,
		6,13,B,
		6,12,w f Bsh,
		9,13,w f Bsh,
		10,12,B,
		11,10,B,
		12,8,B,
		13,6,B,
		11,11,w Ite,
		13,7,w Ite,
		6,17,B,
		7,17,B,
		8,17,B,
		9,16,B,
		9,15,B,
		9,14,B,
		//Alternate home
		5+41,17+27,w Ite,
		5+41,16+27,B,
		5+41,17+27,w Ite,
		5+41,16+27,B,
		5+41,15+27,w f Bsh,
		6+41,14+27,w Ite,
		6+41,13+27,B,
		6+41,12+27,w f Bsh,
		9+41,13+27,w f Bsh,
		10+41,12+27,B,
		11+41,10+27,B,
		12+41,8+27,B,
		13+41,6+27,B,
		11+41,11+27,w Ite,
		13+41,7+27,w Ite,
		6+41,17+27,B,
		7+41,17+27,B,
		8+41,17+27,B,
		9+41,16+27,B,
		9+41,15+27,B,
		9+41,14+27,B,
		5,18,Bm,
		4,19,Bm,
		3,20,Bm,
		4,21,Bm,
		4,22,Bm,
		4,23,Bm,
		4,24,Bm,
		5,24,Bm,
		1,25,Bm,
		2,25,Bm,
		5,25,Bm,
		5,25,Bm,
		5,26,Bm,
		5,27,Bm,
		2,28,Bm,
		3,28,Bm,
		4,28,Bm,
		5,28,Bm,
		1,31,Bm,
		2,31,Bm,
		3,31,Bm,
		1,35,Bm,
		3,35,Bm,
		4,35,Bm,
		5,35,Bm,
		6,35,Bm,
		1,38,Bm,
		2,38,Bm,
		3,38,Bm,
		4,38,Bm,
		5,38,Bm,
		5,37,Bm,
		2,44,Wm,
		2,45,Wm,
		6,54,Wm,
		1,52,Wm,
		1,60,Wm,
		10,60,Wm,
		3,53,Wm,
		3,52,Wm,
		4,52,Wm,
		4,51,Wm,
		5,51,Wm,
		5,50,Wm,
		6,50,Wm,
		7,50,Wm,
		7,51,Wm,
		8,51,Wm,
		8,52,Wm,
		9,52,Wm,
		46,36,W,
		46,37,Bsb,
		17,36,w f Bsa,
		17,37,w f Bsb,
		14,41,u Bm,
		44,35,u W,
		44,36,w Wg,
		10,45,w Sg,
		10,57,u Wm,
		10,64,u Wm,
		12,66,Wm,
		14,66,Wm,
		16,66,Wm,
		12,41,Wm,
		12,42,Wm,
		12,43,Wm,
		16,41,Wm,
		16,42,Wm,
		16,43,Wm,
		13,41,Wm,
		15,41,Wm,
		11,65,Wm,
		11,66,Wm,
		11,67,Wm,
		17,76,w Wsb,
		18,76,Wsa,
		19,76,Wsa,
		20,76,w Wsb,
		21,76,Wsa,
		22,76,w Wsb,
		23,77,Wsa,
		24,78,w Wsb,
		25,79,Wsa,
		1,68,Wsa,
		2,68,w Wsb,
		3,68,Wsa,
		19,67,Wsa,
		22,70,Wsa,
		23,68,w Wsb,
		24,68,Wsa,
		25,68,Wsa,
		24,70,Wsa,
		18,73,Wsa,
		24,71,Bsa,
		24,72,Bsa,
		22,71,Bsa,
		21,70,Bsa,
		20,70,Bsa,
		20,71,Bsa,
		17,73,Bsa,
		60,42,Bd,
		78,32,Bd,
		78,33,Bd,
		72,39,Bd,
		72,40,Bd,
		73,40,Bd,
		74,40,Bd,
		75,40,Bd,
		75,39,Bd,
		55,41,Wd,
		56,41,Wd,
		56,42,Wd,
		60,44,Wd,
		51,62,Wd,
		72,59,Wd,
		63,53,Wd,
		64,53,Wd,
		63,57,Wd,
		64,57,Wd,
		64,56,Wd,
		66,49,Wd,
		74,42,Wd,
		69,42,Wd,
		69,47,Wd,
		70,47,Wd,
		73,54,Wd,
		89,6,u B,
		91,6,u B,
		93,7,u B,
		94,9,u B,
		95,11,u B,
		95,13,u B,
		94,15,u B,
		93,17,u B,
		91,18,u B,
		89,18,u B,
		87,17,u B,
		86,15,u B,
		85,13,u B,
		85,11,u B,
		86,9,u B,
		87,7,u B,
		93,40,B,
		94,38,B,
		84,46,B,
		83,46,B,
		83,47,B,
		83,48,B,
		83,49,B,
		84,49,B,
		85,49,B,
		86,49,B,
		86,48,B,
		88,48,B,
		91,45,B,
		88,30,B,
		86,32,B,
		86,34,B,
		96,44,B,
		82,33,B,
		88,50,B,
		90,50,B,
		85,58,B,
		86,57,B,
		87,56,B,
		88,57,B,
		88,54,B,
		89,55,B,
		90,56,B,
		98,61,B,
		97,63,B,
		89,70,B,
		90,71,B,
		88,71,B,
		89,72,B,
		98,70,B,
		98,75,w f W,
		10,46,u Wm,
		25,25,w Bsh,
		32,26,w Bsh,
		37,30,w Bsh,
		38,30,w Bsh,
		39,30,w Bsh,
		40,30,w Bsh,
		35,25,w Bsh,
		39,26,w Bsh,
		41,23,w Bsh,
		47,22,w Bsh,
		51,25,w Bsh,
		59,22,w Bsh,
		60,22,w Bsh,
		66,24,w Bsh,
		72,20,w Bsh,
		78,46,w Bsh,
		78,36,w Bsh,
		75,38,w Bsh,
		78,27,w Bsh,
		68,36,w Bsh,
		58,33,w Bsh,
		57,29,w Bsh,
		50,30,w Bsh,
		54,68,w Bsh,
		50,61,w Bsh,
		53,42,w Bsh,
		65,49,w Bsh,
		64,52,w Bsh,
		64,59,w Bsh,
		66,62,w Bsh,
		72,58,w Bsh,
		76,56,w Bsh,
		70,65,w Bsh,
		60,64,w Bsh,
		62,29,w Bsh,
		64,30,w Bsh,
		68,29,w Bsh,
		71,27,w Bsh,
		79,32,w Bsh,
		67,45,w Bsh,
		68,39,w Bsh,
	};
	assert(!(sizeof(tiles)%3));
	for(int i=0;i<sizeof(tiles);i+=3)
		L[tiles[i]][tiles[i+1]]=128^tiles[i+2];
}