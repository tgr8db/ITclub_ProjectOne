#include <ESP8266WiFi.h>
#include <LedControl.h>

const char* ssid = "itclubproject";
const char* password = "thepassword";
WiFiServer server(80);

int din = 13, clk = 2, cs = 15;
LedControl lc = LedControl(din, clk, cs, 4);

unsigned long delaytime = 100;
int devices = lc.getDeviceCount();

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

void stationary(char x, char y) {  //works
  for (int i = 2; i < 4; i++)
    lc.setColumn(1, i, divider);
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


void setup() {
  for (int i = 0 ; i < devices ; i++) {
    lc.shutdown(i, false);
    lc.setIntensity(i, 0);
    lc.clearDisplay(i)  ;
  }
  Serial.begin(115200);
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.println("Server started");

  Serial.println(WiFi.localIP());
  server.begin();
  stationary('0', '0');

}

void loop() {
  //String s = "<!DOCTYPE html><html><body><style>.button{border-radius:999px;box-shadow:0 6px 0 0 #444;color:#000;display:inline-block;padding:10px 40px;font:normal 700 33px/1 'Droid Serif', serif;text-align:center;text-shadow:0px 0px 0 #000;background:#3d85c6;border:2px solid #569;}</style><form id='frm' name='frm' method='GET'><input type='hidden' name='i' value='1'></form<input type='button' class='button' onclick='location.href='/1'' value='Next'></body></html>");
  /** for (char i = '9'; i > 47; i--)
                                { for (char j = '9'; j > 47; j--)
                                { stationary(i, j);
                                  delay(200); lc.clearDisplay(0); lc.clearDisplay(1);
                                }
                                } //used to debug stationary function...*/
  char curx = 48, cury = 48;
  String request;
  String getval;
  String searchString = "/1";

  // listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    if (client.connected()) {
      Serial.println("Connection established");

      // client.print(s); This was for sending an HTML webpage . Check out the above string s. (In the comment)

      request = client.readString();
      Serial.println(request);
      getval = client.readStringUntil('\r');

      if (request.indexOf(searchString) != -1) {
        if (cury == 57) {
            curx++;
            cury=48; // curx will be representing the character so it will have 0's ASCII value.
        } 
        else cury++;
        
        stationary(curx, cury); // once the loop is over , this can be called to set the MAX7219 Display.
      }
    }
    // close the connection:
    client.stop(); // I want this setup to work for a long time , so when the loop executes again , will this work ?
  }
}
