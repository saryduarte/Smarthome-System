#include <WiFi.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

// =======================
// WIFI
// =======================
const char* ssid = "ESTUDIANTES";
const char* password = "Americana#45";

// =======================
// MQTT
// =======================
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;

const char* client_id = "esp32_carita_rele_ultra_servo_001";

const char* topic_rele = "carita/esp32/rele";
const char* topic_estado = "carita/esp32/estado";
const char* topic_distancia = "carita/esp32/distancia";
const char* topic_presencia = "carita/esp32/presencia";

// Canal dedicado para el servo / puerta
const char* topic_puerta = "carita/esp32/puerta";

// =======================
// PINES
// =======================
#define PIN_RELE 27
#define LED_ESP32 2

#define PIN_TRIG 25
#define PIN_ECHO 33

#define PIN_SERVO 13

WiFiClient espClient;
PubSubClient client(espClient);

Servo servoPuerta;

unsigned long ultimoMensaje = 0;
unsigned long ultimoReporteUltrasonico = 0;

// =======================
// CONFIGURACION PRESENCIA
// =======================
int distanciaPresencia = 20;
bool presenciaDetectada = false;

// =======================
// CONFIGURACION SERVO
// =======================
int ANGULO_CERRADO = 0;
int ANGULO_ABIERTO = 90;

bool puertaAbierta = false;

unsigned long tiempoUltimaDeteccion = 0;
unsigned long tiempoPuertaAbierta = 5000;

// =======================
// PUBLICAR ESTADO PUERTA
// =======================
void publicarPuerta(const char* estadoPuerta) {
  Serial.print("[PUERTA/MQTT] Publicando en ");
  Serial.print(topic_puerta);
  Serial.print(": ");
  Serial.println(estadoPuerta);

  if (client.connected()) {
    client.publish(topic_puerta, estadoPuerta);
  } else {
    Serial.println("[PUERTA/MQTT] No publicado, MQTT no conectado");
  }
}

// =======================
// RELE ACTIVO POR LOW
// =======================
void activarRele() {
  digitalWrite(PIN_RELE, LOW);
  digitalWrite(LED_ESP32, HIGH);

  Serial.println("[ACCION] Rele ACTIVADO");
  client.publish(topic_estado, "Rele activado");
}

void desactivarRele() {
  digitalWrite(PIN_RELE, HIGH);
  digitalWrite(LED_ESP32, LOW);

  Serial.println("[ACCION] Rele DESACTIVADO");
  client.publish(topic_estado, "Rele desactivado");
}

// =======================
// SERVO / PUERTA
// =======================
void abrirPuerta() {
  if (!puertaAbierta) {
    Serial.println();
    Serial.println("===== SERVO DEBUG =====");
    Serial.println("[SERVO] Orden recibida: ABRIR");
    Serial.print("[SERVO] Pin usado: GPIO");
    Serial.println(PIN_SERVO);
    Serial.print("[SERVO] Angulo destino: ");
    Serial.println(ANGULO_ABIERTO);

    publicarPuerta("ABRIENDO");

    servoPuerta.write(ANGULO_ABIERTO);
    delay(600);

    puertaAbierta = true;

    Serial.println("[SERVO] Movimiento enviado al servo");
    Serial.println("[PUERTA] Estado actualizado: ABIERTA");
    Serial.println("=======================");

    publicarPuerta("ABIERTA");
    client.publish(topic_estado, "Puerta abierta");
  } else {
    Serial.println("[SERVO] La puerta ya estaba ABIERTA, no se mueve");
    publicarPuerta("ABIERTA");
  }

  tiempoUltimaDeteccion = millis();
}

