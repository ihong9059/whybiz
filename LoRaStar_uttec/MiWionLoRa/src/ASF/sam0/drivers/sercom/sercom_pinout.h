/**
 * \file
 *
 * \brief SAM SERCOM Module Pinout Definitions
 *
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#ifndef SERCOM_PINOUT_H_INCLUDED
#define SERCOM_PINOUT_H_INCLUDED

#include <compiler.h>

#if SAMR21E
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA08C_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA09C_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA14C_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA15C_SERCOM2_PAD3

	/* SERCOM3 */
# if SAM_PART_IS_DEFINED(SAMR21E19A)
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
# else
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA27F_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA28F_SERCOM3_PAD1
#endif
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA24C_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA25C_SERCOM3_PAD3

	/* SERCOM4 */
# if SAM_PART_IS_DEFINED(SAMR21E19A)
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PB08D_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PB09D_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PA14D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PA15D_SERCOM4_PAD3
# else
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PC19F_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PB31F_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PB30F_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PC18F_SERCOM4_PAD3
# endif

	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PB30D_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PB31D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PA24D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PA25D_SERCOM5_PAD3

#elif SAMR21G
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA00D_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA01D_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA30D_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA31D_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA12C_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA13C_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA14C_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA15C_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA18D_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA19D_SERCOM3_PAD3

	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PC19F_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PB31F_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PB30F_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PC18F_SERCOM4_PAD3

	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PA22D_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PA23D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PA24D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PA25D_SERCOM5_PAD3

#elif (SAMD09)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA08D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA09D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA30C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA31C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA24C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA25C_SERCOM1_PAD3

#elif (SAMD10DS) || (SAMD10DM) || (SAMD10DU) || (SAMD11DS) || (SAMD11DM) || (SAMD11DU) 
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA22C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA23C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA30D_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA31D_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA22D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA23D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA16D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA25D_SERCOM2_PAD3

#elif (SAMD10C) || (SAMD11C)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA08D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA09D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA30C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA31C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA24C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA25C_SERCOM1_PAD3

#elif SAM_PART_IS_DEFINED(SAMD21E15L) || SAM_PART_IS_DEFINED(SAMD21E16L)

	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA10D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA11D_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA22C_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA23C_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA24C_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA25C_SERCOM3_PAD3

#elif (SAML22N)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA08C_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA09C_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA10C_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA11C_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA22D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA23D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA20D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA21D_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PB02C_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PB21C_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PB00C_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PB01C_SERCOM3_PAD3

	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PA12C_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PA13C_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PA14C_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PA15C_SERCOM4_PAD3

	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PB30D_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PB31D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PB22D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PB23D_SERCOM5_PAD3
#elif (SAML22J) || (SAML22G)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA08C_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA09C_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA10C_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA11C_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA22D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA23D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA20D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA21D_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA12D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA13D_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA14D_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA15D_SERCOM3_PAD3
#elif (SAMC20E) || (SAMC21E)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA10D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA11D_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA22C_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA23C_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA24C_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA25C_SERCOM3_PAD3

#elif (SAMC20G) || (SAMC21G)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA12C_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA13C_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA14C_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA15C_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA22C_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA23C_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA24C_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA25C_SERCOM3_PAD3

  #ifdef ID_SERCOM4
	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PB08D_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PB09D_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PB10D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PB11D_SERCOM4_PAD3
  #endif

  #ifdef ID_SERCOM5
	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PB02D_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PB03D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PB22D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PB23D_SERCOM5_PAD3
  #endif

#elif (SAMC20J) || (SAMC21J)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA12C_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA13C_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA14C_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA15C_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA22C_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA23C_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA24C_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA25C_SERCOM3_PAD3

  #ifdef ID_SERCOM4
	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PB08D_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PB09D_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PB10D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PB11D_SERCOM4_PAD3
  #endif

  #ifdef ID_SERCOM5
	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PB02D_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PB03D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PB00D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PB01D_SERCOM5_PAD3
  #endif

#elif (SAMDA1)
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA00D_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA01D_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA30D_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA31D_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA10D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA11D_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA18D_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA19D_SERCOM3_PAD3

  #if (SAMDA1E)
	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      0 /* No available pin */
	#define SERCOM4_PAD1_DEFAULT      0 /* No available pin */
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PA14D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PA15D_SERCOM4_PAD3
  #else
	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PA12D_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PA13D_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PA14D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PA15D_SERCOM4_PAD3
  #endif

	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PA22D_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PA23D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PA24D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PA25D_SERCOM5_PAD3

#elif   (SAMHA1E) || (SAMHA0E)
	
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA08C_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA09C_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA14C_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA15C_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA18D_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA19D_SERCOM3_PAD3

	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      0 /* No available pin */
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PA13D_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PA14D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PB11D_SERCOM4_PAD3

	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      0 /* No available pin */
	#define SERCOM5_PAD1_DEFAULT      0 /* No available pin */
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PA20C_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      0 /* No available pin */
	
#elif   (SAMHA1G) || (SAMHA0G)
	
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA10C_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA11C_SERCOM0_PAD3
   
	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3
    
	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA14C_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA15C_SERCOM2_PAD3
    
	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA18D_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA19D_SERCOM3_PAD3
    
	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      0 /* No available pin */
	#define SERCOM4_PAD1_DEFAULT      0 /* No available pin */
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PB10D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PB11D_SERCOM4_PAD3

	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PB16C_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PB17C_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PA20C_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PA21C_SERCOM5_PAD3

