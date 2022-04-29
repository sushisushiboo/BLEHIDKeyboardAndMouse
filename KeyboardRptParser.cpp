#include "KeyboardRptParser.h"

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
#ifdef DEBUG_PRINT
  MODIFIERKEYS mod;
  *((uint8_t *)&mod) = m;
  Serial.print((mod.bmLeftCtrl == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI == 1) ? "G" : " ");
#endif
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  // キー変換
  if (this->modifyKey(&key, true))
  {
#ifdef DEBUG_PRINT
    Serial.print("DN ");
#endif
    keyReport.key = key;
    PrintKey(keyReport.modifiers, key);
    (*this->callbackSendReport)(&keyReport);

    //  uint8_t c = OemToAscii(mod, key);
    //  if (c)
    //    OnKeyPressed(c);
  }
}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  if (modifyKey(&key, false))
  {
#ifdef DEBUG_PRINT
    Serial.print("UP ");
#endif
    keyReport.key = 0;
    PrintKey(keyReport.modifiers, key);
    (*this->callbackSendReport)(&keyReport);
  }
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after)
{

  MODIFIERKEYS beforeMod;
  *((uint8_t *)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t *)&afterMod) = after;

  if (before != after)
  {
    if (this->modifyModifier(beforeMod, afterMod))
    {
      return;
    }
#ifdef DEBUG_PRINT
    Serial.print("Modifer changed:");
    Serial.println(after);
#endif
  }
  keyReport.modifiers = after;

#ifdef DEBUG_PRINT
  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl)
  {
    Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift)
  {
    Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt)
  {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI)
  {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl)
  {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift)
  {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt)
  {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI)
  {
    Serial.println("RightGUI changed");
  }
#endif
}

// void KbdRptParser::OnKeyPressed(uint8_t key)
//{
//   Serial.print("ASCII: ");
//   Serial.println((char)key);
//   Keyboard.write(key);
// };

/**
 * 修飾キー変換
 * @note Controlの場合はCapsLockに変更
 * @param [in] MODIFIERKEYS before: 前回の修飾キー状態
 * @param [in] MODIFIERKEYS after: 今回の修飾キー状態
 * @return bool : 修飾キー変換したかどうか
 */
bool KbdRptParser::modifyModifier(MODIFIERKEYS before, MODIFIERKEYS after)
{
  // Control->CAPSLOCK(0x39)
  if (before.bmLeftCtrl != after.bmLeftCtrl)
  {
    if (after.bmLeftCtrl)
    {
#ifdef DEBUG_PRINT
      Serial.println("CAPSLOCK DOWN");
#endif
      keyReport.key = 0x39;
    }
    else
    {
#ifdef DEBUG_PRINT
      Serial.println("CAPSLOCK UP");
#endif
      keyReport.key = 0;
    }
    (*this->callbackSendReport)(&keyReport);
    return true;
  }

  return false;
}

/**
 * キー変換
 * @param [in/out] uint8_t* pKey: キーコード
 * @param [in] bool bDown: 押下
 * @return bool : true キー変換実施, false modifierとして処理
 */
bool KbdRptParser::modifyKey(uint8_t *pKey, bool bDown)
{
  switch (*pKey)
  {
  case 0x29: // ESC(0x29)->半角/全角(0x35)
    *pKey = 0x35;
    break;
  case 0x35: // 半角/全角(0x35)->ESC(0x29)
    *pKey = 0x29;
    break;
  case 0x8A: // 変換(0x8A)->半角/全角(0x35)
    *pKey = 0x35;
    break;
  case 0x8B: // 無変換(0x8B)->半角/全角(0x35)
    *pKey = 0x35;
    break;
  case 0x39: // CAPSLOCK(0x39)->Control
  {
    KBDINFO *pKbdInfo;
    pKbdInfo = (KBDINFO *)&keyReport;
    if (bDown)
    {
      pKbdInfo->bmLeftCtrl = 1;
#ifdef DEBUG_PRINT
      Serial.println("LeftCtrl down");
#endif
    }
    else
    {
      pKbdInfo->bmLeftCtrl = 0;
#ifdef DEBUG_PRINT
      Serial.println("LeftCtrl up");
#endif
    }
    return false;
  }
  default:
    break;
  }

#ifdef DEBUG_PRINT
  Serial.print("changed to ");
  Serial.println(*pKey, HEX);
#endif

  return true;
}

void KbdRptParser::setCallback(void (*sendReport)(KeyReport *))
{
  this->callbackSendReport = sendReport;
}