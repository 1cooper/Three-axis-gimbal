/* Private includes ----------------------------------------------------------*/
#include "maintask.h"
#include "adc.h"
#include "calibration.h"
#include "canbsp.h"
#include "data.h"
#include "encoder.h"
// #include "fliter.h"
// #include "flux_observer.h"
#include "foc.h"
// #include "hfi_q.h"
#include "main.h"
#include "pid.h"
// #include "smo.h"
#include "task_cfg.h"
#include "tim.h"
// #include "usbd_cdc_if.h"
#include "user_lib.h"
#include "ws2812.h"
/* Private define ------------------------------------------------------------*/
/*switch board*/
#define board_green    1
#define board_black    0
#define board_4310     0
/*switch mode*/
#define curr_mode      0
#define cali_mode      0
#define open_mode      1
#define hfi__mode      0
#define hfi__mode_test 0  // 极性辨识测试
#define flux_mode      0
#define smo_mode       0
/* Private typedef -----------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance == ADC1)
  {
    foc.u_a = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_1);
    foc.u_b = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_2);
    foc.u_c = HAL_ADCEx_InjectedGetValue(&hadc1, ADC_INJECTED_RANK_3);
#if board_green
    foc.i_a = 3.3f / 4096.0f * (1989.3f - foc.u_a) / AMULTIPLE / RES;
    foc.i_b = 3.3f / 4096.0f * (1983.7f - foc.u_b) / AMULTIPLE / RES;
    foc.i_c = 3.3f / 4096.0f * (1979.2f - foc.u_c) / AMULTIPLE / RES;
#endif
#if board_black
    foc.i_a = 3.3f / 4096.0f * (1965 - foc.u_a) / AMULTIPLE / RES;
    foc.i_b = 3.3f / 4096.0f * (1989 - foc.u_b) / AMULTIPLE / RES;
    foc.i_c = 3.3f / 4096.0f * (1977 - foc.u_c) / AMULTIPLE / RES;
#endif
#if board_4310
    foc.i_a = 3.3f / 4096.0f * (1993 - foc.u_a) / AMULTIPLE / RES;
    foc.i_b = 3.3f / 4096.0f * (1988 - foc.u_b) / AMULTIPLE / RES;
    foc.i_c = 3.3f / 4096.0f * (1954 - foc.u_c) / AMULTIPLE / RES;
#endif

#if curr_mode
    foc.pulse = encoder_read();
    foc.theta = Encoder_Comp(&Encoder, offset_lut, foc.pulse, 306306);
    ClarkeTrans(&foc);
    SinCosCalculate(&foc);
    ParkTrans(&foc);
    Speed_PID.kp = Speed_PID.kp;
    Speed_PID.ki = Speed_PID.ki;
    Speed_PID.kd = Speed_PID.kd;
    Pos_PID.kp   = Pos_PID.kp;
    Pos_PID.ki   = Pos_PID.ki;
    Pos_PID.kd   = Pos_PID.kd;
    Curr_PI_Cal_VoltCircLimit(&Iqd_PI, Speed_PID.result, 0.0f, foc.i_q, foc.i_d);
    foc.u_d = Iqd_PI.id_output;
    foc.u_q = Iqd_PI.iq_output;
    IparkTrans(&foc);
    SVPWM(&foc);
#endif
#if cali_mode
    Calibration_Loop(&foc);
#endif
#if open_mode
    foc.pulse = encoder_read();
    //    process_numbers(foc.u_a, foc.u_b, foc.u_c, &foc.u_a_offset, &foc.u_b_offset, &foc.u_c_offset);
    OpenLoop(&foc, 0.0003f);
#endif
#if hfi__mode
    if(hfi.hfi_inj_cnt == 2)
    {
      ClarkeTrans(&foc);
      HFI_AlphaBetah_Get(&hfi, foc.i_alpha, foc.i_beta);
      HFI_Pos_err(&hfi);
      PLL_Cale_HFI(&hfi);
      foc.theta = hfi.Angle;
      SinCosCalculate(&foc);
      ParkTrans(&foc);
      HFI_dqf_Get(&hfi, foc.i_d, foc.i_q);
      Curr_PI_Cal_VoltCircLimit(&Iqd_PI, 0.5f, 0.0f, hfi.i_qf, hfi.i_df);
      foc.u_d = HFI_Injc_Volt(&hfi, Iqd_PI.id_output);
      foc.u_q = Iqd_PI.iq_output;
      IparkTrans(&foc);
      SVPWM(&foc);
      hfi.hfi_inj_cnt = 0;
    }
    hfi.hfi_inj_cnt++;
#endif
#if hfi__mode_test  // 极性辨识测试
    if(hfi.hfi_inj_cnt == 2)
    {
      ClarkeTrans(&foc);
      HFI_AlphaBetah_Get(&hfi, foc.i_alpha, foc.i_beta);
      HFI_Pos_err(&hfi);
      PLL_Cale_HFI(&hfi);
      foc.theta = hfi.Angle;
      SinCosCalculate(&foc);
      ParkTrans(&foc);
      HFI_dqf_Get(&hfi, foc.i_d, foc.i_q);
      if(hfi.hfi_init_flag == 0)
      {
        NS_Check(&hfi);
      }
      Curr_PI_Cal_VoltCircLimit(&Iqd_PI, hfi.trg_iq, hfi.trg_id, hfi.i_qf, hfi.i_df);
      foc.u_d = HFI_Injc_Volt(&hfi, Iqd_PI.id_output);
      foc.u_q = Iqd_PI.iq_output;
      IparkTrans(&foc);
      SVPWM(&foc);
      hfi.hfi_inj_cnt = 0;
    }
    hfi.hfi_inj_cnt++;
#endif
#if flux_mode
    ClarkeTrans(&foc);
    Flux_Processing(&flux, &foc);
    Flux_PLL_Processing(&flux);
    foc.theta = flux.eangle;
    SinCosCalculate(&foc);
    ParkTrans(&foc);
    Curr_PI_Cal_VoltCircLimit(&Iqd_PI, 1.0f, 0.0f, foc.i_q, foc.i_d);
    foc.u_d = Iqd_PI.id_output;
    foc.u_q = Iqd_PI.iq_output;
    IparkTrans(&foc);
    SVPWM(&foc);
#endif
#if smo_mode
    foc.pulse = encoder_read();
    foc.theta = Encoder_Comp(&Encoder, offset_lut, foc.pulse, encoder_offset);
    ClarkeTrans(&foc);
    SinCosCalculate(&foc);
    ParkTrans(&foc);
    // if(smo.cnt == 2)
    // {
    Smo_Processing(&smo, &foc);
    //   smo.cnt = 0;
    // }
    Curr_PI_Cal_VoltCircLimit(&Iqd_PI, 0.5f, 0.0f, foc.i_q, foc.i_d);
    foc.u_d = Iqd_PI.id_output;
    foc.u_q = Iqd_PI.iq_output;
    IparkTrans(&foc);
    SVPWM(&foc);
    // smo.cnt++;
#endif
  }
}
float trg_speed = 50.0f;
float trg_pos   = 500000.0f;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
  if(htim == (&htim2))
  {
    trg_pos = trg_pos;
    Speed_Cal(&speed, foc.pulse);
    PID_Cal(&Pos_PID, trg_pos, foc.pulse);
    if(Pos_PID.error < -ENCODER_CPR_F)
    {
      Pos_PID.result = -Pos_PID.result;
    }
    PID_Cal(&Speed_PID, Pos_PID.result, speed.rpm);
    WS2812_BreathEffect_Smooth(&ws2812);
    //    // PID_Cal(&Speed_PID, trg_speed, speed.rpm);
  }
}
