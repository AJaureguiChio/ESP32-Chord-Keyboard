#include <BleKeyboard.h>

// Crear un objeto de teclado Bluetooth
BleKeyboard bleKeyboard;

// Definir los pines para las teclas de letra
const int hPin = 32;  // GPIO 32
const int ePin = 33;  // GPIO 33
const int lPin = 25;  // GPIO 25
const int oPin = 26;  // GPIO 26
const int bPin = 27;  // GPIO 27

const unsigned long debounceWindow = 200;  // Ventana de tiempo 

void setup() {
  Serial.begin(9600);
  
  // Configuración de los pines de las teclas de letra
  pinMode(hPin, INPUT_PULLUP);
  pinMode(ePin, INPUT_PULLUP);
  pinMode(lPin, INPUT_PULLUP);
  pinMode(oPin, INPUT_PULLUP);
  pinMode(bPin, INPUT_PULLUP);
  
  // Iniciar la función de teclado Bluetooth
  bleKeyboard.begin();
  
  Serial.println("Bluetooth Keyboard is ready to pair");
  Serial.println("Waiting for connection...");
}

void loop() {
  if (!bleKeyboard.isConnected()) {
    Serial.println("Keyboard not connected - waiting...");
    delay(1000);
    return;
  }
  
  // Lectura de todos los botones para verficiar si alguno se encuentra presionado
  bool hPressed = (digitalRead(hPin) == LOW);
  bool ePressed = (digitalRead(ePin) == LOW);
  bool lPressed = (digitalRead(lPin) == LOW);
  bool oPressed = (digitalRead(oPin) == LOW);
  bool bPressed = (digitalRead(bPin) == LOW);
  
  // Si ningún botón se encuentra presionado, no hacer nada
  if (!hPressed && !ePressed && !lPressed && !oPressed && !bPressed) {
    delay(50);
    return;
  }
  
  // Debounce: esperar para que la ventana capture todos los botones que fueron presionados
  unsigned long windowStartTime = millis();
  bool hPressedFinal = hPressed;
  bool ePressedFinal = ePressed;
  bool lPressedFinal = lPressed;
  bool oPressedFinal = oPressed;
  bool bPressedFinal = bPressed;
  
  while (millis() - windowStartTime < debounceWindow) {
    hPressedFinal |= (digitalRead(hPin) == LOW);
    ePressedFinal |= (digitalRead(ePin) == LOW);
    lPressedFinal |= (digitalRead(lPin) == LOW);
    oPressedFinal |= (digitalRead(oPin) == LOW);
    bPressedFinal |= (digitalRead(bPin) == LOW);
    delay(10);
  }
  
  // Crear un patrón binario
  int pattern = (hPressedFinal << 4) | (ePressedFinal << 3) | 
                (lPressedFinal << 2) | (oPressedFinal << 1) | bPressedFinal;
  
  // Imprimir información para debuggear
  Serial.print("Pattern (binary): ");
  Serial.print(hPressedFinal);
  Serial.print(ePressedFinal);
  Serial.print(lPressedFinal);
  Serial.print(oPressedFinal);
  Serial.println(bPressedFinal);
  Serial.print("Pattern (decimal): ");
  Serial.println(pattern);
  
  // Mapeo de patrón asignado a un caracter
  char outputChar = '\0';
  switch (pattern) {
    case 0b10000: outputChar = ' '; break;
    case 0b01000: outputChar = 'e'; break;
    case 0b00100: outputChar = 't'; break;
    case 0b00010: outputChar = 'a'; break;
    case 0b00001: outputChar = 'o'; break;
    case 0b11000: outputChar = 'i'; break;
    case 0b10100: outputChar = 'n'; break;
    case 0b10010: outputChar = 's'; break;
    case 0b10001: outputChar = 'h'; break;
    case 0b01100: outputChar = 'r'; break;
    case 0b01010: outputChar = 'd'; break;
    case 0b01001: outputChar = 'l'; break;
    case 0b00110: outputChar = 'c'; break;
    case 0b00101: outputChar = 'u'; break;
    case 0b00011: outputChar = 'm'; break;
    case 0b11100: outputChar = 'w'; break;
    case 0b11010: outputChar = 'f'; break;
    case 0b11001: outputChar = 'g'; break;
    case 0b10110: outputChar = 'y'; break;
    case 0b10101: outputChar = 'p'; break;
    case 0b10011: outputChar = 'b'; break;
    case 0b01110: outputChar = 'v'; break;
    case 0b01101: outputChar = 'k'; break;
    case 0b01011: outputChar = 'j'; break;
    case 0b00111: outputChar = KEY_BACKSPACE; break;
    case 0b11110: outputChar = 'x'; break;
    case 0b11101: outputChar = 'q'; break;
    case 0b11111: outputChar = '\n'; break;
    case 0b10111: outputChar = '.'; break;
    case 0b11011: outputChar = ','; break;
    case 0b01111: outputChar = '?'; break;
    default: 
      Serial.print("Unknown pattern: ");
      Serial.println(pattern);
      break;
  }
  
  // Envío de caracter vía Bluetooth
  if (outputChar != '\0') {
    Serial.print("Sending: ");
    Serial.println(outputChar);
    
    if (outputChar == KEY_BACKSPACE) {
      bleKeyboard.write(KEY_BACKSPACE);
    } else {
      bleKeyboard.print(outputChar);
    }
    
    delay(300);  // Debounce entre caracteres
  }
  
  // Espera a que todos los botones hayan sido dejados de presionar
  while (digitalRead(hPin) == LOW || digitalRead(ePin) == LOW || 
         digitalRead(lPin) == LOW || digitalRead(oPin) == LOW || 
         digitalRead(bPin) == LOW) {
    delay(10);
  }
  
  delay(100);  // debounce extra antes de la siguiente lectura
}