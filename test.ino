#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include "LedControl.h"
#include <Arduino.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Hash.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server = ESP8266WebServer(80);
WebSocketsServer webSocket = WebSocketsServer(81);
LedControl lc = LedControl(13, 14, 2, 4);

unsigned long delaytime = 100;
int devices = lc.getDeviceCount();
bool pool = true;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  char curx = 48, cury = 48;
  while (pool)
  { stationary(curx, cury);
    pool = false;
  }
  switch (type) {
    case WStype_DISCONNECTED:
      USE_SERIAL.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        USE_SERIAL.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      USE_SERIAL.printf("[%u] get Text: %s\n", num, payload);

      if (payload[0] == '#') {
        // we get abc data

        // decode abc data
        uint32_t abc = (uint32_t) strtol((const char *) &payload[1], NULL, 16);
        String abc2 = (char * )payload;
        if (abc2.substring(14, 18) == "1")
        { if (cury == 57)
            curx++;
          else cury++;
        }
        stationary(curx, cury);
        delay(50);

      }

      break;
  }

}


/************************************/
/*  All the letters and numbers...  */
/************************************/
const byte ch[313] =
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
  B00000000, B00000000, B00000000                        /* */
};
const byte divider   = {B11111111};
byte placeholder[200] = {B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                         B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                         B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
                         B11111110, B01000000, B00100000, B01000000, B11111110,
                         B00000000, B00111110, B00010000, B00100000, B00100000,
                         B00000010, B00000000, B00000000, B00000000
                        };


int initloc(char x) {   //works
  int location;
  if (x >= '0' && x <= '9')  location = 5 * (x - 48)       ;
  if (x >= 'A' && x <= 'Z')  location = 5 * (x - 65) + 50  ;
  if (x >= 'a' && x <= 'z')  location = 5 * (x - 97) + 180 ;
  if (x == ' ')          location = 310            ;
  return location;
}
byte make(char c, int i) { //works
  byte x;
  int loc = initloc(c);
  x = ch[loc + i];
  return x;
}

void scroll(char x, int cur, int last, int  iHelper) {   //some error
  byte bHelper;
  int  n = 5;
  while (cur != last)
    for (int i = 0; i < n; i++)
    { if (x == ' ')n = 3;
      placeholder[iHelper] = make(x, i);
      iHelper++;
      return;
    }
  n = sizeof(placeholder) / sizeof(placeholder[0]);
  bHelper = placeholder[0];
  for (int i = 0; i < n; i++)
  {
    for (int i = 0; i < 2; i++)
      lc.setColumn(1, i, placeholder[i]);
    for (int i = 2; i < 10; i++)
      lc.setColumn(2, i - 2, placeholder[i]);
    for (int i = 10; i < 18; i++)
      lc.setColumn(3, i - 10, placeholder[i]);
    delay(50);

    while (i + 1 != n)
      placeholder[i] = placeholder[i + 1];
    placeholder[n] = bHelper;
  }

}

void stationary(char x, char y) {  //works
  for (int i = 2; i < 4; i++)
    lc.setColumn(1, i, divider);
  byte placeholder[12];
  for (int i = 0; i < 5; i++)
    placeholder[i] = make(x, i);
  placeholder[5] = B00000000;
  for (int i = 0; i < 5; i++)
    placeholder[i + 6] = make(y, i);
  for (int i = 5; i < 8; i++)
    lc.setColumn(1, i, placeholder[i - 5]);
  for (int i = 0; i < 8; i++)
    lc.setColumn(0, i, placeholder[i + 3]);

}


void setup() {
  for (int i = 0 ; i < devices ; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 0);
    lc.clearDisplay(i)  ;
  }
  USE_SERIAL.begin(115200);
  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();
  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }
  WiFiMulti.addAP("project", "password");
  while (WiFiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  if (MDNS.begin("esp8266")) {
    USE_SERIAL.println("MDNS responder started");
  }
  server.on("/", []() {
  
    server.send(200, "text/html", "<html><head><script>var connection = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);connection.onopen = function () {  connection.send('Connect ' + new Date()); }; connection.onerror = function (error) {    console.log('WebSocket Error ', error);};connection.onmessage = function (e) {  console.log('Server: ', e.data);};function sendi() {  var i = document.getElementById('r').value;  console.log('INC: ' + abc); connection.send(i); }</script></head><body>Incrementor : <br/><br/> <input id=\"i\" type=\"range\" min=\"0\" max=\"1\" step=\"1\" onchange=\"sendi();\" /><br/><br/></body></html>");
  });
  server.begin();
  MDNS.addService("http", "tcp", 80);
  MDNS.addService("ws", "tcp", 81);
}

void loop() {
  //String s = "<!DOCTYPE html><html><body><style>.button{border-radius:999px;box-shadow:0 6px 0 0 #444;color:#000;display:inline-block;padding:10px 40px;font:normal 700 33px/1 'Droid Serif', serif;text-align:center;text-shadow:0px 0px 0 #000;background:#3d85c6;border:2px solid #569;}</style><form id='frm' name='frm' method='GET'><input type='hidden' name='i' value='1'></form<input type='button' class='button' onclick='location.href='/1'' value='Next'></body></html>";
  /*char s[21] = "Aman Dwivedi"; //scrollable name through device 2 and 3 ...
    //nonscrollable number through device 0 and 1 ...
                                  int ihelper;
                                  for (int x = 0; x < 20; x++)
                                { if (s[x-1] == ' ')
                                  ihelper = 5 * (x - 1) + 36;    // used to debug scroll function
                                  else ihelper = 5 * x + 33;
                                  scroll(s[x], x, 20, ihelper);
                                }  */

  /** for (char i = '9'; i > 47; i--)
                                { for (char j = '9'; j > 47; j--)
                                { stationary(i, j);
                                  delay(200); lc.clearDisplay(0); lc.clearDisplay(1);
                                }
                                } //used to debug stationary function...*/
   webSocket.loop();
  server.handleClient();
}

