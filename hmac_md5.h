/****************************************************************************

    File: "hmac_md5.h"

    This file is part of the Doorman Daemon & Portknocker client.

    This file is placed in the PUBLIC DOMAIN by its author,
    J.B.Ward, April 2006.



*****************************************************************************/



#ifndef HMAC_MD5_H
#define HMAC_MD5_H

void hmac_md5 (
    unsigned char*  text,        /* pointer to data stream */
    int             text_len,    /* length of data stream */
    unsigned char*  key,         /* pointer to authentication key */
    int             key_len,     /* length of authentication key */
    caddr_t         digest       /* caller digest to be filled in */
) ;

#endif

