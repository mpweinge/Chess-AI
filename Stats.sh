#!/bin/bash

for files in ./*; do
	wc -l $files
done
