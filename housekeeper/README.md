# Housekeeper

`housekeeper` is a Python housekeeping tool which automatically cleans up 
directories by limiting
- the number of files in a directory, or
- the maximum age of the file

## Setup

This housekeeping tool `housekeeper` is written in Python 3.7.

Install all required libraries by doing
```sh
pip install -r requirements.txt
```

## YAML Configuration Format

`housekeeper` supports two configuration options: `maximum_files` and 
`maximum_age` (in days). Top level configurations are global configurations but
can be overwritten. All directory paths must be absolute.

An example is as follows:
```yaml
# global configuration
maximum_files: 3
maximum_age: 7

# individual directory configurations
directories:
- path: "/home/some/location"
  maximum_files: 2
- path: "/home/another/location"
  maximum_age: 3
```

## How to run

To run `housekeeper`, do
```sh
python -m housekeeper <path-to-yml>
```
