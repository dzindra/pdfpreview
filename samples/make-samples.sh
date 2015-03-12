#!/bin/bash

cat sample.pdf | ../pdfpreview 100 150 2 > sample-1.jpg
cat sample.pdf | ../pdfpreview 100 150 4 > sample-2.jpg
cat sample.pdf | ../pdfpreview 150 100 3 > sample-3.jpg
cat sample.pdf | ../pdfpreview 100 150 3 3 7 > sample-4.jpg
cat sample.pdf | ../pdfpreview 100 150 1 5 5 > sample-5.jpg
