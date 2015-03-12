//
//  pdfpreview
//
//  Copyright (c) 2015 Jindrich Dolezy <jindrich@dolezy.cz>
//
//  Licensed under GPL version 2
//

#include <stdio.h>
#include <math.h>
#include "poppler/goo/GooString.h"
#include "poppler/GlobalParams.h"
#include "poppler/PDFDoc.h"
#include "poppler/PDFDocFactory.h"
#include "poppler/SplashOutputDev.h"
#include "poppler/splash/SplashBitmap.h"
#include "poppler/splash/Splash.h"


// Enabling this will fill background of image boxes with different colors, so they are easily recognizable.
//#define SQUARE_FILL

// SplashBitmap does not provide info about bytes per pixel, so we lazily hardcode it here
#define BPP 3

// Fills rectangle on given bitmap with color c. Does not perform any range checks.
static void fillRect(SplashBitmap *bitmap, int x, int y, int w, int h, int c) {
    for (int ypos = y; ypos < y + h; ypos++) {
        memset(bitmap->getDataPtr() + ypos * bitmap->getRowSize() + BPP * x, c, (size_t) (w * BPP));
    }
}

// Draws whole source bitmap to dest bitmap at position x,y. Does not perform any range checks.
static void drawBitmap(SplashBitmap *source, SplashBitmap *dest, int x, int y) {
    for (int ypos = 0; ypos < source->getHeight(); ypos++) {
        memcpy(dest->getDataPtr() + (ypos + y) * dest->getRowSize() + BPP * x, source->getDataPtr() + ypos * source->getRowSize(), (size_t) source->getWidth() * BPP);
    }
}

// Draws page pg from document doc using splashOut. Image is resized to fit box_w x box_h pixels. Resulting image is either written to stdout if 
// canvas is NULL or drawn to its box on canvas based on page number.
static void drawPage(PDFDoc *doc, SplashOutputDev *splashOut, SplashBitmap *canvas, int pg, int box_w, int box_h, int box_x, int box_y, GBool useCropBox) {
    double pg_w, pg_h;

    if (useCropBox) {
        pg_w = doc->getPageCropWidth(pg);
        pg_h = doc->getPageCropHeight(pg);
    } else {
        pg_w = doc->getPageMediaWidth(pg);
        pg_h = doc->getPageMediaHeight(pg);
    }

    // Find resolution that fits to box_w x box_h
    double resolution = std::min((72.0 * box_w) / pg_w, (72.0 * box_h) / pg_h);
    pg_w = pg_w * (resolution / 72.0);
    pg_h = pg_h * (resolution / 72.0);

    // Rotate page if needed
    if ((doc->getPageRotate(pg) == 90) || (doc->getPageRotate(pg) == -90) || (doc->getPageRotate(pg) == 270)) {
        double tmp = pg_w;
        pg_w = pg_h;
        pg_h = tmp;
    }

    // Round page dimensions and eliminate rounding errors.
    int w = std::min((int) ceil(pg_w), box_w);
    int h = std::min((int) ceil(pg_h), box_h);

    doc->displayPageSlice(splashOut,
            pg, resolution, resolution,
            0,
            !useCropBox, gFalse, gFalse,
            0, 0, w, h
    );

    if (canvas == NULL)
        splashOut->getBitmap()->writeImgFile(splashFormatJpeg, stdout, 300, 300);
    else
        drawBitmap(splashOut->getBitmap(), canvas, box_x * box_w + (box_w - w) / 2, box_y * box_h + (box_h - h) / 2);
}

