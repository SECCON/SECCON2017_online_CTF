/* vim: set sw=4 ts=4 et ai : */
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    FILE *devnull = fopen("/dev/null", "w");
    FILE *script;
    char buf[1024];
    unsigned char m[32] = {0};
    unsigned char flag[17] = {0};

    script = fopen(argc==2?argv[1]:"default.fs", "r");
    printf("Enter flag: ");
    fgets(flag, sizeof(flag), stdin);
    memcpy(&m[16], flag, 16);
    while(fgets(buf, sizeof(buf), script) != NULL){
        fprintf(devnull, buf,
        &m[0 ], &m[1 ], &m[2 ], &m[3 ], &m[4 ], &m[5 ], &m[6 ], &m[7 ],
        &m[8 ], &m[9 ], &m[10], &m[11], &m[12], &m[13], &m[14], &m[15],
        &m[16], &m[17], &m[18], &m[19], &m[20], &m[21], &m[22], &m[23],
        &m[24], &m[25], &m[26], &m[27], &m[28], &m[29], &m[30], &m[31],
         m[0 ],  m[1 ],  m[2 ],  m[3 ],  m[4 ],  m[5 ],  m[6 ],  m[7 ],
         m[8 ],  m[9 ],  m[10],  m[11],  m[12],  m[13],  m[14],  m[15],
         m[16],  m[17],  m[18],  m[19],  m[20],  m[21],  m[22],  m[23],
         m[24],  m[25],  m[26],  m[27],  m[28],  m[29],  m[30],  m[31]
        );
    }
    if(m[15] == 0){
        printf("Congrats! The flag is SECCON{%s}.\n", flag);
    }else{
        printf("Invalid flag\n");
    }
}
