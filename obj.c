#include "m8.h"
static float rnorm(float a){
	a=fmodf(a,M_PI*2);
	return a>M_PI?a-M_PI*2:a;
}
static float rrot(float a,float b,float m){
	float c=rnorm(b-a);
	return fabsf(c)<=m?b:c>0?a+m:a-m;
}
void eloop(){
	/*for(obje*e=E;e<Etop;e++){
		if(0)kille:if(kille(e))break;
		float x=e->x,y=e->y,r;
		switch(e->t){
		}
	}*/
}