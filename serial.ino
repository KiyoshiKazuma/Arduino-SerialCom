#include "common.h"

//外部関数参照
extern VD fn_control_data_push(U1 u1_t_data);


//外部関数宣言
VD fn_serial_init  (VD);
VD fn_serial_main  (VD);

//内部関数宣言
VD fn_serial_read(VD);


//外部関数定義
VD fn_serial_init(VD){
  Serial.begin(9600);
  return;
}

VD fn_serial_main(VD){
    fn_serial_read();
    return;
}

//内部関数定義
VD fn_serial_read(VD){
    int incomingbyte = 0;
    U1 u1_t_receive_data = 0;
    if(Serial.available() > 0){
        incomingbyte = Serial.read();
        u1_t_receive_data = (U1)incomingbyte;
        fn_control_data_push(u1_t_receive_data);

#ifdef _DEBUG
    Serial.print("DEBUG\tserial received\t");
    Serial.println(incomingbyte);
#endif //_DEBUG
    }
    return;
}