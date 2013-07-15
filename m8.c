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
	{144,27},
	{424,29},{432,29},{440,29},{448,29},{456,29},
	{32,54},
};
int b1x=96,b1xx,b1y=288,b1h=3,b1hh,b1j;
float b1yy;
int b3x=16,b3xx=2,b3h=3;
void load(){
	uint32_t p;
	uint8_t sv;
	FILE*f=fopen("8","rb");
	if(!f)return;
	fread(&p,4,1,f);
	fread(&sv,1,1,f);
	fclose(f);
	for(int i=0;i<sizeof(pt);i+=2)
		if(p&1<<i/2)pt[i]|=128;
	Px=sav[sv]*8;
	Py=sav[sv+1]*8;
}
void save(uint8_t sv){
	uint32_t p;
	for(int i=0;i<sizeof(pt);i+=2)
		if(pt[i]&128)p|=1<<i/2;
	FILE*f=fopen("8","wb");
	fwrite(&p,4,1,f);
	fwrite(&sv,1,1,f);
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
		if(b1h){
			if(b1y==288&&b1j<0){
				b1j=5;
				b1xx=Px<b1x+12?-1:1;
				b1yy=-3.15;
			}else if(--b1j<0){
				b1yy=fmin(b1yy+.1875,4);
				b1y=fmin(b1y+b1yy,288);
				b1x+=b1xx;
				if(b1x<64)b1x=64;
				else if(b1x>120)b1x=120;
			}
			if(Px+8>b1x&&Px<b1x+16&&Py<b1y+14&&Py+8>b1y){
				Pya=fmin(Pya,-2);
				b1yy=fmax(b1yy,0);
				if(Py>b1y+2)Ph=0;
				else if(!b1hh){
					--b1h;
					b1hh=1;
				}
			}
			drawSpr(Sbo,b1x+1,b1y+1);
			if(b1hh){
				if(Py==296)b1hh=0;
				notex();
				glLine(b1x,b1y,b1x+17,b1y);
				glLine(b1x+17,b1y,b1x+17,b1y+17);
				glLine(b1x+17,b1y+17,b1x,b1y+17);
				glLine(b1x,b1y+17,b1x,b1y);
				retex();
			}
		}
		notex();
		if(b3h){
			b3x+=b3xx;
			if(b3x<8){
				b3x=8;
				b3xx=2;
			}else if(b3x>80){
				b3x=80;
				b3xx=-2;
			}
			if((Px+8>b3x||Px<b3x+8)&&Py>487&&Py<489){
				b3h--;
				Pya=-3.6;
			}
			glTriangleLines(b3x,496,b3x+8,496,b3x+4,496+b3h);
			glTriangleLines(b3x,496,b3x+8,496,b3x+4,496+b3h*3);
			glTriangleLines(b3x,496,b3x+8,496,b3x+4,496+b3h*5);
		}
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
				save(i);
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