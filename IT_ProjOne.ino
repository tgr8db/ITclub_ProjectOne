#include <LedControl.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/* This character has been edited from "const char*" to "const char"*/
// It didn't improve the speed, so I'm reverting it back.

const char* ssid = "cslab_roomx";
const char* password = "projectone";

unsigned long prevScroll = 0;
const long interval = 75;

ESP8266WebServer server(80);
IPAddress ip(192, 166, 3, 2);   //for both ip and gateway
IPAddress subnet(255, 255, 255, 0);

char nox = '0', noy = '0';

void increment() {
  if (nox == '9' && noy == '9') {
    nox = '0';
    noy = 47;
  }
  if (noy == '9') {
    nox++;
    noy = 47;
  }
  noy++;
}

void handleGET() {
  if (server.arg("i") == "1") {
    increment();
    stationary(nox, noy);
  }
  else if (server.arg("i") == "0") {
    nox = '0';
    noy = '0';
    stationary(nox, noy);
  }
  server.send(200, "text/html", "<html><body><form method=\"get\" action=\"/\"><input type=\"hidden\" name=\"i\" value=\"0\"><input type=\"submit\" value=\"Reset\"></form><form method=\"get\" action=\"/\"><input type=\"hidden\" name=\"i\" value=\"1\"><input type=\"submit\" value=\"Next \"></form></body></html>");
}

int din = 2, clk = 13, cs = 15;
LedControl lc = LedControl(din, clk, cs, 4);

int devices = lc.getDeviceCount();
char sex = 'm'; //m for male , f for female (To add Mr. OR Mrs. before the name...)
char thename[] = "Aman Dwivedi";
int y = sizeof(thename) / sizeof(byte); //to get the size of the name (just like strlen in c++)...
byte plch[92];  // After changing the name , run the program once, use the value of r from the serial monitor (approx. r-7 use trial and error method) and use it as the size of this array...
int r;

