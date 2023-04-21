//programm Schliessanlage; Version 1.01 (Laienfreundlich kommentiert [fast...]) 
//DAA-DT_W23[doofer]FiSi Nikolay Shtanin
#include <LiquidCrystal.h>
#include <Keypad.h>
#include <Servo.h>
//#include "Ge.h"/rn
//Lass den Pre-Processor über den Typ der folgenden Konstanten entscheiden
#define ANDF A4 //Reserviert
#define ANFD A5 //Reserviert
#define BACKLIGHT 13 //Hintergrundbeleutung 
#define SERVO_PIN 6 //Definiere Servo-Parameter
#define SERVO_LOCK_POS 93 //Verriegelt
#define SERVO_UNLOCK_POS 3 //Entriegelt
#define KEYPAD_ROWS 4 //Keypad-Zeilen
#define KEYPAD_COLS 4 //Keypad-Spalten
#define LCD_COLS 16 //LCD-Spalten
#define LCD_ROWS 2 //LCD-Zeilen
const String HELLO = "X:A, <-:C, OK:D";//LCDs 1.Zeile 
const unsigned long sleepTimer = 10000; // nach 10s ohne Aktivität
//var
unsigned long previousMillis;//Aktivitätsmarker
byte cursorPos;//Position des Cursors innerhalb der Zeile
bool cheatingOK = false;  //Backdoor-Trigger
bool active; //Keypad kann Zahleneingabe annehmen
byte fails; //Anzahl Fehlversuche
String testPin; //Eingegebene PIN
//functions
bool testOK();   //Vergleich der Eingabe auf Übereinstimmung mit jedem gültigen Schlüssel
void resetPad(); //PIN-Eingabe zurücksetzen
void backDoor(); //Entriegelt den Schloss und verriegelt ihn wieder nach einer Pause 
byte setCharLCD(byte pos, char arg);//Schreibe das Zeichen arg auf der Stelle pos in der unteren Zeile
//objects
// Initialisiere Keypad
byte rowPins[KEYPAD_ROWS] = {5, 4, 3, 2};//Pins für Zeilen
byte colPins[KEYPAD_COLS] = {A0, A1, A2, A3};//Pins für Spalten
char keys[KEYPAD_ROWS][KEYPAD_COLS] = {//Standardbelegung der Tasten z.B.: keys[2][0]='7'
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};                                      //Keypad mit Konstruktor anlegen
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, KEYPAD_ROWS, KEYPAD_COLS);
Servo servo;                            // Erzeuge Servo
LiquidCrystal lcd(12, 11, 10, 9, 8, 7); // Erstelle Display
String secretPins [] = {"0000", "1234", "1111", "2222", "3333", "4444", "5555",
                        "7777", "8888", "6969", "7676",}; //Damit kommste rein
void setup() {//begin
  servo.attach(SERVO_PIN);//Servo mit dem Pin verbienden
  pinMode(BACKLIGHT, OUTPUT); //Hintergrundbeleuchtung steuerbar
  lcd.begin(LCD_COLS, LCD_ROWS);//LCD starten
  digitalWrite(BACKLIGHT, HIGH); //Hintergrundbeleuchtung AN
  lcd.print("Crazy Nicky Inc.");//Authoren kurz würdigen
  fails = 0;  //Fehlversuche zurücksetzen
  delay(500); 
  resetPad(); //Zurücksetzen
}
void loop() {//repeat
  char key = NO_KEY;//Abgelesene Taste zurücksetzen
  key = keypad.getKey();//Versuche Taste abzulesen
  if (key != NO_KEY) {//Falls eine Taste gedrückt wurde
    if (active) { //Wenn auch noch Zahleneingabe erlaubt ist
      switch (key) {//Werte die Taste aus
        case 'D': //Eingabe bestätigen, die bis hier eingetippte Eingabe prüfen
          if (testOK()) backDoor(); //Bei positivem Ergebnis Schloss entriegeln
          else resetPad(); //sonst Neuversuch ermöglichen
          break; 
        case 'C':             //Korrektur
          if(cursorPos > 0){  //Falls die Eingebe nicht leer ist
            testPin.remove(--cursorPos); //Lösche das letzte Zeichen von der Eingabe
            cursorPos = setCharLCD(cursorPos, ' ') - 1; //und vom LCD
          }
          break;
        case 'B':                     //Wenn Keypad aktiviert und 
          if(cheatingOK) backDoor();  //Schummeln erlaubt ist, Schloss entriegeln
          break;
        case '*': break; //noch nicht definiert
        case '#': //Schummeln gestatten/verbieten
          cheatingOK = !cheatingOK; //Flip-Flop (den Zustand umkehren)
          break; 
        default: 
          if(key != 'A'){   //wenn kein Abbruch
            testPin += key; //nächstes Zeichen der abgelesenen Eingabe hinzufügen
            cursorPos = setCharLCD(cursorPos, key);//und auf dem LCD wiedergeben
            previousMillis = millis();  //Aktivitätsmarker erneuern 
            break;
          }
          else {        //beim Abbruch
            resetPad(); //Zurücksetzen
          }
       }//switch Ende
    }//Ende if active
    else {//wenn Zahleneingabe nicht erlaubt ist
      if (cheatingOK && (key == 'B')) backDoor();//Schummeln, wenn gestatten
      if (key == 'A') resetPad(); //Zahleneingabe Aktivieren
    }
  }//Ende Tastendrück
  if ((digitalRead(BACKLIGHT) == HIGH) && //Falls Hintergrundbeleuchtung AN ist
     (millis() - previousMillis >= sleepTimer)) {//und bei längerer Inaktivität
      resetPad();                  //Zurücksetzen
      //lcd.print("off A - activate");//für WOKWI, da keine Hintergrundbeleuchtung
      digitalWrite(BACKLIGHT, LOW);//Hintergrundbeleuchtung abschalten
      active = false;              //Zahleneingabe sperren
   }
} //untill (0=1); end. 

