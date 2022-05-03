#include "MouseRptParser.h"
#include "KeyboardRptParser.h"

#include <bluefruit.h>

BLEDis bledis;
BLEHidAdafruit blehid;

#include <SPI.h>

// #define DEBUG_PRINT

/**
 * @brief キーボード入力通知
 * @param report キー状態
 */
void sendReport(KbdRptParser::KeyReport *report)
{
#ifdef DEBUG_PRINT
    Serial.print("== ");
    Serial.print(report->modifiers, HEX);
    Serial.print(":");
    Serial.print(report->key, HEX);
    Serial.println(" ==");
#endif
    uint8_t keycode[6] = {0};
    keycode[0] = report->key;
    blehid.keyboardReport(report->modifiers, keycode);
}

USB Usb;
USBHub Hub(&Usb);

HIDBoot<USB_HID_PROTOCOL_KEYBOARD | USB_HID_PROTOCOL_MOUSE> HidComposite(&Usb, true);

KbdRptParser KbdPrs;
MouseRptParser MousePrs;

/**
 * @brief マウス移動の通知
 * @param dx x移動量
 * @param dy y移動量
 */
void mouseMove(int8_t dx, int8_t dy)
{
    blehid.mouseMove(dx, dy);
}

/**
 * @brief スクロールの通知
 * @param dh H移動量
 */
void mouseScroll(int8_t dh)
{
    blehid.mouseScroll(dh);
}

/**
 * @brief マウス左ボタンの通知
 * @param press 押下
 */
void mouseButtonLeft(bool press)
{
    if (press)
    {
        blehid.mouseButtonPress(MOUSE_BUTTON_LEFT);
    }
    else
    {
        blehid.mouseButtonRelease(MOUSE_BUTTON_LEFT);
    }
}

/**
 * @brief マウス左ボタンの通知
 * @param press 押下
 */
void mouseButtonRight(bool press)
{
    if (press)
    {
        blehid.mouseButtonPress(MOUSE_BUTTON_RIGHT);
    }
    else
    {
        blehid.mouseButtonRelease(MOUSE_BUTTON_RIGHT);
    }
}

/**
 * @brief マウス中央ボタンの通知
 * @param press 押下
 */
void mouseButtonMiddle(bool press)
{
    if (press)
    {
        blehid.mouseButtonPress(MOUSE_BUTTON_MIDDLE);
    }
    else
    {
        blehid.mouseButtonRelease(MOUSE_BUTTON_MIDDLE);
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Start");

    if (Usb.Init() == -1)
        Serial.println("OSC did not start.");

    delay(200);

    // USBデバイスのイベント処理の設定
#if 1
    HidComposite.SetReportParser(0, &KbdPrs);
    HidComposite.SetReportParser(1, &MousePrs);
#else
    HidUniversal.SetReportParser(0, &UnivPrs);
#endif

    // イベント処理コールバックの設定
    MousePrs.setCallback(mouseMove, mouseScroll, mouseButtonRight, mouseButtonLeft, mouseButtonMiddle);
    KbdPrs.setCallback(sendReport);

    // BLEの初期設定

    Bluefruit.begin();
    // HID Device can have a min connection interval of 9*1.25 = 11.25 ms
    Bluefruit.Periph.setConnInterval(9, 16); // min = 9*1.25=11.25 ms, max = 16*1.25=20ms
    Bluefruit.setTxPower(4);                 // Check bluefruit.h for supported values

    // Configure and Start Device Information Service
    bledis.setManufacturer("Adafruit Industries");
    bledis.setModel("Bluefruit Feather 52");
    bledis.begin();

    // BLE HID
    blehid.begin();

    // Set up and start advertising
    startAdv();
}

void startAdv(void)
{
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_MOUSE);

    // Include BLE HID service
    Bluefruit.Advertising.addService(blehid);

    // There is enough room for 'Name' in the advertising packet
    Bluefruit.Advertising.addName();

    /* Start Advertising
     * - Enable auto advertising if disconnected
     * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     * - Timeout for fast mode is 30 seconds
     * - Start(timeout) with timeout = 0 will advertise forever (until connected)
     *
     * For recommended advertising interval
     * https://developer.apple.com/library/content/qa/qa1931/_index.html
     */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
    Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}

void loop()
{
    Usb.Task();
}
