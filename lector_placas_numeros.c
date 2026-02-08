#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER 1024

//Eduardo Enrique Ponce 

//Programa que lee archivos de texto, identifica placas de vehiculo y numeros decimales.

//Funciones

void leer_archivo(int fd, char *buffer_salida);
int es_decimal(const char *raw_data);
int leer_placa(const char *raw_data);

int main() {

    char almacenamiento[MAX_BUFFER];
    int total_decimales = 0;
    int total_placas = 0;

    int descriptor_archivo = open("archivo.txt", O_RDONLY);

    if (descriptor_archivo != -1) {

        // 1. Carga de datos
        leer_archivo(descriptor_archivo, almacenamiento);

        // 2. Procesamiento de patrones
        total_decimales = es_decimal(almacenamiento);
        total_placas = leer_placa(almacenamiento);

        // 3. Salida de resultados
        printf("--- Reporte de Analisis ---\n");
        printf("Texto leido: %s\n", almacenamiento);
        printf("Total Decimales: %d\n", total_decimales);
        printf("Total Placas:    %d\n", total_placas);

    } else {

        printf("Error al abrir el recurso\n");

    }

    return 0;

}

// Implementación de funciones

void leer_archivo(int fd, char *buffer_salida) {

    ssize_t bytes_leidos;
    int index = 0;
    char byte_temp;

    while ((bytes_leidos = read(fd, &byte_temp, 1)) > 0 && index < MAX_BUFFER - 1) {
        
        buffer_salida[index++] = byte_temp;

    }

    buffer_salida[index] = '\0';
    close(fd);

}

int es_decimal(const char *raw_data) {

    int contador = 0;
    int fase = 0;
    size_t longitud = strlen(raw_data);

    for (size_t i = 0; i < longitud; i++) {

        char c = raw_data[i];

        switch (fase) {

            case 0: // Inicio

                if (c == '+' || c == '-') {

                    fase = 1;

                } else if (c >= '0' && c <= '9') {

                    fase = 2;

                }

                break;

            case 1: // Tras signo (+/-)

                if (c >= '0' && c <= '9') {

                    fase = 2;

                } else { 

                    fase = 0; 
                    i--; 

                }

                break;

            case 2: // Parte entera

                if (c >= '0' && c <= '9') {

                    fase = 2;

                } else if (c == '.') {

                    fase = 3;

                } else { 

                    fase = 0; 
                    i--; 

                }

                break;

            case 3: // Tras punto decimal

                if (c >= '0' && c <= '9') {

                    fase = 4;

                } else { 

                    fase = 0; 
                    i--; 

                }

                break;

            case 4: // Parte decimal (Aceptación)

                if (c >= '0' && c <= '9') {

                    fase = 4;

                } else {

                    contador++;
                    fase = 0;
                    i--; 

                }

                break;

        }

    }

    if (fase == 4) {

        contador++;

    }
    
    return contador;

}

int leer_placa(const char *raw_data) {

    int hallazgos = 0;
    int step = 0;
    size_t len = strlen(raw_data);

    for (size_t i = 0; i < len; i++) {

        char token = raw_data[i];

        switch (step) {

            case 0: // Letra 1

                if (token >= 'A' && token <= 'Z') {

                    step = 1;

                }

                break;

            case 1: // Letra 2

                if (token >= 'A' && token <= 'Z') {

                    step = 2;

                } else { 

                    step = 0; 
                    i--; 

                }

                break;

            case 2: // Separador (- o *)

                if (token == '-' || token == '*') {

                    step = 3;

                } else { 

                    step = 0; 
                    i--; 

                }

                break;

            case 3: // Digito 1
                if (token >= '0' && token <= '9') {

                    step = 4;

                } else { 

                    step = 0; 
                    i--; 

                }

                break;

            case 4: // Digito 2

                if (token >= '0' && token <= '9') {

                    step = 5;

                } else { 

                    step = 0; 
                    i--; 

                }

                break;

            case 5: // Digito 3 (Aceptación)

                if (token >= '0' && token <= '9') {

                    hallazgos++;
                    step = 0;

                } else {

                    step = 0;
                    i--;

                }

                break;

        }

    }

    return hallazgos;

}