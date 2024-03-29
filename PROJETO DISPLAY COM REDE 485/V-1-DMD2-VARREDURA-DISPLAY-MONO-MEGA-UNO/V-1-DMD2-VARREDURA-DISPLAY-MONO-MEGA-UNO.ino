/*
 ----------------------------------------------------------
|   PROJETO  : Controle de display via rede 485            |
|   AUTOR    : STENIO RODRIGUES                            |
|   DATA     : 09/08/2021                                  |
|   OBJETIVO :                                             |
|              Imprimir mensagens recebidas via rede 485   |
|              aplicação industria de fertilizantes        |
 ----------------------------------------------------------
      Organizando código.
    // =================================================================================
    // --- Bibliotecas Auxiliares       --- //
    // =================================================================================
    // --- Mapeamento de Hardware       --- //
    // =================================================================================
    // --- Instâncias                   --- //
    // =================================================================================
    // --- Protótipo das Funções        --- //
    // =================================================================================
    // --- Variáveis Globais            --- //
    // =================================================================================
    // --- Interrupções                 --- //
    // =================================================================================
    // --- Configurações Iniciais SETUP --- //
    // =================================================================================
    // --- Loop Infinito                --- //
    // =================================================================================
    // --- Desenvolvimento das Funções  --- //
    // =================================================================================

   Mapeamento hardware
 -----------------------
|1 pin_noe 2  pin_a     |
|3  GND    4  pin_b     |
|5  GND    6  N/C       |
|7  GND    8  pin_sck   |
|9  GND    10 pin_clk   |
|11 GND    12 pin_r_data|
 -----------------------
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

// PAINEL           MEGA      UNO
//byte panelsWide   N/C
//byte panelsHigh   N/C
//byte pin_noe      9         9
//byte GND          GND       GND
//byte pin_a        6         6
//byte pin_b        7         7
//byte pin_sck      52        12    
//byte pin_clk      8         8
//byte pin_r_data   51        11        */

// --- Bibliotecas Auxiliares       --- //
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>
// =================================================================================
// --- Variáveis Globais            --- //
const int COUNTDOWN_FROM = 12;
int counter = COUNTDOWN_FROM;
int QDISPLAY = 1;
int tam_msg = 6;
// --- Instâncias                   --- //
SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 52, 51); // MEGA DMD controls the entire display
//SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 12, 11); // UNO
DMD_TextBox box(dmd, 1, 2); // "box" provides a text box to automatically write to/scroll the display
//DMD_TextBox box1(dmd, 31, 2);
//DMD_TextBox box2(dmd, 63, 2);
//DMD_TextBox box3(dmd, 95, 2);
// =================================================================================
// --- Configurações Iniciais SETUP --- //
void setup() {
    Serial.begin(9600);
    dmd.setBrightness(255);
    dmd.selectFont(Arial14);
    dmd.begin();
    Serial.println("Projeto controle rede de displays fabrica de fertilizantes.\n");
    Serial.println("                                                             ");
    Serial.println("Nome arquivo:V-1-DMD2-VARREDURA-DISPLAY-MONO-MEGA.ino... \n");
    
} // end setup
// =================================================================================
// --- Loop Infinito                --- //
void loop() {
  Serial.print(counter);
  Serial.println(F("..."));
  box.print(' ');
  box.print(counter);
  box.println(F("..."));
  // DISPLAY 2
  counter--;
  delay(1000);

  if(counter == 0) {
      dmd.fillScreen(true);
      delay(500);
      dmd.clearScreen();
      delay(500);
      box.clear();
    counter = 12;
    delay(1000);
  }// end if(counter == 0)
}// end loop
// =================================================================================

