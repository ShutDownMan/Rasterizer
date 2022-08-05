#include "polygon.h"

EdgeTableTuple EdgeTable[maxHt], ActiveEdgeTuple;

PolygonPoint *polygonPoints = NULL;
uint32_t polygonPointsCount = 0;

// Scanline Function
void initEdgeTable()
{
    int i;
    for (i = 0; i < maxHt; i++)
    {
        EdgeTable[i].countEdgeBucket = 0;
    }

    ActiveEdgeTuple.countEdgeBucket = 0;
}

void printTuple(EdgeTableTuple *tup)
{
    int j;

    if (tup->countEdgeBucket)
        printf("\nCount %d-----\n", tup->countEdgeBucket);

    for (j = 0; j < tup->countEdgeBucket; j++)
    {
        printf(" %d+%.2f+%.2f",
               tup->buckets[j].ymax, tup->buckets[j].xofymin, tup->buckets[j].slopeinverse);
    }
}

void printTable()
{
    int i, j;

    for (i = 0; i < maxHt; i++)
    {
        if (EdgeTable[i].countEdgeBucket)
            printf("\nScanline %d", i);

        printTuple(&EdgeTable[i]);
    }
}

/* Function to sort an array using insertion sort*/
void insertionSort(EdgeTableTuple *ett)
{
    int i, j;
    EdgeBucket aux;

    for (i = 1; i < ett->countEdgeBucket; i++)
    {
        aux.ymax = ett->buckets[i].ymax;
        aux.xofymin = ett->buckets[i].xofymin;
        aux.slopeinverse = ett->buckets[i].slopeinverse;
        j = i - 1;

        while ((aux.xofymin < ett->buckets[j].xofymin) && (j >= 0))
        {
            ett->buckets[j + 1].ymax = ett->buckets[j].ymax;
            ett->buckets[j + 1].xofymin = ett->buckets[j].xofymin;
            ett->buckets[j + 1].slopeinverse = ett->buckets[j].slopeinverse;
            j = j - 1;
        }
        ett->buckets[j + 1].ymax = aux.ymax;
        ett->buckets[j + 1].xofymin = aux.xofymin;
        ett->buckets[j + 1].slopeinverse = aux.slopeinverse;
    }
}

void storeEdgeInTuple(EdgeTableTuple *receiver, int ym, int xm, float slopInv)
{
    // both used for edgetable and active edge table..
    // The edge tuple sorted in increasing ymax and x of the lower end.
    (receiver->buckets[(receiver)->countEdgeBucket]).ymax = ym;
    (receiver->buckets[(receiver)->countEdgeBucket]).xofymin = (float)xm;
    (receiver->buckets[(receiver)->countEdgeBucket]).slopeinverse = slopInv;

    // sort the buckets
    insertionSort(receiver);

    (receiver->countEdgeBucket)++;
}

void storeEdgeInTable(int x1, int y1, int x2, int y2)
{
    float m, minv;
    int ymaxTS, xwithyminTS, scanline; // ts stands for to store

    if (x2 == x1)
    {
        minv = 0.000000;
    }
    else
    {
        m = ((float)(y2 - y1)) / ((float)(x2 - x1));

        // horizontal lines are not stored in edge table
        if (y2 == y1)
            return;

        minv = (float)1.0 / m;
        printf("\nSlope string for %d %d & %d %d: %f", x1, y1, x2, y2, minv);
    }

    if (y1 > y2)
    {
        scanline = y2;
        ymaxTS = y1;
        xwithyminTS = x2;
    }
    else
    {
        scanline = y1;
        ymaxTS = y2;
        xwithyminTS = x1;
    }
    // the assignment part is done..now storage..
    storeEdgeInTuple(&EdgeTable[scanline], ymaxTS, xwithyminTS, minv);
}

void removeEdgeByYmax(EdgeTableTuple *Tup, int yy)
{
    int i, j;
    for (i = 0; i < Tup->countEdgeBucket; i++)
    {
        if (Tup->buckets[i].ymax == yy)
        {
            printf("\nRemoved at %d", yy);

            for (j = i; j < Tup->countEdgeBucket - 1; j++)
            {
                Tup->buckets[j].ymax = Tup->buckets[j + 1].ymax;
                Tup->buckets[j].xofymin = Tup->buckets[j + 1].xofymin;
                Tup->buckets[j].slopeinverse = Tup->buckets[j + 1].slopeinverse;
            }
            Tup->countEdgeBucket--;
            i--;
        }
    }
}

void updatexbyslopeinv(EdgeTableTuple *Tup)
{
    int i;

    for (i = 0; i < Tup->countEdgeBucket; i++)
    {
        (Tup->buckets[i]).xofymin = (Tup->buckets[i]).xofymin + (Tup->buckets[i]).slopeinverse;
    }
}