int main(int argc, char *argv[]) {
    GBool debug = gFalse;
    SplashBitmap *canvas = NULL;
    PDFDoc *doc;
    SplashOutputDev *splashOut;
    SplashColor paperColor = {255, 255, 255, 0};


    int box_w, box_h, box_xnum;
    int firstPage = 1;
    int lastPage = -1;

    // Poor man's argument parsing.
    if (argc < 4) {
        fprintf(stderr, "Usage:\n\tpdfpreview <box width> <box height> <box max x> [first page] [last page] [verbose 0|1]\n");
        return 2;
    }

    box_w = atoi(argv[1]);
    if (box_w <= 0) {
        fprintf(stderr, "Box width must be positive number.\n");
        return 2;
    }
    box_h = atoi(argv[2]);
    if (box_h <= 0) {
        fprintf(stderr, "Box height must be positive number.\n");
        return 2;
    }
    box_xnum = atoi(argv[3]);
    if (box_xnum <= 0) {
        fprintf(stderr, "Box max x must be positive number.\n");
        return 2;
    }
    if (argc > 4) {
        firstPage = atoi(argv[4]);
    }
    if (argc > 5) {
        lastPage = atoi(argv[5]);
    }
    if (argc > 6) {
        debug = atoi(argv[6]) ? gTrue : gFalse;
    }

    if (debug)
        fprintf(stderr, "fist_page: %d\nlast_page: %d\nbox_w: %d\nbox_h: %d\nbox_xnum: %d\n", firstPage, lastPage, box_w, box_h, box_xnum);

    // Open pdf document.
    globalParams = new GlobalParams(); // this must be created, or pdf render will crash
    {
        GooString *fileName = new GooString("fd://0"); // read from stdin
        doc = PDFDocFactory().createPDFDoc(*fileName, NULL, NULL);
        delete fileName;
    }

    if (!doc->isOk()) {
        delete doc;
        delete globalParams;
        return 1;
    }

    // Check page ranges ( 1 <= firstPage <= lastPage <= pages in document )
    if (firstPage < 1) {
        fprintf(stderr, "Adjusted first page from %d to 1.\n", firstPage);
        firstPage = 1;
    }
    if (lastPage < 1 || lastPage > doc->getNumPages()) {
        if (lastPage != -1) // only warn when page count set
            fprintf(stderr, "Adjusted last page from %d to %d.\n", lastPage, doc->getNumPages());
        lastPage = doc->getNumPages();
    }
    if (lastPage < firstPage) {
        fprintf(stderr, "Wrong page range given: the first page (%d) can not be after the last page (%d).\n", firstPage, lastPage);
        delete doc;
        delete globalParams;
        return 2;
    }

    // Prepare for rendering.
    splashOut = new SplashOutputDev(splashModeRGB8, 4, gFalse, paperColor, gTrue, splashThinLineDefault);
    splashOut->setVectorAntialias(gTrue);
    splashOut->startDoc(doc);

    // Create resulting bitmap if needed
    if (firstPage != lastPage) {
        int box_ynum = (int) ceil((double) (lastPage - firstPage + 1) / box_xnum);
        if (debug)
            fprintf(stderr, "box_ynum: %d\n", box_ynum);
        canvas = new SplashBitmap(box_w * box_xnum, box_h * box_ynum, 4, splashModeRGB8, gFalse);
#ifdef SQUARE_FILL
        for (int x = 0; x < box_xnum; x++) {
            for (int y = 0; y < box_ynum; y++) {
                fillRect(canvas, x * box_w, y * box_h, box_w, box_h, (x + y * box_xnum) * 30);
            }
        }
#else
        fillRect(canvas, 0, 0, canvas->getWidth(), canvas->getHeight(), 255);
#endif
    }

    if (debug) {
        if (firstPage == lastPage) {
            fprintf(stderr, "Processing only one page, will render to exact size\n");
        } else {
            fprintf(stderr, "Will process pages from %d to %d\n", firstPage, lastPage);
        }
    }

    // Process pages.
    for (int pg = firstPage; pg <= lastPage; ++pg) {
        if (debug)
            fprintf(stderr, "Processing page %d\n", pg);

        drawPage(doc, splashOut, canvas, pg, box_w, box_h, ((pg - firstPage) % box_xnum), ((pg - firstPage) / box_xnum), gFalse);
    }

    // Write result.
    if (canvas != NULL) {
        canvas->writeImgFile(splashFormatJpeg, stdout, 300, 300);
        delete canvas;
    }

    // Cleanup and exit.
    delete splashOut;
    delete doc;
    delete globalParams;

    return 0;
}
