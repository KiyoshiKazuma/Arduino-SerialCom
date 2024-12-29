#include "common.h"
#include "port.h"

#define U4_ON_TIME (100U)

#define U1_TARGET_MASK  (0b11110000)
#define U1_CONTROL_MASK (0b00001111)

#define U1_TARGET_NONE          (0b00000000)
#define U1_TARGET_AZUKI         (0b00010000)
#define U1_TARGET_CASHEW         (0b00100000)
#define U1_ORDER_NONE           (0b00000000)
#define U1_ORDER_PUSH_SWITCH    (0b00000001)

#define U1_CONTROL_STATE_NO_REQ    (0U)     //要求なし状態
#define U1_CONTROL_STATE_REQUESTED (1U)     //要求あり状態
#define U1_CONTROL_STATE_RUNNING   (2U)     //実行中状態

//外部関数参照
extern VD fn_gpio_init    (VD);
extern VD fn_gpio_on   (U1 u1_t_pin_id);
extern VD fn_gpio_off  (U1 u1_t_pin_id);

//外部関数宣言
VD fn_control_init(VD);
VD fn_control_main(VD);
VD fn_control_data_push(U1 u1_t_data);

//内部関数宣言
VD fn_control_data_analize(VD);
VD fn_control_execute(VD);
VD fn_control_push_switch(U1 u1_t_pin);

//内部変数定義
static U1 u1_s_data_buffer;
static U1 u1_s_control_stm;
static U1 u1_s_control_target;
static U1 u1_s_control_order;
static U4 u4_s_control_timer;

//外部関数定義
VD fn_control_init(VD){
    u1_s_control_stm    = U1_CONTROL_STATE_NO_REQ;
    u1_s_control_target = U1_TARGET_NONE;
    u1_s_control_order  = U1_ORDER_NONE;
    u1_s_data_buffer = 0U;
    u4_s_control_timer = 0U;

    fn_gpio_init();
    return;
}

VD fn_control_main(VD){
    static U1 u1_s_control_queue_cur = 0U;

    if(u1_s_control_stm == U1_CONTROL_STATE_REQUESTED){
        fn_control_data_analize();
    }

    if(u1_s_control_stm == U1_CONTROL_STATE_RUNNING){
        fn_control_execute();
    }

    return;
}

VD fn_control_data_push(U1 u1_t_data){
    if(u1_s_control_stm == U1_CONTROL_STATE_NO_REQ){   //要求無し状態の場合
        u1_s_data_buffer = u1_t_data;
        u1_s_control_stm = U1_CONTROL_STATE_REQUESTED;
    }

#ifdef _DEBUG
    Serial.print("DEBUG\tcontrol pushed\t");
    Serial.println(u1_s_data_buffer);
#endif //_DEBUG
    return;
}


//内部関数宣言
VD fn_control_data_analize(VD){
    switch (u1_s_data_buffer & U1_TARGET_MASK){
        case U1_TARGET_AZUKI:                             //AZUKIが操作対象の場合
            switch (u1_s_data_buffer & U1_CONTROL_MASK){
                case U1_ORDER_PUSH_SWITCH:                 //電源スイッチONが命令された場合
                    u1_s_control_stm    = U1_CONTROL_STATE_RUNNING;
                    u4_s_control_timer  = 0U;
                    u1_s_control_target = U1_TARGET_AZUKI;
                    u1_s_control_order  = U1_ORDER_PUSH_SWITCH;
                    break;
                default:
                    u1_s_control_stm = U1_CONTROL_STATE_NO_REQ;                    
                    break;
            }
            break;
        case U1_TARGET_CASHEW:                             //AZUKIが操作対象の場合
            switch (u1_s_data_buffer & U1_CONTROL_MASK){
                case U1_ORDER_PUSH_SWITCH:                 //電源スイッチONが命令された場合
                    u1_s_control_stm    = U1_CONTROL_STATE_RUNNING;
                    u4_s_control_timer  = 0U;
                    u1_s_control_target = U1_TARGET_CASHEW;
                    u1_s_control_order  = U1_ORDER_PUSH_SWITCH;
                    break;
                default:
                    u1_s_control_stm = U1_CONTROL_STATE_NO_REQ;                    
                    break;
            }
            break;
        default:
            u1_s_control_stm = U1_CONTROL_STATE_NO_REQ;
            break;
    }
    u1_s_data_buffer = 0U;
    return;
}


VD fn_control_execute(VD){
    switch (u1_s_control_order){
    case U1_ORDER_PUSH_SWITCH:
        switch (u1_s_control_target){
            case U1_TARGET_AZUKI:
                fn_control_push_switch(U1_PORT_NUM_AZUKI_SWITCH);
                break;
            case U1_TARGET_CASHEW:
                fn_control_push_switch(U1_PORT_NUM_CASHEW_SWITCH);
                break;
        }   
    default:
        break;
    }
    return;
}


VD fn_control_push_switch(U1 u1_t_pin){
    if(u4_s_control_timer == 0U){
         fn_gpio_on(u1_t_pin);
    }

    if(u4_s_control_timer >= U4_ON_TIME){
        fn_gpio_off(u1_t_pin);
        u1_s_control_stm = U1_CONTROL_STATE_NO_REQ;
    }
    u4_s_control_timer++;
    return;
}