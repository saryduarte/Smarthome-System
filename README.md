# Casa Inteligente Basada en ESP32

## Descripción del Proyecto

El proyecto que llevamos a cabo es una maqueta de una casa inteligente basada en ESP32. El objetivo principal consiste en que algunas de las funciones de la casa se ejecuten de manera automática mediante el uso de sensores y dispositivos de control.

La primera función es el control de las luces. Para ello no se utiliza ningún sensor, sino que el encendido y apagado se realiza de forma remota desde una página web conectada al ESP32 mediante WiFi. Desde esa página, el usuario puede activar el módulo relé que enciende o apaga las luces decorativas de la maqueta.

La segunda de las funciones es una puerta de garaje automática. En este caso se utiliza un sensor ultrasónico HC-SR04 que mide la distancia de los objetos que se aproximan. Cuando detecta un objeto a una distancia determinada, el ESP32 se encarga de activar un servomotor para abrir la puerta del garaje.

Con este proyecto se persigue el objetivo de aplicar aspectos trabajados en clase como la programación de microcontroladores, el uso de sensores y el control de actuadores.

## Problema Identificado

Hoy en día existen muchas tareas dentro de una vivienda que continúan haciéndose de forma manual. Aunque se consideren acciones sencillas, en algunos casos esto puede resultar una molestia y puede llegar a suponer un tiempo perdido.

Por ejemplo, la persona que quiere encender una luz sin tener que levantarse o estar físicamente cerca del interruptor, o la persona que llega a casa y necesita abrir una puerta de garaje. Por esta razón, ya que se producen situaciones cotidianas donde se requiere comodidad y control a distancia, surge la necesidad de automatizar estos procesos.

## Objetivo General

Diseñar y construir una maqueta de casa inteligente basada en ESP32 que automatice el control de la iluminación interior mediante una página web conectada por WiFi y un módulo relé, así como la apertura de la puerta del garaje mediante sensor ultrasónico HC-SR04 y servomotor, demostrando la aplicación de sistemas embebidos en soluciones domóticas de bajo costo.

## Objetivos Específicos

- Programar el ESP32 para alojar una página web que permita controlar de forma remota, mediante WiFi, el encendido y apagado de las luces decorativas de la maqueta a través de un módulo relé.
- Integrar el sensor ultrasónico HC-SR04 para medir la distancia y activar el servomotor que simula la apertura y cierre automático de la puerta del garaje.
- Programar el ESP32 para gestionar ambos sistemas de forma simultánea e independiente utilizando el lenguaje de programacion en el entorno de desarrollo correspondiente.
- Ensamblar todos los componentes (protoboard, HC-SR04, servomotor, relé, resistencias, luces decorativas, ESP32 y cables jumper) en la estructura física del proyecto.
- Validar el correcto funcionamiento del prototipo mediante pruebas de control remoto vía WiFi y de respuesta del sensor de proximidad en distintas condiciones.

## Mejoras Futuras

- Control por aplicación móvil: aprovechar las capacidades WiFi y Bluetooth del ESP32 para controlar las luces y el garaje desde un smartphone.
- Iluminación por zonas con sensores PIR: separar la iluminación por habitaciones e instalar sensores de movimiento en cada zona para mayor eficiencia energética.
- Panel de estado con pantalla LCD: añadir una pantalla LCD u OLED que muestre en tiempo real el estado del sistema (luces encendidas/apagadas, puerta abierta/cerrada, distancia detectada).
- Sistema de alertas con buzzer: implementar una alarma sonora cuando la puerta del garaje permanezca abierta por demasiado tiempo o se detecte un evento inusual.
- Cámara de seguridad: incorporar una pequeña cámara en el garaje que capture imágenes al detectar proximidad y las envíe al celular del usuario.
- Alimentación con energía solar: alimentar el sistema con un panel solar y batería recargable para hacerlo más eficiente y sostenible.
