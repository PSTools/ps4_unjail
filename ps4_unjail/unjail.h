#pragma once

#include <_types.h>

#define X86_CR0_WP (1 << 16)

// Used in every payload that uses jailbreak();
#define K300_XFAST_SYSCALL         0x0
#define K310_XFAST_SYSCALL         0x0
#define K311_XFAST_SYSCALL         0x0
#define K315_XFAST_SYSCALL         0x0
#define K350_XFAST_SYSCALL         0x003A1AD0
#define K355_XFAST_SYSCALL         0x003A1F10
#define K370_XFAST_SYSCALL         0x003A2000
#define K400_XFAST_SYSCALL         0x0030EA00
#define K401_XFAST_SYSCALL         0x0030EA00
#define K405_XFAST_SYSCALL         0x0030EB30
#define K406_XFAST_SYSCALL         0x0030EB40
#define K407_XFAST_SYSCALL         0x0030EB40
#define K450_XFAST_SYSCALL         0x003095D0
#define K455_XFAST_SYSCALL         0x003095D0
#define K470_XFAST_SYSCALL         0x0030B840
#define K471_XFAST_SYSCALL         0x0030B7D0
#define K472_XFAST_SYSCALL         0x0030B7D0
#define K473_XFAST_SYSCALL         0x0030B7D0
#define K474_XFAST_SYSCALL         0x0030B7D0
#define K500_XFAST_SYSCALL         0x000001C0
#define K501_XFAST_SYSCALL         0x000001C0
#define K503_XFAST_SYSCALL         0x000001C0
#define K505_XFAST_SYSCALL         0x000001C0
#define K507_XFAST_SYSCALL         0x000001C0
#define K550_XFAST_SYSCALL         0x000001C0
#define K553_XFAST_SYSCALL         0x000001C0
#define K555_XFAST_SYSCALL         0x000001C0
#define K556_XFAST_SYSCALL         0x000001C0
#define K600_XFAST_SYSCALL         0x000001C0
#define K602_XFAST_SYSCALL         0x000001C0
#define K620_XFAST_SYSCALL         0x000001C0
#define K650_XFAST_SYSCALL         0x000001C0
#define K651_XFAST_SYSCALL         0x000001C0
#define K670_XFAST_SYSCALL         0x000001C0
#define K671_XFAST_SYSCALL         0x000001C0
#define K672_XFAST_SYSCALL         0x000001C0
#define K700_XFAST_SYSCALL         0x000001C0
#define K701_XFAST_SYSCALL         0x000001C0
#define K702_XFAST_SYSCALL         0x000001C0
#define K750_XFAST_SYSCALL         0x000001C0
#define K751_XFAST_SYSCALL         0x000001C0
#define K755_XFAST_SYSCALL         0x000001C0
#define K800_XFAST_SYSCALL         0x000001C0
#define K801_XFAST_SYSCALL         0x000001C0
#define K803_XFAST_SYSCALL         0x000001C0
#define K850_XFAST_SYSCALL         0x000001C0
#define K852_XFAST_SYSCALL         0x000001C0
#define K900_XFAST_SYSCALL         0x000001C0

