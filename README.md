# Timba-loca

## descripcion del circuito

Timba loca es un circuito que simula el juego "craps" de dados, primero se ingresa una apuesta, luego se confirma dicha apuesta, se tiran los dados (presionando una tecla cualquiera del teclado) y se obtiene el resultado y con ello se sabe si se gano o no.
Adicionalmente se agrega un una perilla desde la cual se permite hacer trampa y poder forzar el resultado del que saldra de la apuesta. 
Se agrega a este componente para hacer trampa un led que indica según su luminosidad si se ganará o no la apuesta.
También está presente una bocina para darle música al juego.

## reglas del craps

se tiran dos dados (con caras de valor 1 a 6) y la suma de estos produce los siguientes resultados en las apuestas:

2: se gana el doble de lo apostado y termina el juego
3 a 6: se gana el doble de lo apostado y se sigue jugando
7: se pierde todo lo apostado hasta el momento y termina el juego
8 a 10: se gana el doble de lo apostado y se sigue jugando
11: se pierde todo lo apostado hasta el momento y termina el juego
12: se gana el triple de lo apostado y termina el juego

Si se tiraron los dados y NO se termino el juego, se "marca" el tiro. Es decir, que si en cualquiera de los siguientes tiros se vuelve a obtener la misma suma de los dados, también se termina el juego.

Por ejemplo: en el primer tiro se saca 3, en el segundo 4 y en el tercero otra vez se vuelve a sacar 3. Como el 3 ya se saco con anterioridad se dobla la apuesta una vez más y se termina el juego.

## código
En este repositorio se deja el codigo utilizado en el diseño del circuito.

