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
    Serial.println("V-35-DMD2-RETORNO-REDE-150-ENVIA--SERIAL1:");
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
#include <SPI.h>

#include <DMD2.h>

#include <fonts/Arial14.h>

#define led 13
const int COUNTDOWN_FROM = 12;
int counter = COUNTDOWN_FROM;
int QDISPLAY = 2;

//SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 52, 51); // MEGA DMD controls the entire display
SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 12, 11); // UNO
// PAINEL           MEGA      UNO
//byte panelsWide   N/C
//byte panelsHigh   N/C
//byte pin_noe      9         9
//byte GND          GND       GND
//byte pin_a        6         6
//byte pin_b        7         7
//byte pin_sck      52        12    
//byte pin_clk      8         8
//byte pin_r_data   51        11

DMD_TextBox box(dmd, 1, 2); // "box" provides a text box to automatically write to/scroll the display
DMD_TextBox box1(dmd, 31, 2);
DMD_TextBox box2(dmd, 63, 2);
DMD_TextBox box3(dmd, 95, 2);

void setup() {
    dmd.setBrightness(255);
    dmd.selectFont(Arial14);
    dmd.begin();
    Serial.begin(9600);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    Serial.println("V-38-UNO-TESTE-VARREDURA-REDE-RECEBE:");
}

void loop() {
   delay(1000);
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c == 'A') {
            digitalWrite(led, HIGH);
            box.print(' ');
            box.print(c);
            box.println(F("..."));

            box.print(' ');
            box.print(c);
            box.println(F("..."));

        }
        if (c == 'B') {
            digitalWrite(led, LOW);
            box.print(' ');
            box.print(counter);
            box.println(F("..."));
            counter--;

            box1.print(' ');
            box1.print(counter);
            box1.println(F("..."));
            counter--;
        }
       
    }
    // DISPLAY 2
    if (QDISPLAY == 4) {
        box2.print(' ');
        box2.print(counter);
        box2.println(F("..."));

        box3.print(' ');
        box3.print(counter);
        box3.println(F("..."));
    }

    if (counter == 0) {
        dmd.fillScreen(true);
        delay(500);
        dmd.clearScreen();
        delay(500);
        box.clear();
        counter = 12;
    }
} // FIM DO LOOP.