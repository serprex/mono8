#include "m8.h"
int Ph=10,Px=48,Py=128;
double Vy=128+40,Pya;
static int pin;
float Pbx[2],Pby[2];
static int inL(int x,int y){
	return x<0||y<0||y>640?0:L[x>>3][y>>3]&192;
}
static int inL8(int x,int y){
	return x<0||y<0||y>640?0:L[x>>3][y>>3]&128;
}
static int tryPxx(){
	int px=Px;
	if(pin){
		if(!inL(Px+8,Py)&&!inL(Px+8,Py+7))Px++;
	}else(!inL(Px-1,Py)&&!inL(Px-1,Py+7))Px--;
	return px!=Px;
}
int main(int argc,char**argv){
	sprInit();
	genL();
	pin=0;
	for(;;){
		int lpin=pin;
		pin=sprInput();
		if(pin){
			if(!lpin){
				Pbx[0]=Px-4;
				Pbx[1]=Px+4;
				Pby[0]=Pby[1]=Py;
			}
		}else(lpin&&(inL(Px,Py+8)||inL(Px+7,Py+8)))Pya=-3.6;
		Pya=fmin(Pya+.1875,4);
		int Pyy=(Pya>0?1:-1),Pxxdone=tryPxx();
		for(int i=fabs(ceil(Pya));i>0;i--){
			if(Pya>0?inL(Px,Py+8)||inL(Px+7,Py+8):inL8(Px,Py-1)||inL8(Px+7,Py-1)){
				Pya=0;
				break;
			}
			Py+=Pyy;
			if(!Pxxdone)Pxxdone=tryPxx();
		}
		sprBegin();
		eloop();
		drawSpr(Sno,Px,Py);
		notex();
		glRect(Px+2+pin,Py+3,Px+3+pin,Py+4);
		glRect(Px+4+pin,Py+3,Px+5+pin,Py+4);
		retex();
		sprEnd();
	}
}