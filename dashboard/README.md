# Dashboard MQTT ESP32 - Smart Home System

Este es el README Técnico de la interfaz web para monitorear y controlar el sistema de casa inteligente basado en ESP32. Permite controlar actuadores (como relés y servomotores) y monitorear sensores en tiempo real mediante el protocolo MQTT sobre WebSockets.

---

## Arquitectura y Tecnologías
La interfaz está construida como una aplicación de una sola página (SPA) cliente-servidor directa, sin intermediarios ni servidores web de aplicación (el navegador web se conecta directamente al broker MQTT).

- **HTML5 & Bootstrap v5.3.3**: Estructuración semántica y diseño responsivo (`row`, `col-lg-4`, `g-3`).
- **CSS3 (Vanilla)**: Hoja de estilos personalizada ([style.css](file:///home/eddy/Smarthome-System/dashboard/style.css)) que implementa variables de diseño (`:root`), efectos de brillo, fondos translúcidos (glassmorphism) y animaciones clave.
- **JavaScript (ES6+)**: Lógica de interacción y manipulación del DOM ([script.js](file:///home/eddy/Smarthome-System/dashboard/script.js)).
- **MQTT.js (v4.x.x)**: Cargado mediante CDN (`unpkg.com`) para habilitar la comunicación MQTT sobre WebSockets directamente en el navegador.

---

## Protocolo y Flujo de Comunicación (MQTT)

### Conexión
- **Broker**: `ws://broker.hivemq.com:8000/mqtt` (Broker público HiveMQ vía WebSockets).
- **Client ID**: Generado dinámicamente en tiempo de ejecución con el prefijo `dashboard_carita_` seguido de una cadena aleatoria de 8 caracteres hexadecimales para evitar colisiones.

### Tópicos (Topics) y Cargas Útiles (Payloads)

| Tópico | Dirección (Dashboard) | Formato / Valores | Descripción |
| :--- | :--- | :--- | :--- |
| `carita/esp32/estado` | Suscripción (IN) | Texto (`ON` / `OFF` / estados) | Mensajes de estado general y depuración enviados por el ESP32. |
| `carita/esp32/distancia` | Suscripción (IN) | `[número]` en cm / `sin_lectura` | Distancia reportada por el sensor ultrasónico HC-SR04. |
| `carita/esp32/presencia` | Suscripción (IN) | `SI` / `NO` | Bandera de presencia detectada a menos de 20 cm del sensor. |
| `carita/esp32/puerta` | Suscripción (IN) | `ABIERTA` / `CERRADA` / `ABRIENDO` / `CERRANDO` | Estado físico del portón controlado por servomotor. |
| `carita/esp32/rele` | Publicación (OUT) | `ON` / `OFF` | Comandos enviados para encender/apagar de manera remota las luces (relé). |

---

## Archivos del Sitio

- **[index.html](file:///home/eddy/Smarthome-System/dashboard/index.html)**: Contenedor HTML principal. Organiza la cuadrícula de tarjetas de visualización del sistema (Conexión, Control de Relé, Estado general, Sensor ultrasónico, Portón/Servo, y Terminal de eventos).
- **[script.js](file:///home/eddy/Smarthome-System/dashboard/script.js)**: Inicializa la conexión, suscribe a los tópicos en el evento `connect`, procesa los mensajes entrantes para actualizar dinámicamente las clases del DOM, genera las marcas de tiempo y maneja los clics de la interfaz.
- **[style.css](file:///home/eddy/Smarthome-System/dashboard/style.css)**: Controla el tema oscuro, las animaciones (`pulse-dot`, `pulse-warning`, `log-fade`), sombras, bordes brillantes y adaptabilidad a dispositivos móviles.

Para detalles sobre la dirección de arte, colores y micro-animaciones, consulte el archivo de diseño: **[DESIGN.md](file:///home/eddy/Smarthome-System/dashboard/DESIGN.md)**.
