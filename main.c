#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
// #include <malloc.h>
#include "common.h"
#include <netdb.h>
#include <string.h>
#include </usr/include/arpa/inet.h>

#define MAX_VCO                   (3200) //MHz
#define MIN_VCO                   (2000)
#define MAX_FBDIV                 (250)
#define MIN_FBDIV                 (16)
#define MAX_POS                   (7)
#define MIN_POS                   (0)
#define REF_FREQ                  (25)

#define PLLLOCKED(x)              (x)<<(31)
#define PLLEN(x)                  (x)<<(30)
#define PLL_BYPASS(x)             (x)<<(29)
#define VCO_SEL(x)                (x)<<(28)
#define FBDIV(x)                  ((x) & 0xfff)<<(16)
#define FASTCALEN(x)              (x)<<(15)
#define CALEN(x)                  (x)<<(14)
#define REFDIV(x)                 ((x) & 0x3f)<<(8)
#define POSDIV1(x)                ((x) & 0x7)<<(4)
#define POSDIV2(x)                ((x) & 0x7)

#define offsetof(type, member)  (&((type *)0)->member)
struct student
{
  int age;
  int grade;
  int score;
};

static float inferred_plldivider(uint32_t *param, float freq)
{
    float vco = 0.0;
    float actual_freq = 0.0;
    int refdiv = 1; //1~2
    int fbdiv = 32;
    int postdiv1 = MIN_POS;
    int postdiv2 = MIN_POS;

    //default value: 0xC0540165(50.0M)
    *param = PLLLOCKED(1) | PLLEN(1) | FBDIV(84) | \
             REFDIV(1) | POSDIV1(6) | POSDIV2(5);

    for (refdiv = 2; refdiv >= 1; refdiv--) {
        for (postdiv1 = MIN_POS; postdiv1 <= MAX_POS; postdiv1++) {
            for (postdiv2 = postdiv1; postdiv2 >= MIN_POS; postdiv2--) {
                float fb = (float)(freq * (postdiv1+1) * (postdiv2+1) * refdiv / REF_FREQ) * 100;
                //rounding up
                if ((int)fb % 100 > 50)
                    fbdiv = ((int)fb / 100) + 1;
                else
                    fbdiv = (int)fb / 100;

                vco = (float)REF_FREQ * fbdiv / refdiv;
				printf("target_freq:%.2f actual_freq:%.2f,vco:%.2f fbdiv:%d refdiv:%d postdiv1:%d, postdiv2:%d\n",
                                freq, actual_freq, vco, fbdiv, refdiv, postdiv1, postdiv2);
                if (fbdiv > MAX_FBDIV || fbdiv < MIN_FBDIV)
                    continue;

                if (refdiv == 1 && vco > (float)MAX_FBDIV * REF_FREQ / 2)
                    continue;

                if ((vco <= MAX_VCO) && (vco >= MIN_VCO)) {
                    uint32_t vco_sel = vco > 2400 ? 1 : 0;
                    actual_freq = ((float)REF_FREQ * fbdiv / refdiv / (postdiv1+1) / (postdiv2+1));
                    *param = PLLLOCKED(1) | PLLEN(1) | VCO_SEL(vco_sel) | FBDIV(fbdiv) | \
                                REFDIV(refdiv) | POSDIV1((uint8_t)postdiv1) | POSDIV2((uint8_t)postdiv2);
                    float diff = ((actual_freq - freq) < 0) ? -(actual_freq - freq) : (actual_freq - freq);
                    if (diff < 3.0) {
                        printf("target_freq:%.2f actual_freq:%.2f,vco:%.2f fbdiv:%d refdiv:%d postdiv1:%d, postdiv2:%d\n",
                                freq, actual_freq, vco, fbdiv, refdiv, postdiv1, postdiv2);
                        return actual_freq;
                    } else {
						printf("diff > 3\n");
                        actual_freq = 0.0;
                    }
                }
            }
        }
    }

	printf("don't find suitable para\n");
    return actual_freq;
}



int main(void)
{
	u_int32_t para = 0;
	float freq = 0;


	for(freq = 400; freq < 425; freq += 6.25)
	{
		inferred_plldivider( &para, freq);
		printf("para = 0x%08x\n\n", para);
	}
	return 0;
}
