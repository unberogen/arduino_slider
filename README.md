# arduino_slider
codigo para un slider hecho con arduino
El código permite configurar el slider para los siguientes pasos.

Pasos en cada bucle
1.-  Mover el motor.
2a.- Con disparador SONY
      2a.1.- Tiempo de espera.
2b.- Sin disparador SONY
      2b.1.- Tiempo de espera offset.
      2b.2.- Disparar camara.
      2b.3.- Tiempo restante.
    
  
# menu
1.- Decidir sentido del motor.
    sentido => 0=der 1=izq
2.- Tiempo que pasa entre un movimiento y otro
    intervaloMov == tiempo entre cada movimiento
3.- velocidadMov == numero de semirotaciones por cada movimiento
4.- usarDiaparador => 0=no 1=si
5.- offsetDis == tiempo desde que se mueve hasta que dispara
6.- duracionDis == duracion del disparo

# cosas que faltan por hacer
- Añadir 500ms al disparador entre el e nfocar y el disparo
