#include "CG.h"
#include "vec.h"

GLfloat coordenadas_vertex[N_VERTICES][3];


/*Função para leitura de vértices em um arquivo txt.*/
int leituraVertices(){

    FILE *arquivo;
    int i=0,j=0;
    arquivo = fopen("vertices.txt","r");
    if(!arquivo){
        printf("Erro na abertura do arquivo\n");
        return 1;
    }

    

    while(!feof(arquivo)){
        if (j == 3){
            i++;
            j=0;
        }

        fscanf(arquivo,"%f",&coordenadas_vertex[i][j]);
        j++;

    }
    return 0;
}


 /* Função para desenhar as arestas da pirâmide*/
void drawLines(void){
    glBegin(GL_LINES);
    glColor3f(0.0,0.0,0.0);
    int i =0,j;
    for(i=0;i<4;i++){
            //printf("%f\n",coordenadas_vertex[i][quadrados[0][i]]);
            glVertex3fv(coordenadas_vertex[quadrados[0][i]]);
    }


    for(i=0;i<4;i++){
       for(j=0;j<3;j++)
            glVertex3fv(coordenadas_vertex[triangulos[i][j]]);
        }

    glEnd();
}


/*Função para desenhar o quadrado da base da pirâmide*/
void drawQuad(float size)
{
        glBegin(GL_QUADS);
        glColor3f(size,0.0,0.0);
        int i;
        for(i=0;i<4;i++){
            //printf("%f\n",coordenadas_vertex[i][quadrados[0][i]]);
            glVertex3fv(coordenadas_vertex[quadrados[0][i]]);
        }
        glEnd();
}

/*Função para desenhar os triângulos*/
void drawTriang(){

    glBegin(GL_TRIANGLES);
        int i;
        for( i=0;i<4;i++){
            
            switch(i){
                case 1:
                glColor3f(0.0,1.0,0.0);
                break;
                case 2:
                glColor3f(0.0,0.0,1.0);
                break;
                case 3:
                glColor3f(0.0,100.0,1.0);
                break;
                case 4:
                glColor3f(0.0,1.0,100.0);
                break;
            }
            int j;
            for(j=0;j<3;j++)
                glVertex3fv(coordenadas_vertex[triangulos[i][j]]);
        }
    glEnd();


}


void init(void)
{
        glClearColor(0.0,0.0,0.0,1.0); 
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45,640.0/480.0,1.0,500.0);
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_DEPTH_TEST);
}
 

void display(float angle)
{
        // glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        // glLoadIdentity();
        //glTranslatef(0.0,0.0,-5.0);
        //glRotatef(angle,1.0,1.0,1.0);   
        angle++;
        angle--;
        drawQuad(1.0);
        drawTriang();
        drawLines();
        
}