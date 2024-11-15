#ifndef _ActivePixelFormatH_
#define _ActivePixelFormatH_

namespace ActiveSilicon {
   namespace ActiveApi {

      typedef enum ActivePixelFormat
      {
         ActivePixelFormatUnknown = 0,
         ActivePixelFormatMono8 = 0x01080001,
         ActivePixelFormatMono10 = 0x01100003,
         ActivePixelFormatMono10p = 0x010A0046,
         ActivePixelFormatMono12 = 0x01100005,
         ActivePixelFormatMono12p = 0x010C0047,
         ActivePixelFormatMono14 = 0x01100025,
         ActivePixelFormatMono14p = 0x010E0048,
         ActivePixelFormatMono16 = 0x01100007,
         ActivePixelFormatBayerGR8 = 0x01080008,
         ActivePixelFormatBayerGR10 = 0x0110000C,
         ActivePixelFormatBayerGR10p = 0x010A0056,
         ActivePixelFormatBayerGR12 = 0x01100010,
         ActivePixelFormatBayerGR12p = 0x010C0057,
         ActivePixelFormatBayerGR14 = 0x01100014,
         ActivePixelFormatBayerGR14p = 0x010E0058,
         ActivePixelFormatBayerGR16 = 0x0110002E,
         ActivePixelFormatBayerRG8 = 0x01080009,
         ActivePixelFormatBayerRG10 = 0x0110000D,
         ActivePixelFormatBayerRG10p = 0x010A0058,
         ActivePixelFormatBayerRG12 = 0x01100011,
         ActivePixelFormatBayerRG12p = 0x010C0059,
         ActivePixelFormatBayerRG14 = 0x01100015,
         ActivePixelFormatBayerRG14p = 0x010E0060,
         ActivePixelFormatBayerRG16 = 0x0110002F,
         ActivePixelFormatBayerGB8 = 0x0108000A,
         ActivePixelFormatBayerGB10 = 0x0110000E,
         ActivePixelFormatBayerGB10p = 0x010A0054,
         ActivePixelFormatBayerGB12 = 0x01100012,
         ActivePixelFormatBayerGB12p = 0x010C0055,
         ActivePixelFormatBayerGB14 = 0x01100016,
         ActivePixelFormatBayerGB14p = 0x010E0056,
         ActivePixelFormatBayerGB16 = 0x01100030,
         ActivePixelFormatBayerBG8 = 0x0108000B,
         ActivePixelFormatBayerBG10 = 0x0110000F,
         ActivePixelFormatBayerBG10p = 0x010A0052,
         ActivePixelFormatBayerBG12 = 0x01100013,
         ActivePixelFormatBayerBG12p = 0x010C0053,
         ActivePixelFormatBayerBG14 = 0x01100017,
         ActivePixelFormatBayerBG14p = 0x010E0054,
         ActivePixelFormatBayerBG16 = 0x01100031,
         ActivePixelFormatRGB8 = 0x02180014,
         ActivePixelFormatRGB10 = 0x02300018,
         ActivePixelFormatRGB12 = 0x0230001A,
         ActivePixelFormatRGB14 = 0x0230005E,
         ActivePixelFormatRGB16 = 0x02300033,
         ActivePixelFormatBGR8 = 0x02180015,
         ActivePixelFormatBGR10 = 0x02300019,
         ActivePixelFormatBGR12 = 0x0230001B,
         ActivePixelFormatBGR14 = 0x0230004A,
         ActivePixelFormatBGR16 = 0x0230004B,
         ActivePixelFormatRGBa8 = 0x02200016,
         ActivePixelFormatRGBa10 = 0x0240005F,
         ActivePixelFormatRGBa12 = 0x02400061,
         ActivePixelFormatRGBa14 = 0x02400063,
         ActivePixelFormatRGBa16 = 0x02400064,
         ActivePixelFormatBGRa8 = 0x02200017,
         ActivePixelFormatBGRa10 = 0x0240004C,
         ActivePixelFormatBGRa12 = 0x0240004E,
         ActivePixelFormatBGRa14 = 0x02400050,
         ActivePixelFormatBGRa16 = 0x02400051,
         ActivePixelFormatYUV422_8 = 0x02100032,
         ActivePixelFormatxRGB8 = 0x80000000
      } ActivePixelFormat;

   }
}

#endif // _ActivePixelFormatH_
