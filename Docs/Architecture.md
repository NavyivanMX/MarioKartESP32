MarioKart ESP32 RC
Hardware Design v1.0
Objetivo

Definir la arquitectura de hardware del Receiver para garantizar:

Modularidad.
Facilidad de mantenimiento.
Compatibilidad con futuras expansiones.
Máximo aprovechamiento del ESP32.

1. Arquitectura general
                     ┌──────────────────────────┐
                     │        ESP32 DevKit      │
                     │                          │
                     │      ESP-NOW Receiver    │
                     └─────────────┬────────────┘
                                   │
          ┌────────────────────────┼─────────────────────────┐
          │                        │                         │
          ▼                        ▼                         ▼
   TB6612FNG #1             TB6612FNG #2               GPIO Auxiliares
 Tracción principal         Sistema Gravity         LEDs, buzzer, etc.

      │      │                   │
      ▼      ▼                   ▼

 Motor Izq  Motor Der      Motor Gravity

2. Subsistemas

Comunicación

Hardware

ESP32 DevKit V1

Funciones

ESP-NOW
PWM
GPIO
Debug Serial
Tracción

Controlador

TB6612FNG #1

Motores

Motor izquierdo
Motor derecho
Gravity

Controlador

TB6612FNG #2

Motor

Motor Gravity

El segundo canal queda disponible para futuras funciones.

3. Asignación definitiva de GPIO

TB6612 #1 (Tracción)
Función	GPIO
AIN1	GPIO26
AIN2	GPIO27
PWMA	GPIO25
BIN1	GPIO33
BIN2	GPIO32
PWMB	GPIO14
STBY	GPIO5
TB6612 #2 (Gravity)
Función	GPIO
AIN1	GPIO18
AIN2	GPIO19
PWMA	GPIO23
BIN1	Libre
BIN2	Libre
PWMB	Libre
STBY	GPIO21
GPIO reservados
GPIO	Uso futuro
4	LED Estado
16	UART2 TX
17	UART2 RX
22	I²C SDA
15	I²C SCL

4. Alimentación

Batería

Recomendación

LiPo 2S
7.4 V

o

18650 x2
7.4 V
Alimentación de lógica
Batería
        │

 Buck Converter
     5V

        │

ESP32
Alimentación de motores

La batería alimenta directamente los TB6612.

Batería

    │

VM (TB6612)

    │

Motores

El ESP32 nunca debe alimentar motores.

5. Control PWM

Actualmente

analogWrite(...)

Migrar completamente a LEDC.

Propuesta:

Canal LEDC	Función
Canal 0	Motor Izquierdo
Canal 1	Motor Derecho
Canal 2	Motor Gravity

Configuración

Frecuencia

20 kHz

Resolución

8 bits

Rango

0 - 255

6. Niveles de velocidad
Stop

0

Normal

180

Turbo

255

Estos valores estarán centralizados en

MotorConfig.h

7. Flujo completo
Botones

      │

InputManager

      │

DriverCommand

      │

ESP-NOW

      │

Receiver

      │

VehicleController

      │

MotionController

      │

MotorController

      │

MotorDriver

      │

TB6612FNG

      │

Motores

8. Futuras expansiones

La arquitectura deja disponibles GPIO para incorporar:

OLED I²C
IMU MPU6050
Sensor Hall
Encoder
Telemetría
OTA
Bluetooth
Luces RGB
Buzzer
Sensor de batería

9. Lista de materiales (BOM)
Cantidad	Componente
2	ESP32 DevKit V1
2	TB6612FNG
3	Motores DC
1	Batería LiPo 2S
1	Convertidor Buck 7.4→5 V
1	Interruptor general
3	Capacitores cerámicos 100 nF (uno por motor)
1	Capacitor electrolítico 470–1000 µF cerca de los TB6612
1	Fusible adecuado al consumo del sistema
Varios	Cables, conectores y terminales
Mi sugerencia para el siguiente paso

Creo que este documento puede convertirse en la documentación oficial del hardware del proyecto. Incluso iría un paso más allá y prepararía dos entregables adicionales:

Esquemático eléctrico (tipo KiCad o dibujado a mano con estilo profesional), mostrando todas las conexiones entre ESP32, TB6612FNG, batería y motores.
Plano de cableado ("wiring diagram") con el formato típico de Fritzing o una vista simplificada, muy útil durante el montaje físico.

Así tendrías el proyecto documentado en el mismo nivel de calidad que el software: arquitectura, código y hardware perfectamente alineados. Creo que sería un excelente cierre para la fase de diseño antes de empezar a ensamblar el kart.