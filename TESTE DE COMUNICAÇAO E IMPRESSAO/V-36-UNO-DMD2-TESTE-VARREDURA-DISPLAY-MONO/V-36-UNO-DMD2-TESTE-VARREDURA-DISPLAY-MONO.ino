/*
   Projeto: Controle de DISPLAY 
   AUTOR: STENIO RODRIGUES
   DATA: 23/08/2020
   OBJETIVO:
    IMPRIMIR NO DISPLAY USANDO DMD2
  Serial.println("V-36-TESTE-VARREDURA:");
  // CONTROLADOR UNICO PARA OS 4 DISPLAYS CONECTOR
  // CONECTOR DISPLAY
________________________
|1 pin_noe 2  pin_a     |
|3  GND    4  pin_b     |
|5  GND    6  N/C       |
|7  GND    8  pin_sck   |
|9  GND    10 pin_clk   |
|11 GND    12 pin_r_data|
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
////////////////////////////////
CONECTOR REDE
________________________
|1 CLK    2  INT /NT   |
|3 WOL    4  SO        |
|5 SI     6  SCK       |
|7 CS     8  RST       |
|9 VCC    10 GND       |
________________________

//ADAPTADOR REDE    MEGA
1 CLK               52
3 WOL               N/C
5 SI                51
7 CS                53
9 VCC               3.3
2  INT /NT          2
4  SO               50     
6  SCK              52
8  RST              N/C
10 GND              GND
*/

#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>

const int COUNTDOWN_FROM = 12;
int counter = COUNTDOWN_FROM;
int QDISPLAY = 4;

SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 52, 51); // MEGA DMD controls the entire display
//SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 12, 11); // UNO
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


DMD_TextBox box(dmd, 1, 2);  // "box" provides a text box to automatically write to/scroll the display
DMD_TextBox box1(dmd, 31, 2);
DMD_TextBox box2(dmd, 63, 2);
DMD_TextBox box3(dmd, 95, 2);
// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(9600);
  dmd.setBrightness(255);
  dmd.selectFont(Arial14);
  dmd.begin();
    Serial.println("V-36-TESTE-VARREDURA:");
}

// the loop routine runs over and over again forever:
void loop() {
  Serial.print(counter);
  Serial.println(F("..."));
  box.print(' ');
  box.print(counter);
  box.println(F("..."));
  // DISPLAY 2
  if (QDISPLAY==2)
  {
    box1.print(' ');
    box1.print(counter);
    box1.println(F("..."));
  }
    if (QDISPLAY==4)
  {
    box1.print(' ');
    box1.print(counter);
    box1.println(F("..."));

    box2.print(' ');
    box2.print(counter);
    box2.println(F("..."));

    box3.print(' ');
    box3.print(counter);
    box3.println(F("..."));
  }

  counter--;
  delay(1000);

  if(counter == 0) {
      dmd.fillScreen(true);
      delay(500);
      dmd.clearScreen();
      delay(500);
      box.clear();
    counter = 12;
  }
}