void backDoor() {//Entriegelt den Schloss und verriegelt ihn wieder nach einer Pause 
  active = false;//Sperre der Zahleneingabe während des Vorgangs
  fails = 0;     //Zurücksetzen der Fehlversuche
  lcd.clear();                //Ausgabe
  lcd.print("Wellcome home!");//anpassen
  for (byte i = SERVO_LOCK_POS; i > SERVO_UNLOCK_POS; --i) {
    servo.write(i);//langsam entriegeln
    delay(12);
  }
  delay(3000);     //Warten mit offenem Schloss
  for (byte i = SERVO_UNLOCK_POS; i < SERVO_LOCK_POS; ++i) {
    servo.write(i);//langsam verriegeln
    delay(12);
  }
  resetPad();     //Zurücksetzen
}
void resetPad() {      //Zurücksetzen
  testPin = "";        //Versuchs-PIN zurücksetzen
  digitalWrite(BACKLIGHT, HIGH);//LCD-Hinterbeleuchtung einschalten
  lcd.clear();         //LCD Inhalt löschen
  lcd.print(HELLO);    //Oberste zeile ausgeben
  lcd.setCursor(0, LCD_ROWS-1);//Cursor auf den Anfang der untersten Zeile stellen
  cursorPos = 0;       //Position des Cursors zurücksetzen
  active = true;       //Zahleneingabe zulassen
  previousMillis = millis();//Aktivitätsmarker zurücksetzen
}
bool testOK() { /* Vergleich der Eingabe auf Übereinstimmung mit jedem Schlüssel.
  Teile die Gesamtgröße des Arrays dürch die Größe eines Elementes, um an die
  Gesamtanzahl der Elemente zu kommen */
  for(word i = 0; i < (sizeof(secretPins)/sizeof(secretPins[i])); i++){
    if(testPin.equals(secretPins[i])){  //sobald ein Treffer gefunden
      fails = 0;                        //Fehlversuche zurücksetzen
      testPin = "";                     //Eingabe zurüksetzen
      return true;                      //Test bestanden
    }
  }     //Wenn keine übereinstimmung gefunden, muss die Eingabe falsch sein
  fails++;           //Fehlversuch zur Kenntnis nehmen
  unsigned long      //Strafe für falsche Eingabe
  penalty = pow(100, fails)/10000; //Nach dem 3.Fehlversuch sperren für 100s
  while(penalty > 0){             //Nach dem 4. für 10ks und so weiter
    lcd.clear();                  //Alte Ausgabe löschen
    lcd.print("Wrong PIN!");      //und Neue aufbauen
    lcd.setCursor(0, LCD_ROWS-1); //Auf den Anfang der unteren Zeile springen
    lcd.print(("wait: " + (String)penalty));//entsprechnde Meldung mit Sekundenabzähler
    delay(1000);
    penalty--;                  //Reststrafe reduzieren
  }
  return false;                 //Test durchgefallen
}
//Wer das selber versteht, ist cool (*Aufgabe;)
byte setCharLCD(byte pos, char arg) {
  lcd.setCursor(pos, LCD_ROWS - 1);
  lcd.print(arg);
  return ++pos;
}
