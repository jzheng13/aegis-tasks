import pytest

from housekeeper import yaml_parser

@pytest.fixture
def yaml(sample_yaml):
    return yaml_parser.YAMLParser(sample_yaml)

def test_init(yaml, sample_yaml):
    assert(yaml.yaml_path == sample_yaml)
    assert(yaml.verbosity)
    assert(yaml.maximum_files == 3)
    assert(yaml.maximum_age == 7)
    assert(len(yaml.configurations) == 2)