/************************************/
/*  All the letters and numbers...  */
/************************************/
const byte ch[315] =
{
  B01111100, B10100010, B10010010, B10001010, B01111100, /*0*/
  B00100010, B01000010, B11111110, B00000010, B00000010, /*1*/
  B01001110, B10001010, B10001010, B10001010, B01111010, /*2*/
  B10000100, B10010010, B10110010, B11010010, B10001100, /*3*/
  B00011000, B00101000, B01001000, B11111110, B00001000, /*4*/
  B11110100, B10010010, B10010010, B10010010, B10001100, /*5*/
  B01111100, B10010010, B10010010, B10010010, B01001100, /*6*/
  B10000000, B10001110, B10010000, B10100000, B11000000, /*7*/
  B01101100, B10010010, B10010010, B10010010, B01101100, /*8*/
  B01100100, B10010010, B10010010, B10010010, B01111100, /*9*/
  B01111110, B10001000, B10001000, B10001000, B01111110, /*A*/
  B11111110, B10000010, B10000010, B10010010, B01101100, /*B*/
  B01111100, B10000010, B10000010, B10000010, B01000100, /*C*/
  B10000010, B11111110, B10000010, B10000010, B01111100, /*D*/
  B11111110, B10010010, B10010010, B10000010, B10000010, /*E*/
  B11111110, B10010000, B10010000, B10010000, B10010000, /*F*/
  B11111110, B10000010, B10001010, B10001010, B10001110, /*G*/
  B11111110, B00010000, B00010000, B00010000, B11111110, /*H*/
  B10000010, B10000010, B11111110, B10000010, B10000010, /*I*/
  B10001110, B10000010, B11111110, B10000000, B10000000, /*J*/
  B11111110, B00010000, B00101000, B01000100, B10000010, /*K*/
  B11111110, B00000010, B00000010, B00000010, B00000010, /*L*/
  B11111110, B01000000, B00100000, B01000000, B11111110, /*M*/
  B11111110, B01000000, B00100000, B00010000, B11111110, /*N*/
  B01111100, B10000010, B10000010, B10000010, B01111100, /*O*/
  B01111110, B10010000, B10010000, B10010000, B01100000, /*P*/
  B01111100, B10000010, B10000010, B10000010, B01111101, /*Q*/
  B11111110, B10010000, B10001000, B10010100, B01100010, /*R*/
  B01100100, B10010010, B10010010, B10010010, B01001100, /*S*/
  B10000000, B10000000, B11111110, B10000000, B10000000, /*T*/
  B11111100, B00000010, B00000010, B00000010, B11111100, /*U*/
  B11111000, B00000100, B00000010, B00000100, B11111000, /*V*/
  B11111110, B00000100, B00001000, B00000100, B11111110, /*W*/
  B11100110, B00101000, B00010000, B00101000, B11100110, /*X*/
  B11100100, B00010010, B00010010, B00010010, B11111100, /*Y*/
  B11000110, B10001010, B10010010, B10100010, B11000110, /*Z*/
  B00101110, B00101010, B00111110, B00000010, B00000000, /*a*/
  B11111110, B00010010, B00010010, B00001100, B00000000, /*b*/
  B00111110, B00100010, B00100010, B00100010, B00000000, /*c*/
  B00011110, B00010010, B00010010, B11111110, B00000000, /*d*/
  B00111110, B00101010, B00101010, B00111010, B00000000, /*e*/
  B00010000, B11111110, B10010000, B10010000, B11000000, /*f*/
  B00011010, B00101001, B00101001, B00011110, B00100000, /*g*/
  B11111110, B00010000, B00010000, B00001110, B00000000, /*h*/
  B00000010, B01011110, B00000010, B00000000, B00000000, /*i*/
  B00000110, B00000001, B01011110, B00000000, B00000000, /*j*/
  B11111110, B00001000, B00010100, B00100010, B00000000, /*k*/
  B11111110, B00000000, B00000000, B00000000, B00000000, /*l*/
  B00111110, B00010000, B00011000, B00010000, B00011110, /*m*/
  B00100000, B00111110, B00010000, B00011110, B00000000, /*n*/
  B00011100, B00100010, B00100010, B00011100, B00000000, /*o*/
  B00111111, B00010100, B00011100, B00000000, B00000000, /*p*/
  B00011100, B00010100, B00111111, B00000001, B00000000, /*q*/
  B00111110, B00010000, B00100000, B00100000, B00000000, /*r*/
  B00011010, B00101010, B00101010, B00101100, B00000000, /*s*/
  B00100000, B11111100, B00100010, B00000100, B00000000, /*t*/
  B00111100, B00000010, B00000010, B00111100, B00000010, /*u*/
  B00111000, B00000100, B00000010, B00000100, B00111000, /*v*/
  B00111100, B00000010, B00000100, B00000010, B00111100, /*w*/
  B00100010, B00010100, B00001000, B00010100, B00100010, /*x*/
  B00111001, B00001001, B00001001, B00111111, B00000000, /*y*/
  B00100110, B00101010, B00110010, B00100010, B00000000, /*z*/
  B00000000, B00000000, B00000000,                       /* */
  B00000010                                              /*.*/
};
const byte divider   = {B11111111};

int initloc(char x) {
  int location;
  if (x >= '0' && x <= '9')  location = 5 * (x - 48)       ;
  if (x >= 'A' && x <= 'Z')  location = 5 * (x - 65) + 50  ;
  if (x >= 'a' && x <= 'z')  location = 5 * (x - 97) + 180 ;
  if (x == ' ')              location = 310                ;
  if (x == '.')              location = 313                ;
  return location;
}
byte make(char c, int i) {
  byte x;
  int loc = initloc(c);
  x = ch[loc + i];
  return x;
}

