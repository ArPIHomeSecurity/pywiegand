# Python Wiegand reader on Raspberry PI

Using GPIO you can read the key presses and card codes from a keypad with the Wiegand protocol.

[![Downloads](https://static.pepy.tech/personalized-badge/pywiegand?period=week&units=international_system&left_color=black&right_color=orange&left_text=Downloads)](https://pepy.tech/project/pywiegand)

# Requirements

* Raspberry PI OS
* WiringPI

# Wiring

![Wiring](wiring.png)

# Install

```
pip install pywiegand
```

# Usage

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

# Building

Do the following steps on the target device to build the package.

1. Install WiringPI https://github.com/wiringPi/WiringPi/
2. Install Python3 and poetry https://python-poetry.org/
3. Clone the repository
4. Run `poetry build`
5. Install the package with `pip install dist/pywiegand-<version>.tar.gz`
