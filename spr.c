#include "m8.h"
#include "tgen.h"
#include <GLFW/glfw3.h>
#include <winix/time.h>
//#define WID 108
//#define HEI 80
#define WID 800
#define HEI 640
static struct spr{
	int x,y,w,h;
}spr[LSPR]={
	{32,0,8,8},
	{40,0,8,8},
	{48,0,8,8},
	{56,0,8,8},
	{32,8,6,4},
	{32,16,14,14}
};
static GLuint Stx;
static GLFWwindow*wnd;
void notex(){
	glBindTexture(GL_TEXTURE_2D,0);
}
void retex(){
	glBindTexture(GL_TEXTURE_2D,Stx);
}
static void drawRect(int x,int y,int w,int h,float tx,float ty,float tw,float th){
	glTexCoord2f(tx,ty);
	glVertex2i(x,y);
	glTexCoord2f(tx+tw,ty);
	glVertex2i(x+w,y);
	glTexCoord2f(tx+tw,ty+th);
	glVertex2i(x+w,y+h);
	glTexCoord2f(tx,ty+th);
	glVertex2i(x,y+h);
}
void drawSpr(sprid s,int x,int y){
	//x-=Px-WID/2+4;
	//y-=Py-HEI/2+4;
	glBegin(GL_QUADS);
	drawRect(x,y,spr[s].w,spr[s].h,spr[s].x/(float)SW,spr[s].y/(float)SH,spr[s].w/(float)SW,spr[s].h/(float)SH);
	glEnd();
}
void glTri(int x,int y,int T){
	glBegin((T<0)||((T&2)&&!(rnd()&1))?GL_LINE_STRIP:GL_TRIANGLES);
	T=abs(T);
	for(int i=0;i<3;i++)
		glVertex2i(x+4+cos(i*M_PI*2/3+T*M_PI/32)*4,y+4+sin(i*M_PI*2/3+T*M_PI/32)*4);
	glEnd();
}
void glPt(int x,int y){
	glRecti(x,y,x+1,y+1);
}
void glCirc(int x,int y,float r){
	if(!r)return;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	for(float a=0;a<M_PI*2;a+=2/r){
		glVertex2i(x+cosf(a)*r,y+sinf(a)*r);
	}
	glVertex2i(x+r,y);
	glEnd();
}
void glLine(float x1,float y1,float x2,float y2){
	glBegin(GL_QUADS);
	if(fabsf(x1-x2)<fabsf(y1-y2)){
		glVertex2f(x1-.5,y1);
		glVertex2f(x1+.5,y1);
		glVertex2f(x2-.5,y2);
		glVertex2f(x2+.5,y2);
	}else{
		glVertex2f(x1,y1-.5);
		glVertex2f(x1,y1+.5);
		glVertex2f(x2,y2-.5);
		glVertex2f(x2,y2+.5);
	}
	glEnd();
}
void glTriangleLines(float x1,float y1,float x2,float y2,float x3,float y3){
	glLine(x1,y1,x2,y2);
	glLine(x2,y2,x3,y3);
	glLine(x3,y3,x1,y1);
}
void glRect(float x1,float y1,float x2,float y2){
	/*x1-=Px-WID/2+4;
	y1-=Py-HEI/2+4;
	x2-=Px-WID/2+4;
	y2-=Py-HEI/2+4;*/
	glRectf(x1,y1,x2,y2);
}
void glWhite(float a){
	glColor4f(1,1,1,a);
}
void glBlack(float a){
	glColor4f(1,1,1,a);
}
uint32_t rnd(){
	static uint32_t v=0x5A5E4943;
	return v=36969*(v&65535)+(v>>16);
}
static int anyInput;
void onKey(GLFWwindow*wnd,int key,int scancode,int action,int mods){
	static int lastkey;
	if(action==GLFW_PRESS){
		if(key==GLFW_KEY_ESCAPE){
			exit(0);
		}
		lastkey=key;
		anyInput=1;
	}else if(action==GLFW_RELEASE&&lastkey==key){
		anyInput=0;
	}
}
void sprInit(){
	GLubyte Sask[64][64];
	for(int x=0;x<32;x++)
		for(int y=0;y<32;y++){
			int v=255-255*sin(M_PI*x/64)*sin(M_PI*y/64);
			Sask[x][y]=Sask[63-x][y]=Sask[x][63-y]=Sask[63-x][63-y]=v;
		}
	glfwInit();
	wnd=glfwCreateWindow(WID,HEI,0,0,0);
	glfwMakeContextCurrent(wnd);
	glOrtho(0,WID,HEI,0,1,-1);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glGenTextures(2,&Stx);
	glBindTexture(GL_TEXTURE_2D,Stx+1);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE,64,64,0,GL_LUMINANCE,GL_UNSIGNED_BYTE,Sask);
	glBindTexture(GL_TEXTURE_2D,Stx);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,SF,SW,SH,0,SF,GL_UNSIGNED_BYTE,S);
	glfwSetKeyCallback(wnd,onKey);
}
void sprBegin(){
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_QUADS);
	for(int x=0;x<100;x++)
		for(int y=0;y<80;y++){
			if(L[x][y]&&!(L[x][y]&16)){
				int t=L[x][y]&15;
				int xx=x*8,yy=y*8;//int xx=x*8-Px+WID/2-4,yy=y*8-Py+HEI/2-4;
				drawRect(xx,yy,8,8,(t&3)*8/(float)SW,(t>>2)*8/(float)SH,8/(float)SW,8/(float)SH);
			}
		}
	glEnd();
}
void sprFg(){
	glBegin(GL_QUADS);
	for(int x=0;x<100;x++)
		for(int y=0;y<80;y++){
			if(L[x][y]&&(L[x][y]&16)){
				int t=L[x][y]&15;
				int xx=x*8,yy=y*8;//int xx=x*8-Px+WID/2-4,yy=y*8-Py+HEI/2-4;
				drawRect(xx,yy,8,8,(t&3)*8/(float)SW,(t>>2)*8/(float)SH,8/(float)SW,8/(float)SH);
			}
		}
	glEnd();
	/*glBindTexture(GL_TEXTURE_2D,Stx+1);
	glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
	glBlendFunc(GL_ONE,GL_ONE);
	glBegin(GL_QUADS);
	drawRect(0,0,WID,HEI,0,0,1,1);
	glEnd();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	retex();*/
}
void sprEnd(){
	glfwSwapBuffers(wnd);
	endframe(30);
}
int sprInput(){
	glfwPollEvents();
	if(glfwWindowShouldClose(wnd))exit(0);
	return anyInput;
}