void cerrarPuerta() {
  if (puertaAbierta) {
    Serial.println();
    Serial.println("===== SERVO DEBUG =====");
    Serial.println("[SERVO] Orden recibida: CERRAR");
    Serial.print("[SERVO] Pin usado: GPIO");
    Serial.println(PIN_SERVO);
    Serial.print("[SERVO] Angulo destino: ");
    Serial.println(ANGULO_CERRADO);

    publicarPuerta("CERRANDO");

    servoPuerta.write(ANGULO_CERRADO);
    delay(600);

    puertaAbierta = false;

    Serial.println("[SERVO] Movimiento enviado al servo");
    Serial.println("[PUERTA] Estado actualizado: CERRADA");
    Serial.println("=======================");

    publicarPuerta("CERRADA");
    client.publish(topic_estado, "Puerta cerrada");
  } else {
    Serial.println("[SERVO] La puerta ya estaba CERRADA, no se mueve");
    publicarPuerta("CERRADA");
  }
}

// =======================
// MEDIR DISTANCIA
// =======================
long medirDistanciaCM() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duracion = pulseIn(PIN_ECHO, HIGH, 30000);

  if (duracion == 0) {
    return -1;
  }

  long distancia = duracion * 0.034 / 2;
  return distancia;
}

// =======================
// PROCESAR ULTRASONICO
// =======================
void procesarUltrasonico() {
  if (millis() - ultimoReporteUltrasonico >= 1000) {
    ultimoReporteUltrasonico = millis();

    long distancia = medirDistanciaCM();

    Serial.println();
    Serial.println("----- ULTRASONICO -----");

    if (distancia == -1) {
      Serial.println("[ULTRA] Sin lectura");
      client.publish(topic_distancia, "sin_lectura");
      client.publish(topic_presencia, "NO");
      Serial.println("-----------------------");
      return;
    }

    Serial.print("[ULTRA] Distancia: ");
    Serial.print(distancia);
    Serial.println(" cm");

    char bufferDistancia[16];
    sprintf(bufferDistancia, "%ld", distancia);
    client.publish(topic_distancia, bufferDistancia);

    if (distancia <= distanciaPresencia) {
      presenciaDetectada = true;
      tiempoUltimaDeteccion = millis();

      Serial.println("[ULTRA] Presencia detectada");
      client.publish(topic_presencia, "SI");

      abrirPuerta();
    } else {
      presenciaDetectada = false;

      Serial.println("[ULTRA] Sin presencia");
      client.publish(topic_presencia, "NO");
    }

    Serial.println("-----------------------");
  }

  if (puertaAbierta && !presenciaDetectada) {
    if (millis() - tiempoUltimaDeteccion >= tiempoPuertaAbierta) {
      cerrarPuerta();
    }
  }
}

// =======================
// WIFI
// =======================
void conectarWiFi() {
  Serial.println();
  Serial.println("================================");
  Serial.println("[WIFI] Conectando...");
  Serial.print("[WIFI] SSID: ");
  Serial.println(ssid);
  Serial.println("================================");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int intentos = 0;

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    intentos++;

    Serial.print("[WIFI] Intento ");
    Serial.print(intentos);
    Serial.print(" estado=");
    Serial.println(WiFi.status());

    digitalWrite(LED_ESP32, !digitalRead(LED_ESP32));

    if (intentos >= 40) {
      Serial.println("[WIFI] No conecto. Reiniciando...");
      delay(2000);
      ESP.restart();
    }
  }

  digitalWrite(LED_ESP32, LOW);

  Serial.println("[WIFI] Conectado");
  Serial.print("[WIFI] IP: ");
  Serial.println(WiFi.localIP());
}

// =======================
// MQTT CALLBACK
// =======================
void callback(char* topic, byte* payload, unsigned int length) {
  String mensaje = "";

  for (int i = 0; i < length; i++) {
    mensaje += (char)payload[i];
  }

  mensaje.trim();
  mensaje.toUpperCase();

  Serial.println();
  Serial.println("---------- MQTT RECIBIDO ----------");
  Serial.print("[MQTT] Topic: ");
  Serial.println(topic);
  Serial.print("[MQTT] Mensaje: ");
  Serial.println(mensaje);

  if (String(topic) == topic_rele) {
    if (mensaje == "ON") {
      activarRele();
    } else if (mensaje == "OFF") {
      desactivarRele();
    } else {
      Serial.println("[ERROR] Comando no reconocido. Usa ON u OFF.");
      client.publish(topic_estado, "Comando no reconocido");
    }
  }

  Serial.println("-----------------------------------");
}

