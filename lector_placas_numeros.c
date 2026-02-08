#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define MAX_BUFFER 1024

//Eduardo Enrique Ponce 

//Programa que lee archivos de texto, identifica placas de vehiculo y numeros decimales.

//Funciones

void leer_archivo(int fd, char *buffer_salida);
int es_decimal(const char *cadena);
int leer_placa(const char *cadena);

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
    int indice = 0;
    char byte_temp;

    while ((bytes_leidos = read(fd, &byte_temp, 1)) > 0 && indice < MAX_BUFFER - 1) {
        
        buffer_salida[indice++] = byte_temp;

    }

    buffer_salida[indice] = '\0';
    close(fd);

}

int es_decimal(const char *cadena) {

    int contador = 0;
    int estado = 0;
    size_t longitud = strlen(cadena);

    for (size_t i = 0; i < longitud; i++) {

        char c = cadena[i];

        switch (estado) {

            case 0: // Inicio

                if (c == '+' || c == '-') {

                    estado = 1;

                } else if (c >= '0' && c <= '9') {

                    estado = 2;

                }

                break;

            case 1: // Tras signo (+/-)

                if (c >= '0' && c <= '9') {

                    estado = 2;

                } else { 

                    estado = 0; 
                    i--; 

                }

                break;

            case 2: // Parte entera

                if (c >= '0' && c <= '9') {

                    estado = 2;

                } else if (c == '.') {

                    estado = 3;

                } else { 

                    estado = 0; 
                    i--; 

                }

                break;

            case 3: // Tras punto decimal

                if (c >= '0' && c <= '9') {

                    estado = 4;

                } else { 

                    estado = 0; 
                    i--; 

                }

                break;

            case 4: // Parte decimal (Aceptación)

                if (c >= '0' && c <= '9') {

                    estado = 4;

                } else {

                    contador++;
                    estado = 0;
                    i--; 

                }

                break;

        }

    }

    if (estado == 4) {

        contador++;

    }
    
    return contador;

}

int leer_placa(const char *cadena) {

    int hallazgos = 0;
    int estado = 0;
    size_t len = strlen(cadena);

    for (size_t i = 0; i < len; i++) {

        char simbolo = cadena[i];

        switch (estado) {

            case 0: // Letra 1

                if (simbolo >= 'A' && simbolo <= 'Z') {

                    estado = 1;

                }

                break;

            case 1: // Letra 2

                if (simbolo >= 'A' && simbolo <= 'Z') {

                    estado = 2;

                } else { 

                    estado = 0; 
                    i--; 

                }

                break;

            case 2: // Separador (- o *)

                if (simbolo == '-' || simbolo == '*') {

                    estado = 3;

                } else { 

                    estado = 0; 
                    i--; 

                }

                break;

            case 3: // Digito 1
                if (simbolo >= '0' && simbolo <= '9') {

                    estado = 4;

                } else { 

                    estado = 0; 
                    i--; 

                }

                break;

            case 4: // Digito 2

                if (simbolo >= '0' && simbolo <= '9') {

                    estado = 5;

                } else { 

                    estado = 0; 
                    i--; 

                }

                break;

            case 5: // Digito 3 (Aceptación)

                if (simbolo >= '0' && simbolo <= '9') {

                    hallazgos++;
                    estado = 0;

                } else {

                    estado = 0;
                    i--;

                }

                break;

        }

    }

    return hallazgos;

}