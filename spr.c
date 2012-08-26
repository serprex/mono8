#include "m8.h"
#include "tgen.h"
#include <GL/glfw.h>
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
void glCirc(float x,float y,float r){
	if(!r)return;
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x,y);
	for(float a=0;a<M_PI*2;a+=2/r){
		glVertex2f(x+cosf(a)*r,y+sinf(a)*r);
	}
	glVertex2f(x+r,y);
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
void glTriangle(float x1,float y1,float x2,float y2,float x3,float y3){
	glBegin(GL_TRIANGLES);
	glVertex2f(x1,y1);
	glVertex2f(x2,y2);
	glVertex2f(x3,y3);
	glEnd();
}
void glRect(float x1,float y1,float x2,float y2){
	/*x1-=Px-WID/2+4;
	y1-=Py-HEI/2+4;
	x2-=Px-WID/2+4;
	y2-=Py-HEI/2+4;*/
	glRectf(x1,y1,x2,y2);
}
void glWhite(){
	glColor3ub(255,255,255);
}
void glBlack(){
	glColor3ub(0,0,0);
}
uint32_t rnd(){
	static uint32_t v=0x5A5E4943;
	return v=36969*(v&65535)+(v>>16);
}
void sprInit(){
	GLubyte Sask[64][64];
	for(int x=0;x<32;x++)
		for(int y=0;y<32;y++){
			int v=255-255*sin(M_PI*x/64)*sin(M_PI*y/64);
			Sask[x][y]=Sask[63-x][y]=Sask[x][63-y]=Sask[63-x][63-y]=v;
		}
	glfwInit();
	glfwDisable(GLFW_AUTO_POLL_EVENTS);
	glfwOpenWindow(WID*2/2,HEI*2/2,0,0,0,0,0,0,GLFW_WINDOW);
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
void sprEnd(){
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
	glfwSwapBuffers();
	double gT=1./30-glfwGetTime();
	if(gT>0)glfwSleep(gT);
	else printf("sleep %f\n",gT);
	glfwSetTime(0);
}
int sprInput(){
	glfwPollEvents();
	if(glfwGetKey(GLFW_KEY_ESC)||!glfwGetWindowParam(GLFW_OPENED))exit(0);
	return glfwGetKey('Z')||glfwGetKey('X')||glfwGetKey(GLFW_KEY_SPACE);
}