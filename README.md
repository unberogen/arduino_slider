# arduino_slider
codigo para un slider hecho con arduino
El código permite configurar el slider para los siguientes pasos.<br>
<br>
Pasos en cada bucle<br>
1.-  Mover el motor.<br>
2a.- Con disparador SONY<br>
      2a.1.- Tiempo de espera.<br>
2b.- Sin disparador SONY<br>
      2b.1.- Tiempo de espera offset.<br>
      2b.2.- Disparar camara.<br>
      2b.3.- Tiempo restante.<br>
<br>
<br>
# menu
1.- Decidir sentido del motor.<br>
    sentido => 0=der 1=izq<br>
2.- Tiempo que pasa entre un movimiento y otro<br>
    intervaloMov<br>
3.- numero de semirotaciones por cada movimiento<br>
velocidadMov<br>
4.- usara o no el disparador sony<br>
usarDiaparador => 0=no 1=si<br>
5.- tiempo desde que se mueve hasta que dispara<br>
offsetDis<br>
6.- duracion del disparo<br>
duracionDis<br>

# cosas que faltan por hacer
- Añadir 500ms al disparador entre el e nfocar y el disparo<br>
