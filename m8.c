#include "m8.h"
int Ph=64,Px=48,Py=128;
double Pya;
static int pin;
float Pbx[2],Pby[2];
static int inL(int x,int y){
	return x<0||y<0||y>640?0:L[x>>3][y>>3]&192;
}
static int inL8(int x,int y){
	return x<0||y<0||y>640?0:L[x>>3][y>>3]&128;
}
static int pcol(int x,int y){
	return Px+8>x&&Px<x+8&&Py+8>y&&Py<y+8;
}
static int tryPxx(){
	int px=Px;
	if(pin){
		if(!inL(Px+8,Py)&&!inL(Px+8,Py+7))Px++;
	}else(!inL(Px-1,Py)&&!inL(Px-1,Py+7))Px--;
	return px!=Px;
}
static uint8_t pt[]={
	0,1,
	5,17,
	6,28,
	6,51,
	6,57,
	12,44,
	16,44,
	21,71,
	25,78,
	0,78,
	39,1,
	82,42,
	74,27,
	57,67,
	67,57,
	55,52,
	73,50,
	77,33,
	77,26,
	71,26,
	49,43,
	1,39,
	26,36,
	8,24,
	16,24,
	52,25,
	18,32,
	58,62,
	52,46,
	78,46,
	62,50,
};
static const uint8_t spk[]={
	3,19,
	4,18,
	1,63,
	2,63,
	3,63,
	4,63,
	5,63,
	6,63,
	7,63,
	8,63,
	9,63,
	10,63,
	13,42,
	13,43,
	15,42,
	15,43,
	42,36,
	42,37,
	19,27,
	21,26,
	23,26,
	31,26,
	35,28,
	36,29,
	41,31,
	27,34,
	30,34,
	33,34,
	36,34,
	39,34,
	55,35,
	54,42,
	57,42,
	58,37,
	59,37,
	63,41,
	63,38,
	64,38,
	70,39,
	74,39,
	69,36,
	76,31,
	45,28,
	46,28,
	60,43,
	63,44,
	68,45,
	72,64,
	55,68,
	51,64,
	56,64,
	64,49,
	70,50,
	77,49,
	76,50,
	73,51,
	56,67,
};
static const uint8_t sav[]={
	80,4,
	6,39,
	4,20,
	1,57,
	12,67,
	67,55,
	56,31
};
static struct goo{uint16_t x;uint8_t y;}goo[]={
	{40,5},{56,5},{72,5},
	{72,26},{112,26},
	{16,27},
	{144,28},
	{424,29},{432,29},{440,29},{448,29},{456,29},
	{32,54},
};
int b1x=96,b1xx,b1y=288,b1h;
float b1yy;
void load(){
	uint32_t p;
	FILE*f=fopen("8","rb");
	if(!f)return;
	fread(&p,4,1,f);
	fclose(f);
	for(int i=0;i<sizeof(pt);i+=2)
		if(p&1<<i/2)pt[i]|=128;
}
void save(){
	uint32_t p;
	for(int i=0;i<sizeof(pt);i+=2)
		if(pt[i]&128)p|=1<<i/2;
	FILE*f=fopen("8","wb");
	fwrite(&p,4,1,f);
	fclose(f);
}
int main(int argc,char**argv){
	sprInit();
	genL();
	load();
	pin=0;
	int T=0;
	for(;;){
		T++;
		int lpin=pin;
		pin=sprInput();
		if(pin){
			if(!lpin){
				Pbx[0]=Px-4;
				Pbx[1]=Px+4;
				Pby[0]=Pby[1]=Py;
			}
		}else(lpin&&(inL(Px,Py+8)||inL(Px+7,Py+8)))Pya=-3.65;
		Pya=fmin(Pya+.1875,4);
		int Pyy=(Pya>0?1:-1),Pxxdone=pin!=lpin||tryPxx();
		for(int i=fabs(ceil(Pya));i>0;i--){
			if(Pya>0?inL(Px,Py+8)||inL(Px+7,Py+8):inL8(Px,Py-1)||inL8(Px+7,Py-1)){
				Pya=0;
				break;
			}
			Py+=Pyy;
			if(!Pxxdone)Pxxdone=tryPxx();
		}
		sprBegin();
		glWhite(Ph/64.);
		drawSpr(Sno,Px,Py);
		glWhite(1);
		for(int i=0;i<sizeof(goo)/sizeof(struct goo);i++){
			int x=goo[i].x;
			if(x==65535)continue;
			int y=(goo[i].y&127)*8,d=goo[i].y&128;
			drawSpr(Sno,x,y);
			if(pcol(x,y)){
				if(Py<y){
					goo[i].x=65535;
					continue;
				}else Ph-=6;
			}
			if(d){
				if(!inL(x+8,y)&&inL(x+8,y+8))goo[i].x++;
				else goo[i].y^=128;
			}else(!inL(x-1,y)&&inL(x-1,y+8))goo[i].x--;
			else goo[i].y^=128;
		}
		for(int i=0;i<sizeof(spk);i+=2){
			int x=spk[i]*8,y=spk[i+1]*8;
			drawSpr(Spk,x+1,y+4);
			if(pcol(x,y)&&(Py+8)>y+4)Ph--;
		}
		if(b1x!=65535){
			if(b1y>288){
				b1y=288;
				b1xx=Px<b1x+12?-1:1;
				b1yy=-3.6;
			}else{
				b1yy=fmin(b1yy+.1875,4);
				b1y+=b1yy;
			}
			b1x+=b1xx;
			if(b1x<64)b1x=64;
			else if(b1x>120)b1x=120;
			if(Px+8>b1x&&Px<b1x+16&&Py<b1y+14&&Py+8>b1y){
				// player collision
			}
			drawSpr(Sbo,b1x+1,b1y+1);
		}
		notex();
		glPt(Px+2+pin,Py+3);
		glPt(Px+4+pin,Py+3);
		for(int i=0;i<sizeof(pt);i+=2){
			if(pt[i]&128)continue;
			glTri(pt[i]*8,pt[i+1]*8,T);
			if(pcol(pt[i]*8,pt[i+1]*8))pt[i]|=128;
		}
		for(int i=0;i<sizeof(sav);i+=2){
			int x=sav[i]*8,y=sav[i+1]*8;
			for(int xx=0;xx<8;xx++)
				for(int yy=0;yy<8;yy++)
					if(!(rnd()&3))glPt(x+xx,y+yy);
			if(pcol(x,y)){
				save();
				Ph=64;
				for(int i=0;i<sizeof(pt);i+=2){
					if(pt[i]&128)continue;
					glLine(x+4,y+4,pt[i]*8+4,pt[i+1]*8+4);
				}
			}
		}
		retex();
		sprFg();
		sprEnd();
	}
}