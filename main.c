#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "polygon.h"
#include "line.h"

typedef enum MenuChoice
{
    Line = 1,
    Circle = 2,
    Polygon = 3,
    Quit = 0
} MenuChoice;

void initOpenGL();

void displayInit(void);

MenuChoice mainMenu();

void drawPolygonFromFile(char *fileName);

void polygonMenu();

void lineMenu();

void circleMenu();

int main(int argc, char const *argv[])
{
    glutInit(&argc, argv);

    initOpenGL();

    char choice = '\0';
    while (choice = mainMenu())
    {
        switch (choice)
        {
        case (MenuChoice)Line:
            lineMenu();
            break;
        case (MenuChoice)Circle:
            circleMenu();
            break;
        case (MenuChoice)Polygon:
            polygonMenu();
            break;

        default:
            break;
        }
    }

    return 0;
}

void initOpenGL()
{
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(maxHt, maxWd);
    glutInitWindowPosition(100, 150);
    glutCreateWindow("Imagem Rasterizada");
    displayInit();
}

void displayInit(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    gluOrtho2D(0, maxHt, 0, maxWd);
    glClear(GL_COLOR_BUFFER_BIT);
}

MenuChoice mainMenu()
{
    char choice = '\0';

    printf("H para ajuda e Q para sair\n");
    printf("O que deseja desenhar?\n");

    do
    {
        choice = getchar();
        scanf("%*c");
        // while (isspace(choice = getchar()) || choice == EOF)
        //     ;

        printf("choice = %c\n", choice);
        switch (choice)
        {
        case 'H':
            printf("(H) Ajuda\n");
            printf("(L) Linha\n");
            printf("(C) Circulo\n");
            printf("(P) Poligono\n");
            printf("(Q) Sair\n");
            break;
        case 'L':
            return (MenuChoice)Line;
        case 'C':
            return (MenuChoice)Circle;
        case 'P':
            return (MenuChoice)Polygon;
        default:
            break;
        }
    } while (choice != 'Q');

    printf("Saindo...\n");
    exit(0);
}

void lineMenu()
{
    int x1, y1, x2, y2;

    printf("Digite os pontos necessários para o desenho: ");
    scanf("%d %d %d %d", &x1, &y1, &x2, &y2);
    printf("%d %d %d %d\n", x1, y1, x2, y2);

    drawLineFromInput(x1, y1, x2, y2);
}

void lineMenu()
{
    int pntX1, pntY1, r;

    printf("Digite os pontos necessários para o desenho...\n");

	printf("Coordenada X   : ");
    scanf("%d", pntX1);
	printf("\nCoordenada Y : ");
    scanf("%d", pntY1);
	printf("\nRaio : ");
    scanf("%d", r);

    drawCircleFromInput(pntX1, pntY1, r);
}

void polygonMenu()
{
    char c, fileName[FILENAME_MAX];

    printf("Digite o nome do arquivo contendo os pontos a serem desenhados: ");
    fgets(fileName, FILENAME_MAX - 1, stdin);

    if (strlen(fileName))
    {
        fileName[strlen(fileName) - 1] = '\0';
    }
    else
    {
        printf("Impossivel!\n");
    }

    printf("fileName = %s\n", fileName);
    drawPolygonFromFile(fileName);
}