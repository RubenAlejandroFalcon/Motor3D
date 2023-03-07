-----------------------------------------------------------------------------

                    Engine 3D versi¢n 0.04 (LOE3D004)
                                                             9/05/1999 22:32
-----------------------------------------------------------------------------



Objetivo: Incorporarle al Engine el corte 3D de los objetos simulando
          la visi¢n mediante un frustrum (pir mide truncada) y, por lo
          tanto, haciendo el clipping contra ‚l.

Autor:    Falc¢n, Rub‚n A. (hawks1@bbs.frc.utn.edu.ar)


Para ello se llevaron a cabo los siguientes cambios con respecto
a la versi¢n 0.03:

            * Se agrega el m¢dulo de corte 3D mediante el frustrum.
            * Dentro de la funci¢n Draw() (m¢dulo main.c):
                     - se elimina la llamada a Initmatrix() , ya que no es
                       necesaria la inicializaci¢n de la matrix a rotar.
                     - la funci¢n ProductMatrix se ha modificado debido a
                       que no actualizaba la matriz de la c mara, adem s de
                       realizar mal el producto de las matrices (ups!). Por
                       lo primero los objetos eran rotados convenientemente
                       (al rotar la c mara en forma local) pero los
                       acercamientos y alejamientos que se produc¡an en el
                       eje z del espacio de la c mara eran iguales al los
                       del eje z del espacio de mundo 3D. Por lo segundo las
                       rotaciones se daban en sobre el mundo3D, no sobre las
                       el nuevo espacio de la c mara (esto quedaba
                       evidenciado luego de rotar sobre el eje z 90 grados).
                       Estos bugs fueron encontrados en esta versi¢n ya que
                       ahora nos podemos mover libremente por el "world
                       space" ;-).
            * En la funci¢n DrawObject3D(): ya no se le pasa como par metro
              la matriz rotada ya que esta se utiliza para actualizar a la
              matriz de la c mara (en ProductMatrix() ) y los objetos
              son posicionados con esta £ltima que es global.
            * Se declara el pol¡gono clippoly, que ser  el nuevo pol¡gono
              luego de los cortes necesarios contra los planos del frustrum y
              que es, al final, el que se dibujar .
            * Siguiendo dentro de la funci¢n DrawObject3D(), el pol¡gono,
              luego de haberse comprobado que se encuentra de frente a la
              c mara ( Hidden() ), es pasado como par metro a la funci¢n
              ClipToFrustrum() definiendo las coordenadas (las nuevas en caso
              de corte) del segundo par metro: clippoly.


Otras modificaciones:
        
            * Para hacer m s intuitivo la creaci¢n de los pol¡gonos se cambia
              el sistema de coordenadas 2D de la pantalla

                                                             y+
                              |                              |
                              |                              | 
                              |                              |         x+
              de   ----------|----------   a    ----------|----------
                              |         x+                   |
                              |                              |
                              |                              |
                               y+                           

              agregando un signo menos (-) en la funci¢n ProyectPolygon().
                              

Comentarios de la versi¢n:

            * Se inicializa y muestra un nuevo objeto ob2 que representar¡a
              el piso del mundo 3D, aunque sea posible traspasarlo :-). Este
              objeto ayuda bastante para ubicarnos tridimensionalmente.
            * Creo que este es el momento adecuado de empezar a optimizar
              las rutinas, ya que el punto flotante y el c lculo de las
              funciones trigonom‚tricas se est n haciendo sentir.
              Seg£n los testeos los fps var¡an alrededor de los 35 en este
              ejemplo particular de 13 pol¡gonos.
            * Algunos documentos interesantes se en encuentran en .\Docs


Grupo de Investigaci¢n Engine 3D, LabSis - FRC - UTN:
                        
          Rodrigo Mat¡as Cabrera   icaro@bbs.frc.utn.edu.ar
          Pablo Alfredo Castro     lobo@bbs.frc.utn.edu.ar
          Rub‚n Alejandro Falc¢n   hawks1@bbs.frc.utn.edu.ar
          Leonardo David Gonzalez  mctavish@bbs.frc.utn.edu.ar

