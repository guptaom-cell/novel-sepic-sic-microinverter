/**
 * @file    spwm_control.c
 * @brief   SPWM Gate Pulse Generator for 5-Switch Novel SEPIC Microinverter
 * @target  Texas Instruments C2000 Microcontrollers (TMS320F28379D / F280049C)
 * @author  Om Gupta (IIT Gandhinagar)
 */

#include <math.h>
#include <stdint.h>

// ============================================================================
// 1. SYSTEM PARAMETERS & CONSTANTS
// ============================================================================
#define FS          50000.0f            // Switching frequency = 50 kHz
#define TS          (1.0f / FS)         // Switching period = 20 us
#define TD          300e-9f             // Dead time = 300 ns
#define TD_S1       200e-9f             // S1 delay relative to S3 = 200 ns
#define FO          50.0f               // Output fundamental frequency = 50 Hz
#define VO_RMS      200.0f              // Target RMS output voltage = 200 V
#define VDC         100.0f              // Input DC voltage = 100 V

#define TWO_PI      6.283185307179586f  // 2 * pi
#define SQRT_2      1.414213562373095f  // sqrt(2)

// ============================================================================
// 2. DATA STRUCTURES
// ============================================================================
typedef struct {
    uint8_t S1;     // Gate signal for Switch S1 (1 = ON, 0 = OFF)
    uint8_t S2;     // Gate signal for Switch S2
    uint8_t S3;     // Gate signal for Switch S3
    uint8_t S4;     // Gate signal for Switch S4
    uint8_t S5;     // Gate signal for Switch S5
    float   D;      // Calculated duty cycle
} GatePulses_t;

// ============================================================================
// 3. GATE PULSE GENERATION FUNCTION
// ============================================================================
/**
 * @brief  Computes instantaneous gate signals for 5-switch SEPIC inverter.
 * @param  t Current time step in seconds (e.g., system timer variable)
 * @return GatePulses_t Struct containing discrete states of S1 to S5 and Duty Cycle
 */
GatePulses_t calculate_gate_pulses(float t) 
{
    GatePulses_t pulses = {0, 0, 0, 0, 0, 0.0f};

    // --- Physical Reference Signal ---
    float v_ref = (VO_RMS * SQRT_2) * sinf(TWO_PI * FO * t);

    // --- Duty Cycle from Complete SEPIC Converter Gain ---
    // Modulation index: M = |Vo| / Vdc
    // SEPIC duty formula: D = [1 + 2M - sqrt(1 + 4M)] / (2M)
    float M = fabsf(v_ref) / VDC;
    float D = 0.0f;

    if (M >= 1e-12f) {
        D = (1.0f + 2.0f * M - sqrtf(1.0f + 4.0f * M)) / (2.0f * M);
    } else {
        D = 0.0f;
    }

    // Saturation limits [0.0, 0.999]
    if (D < 0.0f)   D = 0.0f;
    if (D > 0.999f) D = 0.999f;
    pulses.D = D;

    float T_on = D * TS;
    float t_sw = fmodf(t, TS);

    // --- Switch Logic Determination ---
    if (v_ref >= 1e-3f) 
    {
        // ============================================================
        // POSITIVE HALF-CYCLE (Charging: S2+S4 | Freewheel: S4+S5)
        // ============================================================
        pulses.S4 = 1;
        pulses.S1 = 0;
        pulses.S5 = 0;

        if ((t_sw > TD) && (t_sw < T_on)) {
            pulses.S2 = 1;
        } else if (t_sw > (T_on + TD)) {
            pulses.S5 = 1;
        }
    } 
    else if (v_ref <= -1e-3f) 
    {
        // ============================================================
        // NEGATIVE HALF-CYCLE (Charging: S1+S3+S5 | Freewheel: S4+S5)
        // ============================================================
        pulses.S5 = 1;
        pulses.S2 = 0;

        if ((t_sw > TD) && (t_sw < T_on)) {
            pulses.S3 = 1;
            
            // Controlled delay for S1 relative to S3
            if ((t_sw > (TD + TD_S1)) && (t_sw < T_on)) {
                pulses.S1 = 1;
            } else {
                pulses.S5 = 0;
            }
        } 
        else if (t_sw > (T_on + TD)) {
            pulses.S4 = 1;
            pulses.S1 = 0;
        }
    } 
    else 
    {
        // ============================================================
        // ZERO CROSSING (All switches OFF)
        // ============================================================
        pulses.S1 = 0;
        pulses.S2 = 0;
        pulses.S3 = 0;
        pulses.S4 = 0;
        pulses.S5 = 0;
    }

    return pulses;
}