#elif (SAML21E) || (SAMR34) || (SAMR35) 

	/* SERCOM0 */
 	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
 	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA00D_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA01D_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA30D_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA31D_SERCOM1_PAD3

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA10D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA11D_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA18D_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA19D_SERCOM3_PAD3

  #if !SAM_PART_IS_DEFINED(SAML21E18A) 
	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      0 /* No available pin */
	#define SERCOM4_PAD1_DEFAULT      0 /* No available pin */
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PA14D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PA15D_SERCOM4_PAD3
	
    /* SERCOM5 */
    #define SERCOM5_PAD0_DEFAULT      PINMUX_PA22D_SERCOM5_PAD0
    #define SERCOM5_PAD1_DEFAULT      PINMUX_PA23D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PA24D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PA25D_SERCOM5_PAD3
  #endif
  
#elif  (SAMR30E)

   /* SERCOM0 */
   #define SERCOM0_PAD0_DEFAULT      0 /* No available pin */
   #define SERCOM0_PAD1_DEFAULT      0 /* No available pin */
   #define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
   #define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

   /* SERCOM1 */
   #define SERCOM1_PAD0_DEFAULT      0 /* No available pin */
   #define SERCOM1_PAD1_DEFAULT      0 /* No available pin */
   #define SERCOM1_PAD2_DEFAULT      PINMUX_PA30D_SERCOM1_PAD2
   #define SERCOM1_PAD3_DEFAULT      PINMUX_PA31D_SERCOM1_PAD3

   /* SERCOM2 */
   #define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
   #define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
   #define SERCOM2_PAD2_DEFAULT      0 /* No available pin */
   #define SERCOM2_PAD3_DEFAULT      0 /* No available pin */

   /* SERCOM3 */
   #define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
   #define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
   #define SERCOM3_PAD2_DEFAULT      PINMUX_PA18D_SERCOM3_PAD2
   #define SERCOM3_PAD3_DEFAULT      PINMUX_PA19D_SERCOM3_PAD3

  /* SERCOM4 */
  #define SERCOM4_PAD0_DEFAULT      0 /* No available pin */
  #define SERCOM4_PAD1_DEFAULT      0 /* No available pin */
  #define SERCOM4_PAD2_DEFAULT      PINMUX_PA14D_SERCOM4_PAD2
  #define SERCOM4_PAD3_DEFAULT      PINMUX_PA15D_SERCOM4_PAD3

  /* SERCOM5 */
  #define SERCOM5_PAD0_DEFAULT      0
  #define SERCOM5_PAD1_DEFAULT      0
  #define SERCOM5_PAD2_DEFAULT      PINMUX_PA24D_SERCOM5_PAD2
  #define SERCOM5_PAD3_DEFAULT      PINMUX_PA25D_SERCOM5_PAD3

#else
	/* SERCOM0 */
	#define SERCOM0_PAD0_DEFAULT      PINMUX_PA04D_SERCOM0_PAD0
	#define SERCOM0_PAD1_DEFAULT      PINMUX_PA05D_SERCOM0_PAD1
	#define SERCOM0_PAD2_DEFAULT      PINMUX_PA06D_SERCOM0_PAD2
	#define SERCOM0_PAD3_DEFAULT      PINMUX_PA07D_SERCOM0_PAD3

	/* SERCOM1 */
#if SAM_PART_IS_DEFINED(SAMD21G15L) || SAM_PART_IS_DEFINED(SAMD21G16L)
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA16C_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA17C_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA18C_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA19C_SERCOM1_PAD3
#else
	#define SERCOM1_PAD0_DEFAULT      PINMUX_PA00D_SERCOM1_PAD0
	#define SERCOM1_PAD1_DEFAULT      PINMUX_PA01D_SERCOM1_PAD1
	#define SERCOM1_PAD2_DEFAULT      PINMUX_PA30D_SERCOM1_PAD2
	#define SERCOM1_PAD3_DEFAULT      PINMUX_PA31D_SERCOM1_PAD3
#endif

	/* SERCOM2 */
	#define SERCOM2_PAD0_DEFAULT      PINMUX_PA08D_SERCOM2_PAD0
	#define SERCOM2_PAD1_DEFAULT      PINMUX_PA09D_SERCOM2_PAD1
	#define SERCOM2_PAD2_DEFAULT      PINMUX_PA10D_SERCOM2_PAD2
	#define SERCOM2_PAD3_DEFAULT      PINMUX_PA11D_SERCOM2_PAD3

	/* SERCOM3 */
	#define SERCOM3_PAD0_DEFAULT      PINMUX_PA16D_SERCOM3_PAD0
	#define SERCOM3_PAD1_DEFAULT      PINMUX_PA17D_SERCOM3_PAD1
	#define SERCOM3_PAD2_DEFAULT      PINMUX_PA18D_SERCOM3_PAD2
	#define SERCOM3_PAD3_DEFAULT      PINMUX_PA19D_SERCOM3_PAD3

  #if !(SAMD20E || SAMD21E)
	/* SERCOM4 */
	#define SERCOM4_PAD0_DEFAULT      PINMUX_PA12D_SERCOM4_PAD0
	#define SERCOM4_PAD1_DEFAULT      PINMUX_PA13D_SERCOM4_PAD1
	#define SERCOM4_PAD2_DEFAULT      PINMUX_PA14D_SERCOM4_PAD2
	#define SERCOM4_PAD3_DEFAULT      PINMUX_PA15D_SERCOM4_PAD3

	/* SERCOM5 */
	#define SERCOM5_PAD0_DEFAULT      PINMUX_PA22D_SERCOM5_PAD0
	#define SERCOM5_PAD1_DEFAULT      PINMUX_PA23D_SERCOM5_PAD1
	#define SERCOM5_PAD2_DEFAULT      PINMUX_PA24D_SERCOM5_PAD2
	#define SERCOM5_PAD3_DEFAULT      PINMUX_PA25D_SERCOM5_PAD3
  #endif
  
#endif
#endif /* SERCOM_PINOUT_H_INCLUDED */
