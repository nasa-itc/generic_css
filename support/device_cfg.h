#ifndef _GENERIC_CSS_CHECKOUT_DEVICE_CFG_H_
#define _GENERIC_CSS_CHECKOUT_DEVICE_CFG_H_

/*
** GENERIC_CSS Checkout Configuration
*/
#define GENERIC_CSS_CFG
/* Note: NOS3 uart requires matching handle and bus number */
#define GENERIC_CSS_CFG_STRING           "/dev/i2c_3"
#define GENERIC_CSS_CFG_HANDLE           3 
#define GENERIC_CSS_CFG_BAUDRATE_HZ      1000000
#define GENERIC_CSS_CFG_MS_TIMEOUT       250
#define GENERIC_CSS_CFG_DEBUG

#endif /* _GENERIC_CSS_CHECKOUT_DEVICE_CFG_H_ */