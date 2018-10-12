Los datos que se le solicitan al usuario son el número de personas por simulación, la potencia infecciosa de la enfermedad, la 
posibilidad de que alguien se recupere, el tiempo que tarda en morirse o inmunizarse una persona, los infectados inicialmente, el
tamaño de la matriz y la cantidad de tics que dura en total la simulación respectivamente.
Si ingresa algún dato inválido, se le volverán a pedir todos.
Cuando el mensaje "Archivo generado" aparezca, significa que el txt con la información de cada tic ha sido creado. El nombre de los
archivos es "report_(numero de simulacion)".
Cuando el mundo se estabiliza, el programa termina el while así que no hay líneas de texto más allá de eso.
Después de generar los archivos, un mensaje aparece preguntando si desea ver otra simulación. Si desea ver otra, escriba 1 y sino
escriba 2 y podrá salirse del programa.
Se recomienda eliminar los txt si se cierra el programa y se vuelve a abrir porque los append provocarán que las líneas de otra 
simulación caigan en un mismo archivo.