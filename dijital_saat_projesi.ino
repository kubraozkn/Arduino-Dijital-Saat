#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// LCD Adresi genellikle 0x27 veya 0x3F olur.
LiquidCrystal_I2C lcd(0x27, 16, 2); 

volatile int saniye = 0, dakika = 0, saat = 0;

void setup() {
  lcd.init();
  lcd.backlight();
  
  // TIMER1 SETUP (Gömülü Sistemler Lab Bilgisiyle)
  noInterrupts();           
  TCCR1A = 0;             // Normal mod
  TCCR1B = 0;             // Reset
  TCNT1  = 0;             // Sayacı sıfırla
  OCR1A = 15624;          // 16MHz işlemci için 1 saniyelik eşik değeri
  TCCR1B |= (1 << WGM12); // CTC Modu (Karşılaştır ve Sıfırla)
  TCCR1B |= (1 << CS12) | (1 << CS10); // 1024 Prescaler (Hız bölücü)
  TIMSK1 |= (1 << OCIE1A); // Kesmeyi aktif et
  interrupts();             
}

// Donanımsal Kesme Servis Rutini (ISR)
ISR(TIMER1_COMPA_vect) {
  saniye++;
  if (saniye >= 60) { saniye = 0; dakika++; }
  if (dakika >= 60) { dakika = 0; saat++; }
}

void loop() {
  lcd.setCursor(3, 0);
  lcd.print("Sistem Saati");
  lcd.setCursor(4, 1);
  if(saat < 10) lcd.print("0"); lcd.print(saat);
  lcd.print(":");
  if(dakika < 10) lcd.print("0"); lcd.print(dakika);
  lcd.print(":");
  if(saniye < 10) lcd.print("0"); lcd.print(saniye);
}


