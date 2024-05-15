#ifndef BITMATH_H
#define BITMATH_H


#define SET_BIT(REG,BIT) (REG |= (1 << BIT)  )
#define CLR_BIT(REG,BIT) (REG &= ~(1 << BIT ))
#define GET_BIT(REG,BIT) (REG & (1 << BIT)   )

/* Two macros used in conjunction to insert a value */
#define REG_CLRMSK(REG,MSK,POS) (REG &= ~(MSK << POS))
#define REG_MODIFY(REG,VAL)     (REG |= VAL)



#endif