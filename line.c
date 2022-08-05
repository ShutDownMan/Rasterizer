#include "line.h"

void drawLineFromInput(int x1, int y1, int x2, int y2)
{
    if (x1 > x2)
    {
        drawLineFromInput(x2, y2, x1, y1);
        return;
    }

    int slope;
    int dx, dy, d, x, y;

    dx = x2 - x1;
    dy = y2 - y1;
    d = dx - 2 * dy;
    y = y1;

    if (dy < 0)
    {
        slope = -1;
        dy = -dy;
    }
    else
    {
        slope = 1;
    }

    for (x = x1; x < x2; x++)
    {
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        if (d <= 0)
        {
            d += 2 * dx - 2 * dy;
            y += slope;
        }
        else
        {
            d += -2 * dy;
        }
        glEnd();
    }
}
