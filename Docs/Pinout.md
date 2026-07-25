# MarioKart ESP32 RC

# Pinout

**Version:** 1.1.0 RC1  
**Author:** Narciso Ivan Cisneros Acosta

---

# 1. Objetivo

Este documento define la asignación oficial de los GPIO utilizados por el
Receiver.

Su propósito es centralizar la documentación del hardware y evitar conflictos
entre módulos durante el desarrollo.

Toda modificación en la asignación de pines deberá reflejarse tanto en este
documento como en el archivo `Pins.h`.

---

# 2. Plataforma

| Componente | Valor |
|------------|-------|
| MCU | ESP32 DevKit V1 |
| SoC | ESP32-WROOM-32 |
| Voltaje lógico | 3.3 V |
| Comunicación | ESP-NOW |
| Framework | Arduino-ESP32 |

---

# 3. Distribución General

```text
                    ESP32 DevKit V1

        +---------------------------------------+

        GPIO25  ---> PWM Motor Izquierdo

        GPIO26  ---> IN1 Motor Izquierdo

        GPIO27  ---> IN2 Motor Izquierdo


        GPIO14  ---> PWM Motor Derecho

        GPIO33  ---> IN1 Motor Derecho

        GPIO32  ---> IN2 Motor Derecho


        GPIO23  ---> PWM Motor Gravity

        GPIO18  ---> IN1 Gravity

        GPIO19  ---> IN2 Gravity


        GPIO5   ---> STBY TB6612 Tracción

        GPIO21  ---> STBY TB6612 Gravity


        GPIO13  ---> Gravity Enable (Reservado)

        +---------------------------------------+
```

---

# 4. TB6612FNG #1

## Tracción Principal

Este controlador administra los motores de tracción.

| Función | GPIO |
|----------|------|
| PWMA | GPIO25 |
| AIN1 | GPIO26 |
| AIN2 | GPIO27 |
| PWMB | GPIO14 |
| BIN1 | GPIO33 |
| BIN2 | GPIO32 |
| STBY | GPIO5 |

Controla:

- Motor izquierdo
- Motor derecho

---

# 5. TB6612FNG #2

## Sistema Gravity

Este controlador administra el motor encargado del mecanismo Gravity.

Actualmente únicamente se utiliza el canal A.

| Función | GPIO |
|----------|------|
| PWMA | GPIO23 |
| AIN1 | GPIO18 |
| AIN2 | GPIO19 |
| STBY | GPIO21 |

Canal B:

Reservado para futuras expansiones.

---

# 6. GPIO Reservados

Los siguientes pines permanecen disponibles para futuras versiones.

| GPIO | Uso previsto |
|------|--------------|
| GPIO4 | LED de estado |
| GPIO15 | I²C SCL |
| GPIO22 | I²C SDA |
| GPIO16 | UART2 TX |
| GPIO17 | UART2 RX |

---

# 7. GPIO No Utilizados

Actualmente permanecen libres.

| GPIO |
|------|
| GPIO2 |
| GPIO12 |
| GPIO34* |
| GPIO35* |
| GPIO36* |
| GPIO39* |

\* Entradas únicamente.

---

# 8. GPIO Reservados por el ESP32

Los siguientes pines no deben utilizarse.

| GPIO | Motivo |
|------|--------|
| GPIO6 | Flash SPI |
| GPIO7 | Flash SPI |
| GPIO8 | Flash SPI |
| GPIO9 | Flash SPI |
| GPIO10 | Flash SPI |
| GPIO11 | Flash SPI |

---

# 9. Alimentación

## ESP32

Alimentación:

- 5 V por USB o pin VIN.
- Regulación interna a 3.3 V.

---

## TB6612FNG

VM:

Alimentación de motores.

Recomendado:

7.4 V (LiPo 2S)

VCC:

3.3 V desde el ESP32.

---

# 10. PWM

El proyecto utilizará LEDC como controlador oficial de PWM.

Configuración prevista.

| Canal LEDC | GPIO | Función |
|-------------|------|----------|
| Canal 0 | GPIO25 | Motor izquierdo |
| Canal 1 | GPIO14 | Motor derecho |
| Canal 2 | GPIO23 | Gravity |

Configuración común.

Frecuencia:

20 kHz

Resolución:

8 bits

Duty Cycle:

0–255

---

# 11. Expansiones Futuras

La distribución de GPIO permite agregar.

- Pantalla OLED I²C
- IMU MPU6050
- Sensor Hall
- Encoders
- Luces RGB
- Bocina
- Sensor de batería
- Telemetría
- OTA

Sin modificar el hardware principal.

---

# 12. Convenciones

Todos los GPIO deberán definirse exclusivamente dentro de:

```text
Receiver/
    src/
        Config/
            Pins.h
```

No deberán utilizarse valores numéricos directamente dentro de los drivers.

Ejemplo correcto:

```cpp
pinMode(Pins::LeftMotorIn1, OUTPUT);
```

Ejemplo incorrecto:

```cpp
pinMode(26, OUTPUT);
```

---

# 13. Estado

| Subsistema | Estado |
|------------|--------|
| Tracción | Definido |
| Gravity | Definido |
| ESP-NOW | Definido |
| PWM LEDC | Pendiente |
| PCB | Pendiente |