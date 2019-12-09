import logging
import os

import yaml

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
        self.maximum_files = self.yaml_dict.get('maximum_files')
        self.maximum_age = self.yaml_dict.get('maximum_age')
        self.__parse_configurations()
    
    def __setup_logging(self):
        self.logger = logging.getLogger('YAMLParser')
        self.logger.setLevel(self.verbosity)
        console = logging.StreamHandler()
        console.setLevel(self.verbosity)
        log_format = '%(asctime)s | %(name)s [%(levelname)s] %(message)s'
        formatter = logging.Formatter(log_format)
        console.setFormatter(formatter)
        self.logger.handlers = [console]
    
    def __parse_configurations(self):
        directory_configs = []
        directories = self.yaml_dict.get('directories')
        for folder in directories:
            folder_path = folder.get('path')
            if folder_path is None or not os.path.isdir(folder_path):
                self.logger.warning(f'Removed {folder_path} from directories.')
                continue
            else:
                folder['maximum_files'] = folder.get('maximum_files') \
                    or self.maximum_files
                folder['maximum_age'] = folder.get('maximum_age') \
                    or self.maximum_age
                directory_configs.append(folder)
        self.configurations = directory_configs
