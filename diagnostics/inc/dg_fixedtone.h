/*
 * dg_fixedtone.h: Audio fixed tone header file.
 *
 * (C) Copyright 2010
 * Mistral Solutions Private Limited, <www.mistralsolutions.com>
 *
 * Author :
 *	Ganeshan <nganeshan@mistralsolutions.com>
 *
 * Derived from OMAP2EVM ITBOK.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
/*
 * modification history
 * --------------------
 *   10 mar 2009 - Created from OMAP2 ITBOK source.
 */
/*
******************************************************************************
**  Sampling Frequency =    44100                                           **
**  Tone Frequency     =    1000                                            **
**  Number of Samples  =    441                                             **
**  Amplitude Ch1      =    -3.00                                           **
**  Phase Ch1          =    0.00                                            **
**  Amplitude Ch2      =    -3.00                                           **
**  Phase Ch2          =    90.00                                           **
******************************************************************************
*/

static U32 audio_fixedtone[] = {
    0x00005a9d, 0x0cdd59b2, 0x197956f6, 0x25905276, 0x30e44c4b, 0x3b3b4494,
    0x445e3b79, 0x4c1e3129, 0x525425db, 0x56df19c8, 0x59a70d2f, 0x5a9d0052,
    0x59bef374, 0x570de6d7,
    0x5299dabb, 0x4c78cf62, 0x44cac504, 0x3bb7bbd9, 0x316fb40f, 0x2626adcf,
    0x1a17a939, 0x0d81a666, 0x00a5a563, 0xf3c6a637, 0xe726a8dc, 0xdb07ad46,
    0xcfa7b35c, 0xc543bb01,
    0xbc0fc40b, 0xb43cce4c, 0xadf1d98f, 0xa951e59a, 0xa672f22d, 0xa564ff09,
    0xa62c0be8, 0xa8c6188a, 0xad2424ae, 0xb3303013, 0xbacb3a7e, 0xc3cd43ba,
    0xce074b97, 0xd94451eb,
    0xe54b5697, 0xf1dc5981, 0xfeb65a9b, 0x0b9659df, 0x183b5751, 0x246252fd,
    0x2fcc4cfb, 0x3a3f456a, 0x43833c71, 0x4b69323e, 0x51c82706, 0x567e1b04,
    0x59740e76, 0x5a9a019d,
    0x59e9f4bc, 0x5767e815, 0x531edbea, 0x4d27d07a, 0x459fc601, 0x3caebcb4,
    0x3283b4c5, 0x2751ae5c, 0x1b53a99a, 0x0ec7a699, 0x01efa568, 0xf50ea60c,
    0xe865a884, 0xdc36acc1,
    0xd0c0b2ae, 0xc640ba2c, 0xbcecc314, 0xb4f3cd39, 0xae80d865, 0xa9b3e45e,
    0xa6a7f0e7, 0xa56afdbe, 0xa6030aa0, 0xa86e174c, 0xaca0237e, 0xb2832ef9,
    0xb9f83980, 0xc2d742dd,
    0xccf54adf, 0xd81b515c, 0xe4105633, 0xf096594b, 0xfd6c5a94, 0x0a4e5a07,
    0x16fc57a7, 0x23325380, 0x2eb24da8, 0x3940463d, 0x42a53d65, 0x4ab03350,
    0x5137282f, 0x561a1c3f,
    0x593d0fbc, 0x5a9202e7, 0x5a10f604, 0x57bce954, 0x53a0dd1a, 0x4dd2d194,
    0x4671c700, 0x3da2bd93, 0x3394b57f, 0x2879aeed, 0x1c8daa00, 0x100da6d1,
    0x033aa571, 0xf656a5e7,
    0xe9a4a830, 0xdd66ac40, 0xd1dcb204, 0xc740b95b, 0xbdcbc221, 0xb5aecc29,
    0xaf12d73d, 0xaa1be324, 0xa6e0efa2, 0xa574fc74, 0xa5de0957, 0xa81c160c,
    0xac21224d, 0xb1da2ddd,
    0xb928387f, 0xc1e541fc, 0xcbe54a22, 0xd6f350c8, 0xe2d655cb, 0xef505911,
    0xfc215a88, 0x09055a2a, 0x15bb57f8, 0x220153fe, 0x2d964e50, 0x383f470c,
    0x41c33e57, 0x49f3345f,
    0x50a32957, 0x55b01d78, 0x59011101, 0x5a850431, 0x5a32f74d, 0x580cea95,
    0x541dde4c, 0x4e7ad2b2, 0x473fc802, 0x3e93be76, 0x34a2b63d, 0x29a0af83,
    0x1dc6aa6b, 0x1152a70e,
    0x0484a57f, 0xf79fa5c6, 0xeae5a7e1, 0xde98abc4, 0xd2f9b15d, 0xc843b88e,
    0xbeafc132, 0xb66dcb1b, 0xafa9d617, 0xaa86e1ec, 0xa71eee5d, 0xa584fb2a,
    0xa5be080e, 0xa7ce14cb,
    0xaba6211b, 0xb1342cbf, 0xb85b377b, 0xc0f64118, 0xcad84962, 0xd5cd5030,
    0xe19e555e, 0xee0c58d2, 0xfad75a78, 0x07bc5a49, 0x147a5845, 0x20ce5478,
    0x2c774ef4, 0x373a47d7,
    0x40de3f45, 0x4932356b, 0x500a2a7c, 0x55431eb0, 0x58c11245, 0x5a73057b,
    0x5a50f896, 0x5858ebd6, 0x5496df7f, 0x4f1dd3d1, 0x4809c908, 0x3f80bf5c,
    0x35aeb6ff, 0x2ac5b01d,
    0x1efeaada, 0x1296a750, 0x05cea592, 0xf8e9a5aa, 0xec27a796, 0xdfcdab4d,
    0xd419b0bb, 0xc949b7c5, 0xbf96c045, 0xb730ca10, 0xb044d4f3, 0xaaf6e0b5,
    0xa761ed19, 0xa598f9e0,
    0xa5a306c5, 0xa7841388, 0xab301fe6, 0xb0932b9f, 0xb7933675, 0xc00a4030,
    0xc9ce489f, 0xd4aa4f94, 0xe06754ed, 0xecc8588e, 0xf98d5a63, 0x06735a63,
    0x1338588e, 0x1f9954ed,
    0x2b564f94, 0x3632489f, 0x3ff64030, 0x486d3675, 0x4f6d2b9f, 0x54d01fe6,
    0x587c1388, 0x5a5d06c5, 0x5a68f9e0, 0x589fed19, 0x550ae0b5, 0x4fbcd4f3,
    0x48d0ca10, 0x406ac045,
    0x36b7b7c5, 0x2be7b0bb, 0x2033ab4d, 0x13d9a796, 0x0717a5aa, 0xfa32a592,
    0xed6aa750, 0xe102aada, 0xd53bb01d, 0xca52b6ff, 0xc080bf5c, 0xb7f7c908,
    0xb0e3d3d1, 0xab6adf7f,
    0xa7a8ebd6, 0xa5b0f896, 0xa58d057b, 0xa73f1245, 0xaabd1eb0, 0xaff62a7c,
    0xb6ce356b, 0xbf223f45, 0xc8c647d7, 0xd3894ef4, 0xdf325478, 0xeb865845,
    0xf8445a49, 0x05295a78,
    0x11f458d2, 0x1e62555e, 0x2a335030, 0x35284962, 0x3f0a4118, 0x47a5377b,
    0x4ecc2cbf, 0x545a211b, 0x583214cb, 0x5a42080e, 0x5a7cfb2a, 0x58e2ee5d,
    0x557ae1ec, 0x5057d617,
    0x4993cb1b, 0x4151c132, 0x37bdb88e, 0x2d07b15d, 0x2168abc4, 0x151ba7e1,
    0x0861a5c6, 0xfb7ca57f, 0xeeaea70e, 0xe23aaa6b, 0xd660af83, 0xcb5eb63d,
    0xc16dbe76, 0xb8c1c802,
    0xb186d2b2, 0xabe3de4c, 0xa7f4ea95, 0xa5cef74d, 0xa57b0431, 0xa6ff1101,
    0xaa501d78, 0xaf5d2957, 0xb60d345f, 0xbe3d3e57, 0xc7c1470c, 0xd26a4e50,
    0xddff53fe, 0xea4557f8,
    0xf6fb5a2a, 0x03df5a88, 0x10b05911, 0x1d2a55cb, 0x290d50c8, 0x341b4a22,
    0x3e1b41fc, 0x46d8387f, 0x4e262ddd, 0x53df224d, 0x57e4160c, 0x5a220957,
    0x5a8cfc74, 0x5920efa2,
    0x55e5e324, 0x50eed73d, 0x4a52cc29, 0x4235c221, 0x38c0b95b, 0x2e24b204,
    0x229aac40, 0x165ca830, 0x09aaa5e7, 0xfcc6a571, 0xeff3a6d1, 0xe373aa00,
    0xd787aeed, 0xcc6cb57f,
    0xc25ebd93, 0xb98fc700, 0xb22ed194, 0xac60dd1a, 0xa844e954, 0xa5f0f604,
    0xa56e02e7, 0xa6c30fbc, 0xa9e61c3f, 0xaec9282f, 0xb5503350, 0xbd5b3d65,
    0xc6c0463d, 0xd14e4da8,
    0xdcce5380, 0xe90457a7, 0xf5b25a07, 0x02945a94, 0x0f6a594b, 0x1bf05633,
    0x27e5515c, 0x330b4adf, 0x3d2942dd, 0x46083980, 0x4d7d2ef9, 0x5360237e,
    0x5792174c, 0x59fd0aa0,
    0x5a96fdbe, 0x5959f0e7, 0x564de45e, 0x5180d865, 0x4b0dcd39, 0x4314c314,
    0x39c0ba2c, 0x2f40b2ae, 0x23caacc1, 0x179ba884, 0x0af2a60c, 0xfe11a568,
    0xf139a699, 0xe4ada99a,
    0xd8afae5c, 0xcd7db4c5, 0xc352bcb4, 0xba61c601, 0xb2d9d07a, 0xace2dbea,
    0xa899e815, 0xa617f4bc, 0xa566019d, 0xa68c0e76, 0xa9821b04, 0xae382706,
    0xb497323e, 0xbc7d3c71,
    0xc5c1456a, 0xd0344cfb, 0xdb9e52fd, 0xe7c55751, 0xf46a59df, 0x014a5a9b,
    0x0e245981, 0x1ab55697, 0x26bc51eb, 0x31f94b97, 0x3c3343ba, 0x45353a7e,
    0x4cd03013, 0x52dc24ae,
    0x573a188a, 0x59d40be8, 0x5a9cff09, 0x598ef22d, 0x56afe59a, 0x520fd98f,
    0x4bc4ce4c, 0x43f1c40b, 0x3abdbb01, 0x3059b35c, 0x24f9ad46, 0x18daa8dc,
    0x0c3aa637, 0xff5ba563,
    0xf27fa666, 0xe5e9a939, 0xd9daadcf, 0xce91b40f, 0xc449bbd9, 0xbb36c504,
    0xb388cf62, 0xad67dabb, 0xa8f3e6d7, 0xa642f374, 0xa5630052, 0xa6590d2f,
    0xa92119c8, 0xadac25db,
    0xb3e23129, 0xbba23b79, 0xc4c54494, 0xcf1c4c4b, 0xda705276, 0xe68756f6,
    0xf32359b2
};
