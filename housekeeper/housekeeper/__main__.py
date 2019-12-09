if __name__ == '__main__':
    import argparse
    
    from housekeeper import Housekeeper
    from housekeeper.yaml_parser import YAMLParser

    arg_parser = argparse.ArgumentParser(prog='housekeeper',
                                         description='Perform housekeeping.')
    arg_parser.add_argument('-v', action='store_true', help='Verbosity')
    arg_parser.add_argument('yaml_path', type=str, help='Path to YAML config.')
    args = arg_parser.parse_args()
    
    yaml_configs = YAMLParser(args.yaml_path, verbose=args.v)
    house_keeper = Housekeeper(yaml_configs, verbose=args.v)
    house_keeper.housekeep()
