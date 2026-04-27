# Remos

**Remos** es un programa en C, el cual permite analizar la terminal y reconocer la salida, guardando todos los datos importantes.

Remos se ejecuta por dentro de la terminal, permitiendo al usuario recolectar y analizar las diferentes lineas de salida de cualquier comando o proceso que accione, para posteriormente almacenar la informacion relevante en una bitacora, permitiendo analizar esta salida despues de haber concluido la ejecucion del programa original.

## Uso

Remos lee la salida de la terminal y busca palabras claves, las cuales indican que lineas son de importancia, y por ende, se almacenan en un archivo de bitacora, se pueden determinar las palabras clave para buscar y el nombre de la bitacora, esto permite que mantener multiples bitacoras con diferentes configuraciones desde el programa.

Al ser ejecutado, muestra el logo del progama, posteriormente, se despliega un menu con tres opciones, **Iniciar, Opciones, y Salida**.

### Iniciar

Si el archivo de configuracion existe, el programa obtiene las palabras clave y el nombre de la bitacora actual, en caso contrario, indica que se requiere un archivo de configuracion, el cual debe ser creado en el menu de **Opciones**.

Se muestran las palabras clave que el programa buscara y el nombre de la bitacora actual, si detecta que ya existe una bitacora con el mismo nombre, alertara al usuario, indicando que se sobreescribira la bitacora anterior.

Al ingresar la opcion de `continuar` con el teclado, el programa pedira un comando en la terminal para ejecutar y comenzar la lectura, una vez termine la ejecucion del comando, concluye la bitacora, indicando donde fue almacenada y la cantidad de ocurrencias de cada palabra, posteriormente, se regresara al menu principal.

Si se elige la opcion de `regresar`, despliega el menu principal con las tres opciones por defecto.

Antes de leer cualquier linea de la terminal, la bitacora tendra un encabezado, indicando la fecha y hora de inicio, y las palabras que debe identificar.

Mientras el programa ejecuta la accion indicada, lee y analiza cada linea de la salida, buscando las palabras clave definidas por el usuario, al encontrar alguna de ellas, guarda la linea completa dentro de un archivo, y repite el proceso hasta terminar la ejecucion.

La lectura de cada linea se da por el delimitador `\n`, con estos datos, se busca la ocurrencia de cada palabra clave, y en caso de ser encontrada, se guardara una unica vez, sin embargo, los contadores de cada palabra incrementaran por cada ocurrencia, al finalizar la ejecucion, estos contadores son desplegados asi como agregados al final de la bitacora.

### Opciones

Las opciones disponibles son para configurar las palabras clave, el nombre de la bitacora, o, regresar al menu principal.

Configurar las palabras clave permite agregar, modificar o eliminar palabras, al agregar palabras, estas se iran apilando una tras otra, para modificar y eliminar palabras, se debe utilizar el indice que tienen en la lista.

La bitacora puede ser asignada un nombre, sin embargo, unicamente se admiten caracteres alfanumericos, la extension `.txt` se coloca automaticamente al ser creada la bitacora.

### Salida

Finaliza la ejecucion del programa.

## Organizacion

Remos se organiza en un directorio principal ```remos/```, donde se encuentra un archivo de configuracion, con el nombre de la bitacora y las palabras clave, y un directorio para las bitacoras, una estructura que puede ser visualizada de el siguiente modo:

```none
remos/
╠═ config.cfg
╚═ logs/
   ╠═ log1.txt
   ╠═ log2.txt
        •
        •
        •
```

- ```config.cfg``` es el nombre estatico por defecto, del archivo de configuracion.
