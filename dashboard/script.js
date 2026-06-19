const $ = id => document.getElementById(id);

// ── Reloj en tiempo real ──
function actualizarReloj() {
    const ahora = new Date();
    $('reloj').textContent = ahora.toLocaleTimeString('es-ES', { hour12: false });
}
actualizarReloj();
setInterval(actualizarReloj, 1000);

// ── Conexión MQTT ──
const broker = 'ws://broker.hivemq.com:8000/mqtt';
const topicRele = 'carita/esp32/rele';
const topicEstado = 'carita/esp32/estado';
const topicDistancia = 'carita/esp32/distancia';
const topicPresencia = 'carita/esp32/presencia';
const topicPuerta = 'carita/esp32/puerta';
const clientId = 'dashboard_carita_' + Math.random().toString(16).substr(2, 8);
const options = { clientId, clean: true, connectTimeout: 4000, reconnectPeriod: 2000 };

const client = mqtt.connect(broker, options);

const connDot = $('connDot');
const connLabel = $('connLabel');
const mqttBadge = $('mqttBadge');
const clientIdSpan = $('clientIdSpan');
const log = $('log');
const estadoEsp32 = $('estadoEsp32');
const ultimoMensaje = $('ultimoMensaje');
const lastUpdate = $('lastUpdate');
const lastMsgTime = $('lastMsgTime');
const releIcon = $('releIcon');
const stateIcon = $('stateIcon');
const stateIconWrap = $('stateIconWrap');
const distanceDisplay = $('distanceDisplay');
const presenceBadge = $('presenceBadge');
const presenceValue = $('presenceValue');
const sensorUpdate = $('sensorUpdate');
const doorBadge = $('doorBadge');
const doorDisplay = $('doorDisplay');
const doorIcon = $('doorIcon');
const doorUpdate = $('doorUpdate');

clientIdSpan.textContent = clientId;

const STATUS_STYLES = {
    online:  { label: 'Conectado',   color: '#10b981', dot: 'online' },
    offline: { label: 'Desconectado', color: '#ef4446', dot: 'offline' },
    reconnecting: { label: 'Reconectando…', color: '#f59e0b', dot: 'reconnecting' },
    error:  { label: 'Error',        color: '#ef4446', dot: 'offline' },
};

function setConnStatus(key) {
    const s = STATUS_STYLES[key] || STATUS_STYLES.offline;
    connDot.className = 'conn-dot ' + s.dot;
    connLabel.textContent = s.label;
    connLabel.style.color = s.color;
    mqttBadge.textContent = s.label;
    mqttBadge.style.background = s.color + '26';
    mqttBadge.style.color = s.color;
}

function agregarLog(texto) {
    const hora = new Date().toLocaleTimeString('es-ES', { hour12: false });
    const entry = document.createElement('div');
    entry.className = 'log-entry';
    entry.innerHTML = `<span class="time">[${hora}]</span> ${texto}`;
    log.appendChild(entry);
    log.scrollTop = log.scrollHeight;
}

$('clearLog').addEventListener('click', function () {
    log.innerHTML = '';
    agregarLog('Registro limpiado');
});

// ── Eventos MQTT ──
client.on('connect', function () {
    setConnStatus('online');
    agregarLog('Conectado al broker MQTT');

    client.subscribe(topicEstado, function (err) {
        if (!err) {
            agregarLog('Suscrito a <span class="highlight">' + topicEstado + '</span>');
        } else {
            agregarLog('Error al suscribirse');
        }
    });

    client.subscribe(topicDistancia, function (err) {
        if (!err) {
            agregarLog('Suscrito a <span class="highlight">' + topicDistancia + '</span>');
        } else {
            agregarLog('Error al suscribirse');
        }
    });

    client.subscribe(topicPresencia, function (err) {
        if (!err) {
            agregarLog('Suscrito a <span class="highlight">' + topicPresencia + '</span>');
        } else {
            agregarLog('Error al suscribirse');
        }
    });

    client.subscribe(topicPuerta, function (err) {
        if (!err) {
            agregarLog('Suscrito a <span class="highlight">' + topicPuerta + '</span>');
        } else {
            agregarLog('Error al suscribirse');
        }
    });
});

