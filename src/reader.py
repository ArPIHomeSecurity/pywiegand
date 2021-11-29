import pywiegand as wr

class WiegandReader:

    def __init__(self, data0, data1) -> None:
        self._reader = wr.construct()
        wr.begin(self._reader, data0, data1)

        # Cleanup before using
        wr.ReadData(self._reader)

    def read(self):
        binary_data, bits = wr.ReadData(self._reader)

        if bits in (26, 34):
            return str(int(binary_data, 2))
        else:
            return self.decode_keys(binary_data, bits)

    @staticmethod
    def decode_keys(binary, bits):
        """
        Reading multiple keys presses from the keypad.
        """
        words = [binary[idx:idx+8] for idx in range(0, bits, 8)]

        keys = []
        for word in words:
            for idx in range(8-min(8, bits), 7, 4):
                if bits > 0:
                    key = int(word[idx:idx+4], 2)
                    # replace number with character
                    if key == 11:
                        key = '#'
                    elif key == 12:
                        key = '*'
                    keys.append(str(key))
                    bits -= 4

        return keys
