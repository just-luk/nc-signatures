#!/bin/bash
rm -rf ~/.local/include/kodr
rm -rf ~/.local/lib/libkodr.a
cp -r kodr/include ~/.local/include/kodr
cp -r kodr/build/src/libkodr.a ~/.local/lib/libkodr.a
echo "kodr installed"

