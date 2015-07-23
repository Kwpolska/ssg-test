#include <stdio.h>
#include <sys/time.h>
#include "kwpbar.h"

/*
 * SSG Test Loop Runner
 * Copyright © 2015, Chris Warrick.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions, and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author of this software nor the names of
 *    contributors to this software may be used to endorse or promote
 *    products derived from this software without specific prior written
 *    consent.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

int main(int argc, char* argv[]) {
    // SETTINGS
    int cycles = 10;
    int incycle = 11;
    int total = cycles * incycle;

    if (argc != 2) {
        fprintf(stderr, "usage: ./run-loop (nikola|pelican|octopress|hexo)\n");
        return 2;
    }
    char build[64];
    char clean[64];
    char outfile[64];
    if (strcmp(argv[1], "nikola") == 0) {
        strcpy(build, "nikola build > nikola.log 2>&1");
        strcpy(clean, "rm -rf .doit.db cache output");
        strcpy(outfile, "nikola.csv");
    } else if (strcmp(argv[1], "pelican") == 0) {
        strcpy(build, "pelican content > pelican.log 2>&1");
        strcpy(clean, "rm -rf output");
        strcpy(outfile, "pelican.csv");
    } else if (strcmp(argv[1], "octopress") == 0) {
        strcpy(build, "rake generate > octopress.log 2>&1");
        strcpy(clean, "rm -rf .sass-cache .pygments-cache public");
        strcpy(outfile, "octopress.csv");
    } else if (strcmp(argv[1], "hexo") == 0) {
        strcpy(build, "hexo generate > hexo.log 2>&1");
        strcpy(clean, "rm -rf db.json public");
        strcpy(outfile, "hexo.csv");
    } else {
        fprintf(stderr, "usage: ./run-loop (nikola|pelican|octopress|hexo)\n");
        return 2;
    }
    int ret = 0;
    ret = system(clean);
    if (ret != 0) {
        fprintf(stderr, "\nCLEANUP ERROR\n");
        return ret;
    } else {
    }
    FILE *fh;
    fh = fopen(outfile, "w");
    int c = 0;
    int i = 0;
    int t = 0;
    pbar(0, total);
    while (c < cycles) {
        while (i < incycle) {
            struct timeval timedata_pre;
            struct timeval timedata_post;
            int seconds;
            int micro;
            double combined;
            gettimeofday(&timedata_pre, NULL);
            ret = system(build);
            gettimeofday(&timedata_post, NULL);
            seconds = timedata_post.tv_sec - timedata_pre.tv_sec;
            micro = timedata_post.tv_usec - timedata_pre.tv_usec;
            combined = (double)seconds + (double)micro/1000000.0;
            if (ret != 0) {
                fprintf(stderr, "\nEXECUTION ERROR, see log for details\n");
                return ret;
            } else {
                fprintf(fh, "%f,", combined);
            }
            i++;
            t++;
            pbar(t, total);
        }
        ret = system(clean);
        if (ret != 0) {
            fprintf(stderr, "\nCLEANUP ERROR\n");
            return ret;
        } else {
            fprintf(fh, "\n");
        }
        c++;
        i = 0;
    }
    fclose(fh);
    return 0;
}
