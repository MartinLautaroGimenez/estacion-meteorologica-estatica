# Estación Meteorológica Fija
## ¿Qué es una estación meteorológica?
Una estación meteorológica es un dispositivo tecnológico de características electrónica e informática que permite nuclear parámetros relevantes para realizar estudios y sacar conclusiones frente a fenómenos físicos.

## ¿Qué es E.M.F.?
Este es el inicio del proyecto EME, fue desarrollado en 2022 por el Alumno Nicolas Pérez de la escuela ETec UM, para posteriormente en el año 2024, tras el egreso de Nicolas de la escuela, la estación fue acogida por el taller Aeroglobe ETec y en conjunto con el nevo equipo que se creó en el taller se le pudo dar continuidad y agregar sensores y actualizar partes de la estación. 

## E.M.E.
Este es un proyecto institucional y surge de la necesidad de poder comprar los sistemas atmosféricos de algunos departamentos como Godoy Cruz, Gran Mendoza, Lavalle entre otros departamentos unidos a la red de la Escuela ETec de la Universidad de Mendoza para tener un conocimiento amplio de las condiciones atmosféricas de la provincia y así contribuir al análisis de las condiciones climáticas de la misma. Este es el nexo entre todas las estaciones y su finalidad es crear una red de nodos open surce en la cual podamos extraer datos para diferentes investigaciones o proyectos ya sean de la escuela o externos ya que los datos son públicos.  

