#include <Python.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <wiringPi.h>

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


ISREntry ISRArray[4] = {
    {.d0 = 0, .d1 = 0},
    {.d0 = 0, .d1 = 0},
    {.d0 = 0, .d1 = 0},
    {.d0 = 0, .d1 = 0}
};

void ISR1(void) {
  if (ISRArray[0].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[0].__wiegandData[ISRArray[0].__wiegandBitCount / 8] <<= 1;
    ISRArray[0].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[0].__wiegandBitTime);
}

void ISR2(void) {
  if (ISRArray[0].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[0].__wiegandData[ISRArray[0].__wiegandBitCount / 8] <<= 1;
    ISRArray[0].__wiegandData[ISRArray[0].__wiegandBitCount / 8] |= 1;
    ISRArray[0].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[0].__wiegandBitTime);
}

void ISR3(void) {
  if (ISRArray[1].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[1].__wiegandData[ISRArray[1].__wiegandBitCount / 8] <<= 1;
    ISRArray[1].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[1].__wiegandBitTime);
}

void ISR4(void) {
  if (ISRArray[1].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[1].__wiegandData[ISRArray[1].__wiegandBitCount / 8] <<= 1;
    ISRArray[1].__wiegandData[ISRArray[1].__wiegandBitCount / 8] |= 1;
    ISRArray[1].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[1].__wiegandBitTime);
}

void ISR5(void) {
  if (ISRArray[2].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[2].__wiegandData[ISRArray[2].__wiegandBitCount / 8] <<= 1;
    ISRArray[2].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[2].__wiegandBitTime);
}

void ISR6(void) {
  if (ISRArray[2].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[2].__wiegandData[ISRArray[2].__wiegandBitCount / 8] <<= 1;
    ISRArray[2].__wiegandData[ISRArray[2].__wiegandBitCount / 8] |= 1;
    ISRArray[2].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[2].__wiegandBitTime);
}

void ISR7(void) {
  if (ISRArray[3].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[3].__wiegandData[ISRArray[3].__wiegandBitCount / 8] <<= 1;
    ISRArray[3].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[3].__wiegandBitTime);
}

void ISR8(void) {
  if (ISRArray[3].__wiegandBitCount / 8 < WIEGANDMAXDATA) {
    ISRArray[3].__wiegandData[ISRArray[3].__wiegandBitCount / 8] <<= 1;
    ISRArray[3].__wiegandData[ISRArray[3].__wiegandBitCount / 8] |= 1;
    ISRArray[3].__wiegandBitCount++;
  }
  clock_gettime(CLOCK_MONOTONIC, &ISRArray[3].__wiegandBitTime);
}

class Wiegand {
public:
  Wiegand();

  int begin(int d0pin, int d1pin);
  int GetPendingBitCount(void);
  int ReadData(void *data, int dataMaxLen);
  void printbincharpad(char c, char *resarr);
  int d0pin;
  int d1pin;
  int isinit = 0;
  int isrord = -1;

private:
  void Reset(void);
};

Wiegand::Wiegand() {
  this->d0pin = 0;
  this->d1pin = 0;
  this->isinit = 0;
  this->isrord = -1;
  for (int i = 0; i < 4; i++) {
    if (ISRArray[i].d0 == 0) {
      this->isrord = i;
      break;
    }
  }
}

int Wiegand::begin(int d0pin, int d1pin) {
  if ((this->isinit == 0) && (this->isrord >= 0)) {
    if (wiringPiSetupGpio() < 0) {
      printf("Unable to init wiringPI");
      return 0;
    }
    switch (this->isrord) {
    case 0:
      ISRArray[this->isrord].isrd0 = &ISR1;
      ISRArray[this->isrord].isrd1 = &ISR2;
      break;
    case 1:
      ISRArray[this->isrord].isrd0 = &ISR3;
      ISRArray[this->isrord].isrd1 = &ISR4;
      break;
    case 2:
      ISRArray[this->isrord].isrd0 = &ISR5;
      ISRArray[this->isrord].isrd1 = &ISR6;
      break;
    case 3:
      ISRArray[this->isrord].isrd0 = &ISR7;
      ISRArray[this->isrord].isrd1 = &ISR8;
      break;
    }

    this->d0pin = d0pin;
    this->d1pin = d1pin;
    ISRArray[this->isrord].d0 = d0pin;
    ISRArray[this->isrord].d1 = d1pin;
    pinMode(d0pin, INPUT);
    pinMode(d1pin, INPUT);

    if (wiringPiISR(d0pin, INT_EDGE_FALLING, ISRArray[this->isrord].isrd0) < 0) {
      printf("Unable to setup ISR at %d ", d0pin);
      return 0;
    }

    if (wiringPiISR(d1pin, INT_EDGE_FALLING, ISRArray[this->isrord].isrd1) < 0) {
      printf("Unable to setup ISR at %d ", d1pin);
      return 0;
    }

    if ((this->isrord >= 0) && (this->isrord < 4)) {
      this->isinit = 1;
    }
  }

  this->Reset();
  return this->isinit;
}

