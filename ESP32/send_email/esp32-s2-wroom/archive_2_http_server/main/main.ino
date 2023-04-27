#include <WiFi.h>
#include <SPIFFS.h>

const char* ssid = "nome";
const char* password = "senha";

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("Conectando-se à rede WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Tentando se conectar à rede WiFi...");
  }

  Serial.println("Conectado à rede WiFi!");

  if (!SPIFFS.begin()) {
    Serial.println("Erro ao montar o sistema de arquivos SPIFFS");
    return;
  }
}

void loop() {
  File htmlFile = SPIFFS.open("/arquivo_html.html", "r");
  if (!htmlFile) {
    Serial.println("Erro ao abrir o arquivo HTML");
    return;
  }

  String html = htmlFile.readString();

  Serial.println("Conectando ao servidor...");

  WiFiClient client;
  if (!client.connect("endereco_do_servidor", 80)) {
    Serial.println("Falha ao se conectar ao servidor");
    htmlFile.close();
    return;
  }

  Serial.println("Conectado ao servidor");

  client.print("POST / HTTP/1.1\r\n");
  client.print("Host: endereco_do_servidor\r\n");
  client.print("Content-Type: text/html\r\n");
  client.print("Content-Length: ");
  client.print(html.length());
  client.print("\r\n\r\n");
  client.print(html);

  Serial.println("HTML enviado com sucesso!");

  htmlFile.close();

  delay(60000); // espera um minuto antes de publicar novamente
}
