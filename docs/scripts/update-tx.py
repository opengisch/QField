#!/usr/bin/env python

import pathlib
from slugify import slugify

with open('.tx/config', 'w') as f:
    f.write('''[main]
host = https://www.transifex.com
lang_map = zh-Hant: zh_TW''')
    for md_file in list(pathlib.Path('docs/en').glob('**/*.md')):
        relpath = md_file.relative_to('docs/en')


        f.write(f'''
[qfdocstest.{slugify(str(relpath))}]
file_filter = docs/<lang>/{relpath}
minimum_perc = 0
source_file = docs/en/{relpath}
source_lang = en
type = GITHUBMARKDOWN
''')

# TODO: keep mkdocs.yml translation languages up to date
