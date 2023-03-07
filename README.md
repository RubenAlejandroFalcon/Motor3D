# Motor3D
Proyecto de Investigación en el Laboratorio de Sistemas de la UTN-FRC (1999)


-----------------------------------------------------------------------------

                    Engine 3D versión 0.04 (LOE3D004)
                                                             9/05/1999 22:32
-----------------------------------------------------------------------------



Objetivo: Incorporarle al Engine el corte 3D de los objetos simulando
          la visión mediante un frustrum (pirámide truncada) y, por lo
          tanto, haciendo el clipping contra él.

Autor:    Falcón, Rubén Alejandro (hawks1@bbs.frc.utn.edu.ar)


Para ello se llevaron a cabo los siguientes cambios con respecto
a la versión 0.03:

            * Se agrega el módulo de corte 3D mediante el frustrum.
            * Dentro de la función Draw() (módulo main.c):
                     - se elimina la llamada a Initmatrix() , ya que no es
                       necesaria la inicialización de la matrix a rotar.
                     - la función ProductMatrix se ha modificado debido a
                       que no actualizaba la matriz de la cámara, además de
                       realizar mal el producto de las matrices (ups!). Por
                       lo primero los objetos eran rotados convenientemente
                       (al rotar la cámara en forma local) pero los
                       acercamientos y alejamientos, que se producían en el
                       eje z del espacio de la cámara, eran iguales al los
                       del eje z del espacio de mundo 3D. Por lo segundo las
                       rotaciones se daban sobre el mundo3D, no sobre las
                       el nuevo espacio de la cámara (esto quedaba
                       evidenciado luego de rotar sobre el eje z 90 grados).
                       Estos bugs fueron encontrados en esta versión ya que
                       ahora nos podemos mover libremente por el "world
                       space" ;-).
            * En la función DrawObject3D(): ya no se le pasa como parámetro
              la matriz rotada ya que esta se utiliza para actualizar a la
              matriz de la cámara (en ProductMatrix() ) y los objetos
              son posicionados con esta última que es global.
            * Se declara el polígono clippoly, que será el nuevo polígono
              luego de los cortes necesarios contra los planos del frustrum y
              que es, al final, el que se dibujará.
            * Siguiendo dentro de la función DrawObject3D(), el polígono,
              luego de haberse comprobado que se encuentra de frente a la
              cámara ( Hidden() ), es pasado como parámetro a la función
              ClipToFrustrum() definiendo las coordenadas (las nuevas en caso
              de corte) del segundo parámetro: clippoly.


Otras modificaciones:
        
            * Para hacer más intuitivo la creación de los polígonos se cambia
              el sistema de coordenadas 2D de la pantalla

                              🡡                             🡡 y+
                              |                              |
                              |                              | 
                              |                              |         x+
              de  🡠----------|----------🡢   a   🡠----------|----------🡢
                              |         x+                   |
                              |                              |
                              |                              |
                              🡣 y+                          🡣

              agregando un signo menos (-) en la función ProyectPolygon().
                              

Comentarios de la versión:

            * Se inicializa y muestra un nuevo objeto ob2 que representaría
              el piso del mundo 3D, aunque sea posible traspasarlo :-). Este
              objeto ayuda bastante para ubicarnos tridimensionalmente.
            * Creo que este es el momento adecuado de empezar a optimizar
              las rutinas, ya que el punto flotante y el cálculo de las
              funciones trigonométricas se están haciendo sentir.
              Según los testeos los fps varían alrededor de los 35 en este
              ejemplo particular de 13 polígonos.
            * Algunos documentos interesantes se en encuentran en .\Docs


Grupo de Investigación Engine 3D, LabSis - FRC - UTN:
                        
          Rodrigo Matías Cabrera   icaro@bbs.frc.utn.edu.ar
          Pablo Alfredo Castro     lobo@bbs.frc.utn.edu.ar
          Rubén Alejandro Falcón   hawks1@bbs.frc.utn.edu.ar
          Leonardo David Gonzalez  mctavish@bbs.frc.utn.edu.ar

