#include <iostream>
#include <stdio.h>
#include <openssl/sha.h>

int do_sha1_file(char *name, unsigned char *out){
    FILE *f;
    unsigned char buf[8192];
    SHA_CTX sc;
    int err;

    f = fopen(name, "rb");
    if (f == NULL) {
        /* do something smart here: the file could not be opened */
        std::cout<<"file could not be opened"<<std::endl;
        return -1;
    }

    SHA1_Init(&sc);
    //viene eseguito una sola volta... non so perchè hanno inserito il for...
    for (;;) {
        size_t len;

        len = fread(buf, 1, sizeof buf, f);
        if (len == 0)
            break;
        SHA1_Update(&sc, buf, len);
    }
    err = ferror(f);
    fclose(f);
    if (err) {
        /* some I/O error was encountered; report the error */
        std::cout<<"I/O error"<<std::endl;
        return -1;
    }
    SHA1_Final(out, &sc);
    return 0;
}

int main(){
    unsigned char hash[SHA_DIGEST_LENGTH];
    do_sha1_file("/home/gabriele/Desktop/test.txt", hash);
    std::cout<<hash<<std::endl;
    return  0;
}