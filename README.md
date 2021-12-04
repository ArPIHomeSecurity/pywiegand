# Python Wiegand reader on raspberry

Using GPIO you can read the key presses and card codes from a keypad with the Wiegand protocol.

```python
>>> from  pywiegand import WiegandReader
>>> wr = WiegandReader(6, 5)
# reading card
>>> wr.read()
'560019750914'

# reading keys
>>> wr.read()
['0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '#']
```
