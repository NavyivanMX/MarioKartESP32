# MarioKart ESP32 RC

# Hardware

**Version:** 1.1.0 RC1  
**Author:** Narciso Ivan Cisneros Acosta

---

# 1. Objetivo

Este documento describe la arquitectura física del vehículo.

Define:

- Componentes electrónicos.
- Alimentación.
- Controladores de motores.
- Distribución eléctrica.
- Conexiones entre módulos.

Su propósito es servir como referencia durante el ensamblaje y futuras
revisiones del hardware.

---

# 2. Arquitectura General

```text
                   +----------------------+
                   |      Batería 2S      |
                   |      7.4 V LiPo      |
                   +----------+-----------+
                              |
               +--------------+--------------+
               |                             |
               |                             |
               ▼                             ▼
      +----------------+            +----------------+
      | Buck Converter |            |   TB6612 #1    |
      |     5 V        |            |   Tracción     |
      +-------+--------+            +-------+--------+
              |                             |
              |                             |
              ▼                             ▼
        +-------------+            Motor Izquierdo
        |    ESP32    |            Motor Derecho
        +------+------+ 
               |
               |
               ▼
        +--------------+
        |  TB6612 #2   |
        |   Gravity    |
        +------+-------+
               |
               ▼
        Motor Gravity
```

---

# 3. Componentes

## Microcontrolador

| Elemento | Valor |
|----------|-------|
| Modelo | ESP32 DevKit V1 |
| MCU | ESP32-WROOM-32 |
| Voltaje | 3.3 V |
| Comunicación | ESP-NOW |

---

## Driver de Motores

Modelo:

TB6612FNG

Se utilizarán dos módulos.

| Driver | Función |
|---------|----------|
| TB6612 #1 | Motores de tracción |
| TB6612 #2 | Motor Gravity |

---

## Motores

Tres motores DC.

| Motor | Función |
|--------|----------|
| Izquierdo | Tracción |
| Derecho | Tracción |
| Gravity | Cambio de modo |

---

# 4. Alimentación

El sistema se divide en dos dominios eléctricos.

```text
                Batería

                   │

          +--------+--------+

          │                 │

          ▼                 ▼

     Motores             Buck 5 V

                              │

                              ▼

                            ESP32
```

Esto evita que los picos de corriente de los motores afecten al
microcontrolador.

---

# 5. Batería

Se recomienda utilizar:

## Opción 1

LiPo 2S

```text
7.4 V
```

---

## Opción 2

Dos celdas 18650

```text
7.4 V
```

---

No se recomienda alimentar los motores desde USB.

---

# 6. Regulación

La lógica deberá alimentarse mediante un convertidor Buck.

```text
Entrada

7.4 V

↓

Buck Converter

↓

5 V

↓

ESP32
```

El ESP32 generará internamente los 3.3 V necesarios para la lógica.

---

# 7. Distribución de potencia

## VM

Alimenta motores.

Proviene directamente de la batería.

---

## VCC

Alimenta la lógica del TB6612.

Proviene del ESP32.

```text
ESP32 3.3 V

↓

VCC TB6612
```

---

# 8. TB6612FNG #1

Controla la tracción.

```text
              TB6612

        Canal A

Motor Izquierdo

        Canal B

Motor Derecho
```

---

# 9. TB6612FNG #2

Controla Gravity.

```text
            Canal A

Motor Gravity


            Canal B

Reservado
```

El segundo canal queda disponible para futuras expansiones.

---

# 10. PWM

El proyecto utilizará LEDC.

No se utilizará analogWrite().

Configuración.

| Parámetro | Valor |
|------------|--------|
| Frecuencia | 20 kHz |
| Resolución | 8 bits |

Canales.

| Canal | Motor |
|---------|--------|
| LEDC 0 | Izquierdo |
| LEDC 1 | Derecho |
| LEDC 2 | Gravity |

---

# 11. Capacitores

Se recomienda colocar un capacitor cerámico en cada motor.

```text
100 nF

Motor

+ ───||─── -

```

Reduce ruido eléctrico.

---

Además.

Un capacitor electrolítico.

```text
470 µF

ó

1000 µF
```

Entre VM y GND del TB6612.

---

# 12. Tierra

Todos los módulos deberán compartir tierra.

```text
ESP32

GND

─────────────┐

TB6612 #1

GND

─────────────┤

TB6612 #2

GND

─────────────┤

Buck

GND

─────────────┤

Batería

-
```

Una tierra común es indispensable para el correcto funcionamiento.

---

# 13. Interruptor General

Se recomienda colocar un interruptor antes del Buck Converter.

```text
Batería

↓

Interruptor

↓

Buck

↓

ESP32
```

Y otro para VM si se desea desconectar únicamente los motores.

---

# 14. Protección

Se recomienda incluir.

- Fusible.
- Protección contra inversión de polaridad.
- Capacitor de filtrado.
- Cableado AWG adecuado.

---

# 15. Expansiones

La arquitectura fue diseñada para permitir futuras funciones.

Ejemplos.

- OLED
- IMU
- Encoder
- Sensor Hall
- Luces
- Bocina
- Bluetooth
- OTA
- Telemetría

Sin modificar el hardware principal.

---

# 16. BOM (Bill of Materials)

| Cantidad | Componente |
|----------|------------|
| 2 | ESP32 DevKit V1 |
| 2 | TB6612FNG |
| 3 | Motor DC |
| 1 | Batería LiPo 2S |
| 1 | Buck Converter 7.4 → 5 V |
| 1 | Interruptor |
| 3 | Capacitor cerámico 100 nF |
| 1 | Capacitor electrolítico 470–1000 µF |
| 1 | Fusible |
| 1 | Conector XT30 o XT60 |
| Varios | Cables Dupont y terminales |

---

# 17. Diseño futuro

Este documento servirá como base para:

- Diseño del esquemático.
- Diseño de PCB.
- Ensamblaje del prototipo.
- Validación eléctrica.
- Manual de mantenimiento.

Toda modificación al hardware deberá actualizar este documento.