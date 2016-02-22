#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define TCP_PORT    8000

void imprimirEstadisticas(int* datos, int len)
{
    int max, sum = 0, min;

    for (int i = 0; i < len; i++)
    {
        if (i == 0)
        {
            max = *(datos + i);
            min = *(datos + i);
        }
        else
        {
            if (*(datos + i) > max)
            {
                max = *(datos + i);
            }
            else if (*(datos + i) < min)
            {
                min = *(datos + i);
            }
        }
        sum += *(datos + i);
    }

    float avg = sum * 1.0f / len;


    printf(" MAX: %i\n", max);
    printf(" MIN: %i\n", min);
    printf(" AVG: %f\n", avg);
}

int main(int argc, const char * argv[])
{
    struct sockaddr_in direccion;
    char               buffer[1000];
    int                servidor, cliente;
    int                * datos = (int*)malloc(100 * sizeof(int));
    int                index   = 0;


    ssize_t leidos, escritos;
    int     continuar = 1;
    pid_t   pid;

    // Crear el socket
    servidor = socket(PF_INET, SOCK_STREAM, 0);
    // Enlace con el socket
    inet_aton(argv[1], &direccion.sin_addr);
    direccion.sin_port   = htons(TCP_PORT);
    direccion.sin_family = AF_INET;

    bind(servidor, (struct sockaddr*)&direccion, sizeof(direccion));

    // Escuhar
    listen(servidor, 10);

    escritos = sizeof(direccion);

    // Aceptar conexiones
    while (continuar)
    {
        cliente = accept(servidor, (struct sockaddr*)&direccion, &escritos);

        printf("Aceptando conexiones en %s:%d \n",
               inet_ntoa(direccion.sin_addr),
               ntohs(direccion.sin_port));
        pid = fork();
        if (pid == 0)
            continuar = 0;
    }

    if (pid == 0)
    {
        close(servidor);

        if (cliente >= 0)
        {
            // LOGICA DEL SERVIDOR
            while (leidos = read(cliente, &buffer, sizeof(buffer)))
            {
                printf("IN: %s \n", buffer);

                //Separa la cadena por tokens
                const char s[2] = ",";
                char       *token;
                token = strtok(buffer, s);
                while (token != NULL)
                {
                    datos[index] = atoi(token);
                    index++;
                    printf(" %s\n", token);
                    token = strtok(NULL, s);
                }




                printf("---- DATOS (%d)---- \n", index);
                for (int i = 0; i < index; i++)
                    printf("%i ", datos[i]);
                printf("\n");

                //Envia un ping de recibido
                int r = 1;
                sprintf(buffer, "%d", r);
                int nBytes = strlen(buffer) + 1;
                write(cliente, &buffer, nBytes);
            }
            //FIN LOGICA SERVIDOR
        }

        close(cliente);
        imprimirEstadisticas(datos, index);
        free(datos);
    }

    else if (pid > 0)
    {
        while (wait(NULL) != -1)
            ;

        // Cerrar sockets
        close(servidor);
    }
    return 0;
}
