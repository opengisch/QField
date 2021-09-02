#!/bin/bash

shopt -s globstar
for file in **/*.md; do # Whitespace-safe and recursive
  filename=$(basename -- "$file")
cat <<EOT >> .tx/config
EOT
done