void Wiegand::Reset(void) {
  if (this->isinit == 1) {
    memset((void *)ISRArray[this->isrord].__wiegandData, 0, WIEGANDMAXDATA);
    ISRArray[this->isrord].__wiegandBitCount = 0;
  }
}

int Wiegand::GetPendingBitCount(void) {
  struct timespec now, delta;
  if (this->isinit == 1) {
    clock_gettime(CLOCK_MONOTONIC, &now);
    delta.tv_sec = now.tv_sec - ISRArray[this->isrord].__wiegandBitTime.tv_sec;
    delta.tv_nsec =
        now.tv_nsec - ISRArray[this->isrord].__wiegandBitTime.tv_nsec;

    if ((delta.tv_sec > 1) || (delta.tv_nsec > WIEGANDTIMEOUT))
      return ISRArray[this->isrord].__wiegandBitCount;
  }
  return 0;
}

int Wiegand::ReadData(void *data, int dataMaxLen) {
  if (ISRArray[this->isrord].__wiegandBitCount > 0) {
    int bitCount = ISRArray[this->isrord].__wiegandBitCount;
    int byteCount = (bitCount / 8) + 1;
    memcpy(data, (void *)ISRArray[this->isrord].__wiegandData,((byteCount > dataMaxLen) ? dataMaxLen : byteCount));

    this->Reset();
    return bitCount;
  }
  return 0;
}

void Wiegand::printbincharpad(char c, char *resarr) {
  int i;
  memset(&resarr[0], 0, 9);
  for (i = 7; i >= 0; --i) {
    resarr[7 - i] = ((c & (1 << i)) ? '1' : '0');
  }
}

PyObject *construct(PyObject *self, PyObject *args) {
  Wiegand *WR = new Wiegand();
  PyObject *wrCapsule = PyCapsule_New((void *)WR, "WRPtr", NULL);
  PyCapsule_SetPointer(wrCapsule, (void *)WR);
  return Py_BuildValue("O", wrCapsule);
}

PyObject *begin(PyObject *self, PyObject *args) {
  PyObject *wrCapsule_;
  unsigned int d0_ = 0;
  unsigned int d1_ = 0;
  PyArg_ParseTuple(args, "OII", &wrCapsule_, &d0_, &d1_);
  if (PyCapsule_IsValid(wrCapsule_, "WRPtr")) {
    Wiegand *WR = (Wiegand *)PyCapsule_GetPointer(wrCapsule_, "WRPtr");
    return Py_BuildValue("i", WR->begin(d0_, d1_));
  } else {
    return 0;
  }
}

PyObject *GetPendingBitCount(PyObject *self, PyObject *args) {
  PyObject *wrCapsule_;
  PyArg_ParseTuple(args, "O", &wrCapsule_);
  Wiegand *WR = (Wiegand *)PyCapsule_GetPointer(wrCapsule_, "WRPtr");
  return Py_BuildValue("i", WR->GetPendingBitCount());
}

PyObject *isinitialized(PyObject *self, PyObject *args) {
  PyObject *wrCapsule_;
  PyArg_ParseTuple(args, "O", &wrCapsule_);
  if (PyCapsule_IsValid(wrCapsule_, "WRPtr")) {
    Wiegand *WR = (Wiegand *)PyCapsule_GetPointer(wrCapsule_, "WRPtr");
    return Py_BuildValue("i", WR->isinit);
  } else {
    return 0;
  }
}

PyObject *ReadData(PyObject *self, PyObject *args) {
  char data[WIEGANDMAXDATA];
  char binstr[100];
  char bstr[9];
  int bitlen;
  int slen;
  int i;
  PyObject *wrCapsule_;
  PyArg_ParseTuple(args, "O", &wrCapsule_);
  Wiegand *WR = (Wiegand *)PyCapsule_GetPointer(wrCapsule_, "WRPtr");
  memset(&data[0], 0, WIEGANDMAXDATA);
  bitlen = WR->ReadData((void *)data, WIEGANDMAXDATA);
  memset(&binstr[0], 0, 100);
  slen = (bitlen / 8 + 1);
  for (i = 0; i < slen; i++) {
    WR->printbincharpad(data[i], bstr);
    strcat(binstr, bstr);
  }

  return Py_BuildValue("si", binstr, bitlen);
}

static PyMethodDef pywiegand_methods[] = {
    {"construct", construct, METH_VARARGS, "Create Wiegand object"},
    {"begin", begin, METH_VARARGS, "Init GPIO pins"},
    {"isinitialized", isinitialized, METH_VARARGS, "return 1 if initialized"},
    {"GetPendingBitCount", GetPendingBitCount, METH_VARARGS,
     "Get pending bits in queue"},
    {"ReadData", ReadData, METH_VARARGS, "Read data"},

    {NULL, NULL, 0, NULL}};

static struct PyModuleDef pywiegand = {
    PyModuleDef_HEAD_INIT, "pywiegand", /* name of module */
    "",                                  /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module, or -1 if the module
           keeps state in global variables. */
    pywiegand_methods};

PyMODINIT_FUNC PyInit_pywiegand(void) { return PyModule_Create(&pywiegand); }
