#define BLYNK_TEMPLATE_ID "TMPL2rzZSmOt-"
#define BLYNK_TEMPLATE_NAME "Alerta"
#define BLYNK_AUTH_TOKEN "NMCEDMQre9yznO07wF1EcMDXOmIyCbdA"

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <BlynkSimpleEsp8266.h>
#include <LittleFS.h>

#define BOTAO 14  // Pino ao qual o botão está conectado (GPIO14, D5)
#define LED 2     // Pino ao qual o LED está conectado (GPIO2, D4)

// Credenciais da rede Wi-Fi
const char* ssid = "Proa_LifeBand1121";
const char* password = "Proa_LifeBand1121";

// const char* ssid = "GABRIELPC";
// const char* password = "Gabriel123";

// Cria um objeto AsyncWebServer na porta 80
AsyncWebServer server(80);
AsyncEventSource events("/events");

// Página HTML para o servidor web
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Life Band</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta http-equiv="X-UA-Compatible" content="IE=7">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <meta http-equiv="Content-Type" content="text/html;charset=UTF-8">
  <link rel="shortcut icon" href="/favicon.png" type="image/x-icon">  

  <style>
    html, body {
      height: 100%;
      margin: 0;
    }

    body {
      background-image: url(/TelaInicial.jpg);
      width: 100%;
      height: 100%;
      background-size: cover;
      background-position: center;
      background-repeat: no-repeat;
      position: relative;
      overflow: hidden;
    }

    #Botao {
    width: 30px;
    height: 30px;
    align-items: center;
    justify-content: center;
    position: absolute;
    font-size: 1.5rem;
    top: 21%;
    right: 5%;
    border-radius: 10px;
    background-color: transparent;
    border: none;
    cursor: pointer;
    color: white;
    display: none;
    }

    #Botao2 {
    width: 100px;
    height: 30px;
    align-items: center;
    justify-content: center;
    position: absolute;
    font-size: 1.5rem;
    top: 3.5%;
    right: 36%;
    border-radius: 10px;
    background-color: transparent;
    border: none;
    cursor: pointer;
    color: white;
    }

  </style>

</head>

<body>
  
    <button id="Botao">X</button>
    <button id="Botao2"></button>

  <script>
    if (!!window.EventSource) {
      var source = new EventSource('/events');
      source.addEventListener('alert', function(e) {

            var body = document.body;
            var botao = document.querySelector('#Botao');

            if (body.style.backgroundImage.includes('TelaInicial.jpg')) {
                body.style.backgroundImage = 'url("TelaPaciente.jpg")';
                body.style.transition = "0.1s linear";
                botao.style.display = "block";
            } 
            else {
                body.style.backgroundImage = 'url("TelaInicial.jpg")';
                botao.style.display = "none";
            }
          
            //Reaplicar as propriedades CSS

            // html.style.margin = "0";
            body.style.margin = "0";
            body.style.width = "100%";
            body.style.height = "100%";
            body.style.backgroundSize = "cover";
            body.style.backgroundPosition = "center";
            body.style.backgroundRepeat = "no-repeat";
            body.style.position = "relative";
            body.style.overflow = "hidden";
      
      }, false);
    }

      document.querySelector('#Botao').onclick = function() {
        var body = document.body;
        var botao = document.querySelector('#Botao');

        body.style.backgroundImage = 'url("TelaInicial.jpg")';
        botao.style.display = "none";

        // Reaplicar as propriedades CSS
        body.style.margin = "0";
        body.style.width = "100%";
        body.style.height = "100%";
        body.style.backgroundSize = "cover";
        body.style.backgroundPosition = "center";
        body.style.backgroundRepeat = "no-repeat";
        body.style.position = "relative";
        body.style.overflow = "hidden";
        body.style.transition = "0.1s linear";
      }

        document.querySelector('#Botao2').onclick = function() {
        var body = document.body;
        var botao = document.querySelector('#Botao'); // Seleciona o botão .Botao

        if (body.style.backgroundImage.includes('TelaInicial.jpg')) {
            body.style.backgroundImage = 'url("TelaPaciente.jpg")';
            body.style.transition = "0.1s linear";
            botao.style.display = "block";
        }
        else {
            body.style.backgroundImage = 'url("TelaInicial.jpg")';
            botao.style.display = "none";
        }
        
        html.style.margin = "0";
        body.style.margin = "0";
        body.style.width = "100%";
        body.style.height = "100%";
        body.style.backgroundSize = "cover";
        body.style.backgroundPosition = "center";
        body.style.backgroundRepeat = "no-repeat";
        body.style.position = "relative";
        body.style.overflow = "hidden";
        body.style.transition = "0.1s linear";
    }

  </script>
</body>
</html>
)rawliteral";

void setup() {
  // Configura os pinos
  pinMode(BOTAO, INPUT_PULLUP);  // Configura o pino do botão como entrada com pull-up interno
  pinMode(LED, OUTPUT);          // Configura o pino do LED como saída
  Serial.begin(115200);          // Inicializa a comunicação serial para depuração

  if (!LittleFS.begin()) {
    Serial.println("Erro ao montar o sistema de arquivos");
    return;
  }

  // Conecta-se ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
  Serial.println(WiFi.localIP());

  // Iniciar o Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // Configura a rota para a página principal
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Configura a rota para servir as imagens
  server.serveStatic("/TelaInicial.jpg", LittleFS, "/TelaInicial.jpg");
  server.serveStatic("/TelaPaciente.jpg", LittleFS, "/TelaPaciente.jpg");
  // server.serveStatic("/LifeClick.jpg", LittleFS, "/LifeClick.jpg");
  server.serveStatic("/favicon.png", LittleFS, "/favicon.png");

  // Adiciona a fonte de eventos
  server.addHandler(&events);

  // Inicia o servidor
  server.begin();
}

void loop() {

  Blynk.run();  // Mantém o Blynk rodando

  int botaoEstado = digitalRead(BOTAO);  // Lê o estado do botão

  if (botaoEstado == LOW) {  // Se o botão for pressionado
    digitalWrite(LED, LOW);  // Liga o LED
    events.send("A Life Click está chegando!!", "alert", millis());

    String mensagem = "Usuário da pulseira enviou um alerta - Clique para verificar";

    Blynk.logEvent("_", mensagem); // Envia a notificação push

    delay(1000);  // Debounce simples
  } else {  // Caso contrário
    digitalWrite(LED, HIGH); // Desliga o LED
  }
}
