# novel-sepic-sic-microinverter
Design, simulation, and control of a 500W novel SEPIC-based SiC microinverter using SPWM.
# 500W Single-Stage Novel SEPIC-Based SiC Microinverter

Design, simulation, and control of a high-efficiency 500W, 50kHz single-stage SEPIC microinverter ($45\text{ V}_{mp}$ to $230\text{ VAC}$) using SPWM control and Silicon Carbide (SiC) MOSFETs.

---

# 500W Single-Stage Novel SEPIC-Based SiC Microinverter

Design, simulation, and control of a high-efficiency 500W, 50kHz single-stage SEPIC microinverter (45 Vmp to 230V AC) using SPWM control and Silicon Carbide (SiC) MOSFETs.

---

## Technical Highlights
* **Common Ground Structure:** Engineered to eliminate high-frequency common-mode leakage currents between PV modules and grid output without bulky isolation transformers.
* **DC-Link Decoupling:** Eliminates short-lived electrolytic DC-link capacitors, extending overall inverter operational lifespan and reducing conduction losses.
* **Isolated SiC Gate Drivers:** Designed custom gate drivers with Active Miller Clamping and bootstrap supplies to withstand high dv/dt switching stress.
* **Control Architecture:** Implemented closed-loop PR/PI controllers with PLL and SPWM on a TI C2000 DSP (F28379D) using ePWM and ADC modules.
* **Analog Sensing & EMI Filter:** Differential op-amp voltage/current sensing with anti-aliasing filters and integrated conducted EMI filter stages (CM/DM chokes).

---

## Tools & Technologies Used
* **Simulation & Modeling:** PSIM, PSPICE, LTspice, MATLAB / Simulink
* **Hardware & PCB Layout:** KiCad, High-Frequency Magnetics Sizing (L1, L2, L3)
* **Embedded Control:** TI C2000 MCU (F28379D), C, SPWM / SVPWM
* **Validation Equipment:** High-Bandwidth DSO, Power Analyzers, Grid Simulators

---

## Project Structure
* `/simulation` — PSIM / LTspice circuit simulation models
* `/firmware` — C code for TI C2000 DSP (SPWM, PR/PI, ADC/ePWM)
* `/hardware` — KiCad schematic PDFs and PCB design files
* `/docs` — Detailed project report, waveforms, and calculations

---

## Author
**Om Gupta**  
M.Tech in Power Engineering & Control Systems, IIT Gandhinagar  
* **Email:** gupta.om@iitgn.ac.in  
* **Phone:** +91 8238173514

