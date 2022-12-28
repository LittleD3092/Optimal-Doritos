# Optimal-Doritos

Uses tesseract and opencv to convert content on a picture to pure text.

To compile:

```bash
$ g++ main.cpp -o main cutter.cpp `pkg-config --cflags --libs opencv` -ltesseract -lpthread -ldpp -std=c++17
```

To run, I recommend running using option `--help` first.

```bash
$ ./main.cpp --help
```