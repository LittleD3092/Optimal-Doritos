// compile with: 
// g++ main.cpp -o main cutter.cpp `pkg-config --cflags --libs opencv` -ltesseract -lpthread -ldpp -std=c++17

#define DEBUG

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <string>
using std::string;

#include <unordered_map>
using std::unordered_map;

#include "cutter.h"

const unordered_map<string, string> options = {
    {"--help", "help"},
    {"-h", "help"},

    {"--token", "token"},
    
    {"--single-image", "single-image"},
    
    {"--use-cut", "use-cut"},

    {"--path", "path"},
    {"-p", "path"}
};

int main(int argc, char* argv[])
{
    string path;
    bool singleImage = false;
    bool useCut = false;

    if(argc == 1)
    {
        cout << "Use --help to see usage" << endl;
        return 0;
    }

    for(int i = 1; i < argc; i++)
    {
        if(options.find(argv[i]) == options.end())
        {
            cout << "Unknown option: " << argv[i] << endl;
            cout << "Use --help to see usage" << endl;
            return 0;
        }
        
        if(options.at(argv[i]) == "help" || argc == 1)
        {
            cout << "Usage: \n";
            cout << "Convert single image: " << argv[0] << " --single-image --path <image path>" << endl;
            cout << "Convert single image with cut: " << argv[0] << " --single-image --use-cut --path <image path>" << endl;
            return 0;
        }
        else if(options.at(argv[i]) == "single-image")
            singleImage = true;
        else if(options.at(argv[i]) == "use-cut")
            useCut = true;
        else if(options.at(argv[i]) == "path")
            path = argv[++i];
    }

    if(singleImage && useCut)
    {
        cout << "Converting image: " << path << endl;
        cv::Mat image = cv::imread(path);
        Cutter cutter(image);
        cv::Mat document = cutter.cut();
        string text = cutter.ocr();
        cout << "Text: \n" << text << endl;
        cv::imwrite("document.png", document);
        cout << "Document image saved to document.png" << endl;
        return 0;
    }
    else if(singleImage)
    {
        cout << "Converting image: " << path << endl;
        cv::Mat image = cv::imread(path);
        Cutter cutter(image);
        string text = cutter.ocr();
        cout << "Text: \n" << text << endl;
        return 0;
    }

    return 0;
}