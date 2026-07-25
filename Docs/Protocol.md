# MarioKart ESP32 RC

# Protocol

**Version:** 1.1.0 RC1  
**Author:** Narciso Ivan Cisneros Acosta

---

# 1. Objetivo

Este documento define el protocolo de comunicación utilizado entre el
Transmitter y el Receiver mediante ESP-NOW.

El protocolo tiene como objetivo transmitir únicamente el estado actual
del control del vehículo mediante una estructura compacta, simple y
determinística.

Toda modificación al protocolo deberá realizarse dentro del proyecto
MKShared para garantizar la compatibilidad entre ambos dispositivos.

---

# 2. Arquitectura

```text
              Transmitter

                   │

            DriverCommand

                   │

              ESP-NOW

                   │

            DriverCommand

                   │

               Receiver
```

El protocolo es completamente unidireccional.

El Transmitter genera el comando.

El Receiver únicamente lo interpreta.

---

# 3. DriverCommand

Toda la comunicación del sistema gira alrededor de una única estructura.

```cpp
struct DriverCommand
{
    Types::Vehicle::Direction direction;

    Types::Vehicle::Steering steering;

    Types::Vehicle::Turbo turbo;

    Types::Vehicle::DriveMode driveMode;
};
```

Esta estructura representa completamente el estado solicitado para el
vehículo.

---

# 4. Tamaño del paquete

Cada miembro corresponde a un enum de tipo `std::uint8_t`.

Por lo tanto:

| Campo | Bytes |
|--------|------:|
| Direction | 1 |
| Steering | 1 |
| Turbo | 1 |
| DriveMode | 1 |

Total:

```text
4 Bytes
```

El Receiver validará que el tamaño recibido sea exactamente:

```cpp
sizeof(DriverCommand)
```

En caso contrario el paquete será descartado.

---

# 5. Direction

Representa el movimiento longitudinal del vehículo.

| Valor | Descripción |
|--------|-------------|
| Stop | Vehículo detenido |
| Forward | Avanzar |
| Reverse | Retroceder |

Visualmente.

```text
Stop

     □

Forward

     ↑

Reverse

     ↓
```

---

# 6. Steering

Representa la dirección de giro.

| Valor | Descripción |
|--------|-------------|
| Straight | Sin giro |
| Left | Girar izquierda |
| Right | Girar derecha |

Visualmente.

```text
Left

   ↖

Straight

   ↑

Right

   ↗
```

---

# 7. Turbo

Indica la velocidad solicitada.

| Valor | Descripción |
|--------|-------------|
| Disabled | Velocidad normal |
| Enabled | Máxima velocidad |

Este campo será utilizado por el MotorDriver para seleccionar el duty
cycle PWM correspondiente.

---

# 8. DriveMode

Representa el modo físico del vehículo.

| Valor | Descripción |
|--------|-------------|
| Normal | Configuración estándar |
| Gravity | Configuración Gravity |

Este valor será procesado exclusivamente por GravityController.

---

# 9. Combinaciones

El protocolo permite representar cualquier combinación de movimiento.

Ejemplos.

## Avanzar

```text
Direction = Forward

Steering = Straight
```

---

## Retroceder

```text
Direction = Reverse

Steering = Straight
```

---

## Girar izquierda

```text
Direction = Stop

Steering = Left
```

---

## Girar derecha

```text
Direction = Stop

Steering = Right
```

---

## Avanzar izquierda

```text
Direction = Forward

Steering = Left
```

---

## Avanzar derecha

```text
Direction = Forward

Steering = Right
```

---

## Retroceder izquierda

```text
Direction = Reverse

Steering = Left
```

---

## Retroceder derecha

```text
Direction = Reverse

Steering = Right
```

---

# 10. Flujo de comunicación

```text
Botones

     │

InputManager

     │

DriverCommand

     │

ESPNowHandler

     │

ESP-NOW

     │

ESPNowReceiver

     │

VehicleController

     │

MotionController

     │

MotorController
```

---

# 11. Validaciones

El Receiver realiza las siguientes verificaciones.

## Longitud

```cpp
len == sizeof(DriverCommand)
```

Si la longitud es incorrecta el paquete se descarta.

---

## Dirección MAC

Se valida que el origen corresponda al Transmitter autorizado.

Si la MAC no coincide el paquete será ignorado.

---

## Integridad

Una vez superadas las validaciones:

```cpp
std::memcpy(
    &m_command,
    data,
    sizeof(DriverCommand));
```

El nuevo comando queda disponible para el resto del sistema.

---

# 12. Frecuencia de transmisión

El Transmitter únicamente envía un paquete cuando el estado del control
ha cambiado.

Ejemplo.

```text
Forward

↓

(no cambia)

↓

No transmite

↓

Forward

↓

No transmite

↓

Stop

↓

Transmite
```

Esta estrategia reduce considerablemente el tráfico inalámbrico.

---

# 13. Compatibilidad

Toda modificación al protocolo deberá realizarse dentro del proyecto
MKShared.

No deberán existir estructuras duplicadas entre:

- Transmitter
- Receiver

La estructura DriverCommand constituye la única fuente de verdad del
protocolo.

---

# 14. Versionado

La compatibilidad entre dispositivos depende de que ambos utilicen la
misma versión de MKShared.

Si se agrega un nuevo campo al DriverCommand deberá incrementarse la
versión del protocolo.

---

# 15. Estado actual

| Elemento | Estado |
|----------|--------|
| DriverCommand | Implementado |
| Direction | Implementado |
| Steering | Implementado |
| Turbo | Implementado |
| DriveMode | Implementado |
| Validación de tamaño | Implementado |
| Validación MAC | Implementado |
| ESP-NOW | Implementado |

---

# 16. Evolución futura

El protocolo fue diseñado para mantenerse pequeño y eficiente.

En futuras versiones podrán agregarse nuevos mensajes sin modificar la
estructura principal del DriverCommand.

Ejemplos.

- Telemetría
- Estado de batería
- Diagnóstico
- Configuración remota
- Actualización OTA
- Estadísticas de comunicación

Cada nuevo tipo de mensaje deberá documentarse en este archivo.