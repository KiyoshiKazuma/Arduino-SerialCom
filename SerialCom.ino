#include <MsTimer2.h>
#include "common.h"

//外部関数参照
extern VD fn_serial_init  (VD);
extern VD fn_control_init (VD);
extern VD fn_serial_main  (VD);
extern VD fn_control_main (VD);

//内部関数宣言
VD fn_task_init(VD);
VD fn_task_main(VD);

VD setup() {
  fn_control_init();
  fn_serial_init();
  fn_task_init();

#ifdef _DEBUG
  Serial.print("DEBUG\tinitial task executed\n");
#endif //_DEBUG
  return;
}

VD loop() {
}

VD fn_task_init(VD){
  MsTimer2::set(1, fn_task_main);
  MsTimer2::start();
  return;
}

VD fn_task_main(VD){
  fn_serial_main();
  fn_control_main();

#ifdef _DEBUG
  static U2 u2_s_timer = 0U;
  u2_s_timer++;
  if(u2_s_timer == 1000U){
    Serial.print("DEBUG\tperiodic task executed\n");
    u2_s_timer=0;
  }
#endif //_DEBUG
  return;
}

