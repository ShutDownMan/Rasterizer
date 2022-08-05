#if !defined(POLYGON_H)
#define POLYGON_H

#define maxHt 800
#define maxWd 600
#define maxVer 10000

#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

typedef struct edgebucket
{
    int ymax;      // max y-coordinate of edge
    float xofymin; // x-coordinate of lowest edge point updated only in aet
    float slopeinverse;
} EdgeBucket;

typedef struct edgetabletup
{
    // the array will give the scanline number
    // The edge table (ET) with edges entries sorted
    // in increasing y and x of the lower end

    int countEdgeBucket; // no. of edgebuckets
    EdgeBucket buckets[maxVer];
} EdgeTableTuple;

extern EdgeTableTuple EdgeTable[maxHt], ActiveEdgeTuple;

typedef struct PolygonPoint
{
    int32_t x;
    int32_t y;
} PolygonPoint;

extern PolygonPoint *polygonPoints;
extern uint32_t polygonPointsCount;

void drawPolygon(void);
void drawPolygonFromFile(char *fileName);

#endif // POLYGON_H