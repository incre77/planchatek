Una placa base para impremir en una sola cara y conseguir una base para construir un faketek en casa:

El metodo de construccion de una pcb casera con una impresora laser lo podeis ver en varios videos de youtube.

Planchatek: promicro nrf52840 + ( ra01sh or ht-ra62 ) + nice nano v2 promicro nrf52840 +  medidor batt% + cn3791 + xb8789d0 + attiny.

el xb8789d0 se encarga de cortar la energia al promicro cuando el voltaje cae a 2.9v
el attiny se debe programar para enviar un pulso de 200ms cada 24h para resetear el promicro.
el cn3791 es un modulo cargador solar para la bateria ion-litio.

Los componentes smd son todos 1206 y su valor está anotado en la misma placa.

<img width="787" height="844" alt="picture" src="https://github.com/user-attachments/assets/44118a23-9c30-4fa3-b461-b3f41a423121" />


