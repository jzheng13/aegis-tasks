import os
import time

DAY_SECONDS = 86400.0

def file_expired(file_path, maximum_age):
    lastmod_ts = os.path.getmtime(file_path)
    current_ts = time.time()
    return ((current_ts - lastmod_ts) / 86400.0) > maximum_age

def order_by_lastmod(file_paths):
    return sorted(file_paths, key=os.path.getmtime)

def list_all_files(directory_path):
    all_files = []
    for root, _, files in os.walk(directory_path):
        for f in files:
            all_files.append(os.path.join(root, f))
    return all_files