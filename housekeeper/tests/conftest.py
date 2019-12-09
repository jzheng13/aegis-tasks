import os

import pytest

@pytest.fixture
def root_directory():
    file_path = os.path.realpath(__file__)
    return os.path.dirname(os.path.dirname(file_path))

@pytest.fixture
def fixture_directory(root_directory):
    return os.path.join(root_directory, 'fixtures')

@pytest.fixture
def sub_fixture_directory(fixture_directory):
    return os.path.join(fixture_directory, 'sub_fixtures')

@pytest.fixture
def subsub_fixture_directory(sub_fixture_directory):
    return os.path.join(sub_fixture_directory, 'subsub_fixtures')

@pytest.fixture
def sample_yaml(fixture_directory):
    return os.path.join(fixture_directory, 'sample.yml')

@pytest.fixture
def fixture_files(fixture_directory, 
                  sub_fixture_directory, 
                  subsub_fixture_directory, 
                  sample_yaml):
    return [sample_yaml, 
            os.path.join(sub_fixture_directory, 'empty_file.txt'),
            os.path.join(subsub_fixture_directory, 'empty_file.txt')]