// Used in every payload that uses jailbreak();
#define K300_PRISON_0              0x0
#define K310_PRISON_0              0x0
#define K311_PRISON_0              0x0
#define K315_PRISON_0              0x0
#define K350_PRISON_0              0x00EF5A00
#define K355_PRISON_0              0x00EF5A00
#define K370_PRISON_0              0x00EFEF10
#define K400_PRISON_0              0x00F26010
#define K401_PRISON_0              0x00F26010
#define K405_PRISON_0              0x00F26010
#define K406_PRISON_0              0x00F26010
#define K407_PRISON_0              0x00F2A010
#define K450_PRISON_0              0x010399B0
#define K455_PRISON_0              0x010399B0
#define K470_PRISON_0              0x01042AB0
#define K471_PRISON_0              0x01042AB0
#define K472_PRISON_0              0x01042AB0
#define K473_PRISON_0              0x01042AB0
#define K474_PRISON_0              0x01042AB0
#define K500_PRISON_0              0x010986A0
#define K501_PRISON_0              0x010986A0
#define K503_PRISON_0              0x010986A0
#define K505_PRISON_0              0x010986A0
#define K507_PRISON_0              0x010986A0
#define K550_PRISON_0              0x01134180
#define K553_PRISON_0              0x01134180
#define K555_PRISON_0              0x01139180
#define K556_PRISON_0              0x01139180
#define K600_PRISON_0              0x01139458
#define K602_PRISON_0              0x01139458
#define K620_PRISON_0              0x0113D458
#define K650_PRISON_0              0x0113D4F8
#define K651_PRISON_0              0x0113D4F8
#define K670_PRISON_0              0x0113E518
#define K671_PRISON_0              0x0113E518
#define K672_PRISON_0              0x0113E518
#define K700_PRISON_0              0x0113E398
#define K701_PRISON_0              0x0113E398
#define K702_PRISON_0              0x0113E398
#define K750_PRISON_0              0x0113B728
#define K751_PRISON_0              0x0113B728
#define K755_PRISON_0              0x0113B728
#define K800_PRISON_0              0x0111A7D0
#define K801_PRISON_0              0x0111A7D0
#define K803_PRISON_0              0x0111A7D0
#define K850_PRISON_0              0x0111A8F0
#define K852_PRISON_0              0x0111A8F0
#define K900_PRISON_0              0x0111F870

// Used in every payload that uses jailbreak();
#define K300_ROOTVNODE             0x0
#define K310_ROOTVNODE             0x0
#define K311_ROOTVNODE             0x0
#define K315_ROOTVNODE             0x0
#define K350_ROOTVNODE             0x01963000
#define K355_ROOTVNODE             0x01963040
#define K370_ROOTVNODE             0x0196F040
#define K400_ROOTVNODE             0x0206D250
#define K401_ROOTVNODE             0x0206D250
#define K405_ROOTVNODE             0x0206D250
#define K406_ROOTVNODE             0x0206D250
#define K407_ROOTVNODE             0x02071250
#define K450_ROOTVNODE             0x021AFA30
#define K455_ROOTVNODE             0x021AFA30
#define K470_ROOTVNODE             0x021B89E0
#define K471_ROOTVNODE             0x021B89E0
#define K472_ROOTVNODE             0x021B89E0
#define K473_ROOTVNODE             0x021B89E0
#define K474_ROOTVNODE             0x021B89E0
#define K500_ROOTVNODE             0x022C19F0
#define K501_ROOTVNODE             0x022C19F0
#define K503_ROOTVNODE             0x022C1A70
#define K505_ROOTVNODE             0x022C1A70
#define K507_ROOTVNODE             0x022C1A70
#define K550_ROOTVNODE             0x022EF570
#define K553_ROOTVNODE             0x022EF570
#define K555_ROOTVNODE             0x022F3570
#define K556_ROOTVNODE             0x022F3570
#define K600_ROOTVNODE             0x021BFAC0
#define K602_ROOTVNODE             0x021BFAC0
#define K620_ROOTVNODE             0x021C3AC0
#define K650_ROOTVNODE             0x02300320
#define K651_ROOTVNODE             0x02300320
#define K670_ROOTVNODE             0x02300320
#define K671_ROOTVNODE             0x02300320
#define K672_ROOTVNODE             0x02300320
#define K700_ROOTVNODE             0x022C5750
#define K701_ROOTVNODE             0x022C5750
#define K702_ROOTVNODE             0x022C5750
#define K750_ROOTVNODE             0x01B463E0
#define K751_ROOTVNODE             0x01B463E0
#define K755_ROOTVNODE             0x01B463E0
#define K800_ROOTVNODE             0x01B8C730
#define K801_ROOTVNODE             0x01B8C730
#define K803_ROOTVNODE             0x01B8C730
#define K850_ROOTVNODE             0x01C66150
#define K852_ROOTVNODE             0x01C66150
#define K900_ROOTVNODE             0x021EFF20


