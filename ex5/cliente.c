#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TCP_PORT    8000
#define TEMPO       7

int main(int argc, const char * argv[])
{
    struct sockaddr_in direccion;
    char               buffer[1000];

    //Crear el socket
    int cliente = socket(PF_INET, SOCK_STREAM, 0);

    // Establecer conexión
    direccion.sin_port   = htons(TCP_PORT);
    direccion.sin_family = AF_INET;
    inet_aton(argv[1], &direccion.sin_addr);

    int estado = connect(cliente, (struct sockaddr*)&direccion, sizeof(direccion));

    // Comunicación
    if (estado == 0)
    {
        printf("Conectado a %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));

        //LOGICA DEL CLIENTE
        char* c = (char*)malloc(100*sizeof(char));
        char* cTemp = (char*)malloc(100*sizeof(char));
        int  tim = 0;
        while (1)
        {
            if (tim < TEMPO)
            {
                sleep(3);
                srand(time(NULL));
                int r = rand() % 10;
                if (r == 0)
                {
                    printf("Termino cliente\n");
                    exit(0);
                }
                if (tim + 3 < TEMPO)
                    sprintf(cTemp, "%i,", r);
                else
                    sprintf(cTemp, "%i", r);
                strcat(c, cTemp);
                printf("%s\n", c);
                tim += 3;
            }
            else
            {
                printf("Enviando\n");
                tim = 0;
                strncpy(buffer, c, sizeof(c));
                c = (char*)malloc(100*sizeof(char));
                cTemp = (char*)malloc(100*sizeof(char));
                int nBytes = strlen(buffer) + 1;
                write(cliente, &buffer, nBytes);
                read(cliente, &buffer, sizeof(buffer));
            }
        }

        free(c);
        free(cTemp);
        //FIN LOGICA
    }

    close(cliente);

    return 0;
}
