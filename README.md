# Aegis Tasks

C and Python coding tasks from [Aegis](https://www.aegis-technologies.com.sg/)

## 1. Time-based Cache

Implement a [time-based cache in C](./time-cache/README.md) and
- Support a maximum number of items and remove the oldest if the maximum count 
  is hit
- Support expiring the items when their creation time is older than specified 
  expiry time

3rd party hash libraries (for instance G Hashtable) can be used to implement the
aforementioned features.

## 2. Housekeeping Tool

Implement a [housekeeping tool in Python](./housekeeper/README.md) and
- Support the YAML config file format to specify directories and configurations
- Support a maximum number of files to keep or the oldest files to keep based on 
  their last modified time