client.on('reconnect', function () {
    setConnStatus('reconnecting');
    agregarLog('Reconectando…');
});

client.on('error', function (error) {
    setConnStatus('error');
    agregarLog('Error MQTT: ' + error);
});

client.on('offline', function () {
    setConnStatus('offline');
    agregarLog('Cliente offline');
});

client.on('message', function (topic, message) {
    const msg = message.toString();
    agregarLog('Mensaje recibido [<span class="highlight">' + topic + '</span>]: ' + msg);

    if (topic === topicEstado) {
        const valor = msg.toUpperCase();
        estadoEsp32.textContent = valor;
        ultimoMensaje.textContent = valor;
        const ahora = new Date().toLocaleTimeString('es-ES', { hour12: false });
        lastUpdate.textContent = 'Último comando: ' + ahora;
        lastMsgTime.textContent = ahora;

        estadoEsp32.className = 'state-display';
        if (valor === 'ON') {
            estadoEsp32.classList.add('on');
            releIcon.style.color = '#10b981';
            stateIcon.style.color = '#10b981';
            stateIconWrap.style.background = 'rgba(16,185,129,0.15)';
        } else if (valor === 'OFF') {
            estadoEsp32.classList.add('off');
            releIcon.style.color = '#ef4446';
            stateIcon.style.color = '#ef4446';
            stateIconWrap.style.background = 'rgba(239,68,70,0.12)';
        } else {
            estadoEsp32.classList.add('idle');
            releIcon.style.color = '#3d4f73';
            stateIcon.style.color = '#3d4f73';
            stateIconWrap.style.background = 'rgba(16,185,129,0.08)';
        }
    } else if (topic === topicDistancia) {
        if (msg === 'sin_lectura') {
            distanceDisplay.textContent = '—';
        } else {
            distanceDisplay.textContent = msg + ' cm';
        }
        const ahora = new Date().toLocaleTimeString('es-ES', { hour12: false });
        sensorUpdate.textContent = ahora;
    } else if (topic === topicPresencia) {
        const valor = msg.toUpperCase();
        presenceValue.textContent = valor === 'SI' ? 'Detectado' : 'Vacío';
        presenceBadge.textContent = valor === 'SI' ? 'DETECTADO' : 'VACÍO';
        presenceBadge.className = 'badge-presence ' + (valor === 'SI' ? 'detectado' : 'vacio');
        presenceValue.className = 'presence-value ' + (valor === 'SI' ? 'detectado' : 'vacio');
        const ahora = new Date().toLocaleTimeString('es-ES', { hour12: false });
        sensorUpdate.textContent = ahora;
    } else if (topic === topicPuerta) {
        const valor = msg.toUpperCase();
        doorDisplay.textContent = valor;
        doorBadge.textContent = valor;
        doorDisplay.className = 'door-display';
        doorBadge.className = 'badge-door';
        const icon = doorIcon.querySelector('i');
        if (valor === 'ABIERTA') {
            doorDisplay.classList.add('abierta');
            doorBadge.classList.add('abierta');
            icon.className = 'bi bi-door-open';
        } else if (valor === 'CERRADA') {
            doorDisplay.classList.add('cerrada');
            doorBadge.classList.add('cerrada');
            icon.className = 'bi bi-door-closed';
        } else if (valor === 'ABRIENDO' || valor === 'CERRANDO') {
            doorDisplay.classList.add('transicion');
            doorBadge.classList.add('transicion');
            icon.className = 'bi bi-arrow-left-right';
        }
        const ahora = new Date().toLocaleTimeString('es-ES', { hour12: false });
        doorUpdate.textContent = ahora;
    }
});

function encenderRele() {
    client.publish(topicRele, 'ON');
    agregarLog('Comando enviado: <span class="highlight">ON</span>');
}

function apagarRele() {
    client.publish(topicRele, 'OFF');
    agregarLog('Comando enviado: <span class="highlight">OFF</span>');
}

$('btnOn').addEventListener('click', encenderRele);
$('btnOff').addEventListener('click', apagarRele);

agregarLog('Dashboard iniciado — cliente: ' + clientId);
