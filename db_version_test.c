//
// Test program
// How compatible are all those Berkeley DB versions ?
//
// gcc -o db_version_test  db_version_test.c -ldb -O0 -g\
//     -DHAVE_DB_STRERROR   -DHAVE_DB4_DB_H
//
// ./db_version_test   `pwd`
//


#include <sys/types.h>
#include <limits.h>

#ifdef HAVE_DB4_DB_H
#  include <db4/db.h>
#else
#  ifdef HAVE_DB3_DB_H
#    include <db3/db.h>
#  else
#    ifdef HAVE_DB2_DB_H
#      include <db2/db.h>
#    else
#      ifdef HAVE_DB1_DB_H
#        include <db1/db.h>
#      else
#        include <db.h>
#      endif
#    endif
#  endif
#endif

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>



void errcall (const char *errpfx, char *msg)
{
    printf (" --- errcall: msg = '%s'\n", msg) ;
}



int main (int argc, char *argv[])
{
    DB      *db ;
    DBT     key, data ;
//    DB_ENV  *env ;
    FILE    *f, *g ;
    int   zero = 0 ;
    int   err ;
    char  *s1 = "qwertyuiop" ;
    char  *s2 = "1234567890" ;
    char  *s3 = "AbCdEfG." ;
    char  *s4 = "This ain't in the database." ;
    char  *d1 = "data for the first put." ;
    char  *d2 = "data for the second put." ;
    char  *d3 = "data for the third put." ;

#ifndef DB_VERSION_MAJOR
#  define DB_VERSION_MAJOR 1
#endif

//    if (argc < 2) {
//        fprintf (stderr, "\aUsage: %s <directory-name>\n", argv[0]) ;
//	exit(1) ;
//    }

#if (DB_VERSION_MAJOR == 1)
    printf ("DB version = %d\n", DB_VERSION_MAJOR) ;
#else
    printf ("DB version: %d.%d.%d\n",
             DB_VERSION_MAJOR, DB_VERSION_MINOR, DB_VERSION_PATCH) ;
#endif


//           open/create


#if (DB_VERSION_MAJOR == 1)
    db = dbopen ("db_test.db", O_CREAT|O_RDWR, 0600, DB_HASH, NULL) ;
    if (db == NULL) {
        perror ("'dbopen' failed.") ;
        exit (1) ;
    }
#elif (DB_VERSION_MAJOR == 2)
    err = dbopen ("db_test.db", DB_HASH, O_CREAT|O_RDWR, 0600, NULL, NULL, &db) ;
    if (db == NULL) {
        perror ("'dbopen' failed.") ;
        exit (1) ;
    }
#else
//    err = db_env_create (&env, 0) ;
//    if (err) {
//        fprintf (stderr, "'db_env_create' failed: %s\n", db_strerror(err)) ;
//        exit (1) ;
//    }
#  if ( (DB_VERSION_MAJOR < 4) ||\
       ((DB_VERSION_MAJOR == 4) && (DB_VERSION_MINOR == 0)) ||\
       ((DB_VERSION_MAJOR == 4) && (DB_VERSION_MINOR == 1) && (DB_VERSION_PATCH < 25)) ) // XXX check ok?
//    err = env->open (env, 0, 0600) ;
#else
//     err = env->open (env, argv[1], 0, 0600) ;
//     err = env->open (env, NULL, DB_CREATE, 0600) ;
#endif
//    if (err) {
//        fprintf (stderr, "'env open' failed: %s\n", db_strerror(err)) ;
//        exit (1) ;
//    }
    err = db_create (&db, NULL, 0) ;
    if (err) {
        fprintf (stderr, "'db_create' failed: %s\n", db_strerror(err)) ;
        exit (1) ;
    }
    f = fopen ("db_errfile1.txt", "w") ;
    if (f == NULL) perror ("fopen") ;
    db->set_errfile (db, f) ;

#  if ( (DB_VERSION_MAJOR < 4) ||\
       ((DB_VERSION_MAJOR == 4) && (DB_VERSION_MINOR == 0)) ||\
       ((DB_VERSION_MAJOR == 4) && (DB_VERSION_MINOR == 1) && (DB_VERSION_PATCH < 25)) ) // XXX check ok?
    err = db->open (db, "db_test.db", NULL, DB_HASH, DB_CREATE, 0600) ;
#  else
    err = db->open (db, NULL, "db_test.db", NULL, DB_HASH, DB_CREATE, 0600) ;
