import logging
import os
import sys

import yaml

def set_fallback(value, fall_back):
    return fall_back if value is None else value

class YAMLParser:
    """
    Parser to read and check YAML configurations.
    """

    def __init__(self, yaml_path, verbose=True):
        self.verbosity = logging.INFO if verbose else logging.ERROR
        self.__setup_logging()
        self.yaml_path = yaml_path
        with open(yaml_path, 'rt') as yaml_file:
            self.yaml_dict = yaml.safe_load(yaml_file.read())
        self.maximum_files = set_fallback(self.yaml_dict.get('maximum_files'),
                                          sys.maxsize)
        self.maximum_age = set_fallback(self.yaml_dict.get('maximum_age'),
                                        sys.maxsize)
        self.__parse_configurations()
    
    def __setup_logging(self):
        self.logger = logging.getLogger('YAMLParser')
        self.logger.setLevel(self.verbosity)
        console = logging.StreamHandler()
        console.setLevel(self.verbosity)
        log_format = '%(asctime)s | %(name)-12s [%(levelname)-8s] %(message)s'
        formatter = logging.Formatter(log_format)
        console.setFormatter(formatter)
        self.logger.handlers = [console]
    
    def __parse_configurations(self):
        directory_configs = []
        directories = self.yaml_dict.get('directories')
        for folder in directories:
            folder_path = folder.get('path')
            if folder_path is None or not os.path.isdir(folder_path):
                self.logger.warning(f'Removed {folder_path} from config.')
                continue
            folder['maximum_files'] = set_fallback(folder.get('maximum_files'), 
                                                   self.maximum_files)
            folder['maximum_age'] = set_fallback(folder.get('maximum_age'),
                                                 self.maximum_age)
            directory_configs.append(folder)
        self.configurations = directory_configs
