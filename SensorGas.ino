//==============================================||
// TRABALHO DE GERÊNCIA DE PROJETOS   E         ||
//   TÓPICOS ESPECIAIS EM HARDWARE              ||
// Acadêmicos: Mathias Giongo Bellé             ||
//             Sandro Miguel Weizenmann         ||
//             Ianael Bianchessi                ||
// Professores: Márcia Cristina Dazzi           ||
//              Thiago Felski Pereira           ||
//==============================================||

#include <CircusESP32Lib.h>
#include <DHT.h>

/** Estas são as declarações relacionadas ao CircusESP32Lib**/
char ssid[] = "Sua rede";                               // Coloque seu SSID wifi aqui
char password[] =  "sua senha";                         // Coloque sua senha wifi aqui
char token[] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiI3MDcifQ.RiLTLZUSSAV-bD4_aRbFZBTrUqQfl9Ow8c9pWE-nkV8"; // Coloque seu token da conta Circus Of Things.
char server[] = "www.circusofthings.com";
char temperature_key[] = "2075";                        // Coloque a chave do sinal que você criou no Circus Of Things para a temperatura
char humidity_key[] = "13451";                          // Coloque a chave do sinal que você criou no Circus Of Things para a umidade
char co_key[] = "22793";                                // Coloque a chave do sinal que você criou no Circus Of Things para o nível de CO.
CircusESP32Lib circusESP32(server,ssid,password);       // O objeto que representa um ESP32 para você poder solicitar gravação ou leitura


/** Estas são as declarações relacionadas ao Sensor DHT-11**/
#define DHTPIN 27                                       // entrada digital do ESP32 conectado ao DHT11
#define DHTTYPE DHT11                                   // declaração do modelo exato do sensor de temperatura DHT 11 para a biblioteca geral
DHT dht(DHTPIN, DHTTYPE);                               // O objeto que representa o sensor DHT11


/**Estas são as declarações relacionadas ao MQ-7**/
#define AOUTpin 33                                      // o pino AOUT do sensor Mq-7 entra no pino analógico 33 do ESP-32
#define DOUTpin 14                                      // o pino DOUT do sensor Mq-7 entra no pino digital 14 do ESP-32
#define buzzer 13                                       // o pino buzzer sai do ESP-32 e entra no buzzer
int limit = LOW;
int value;
 
void setup() {
    Serial.begin(115200);                               // Lembre-se de combinar esse valor com a taxa de transmissão no console
    pinMode(DOUTpin, INPUT);                            // Define o pino como uma entrada para o ESP-32
    pinMode(buzzer, OUTPUT);                            // Define o pino como saída do ESP-32
    dht.begin();                                        // Defina o DHT11 pronto
    circusESP32.begin();                                // Deixe o objeto Circus configurar-se para uma conexão SSL / Secure
}
 
void loop() {                                           // Agora que tudo está configurado, vamos começar com as tarefas
    delay(30000);
    // Deixe a biblioteca obter a temperatura que o sensor DHT11 está medindo.
    float t = dht.readTemperature(); 
    if (isnan(t))
        t=-1;                                           // Nesse caso, verifique a conexão do seu sensor DHT11 ... algo está desconectado ;-)
    float h = dht.readHumidity();
    if (isnan(h))
        h=-1;                                           // Nesse caso, verifique a conexão do seu sensor DHT11 ... algo está desconectado ;-)
    value = analogRead(AOUTpin);                        // Lê o valor analógico do pino AOUT do sensor de CO
    limit = digitalRead(DOUTpin);                       // Lê o valor digital do pino DOUT do sensor de CO
    // Mostrar valores, apenas para depuração
    Serial.println(""); Serial.print("Temperatura: "); Serial.println(t); Serial.print("Umidade: "); Serial.println(h);
    Serial.print("Nivel CO: "); Serial.println(value);  
    Serial.print("Limite: ");    Serial.println(limit); // Imprime o limite atingido como BAIXO ou ALTO (acima ou abaixo)
    //Verifica condição para acionamento do Buzzer
    if (limit == LOW or h < 30 or t > 50){
        digitalWrite(buzzer, HIGH);                     // Se o limite foi atingido, o buzzer aciona indicando o perigo
        }
        else{
        digitalWrite(buzzer, LOW);                      // Se o limite não for atingido, o buzzer permanecerá sem acionar.
    }
      // Relate os valores coletados pelo sensor ao Circus
    circusESP32.write(temperature_key,t,token);         // Informe a temperatura medida ao Circus
    circusESP32.write(humidity_key,h,token);            // Informe a umidade medida ao Circus
    circusESP32.write(co_key,value,token);              // Informe o nível de CO medido ao Circus
}