// Used in ToDex
#define K300_TARGET_ID             0x0
#define K310_TARGET_ID             0x0
#define K311_TARGET_ID             0x0
#define K315_TARGET_ID             0x0
#define K350_TARGET_ID             0x019A121D
#define K355_TARGET_ID             0x019A121D
#define K370_TARGET_ID             0x019AD21D
#define K400_TARGET_ID             0x0200151D
#define K401_TARGET_ID             0x0200151D
#define K405_TARGET_ID             0x0200151D
#define K406_TARGET_ID             0x0200151D
#define K407_TARGET_ID             0x0200551D
#define K450_TARGET_ID             0x01B6D08D
#define K455_TARGET_ID             0x01B6D08D
#define K470_TARGET_ID             0x01B7D08D
#define K471_TARGET_ID             0x01B7D08D
#define K472_TARGET_ID             0x01B7D08D
#define K473_TARGET_ID             0x01B7D08D
#define K474_TARGET_ID             0x01B7D08D
#define K500_TARGET_ID             0x01CD068D
#define K501_TARGET_ID             0x01CD068D
#define K503_TARGET_ID             0x01CD068D
#define K505_TARGET_ID             0x01CD068D
#define K507_TARGET_ID             0x01CD068D
#define K550_TARGET_ID             0x022BBF8D
#define K553_TARGET_ID             0x022BBF8D
#define K555_TARGET_ID             0x022BFF8D
#define K556_TARGET_ID             0x022BFF8D
#define K600_TARGET_ID             0x02159B8D
#define K602_TARGET_ID             0x02159B8D
#define K620_TARGET_ID             0x0215DB8D
#define K650_TARGET_ID             0x01BD800D
#define K651_TARGET_ID             0x01BD800D
#define K670_TARGET_ID             0x01BD800D
#define K671_TARGET_ID             0x01BD800D
#define K672_TARGET_ID             0x01BD800D
#define K700_TARGET_ID             0x022FED8D
#define K701_TARGET_ID             0x022FED8D
#define K702_TARGET_ID             0x022FED8D
#define K750_TARGET_ID             0x022287CD
#define K751_TARGET_ID             0x022287CD
#define K755_TARGET_ID             0x022287CD
#define K800_TARGET_ID             0x01B5158D
#define K801_TARGET_ID             0x01B5158D
#define K803_TARGET_ID             0x01B5158D
#define K850_TARGET_ID             0x01C8338D
#define K852_TARGET_ID             0x01C8338D
#define K900_TARGET_ID             0x0221688D

//spoof to dex
#define SPOOF 0x82


#define K300_NPDRM_OPEN            0x0
#define K310_NPDRM_OPEN            0x0
#define K311_NPDRM_OPEN            0x0
#define K315_NPDRM_OPEN            0x0
#define K350_NPDRM_OPEN            0x005E5870
#define K355_NPDRM_OPEN            0x005E5FE0
#define K370_NPDRM_OPEN            0x005E9AC0
#define K400_NPDRM_OPEN            0x00620340
#define K401_NPDRM_OPEN            0x006202C0
#define K405_NPDRM_OPEN            0x006204F0
#define K406_NPDRM_OPEN            0x00620660
#define K407_NPDRM_OPEN            0x006207A0
#define K450_NPDRM_OPEN            0x0062F050
#define K455_NPDRM_OPEN            0x0062EFF0
#define K470_NPDRM_OPEN            0x00632340
#define K471_NPDRM_OPEN            0x006322C0
#define K472_NPDRM_OPEN            0x006322C0
#define K473_NPDRM_OPEN            0x006322C0
#define K474_NPDRM_OPEN            0x006323E0
#define K500_NPDRM_OPEN            0x0064D420
#define K501_NPDRM_OPEN            0x0064D420
#define K503_NPDRM_OPEN            0x0064D7C0
#define K505_NPDRM_OPEN            0x0064D800
#define K507_NPDRM_OPEN            0x0064D800
#define K550_NPDRM_OPEN            0x00664DE0
#define K553_NPDRM_OPEN            0x00664CE0
#define K555_NPDRM_OPEN            0x00665220
#define K556_NPDRM_OPEN            0x00665220
#define K600_NPDRM_OPEN            0x00669220
#define K602_NPDRM_OPEN            0x00669220
#define K620_NPDRM_OPEN            0x006696E0
#define K650_NPDRM_OPEN            0x00669020
#define K651_NPDRM_OPEN            0x00669020
#define K670_NPDRM_OPEN            0x0066A630
#define K671_NPDRM_OPEN            0x0066A630
#define K672_NPDRM_OPEN            0x0066A630
#define K700_NPDRM_OPEN            0x00669B80
#define K701_NPDRM_OPEN            0x00669B80
#define K702_NPDRM_OPEN            0x00669B80
#define K750_NPDRM_OPEN            0x006678C0
#define K751_NPDRM_OPEN            0x006678C0
#define K755_NPDRM_OPEN            0x006678C0
#define K800_NPDRM_OPEN            0x0064D680
#define K801_NPDRM_OPEN            0x0064D680
#define K803_NPDRM_OPEN            0x0064D680
#define K850_NPDRM_OPEN            0x0064D3E0
#define K852_NPDRM_OPEN            0x0064D3E0
#define K900_NPDRM_OPEN            0x0064F160

