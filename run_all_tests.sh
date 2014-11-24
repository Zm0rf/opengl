#!/bin/bash

# If using out of source build: move to the build root.
if [[ -d build ]]; then
    cd build
fi

for file in $(find test/ -name 'run_test'); do
    echo "### Running test: "${file}
    ./${file}
done
