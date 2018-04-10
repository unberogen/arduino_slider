# arduino_slider
codigo para un slider hecho con arduino, en principio para controlarlo como time lapse, pero con posibles mejoras
El código permite configurar el slider en modo time laspse para los siguientes pasos.<br>
<br>
Pasos en cada bucle<br>
1.-  Mover el motor.<br><br>
2a.- Con disparador SONY<br>
&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; 2a.1.- Tiempo de espera.<br><br>
2b.- Sin disparador SONY<br>
&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; 2b.1.- Tiempo de espera offset.<br>
&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; 2b.2.- Disparar camara.<br>
&nbsp;&nbsp;&nbsp;&nbsp; &nbsp;&nbsp;&nbsp;&nbsp; 2b.3.- Tiempo restante.<br>
<br>
<br>
# menu
1.- Decidir sentido del motor.<br>
&nbsp;&nbsp;&nbsp;&nbsp; sentido => 0=der 1=izq<br>
2.- Tiempo que pasa entre un movimiento y otro<br>
&nbsp;&nbsp;&nbsp;&nbsp; intervaloMov<br>
3.- numero de semirotaciones por cada movimiento<br>
&nbsp;&nbsp;&nbsp;&nbsp; velocidadMov<br>
4.- usara o no el disparador sony<br>
&nbsp;&nbsp;&nbsp;&nbsp; usarDiaparador => 0=no 1=si<br>
5.- tiempo desde que se mueve hasta que dispara<br>
&nbsp;&nbsp;&nbsp;&nbsp; offsetDis<br>
6.- duracion del disparo<br>
&nbsp;&nbsp;&nbsp;&nbsp; duracionDis<br>

# cosas que faltan por hacer
- Añadir 500ms al disparador entre el e nfocar y el disparo<br>

# Mejoras
- Menu principal<
-->Movimiento libre<br>
-->Movimiento video<br>
&nbsp;&nbsp;&nbsp;&nbsp; Selección posicion inicial.<br>
&nbsp;&nbsp;&nbsp;&nbsp; Selección posición final.<br>
&nbsp;&nbsp;&nbsp;&nbsp; Selección duración en segundos. (lo que ha tardado minimo)<br>
&nbsp;&nbsp;&nbsp;&nbsp; Loop --> ir y volver.<br>
&nbsp;&nbsp;&nbsp;&nbsp; Inicio --> si no hay loop pregunta si quieres repetir la toma, puedes cambiar la duración en segundos.<br><br>
-->Movimiento Time Lapse<br>
