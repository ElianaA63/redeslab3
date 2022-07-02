# Laboratorio 3

## Primer paso

A continuacion mostramos las graficas que reflejan el estado de los buffer.
Vale aclarar que QueueTx no tiene ninguna limitacion, ni de almacenamiento ni de velocidad.

### Caso 1

Valores de Queue Network : {datarate = ```1 Mbps```; delay = ```100 us```;}
Valores de Queue Rx : {datarate = ```0.5 Mbps```; delay = ```100 us```;}
generationInterval : ```exponential(0.1)```

![grafica1](graficas/V1Caso1.png) {#g1}

Como QueueRx tiene el valor mas bajo de ```datarate``` es donde se produce la congestion y se llena el buffer.

### Caso 2

Valores de Queue Network : {datarate = ```0.5 Mbps```; delay = ```100 us```;}
Valores de Queue Rx : {datarate = ```1 Mbps```; delay = ```100 us```;}
generationInterval : ```exponential(0.1)```

![grafica2](graficas/V1Caso2.png)

En este caso QueueNetwork tiene el valor mas bajo de ```datarate``` y es donde se produce la congestion y se llena el buffer.

## Tarea de analisis

En esta parte vamos a analizar el flujo y la congestion dados dos casos de estudio

Caso 1

* la transferencia entre ```nodeTx``` y la ```queue``` de la red es de:
{datarate = ```1 Mbps```; delay = ```100 us```;}
* la transferencia entre ```queue``` de la red y ```nodeRx``` es de:
{datarate = ```1 Mbps```; delay = ```100 us```;}
* la transferencia entre ```nodeRx``` y ```Sink``` es de:
{datarate = ```0.5 Mbps```;}

Caso 2

* la transferencia entre ```nodeTx``` y la ```queue``` de la red es de:
{datarate = ```1 Mbps```; delay = ```100 us```;}
* la transferencia entre ```queue``` de la red y ```nodeRx``` es de:
{datarate = ```0.5 Mbps```; delay = ```100 us```;}
* la transferencia entre ```nodeRx``` y ```Sink``` es de:
{datarate = ```1 Mbps```;}

Para ambos casos variamos el generationInterval, usado para establecer la velocidad de generacion de paquetes, entre los siguientes valores

```generationInterval: 0.1|0.14|0.16|0.2|0.5```

Un valor mayor a 0.5 no brindaba mucha informacion ya que la tasa de generacion de paquetes era muy lenta

----

## Graficas para el caso 1

### generationInterval : 0.1

![grafica3](graficas/Caso1P1/c1gi010.png)

Como la tasa de generacion de paquetes es muy alta al poco tiempo se llena el buffer de QueueRx y empieza a dropear los paquetes que llegan

![grafica4](graficas/Caso1P1/c1gi010h.png)

En esta grafica se puede ver la relacion entre los paquetes recibidos y los dropeados. Casi el 50% de los paquetes enviados se pierden.

### generationInterval : 0.14

![grafica5](graficas/Caso1P1/c1gi014.png)

En este caso el buffer tarda mas tiempo en llenarse.

![grafica6](graficas/Caso1P1/c1gi014h.png)

Ya que reducimos la tasa de generacion la perdida de paquetes baja al 20%.

### generationInterval : 0.16

![grafica7](graficas/Caso1P1/c1gi016.png)

Con este tiempo de generacion el buffer no se llena hasta casi al final de la simulacion.

![grafica8](graficas/Caso1P1/c1gi016h.png)

La perdida de paquetes se redujo drasticamente.

### generationInterval : 0.2

![grafica9](graficas/Caso1P1/c1gi020.png)

Aca se puede observar que el buffer no se llena en ningun momento.

![grafica10](graficas/Caso1P1/c1gi020h.png)

La perdida de paquetes se redujo a cero.

----

## Graficas para el caso 2

En este caso, como el Queue que tiene menor velocidad es el del Network se producen las mismas congestiones que en el caso 1 pero en el Queue Newtwork

### generationInterval : 0.1

![grafica11](graficas/Caso2P1/c2gi010.png)

![grafica12](graficas/Caso2P1/c2gi010h.png)

### generationInterval : 0.14

![grafica13](graficas/Caso2P1/c2gi014.png)

![grafica14](graficas/Caso2P1/c2gi014h.png)

### generationInterval : 0.16

![grafica15](graficas/Caso2P1/c2gi016.png)

![grafica16](graficas/Caso2P1/c2gi016h.png)

### generationInterval : 0.2

![grafica17](graficas/Caso2P1/c2gi020.png)

![grafica18](graficas/Caso2P1/c2gi020h.png)

## Respuestas parte 1

La direncia entre los dos casos es el ```datarate``` del QueueNetwork (red) y del QueueRx (receptor).
La fuente limitante de cada caso es donde tiene el menor ```datarate```, en el caso 1 en QueueNetwork y en el caso 2 en QueueRx.

El control de congestión consiste y hace referencia al control de tráfico de entrada a una red para evitar que la misma se congestione; consiste en reducir la tasa de envío de paquete de datos para disminuír la congestión.
Por su parte, el control de flujo es el proceso de gestionar la tasa de transmisión de datos entre dos nodos; el objetivo es prevenir que un transmisor rápido exceda a un receptor lento.

----

## Tarea de diseno

Para solucionar el problema de congestion de la primera parte, nos basamos en la idea de parada y espera para hacer el algortimo que resuelva esto.

**Funcionamiento del algoritmo implementado:**

* Primero se establece la conexion, para eso el Tx envia un paquete ```REQUEST``` y si recive un paquete ```ACCEPT``` queda establecida la conexion para empezar con el envio de los demas paquetes.

* Cada vez que se envia un paquete, el Tx espera un paquete de tipo ```feedback``` con un mensaje, ```ack``` si llego exitosamente o ```nak``` en caso contrario

* Si el ```feedback``` es ```ack``` envia el paquete siguiente

* Encambio, si el ```feedback``` es ```nak``` reenvia el paquete perdido

* Otra situacion posible es que no llegue ningun tipo de ```feedback```, en ese caso se espera un tiempo, dado por un temporizador, cuando el tiempo llega a cero, se reenvia el paquete sin confirmacion. Esto es asi para evitar un posible *Deadlock* por un paquete perdido.

* El camino de un paquete tipo ```cPacket``` es:

    Network.nodeTx ```->``` Network.queue[0] ```->``` network.nodeRx

* El camino de un paquete tipo ```feedback``` es:

    Network.nodeRx ```->``` Network.queue[1] ```->``` network.nodeTx
    A excepcion del primer paquete ```REQUEST``` que sigue el primer camino

## Respuestas parte 2

Creemos que va a mejorar la congestion y perdidas de paquetes, pero se va a ver afectada la cantidad de paquetes enviadas en el mismo tiempo de simulacion.
Suponemos que va a funcionar por igual para los dos casos, ya que el ```feedback``` va a resolver el problema de la perdida de paquetes del nodeRx y el temporizador va a solucioanr la perdida de paquetes de Network.queue

## Graficas resultados de las simulaciones

Las siguientes graficas son **validas para los dos casos** de estudio con todas las variaciones de ```generationInterval```

![grafica19](graficas/buffersP2.png)

Como se puede ver ahora los buffers de Rx y Queue[0] no se llenan, es mas, nunca tienen mas de un paquete.

![grafica20](graficas/paquetesEnviadosP2.png)

La desventaja es que re redujo mas del 60% la cantidad de paquetes enviados.

![grafica21](graficas/enviadosVsRecibidosP2.png)

Pero solucionamos el problema de perdida de paquetes, ya que no se pierde ninguno.

## Carga Ofrecida Vs Carga Util

Carga ofrecida:

* Parte 1:
  * Caso 1 y 2: Los enviados por Network.nodeTx.queue
* Parte 2:
  * Caso 1 y 2: Los enviados por Network.nodeTx.traTx

Carga Util:

* Parte 1:
  * Caso 1 y 2: Recibidos en Network.nodeRx.sink mas encolados en Network.nodeRx.queue
* Parte 2:
  * Caso 1 y 2: Recibidos Network.nodeRx.sink, ya que en estas simulaciones en Network.nodeTx.traRx como mucho hay un paquete encolado

![grafica22](graficas/COCU-CASO1P12.png)

![grafica22](graficas/COCU-CASO2P12.png)

En ambas graficas se puede observar que sin el control de congestion, apartir de cierto punto en la velocidad de envio de paquetes, estos se empiezan a perder, en el [caso1](#caso-1) se pierden en Network.nodeRx.queue y en el [caso 2](#caso-2) en Network.queue.

La desventaja del algoritmo es que ofrece una menor cantidad de paquetes por segundo, para solucionar esto pensamos alguna mejoras al algoritmo, por ejemplo, agregar indentificadores a los paquetes, de esta forma podemos enviar todos como antes y al recibir o no el feedback poder reenviarlos, y en el tiempo que espera el feedback poder seguir enviando otros sin perder la referencia de los paquetes, tambien pensamos en que el TransportRx puede tener ademas un feedback del estado del buffer, y asi poder regular la velocidad de envios de paquetes del TransportRx evitando congestiones, pero por cuestiones de tiempo no pudimos implementarlas.
