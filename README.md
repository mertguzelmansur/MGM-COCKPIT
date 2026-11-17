MGM Sürüş Kokpiti

Web tabanlı araç kokpiti ve akıllı arıza tespiti. BLE ELM327 adaptörüne tarayıcıdan bağlanır, araç verisini canlı gösterir ve ölçülen değerlerden araç sağlığını yorumlar.

EN: A browser-based car cockpit and diagnostics app. It connects to a BLE ELM327 adapter over Web Bluetooth, shows live OBD-II data and assesses vehicle health from the measured values.

Tek dosyalık (index.html), kurulum gerektirmeyen, telefon öncelikli bir uygulamadır. Arayüz Türkçe ve koyu temalıdır. Telefonda alt sekme çubuğu, geniş ekranda sol menü kullanılır.

Özellikler
Bağlantı: Web Bluetooth ile BLE ELM327 adaptörüne bağlanır. Kayıtlı adaptör adına, bilinen adaptör adlarına, BLE servislerine ya da tüm cihazlara göre tarama yapar. İlk kurulumda adım adım bir sihirbaz vardır. Bağlantı günlüğünde kullanılan protokol görünür.
Kokpit: Canlı değerler (ör. devir) gösterge ve temalar üzerinde gösterilir. Hangi bilgilerin hangi kutuda görüneceği ayarlanabilir.
Araç sağlığı: Bulgular ölçülen değerlere dayanır ve acil olandan başlayarak sıralanır. Uygulama aracın normal değerlerini sürüş boyunca öğrenir.
Arıza kodları: Kayıtlı ve bekleyen arıza kodlarını (DTC) okur. Arıza anında motor beyninin kaydettiği değerleri (freeze frame) gösterir. CAN'li araçlarda silindir başına tekleme sayacını (Mode 06) okur.
Kayıt ve takip: Depo seviyesindeki sıçramadan yakıt alımını otomatik algılar. Pompadaki litre girilirse tüketim hesabı araca göre düzeltilir. Motor 2 dakika kapalı kalınca yolculuğu kaydeder. Son 6 ayın aylık özetini ve araç hızı ile GPS hızı arasındaki sapmayı gösterir.
Demo modu: Araç olmadan teşhisin nasıl çalıştığını görmek için örnek arızalar sunar.
Veri gizliliği: Öğrenilen değerler ve yolculuk geçmişi cihazın tarayıcısında (localStorage) saklanır, sunucuya gönderilmez.
Gereksinimler
BLE destekli bir ELM327 OBD-II adaptörü
Web Bluetooth destekleyen bir tarayıcı (ör. Android veya masaüstü Chrome / Edge). iOS Safari Web Bluetooth'u desteklemez.
Web Bluetooth yalnızca güvenli bağlamda çalışır: sayfa HTTPS (ör. GitHub Pages) ya da localhost üzerinden açılmalıdır.
Çalıştırma
Depoyu indirin ya da GitHub Pages ile yayınlayın (Settings → Pages → main dalı).
Sayfayı desteklenen bir tarayıcıda açın.
Adaptörü araca takın, kontağı açın ve uygulamadaki "Aracınıza bağlanın" adımlarını izleyin.
Dosyalar
Dosya	Açıklama
index.html	HTML, CSS ve JavaScript dahil tüm uygulama
Not

Kişisel bir projedir ve araç verisini okuma ve yorumlama amacıyla geliştirilmiştir. Uygulamanın yorumları bir servis teşhisinin yerine geçmez.

— Mert Güzelmansur (MGM)
