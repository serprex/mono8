#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <winix/rand.h>
extern int Ph,Px,Py;
extern double Vy,Pya;
#include "lv.h"
#include "spr.h"
#define case(x) break;case x:;
#define else(x) else if(x)
#define sqr(x) ({__typeof__(x) _x=(x);_x*_x;})
#define max(x,y) ({__typeof__(x) _x=(x);__typeof__(y) _y=(y);_x>_y?_x:_y;})
#define min(x,y) ({__typeof__(x) _x=(x);__typeof__(y) _y=(y);_x<_y?_x:_y;})
#define dst2(x1,y1,x2,y2) (sqr((x1)-(x2))+sqr((y1)-(y2)))
#define dst(x1,y1,x2,y2) sqrt(dst2(x1,y1,x2,y2))
#define dir(x1,y1,x2,y2) atan2((y2)-(y1),(x2)-(x1))