# MarioKart ESP32 Receiver - Ambiente

## Integración realizada

Se agregó:

`src/Vehicle/AmbientLightController.h`
`src/Vehicle/AmbientLightController.cpp`

Y se modificó:

`src/Core/ReceiverController.h`
`src/Core/ReceiverController.cpp`
`src/Config/Pins.h`

## Comportamiento

Cada vez que el Receiver detecta el cambio de perfil mediante `Gravity`:

1. Obtiene el nuevo `DrivingProfileId`.
2. El ambiente muestra el color del perfil.
3. Mantiene ese color durante 10 segundos.
4. Después comienza Rainbow Shift.
5. Rainbow Shift continúa indefinidamente.

Los efectos son no bloqueantes: no se usa `delay()` para la secuencia del ambiente.

## Colores

DrivingProfileId es 0-based:

- Rookie = 0 -> Verde
- Normal = 1 -> Azul
- Advanced = 2 -> Naranja
- Drift = 3 -> Violeta
- Police = 4 -> Azul

## Hardware

- ESP32
- 4 x WS2812B
- GPIO 19
- Adafruit NeoPixel

Instalar la librería:

Adafruit NeoPixel

## Importante

No se modificó el comportamiento de `RearLightController`.

El ambiente es un controlador independiente.

## Perfil + Turbo

El controlador ya incluye:

```cpp
m_ambientLights.Toggle();
```

y:

```cpp
m_ambientLights.SetEnabled(true/false);
```

pero NO se conectó todavía a Perfil + Turbo.

Esto se deja separado para que podamos decidir exactamente cómo debe funcionar
la combinación sin interferir con el turbo real del vehículo.

## Nota sobre el inicio

Al arrancar, el ambiente toma el perfil almacenado y muestra su color durante
10 segundos, después entra en Rainbow Shift.

Si prefieres que durante startup permanezca apagado y sólo se active cuando
haya un cambio de perfil, se puede cambiar con una sola línea.