void stationary(char x, char y) {
  for (int i = 2; i < 4; i++)
    lc.setColumn(1, i, divider);
  lc.setColumn(1, 4, 0);
  byte pholder[12];
  for (int i = 0; i < 5; i++)
    pholder[i] = make(x, i);
  pholder[5] = B00000000;
  for (int i = 0; i < 5; i++)
    pholder[i + 6] = make(y, i);
  for (int i = 5; i < 8; i++)
    lc.setColumn(1, i, pholder[i - 5]);
  for (int i = 0; i < 8; i++)
    lc.setColumn(0, i, pholder[i + 3]);

}
void onehelp(char curchar , int i) {
  int w;
  for (w = 0; w < i; w++) {
    plch[w + r] = make(curchar, w);
  }
  plch[i + r] = B00000000;
  Serial.print(" ");
  Serial.print(plch[w + r]);
  Serial.println();
}
void makeplaceholder() {

  for (int x = 0; x < y; x++) {
    char curchar = thename[x];

    if (curchar == ' ') {
      onehelp(curchar, 2);
      r += 3;
    }
    else if (curchar == 'a' || curchar == 'b' || curchar == 'c' || curchar == 'd' || curchar == 'e' || curchar == 'h' || curchar == 'k' || curchar == 'n' || curchar == 'o' || curchar == 'q' || curchar == 'r' || curchar == 's' || curchar == 't' || curchar == 'y' || curchar == 'z') {
      onehelp(curchar, 5);
      r += 5;
    }
    else if (curchar == 'i' || curchar == 'j' || curchar == 'p') {
      onehelp(curchar, 5);
      r += 4;
    }
    else if (curchar == 'l') {
      onehelp(curchar, 5);
      r += 2;
    }
    else if (curchar != ' ' && curchar != '.') {
      onehelp(curchar, 5);
      r += 6;
    }
    else {
      onehelp(curchar, 1);
      r += 2;
    }
  }
}
void scroll() {
  int w = sizeof(plch) / sizeof(byte);
  byte zhelp;
  zhelp = plch[0];

  for (int i = 0; i < 2; i++)
    lc.setColumn(1, i, plch[i + 16]);
  for (int j = 0; j < 8; j++)
    lc.setColumn(2, j, plch[j + 8]);
  for (int k = 0; k < 8; k++)
    lc.setColumn(3, k, plch[k]);
  for (int q = 0; q < w; q++)
    plch[q] = plch[q + 1];
  plch[w] = zhelp;

}
void initHardware() {
  for (int i = 0 ; i < devices ; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 0);
    lc.clearDisplay(i)  ;
  }
  Serial.begin(115200);
  Serial.println();
  Serial.println("Started...");
  for (int l = 0; l < 18; l++)
    plch[l] = B00000000;
  byte secm[] = {B11111110, B01000000, B00100000, B01000000, B11111110, B00000000, B00111110, B00010000, B00100000, B00100000, B00000010, B00000000, B00000000, B00000000};
  byte secf[] = {B11111110, B01000000, B00100000, B01000000, B11111110, B00000000, B00111110, B00010000, B00100000, B00100000, B00000000, B00011010, B00101010, B00101010, B00101100, B00000000, B00000010, B00000000, B00000000, B00000000};
  if (sex == 'm') {
    for (int h = 0; h < 14; h++)
      plch[18 + h] = secm[h];
    r = 32;
  }
  else if (sex == 'f') {
    for (int h = 0; h < 20; h++)
      plch[18 + h] = secf[h];
    r = 38;
  }
  makeplaceholder();
}
void initHotspot() {
  WiFi.softAP(ssid, password);

  server.on("/", HTTP_GET, handleGET);

  server.begin();
  WiFi.softAPConfig(ip, ip, subnet);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}
void setup() {
  initHardware();
  initHotspot();
  stationary(nox, noy);
}

void loop() {
  unsigned long curScroll = millis();
  server.handleClient();
  if (curScroll - prevScroll >= interval) {
    prevScroll = curScroll;
    scroll();
  }
}
