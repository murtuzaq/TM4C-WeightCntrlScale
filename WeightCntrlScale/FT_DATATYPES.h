#ifndef _FT_DATATYPES_H_
#define _FT_DATATYPES_H_

#define FT_FALSE           (0)
#define FT_TRUE            (1)

typedef unsigned char ft_uint8_t;
typedef char ft_char8_t;
typedef signed char ft_schar8_t;
typedef unsigned char ft_uchar8_t;
typedef short  ft_int16_t;
typedef unsigned short ft_uint16_t;
typedef unsigned int ft_uint32_t;
typedef int ft_int32_t;
typedef void ft_void_t;

typedef unsigned char ft_bool_t;

typedef const unsigned char  ft_prog_uchar8_t;
typedef const char ft_prog_char8_t;
typedef const unsigned short ft_prog_uint16_t;

#define ft_delay(x) delay(x)

#define PROGMEM const
#define FT_PROGMEM PROGMEM
#define pgm_read_byte_near(a) (*((volatile unsigned char *)a))
#define ft_pgm_read_byte_near pgm_read_byte_near
#define ft_pgm_read_byte pgm_read_byte
#define ft_pgm_read_word        (unsigned short const)//pgm_read_word
#define ft_random(x)		(x) //(random(x))

#define TRUE     (1)
#define FALSE    (0)

#endif /*_FT_DATATYPES_H_*/

/* Nothing beyond this*/





