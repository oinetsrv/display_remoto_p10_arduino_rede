 /*
Projeto: Controle de DISPLAY 
CONTROLADOR 1
|- RECEBE DADOS VIA REDE
|- ENVIA VIA SERIAL1
CONTROLADOR 2
|- RECEBE VIA SERIAL1
|- IMPRIME DISPLAY COM 4 MODULOS

AUTOR: STENIO RODRIGUES
DATA: 18/12/2020
OBJETIVO:
IMPRIMIR NO DISPLAY USANDO DMD2
     Serial.println("V-37-DMD2-RETORNO-REDE-151-ENVIA--SERIAL1:");
// CONTROLADOR UNICO PARA OS 4 DISPLAYS CONECTOR
// CONECTOR DISPLAY
________________________
|1 pin_noe 2  pin_a     |
|3  GND    4  pin_b     |
|5  GND    6  N/C       |
|7  GND    8  pin_sck  S|
|9  GND    10 pin_clk  L|
|11 GND    12 pin_r_datR|
________________________

// PAINEL           MEGA
//byte panelsWide   N/C
//byte panelsHigh   N/C
//byte pin_noe      9
//byte GND          GND 
//byte pin_a        6
//byte pin_b        7
//byte pin_sck      52
//byte pin_clk      8
//byte pin_r_data   51
///////////////////////
CONECTOR REDE
________________________
|1 CLK    2  INT /NT   |
|3 WOL    4  SO        |
|5 SI     6  SCK       |
|7 CS     8  RST       |
|9 VCC    10 GND       |
________________________

//ADAPTADOR REDE    MEGA
1 CLK               N/C
3 WOL               N/C
5 SI                51
7 CS                53
9 VCC               3.3
2  INT /NT          2
4  SO               50     
6  SCK              52
8  RST              N/C
10 GND              GND

PARA MONITORAR MENSAGEM RECEBIDA ACESSE
http://192.168.0.x:2000/
       // escolha QUAL DISPLAY - QUAL FONTE 
       // EXEMPLO: http://192.168.0.151:2000/S1234*
       //-->> DISPLAY SAIDA FONTE Arial14
       //http://192.168.0.151:2000/9*
       //-->> DISPLAY POTENCIA MAXIMA
       //http://192.168.0.151:2000/8*
       //-->> DISPLAY POTENCIA MINIMO
       // PRIMEIRO DIGITO ESCOLHE QUAL DISPLAY
       // S -> SAIDA
       // E -> ENTRADA
       // 9 -> BRILHO MAXIMO
       // 8 -> BRILHO MINIMO
       // SEGUNDO DIGITO ESCOLHE QUAL TIPO DE FONTE
       // 0 -> SystemFont5x7
       // 1 -> Droid_Sans_12
       // 2 -> Arial14
       // 3 -> Arial_Black_16
 

*/
 // -- RELACIONADO COM REDE -- //
 #include <SPI.h>
 #include <UIPEthernet.h>
 byte mac[] = {
     0xBA,
     0xBB,
     0xAB,
     0xBA,
     0xEC,
     0xEC
 };
 byte ip[] = {
     192,
     168,
     0,
     149
 };
 byte gateway[] = {
     192,
     168,
     0,
     1
 };
 byte subnet[] = {
     255,
     255,
     255,
     0
 };
 EthernetServer server(2000);
 // -- RELACIONADO COM REDE -- //

 // -- RELACIONADO COM MENSAGEM RECEBIDA -- //
 int tamanho_string = 0;
 int cont = 0;
 String stringRecebida = "";
 String stringDisplayEnt = "";
 String stringDisplaySai = "";

 // -- RELACIONADO COM MENSAGEM RECEBIDA -- //
 void setup() {
     Ethernet.begin(mac, ip, gateway, subnet); //PASSA OS PARÂMETROS PARA A FUNÇÃO QUE VAI FAZER A CONEXÃO COM A REDE
     server.begin(); //INICIA O SERVIDOR PARA RECEBER DADOS NA PORTA 80

     Serial.begin(9600);
     Serial1.begin(9600);

     Serial.println("V-37-DMD2-RETORNO-REDE-149-ENVIA--SERIAL1:");
     Serial.println("IP:");
     Serial.println(Ethernet.localIP());
 }
 // -- RELACIONADO COM MENSAGEM RECEBIDA -- //
 
 // -- RELACIONADO COM MENSAGEM RECEBIDA -- //
 void loop() 
 {
     // Verificação se o cliente está conectado
     EthernetClient client = server.available();
     if (!client) 
       { // Verifica se o cliente está conectado ao servidor, executa este ciclo até estar conectado
           return;
       }
     // Espera até o cliente enviar dados
     char charEnviada[12];
     Serial.println("novo cliente"); //Apresenta esta mensagem quando o cliente se liga ao servidor
     while (!client.available()) 
       {
        delay(1);
       }
     // Ler a primeira linha do pedido
     String request = client.readStringUntil('\r'); // espera até final da linha
     Serial.println(request);
     client.flush(); // Espera até que todos os caracteres de saída no buffer tenham sido enviados.
     // Tratamento da mensagem
     tamanho_string = request.length();
     int DigInicial = 1 + request.indexOf('0/'); // PEGA O MARCADOR DE INICIO
     Serial.println("DigInicial");
     Serial.println(DigInicial);
     int DigFinal = 1 + request.indexOf('*'); // PEGA O MARCADOR DE FINAL
     Serial.println("DigFinal");
     Serial.println(DigFinal);
     Serial.println("substring");
     stringRecebida = request.substring(DigInicial, DigFinal);
     Serial.println(stringRecebida);
     // Envia via serial1 a mensagem bruta para tratamento no display
      Serial.println("FOR Imprimi CHAR");
      cont = 0;
        for (int i = DigInicial; i <= DigFinal; i++) 
            { 
              charEnviada[cont] = request[i];
              Serial1.print(charEnviada[cont]);
              delay(1);
              cont = cont + 1;
            }
     // Condição de impressão
     if (request[DigInicial] == 'E') 
       {
           stringDisplayEnt = request.substring(DigInicial + 1, DigFinal -1);
       }
     if (request[DigInicial] == 'S') 
       {
           stringDisplaySai = request.substring(DigInicial + 1, DigFinal -1);
       }
     // Resposta via HTTP/1.1 pé de boi
     client.println("HTTP/1.1 200 OK");
     client.println("Content-Type: text/html");
     client.println(""); //  quebra linha
     client.println("<!DOCTYPE HTML>");
     client.println("<html>");
     client.print("<font color=#FF0000><b><u>"); // seta fonte e cor

     client.print("ULTIMO DADO ENVIADO PARA IP:   ");
     client.println(Ethernet.localIP());
     client.println("<br />");
     client.println("<br />");

     client.print("USUARIO IP:   ");
     IPAddress remoteip = client.remoteIP();
     client.print(remoteip);
     client.print("</u></b></font>");
     client.println("<br />");
     client.println("<br />");

     client.print("COMANDO BRUTO: ");
     client.print("<b>");
     client.print(stringRecebida);
     client.print("</b>");
     client.println("<br />");

     client.print("ULTIMO COMANDO ENTRADA: ");
     client.print("<b>");
     client.print("STRING: ");
     client.print(stringDisplayEnt); // EXEMPLO
     client.print("</b>");
     client.println("<br />");

     client.print("ULTIMO COMANDO SAIDA: ");
     client.print("<b>");
     client.print("STRING: ");
     client.print(stringDisplaySai); // EXEMPLO
     client.print("</b>");
     client.println("<br />");

     client.println("</b></html>");
     delay(1);
     Serial.println("Cliente desconectado"); // Depois do cliente efectuar o pedido apresenta esta mensagem no monitor série
     Serial.println("");
 }