# SleepMonitorGIDS4092

## Integrantes del equipo
- Cárdenas Baeza Diego Emmanuel
- Mendoza García Fernando
- Ortega Amaro Angel de Jesús

## Visión
Crear un dispositivo portátil que permita a los usuarios obtener información detallada sobre la calidad de su sueño mediante múltiples sensores. Al combinar estos datos con un algoritmo personalizado, el dispositivo proporcionará recomendaciones individuales para mejorar los hábitos de sueño, superando a otros monitores de sueño que solo se enfocan en aspectos básicos.

## Objetivo General
Crear un dispositivo portable (wearable) el cual permita al usuario conocer mas aspectos acerca de que tan bien está durmiendo, esto mediante los multiples sensores elegidos para poder medir aspectos importantes del sueño del humano. De esta forma, combinando los datos obtenidos mediante el dispositivo con un algorimto diseñado para valorar la calidad del sueño en función de los datos obtenidos, se podrán hacer recomendaciones personalizadas por usuario.

### Objetivos específicos
- 1.- Interpretar esquemas eléctricos y realizar el montaje a partir de este, utilizando para ello distintos materiales.
- 2.- Conocer y utilizar los conocimientos previos para poder realizar un algoritmo que interprete los datos obtenidos.
- 3.- Conocer los principios, elementos y aplicaciones básicas de distintos sistemas de control eléctricos.
- 4.- Representar a través de graficos las estadisticas de sueño del usuario.

## Tabla del software utilizado
| id |  Software          |  Versión  |  Tipo  |
|----|--------------------|-----------|--------|
|  1 | Arduino IDE        |  1.71.2   |  IDE   |
|  2 | Visual Studio Code |  2021.3   |  IDE   |
|    |                    |           |        |

