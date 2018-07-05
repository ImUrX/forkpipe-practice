#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
using namespace std;

int main ()
{
	//Utilizando dos procesos: hacer que un proceso le envie "hola, estoy escribiendo en un pipe" a otro proceso.
	//El otro proceso deberá leer el mensaje que le enviaron por el pipe e imprimirlo
    int hijosComunes[2], pipeID[2], pipeMaster[2], cont = 0, pipeHijos[2], pipeUltimo[2];
	pipe(pipeID);
    pipe(pipeHijos);
    pipe(pipeMaster);
    pipe(pipeUltimo);
	int hijoPr = fork();
    char ok[6] = "listo";
    if(hijoPr == 0) {
        while(true) {
            char letra[2];
            read(pipeID[0], letra, 2);
            if(letra[0] == 83) {
                write(pipeHijos[1], letra, 2);
                read(pipeUltimo[0], letra, 2);
                if(letra[0] == 83)
                    write(pipeMaster[1], ok, 6);
            } else {
                cout << letra[0] << endl;
                if(letra[0] != 122) {
                    letra[0]++;
                } else {
                    letra[0] = 97;
                }
                write(pipeHijos[1], letra, 2);
            }
        }
    } else { // aca podes insertar otro for para hacer pipes cada 2 hijos (y hacer i mas logico).
        int pipeHijoComun[2];
        pipe(pipeHijoComun);
        for(cont = 0; cont < 2; cont++) {
            if((hijosComunes[cont] = fork()) == 0) {
                if(hijosComunes[0] == 0) {
                    while(true) {
                        char letra[2];
                        read(pipeHijos[0], letra, 2);
                        if(letra[0] == 83) {
                            write(pipeHijoComun[1], letra, 2);
                        } else {
                            cout << letra[0] << endl;
                            if(letra[0] != 122) {
                                letra[0]++;
                            } else {
                                letra[0] = 97;
                            }
                            write(pipeHijoComun[1], letra, 2);
                        }
                    }
                } else {
                    while(true) {
                        char letra[2];
                        read(pipeHijoComun[0], letra, 2);
                        if(letra[0] == 83) {
                            write(pipeUltimo[1], letra, 2);
                        } else {
                            cout << letra[0] << endl;
                        }
                    }
                }
            }
		}
    }
    char letra[2];
	while(true) {
        cin >> letra;
        if(letra[0] == 83) {
            write(pipeID[1], letra, 2);
            cout << "Esperando Confirmacion" << endl;
            char buffer[6];
            read(pipeMaster[0], buffer, 6);
            if(strcmp(buffer, ok) == 0)
                cout << "ok" << endl;
        } else if(letra[0] > 96 and letra[0] < 123) {
            write(pipeID[1], letra, 2);
        }
    }
}
