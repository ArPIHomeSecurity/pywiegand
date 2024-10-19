import pywiegand.extension.pywiegand_adapter as wa

# default mapping from key codes to characters
DEFAULT_KEY_CODES = {
    11: "#",
    12: "*"
}

class WiegandReader:
    """
    Decoding the raw data from the adapter.
    """

    def __init__(self, data0, data1) -> None:
        """
        Initializing the reader with the GPIO pins.
        Use GPIO/BCM pin numbering.

        Args:
            data0 int : data 0 pin number
            data1 int : data 1 pin number
        """
        self._reader = wa.construct()
        self._mapping = DEFAULT_KEY_CODES
        wa.begin(self._reader, data0, data1)

        # Cleanup before using
        wa.read_data(self._reader)

    def __del__(self):
        wa.end(self._reader)

    def read(self) -> list():
        """
        Reading the key presses or card codes from the keypad.

        Returns:
            list|str: a list of key presses or a card code
        """
        binary_data, bits = wa.read_data(self._reader)

        if bits in (26, 34):
            return str(int(binary_data, 2))
        else:
            return self._decode_keys(binary_data, bits)
        
    def set_key_code_mapping(self, mapping: dict):
        """
        You can replace a received key codes with characters.
        Set to None to disable mapping.

        Args:
            mapping (dict): a dictonary mapping keys to value
        """
        self._mapping = mapping

    def _decode_keys(self, binary, bits):
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
                    try:
                        key = self._mapping[key]
                    except (KeyError, TypeError):
                        pass

                    keys.append(str(key))
                    bits -= 4

        return keys
    
    def get_pending_bit_count(self):
        """
        Retrieved the number of bit available for reading.

        Returns:
            int: number of available bits
        """
        return wa.get_pending_bit_count(self._reader)
    
    def is_initialized(self):
        """
        Check if the module is initialized.

        Returns:
            int: 1 if initialized
        """
        return wa.is_initialized(self._reader)
