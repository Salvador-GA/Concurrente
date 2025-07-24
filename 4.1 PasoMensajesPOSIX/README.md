Ejemplos de paso de mensajes usando las instrucciones POSIX para comunicación entre procesos

Se recomienda que ambos programas se compilen con la biblioteca de tiempo real (-lrt) debido a que las colas POSIX se implementan usando funciones de esa biblioteca en sistemas como Linux, y en algunas distribuciones hay que hacer explicito esto al compilador con la bandera.
gcc -o Ej1Emisor Ej1Emisor.c -lrt
gcc -o Ej1Receptor Ej1Receptor.c -lrt
