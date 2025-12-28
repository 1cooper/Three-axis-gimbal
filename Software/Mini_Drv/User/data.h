#ifndef __DATA_H__
#define __DATA_H__

/* Private includes ----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
// math
#define sqrt3           1.732050807568877293527f
#define third_sqrt3     0.5773502691896257f
#define DoubleTripSqrt3 1.1547005f
#define T_s             0.00005f
#define ARR             4250.0f
#define _2_PI           6.283185307179586476925f
// 采样参数
#define Udc             24.0f  // 母线电压
#define AMULTIPLE       20.0f
#define RES             0.005f
// 电机参数
#if 0
// 黑色A08
#define MOTOR_POLE  14
#define MOTOR_L     0.000025085f    // 相电感
#define MOTOR_R     0.08188123f     // 相电阻
#define MOTOR_Rs    1.5f * MOTOR_R  // 定子电阻Rs = (3/2)R
#define MOTOR_Ls    1.5f * MOTOR_L  // 定子电阻Ls = (3/2)L
#define MOTOR_Phi_m 0.00321064f     // 电机磁链
#define MOTOR_Ld    0.00002064f     // D轴电感
#define MOTOR_Lq    0.00002953f     // Q轴电感
// 黑色A08
#endif

#if 1
// 黑色4310
#define MOTOR_POLE  14
#define MOTOR_L     0.0000476f      // 相电感
#define MOTOR_R     0.0001032f        // 相电阻
#define MOTOR_Rs    1.5f * MOTOR_R  // 定子电阻Rs = (3/2)R
#define MOTOR_Ls    1.5f * MOTOR_L  // 定子电阻Ls = (3/2)L
#define MOTOR_Phi_m 0.00171254f     // 电机磁链
#define MOTOR_Ld    0.00224274f     // D轴电感
#define MOTOR_Lq    0.00196518f     // Q轴电感
// 黑色4310
#endif

#if 0
// 黑色涵道
#define MOTOR_POLE  7
#define MOTOR_L     0.00000124f     // 相电感
#define MOTOR_R     0.0225f         // 相电阻
#define MOTOR_Rs    1.5f * MOTOR_R  // 定子电阻Rs = (3/2)R
#define MOTOR_Ls    1.5f * MOTOR_L  // 定子电阻Ls = (3/2)L
#define MOTOR_Phi_m 0.00068f        // 电机磁链
#define MOTOR_Ld    0.00000124f     // D轴电感
#define MOTOR_Lq    0.00000124f     // Q轴电感
// 黑色A08
#endif

#if 0  // tmotor u12
#define MOTOR_POLE  20
#define MOTOR_L     0.00000657f
#define MOTOR_R     0.0242f
#define MOTOR_Rs    1.5f * MOTOR_R
#define MOTOR_Ls    1.5f * MOTOR_L
#define MOTOR_Phi_m 0.00068f  // 电机磁链

#endif

#if 0  // 黑色3505
#define MOTOR_POLE  10
#define MOTOR_L     0.00001007f
#define MOTOR_R     0.0659f
#define MOTOR_Rs    1.5f * MOTOR_R
#define MOTOR_Ls    1.5f * MOTOR_L
#define MOTOR_Phi_m 0.00068f  // 电机磁链
#define MOTOR_Ld    0.00001f  // D轴电感

#endif

#if 0  // T-motor MN805 kv120
#define MOTOR_POLE 14
#define MOTOR_L    0.00001988f
#define MOTOR_R    0.078095f
#define MOTOR_Rs   1.5f * MOTOR_R
#define MOTOR_Ls   1.5f * MOTOR_L
#endif

#if 0  // 大疆精灵

#define MOTOR_POLE  7
#define MOTOR_L     0.000125f
#define MOTOR_R     0.12537f
#define MOTOR_Rs    1.5f * MOTOR_R  // 定子电阻Rs = (3/2)R
#define MOTOR_Ls    1.5f * MOTOR_L  // 定子电阻Ls = (3/2)L
#define MOTOR_Phi_m 0.000818f       // 电机磁链
#define MOTOR_Ld    0.000021f       // D轴电感
#define MOTOR_Lq    0.000029f       // Q轴电感

#endif

#if 0  // tmotor 505s

#define MOTOR_POLE  14
#define MOTOR_L     0.000135f
#define MOTOR_R     0.10077f
#define MOTOR_Rs    1.5f * MOTOR_R  // 定子电阻Rs = (3/2)R
#define MOTOR_Ls    1.5f * MOTOR_L  // 定子电阻Ls = (3/2)L
#define MOTOR_Phi_m 0.000818f       // 电机磁链
#define MOTOR_Ld    0.000021f       // D轴电感
#define MOTOR_Lq    0.000029f       // Q轴电感

#endif
/* Private typedef -----------------------------------------------------------*/
/* Private extern variables --------------------------------------------------*/
/* Private function declaration ----------------------------------------------*/
#endif