## Tabla con el hardware utilizado
| Id | Componente | Descripción |                                          Imagen                                                   | Cantidad | Costo total |
|----|------------|-------------|---------------------------------------------------------------------------------------------------|----------|-------------|
| 1  |   ESP32    | El módulo ESP32 es una solución de Wi-Fi/Bluetooth todo en uno, integrada y certificada que proporciona no solo la radio inalámbrica, sino también un procesador integrado con interfaces para conectarse con varios periféricos|![image](https://user-images.githubusercontent.com/99991865/192933367-82d60a6b-83a0-4d76-9aa7-742fb42d2d3c.png)|   1      |   165       
| 2  |Led verdes  |Un diodo LED es un dispositivo que permite el paso de corriente en un solo sentido y que al ser polarizado emite un haz de luz |![image](https://user-images.githubusercontent.com/99991865/192932983-84794e7e-a316-4e55-825b-cfb9cd3e808a.png)|   1      |   15          |
| 4  |Led amarillo|Un diodo LED es un dispositivo que permite el paso de corriente en un solo sentido y que al ser polarizado emite un haz de luz|![image](https://user-images.githubusercontent.com/99991865/192933020-9a90e9b7-8c89-410f-a89d-7dc7f99814ef.png)|   1      |      15     |
| 5  |Led rojo    |Un diodo LED es un dispositivo que permite el paso de corriente en un solo sentido y que al ser polarizado emite un haz de luz|![image](https://user-images.githubusercontent.com/99991865/192933021-f55b80cb-b9ae-44ac-99c5-23cd6ff9bc79.png)|   1      |      15     |
| 6  |Resistencias de 1k|se utilizan en los circuitos para limitar el valor de la corriente o para fijar el valor de la tensión, según la Ley de Ohm|![image](https://user-images.githubusercontent.com/99991865/192933039-0b857725-d306-4ced-ae16-679589196f19.png)|   5      |  10         |
| 7  |Protoboard  |es un tablero con orificios que se encuentran conectados eléctricamente entre sí de manera interna, habitualmente siguiendo patrones de líneas, en el cual se pueden insertar componentes electrónicos, cables para el armado|![image](https://user-images.githubusercontent.com/99991865/192933683-d1955de9-b1fa-4063-8218-b822c8b7eedf.png)|   1      |     110        |
| 8  |Cables     |sirve para conectar todos los compenetes a la protoboard|![image](https://user-images.githubusercontent.com/99991865/192934122-6938a1d7-55a6-4caa-85c9-80a27cf06301.png)|   5      |    55        |
| 9  |sensor DHT11|Dispositivo que detecta la temperatura y humedad ambiente.|![image](https://github.com/AngelOrtega26/SleepMonitorGIDS4092/blob/main/descargar%20(1).jpeg)|   1      |    85        |
| 10 |sensor de pulso|Dispositivo que detecta el pulso de un humano.|![image](https://github.com/AngelOrtega26/SleepMonitorGIDS4092/blob/main/e250cb_379ae2060325444ba1b4c287005a3e40~mv2.jpg)|   1      |    85        |
| 11 |Giroscopio|Dispositivo que detecta el movimiento.|![image](https://github.com/AngelOrtega26/SleepMonitorGIDS4092/blob/main/images.jpeg)|   1      |    85        |

## Epicas del proyecto
- El proyecto debe detectar si hay movimiento durante la fase del sueño
- El proyecto debe medir el porcentaje de oxigenación en sangre
- El proyecto debe medir la temperatura del ambiente en el que se está durmiendo
- El proyecto debe combinar estos parámetros para poder mostrar informacion útil posteriormente en la aplicación
- El proyecto debe alertar mediante la aplicación movil cuando haya un cambio de temperatura drástico, para que de esta forma el usuario pueda tomar medidas

## Tabla de historias de usuario
| Id | Historia de usuario | Prioridad | Estimación | Como probarlo | Responsable |
|----|---------------------|-----------|------------|---------------|-------------|
| 1  | Funcionalidad del proyecto                    |  Alta         | 0.5           |Realizar pruebas correspondientes en un entorno controlado para comprobar que su funcionamiento sea el correcto.            |Integrantes de equipo             |
| 2  |Funcionalidad de los sensores y actuadores   | Alta          | 0.5          | Realizar pruebas con algún usuario para verificar que todo funciona correctamente. | Integrantes de equipo |
| 3  | Funcionalidad de los leds             | Media         |  0.3          | Realizar pruebas en el aplicativo para verificar que el flujo de esta es correcto y útil para el usuario             |   integrantes del equipo          |
| 4 | Funcionalidad de las notificaciones | Baja | 0.5 | Realizar pruebas en un entorno controlado para de esta forma poder verificar la funcionalidad | Integrantes del equipo |

## Prototipo en dibujo

![image](https://user-images.githubusercontent.com/99991865/192936794-274c5f21-6002-47d9-a9e5-a5c583eff731.png)

# Video de prueba

![video](https://github.com/AngelOrtega26/SleepMonitorGIDS4092/blob/main/WearablePrototype.mp4)

# Proceso de desarrollo

## Código de arduino
![image](https://github.com/user-attachments/assets/7d62997a-f303-49ab-8f4a-ee211aaac80c)

## Nodos de node-red
![image](https://github.com/user-attachments/assets/7f402327-b4d7-4c51-b751-b745d39f6f9f)
![image](https://github.com/user-attachments/assets/e60bb00a-c5eb-4316-92a1-69ee6cb70dcc)
![image](https://github.com/user-attachments/assets/27a19e5e-b6f8-4580-8d57-4e4ba07c5541)
![image](https://github.com/user-attachments/assets/77674c4a-6de5-4d3b-93fa-f4976c62709b)
![image](https://github.com/user-attachments/assets/efc778bb-ecc1-4f45-a5e2-8dbf73e68c62)

## Dashboard de node-red
![image](https://github.com/user-attachments/assets/9d58c654-f8e9-41a8-ac0f-329631d64569)
![image](https://github.com/user-attachments/assets/f4529237-bf90-47fc-9bd2-56ed7ac2e03c)

## Funciones agregadas en node-red
![image](https://github.com/user-attachments/assets/08a65903-8d57-4993-99ad-0f6f0854c006)

## Código de Flutter
<img width="1440" alt="image" src="https://github.com/user-attachments/assets/2636927c-453e-45a6-86e4-362fd0c25797">

## Dashboard en Flutter
![image](https://github.com/user-attachments/assets/f5af95ec-6d46-4835-a5fd-13b68a720032)
![image](https://github.com/user-attachments/assets/61829158-93e7-428b-ab3c-8e92baaf47db)
![image](https://github.com/user-attachments/assets/05341c79-9e99-4caa-9a2b-2307e3bafb2f)
![image](https://github.com/user-attachments/assets/4a4aa4ec-5aef-423c-888e-70f1486ad1d1)
![image](https://github.com/user-attachments/assets/c13ee3fd-9c1d-47cc-8ce9-e425dc33524f)
![image](https://github.com/user-attachments/assets/6e1565de-5607-48ae-9dd6-fcf7d033c4ad)

## Control de actuadores Flutter
<img width="1440" alt="image" src="https://github.com/user-attachments/assets/0288f828-33bc-44ce-ab41-a4ff590274cc">
![image](https://github.com/user-attachments/assets/c137eee2-232a-4683-88f2-21cc30eb11b0)

## Base de datos
![image](https://github.com/user-attachments/assets/e2307b25-6e3d-4083-8d82-0f4157bea417)
![image](https://github.com/user-attachments/assets/a8bf0340-9145-417a-9d38-d7c0da10e502)

## Esquema
<img width="650" alt="image" src="https://github.com/user-attachments/assets/c4a3b640-bc15-4967-aeca-f7d70d1b184e">

# Videos
## Video de demostración

## Video de patrocinadora

# Carta
![image](https://github.com/user-attachments/assets/c797076a-1b65-4b9b-9bef-a41e5eba6fb1)
