Los datos que se le solicitan al usuario son el n�mero de personas por simulaci�n, la potencia infecciosa de la enfermedad, la 
posibilidad de que alguien se recupere, el tiempo que tarda en morirse o inmunizarse una persona, los infectados inicialmente, el
tama�o de la matriz y la cantidad de tics que dura en total la simulaci�n respectivamente.
Si ingresa alg�n dato inv�lido, se le volver�n a pedir todos.
Cuando el mensaje "Archivo generado" aparezca, significa que el txt con la informaci�n de cada tic ha sido creado. El nombre de los
archivos es "report_(numero de simulacion)".
Cuando el mundo se estabiliza, el programa termina el while as� que no hay l�neas de texto m�s all� de eso.
Despu�s de generar los archivos, un mensaje aparece preguntando si desea ver otra simulaci�n. Si desea ver otra, escriba 1 y sino
escriba 2 y podr� salirse del programa.
Se recomienda eliminar los txt si se cierra el programa y se vuelve a abrir porque los append provocar�n que las l�neas de otra 
simulaci�n caigan en un mismo archivo.