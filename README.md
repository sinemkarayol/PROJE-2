# PROJE-2
LCD Ekran Kullanımı (Tiva C TM4C123)

Bu proje, Texas Instruments Tiva C TM4C123 serisi mikrodenetleyiciler ile bir LCD ekranın nasıl kullanılacağını göstermektedir. Kod, GPIO pinleri üzerinden LCD'ye veri göndererek temel fonksiyonları gerçekleştirmektedir. Ayrıca bir zamanlayıcı kullanılarak saat fonksiyonu da eklenmiştir.

Özellikler
4-bit modunda LCD kontrolü
Ekran temizleme, karakter yazma, imleç konumlandırma
Tiva C mikrodenetleyicisine özel GPIO pin atamaları
Delay kullanılarak zamanlama ayarları
Timer kullanılarak saniye, dakika ve saat sayacı

Donanım Bağlantıları
LCD Pin->Tiva C Bağlantısı
RS->PE4
E->PE5
D4->PC4
D5->PC5
D6->PC6
D7->PC7

Timer Kullanımı
Bu projede Timer 0, 1 saniyelik periyotlarla bir kesme oluşturacak şekilde yapılandırılmıştır. Her kesmede saat güncellenir ve LCD'ye yazdırılır.
Dosya Açıklamaları
main.c: Ana program dosyası, LCD başlatma ve zamanlayıcı işlemlerini içerir.
lcd.c: LCD'nin tüm fonksiyonları burada tanımlanmıştır.
lcd.h: LCD fonksiyon prototipleri ve tanımlamalar içerir.
timer.c: Timer yapılandırma ve kesme fonksiyonlarını içerir.

Kullanılan Kütüphaneler
Kod, TivaWare sürücü kütüphanesini kullanmaktadır. TivaWare yüklenmiş olmalıdır.
driverlib/gpio.h
driverlib/sysctl.h
driverlib/interrupt.h
driverlib/timer.h
inc/hw_types.h
inc/hw_memmap.h
inc/hw_gpio.h

Kurulum ve Derleme
TivaWare Kurulumu: TivaWare Download
Code Composer Studio (CCS) veya Keil IDE kullanarak derleyin
Tiva C TM4C123 LaunchPad'e yükleyin
Bağlantıları yapın ve LCD üzerinde çıktıyı gözlemleyin

Örnek Kullanım
init();
LCD_init();
LCD_SetCursor(0, 0);
LCD_Print("Saat: ");
while(1) {
    // Timer kesmesi ile saat güncellenir ve LCD'ye yazılır.
}