#  endif
    if (err) {
        fprintf (stderr, "'db open' failed: %s\n", db_strerror(err)) ;
        exit (1) ;
    }
#endif


//           puts

    memset(&key, 0, sizeof(key)) ;
    memset(&data, 0, sizeof(data)) ;

    data.data = d1 ;
    data.size = strlen(d1) + 1 ;
    key.data = s1 ;
    key.size = strlen(s1) + 1 ;
#if (DB_VERSION_MAJOR == 1)
    err =  db->put (db, &key, &data, 0) ;
#else
    err =  db->put (db, NULL, &key, &data, 0) ;
#endif

#ifdef HAVE_DB_STRERROR
    if (err) fprintf (stderr, "s1 put error: %s\n", db_strerror(err)) ;
#else
    if (err) perror ("s1 'put' error.") ;
#endif


    data.data = d2 ;
    data.size = strlen(d2) + 1 ;
    key.data = s2 ;
    key.size = strlen(s2) + 1 ;
#if (DB_VERSION_MAJOR == 1)
    err =  db->put (db, &key, &data, 0) ;
#else
    err =  db->put (db, NULL, &key, &data, 0) ;
#endif

#ifdef HAVE_DB_STRERROR
    if (err) fprintf (stderr, "s2 put error: %s\n", db_strerror(err)) ;
#else
    if (err) perror ("s2 'put' error.") ;
#endif


    data.data = d3 ;
    data.size = strlen(d3) + 1 ;
    key.data = s3 ;
    key.size = strlen(s3) + 1 ;
#if (DB_VERSION_MAJOR == 1)
    err =  db->put (db, &key, &data, 0) ;
#else
    err =  db->put (db, NULL, &key, &data, 0) ;
#endif

#ifdef HAVE_DB_STRERROR
    if (err) fprintf (stderr, "s3 put error: %s\n", db_strerror(err)) ;
#else
    if (err) perror ("s3 'put' error.") ;
#endif

    data.data = NULL ;
    data.size = 0 ;


//           gets


    key.data = s1 ;
    key.size = strlen(s1) + 1 ;
#if (DB_VERSION_MAJOR == 1)
    err =  db->get (db, &key, &data, 0) ;
#else
    err =  db->get (db, NULL, &key, &data, 0) ;
#endif
    printf ("s1 len = %d   s1 data = %s\n", data.size, data.data) ;
#ifdef HAVE_DB_STRERROR
    if (err) fprintf (stderr, "s1 get error: %s\n", db_strerror(err)) ;
#else
    if (err) perror ("s1 'get' error.") ;
#endif

    key.data = s2 ;
    key.size = strlen(s2) + 1 ;
#if (DB_VERSION_MAJOR == 1)
    err =  db->get (db, &key, &data, 0) ;
#else
    err =  db->get (db, NULL, &key, &data, 0) ;
#endif
    printf ("s2 data = %s\n", data.data) ;
#ifdef HAVE_DB_STRERROR
    if (err) fprintf (stderr, "s2 get error: %s\n", db_strerror(err)) ;
#else
    if (err) perror ("s2 'get' error.") ;
#endif

    key.data = s3 ;
    key.size = strlen(s3) + 1 ;
#if (DB_VERSION_MAJOR == 1)
    err =  db->get (db, &key, &data, 0) ;
#else
    err =  db->get (db, NULL, &key, &data, 0) ;
#endif
    printf ("s3 data = %s\n", data.data) ;
#ifdef HAVE_DB_STRERROR
    if (err) fprintf (stderr, "s3 get error: %s\n", db_strerror(err)) ;
#else
    if (err) perror ("s3 'get' error.") ;
#endif

    key.data = s4 ;
    key.size = strlen(s4) + 1 ;
#if (DB_VERSION_MAJOR == 1)
    err =  db->get (db, &key, &data, 0) ;
#else
    err =  db->get (db, NULL, &key, &data, 0) ;
#endif
    printf ("s4 data = %s\n", data.data) ;
#ifdef HAVE_DB_STRERROR
    if (err) fprintf (stderr, "s4 get error: %s\n", db_strerror(err)) ;
#else
    if (err) perror ("s4 'get' error.") ;
#endif

#if (DB_VERSION_MAJOR == 1)
    db->close (db) ;
#else
    db->close (db, 0) ;
#endif

    return 0 ;
}