void ScanlineFill()
{
    int i, j, x1, ymax1, x2, ymax2, FillFlag = 0, coordCount;

    // we will start from scanline 0;
    // Repeat until last scanline:
    for (i = 0; i < maxHt; i++) // 4. Increment y by 1 (next scan line)
    {

        // 1. Move from ET bucket y to the
        // AET those edges whose ymin = y (entering edges)
        for (j = 0; j < EdgeTable[i].countEdgeBucket; j++)
        {
            storeEdgeInTuple(&ActiveEdgeTuple, EdgeTable[i].buckets[j].ymax, EdgeTable[i].buckets[j].xofymin,
                             EdgeTable[i].buckets[j].slopeinverse);
        }
        printTuple(&ActiveEdgeTuple);

        // 2. Remove from AET those edges for
        // which y=ymax (not involved in next scan line)
        removeEdgeByYmax(&ActiveEdgeTuple, i);

        // sort AET (remember: ET is presorted)
        insertionSort(&ActiveEdgeTuple);

        printTuple(&ActiveEdgeTuple);

        // 3. Fill lines on scan line y by using pairs of x-coords from AET
        j = 0;
        FillFlag = 0;
        coordCount = 0;
        x1 = 0;
        x2 = 0;
        ymax1 = 0;
        ymax2 = 0;
        while (j < ActiveEdgeTuple.countEdgeBucket)
        {
            if (coordCount % 2 == 0)
            {
                x1 = (int)(ActiveEdgeTuple.buckets[j].xofymin);
                ymax1 = ActiveEdgeTuple.buckets[j].ymax;
                if (x1 == x2)
                {
                    /*
                        1. lines are towards top of the intersection
                        2. lines are towards bottom
                        3. one line is towards top and other is towards bottom
                    */
                    if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
                    {
                        x2 = x1;
                        ymax2 = ymax1;
                    }
                    else
                    {
                        coordCount++;
                    }
                }
                else
                {
                    coordCount++;
                }
            }
            else
            {
                x2 = (int)ActiveEdgeTuple.buckets[j].xofymin;
                ymax2 = ActiveEdgeTuple.buckets[j].ymax;

                FillFlag = 0;

                // checking for intersection...
                if (x1 == x2)
                {
                    /*three cases can arrive-
                        1. lines are towards top of the intersection
                        2. lines are towards bottom
                        3. one line is towards top and other is towards bottom
                    */
                    if (((x1 == ymax1) && (x2 != ymax2)) || ((x1 != ymax1) && (x2 == ymax2)))
                    {
                        x1 = x2;
                        ymax1 = ymax2;
                    }
                    else
                    {
                        coordCount++;
                        FillFlag = 1;
                    }
                }
                else
                {
                    coordCount++;
                    FillFlag = 1;
                }

                if (FillFlag)
                {
                    // drawing actual lines...
                    glColor3f(0.0f, 0.7f, 0.0f);

                    glBegin(GL_LINES);
                    glVertex2i(x1, i);
                    glVertex2i(x2, i);
                    glEnd();
                    glFlush();
                }
            }

            j++;
        }

        // 5. For each nonvertical edge remaining in AET, update x for new y
        updatexbyslopeinv(&ActiveEdgeTuple);
    }

    printf("\nScanline filling complete");
}

void drawPolygonFromPoints()
{
    glColor3f(1.0f, 0.0f, 0.0f);
    int i = 0, count = 0, x1, y1, x2, y2;

    if (!polygonPointsCount)
    {
        printf("Nothing to draw\n");
    }

    printf("points = %d\n", polygonPointsCount);

    for (i = count = 0; i < polygonPointsCount; i++, count++)
    {
        if (count > 1)
        {
            x1 = x2;
            y1 = y2;
            count = 1;
        }

        if (count == 0)
        {
            // fscanf(fp, "%d,%d", &x1, &y1);
            x1 = polygonPoints[i].x;
            y1 = polygonPoints[i].y;
        }
        else
        {
            // fscanf(fp, "%d,%d", &x2, &y2);
            x2 = polygonPoints[i].x;
            y2 = polygonPoints[i].y;
            printf("\n%d,%d", x2, y2);

            glBegin(GL_LINES);
            glVertex2i(x1, y1);
            glVertex2i(x2, y2);
            glEnd();
            storeEdgeInTable(x1, y1, x2, y2); // storage of edges in edge table.

            glFlush();
        }
    }
}

void drawPolygon(void)
{
    initEdgeTable();
    drawPolygonFromPoints();
    printf("\nTable");
    printTable();

    ScanlineFill(); // actual calling of scanline filling..
}

void drawPolygonFromFile(char *fileName)
{
    FILE *fp;
    int x, y;
    uint32_t i = 0, allocatedCount = 0;

    fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("Could not open file %s\n", fileName);
        return;
    }

    if (polygonPoints)
    {
        free(polygonPoints);
    }

    allocatedCount = 10;
    polygonPoints = (PolygonPoint *)malloc(sizeof(PolygonPoint) * allocatedCount);

    rewind(fp);
    for (i = 0; !feof(fp); i++)
    {
        if (allocatedCount <= i)
        {
            allocatedCount *= 2;
            polygonPoints = (PolygonPoint *)realloc(polygonPoints, sizeof(PolygonPoint) * allocatedCount);
        }

        fscanf(fp, "%d, %d", &x, &y);
        polygonPoints[i].x = x;
        polygonPoints[i].y = y;
    }

    polygonPoints = (PolygonPoint *)realloc(polygonPoints, sizeof(PolygonPoint) * i);
    polygonPointsCount = i;

    fclose(fp);

    glutDisplayFunc(drawPolygon);

    glutMainLoop();
}

// https://www.geeksforgeeks.org/scan-line-polygon-filling-using-opengl-c/