// =======================
// MQTT RECONNECT
// =======================
void reconectarMQTT() {
  while (!client.connected()) {
    Serial.println();
    Serial.println("[MQTT] Conectando...");
    Serial.print("[MQTT] Broker: ");
    Serial.println(mqtt_server);
    Serial.print("[MQTT] Puerto: ");
    Serial.println(mqtt_port);
    Serial.print("[MQTT] Client ID: ");
    Serial.println(client_id);

    if (client.connect(client_id)) {
      Serial.println("[MQTT] Conectado");

      bool ok = client.subscribe(topic_rele);

      Serial.print("[MQTT] Suscrito a ");
      Serial.print(topic_rele);
      Serial.print(": ");
      Serial.println(ok ? "OK" : "ERROR");

      client.publish(topic_estado, "ESP32 rele + ultrasonico + servo conectado");
      publicarPuerta(puertaAbierta ? "ABIERTA" : "CERRADA");
    } else {
      Serial.print("[MQTT] Fallo rc=");
      Serial.println(client.state());
      Serial.println("[MQTT] Reintentando en 3 segundos...");
      delay(3000);
    }
  }
}

// =======================
// SETUP
// =======================
void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(PIN_RELE, OUTPUT);
  pinMode(LED_ESP32, OUTPUT);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  digitalWrite(PIN_RELE, HIGH);
  digitalWrite(LED_ESP32, LOW);
  digitalWrite(PIN_TRIG, LOW);

  servoPuerta.attach(PIN_SERVO);
  servoPuerta.write(ANGULO_CERRADO);
  puertaAbierta = false;

  Serial.println();
  Serial.println("################################");
  Serial.println("# ESP32 MQTT + RELE + ULTRASONICO + SERVO");
  Serial.println("################################");
  Serial.println("[BOOT] Rele apagado al iniciar");
  Serial.println("[BOOT] Ultrasonico listo");

  Serial.println("[BOOT] Inicializando servo...");
  Serial.print("[BOOT] Servo conectado a GPIO");
  Serial.println(PIN_SERVO);
  Serial.print("[BOOT] Angulo cerrado inicial: ");
  Serial.println(ANGULO_CERRADO);
  Serial.print("[BOOT] Angulo abierto: ");
  Serial.println(ANGULO_ABIERTO);
  Serial.println("[BOOT] Servo cerrado al iniciar");

  Serial.print("[BOOT] Distancia de presencia: ");
  Serial.print(distanciaPresencia);
  Serial.println(" cm");

  conectarWiFi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// =======================
// LOOP
// =======================
void loop() {
  if (!client.connected()) {
    reconectarMQTT();
  }

  client.loop();

  procesarUltrasonico();

  if (millis() - ultimoMensaje >= 5000) {
    ultimoMensaje = millis();

    Serial.println();
    Serial.println("[LOOP] ESP32 vivo");
    Serial.print("[LOOP] Esperando comandos rele en: ");
    Serial.println(topic_rele);
    Serial.print("[LOOP] MQTT conectado: ");
    Serial.println(client.connected() ? "SI" : "NO");
    Serial.print("[LOOP] Estado pin rele: ");
    Serial.println(digitalRead(PIN_RELE) == LOW ? "LOW / ACTIVADO" : "HIGH / APAGADO");
    Serial.print("[LOOP] Presencia: ");
    Serial.println(presenciaDetectada ? "SI" : "NO");
    Serial.print("[LOOP] Puerta/servo: ");
    Serial.println(puertaAbierta ? "ABIERTA" : "CERRADA");
  }
}