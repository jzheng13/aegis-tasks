import os
import time
from unittest import mock

from housekeeper import file_utils

@mock.patch('housekeeper.file_utils.os')
def test_expired_file_expired(mock_os):
    age = 1
    fake_age = (age + 1) * file_utils.DAY_SECONDS
    mock_os.path.getmtime.return_value = time.time() - fake_age
    assert(file_utils.file_expired('fake_file.txt', age))

@mock.patch('housekeeper.file_utils.os')
def test_fresh_file_unexpired(mock_os):
    age = 1
    fake_age = (age - 1) * file_utils.DAY_SECONDS
    mock_os.path.getmtime.return_value = time.time() - fake_age
    assert(not file_utils.file_expired('fake_file.txt', age))

@mock.patch('housekeeper.file_utils.os')
def test_order_by_lastmod(mock_os):
    def mock_getmtime(i):
        return i
    mock_os.path.getmtime = mock_getmtime
    fake_files = list(range(10))
    assert(file_utils.order_by_lastmod(reversed(fake_files)) == fake_files)

def test_list_all_files(fixture_directory, fixture_files):
    file_list = file_utils.list_all_files(fixture_directory)
    assert(len(file_list) == len(fixture_files))
    assert(all([f in fixture_files for f in file_list]))

