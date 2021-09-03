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

for rst_file in list(pathlib.Path(args.source).glob('**/*.rst')):
    rst_file_relpath = os.path.join('docs', rst_file.relative_to(args.source))
    basename, _ = os.path.splitext(rst_file_relpath)
    if basename.endswith('index') and basename != 'docs/en/index':
        continue
    basedir = pathlib.Path(basename).parent
    md_file_relpath = os.path.join(basename + '.md')

    pathlib.Path(basedir).mkdir(parents=True, exist_ok=True)

    subprocess.run(['pandoc', '-s', '-o', md_file_relpath, rst_file])
    subprocess.run(['sed', '-i', 's/{width=.*}//g', md_file_relpath])
    subprocess.run(['sed', '-i', r's|\(\](\)\.*/image|\1/assets/image|g', md_file_relpath])

copy_tree(os.path.join(args.source, 'en/images'), 'docs/assets/images')

