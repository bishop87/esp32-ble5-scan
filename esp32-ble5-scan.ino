/*
   BLE5 extended scan example for esp32 C3 and S3
   with this code it is simple to scan legacy (BLE4) compatible advertising,
   and BLE5 extended advertising. New coded added in BLEScan is not changing old behavior,
   which can be used with old esp32, but is adding functionality to use on C3/S3.
   With this new API advertised device wont be stored in API, it is now user responsibility

   author: chegewara
*/
#ifndef CONFIG_BT_BLE_50_FEATURES_SUPPORTED
#warning "Not compatible hardware"
#else
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

uint32_t scanTime = 100; //In 10ms (1000ms)
BLEScan* pBLEScan;

class MyBLEExtAdvertisingCallbacks: public BLEExtAdvertisingCallbacks {
    void onResult(esp_ble_gap_ext_adv_reprot_t report) {
      if(report.event_type & ESP_BLE_GAP_SET_EXT_ADV_PROP_LEGACY){
        // here we can receive regular advertising data from BLE4.x devices
        Serial.println("BLE4.2");
            
        // Esempio: Stampa dell'indirizzo del dispositivo
        Serial.print("extend advertising address: ");
        for (int i = 0; i < 6; i++) {
            Serial.printf("%02X ", report.addr[i]);
        }
        Serial.print("Device Address: ");
        for (int i = 0; i < 6; i++) {
            Serial.printf("%02X ", report.dir_addr[i]);
        }
        Serial.printf("sid: %d\n", report.sid);
        Serial.printf("tx_power: %d", report.tx_power);
        Serial.println();
        // Esempio: Stampa del RSSI
        Serial.printf("RSSI: %d dBm\n", report.rssi);
        Serial.printf("adv_data_len: %d", report.adv_data_len);
        for (int i = 0; i < report.adv_data_len; i++) {
            Serial.printf("%d ", report.adv_data[i]);
        }
        
      } else {
        // here we will get extended advertising data that are advertised over data channel by BLE5 divices
        //Serial.printf("Ext advertise: data_le: %d, data_status: %d \n", report.adv_data_len, report.data_status);

                // Dispositivo BLE 5 con dati di advertising estesi
        Serial.printf("Ext advertise: data_len: %d, data_status: %d, event_type: 0x%x\n", report.adv_data_len, report.data_status, report.event_type);

        // Esempio: Stampa dell'indirizzo del dispositivo
        Serial.print("Device Address: ");
        for (int i = 0; i < 6; i++) {
            Serial.printf("%02X ", report.dir_addr[i]);
        }
        Serial.println();
        
        // Esempio: Stampa del nome del dispositivo
        if (report.adv_data_len > 0) {
            Serial.print("Device Name: ");
            for (int i = 0; i < report.adv_data_len; i++) {
                Serial.printf("%c", report.adv_data[i]);
            }
            Serial.println();
        }
      }
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan(); //create new scan
  pBLEScan->setExtendedScanCallback(new MyBLEExtAdvertisingCallbacks());
  pBLEScan->setExtScanParams(); // use with pre-defined/default values, overloaded function allows to pass parameters
  delay(1000); // it is just for simplicity this example, to let ble stack to set extended scan params
  pBLEScan->startExtScan(scanTime, 3); // scan duration in n * 10ms, period - repeat after n seconds (period >= duration)
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(2000);
}
#endif // CONFIG_BT_BLE_50_FEATURES_SUPPORTED
