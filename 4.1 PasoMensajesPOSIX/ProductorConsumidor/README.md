Ejemplo del productor-consumidor con paso de mensajes. 
Esta carpeta contiene 2 formas del mismo ejemplo
 - Productor (productor.c) y consumidor (consumidor.c) en archivos separados, se ejecutan de manera independiente y se comunican con una cola de mensajes POSIX
 - Porductor y consumidor en un solo archivo (prodcons.c), el segundo proceso se crea internamente con fork, luego cada proceso toma uno de los comportamientos, de productor o de consumidor. También se comunican con una cola de mensajes POSIX
