# Estación Meteorológica Estática
## ¿Qué es una estación meteorológica?
Una estación meteorológica es un dispositivo tecnológico de características electrónica e informática que permite nuclear parámetros relevantes para realizar estudios y sacar conclusiones frente a fenómenos físicos.
## ¿Qué es EME?
Este es un proyecto institucional y surge de la necesidad de poder comparar los biomas entre Godoy Cruz, Mendoza y el departamento de lavalle puntualmente entre el colegio ETec de la universidad de Mendoza y los diversos puntos que parada que se tienen en otro proyecto institucional de salida a lavalle llamado “Viaje a las Raíces”.
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
<img src= "" width="30px">
### BMP180
El sensor BMP180 es un dispositivo el cual es utilizado para medir la presión atmosférica y la altitud a nivel del mar con un alto nivel de precisión. Es conocido por su pequeño tamaño y por su simplicidad al momento de realizar mediciones. 
<img src= "" width="30px">
### DHT22
El sensor DHT22 es un componente crucial en el campo de la electrónica y la monitorización ambiental. 
Su función principal es medir con precisión la temperatura ambiente y la humedad relativa del entorno en el que se encuentra la estación meteorológica. Este sensor incorpora un termistor para la medición de la temperatura y un elemento especializado para medir la humedad relativa.
<img src= "" width="30px">
### BMS 3S
El módulo BMS 3S es esencial para la carga de las baterías, ya que desempeña un papel crucial en la regulación del voltaje que reciben durante el proceso de carga. Este módulo garantiza que las baterías reciban el voltaje adecuado para una carga segura y eficiente.
<img src= "" width="30px">
### Baterías 18650
Las baterías 18650 son células recargables de iones de litio conocidas por su alta densidad de energía y fiabilidad. En nuestra estación meteorológica, estas baterías desempeñarán un papel vital al ofrecer una fuente de alimentación constante y duradera, lo que garantizará un funcionamiento continuo incluso en condiciones adversas en las que el panel solar no pueda suministrar la potencia nominal.
<img src= "" width="30px">
### Panel Solar
El panel solar le da la capacidad de poder estar lejos en un sector en donde no sea posible el tendido eléctrico para la alimentación de la EME, además brinda una opción de energía sustentable y limpia, reduciendo así el consumo eléctrico convencional que mayoritariamente proviene de centrales térmicas.
Además el mismo se le incorpora un pequeño módulo regulador de tensión para poder adaptar los niveles de tensión requeridos para la lógica del microcontrolador.
<img src= "" width="30px">
### Sensor de Hoja Mojada
El sensor de hoja mojada permite tomar cómo parámetro el estado de la planta en cuanto a la generación de microorganismos que se presenten en la planta, de está manera se puede regular la aplicación de insecticida sin sobredosificar y ahorrando cantidades del mismo, si el mismo fuese aplicado a calendario. Además permite tener un mejor control en cuanto a la biodiversidad de los demás microorganismos que puedan aparecer en la planta que no sean dañinos. 
<img src= "" width="30px">
### BH1750
El sensor BH1750 es un sensor el cual está diseñado para medir la intensidad de la luz de manera precisa. Es altamente valorado por su facilidad de uso y su capacidad de obtener lecturas rápidas en una amplia variedad de ambientes. Su pequeño tamaño y alta precisión lo hacen el más querido de los sensores de luz.
<img src= "" width="30px">
### MQ-135
El sensor MQ135 es un componente el cual permite detectar gases nocivos para el aire, como el amoníaco, sulfuro de hidrógeno, dióxido de carbono, y otros compuestos orgánicos volátiles. Es conocido por la capacidad de proporcionar alertas tempranas sobre la presencia de estos gases.
<img src= "" width="30px">
De este sensor podemos tomar datos analógicos del comportamiento de la mezcla de gases y la capacidad de marcar un umbral de disparo para una salida digital de ser requerida, para ello es necesario acceder manualmente al trimmer de su pcb y calibrar el disparo del mismo
<img src= "" width="30px">
### Veleta
Permite tomar la dirección actual del viento discretizando dicha dirección en 8 posiciones cardinales gracias a su forma constructiva, un imán de neodimio y 8 reed switch.
<img src= "" width="30px">
### Anemómetro 
Para el anemómetro se requiere tanto de la parte física como del módulo o sensor de efecto hall que es quien detecta cada revolución de giro y la envía a una etapa amplificadora para poder contar cada pulso entrante 
<img src= "https://tienda.ityt.com.ar/10545-large_default/ky035-sensor-efecto-hall-analogico-49e-ss49e-arduino-itytarg.jpg" width="330px"> <img src= "https://www.automatismos-mdq.com.ar/blog/wp-content/uploads/2021/01/05.-Modelo-terminado.jpg" width="330px">
