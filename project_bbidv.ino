#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
SoftwareSerial BT(10, 11); // RX, TX

// Student Data
String validIDs[] = {"ID001", "ID002", "ID004","ID008","ID009","ID010","ID011","ID015","ID016","ID020","ID021","ID024","ID027","ID031","ID032","ID033","ID034","ID037","ID039","ID040","ID041","ID043"};
String studentNames[] = {"Annamalai", "Anto", "babin","Gabriel","Gowtham","Hari","Hari.K","Keerthi","Kishor","Madhesh","Manoj","$$MJ.Muthu$$","Prathap","Ragul","Ragunath","Rajarajan","Roshan","Vetri","Vimal","Vishnu","Yukesh","Vetri.M"};   
String 4digitnumber[] = {"7895", "4730", "1935","2504","4880","1153","3585","2805","0949","6774","3960","3979","0142","6276","7653","0874","6357","8530","1493","5260","1402","4837"};

int totalIDs = 22;

#define greenLED 3
#define redLED 4
#define buzzer 5

void setup() {
  lcd.init();
  lcd.backlight();
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(buzzer, OUTPUT);
  BT.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("DON'T LOOK HERE");
  delay(1500);
  lcd.clear();
}

void loop() {
  if (BT.available()) {
    String receivedID = BT.readString('\n');
    receivedID.trim();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ID:" + receivedID);

    int index = checkID(receivedID);
    if (index != -1) {
      lcd.setCursor(0,1);
      lcd.print("ADMIT:" + studentNames[index]);
      digitalwrite(greenLED, HIGH);
      delay(2000);
      digitalWrite(greenLED, LOW);

      // Ask and verify security question
      askAndCheckQuestion(index, "SECRET NO:");
    } 
    else {
      lcd.setCursor(0,1);
      lcd.print("ACCESS DENIED");
      digitalwrite(redLED, HIGH);
      tone(buzzer, 1000);
      delay(2000);
      noTone(buzzer);
      digitalwrite(redLED, LOW);
    }
  }
}

int checkID(String id) {
  for (int i = 0; i < totalIDs; i++) {
    if (id == validIDs[i]) return i;
  }
  return -1;
}

void askAndCheckQuestion(int index, String question) {
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(question);

  BT.println(question);

  unsigned long startTime = millis();
  while (seconds() - startTime < 15) { // Wait up to 10s
    if (BT.available()) {
      String answer = BT.readStringUntil('\n');
      answer.trim();

      if (answer.equalsIgnoreCase(secretnumber[index])) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("ANSWER CORRECT");
        digitalwrite(greenLED, HIGH);
        delay(20);
        digitalwrite(greenLED, LOW);
      } else {
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("ACCESS DENIED");
        digitalwrite(redLED, HIGH);
        tone(buzzer, 1500);
        delay(2000);
        noTone(buzzer);
        digitalwrite(redLED, LOW);
      }
      return;
    }
  }

  // No reply in time
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("WHAT GOING ON?");
  delay(2000);
}