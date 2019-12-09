import logging
import os

from housekeeper import file_utils

class Housekeeper:
    """
    Housekeeper class which decides which file to keep or discard based on given
    configurations.
    """

    def __init__(self, parsed_yaml, verbose=True):
        self.housekeeping_configs = parsed_yaml.configurations
        self.verbosity = verbose
        self.__setup_logging()

    def __setup_logging(self):
        self.logger = logging.getLogger('Housekeeper')
        self.logger.setLevel(self.verbosity)
        console = logging.StreamHandler()
        console.setLevel(self.verbosity)
        log_format = '%(asctime)s | %(name)-12s [%(levelname)-8s] %(message)s'
        formatter = logging.Formatter(log_format)
        console.setFormatter(formatter)
        self.logger.handlers = [console]

    def housekeep(self):
        for directory_configs in self.housekeeping_configs:
            directory_path = directory_configs['path']
            maximum_files = directory_configs['maximum_files']
            maximum_age = directory_configs['maximum_age']
            directory_files = file_utils \
                .order_by_lastmod(file_utils.list_all_files(directory_path))
            for df in directory_files[maximum_files:]:
                os.remove(df)
                self.logger.info(f'Removed {df} as file limit exceeded.')
            for df in directory_files[:maximum_files]:
                if file_utils.file_expired(df, maximum_age):
                    os.remove(df)
                    self.logger.info(f'Removed {df} as file has expired.')
    
