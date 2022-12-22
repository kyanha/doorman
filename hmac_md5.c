/*****************************************************************************

    File: "hmac_md5.c"

    This file is part of the Doorman Daemon & Portknocker client.

    The following code has been copied from sample code (provided by
    Pau-Chen Cheng and Jeff Kraemer) in the appendix of RFC2104 by
    Krawczyk, Bellare, and Canetti, Feb. 1997.
    It has been modified for the 'doorman' project by J.B.Ward, April 2006.

    Copyright information:
   ??????????????????????????????????????????????????????
   ??????????????????????????????????????????????????????

******************************************************************************/



#include "md5.h"

#ifdef WIN32
   typedef unsigned long caddr_t ;
#else
#  include <sys/types.h>
#endif

#include <string.h>





void hmac_md5 (
    unsigned char*  text,          // pointer to data stream
    int             text_len,      // length of data stream
    unsigned char*  key,           // pointer to authentication key
    int             key_len,       // length of authentication key
    void            *digest)       // caller digest to be filled in
{
    struct md5_ctx   context ;
    unsigned char    k_ipad[65] ;  // inner padding - key XORd with ipad 
    unsigned char    k_opad[65] ;  // outer padding - key XORd with opad
    unsigned char    tk[16] ;
    int              i ;

    // if key is longer than 64 bytes reset it to key=MD5(key)

    if (key_len > 64) {
        struct md5_ctx  tctx ;

        md5_init_ctx (&tctx) ;
        md5_process_bytes (key, key_len, &tctx) ;
        md5_finish_ctx (&tctx, tk) ;

        key = tk ;
        key_len = 16 ;
    }


    // the HMAC_MD5 transform looks like:
    //
    // MD5(K XOR opad, MD5(K XOR ipad, text))
    //
    // where K is an n byte key
    // ipad is the byte 0x36 repeated 64 times
    //
    // opad is the byte 0x5c repeated 64 times
    // and text is the data being protected


    memset (k_ipad, 0, sizeof(k_ipad)) ; // start out by storing key in pads
    memset (k_opad, 0, sizeof(k_opad)) ;
    memcpy (k_ipad, key, key_len) ;
    memcpy (k_opad, key, key_len) ;


    for (i=0; i<64; i++) {               // XOR key with ipad and opad values
        k_ipad[i] ^= 0x36 ;
        k_opad[i] ^= 0x5c ;
    }

    // perform inner MD5

    md5_init_ctx (&context) ;            // init context for 1st pass

    md5_process_block (k_ipad, 64, &context) ;    // start with inner pad
    md5_process_bytes (text, text_len, &context); // then text of datagram
    md5_finish_ctx (&context, digest) ;           // finish up 1st pass

    // perform outer MD5

    md5_init_ctx (&context) ;                     // init context for 2nd pass

    md5_process_block (k_opad, 64, &context) ;    // start with outer pad
    md5_process_bytes (digest, 16, &context) ;    // then results of 1st hash

    md5_finish_ctx (&context, digest) ;           // finish up 2nd pass

}

