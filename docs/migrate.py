#!python3

import argparse
import pathlib
import subprocess
import os
from distutils.dir_util import copy_tree

parser = argparse.ArgumentParser(description='Migrate')
parser.add_argument('source', metavar='source_dir', type=str,
                    help='sorce directory')

args = parser.parse_args()

print(pathlib.Path(args.source))

copy_tree(os.path.join(args.source, 'en/images'), 'docs/en/images')

for rst_file in list(pathlib.Path(args.source).glob('**/*.rst')):
    rst_file_relpath = os.path.join('docs', rst_file.relative_to(args.source))
    basename, _ = os.path.splitext(rst_file_relpath)
    basedir = pathlib.Path(basename).parent
    md_file_relpath = os.path.join(basename + '.md')

    pathlib.Path(basedir).mkdir(parents=True, exist_ok=True)

    subprocess.run(['pandoc', '-s', '-o', md_file_relpath, rst_file])
