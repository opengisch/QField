#!/usr/bin/env python

import pathlib
import os
from slugify import slugify

with open('.tx/config', 'w') as f:
    f.write('''[main]
host = https://www.transifex.com
lang_map = zh-Hant: zh_TW''')
    for md_file in list(pathlib.Path('docs').glob('**/*.md')):
        relpath = md_file.relative_to('docs')
        relbasename, _ = os.path.splitext(relpath)

        f.write(f'''
[qfdocstest.{slugify(str(relpath))}]
file_filter = docs/{relbasename}.<lang>.md
minimum_perc = 0
source_file = docs/{relbasename}.md
source_lang = en
type = GITHUBMARKDOWN
''')

# TODO: keep mkdocs.yml translation languages up to date
