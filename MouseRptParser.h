#include <hidboot.h>
#include <usbhub.h>

class MouseRptParser : public MouseReportParser
{
protected:
    void OnMouseMove(MOUSEINFO *mi);
    void OnScroll(MOUSEINFO *mi);
    void OnLeftButtonUp(MOUSEINFO *mi);
    void OnLeftButtonDown(MOUSEINFO *mi);
    void OnRightButtonUp(MOUSEINFO *mi);
    void OnRightButtonDown(MOUSEINFO *mi);
    void OnMiddleButtonUp(MOUSEINFO *mi);
    void OnMiddleButtonDown(MOUSEINFO *mi);

public:
    void setCallback(void (*move)(int8_t, int8_t),
                     void (*scroll)(int8_t),
                     void (*buttonRight)(bool),
                     void (*buttonLeft)(bool),
                     void (*buttonMiddle)(bool));

private:
    void (*callbackMove)(int8_t dx, int8_t dy);
    void (*callbackScroll)(int8_t dh);
    void (*callbackButtonRight)(bool press);
    void (*callbackButtonLeft)(bool press);
    void (*callbackButtonMiddle)(bool press);
};
