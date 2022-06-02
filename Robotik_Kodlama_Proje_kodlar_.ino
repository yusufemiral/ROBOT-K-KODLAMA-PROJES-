#include <LCD5110_Basic.h> //5110 ekranımızın çalışması için gerekli kütüphaneyi ekliyoruz.
LCD5110 myGLCD(8, 9, 10, 11, 12); // Ekran objemizi bağlamış olduğumuz pinleri belirterek oluşturuyoruz
#define USE_ARDUINO_INTERRUPTS true // eklemiş olduğumuz Pulse kütüphanesinin daha doğru ölçüm yapaması için bu ayarı etkinleştirdik 
#include <PulseSensorPlayground.h> //pulse sensorunun kütüphanesini ekledik
int nabiz; //nabzı tutacağımız değişkeni oluşturduk.
int lcd_ledi = 6; //5110 ekranımızın arka plan aydınlatmasının bağlı olduğu pini belirliyoruz.
extern uint8_t SmallFont[];
extern uint8_t BigNumbers[]; //ekranımızda yazılcak yazı fontlarını ekliyoruz
const int PulseWire = 0; // Pulse sensörümüzü bağlamış olduğumuz Analog pinini belirliyoruz.
const int LED13 = 13; // nabız algılandığında ardunio üzerindeki ledin nabzımıza birlikte yanıp sönmesi için 13 numaralı pin olarak ayarlıyoruz 
int Threshold = 580; // eşik değerini değişkenimize yazdık
PulseSensorPlayground pulseSensor; //Sensörümüzü kodumuzda kullanabilmek için onu obje olarak oluşturuyoruz.

void setup() {
  Serial.begin(9600);  //Bilgisayrımızla olan seri iletişimi başlatıyoruz.
  pulseSensor.analogInput(PulseWire); //Pulse sensörünün bağlıu olduğu pini belirliyoruz.
  pulseSensor.blinkOnPulse(LED13); //arduino üzerindeki ledin nabzımızla yanıp sönmesini istediğimizi söylüyoruz.
  pulseSensor.setThreshold(Threshold); //Değişkene atamış olduğumuz eşik değerini uyguluyoruz.
  digitalWrite(lcd_ledi, HIGH); //5110 ekranın arka plan aydınlatmasına güç veriyoruz.
  myGLCD.InitLCD(); //5110 ekranımızı başlatıyoruz.
  myGLCD.setContrast(58); //5110 ekranımızın kontrastını belirliyoruz.
  myGLCD.setFont(SmallFont); //yazacağımız yazıların fontlarını ekliyoruz
  //myGLCD.drawBitmap(0,0, cmon, 84, 48);
  myGLCD.print("Dakikadaki", CENTER, 0);
  myGLCD.print("Nabziniz:", CENTER, 15); //Ekrana dakikadaki nabzınız yazdırdık.
  myGLCD.setFont(BigNumbers); //yazacağımız yazının yazı fontunu ekledik
  myGLCD.printNumI(0, CENTER, 25); //Dakikadaki nabzınız yazısının altına 0 yazıyoruz.
  if (pulseSensor.begin()) {
  Serial.println("Pulse sensörü objesini yarattık."); 
  } //Pulse sensörü başarıyla başlatılırsa bilgisayara mesaj gönderioyoruz.

}

void loop() {
  // put your main code here, to run repeatedly:
  if (pulseSensor.sawStartOfBeat()) { //Eğer nabız algılarsak
    nabiz = pulseSensor.getBeatsPerMinute(); //Dakikadaki nabzı nabiz değişkenine kaydediyoruz.
    myGLCD.clrScr(); //5110 ekranı temizliyoruz.
    myGLCD.setFont(SmallFont); //Birazdan yazacağımız yazıların küçük fontla yazılmasını sağlıyoruz.
    myGLCD.print("Dakikadaki", CENTER, 0);
    myGLCD.print("Nabziniz:", CENTER, 15); //Ekrana dakikadaki nabzınız yazdırıyoruz.
    myGLCD.setFont(BigNumbers); //Birazdan yazacağımız yazıların büyük sayı fontuyla yazılmasını sağlıyoruz.
    myGLCD.printNumI( nabiz, CENTER, 25); //Dakikadaki nabzınız yazısının altına dakikadki nabzı değerini yazıyoruz.
    Serial.println("Nabız attı. ");
    Serial.print("BPM: ");                        
    Serial.println(nabiz); //Dakikdaki nabız değerini aynı zamanda bilgisayarımıza da gönderiyoruz.
  }


  delay(20); //20 milisaniye bekletiyoruyuz.

}
