#include "MouseRptParser.h"

// #define DEBUG_PRINT

void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
#ifdef DEBUG_PRINT
    Serial.print("dx=");
    Serial.print(mi->dX, DEC);
    Serial.print(" dy=");
    Serial.println(mi->dY, DEC);
#else
    (*this->callbackMove)(mi->dX, mi->dY);
#endif
};
void MouseRptParser::OnLeftButtonUp(MOUSEINFO *mi)
{
#ifdef DEBUG_PRINT
    Serial.println("L Butt Up");
#else
    (*this->callbackButtonLeft)(false);
#endif
};
void MouseRptParser::OnLeftButtonDown(MOUSEINFO *mi)
{
#ifdef DEBUG_PRINT
    Serial.println("L Butt Dn");
#else
    (*this->callbackButtonLeft)(true);
#endif
};
void MouseRptParser::OnRightButtonUp(MOUSEINFO *mi)
{
#ifdef DEBUG_PRINT
    Serial.println("R Butt Up");
#else
    (*this->callbackButtonRight)(false);
#endif
};
void MouseRptParser::OnRightButtonDown(MOUSEINFO *mi)
{
#ifdef DEBUG_PRINT
    Serial.println("R Butt Dn");
#else
    (*this->callbackButtonRight)(true);
#endif
};
void MouseRptParser::OnMiddleButtonUp(MOUSEINFO *mi)
{
#ifdef DEBUG_PRINT
    Serial.println("M Butt Up");
#else
    (*this->callbackButtonMiddle)(false);
#endif
};
void MouseRptParser::OnMiddleButtonDown(MOUSEINFO *mi)
{
#ifdef DEBUG_PRINT
    Serial.println("M Butt Dn");
#else
    (*this->callbackButtonMiddle)(true);
#endif
};

void MouseRptParser::setCallback(void (*move)(int8_t, int8_t),
                                 void (*buttonRight)(bool),
                                 void (*buttonLeft)(bool),
                                 void (*buttonMiddle)(bool))
{
    this->callbackMove = move;
    this->callbackButtonRight = buttonRight;
    this->callbackButtonLeft = buttonLeft;
    this->callbackButtonMiddle = buttonMiddle;
}
