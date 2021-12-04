#include <stdlib.h>

#define WIEGANDMAXDATA 32
#define WIEGANDTIMEOUT 3000000

typedef void (*FUNC)(void);

typedef struct ISREntry {
  int d0;
  int d1;
  FUNC isrd0;
  FUNC isrd1;
  unsigned long __wiegandBitCount;  // number of bits currently captured
  struct timespec __wiegandBitTime; // timestamp of the last bit received (used
                                    // for timeouts)
  unsigned char
      __wiegandData[WIEGANDMAXDATA]; // can capture upto 32 bytes of data
} ISREntry;

class Wiegand {
public:
  Wiegand();

  int Begin(int d0pin, int d1pin);
  int GetPendingBitCount(void);
  int ReadData(void *data, int dataMaxLen);
  void PrintBinCharPad(char c, char *resarr);
  int IsInitialized();

private:
  int m_d0pin;
  int m_d1pin;
  int m_isinit = 0;
  int m_isrord = -1;

private:
  void Reset(void);
};