#define K300_NPDRM_CLOSE           0x0
#define K310_NPDRM_CLOSE           0x0
#define K311_NPDRM_CLOSE           0x0
#define K315_NPDRM_CLOSE           0x0
#define K350_NPDRM_CLOSE           0x005E5890
#define K355_NPDRM_CLOSE           0x005E6000
#define K370_NPDRM_CLOSE           0x005E9AE0
#define K400_NPDRM_CLOSE           0x00620360
#define K401_NPDRM_CLOSE           0x006202E0
#define K405_NPDRM_CLOSE           0x00620510
#define K406_NPDRM_CLOSE           0x00620680
#define K407_NPDRM_CLOSE           0x006207C0
#define K450_NPDRM_CLOSE           0x0062F070
#define K455_NPDRM_CLOSE           0x0062F010
#define K470_NPDRM_CLOSE           0x00632360
#define K471_NPDRM_CLOSE           0x006322E0
#define K472_NPDRM_CLOSE           0x006322E0
#define K473_NPDRM_CLOSE           0x006322E0
#define K474_NPDRM_CLOSE           0x00632400
#define K500_NPDRM_CLOSE           0x0064D440
#define K501_NPDRM_CLOSE           0x0064D440
#define K503_NPDRM_CLOSE           0x0064D7E0
#define K505_NPDRM_CLOSE           0x0064D820
#define K507_NPDRM_CLOSE           0x0064D820
#define K550_NPDRM_CLOSE           0x00664E00
#define K553_NPDRM_CLOSE           0x00664D00
#define K555_NPDRM_CLOSE           0x00665240
#define K556_NPDRM_CLOSE           0x00665240
#define K600_NPDRM_CLOSE           0x00669240
#define K602_NPDRM_CLOSE           0x00669240
#define K620_NPDRM_CLOSE           0x00669700
#define K650_NPDRM_CLOSE           0x00669040
#define K651_NPDRM_CLOSE           0x00669040
#define K670_NPDRM_CLOSE           0x0066A650
#define K671_NPDRM_CLOSE           0x0066A650
#define K672_NPDRM_CLOSE           0x0066A650
#define K700_NPDRM_CLOSE           0x00669BA0
#define K701_NPDRM_CLOSE           0x00669BA0
#define K702_NPDRM_CLOSE           0x00669BA0
#define K750_NPDRM_CLOSE           0x006678E0
#define K751_NPDRM_CLOSE           0x006678E0
#define K755_NPDRM_CLOSE           0x006678E0
#define K800_NPDRM_CLOSE           0x0064D6A0
#define K801_NPDRM_CLOSE           0x0064D6A0
#define K803_NPDRM_CLOSE           0x0064D6A0
#define K850_NPDRM_CLOSE           0x0064D400
#define K852_NPDRM_CLOSE           0x0064D400
#define K900_NPDRM_CLOSE           0x0064F180