## Los modulos incorporados
  - [ESP32](#ESP32)
  - [BMP180](#BMP180)
  - [DHT22](#DHT22)
  - [BMS 3S](#BMS-3S)
  - [Baterías 18650](#Baterías-18650)
  - [Panel Solar](#Panel-Solar)
  - [Sensor de Hoja Mojada](#Sensor-de-Hoja-Mojada)
  - [BH1750](#BH1750)
  - [MQ-135](#MQ-135)
  - [Veleta](#Veleta)
  - [Anemómetro](#Anemómetro)
 
### ESP32
#### DOIT ESP32 Devkit V1
La placa ESP32 (DOIT ESP32 Devkit V1) es un módulo basado en el microcontrolador ESP32, conocido por su capacidad de conexión Wi-Fi y Bluetooth. En nuestra estación meteorológica, esta placa desempeñará un papel vital al ofrecer una base solida para la  recopilación y transmisión de datos ambientales. Con su capacidad para manejar múltiples sensores y su compatibilidad con entornos de desarrollo como Arduino IDE, la ESP32 garantiza un funcionamiento continuo incluso en condiciones adversas, asegurando la integridad y precisión de las mediciones meteorológicas.

<div align="center">
   <img src= "https://mimitoi.com/wp-content/uploads/2024/01/D_NQ_NP_657703-MCO70581862267_072023-O.webp" width="270px">
</div>


### BMP180
El sensor BMP180 es un dispositivo el cual es utilizado para medir la presión atmosférica y la altitud a nivel del mar con un alto nivel de precisión. Es conocido por su pequeño tamaño y por su simplicidad al momento de realizar mediciones. 

<div align="center">
   <img src= "https://http2.mlstatic.com/D_NQ_NP_814857-MLA52961108355_122022-O.webp" width="270px">
</div>


### DHT22
El sensor DHT22 es un componente crucial en el campo de la electrónica y la monitorización ambiental. 
Su función principal es medir con precisión la temperatura ambiente y la humedad relativa del entorno en el que se encuentra la estación meteorológica. Este sensor incorpora un termistor para la medición de la temperatura y un elemento especializado para medir la humedad relativa.

<div align="center">
   <img src= "https://http2.mlstatic.com/D_NQ_NP_863339-MLU75748316379_042024-O.webp" width="270px">
</div>


### BMS 3S
El módulo BMS 3S es esencial para la carga de las baterías, ya que desempeña un papel crucial en la regulación del voltaje que reciben durante el proceso de carga. Este módulo garantiza que las baterías reciban el voltaje adecuado para una carga segura y eficiente.

<div align="center">
   <img src= "https://http2.mlstatic.com/D_NQ_NP_776202-MLA52962647214_122022-O.webp" width="300px">
</div>


### Baterías 18650
Las baterías 18650 son células recargables de iones de litio conocidas por su alta densidad de energía y fiabilidad. En nuestra estación meteorológica, estas baterías desempeñarán un papel vital al ofrecer una fuente de alimentación constante y duradera, lo que garantizará un funcionamiento continuo incluso en condiciones adversas en las que el panel solar no pueda suministrar la potencia nominal.

<div align="center">
   <img src= "https://http2.mlstatic.com/D_918072-MLA70473561138_072023-C.jpg" width="290px">
</div>


### Panel Solar
El panel solar le da la capacidad de poder estar lejos en un sector en donde no sea posible el tendido eléctrico para la alimentación de la EME, además brinda una opción de energía sustentable y limpia, reduciendo así el consumo eléctrico convencional que mayoritariamente proviene de centrales térmicas.
Además el mismo se le incorpora un pequeño módulo regulador de tensión para poder adaptar los niveles de tensión requeridos para la lógica del microcontrolador.

<div align="center">
   <img src= "https://enertik.com/ar/wp-content/uploads/sites/2/imagenes/paneles-solares/panel-solar-policristalino-90w-ps-90.webp" width="300px">
</div>


### Sensor de Hoja Mojada
El sensor de hoja mojada permite tomar cómo parámetro el estado de la planta en cuanto a la generación de microorganismos que se presenten en la planta, de está manera se puede regular la aplicación de insecticida sin sobredosificar y ahorrando cantidades del mismo, si el mismo fuese aplicado a calendario. Además permite tener un mejor control en cuanto a la biodiversidad de los demás microorganismos que puedan aparecer en la planta que no sean dañinos. 

<div align="center">
   <img src= "https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQlj_Hincp1OI7a1BUDK1rYzu_y2ZrrE-nU2w&s" width="300px">
</div>


### BH1750
El sensor BH1750 es un sensor el cual está diseñado para medir la intensidad de la luz de manera precisa. Es altamente valorado por su facilidad de uso y su capacidad de obtener lecturas rápidas en una amplia variedad de ambientes. Su pequeño tamaño y alta precisión lo hacen el más querido de los sensores de luz.

<div align="center">
   <img src= "https://http2.mlstatic.com/D_NQ_NP_787293-MLA52961083887_122022-O.webp" width="280px">
</div>


### MQ-135
El sensor MQ135 es un componente el cual permite detectar gases nocivos para el aire, como el amoníaco, sulfuro de hidrógeno, dióxido de carbono, y otros compuestos orgánicos volátiles. Es conocido por la capacidad de proporcionar alertas tempranas sobre la presencia de estos gases.
De este sensor podemos tomar datos analógicos del comportamiento de la mezcla de gases y la capacidad de marcar un umbral de disparo para una salida digital de ser requerida, para ello es necesario acceder manualmente al trimmer de su pcb y calibrar el disparo del mismo

<div align="center">
   <img src= "https://http2.mlstatic.com/D_NQ_NP_892623-MLA52961069097_122022-O.webp" width="330px" width="25px">
</div>


### Veleta
Permite tomar la dirección actual del viento discretizando dicha dirección en 8 posiciones cardinales gracias a su forma constructiva, un imán de neodimio y 8 reed switch.

<div align="center">
   <img src= "https://http2.mlstatic.com/D_NQ_NP_832401-MLA74569886201_022024-O.webp" width="330px">  <img src= "https://scienceprog.com/wp-content/uploads/2021/01/image-153.jpeg" width="205px">
</div>


### Anemómetro 
Para el anemómetro se requiere tanto de la parte física como del módulo o sensor de efecto hall que es quien detecta cada revolución de giro y la envía a una etapa amplificadora para poder contar cada pulso entrante 

<div align="center">
   <img src= "https://tienda.ityt.com.ar/10545-large_default/ky035-sensor-efecto-hall-analogico-49e-ss49e-arduino-itytarg.jpg" width="330px"> <img src= "https://www.automatismos-mdq.com.ar/blog/wp-content/uploads/2021/01/05.-Modelo-terminado.jpg" width="440px">
</div>