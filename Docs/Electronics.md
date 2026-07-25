# Electronics.md

# MarioKart ESP32 RC
## Diseño Electrónico

---

# Objetivo

Este documento describe la arquitectura electrónica utilizada en el proyecto
MarioKart ESP32 RC.

Su propósito es documentar todas las decisiones relacionadas con la
alimentación, protección, control de motores y conexión de los diferentes
componentes electrónicos.

---

# Filosofía del diseño

El proyecto está diseñado bajo los siguientes principios:

- Arquitectura modular.
- Electrónica sencilla.
- Componentes comerciales fáciles de conseguir.
- Bajo costo.
- Fácil mantenimiento.
- Escalable para futuras versiones.

---

# Alimentación

## Fuente principal

Durante la etapa de desarrollo se utilizarán dos baterías recargables USB de
9V conectadas en paralelo.

```
          +9V
        ┌─────────┐
        │Battery A│
        └────┬────┘
             │
             ├─────────────+
             │             │
        ┌────┴────┐        │
        │Battery B│        │
        └─────────┘        │
                           │
                     Alimentación principal
```

La conexión en paralelo permite:

- aumentar la capacidad disponible
- repartir la corriente
- mejorar la autonomía

Nunca conectar baterías con diferente nivel de carga.

---

## Alimentación del ESP32

El ESP32 no será alimentado directamente desde la línea de motores.

La alimentación recomendada será:

```
Baterías

↓

Buck Converter

↓

5V

↓

VIN ESP32
```

Esto evita reinicios provocados por ruido eléctrico.

---

## Alimentación del TB6612FNG

El pin VM del TB6612 será alimentado directamente desde la batería.

```
Batería

↓

VM
```

El pin VCC será alimentado desde 3.3V del ESP32.

```
ESP32 3.3V

↓

VCC TB6612
```

---

# Drivers de potencia

El proyecto utilizará dos módulos TB6612FNG.

## Driver 1

Control de tracción.

Motor izquierdo.

Motor derecho.

---

## Driver 2

Control del mecanismo Gravity.

Motor Gravity.

---

# Ventajas del TB6612FNG

Se eligió este puente H por las siguientes razones:

- Compatible con lógica de 3.3V.
- Mucho más eficiente que L298N.
- Baja caída de tensión.
- Bajo calentamiento.
- PWM por hardware.
- Hasta 1.2A continuos por canal.
- Muy utilizado en robots y vehículos RC.

---

# Capacitores de desacoplo

Todos los drivers deberán tener desacoplo cercano.

## Cerámico

100nF

Ubicación:

VM ↔ GND

Función:

Eliminar ruido de alta frecuencia.

---

## Electrolítico

470µF

Ubicación:

VM ↔ GND

Función:

Absorber picos de corriente durante el arranque de los motores.

---

# Capacitores del ESP32

Se recomienda colocar:

100nF

entre

3.3V y GND

cerca del ESP32.

---

# Resistencias utilizadas

## 330Ω

Uso:

LEDs de diagnóstico.

---

## 10KΩ

Uso:

Pull-up.

Pull-down.

---

## 47KΩ

Uso:

Divisores de voltaje.

---

## 100KΩ

Uso:

Lectura de batería mediante ADC.

---

# PWM

El proyecto utilizará exclusivamente LEDC del ESP32.

No se utilizará analogWrite().

Ventajas:

- PWM por hardware.
- Mayor resolución.
- Frecuencia configurable.
- Mejor rendimiento.

---

# Protección eléctrica

Se recomienda:

- Nunca desconectar motores energizados.
- Compartir siempre la tierra (GND).
- No alimentar motores desde el ESP32.
- Utilizar cables cortos para alimentación.
- Separar cables de potencia y señales.

---

# Colores recomendados

Rojo

+V

Negro

GND

Azul

Motor izquierdo

Verde

Motor derecho

Amarillo

Gravity

Blanco

PWM

Naranja

GPIO

---

# Buenas prácticas

Siempre medir continuidad antes de energizar.

Nunca conectar el TB6612 sin GND común.

No invertir polaridad.

Verificar alimentación antes del primer encendido.

---

# Lista de componentes

ESP32 DevKit V1

2 × TB6612FNG

Motores originales Mario Kart

2 × Baterías recargables USB 9V

Buck Converter

Capacitores cerámicos

Capacitores electrolíticos

Resistencias SMD

Protoboard

Jumpers

---

# Futuras mejoras

- Medición de batería mediante ADC.
- Protección contra inversión de polaridad.
- Fusible rearmable (PTC).
- Sensor de corriente.
- PCB personalizada.
- Indicador de batería baja.
- Encendido mediante interruptor electrónico.
