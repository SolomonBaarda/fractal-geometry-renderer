#donotrun
// (c) 2018-2020 Claude Heiland-Allen
// SPDX-License-Identifier: GPL-3.0-or-later
// Ilford FP4 Plus black and white film

float fp4plus(float wavelength)
{
const int fp4plus_u[350] = int[350]
( 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x00, 0x00, 0x00, 0x00, 0x0d, 0x22, 0x35, 0x42, 0x4f, 0x5a
, 0x62, 0x68, 0x6d, 0x71, 0x75, 0x78, 0x7c, 0x7f, 0x81, 0x83
, 0x86, 0x87, 0x89, 0x8b, 0x8d, 0x8f, 0x91, 0x94, 0x95, 0x98
, 0x9b, 0x9c, 0x9f, 0xa1, 0xa3, 0xa5, 0xa7, 0xa9, 0xab, 0xac
, 0xad, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb4, 0xb5, 0xb5
, 0xb6, 0xb6, 0xb6, 0xb7, 0xb7, 0xb7, 0xb8, 0xb8, 0xb9, 0xb9
, 0xba, 0xbb, 0xbb, 0xbc, 0xbd, 0xbd, 0xbe, 0xbe, 0xc0, 0xc0
, 0xc2, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xca, 0xcb
, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd2, 0xd3, 0xd4
, 0xd4, 0xd5, 0xd5, 0xd6, 0xd6, 0xd7, 0xd7, 0xd7, 0xd8, 0xd8
, 0xd8, 0xd8, 0xd9, 0xd9, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda
, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda
, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xda, 0xd9, 0xd9, 0xd9
, 0xd9, 0xd8, 0xd8, 0xd8, 0xd8, 0xd7, 0xd7, 0xd7, 0xd7, 0xd6
, 0xd6, 0xd6, 0xd6, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5
, 0xd6, 0xd6, 0xd6, 0xd7, 0xd7, 0xd8, 0xd8, 0xda, 0xda, 0xdb
, 0xdc, 0xdd, 0xde, 0xde, 0xdf, 0xe0, 0xe0, 0xe0, 0xe1, 0xe2
, 0xe2, 0xe2, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3, 0xe3
, 0xe3, 0xe3, 0xe3, 0xe3, 0xe2, 0xe2, 0xe3, 0xe3, 0xe3, 0xe3
, 0xe3, 0xe4, 0xe4, 0xe5, 0xe6, 0xe6, 0xe7, 0xe7, 0xe8, 0xe9
, 0xe9, 0xea, 0xea, 0xeb, 0xec, 0xec, 0xed, 0xee, 0xee, 0xef
, 0xef, 0xf0, 0xf1, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf5, 0xf6
, 0xf6, 0xf7, 0xf8, 0xf8, 0xf9, 0xf9, 0xfa, 0xfa, 0xfb, 0xfb
, 0xfb, 0xfc, 0xfc, 0xfc, 0xfd, 0xfd, 0xfd, 0xfe, 0xfe, 0xfe
, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe
, 0xfd, 0xfd, 0xfd, 0xfc, 0xfc, 0xfb, 0xfb, 0xfb, 0xfa, 0xfa
, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa, 0xfa
, 0xfa, 0xf9, 0xf8, 0xf6, 0xf4, 0xf1, 0xee, 0xeb, 0xe8, 0xe5
, 0xe2, 0xde, 0xdb, 0xd7, 0xd3, 0xcf, 0xcb, 0xc7, 0xc2, 0xbd
, 0xb8, 0xb2, 0xad, 0xa7, 0xa1, 0x9b, 0x94, 0x8c, 0x84, 0x7c
, 0x72, 0x68, 0x5c, 0x4f, 0x3d, 0x2e, 0x1c, 0x0d, 0x00, 0x00
, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
);
  float f = wavelength - 350.0;
  int i0 = int(floor(f));
  int i1 = i0 + 1;
  float f1 = f - float(i0);
  if (i0 < 0) return 0.0;
  if (i1 > 350 - 1) return 0.0;
  return mix(float(fp4plus_u[i0]), float(fp4plus_u[i1]), f1) / float(0xfe);
}
