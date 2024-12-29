#include "common.h"
#include "port.h"

//外部関数宣言
VD fn_gpio_init    (VD);
VD fn_gpio_on   (U1 u1_t_pin_id);
VD fn_gpio_off  (U1 u1_t_pin_id);

//外部関数定義
VD fn_gpio_init    (VD){
    pinMode(U1_PORT_NUM_AZUKI_SWITCH,OUTPUT);
    return;
}

VD fn_gpio_on   (U1 u1_t_pin_id){
    digitalWrite(u1_t_pin_id, HIGH);
#ifdef _DEBUG
    Serial.print("DEBUG\tgpio on\t");
    Serial.println(u1_t_pin_id);
#endif //_DEBUG
    return;
}

VD fn_gpio_off  (U1 u1_t_pin_id){
    digitalWrite(u1_t_pin_id, LOW);
#ifdef _DEBUG
    Serial.print("DEBUG\tgpio off\t");
    Serial.println(u1_t_pin_id);
#endif //_DEBUG
    return;
}
