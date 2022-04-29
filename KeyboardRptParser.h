#include <hidboot.h>
#include <usbhub.h>

class KbdRptParser : public KeyboardReportParser
{
public:
  struct KeyReport
  {
    uint8_t modifiers;
    uint8_t key;
  };

  void
  setCallback(void (*sendReport)(KeyReport *));

protected:
  void OnControlKeysChanged(uint8_t before, uint8_t after);
  void OnKeyDown(uint8_t mod, uint8_t key);
  void OnKeyUp(uint8_t mod, uint8_t key);
  //    void OnKeyPressed(uint8_t key);

private:
  void PrintKey(uint8_t mod, uint8_t key);

  bool modifyKey(uint8_t *pKey, bool bDown);
  bool modifyModifier(MODIFIERKEYS before, MODIFIERKEYS after);

  KeyReport keyReport;

  void (*callbackSendReport)(KeyReport *keyReport);
};
