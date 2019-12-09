import os
import secrets
import shutil
import time
from unittest import mock
import uuid

import pytest

from housekeeper import Housekeeper
from housekeeper import file_utils

SAMPLE_CONFIGS = [{
    'path': os.path.join('/tmp', str(uuid.uuid4())),
    'maximum_files': 1,
    'maximum_age': 2
}]

def create_empty_file(file_path):
    open(file_path, 'a').close()
    return file_path

def setup():
    if not os.path.exists(SAMPLE_CONFIGS[0]['path']):
        os.mkdir(SAMPLE_CONFIGS[0]['path'])
    for i in range(SAMPLE_CONFIGS[0]['maximum_files'] + 1):
        tmp_path = os.path.join(SAMPLE_CONFIGS[0]['path'], 
                                secrets.token_urlsafe(16))
        create_empty_file(tmp_path)

def teardown():
    shutil.rmtree(SAMPLE_CONFIGS[0]['path'], ignore_errors=True)

@pytest.fixture
def house_keeper():
    parsed_yaml = mock.MagicMock()
    parsed_yaml.configurations = SAMPLE_CONFIGS
    return Housekeeper(parsed_yaml)

def test_init(house_keeper):
    assert(house_keeper.housekeeping_configs == SAMPLE_CONFIGS)
    assert(house_keeper.verbosity)

def test_housekeep_file_limit_exceeded(house_keeper):
    setup()
    house_keeper.housekeep()
    assert(len(os.listdir(SAMPLE_CONFIGS[0]['path'])) \
        == SAMPLE_CONFIGS[0]['maximum_files'])
    teardown()

@mock.patch('housekeeper.file_utils.file_expired')
def test_housekeep_file_expired(mock_fexpiry, house_keeper):
    mock_fexpiry.return_value = True
    setup()
    house_keeper.housekeep()
    assert(not len(os.listdir(SAMPLE_CONFIGS[0]['path'])))
    teardown()