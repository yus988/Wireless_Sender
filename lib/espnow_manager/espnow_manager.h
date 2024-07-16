#ifndef ESPNOW_MANAGER_H
#define ESPNOW_MANAGER_H
void initEspNow(void);
void sendSerialViaESPNOW(void);
void SendEspNOW(const char* cmd);
// void testESPNOW(void);
// void testSendWav(void);
#endif