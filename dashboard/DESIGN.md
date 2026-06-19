# Detalles de Diseño - Dashboard MQTT ESP32

Este documento describe la línea gráfica, decisiones estéticas, animaciones e interacciones de usuario implementadas en la interfaz de control de la casa inteligente.

## Concepto y Dirección de Diseño
El dashboard implementa una estética de **Modo Oscuro Tecnológico/Sci-Fi (Glow & Glassmorphic style)**. Está diseñado para verse sofisticado, moderno y de alta tecnología, alejándose de los estilos genéricos planos.

- **Diseño Glassmorphic (Efecto Vidrio)**: Se utiliza en la barra de navegación superior mediante `backdrop-filter: blur(16px)` y colores de fondo semi-transparentes (`rgba(...)`). Esto permite ver el sutil fondo degradado que fluye detrás.
- **Jerarquía y Estructura**: Cada componente está encapsulado dentro de una tarjeta (`dashboard-card`) con bordes oscuros finos y esquinas redondeadas suavizadas (`16px`). Esto crea un orden visual claro.

---

## Paleta de Colores (Sistema de Diseño)
La paleta se define mediante variables personalizadas CSS (`:root`) en [style.css](file:///home/eddy/Smarthome-System/dashboard/style.css):

| Variable CSS | Valor Hex/RGBA | Rol Visual |
| :--- | :--- | :--- |
| `--bg-deep` | `#080c14` | Fondo de página base. |
| `--bg-surface` | `#0f1629` | Fondo de la barra de navegación principal. |
| `--bg-card` | `#131d35` | Fondo base de las tarjetas. |
| `--bg-card-alt` | `#182544` | Fondo secundario/interno para elementos destacados. |
| `--border-glow` | `#1e2d52` | Bordes base para dar una sensación de circuito/hardware. |
| `--accent-amber` | `#f59e0b` | Alertas, presencia detectada, transiciones y acentos secundarios. |
| `--accent-green` | `#10b981` | Estados encendidos, puerta abierta y conexión activa. |
| `--accent-red` | `#ef4446` | Estados apagados, puerta cerrada y desconexión. |
| `--accent-blue` | `#3b82f6` | Detalles informativos (ej. reloj, broker). |

### Fondos Radiales Dinámicos (`body`)
El cuerpo del documento contiene tres gradientes radiales muy tenues que le otorgan profundidad e iluminación indirecta al fondo oscuro del dashboard:
- Destello ámbar en el lateral izquierdo (`rgba(245, 158, 11, 0.03)`).
- Destello azul en la esquina superior derecha (`rgba(59, 130, 246, 0.03)`).
- Destello verde en la parte inferior central (`rgba(16, 185, 129, 0.02)`).

---

## Tipografía
Para dar un aspecto limpio pero técnico se combinan dos familias tipográficas de Google Fonts:

1. **`Plus Jakarta Sans`** (Sans-serif):
   - Utilizada en títulos, botones, y textos generales.
   - Aporta modernidad gracias a sus formas geométricas limpias y gran legibilidad en pantallas digitales.
2. **`JetBrains Mono`** (Monospaced):
   - Utilizada en el reloj, identificadores (como Client ID), valores numéricos (distancia, presencia), estados del sistema y la consola de logs.
   - Da un aspecto técnico e industrial similar al de un sistema embebido o consola de comandos.

---

## Animaciones y Micro-interacciones (Feedback Visual)

### 1. Indicador de Conexión Pulsante (`pulse-dot`)
El punto de estado en el navbar realiza un escalado y cambio de opacidad continuo para denotar actividad:
- **Conectado (Verde)**: Pulso lento y resplandeciente (`box-shadow` verde).
- **Reconectando (Ámbar)**: Pulso rápido y parpadeante que transmite estado de espera/reintento.

### 2. Alertas de Emergencia/Transición (`pulse-warning`)
Cuando se detecta presencia en el ultrasónico (`SI`) o la puerta está en estado intermedio (`ABRIENDO` o `CERRANDO`), el texto realiza un parpadeo periódico de opacidad (de `1` a `0.5` a `1`) para capturar la atención del usuario de manera sutil pero clara.

### 3. Entrada de Logs (`log-fade`)
Las nuevas entradas de la consola no aparecen repentinamente; se deslizan verticalmente `4px` hacia arriba y pasan de opacidad `0` a `1` en `0.3s`, logrando que el flujo de eventos se sienta orgánico.

### 4. Tarjetas Elevadas (Efecto Hover)
Al posicionar el cursor sobre cualquier tarjeta:
- Se traslada verticalmente hacia arriba (`translateY(-3px)`).
- Su sombra se vuelve más difusa y oscura.
- El color del borde pasa de `--border-glow` a un sutil brillo ámbar con opacidad del 25% (`rgba(245, 158, 11, 0.25)`).

### 5. Botones con Efecto de Resplandor Central
Los botones de control de relé ("Encender" y "Apagar") tienen una capa de fondo radial en hover que simula una retroiluminación central tridimensional y se reducen un 4% de tamaño al hacer clic (`transform: scale(0.96)`) para simular la presión física de un botón.
