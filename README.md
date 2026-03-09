# Remos
**Remos** es un programa en C el cual permite analizar la terminal donde es ejecutado para reconocer la salida y guardar todos los datos importantes de la misma.

Remos se ejecuta por encima, y despues de ello, el usuario puede utilizar su terminal para realizar ciertas acciones, es principalmente utilizado para almacenar la salida de un servidor en una bitacora, para que, en caso de que algo salga mal, se pueda analizar el problema de manera mas adecuada.

## Uso

Remos, al ser ejecutado, muestra un menu al usuario, con tres opciones, **Iniciar, Opciones, y Salida.**

### Iniciar

Remos consulta el archivo de configuracion y obtiene las palabras clave que debera evaluar mientras la terminal sigue ejecucion, despliega en pantalla este conjunto de palabras, solicita que el usuario ingrese el comando para ejecutar, o la palabra 'regresar', por si desea volver al menu principal, y advierte que sobreescribira el archivo de bitacora anterior, en caso de detectar que existe.

Si un comando valido es ingresado, ejecutara el comando y terminara la ejecucion del programa despues, sin embargo, para uso dentro de un servidor, esto solo sucederia cuando el usuario detiene la ejecucion o el servidor colapsa inesperadamente.

Mientras el servidor se ejecuta, Remos lee y analiza cada linea de la salida, buscando las palabras clave definidas por el usuario, si encuentra alguna de ellas, guarda la linea completa dentro de un archivo, y repite el proceso hasta terminar la ejecucion, almacenando cada mensaje importante para el usuario en un archivo.

Cuando termine la ejecucion del programa, mostrara donde se guardaron las lineas registradas.

### Opciones

El usuario puede agregar o quitar palabras clave de la lista a buscar, utilizando los indices de posicion de cada palabra, asimismo, puede cambiar el nombre que tendra el archivo de bitacora, y por ultimo, tiene la opcion de regresar al menu principal.

### Salida

Finaliza la ejecucion del programa.

## Directorios

Remos se organiza en un directorio principal ```remos/```, adentro se encuentran dos archivos para el nombre de la bitacora y la lista de palabras, asimismo, aqui se guarda el archivo de bitacora, generando la siguiende estructura:

```
remos/
├─ config.cfg
├─ logs.txt
├─ words.txt
```

- ```config.cfg``` es el nombre por defecto del archivo de configuracion y no se puede cambiar.
- ```logs.txt``` es el nombre del archivo de bitacora, pero se puede cambiar en **Opciones**, tiene un limite de 64 caracteres.
- ```words.txt``` es el nombre del archivo con la lista de palabras, no se puede cambiar el nombre, pero si el contenido en **Opciones**.
