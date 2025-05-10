#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include <SoftwareSerial.h>

// Pin Definitions
#define GREEN_LED 10
#define RED_LED 11
#define RX_PIN 8
#define TX_PIN 9

// Servo Motor Setup
Servo doorServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // LCD setup (address 0x27, 16 columns, 2 rows)

// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8, 9};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Bluetooth Setup
SoftwareSerial BTSerial(RX_PIN, TX_PIN); // RX, TX pins for Bluetooth communication

// PIN Setup
const String correctPassword = "2026";
String enteredPassword = "";
int attemptCounter = 0;
bool locked = false;
String generatedPin = "";

void setup() {
  // Initialize Serial Monitor for debugging
  Serial.begin(9600);

  // Initialize Servo Motor
  doorServo.attach(12);

  // Initialize LED Pins
  pinMode(13, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Initialize Bluetooth Communication
  BTSerial.begin(9600);  // HC-05 default baud rate

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome...");
  lcd.setCursor(0, 1);
  lcd.print("System Ready");
  delay(3000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
  doorServo.write(0);
}

void loop() {
  char key = keypad.getKey();
  if (!key) return;

  Serial.print(key);

  // When # is pressed, check PIN
  if (key == '#') {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Entered: " + enteredPassword);
    Serial.println("\nEntered PIN: " + enteredPassword);
    delay(1000);

    if (locked)
      verifyGeneratedPin(enteredPassword);
    else
      checkPassword(enteredPassword);

    enteredPassword = "";
  } 
  // When * is pressed, clear entered password
  else if (key == '*') {
    enteredPassword = "";
  } 
  // Add the entered key to the password string
  else if (enteredPassword.length() < 6) {
    enteredPassword += key;
  }

  // Update LCD with password entry
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < enteredPassword.length(); i++) {
    lcd.print('*');
  }
}

void checkPassword(String input) {
  bool correct = (input == correctPassword);
  lcd.clear();
  lcd.print(correct ? "Access Granted" : "Incorrect PIN");
  Serial.println(correct ? "Access Granted" : "Incorrect PIN. Attempt: " + String(++attemptCounter));

  if (correct) {
    unlockDoor();
    attemptCounter = 0;
  } else {
    digitalWrite(13, HIGH);
    digitalWrite(RED_LED, HIGH);
    delay(2000);
    digitalWrite(13, LOW);
    digitalWrite(RED_LED, LOW);
    (attemptCounter >= 3) ? generateRandomPin() : lcd.print("\nEnter PIN:");
  }
}

void unlockDoor() {
  digitalWrite(GREEN_LED, HIGH);
  doorServo.write(90);  // Unlock the door (servo moves to 90 degrees)
  delay(4000);          // Keep door unlocked for 4 seconds
  doorServo.write(0);   // Lock the door back (servo moves to 0 degrees)
  digitalWrite(GREEN_LED, LOW);
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Door Locked");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
}

void generateRandomPin() {
  generatedPin = String(random(1000, 9999));  // Generate a random PIN
  Serial.println("Generated PIN sent via Bluetooth");
  BTSerial.println(generatedPin);  // Send the generated PIN to the mobile app via Bluetooth

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Check Mobile App");
  delay(4000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter PIN:");
  locked = true;
}

void verifyGeneratedPin(String input) {
  if (input == generatedPin) {
    Serial.println("PIN Verified. Door Unlocking...");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PIN Verified");
    unlockDoor();
    locked = false;
    attemptCounter = 0;
  } else {
    Serial.println("Incorrect PIN. Try again.");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wrong PIN!");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter PIN:");
  }
}