#define K300_NPDRM_IOCTL           0x0
#define K310_NPDRM_IOCTL           0x0
#define K311_NPDRM_IOCTL           0x0
#define K315_NPDRM_IOCTL           0x0
#define K350_NPDRM_IOCTL           0x005E58ED
#define K355_NPDRM_IOCTL           0x005E605D
#define K370_NPDRM_IOCTL           0x005E9B3D
#define K400_NPDRM_IOCTL           0x006203BD
#define K401_NPDRM_IOCTL           0x0062033D
#define K405_NPDRM_IOCTL           0x0062056D
#define K406_NPDRM_IOCTL           0x006206DD
#define K407_NPDRM_IOCTL           0x0062081D
#define K450_NPDRM_IOCTL           0x0062F0CD
#define K455_NPDRM_IOCTL           0x0062F06D
#define K470_NPDRM_IOCTL           0x006323BD
#define K471_NPDRM_IOCTL           0x0063233D
#define K472_NPDRM_IOCTL           0x0063233D
#define K473_NPDRM_IOCTL           0x0063233D
#define K474_NPDRM_IOCTL           0x0063245D
#define K500_NPDRM_IOCTL           0x0064D497
#define K501_NPDRM_IOCTL           0x0064D497
#define K503_NPDRM_IOCTL           0x0064D837
#define K505_NPDRM_IOCTL           0x0064D877
#define K507_NPDRM_IOCTL           0x0064D877
#define K550_NPDRM_IOCTL           0x00664E57
#define K553_NPDRM_IOCTL           0x00664D57
#define K555_NPDRM_IOCTL           0x00665297
#define K556_NPDRM_IOCTL           0x00665297
#define K600_NPDRM_IOCTL           0x00669297
#define K602_NPDRM_IOCTL           0x00669297
#define K620_NPDRM_IOCTL           0x00669757
#define K650_NPDRM_IOCTL           0x00669097
#define K651_NPDRM_IOCTL           0x00669097
#define K670_NPDRM_IOCTL           0x0066A6A7
#define K671_NPDRM_IOCTL           0x0066A6A7
#define K672_NPDRM_IOCTL           0x0066A6A7
#define K700_NPDRM_IOCTL           0x00669BF7
#define K701_NPDRM_IOCTL           0x00669BF7
#define K702_NPDRM_IOCTL           0x00669BF7
#define K750_NPDRM_IOCTL           0x00667937
#define K751_NPDRM_IOCTL           0x00667937
#define K755_NPDRM_IOCTL           0x00667937
#define K800_NPDRM_IOCTL           0x0064D6F7
#define K801_NPDRM_IOCTL           0x0064D6F7
#define K803_NPDRM_IOCTL           0x0064D6F7
#define K850_NPDRM_IOCTL           0x0064D457
#define K852_NPDRM_IOCTL           0x0064D457
#define K900_NPDRM_IOCTL           0x0064F1D7


extern uint8_t* gKernelBase;

struct auditinfo_addr1 {
	char useless[184];
};

struct ucred {
	uint32_t useless1;
	uint32_t cr_uid;     // effective user id
	uint32_t cr_ruid;    // real user id
	uint32_t useless2;
	uint32_t useless3;
	uint32_t cr_rgid;    // real group id
	uint32_t useless4;
	void *useless5;
	void *useless6;
	void *cr_prison;     // jail(2)
	void *useless7;
	uint32_t useless8;
	void *useless9[2];
	void *useless10;
	struct auditinfo_addr1 useless11;
	uint32_t *cr_groups; // groups
	uint32_t useless12;
};

struct filedesc1 {
	void *useless1[3];
	void *fd_rdir;
	void *fd_jdir;
};

struct proc1 {
	char useless[64];
	struct ucred *p_ucred;
	struct filedesc1 *p_fd;
};

struct thread1 {
	void *useless;
	struct proc1 *td_proc;
};
struct kpayload_args { uint64_t user_arg; };
struct kdump_args { uint64_t argArrayPtr; };


unsigned int long long __readmsr(unsigned long __register);

void  *unjail405(struct thread1 *td);
void  *unjail455(struct thread1 *td);
void  *unjail474(struct thread1 *td);
void  *unjail501(struct thread1 *td);
void  *unjail505(struct thread1 *td);
void  *unjail672(struct thread1 *td);
void  *unjail702(struct thread1 *td);
void  *unjail750(struct thread1 *td);
void  *unjail751(struct thread1 *td);
void  *unjail755(struct thread1 *td);
void  *unjail800(struct thread1 *td);
void  *unjail801(struct thread1 *td);
void  *unjail803(struct thread1 *td);
void  *unjail850(struct thread1 *td);
void  *unjail852(struct thread1 *td);
void  *unjail900(struct thread1 *td);
long unsigned int Temp();
void SetTemp(long unsigned int celsius);
int DecryptPup(const char* path);