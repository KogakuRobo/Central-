#ifndef _L3G4200D_defined_HPP_
#define _L3G4200D_defined_HPP_

//-------------------------------------------------------------------
#define L3G4200D_WHO_AM_I_ADDR		0x0F

//-------------------------------------------------------------------
#define L3G4200D_REG1_ADDR		0x20

#define L3G4200D_REG1_ODR_100		0x00
#define L3G4200D_REG1_ODR_200		0x40
#define L3G4200D_REG1_ODR_400		0x80
#define L3G4200D_REG1_ODR_800		0xC0

#define L3G4200D_REG1_BW_0		0x00
#define L3G4200D_REG1_BW_1		0x10
#define L3G4200D_REG1_BW_2		0x20
#define L3G4200D_REG1_BW_3		0x30

#define L3G4200D_REG1_POWER		0x08
#define L3G4200D_REG1_DOWN		0x00

#define L3G4200D_REG1_Z_ENABLE		0x04
#define L3G4200D_REG1_Z_DISABLE		0x00

#define L3G4200D_REG1_Y_ENABLE		0x02
#define L3G4200D_REG1_Y_DISABLE		0x00

#define L3G4200D_REG1_X_ENABLE		0x01
#define L3G4200D_REG1_X_DISABLE		0x00

//------------------------------------------------------------------------

#define L3G4200D_REG2_ADDR		0x21

//------------------------------------------------------------------------

#define L3G4200D_REG3_ADDR		0x22

//------------------------------------------------------------------------

#define L3G4200D_REG4_ADDR		0x23

#define L3G4200D_REG4_FS_250		0x00
#define L3G4200D_REG4_FS_500		0x10
#define L3G4200D_REG4_FS_2000		0x20

//------------------------------------------------------------------------

#define L3G4200D_REG5_ADDR		0x24

//------------------------------------------------------------------------

#endif