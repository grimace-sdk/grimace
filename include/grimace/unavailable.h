#if !defined(GRIMACE_CE)
#error "include windows.h first"
#endif

#if GRIMACE_WCE == 300 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/arm-ce3.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_EMBEDDED)
#include <grimace/unavailable/arm-ce3-embedded.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_HPC2000)
#include <grimace/unavailable/arm-ce3-hpc2000.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_POCKETPC)
#include <grimace/unavailable/arm-ce3-pocketpc.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/arm-ce52.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_POCKETPC)
#include <grimace/unavailable/arm-ce52-pocketpc.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_SMARTPHONE)
#include <grimace/unavailable/arm-ce52-smartphone.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/arm-ce6.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_BRAIN)
#include <grimace/unavailable/arm-ce6-brain.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_EMBEDDED)
#include <grimace/unavailable/arm-ce6-embedded.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/mips-ce1.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_HPC)
#include <grimace/unavailable/mips-ce1-hpc.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/mips-ce2.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_HPC)
#include <grimace/unavailable/mips-ce2-hpc.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/mips-ce3.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_HPC2000)
#include <grimace/unavailable/mips-ce3-hpc2000.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_POCKETPC)
#include <grimace/unavailable/mips-ce3-pocketpc.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/mips-ce6.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_MIPS && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_EMBEDDED)
#include <grimace/unavailable/mips-ce6-embedded.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/sh3-ce1.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_HPC)
#include <grimace/unavailable/sh3-ce1-hpc.h>
#endif
#if GRIMACE_WCE == 101 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/sh3-ce101.h>
#endif
#if GRIMACE_WCE == 101 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_HPC)
#include <grimace/unavailable/sh3-ce101-hpc.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/sh3-ce2.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_HPC)
#include <grimace/unavailable/sh3-ce2-hpc.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && !defined(GRIMACE_PLATFORM)
#include <grimace/unavailable/sh3-ce3.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_SH3 && !defined(GRIMACE_DEVICE) && defined(GRIMACE_PLATFORM_POCKETPC)
#include <grimace/unavailable/sh3-ce3-pocketpc.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_X86 && !defined(GRIMACE_DEVICE)
#include <grimace/unavailable/x86-ce2.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_ASUS_A6X6__WWE_LITE_WM6_1)
#include <grimace/unavailable/arm-ce52-asus_a6x6__wwe_lite_wm6_1.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_ASUS_A6X6_WM6)
#include <grimace/unavailable/arm-ce52-asus_a6x6_wm6.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_PWA7300)
#include <grimace/unavailable/arm-ce6-pwa7300.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_PWG5300)
#include <grimace/unavailable/arm-ce6-pwg5300.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_PWSH2)
#include <grimace/unavailable/arm-ce6-pwsh2.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_PWSB1)
#include <grimace/unavailable/arm-ce6-pwsb1.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_PWSH4)
#include <grimace/unavailable/arm-ce6-pwsh4.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_CASIO_CASSIOPEIA_A11_CE1)
#include <grimace/unavailable/sh3-ce1-casio_cassiopeia_a11_ce1.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_CASIO_CASSIOPEIA_A11_CE2)
#include <grimace/unavailable/sh3-ce2-casio_cassiopeia_a11_ce2.h>
#endif
#if GRIMACE_WCE == 101 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_CASIO_CASSIOPEIA_A51_CE1)
#include <grimace/unavailable/sh3-ce101-casio_cassiopeia_a51_ce1.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_CASIO_CASSIOPEIA_E700)
#include <grimace/unavailable/mips-ce3-casio_cassiopeia_e700.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_CASIO_CASSIOPEIA_EM500_PPC2000)
#include <grimace/unavailable/mips-ce3-casio_cassiopeia_em500_ppc2000.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_CASIO_CASSIOPEIA_PA2400U_CE2)
#include <grimace/unavailable/sh3-ce2-casio_cassiopeia_pa2400u_ce2.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_CERFOS)
#include <grimace/unavailable/arm-ce6-cerfos.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_COMPAQ_C2010C_CE2_NO)
#include <grimace/unavailable/mips-ce2-compaq_c2010c_ce2_no.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_CE6)
#include <grimace/unavailable/arm-ce6-devemu_ce6.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_CE6_XP)
#include <grimace/unavailable/arm-ce6-devemu_ce6_xp.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_CE6_ALT_CONTROLS)
#include <grimace/unavailable/arm-ce6-devemu_ce6_alt_controls.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_CE6_ALT_CONTROLS_XP)
#include <grimace/unavailable/arm-ce6-devemu_ce6_alt_controls_xp.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_CE6_QVGA)
#include <grimace/unavailable/arm-ce6-devemu_ce6_qvga.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM6)
#include <grimace/unavailable/arm-ce52-devemu_wm6.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM602)
#include <grimace/unavailable/arm-ce52-devemu_wm602.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM602SP)
#include <grimace/unavailable/arm-ce52-devemu_wm602sp.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM61)
#include <grimace/unavailable/arm-ce52-devemu_wm61.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM614)
#include <grimace/unavailable/arm-ce52-devemu_wm614.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM614SP)
#include <grimace/unavailable/arm-ce52-devemu_wm614sp.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM61SP)
#include <grimace/unavailable/arm-ce52-devemu_wm61sp.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM65)
#include <grimace/unavailable/arm-ce52-devemu_wm65.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM653)
#include <grimace/unavailable/arm-ce52-devemu_wm653.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM653_WQVGA)
#include <grimace/unavailable/arm-ce52-devemu_wm653_wqvga.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM653_VGA)
#include <grimace/unavailable/arm-ce52-devemu_wm653_vga.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM653_WVGA)
#include <grimace/unavailable/arm-ce52-devemu_wm653_wvga.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM653SP)
#include <grimace/unavailable/arm-ce52-devemu_wm653sp.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM65_VGA)
#include <grimace/unavailable/arm-ce52-devemu_wm65_vga.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM65SP)
#include <grimace/unavailable/arm-ce52-devemu_wm65sp.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM6_VGA)
#include <grimace/unavailable/arm-ce52-devemu_wm6_vga.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM6SP)
#include <grimace/unavailable/arm-ce52-devemu_wm6sp.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_DEVEMU_WM6SP_QVGA)
#include <grimace/unavailable/arm-ce52-devemu_wm6sp_qvga.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_HITACHI_HPW_200EC_CE2)
#include <grimace/unavailable/sh3-ce2-hitachi_hpw_200ec_ce2.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_HP_300LX_CE1)
#include <grimace/unavailable/sh3-ce1-hp_300lx_ce1.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_HP_320LX_CE2)
#include <grimace/unavailable/sh3-ce2-hp_320lx_ce2.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_HP_360LX_CE2)
#include <grimace/unavailable/sh3-ce2-hp_360lx_ce2.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_HP_620LX_CE2)
#include <grimace/unavailable/sh3-ce2-hp_620lx_ce2.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3100_PPC2000)
#include <grimace/unavailable/arm-ce3-ipaq_h3100_ppc2000.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3100_PPC2002)
#include <grimace/unavailable/arm-ce3-ipaq_h3100_ppc2002.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9456_DE)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9456_de.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9456_ES)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9456_es.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9456_FR)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9456_fr.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9456_IT)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9456_it.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9558_JA)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9558_ja.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9567_ZH_HANS)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9567_zh_hans.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9567_ZH_HANT)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9567_zh_hant.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9616_DE)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9616_de.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9616_ES)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9616_es.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9616_FR)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9616_fr.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2000_9616_IT)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2000_9616_it.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_IPAQ_H3600_PPC2002)
#include <grimace/unavailable/arm-ce3-ipaq_h3600_ppc2002.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_JORNADA525_PPC2000)
#include <grimace/unavailable/sh3-ce3-jornada525_ppc2000.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_JORNADA540_PPC2000_9357_DE)
#include <grimace/unavailable/sh3-ce3-jornada540_ppc2000_9357_de.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_SH3 && defined(GRIMACE_DEVICE_JORNADA548_PPC2000)
#include <grimace/unavailable/sh3-ce3-jornada548_ppc2000.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_JORNADA567_PPC2002)
#include <grimace/unavailable/arm-ce3-jornada567_ppc2002.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_JORNADA720)
#include <grimace/unavailable/arm-ce3-jornada720.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_X86 && defined(GRIMACE_DEVICE_JOYRIDE)
#include <grimace/unavailable/x86-ce2-joyride.h>
#endif
#if GRIMACE_WCE == 502 && GRIMACE_ARM && defined(GRIMACE_DEVICE_JUNOSB)
#include <grimace/unavailable/arm-ce52-junosb.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_NEC_MOBILEPRO_450_CE1)
#include <grimace/unavailable/mips-ce1-nec_mobilepro_450_ce1.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_NEC_MOBILEPRO_700_CE2)
#include <grimace/unavailable/mips-ce2-nec_mobilepro_700_ce2.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_NEC_MOBILEPRO_790_HPC2000)
#include <grimace/unavailable/mips-ce3-nec_mobilepro_790_hpc2000.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_NEC_MOBILEPRO_900_HPC2000)
#include <grimace/unavailable/arm-ce3-nec_mobilepro_900_hpc2000.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_NEC_ROCKHOPPER_MIPS2_CE6)
#include <grimace/unavailable/mips-ce6-nec_rockhopper_mips2_ce6.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_NEC_ROCKHOPPER_MIPS2_CE6_XP)
#include <grimace/unavailable/mips-ce6-nec_rockhopper_mips2_ce6_xp.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_NEC_ROCKHOPPER_MIPS4_CE6)
#include <grimace/unavailable/mips-ce6-nec_rockhopper_mips4_ce6.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_ODO_POSEIDON_CE3)
#include <grimace/unavailable/arm-ce3-odo_poseidon_ce3.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_SHARP_MOBILON_HC4100_HPC2)
#include <grimace/unavailable/mips-ce2-sharp_mobilon_hc4100_hpc2.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_SHARP_MOBILON_HC4500_HPC2)
#include <grimace/unavailable/mips-ce2-sharp_mobilon_hc4500_hpc2.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_SHARP_MOBILON_HC4600_HPC2)
#include <grimace/unavailable/mips-ce2-sharp_mobilon_hc4600_hpc2.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_SIMANTIC_CP4)
#include <grimace/unavailable/arm-ce6-simantic_cp4.h>
#endif
#if GRIMACE_WCE == 300 && GRIMACE_ARM && defined(GRIMACE_DEVICE_SIMPAD_SL4_HPC2000)
#include <grimace/unavailable/arm-ce3-simpad_sl4_hpc2000.h>
#endif
#if GRIMACE_WCE == 600 && GRIMACE_ARM && defined(GRIMACE_DEVICE_SYNC_2)
#include <grimace/unavailable/arm-ce6-sync_2.h>
#endif
#if GRIMACE_WCE == 100 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_VELO1)
#include <grimace/unavailable/mips-ce1-velo1.h>
#endif
#if GRIMACE_WCE == 200 && GRIMACE_MIPS && defined(GRIMACE_DEVICE_VELO1)
#include <grimace/unavailable/mips-ce2-velo1.h>
#endif
