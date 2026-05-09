# Comprehensive Incident Report: CMOS Failure & BIOS Anomalies

## 1. System Specifications
* **Motherboard:** Galax H310M (Chipset Variant: KBL PCH-H / H310C)
* **CPU:** Intel Core i5-8500 (Integrated GPU: Intel UHD Graphics 630)
* **Dedicated GPU:** Galax Graphics Card (Output: DisplayPort / HDMI)
* [cite_start]**Intel ME Firmware:** Version 11.6.10.1196 H 
* **Primary Objective:** Restore system stability, configure BIOS, and enable TPM 2.0 / Secure Boot for Valorant (Riot Vanguard).

---

## 2. Phase 1: Hardware Failure (The Root Cause)
* **Symptom:** The system experienced boot loops, failed to retain hardware configurations, and repeatedly prompted to run Setup.
* **Diagnosis:** The CR2032 CMOS battery suffered severe chemical leakage and catastrophic voltage drop.
    * *Old Battery Voltage:* 0.16V (Completely dead, unable to sustain the RTC/SRAM chip).
    * *New Battery Voltage:* 3.25V (Optimal).
* **Resolution:** Socket cleaned, battery replaced. CMOS cleared successfully to factory defaults.

---

## 3. Phase 2: BIOS Interface & Navigation Anomalies
Following the CMOS reset, the BIOS environment exhibited severe usability issues.

### Anomaly A: Extreme UI Latency ("The 15-Second Lag")
* **Symptom:** The BIOS graphical interface was practically frozen. Mouse cursor movements took up to 15 seconds to register.
* **Root Cause:** "Interrupt Storm" caused by high-polling-rate gaming peripherals. Legacy BIOS input buffers cannot process the high frequency of signals from modern gaming mice, leading to CPU bottlenecking within the BIOS environment.
* **Workaround:** Physically disconnect the mouse prior to entering the BIOS. Rely strictly on keyboard navigation.

### Anomaly B: The Navigation Trap
* **Symptom:** The cursor was trapped inside the configuration pane (e.g., toggling Date/Time) and unable to switch between primary tabs.
* **Root Cause:** The Galax H310M utilizes a non-standard vertical sidebar layout.
* **Workaround:** * Press `Esc` or `Left Arrow (<-)` to force the cursor out of the configuration pane.
    * Use the `Down Arrow (v)` exclusively to navigate between the primary categories (Main, Advanced, Boot).

---

## 4. Phase 3: Firmware & Security Diagnostics (Intel CSME)
To rule out firmware corruption caused by the CMOS battery failure, system-level diagnostics were executed.

* [cite_start]**Diagnostic Tool:** Intel(R) MEInfoWin64.exe (Version: 11.8.92.4189).
* **Execution Environment:** Windows Command Prompt (Administrator).
* **Diagnostic Results:**
    * ME communication is fully functional. [cite_start]The PCH version is identified as 0[cite: 3].
    * [cite_start]Intel(R) Platform Trust Technology (PTT / TPM 2.0) is detected as `PRESENT/DISABLED`.
    * [cite_start]Capability Licensing Service is `Enabled`[cite: 11].
* **Conclusion:** The Intel Management Engine (ME) is structurally intact and was not corrupted by the power loss. A BIOS re-flash is not strictly necessary. The user only needs to navigate the BIOS to manually toggle PTT to `Enabled`.

---

## 5. Phase 4: The Boot Display Dilemma (Bypassing the BIOS)
Attempts to bypass the laggy BIOS by booting directly into a FAT32 USB drive (to flash the BIOS via EFI Shell) resulted in display failures.

* **Symptom 1: Backlit Black Screen:** Booting into `UEFI: USB, Partition 1` resulted in a blank screen with the monitor backlight on.
    * *Root Cause:* A known firmware bug in older dedicated GPUs over DisplayPort. The GPU fails to render low-resolution EFI Shell/DOS environments.
* **Symptom 2: Static Underscore (`_`):** The screen displayed a frozen underscore.
    * *Root Cause:* Bootloader incompatibility or the motherboard failing to execute the `startup.nsh` script.
* **Final Resolution Path:** Bypass the dedicated GPU by plugging the display cable directly into the motherboard (Intel UHD 630). However, since ME diagnostics confirm the firmware is healthy, flashing is deemed unnecessary. The recommended path is to enter BIOS (mouse disconnected) and enable TPM/Secure Boot manually.