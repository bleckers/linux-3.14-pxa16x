#ifndef __ASM_MACH_PXA168_H
#define __ASM_MACH_PXA168_H

#include <linux/reboot.h>

#include <plat/pxa_u2o.h>


extern void pxa168_timer_init(void);
extern void __init icu_init_irq(void);
extern void __init pxa168_init_irq(void);
extern void pxa168_restart(enum reboot_mode, const char *);
extern void pxa168_clear_keypad_wakeup(void);



#include <linux/i2c.h>
#include <linux/i2c/pxa-i2c.h>
#include <mach/devices.h>
#include <linux/platform_data/mtd-nand-pxa3xx.h>
#include <video/pxa168fb.h>
#include <linux/platform_data/keypad-pxa27x.h>
#include <mach/cputype.h>
#include <linux/pxa168_eth.h>
#include <linux/platform_data/mv_usb.h>

#ifdef CONFIG_PCI_TS47XX
#include <mach/pxa168_pcie.h>
#endif

extern struct pxa_device_desc pxa168_device_uart1;
extern struct pxa_device_desc pxa168_device_uart2;
extern struct pxa_device_desc pxa168_device_uart3;
extern struct pxa_device_desc pxa168_device_twsi0;
extern struct pxa_device_desc pxa168_device_twsi1;
extern struct pxa_device_desc pxa168_device_pwm1;
extern struct pxa_device_desc pxa168_device_pwm2;
extern struct pxa_device_desc pxa168_device_pwm3;
extern struct pxa_device_desc pxa168_device_pwm4;
extern struct pxa_device_desc pxa168_device_ssp1;
extern struct pxa_device_desc pxa168_device_ssp2;
extern struct pxa_device_desc pxa168_device_ssp3;
extern struct pxa_device_desc pxa168_device_ssp4;
extern struct pxa_device_desc pxa168_device_ssp5;
extern struct pxa_device_desc pxa168_device_nand;
extern struct pxa_device_desc pxa168_device_fb;
extern struct pxa_device_desc pxa168_device_keypad;
extern struct pxa_device_desc pxa168_device_eth;

#ifdef CONFIG_MACH_TS47XX
extern struct pxa_device_desc pxa168_device_tslcd;
extern struct pxa_device_desc pxa168_device_pcie;
#endif

/* pdata can be NULL */



extern struct platform_device pxa168_device_gpio;

static inline int pxa168_add_uart(int id)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 1: d = &pxa168_device_uart1; break;
	case 2: d = &pxa168_device_uart2; break;
	case 3: d = &pxa168_device_uart3; break;
	}

	if (d == NULL)
		return -EINVAL;

	return pxa_register_device(d, NULL, 0);
}


static inline int pxa168_add_u2o(void *data)
{
   printk("%s %d\n", __func__, __LINE__);
	pxa168_device_u2o.dev.platform_data = data;
	return platform_device_register(&pxa168_device_u2o);
}

static inline int pxa168_add_u2h(struct pxa_usb_plat_info *info)
{
   printk("%s %d\n", __func__, __LINE__);
	pxa168_device_u2h.dev.platform_data = info;
	return platform_device_register(&pxa168_device_u2h);
}

static inline int pxa168_add_u2ootg(struct pxa_usb_plat_info *info)
{
   printk("%s %d\n", __func__, __LINE__);
	pxa168_device_u2ootg.dev.platform_data = info;
	return platform_device_register(&pxa168_device_u2ootg);
}

static inline int pxa168_add_u2oehci(struct pxa_usb_plat_info *info)
{
   printk("%s %d\n", __func__, __LINE__);
	pxa168_device_u2oehci.dev.platform_data = info;
	return platform_device_register(&pxa168_device_u2oehci);
}


static inline int pxa168_add_usb_host(struct mv_usb_platform_data *pdata)
{
	pxa168_device_usb_host.dev.platform_data = pdata;
	return platform_device_register(&pxa168_device_usb_host);
}


static inline int pxa168_add_twsi(int id, struct i2c_pxa_platform_data *data,
				  struct i2c_board_info *info, unsigned size)
{
	struct pxa_device_desc *d = NULL;
	int ret;
   unsigned long x;
	volatile unsigned long *ptr = MPMU_REG(0x1024);
	    
	switch (id) {
	case 0: d = &pxa168_device_twsi0; break;
	case 1: d = &pxa168_device_twsi1; break;
	default:
		return -EINVAL;
	}
		
	/* probably not the best place for this... */
   x = *ptr;
   x |= (1 << 6);    /* enable AP_I2C in the clock-gating register */
   *ptr = x;
	
	ret = i2c_register_board_info(id, info, size);
	if (ret)
		return ret;

	return pxa_register_device(d, data, sizeof(*data));
}

static inline int pxa168_add_pwm(int id)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 1: d = &pxa168_device_pwm1; break;
	case 2: d = &pxa168_device_pwm2; break;
	case 3: d = &pxa168_device_pwm3; break;
	case 4: d = &pxa168_device_pwm4; break;
	default:
		return -EINVAL;
	}

	return pxa_register_device(d, NULL, 0);
}

static inline int pxa168_add_ssp(int id)
{
	struct pxa_device_desc *d = NULL;

	switch (id) {
	case 1: d = &pxa168_device_ssp1; break;
	case 2: d = &pxa168_device_ssp2; break;
	case 3: d = &pxa168_device_ssp3; break;
	case 4: d = &pxa168_device_ssp4; break;
	case 5: d = &pxa168_device_ssp5; break;
	default:
		return -EINVAL;
	}
	return pxa_register_device(d, NULL, 0);
}

static inline int pxa168_add_nand(struct pxa3xx_nand_platform_data *info)
{
	return pxa_register_device(&pxa168_device_nand, info, sizeof(*info));
}

static inline int pxa168_add_fb(struct pxa168fb_mach_info *mi)
{
	return pxa_register_device(&pxa168_device_fb, mi, sizeof(*mi));
}

static inline int pxa168_add_keypad(struct pxa27x_keypad_platform_data *data)
{
	if (cpu_is_pxa168())
		data->clear_wakeup_event = pxa168_clear_keypad_wakeup;

	return pxa_register_device(&pxa168_device_keypad, data, sizeof(*data));
}

static inline int pxa168_add_eth(struct pxa168_eth_platform_data *data)
{
	return pxa_register_device(&pxa168_device_eth, data, sizeof(*data));
}


#ifdef CONFIG_MACH_TS47XX
extern void ts4700_init_mux_irq(void);
#ifdef CONFIG_PCI_TS47XX
static inline int pxa168_add_pcie(struct pxa168_pcie_platform_data *data)
{
	return pxa_register_device(&pxa168_device_pcie, data, sizeof(*data));
}
#endif
#endif


extern unsigned u2o_get(unsigned base, unsigned offset);
extern void u2o_set(unsigned base, unsigned offset, unsigned value);
extern void u2o_clear(unsigned base, unsigned offset, unsigned value);
extern void u2o_write(unsigned base, unsigned offset, unsigned value);

extern int pxa168_usb_phy_init(void __iomem *base);
extern int pxa168_usb_phy_deinit(void __iomem *base);

#endif /* __ASM_MACH_PXA168_H */
