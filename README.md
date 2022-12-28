# Optimal-Doritos

## About Code

Uses tesseract and opencv to convert content on a picture to pure text.

To compile:

```bash
$ g++ main.cpp -o main cutter.cpp `pkg-config --cflags --libs opencv` -ltesseract -lpthread -ldpp -std=c++17
```

To run, I recommend running using option `--help` first.

```bash
$ ./main.cpp --help
```

## Slides

Slides can be found at `./reveal.js-master/index.html`. Use any web browser to open it.