#ifndef _FT_COPRO_CMDS_H_
#define _FT_COPRO_CMDS_H_

ft_void_t Ft_Gpu_CoCmd_Text(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s);
ft_void_t Ft_Gpu_CoCmd_Letter(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, const ft_char8_t s);
ft_void_t Ft_Gpu_CoCmd_Number(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, ft_int32_t n);
ft_void_t Ft_Gpu_CoCmd_LoadIdentity(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Toggle(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t font, ft_uint16_t options, ft_uint16_t state, const ft_char8_t* s);
ft_void_t Ft_Gpu_CoCmd_Gauge(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t major, ft_uint16_t minor, ft_uint16_t val, ft_uint16_t range);
ft_void_t Ft_Gpu_CoCmd_RegRead(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t result);
ft_void_t Ft_Gpu_CoCmd_GetProps(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t w, ft_uint32_t h);
ft_void_t Ft_Gpu_CoCmd_Memcpy(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t dest, ft_uint32_t src, ft_uint32_t num);
ft_void_t Ft_Gpu_CoCmd_Spinner(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_uint16_t style, ft_uint16_t scale);
ft_void_t Ft_Gpu_CoCmd_BgColor(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t c);
ft_void_t Ft_Gpu_CoCmd_Swap(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Inflate(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr);
ft_void_t Ft_Gpu_CoCmd_Translate(Ft_Gpu_Hal_Context_t *phost,ft_int32_t tx, ft_int32_t ty);
ft_void_t Ft_Gpu_CoCmd_Stop(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Slider(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t range);
ft_void_t Ft_Gpu_CoCmd_Interrupt(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ms);
ft_void_t Ft_Gpu_CoCmd_FgColor(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t c);
ft_void_t Ft_Gpu_CoCmd_Rotate(Ft_Gpu_Hal_Context_t *phost,ft_int32_t a);
ft_void_t Ft_Gpu_CoCmd_Button(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s);
ft_void_t Ft_Gpu_CoCmd_MemWrite(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t num);
ft_void_t Ft_Gpu_CoCmd_Scrollbar(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t size, ft_uint16_t range);
ft_void_t Ft_Gpu_CoCmd_GetMatrix(Ft_Gpu_Hal_Context_t *phost,ft_int32_t a, ft_int32_t b, ft_int32_t c, ft_int32_t d, ft_int32_t e, ft_int32_t f);
ft_void_t Ft_Gpu_CoCmd_Sketch(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_uint16_t w, ft_uint16_t h, ft_uint32_t ptr, ft_uint16_t format);
ft_void_t Ft_Gpu_CoCmd_MemSet(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t value, ft_uint32_t num);
ft_void_t Ft_Gpu_CoCmd_Calibrate(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t result);
ft_void_t Ft_Gpu_CoCmd_SetFont(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t font, ft_uint32_t ptr);
ft_void_t Ft_Gpu_CoCmd_Bitmap_Transform(Ft_Gpu_Hal_Context_t *phost,ft_int32_t x0, ft_int32_t y0, ft_int32_t x1, ft_int32_t y1, ft_int32_t x2, ft_int32_t y2, ft_int32_t tx0, ft_int32_t ty0, ft_int32_t tx1, ft_int32_t ty1, ft_int32_t tx2, ft_int32_t ty2, ft_uint16_t result);
ft_void_t Ft_Gpu_CoCmd_GradColor(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t c);
ft_void_t Ft_Gpu_CoCmd_Append(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t num);
ft_void_t Ft_Gpu_CoCmd_MemZero(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t num);
ft_void_t Ft_Gpu_CoCmd_Scale(Ft_Gpu_Hal_Context_t *phost,ft_int32_t sx, ft_int32_t sy);
ft_void_t Ft_Gpu_CoCmd_Clock(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t h, ft_uint16_t m, ft_uint16_t s, ft_uint16_t ms);
ft_void_t Ft_Gpu_CoCmd_Gradient(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x0, ft_int16_t y0, ft_uint32_t rgb0, ft_int16_t x1, ft_int16_t y1, ft_uint32_t rgb1);
ft_void_t Ft_Gpu_CoCmd_SetMatrix(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Track(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t tag);
ft_void_t Ft_Gpu_CoCmd_GetPtr(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t result);
ft_void_t Ft_Gpu_CoCmd_Progress(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_uint16_t options, ft_uint16_t val, ft_uint16_t range);
ft_void_t Ft_Gpu_CoCmd_ColdStart(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Keys(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t w, ft_int16_t h, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s);
ft_void_t Ft_Gpu_CoCmd_Dial(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t r, ft_uint16_t options, ft_uint16_t val);
ft_void_t Ft_Gpu_CoCmd_LoadImage(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t options);
ft_void_t Ft_Gpu_CoCmd_Dlstart(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Snapshot(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr);
ft_void_t Ft_Gpu_CoCmd_ScreenSaver(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Memcrc(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t ptr, ft_uint32_t num, ft_uint32_t result);


ft_void_t Ft_Gpu_CoCmd_Logo(Ft_Gpu_Hal_Context_t *phost);
ft_void_t Ft_Gpu_CoCmd_Calibrate(Ft_Gpu_Hal_Context_t *phost,ft_uint32_t result);
//ft_void_t Ft_Gpu_CoCmd_Text(Ft_Gpu_Hal_Context_t *phost,ft_int16_t x, ft_int16_t y, ft_int16_t font, ft_uint16_t options, const ft_char8_t* s);


#endif  /*FT_COPRO_CMDS_H*/
