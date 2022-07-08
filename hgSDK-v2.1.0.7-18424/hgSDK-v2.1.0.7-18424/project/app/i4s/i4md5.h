/*
 * MD5 internal definitions
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef MD5_I_H
#define MD5_I_H

typedef struct MD5Context_t{
	u32 buf[4];
	u32 bits[2];
	u8 in[64];
}MD5_CTX;

void i4MD5Init(MD5_CTX *context);
void i4MD5Update(MD5_CTX *context, unsigned char const *buf,
	       unsigned len);
void i4MD5Final(unsigned char digest[16], MD5_CTX *context);

#endif /* MD5_I_H */
