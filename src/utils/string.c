#include <stdlib.h>
#include <string.h>
#include <ctype.h>   // for isdigit
#include <stdio.h>
#include "string.h"
#include <curses.h>
#include "../sc.h"
#include "../macros.h"

// Funcion que elimina un caracter de una cadena.
// La posicion del caracter a eliminar es indicada como parametro.
// 0 es la primera posición
// retorna 0 si OK, -1 en caso negativo.
int del_char(char * str, int posicion) {
    int i, slen = strlen(str);

    if ( posicion >= slen || posicion < 0 ) return -1;
    for (i = posicion; i < slen - 1; i++) {
        str[i] = str[i + 1];
    }
    str[--slen] = '\0';
    return 0;
}

// Funcion que elimina un rango de caracteres de una cadena.
// La posicion desde y hasta es indicada como parametro.
// retorna 0 si OK, -1 en caso negativo.
int del_range_chars(char * str, int d, int h) {
    int i = 0, j = 0, slen = strlen(str);

    if ( h >= slen || h < d ) return -1;

    for (j = 0; j <= (h-d); j++)
    for (i = d; i < slen - 1; i++) {
        str[i] = str[i+1];
    }
    str[slen - (h - d) - 1] = '\0';
    return 0;
}

// Funcion que agrega un caracter a una cadena
// en la posicion indicada como parametro.
// Se asume que str tiene alocada suficiente
// memoria como para albergar al nuevo char
// retorna 0 si OK, -1 en caso negativo.
int add_char(char * str, char c, int posicion) {
    int slen = strlen(str);
    int len = slen - posicion;
    if (posicion > slen) return -1;
    while (len) {
        str[posicion + len] = str[posicion + len -1];
        len--;
    }
    str[++slen] = '\0';
    str[posicion] = c;
    return 0;
}

// Funcion que reemplaza de una cadena todas los ocurrencias de un caracter
// por otro
void subst(char * s, char from, char to) {
    while (*s == from) *s++ = to;
    return;
}

// funcion que busca la cadena b dentro de la cadena s
// devuelve la posicion en la cadena s, en caso de ser encontrada
// devuelve -1 en caso contrario
int str_in_str(char * s, char * b) {
    int slen = strlen(s);
    int blen = strlen(b);

    if (! slen || ! blen) return -1;

    int e = 0;
    int i;

    while ( e <= slen-blen ) {
        for (i=0; i<blen; i++) {
            if (s[e+i] != b[i]) {
                break;  
            }
        }
        if (i >= blen) return e;
        else e++;
    }
    return -1;
}

// Funcion que indica si se tiene un caracter especial o de control.
int is_idchar (int d) {
    switch (d) {
        case OKEY_LEFT:
        case OKEY_RIGHT:
        case OKEY_DOWN:
        case OKEY_UP:
        case OKEY_TAB:
        case OKEY_BS:
        case OKEY_HOME:
        case OKEY_END:
        case OKEY_PGUP:
        case OKEY_PGDOWN:
        case OKEY_DEL:
        //case OKEY_ENTER:
        case OKEY_ESC:
           return 1;
    }
    return 0;
}

char ** split(char *string, const char delimiter, int lastnull) {
    int length = 0, count = 0, i = 0, j = 0;
    while(*(string++)) {
        if (*string == delimiter) count++;
        length++;
    }
    string -= (length + 1); // string was incremented one more than length
    char **array = (char **)malloc(sizeof(char *) * (length + 1 + lastnull));
    char ** base = array;
    for(i = 0; i < (count + 1); i++) {
        j = 0;
        while(string[j] != delimiter) j++;
        j++;
        * array = (char *) malloc(sizeof(char) * j);
        memcpy(*array, string, (j-1));
        (*array)[j-1] = '\0';
        string += j;
        array++;
    }
    if (lastnull) {
        *array = NULL;
        array++;
    }
    *array = '\0';
    return base;  
}

char * rtrim(char * string, char junk) {
    char * original = string + strlen(string);
    while(*--original == junk);
    *(original + 1) = '\0';
    return string;
}

char * ltrim(char * string, char junk) {
    char * original = string;
    char * p = original;
    int trimmed = 0;
    do {
        if (*original != junk || trimmed) {
            trimmed = 1;
            *p++ = *original;
        }
    } while (*original++ != '\0');
    return string;
}

// this function tells is a string represents a numeric value
// returns 1 if that is the case. 0 otherwise
int isnumeric(char * string) {
    int i, len = strlen(string);
    int res = 1;
    for (i=0; i<len; i++) {
        if ( string[i] != '.' && string[i] != '-' && ! isdigit(string[i]) ) {
            res = 0;
            break;
        }
    }
    return res;
}

// clean \r and \n from a string
// returns 1 if changes were made
int clean_carrier(char * string) {
    int i, changes = 0, len = strlen(string);
    for (i=0; i<len; i++) {
        if ( string[i] == '\r' || string[i] == '\n' ) {
            del_char(string, i);
            len--;
            changes = 1;
        }
    }
    return changes;
}

/*
 *  * strtok version that handles null fields
 *   */
char * xstrtok(char * line, char * delims) {
    static char * saveline = NULL;
    char *p;
    int n;
 
    if(line != NULL)
       saveline = line;

/*
 * *see if we have reached the end of the line
 * */
    if (saveline == NULL || *saveline == '\0')
       return(NULL);
/*
 * *return the number of characters that aren't delims
 * */
    n = strcspn(saveline, delims);
    p = saveline; /*save start of this token*/

    saveline += n; /*bump past the delim*/

    if (*saveline != '\0') /*trash the delim if necessary*/
       *saveline++ = '\0';

    return(p);
}
