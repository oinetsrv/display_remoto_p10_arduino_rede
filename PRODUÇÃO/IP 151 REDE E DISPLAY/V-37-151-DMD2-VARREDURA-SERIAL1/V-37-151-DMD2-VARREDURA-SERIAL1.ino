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
    Serial.println("V-37-151-DMD2-VARREDURA-SERIAL1:");
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
 //################# DISPLAY //
 #include <SPI.h>
 #include <DMD2.h>
 #include <fonts/Arial14.h>
 #include <fonts/Arial_Black_16.h>
 #include <fonts/Droid_Sans_12.h>
 #include <fonts/SystemFont5x7.h>
 #include <fonts/Droid_Sans_16.h>

#include <avr/io.h>  // wachdog
#include <avr/wdt.h> // wachdog

 const int COUNTDOWN_FROM = 12;
 int counter = COUNTDOWN_FROM;
 int Q_DISPLAY = 4;
 SoftDMD dmd(Q_DISPLAY, 1, 9, 6, 7, 8, 52, 51); // DMD controls the entire display
 DMD_TextBox box1(dmd, 0, 1); // BOX PRIMEIRA IMPRESSAO
 DMD_TextBox box(dmd, 64, 2); // BOX SEGUNDA IMPRESSAO
 //################# DISPLAY //
 //################# TRATAMENTO MENSAGEM //
 int tamanho_string = 0;
 int ultimos_digitos = 0;
 int cont = 0;
 int cont_MARCADOR = 0;
 char stringRecebida[24];
 String str = "";
 String display_saida_temp = "";
 //################# TRATAMENTO MENSAGEM //
 void setup() {
   wdt_disable (); // desativa o cachorro
   Serial.begin(9600);
   wdt_enable (WDTO_8S); // ativa o wachdog
   Serial1.begin(9600);
   dmd.setBrightness(255);
   dmd.selectFont(Arial14);
   dmd.begin();
    Serial.println("V-37-151-DMD2-VARREDURA-SERIAL1:");
 } // FIM SETUP
 void loop() {
  wdt_reset ();
   while (Serial1.available()) {
     //Serial.println("CONECTADO-SERIAL1:");
     char request = Serial1.read();
     str.concat(request);
     if (str.endsWith("*")) {
       tamanho_string = str.length();
       if (tamanho_string >= 24) { // TRATAMENTO DA MENSAGEM VIA RS232
         ultimos_digitos = tamanho_string - 24;
       } else {
         ultimos_digitos = 0;
       }
       // posicao inicial do vetor tratado para impressao
       //Serial.print("TRATAMENTO DA MENSAGEM VIA FOR:  ");
       for (int i = ultimos_digitos; i <= tamanho_string; i++) {
         stringRecebida[cont] = str[i];
       }
       // ESCOLHA DO BRILHO DO DISPLAY
       if (cont == 1) {
         str.remove(0, 1);
       }
       if (str[0] == '9') {
          wdt_disable (); // desativa o cachorro
          delay(10);
          wdt_enable (WDTO_1S);
          dmd.fillScreen(true);
        delay(500);
        dmd.clearScreen();
        delay(500);
  delay(3000);
         dmd.setBrightness(255);
       }
       if (str[0] == '8') {
         dmd.setBrightness(5);
       }
       // FIM ESCOLHA DO BRILHO DO DISPLAY
       // ESCOLHA DISPLAY ENTRADA
       imprimir_display_saida(str);
       // ESCOLHA DISPLAY SAIDA
       // toda vez que imprimir o display de entrada tem que reimprimir o display de saida
       if (str[0] == 'E') {
         display_saida_temp = str;
         imprimir_display_entrada(display_saida_temp);
          
       }
       //imprimir_display_saida(str);
       int limpar_string = str.indexOf('*') + 2;
       str.remove(0, limpar_string);
       Serial.println("");
       Serial.println(""); // ajuste na impressao serial melhor para visualizar
       delay(1);
     }
   } // FIM WHILE
 } // FIM LOOP

 void imprimir_display_entrada(String a) {
   cont = 1;
   String displayEntrada = a;
   if (displayEntrada[0] == 'E') {
     Serial.println("imprimir_display_entrada");
     Serial.println(displayEntrada);
     if (displayEntrada[1] == '0') {
       Serial.println("SystemFont5x7: ");
       dmd.selectFont(SystemFont5x7);
       Serial.println("  ");
     }
     if (displayEntrada[1] == '1') {
       Serial.println("Droid_Sans_12: ");
       dmd.selectFont(Droid_Sans_12);
       Serial.println("  ");
     }
     if (displayEntrada[1] == '2') {
       Serial.println("Arial14: ");
       dmd.selectFont(Arial14);
       Serial.println("  ");
     }
     if (displayEntrada[1] == '3') {
       Serial.println("Arial_Black_16: ");
       dmd.selectFont(Arial_Black_16);
       Serial.println("  ");
     }
     // retirar digitos significativos da impressao
     int primeiro_marcador = displayEntrada.indexOf('E'); // PEGA O MARCADOR DE FINAL
     int tamanho_corte = displayEntrada.length(); // tamanho
     String strTemp = displayEntrada.substring(primeiro_marcador + 2, tamanho_corte - 1);
     int tamanho_temp = strTemp.length(); // tamanho
     // FIM ESCOLHA FONTE
     box.println(strTemp);
   }
   // CORRIGI O BUG DE APAGAR O DISPLAY DE SAIDA QUANDO IMPRIME O DE ENTRADA
   //imprimir_display_saida(display_saida_temp);
 }

 void imprimir_display_saida(String b) {
   cont = 1;
   String displaySaida = b;
   if (displaySaida[0] == 'S') {
     Serial.println("imprimir_display_saida");
     Serial.println(displaySaida);
     if (displaySaida[1] == '0') {
       Serial.println("SystemFont5x7: ");
       dmd.selectFont(SystemFont5x7);
       Serial.println("  ");
     }
     if (displaySaida[1] == '1') {
       Serial.println("Droid_Sans_12: ");
       dmd.selectFont(Droid_Sans_12);
       Serial.println("  ");
     }
     if (displaySaida[1] == '2') {
       Serial.println("Arial14: ");
       dmd.selectFont(Arial14);
       Serial.println("  ");
     }
     if (displaySaida[1] == '3') {
       Serial.println("Arial_Black_16: ");
       dmd.selectFont(Arial_Black_16);
       Serial.println("  ");
     }
     // retirar digitos significativos da impressao
     int primeiro_marcador = displaySaida.indexOf('S'); // PEGA O MARCADOR DE FINAL
     int tamanho_corte = displaySaida.length(); // tamanho
     String strTemp = displaySaida.substring(primeiro_marcador + 2, tamanho_corte - 1);
     int tamanho_temp = strTemp.length(); // tamanho
     // FIM ESCOLHA FONTE
     box1.println(strTemp);
   }
   imprimir_display_entrada(display_saida_temp);
 }
