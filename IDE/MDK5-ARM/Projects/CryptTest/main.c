/* main.c
 *
 * Copyright (C) 2006-2025 wolfSSL Inc.
 *
 * This file is part of wolfSSL.
 *
 * wolfSSL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * wolfSSL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1335, USA
 */

#ifdef HAVE_CONFIG_H
    #include <config.h>
#endif

#include <wolfssl/wolfcrypt/settings.h>

#include <stdio.h>
#include <time.h>

#if defined(WOLFSSL_CMSIS_RTOS)
    #include "cmsis_os.h"
#elif defined(WOLFSSL_CMSIS_RTOSv2)
    #include "cmsis_os2.h"
#endif
/* Dummy definition for test RTC */
#define RTC_YEAR  2023
#define RTC_MONTH 1
#define RTC_DAY   1

#if defined(STM32F7xx)
#include "stm32f7xx_hal.h"
#elif defined(STM32F4xx)
#include "stm32f4xx_hal.h"
#elif defined(STM32F2xx)
#include "stm32f2xx_hal.h"
#endif

#warning "write MPU specific Set ups\n"
static void SystemClock_Config (void) {

}

static void MPU_Config (void) {

}

static void CPU_CACHE_Enable (void) {

}


#if defined(WOLFSSL_CMSIS_RTOS)
extern uint32_t os_time;
#endif

uint32_t HAL_GetTick(void) {
#if defined(WOLFSSL_CMSIS_RTOS)
    return os_time;
#elif defined(WOLFSSL_CMSIS_RTOSv2)
    return osKernelGetTickCount();
#endif
}

static  time_t epochTime;
time_t time(time_t *t) {
     return epochTime;
}

void setTime(time_t t) {
    epochTime = t;
}

/*-----------------------------------------------------------------------------
 *        Initialize a Flash Memory Card
 *----------------------------------------------------------------------------*/
#if !defined(NO_FILESYSTEM)
#include "rl_fs.h"                      /* FileSystem definitions             */

static void init_filesystem(void)
{
    int32_t retv;

    retv = finit ("M0:");
    if (retv == fsOK) {
        retv = fmount ("M0:");
        if (retv == fsOK) {
            printf ("Drive M0 ready!\n");
        }
        else {
            printf ("Drive M0 mount failed(%d)!\n", retv);
        }
    }
    else {
        printf ("Drive M0 initialization failed!\n");
    }
}
#endif


/*-----------------------------------------------------------------------------
 *       main entry
 *----------------------------------------------------------------------------*/
void wolfcrypt_test(void *arg);

int main()
{
    void * arg = NULL;

    MPU_Config();
    CPU_CACHE_Enable();
    HAL_Init();                        /* Initialize the HAL Library     */
    SystemClock_Config();              /* Configure the System Clock     */

#if !defined(NO_FILESYSTEM)
    init_filesystem ();
#endif

    setTime((RTC_YEAR-1970)*365*24*60*60 +
            RTC_MONTH*30*24*60*60 +
            RTC_DAY*24*60*60);

    printf("=== Start: Crypt test === \n");
    wolfcrypt_test(arg);
    printf("=== End: Crypt test  ===\n");

    return 0;
}
