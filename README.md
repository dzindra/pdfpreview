# pdfpreview
Simple tool to generate thumbnails from PDF.

#### Dependencies

* `g++`
* `pkg-config`
* `libpoppler44` (maybe other versions will work, tested on 0.24.5 and 0.29.0)
* `libpoppler-dev` 

#### Compiling

Run `./build.sh` in directory with pdfpreview to compile it.

#### Usage

PDF file is read from `stdin` and thumbnail image is written to `stdout`. Errors and messages will appear in `stderr`.
```
./pdfpreview <box width> <box height> <box max x> [first page] [last page] [verbose 0|1]
```

#### License

Licensed under GPL v2
