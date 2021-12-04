from unittest.mock import patch
from unittest import TestCase

from pywiegand import DEFAULT_KEY_CODES, WiegandReader
from pywiegand.extension import pywiegand_adapter as wa


class WiegandDecoding(TestCase):
    key_test_cases = [
        {
            "input": ("00000111", 4),
            "output": ["7"]
        },
        {
            "input": ("0010000100000000", 8),
            "output": ["2", "1"]
        },
        {
            "input": ("0010001100000100", 12),
            "output": ["2", "3", "4"]
        },
        {
            "input": ("0011000110101001", 16),
            "output": ["3", "1", "10", "9"]
        },
        {
            "input": ("00001011", 4),
            "output": ["11"]
        },
        {
            "input": ("1011100100000000", 8),
            "output": ["11", "9"]
        },
        {
            "input": ("1011010100000000", 9),
            "output": ["11", "5"]
        }
    ]
        
    card_test_cases = [
        {
            "input": ("00101110011001101000101011", 26),
            "output": "12163627"
        },
        {
            "input": ("00000000000000001011001111000100", 34),
            "output": "46020"
        }
    ]
    
    key_test_cases = [
        {
            "input": ("11001011", 8),
            "output": ["*", "#"]
        }
    ]
    
    @patch.object(WiegandReader, "__init__", lambda x, y, z: None)
    @patch.object(wa, "read_data")
    def test_decode_cards(self, mock_read):
        """Test decoding binary output of the adapter without mapping"""
        reader = WiegandReader(0, 1)
        reader._mapping =  None
        reader._reader = None
        
        for test_case in (self.key_test_cases + self.card_test_cases):
            mock_read.return_value = test_case["input"]
            self.assertEqual(reader.read(), test_case["output"])
        

    @patch.object(WiegandReader, "__init__", lambda x, y, z: None)
    @patch.object(wa, "read_data")
    def test_decode_cards(self, mock_read):
        """Test decoding binary output of the adapter with mapping"""
        reader = WiegandReader(0, 1)
        reader._mapping =  DEFAULT_KEY_CODES
        reader._reader = None
        
        for test_case in (self.key_test_cases + self.card_test_cases):
            mock_read.return_value = test_case["input"]
            self.assertEqual(reader.read(), test_case["output"])
