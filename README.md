# Python Wiegand reader on raspberry

Using GPIO you can read the key presses and card codes from a keypad with the Wiegand protocol.

```python
import logging
import pywiegand as wr

reader = wr.construct()
wr.begin(reader, data0, data1)
wr.ReadData(self._reader)

pending = wr.GetPendingBitCount(self._reader)

binary_data, bits = wr.ReadData(self._reader)
logging.debug("Wiegand(Data:%s Bit count:%s Pending: %s)", binary_data, bits, pending)
```
