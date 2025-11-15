Una placa base para imprimir en una sola cara y conseguir una base para construir un faketek en casa:

El metodo de construccion de una pcb casera con una impresora laser lo podeis ver en varios videos de youtube. Ejemplo https://www.youtube.com/watch?v=MX9_9U6Wnu8

Planchatek: ( ra01sh or ht-ra62 ) + nice nano v2 promicro nrf52840 +  medidor batt% + cn3791 + xb8789d0 + attiny.

el xb8789d0 se encarga de cortar la energia al promicro cuando el voltaje cae a 2.9v
el attiny se debe programar para enviar un pulso de 200ms cada 24h para resetear el promicro.
el cn3791 es un modulo cargador solar para la bateria ion-litio.

Los componentes smd son todos 1206 y su valor está anotado en la misma placa.

Añadi las instrucciones del attiny en un documento pdf. En la pcb añadi dos jumpers para modificar el comportamiento del attiny, un jumper es para poner el attiny en modo pruebas y que resetee en un minuto y el otro jumper duplica los tiempos de manera que en modo pruebas resetea en 2 minutos y en modo normal resetea en 24horas en vez de 12 sin el jumper.
En modo de pruebas el attiny emite pulsos en otros patillas para mostrar el tiempo restante a traver de leds. Mas info en el pdf.

-----

A single-sided PCB (Printed Circuit Board) to create a base for building a faketek at home: 
You can find videos on various YouTube channels demonstrating the method for constructing a homemade PCB using a laser printer. Example: https://www.youtube.com/watch?v=MX9_9U6Wnu8

Planchatek: (ra01sh or ht-ra62) + nice nano v2 promicro nrf52840 + batt% meter + cn3791 + xb8789d0 + attiny.

The xb8789d0 is responsible for cutting power to the promicro when the voltage drops to 2.9V. The attiny must be programmed to send a 200ms pulse every 24 hours to reset the promicro. The cn3791 is a solar charger module for the lithium-ion battery. The SMD (Surface-Mount Device) components are all 1206 size, and their value is noted on the board itself.

I added the attiny instructions in a pdf document. In the pcb I added two jumnpers to modify the behavior of the attiny one jumper is to put the attiny in test mode and reset in one minute and the other jumper doubles the times so that in test mode it resets in 2 minutes and in normal mode it resets in 24 hours instead of 12 without the jumper. In test mode the attiny pulses on other pins to show the remaining time through LEDs. More info in the pdf.

<img width="632" height="715" alt="Captura desde 2025-11-05 21-55-38" src="https://github.com/user-attachments/assets/2c6efb78-5fce-4874-a1df-b7d3f3f7ba56" />

----
ACTUALIZACION SABADO 15 NOV 2025
----

Liam, una persona encantadora de pcbway se ofreció para invitarme a unas copias de mi placa hechos por su empresa pcbway. Acepté y gracias a ello pude descubrir algunos errores de diseño que mejoraron este proyecto.

La verdad es que este proyecto esta pensando para que cada cual haga su placa en casa pero la diferencia es notable, al menos para mi que no soy un experto haciendo placas. Así que si no quieres ponerte a hacerla y en forma de agradecimiento a pcbway podeis mandar a pedir este placa en su empresa que las hace muy bien.  www.pcbway.com

![20251115_113653](https://github.com/user-attachments/assets/9be7edb6-d3a4-492b-ad0a-e1b6e694ad2d)

Mejoras realizadas:
- Rediseño de las dimensiones de la placa y recolocación del promicro para que se pueda conectar bien el usb. :P
- Correción en el circuito de deteccion de voltaje
- Modificacion de las opciones del reset para que sea de 24, 12, 6 horas o 1 munuto.
- Corrección de la conexión sda y scl e introduccion del sensor bmp280
- Compatibilidad con los modulos de carga solar cn3791 y cn3065

<img width="581" height="697" alt="image" src="https://github.com/user-attachments/assets/d8a79fce-be4b-4f60-ac11-244e997226c1" />

- Nuevo gerbers y nuevo archivo pdf subido con las correciones citadas.



