/**
 *
* shifty quadtree (pa3)
* sqtree.cpp
* This file will be used for grading.
*
*/

#include "sqtree.h"

// First Node constructor, given.
SQtree::Node::Node(pair<int, int> ul, int w, int h, RGBAPixel a, double v)
    : upLeft(ul), width(w), height(h), avg(a), var(v), NW(NULL), NE(NULL), SE(NULL), SW(NULL) {}

// Second Node constructor, given
SQtree::Node::Node(stats &s, pair<int, int> ul, int w, int h)
    : upLeft(ul), width(w), height(h), NW(NULL), NE(NULL), SE(NULL), SW(NULL) {
    avg = s.getAvg(ul, w, h);
    var = s.getVar(ul, w, h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
    clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree &other) {
    copy(other);
}

// SQtree assignment operator, given.
SQtree &SQtree::operator=(const SQtree &rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

/* ======================================================================================== */

/**
 * SQtree constructor given tolerance for variance.
*/
SQtree::SQtree(PNG &imIn, double tol) {
    pair<int, int> ul = make_pair(0,0);
    PNG imPix(imIn);
    stats s(imPix);
    root = buildTree(s, ul, imIn.width(), imIn.height(), tol);
}

/**
 * Helper for the SQtree constructor.
*/
SQtree::Node *SQtree::buildTree(stats &s, pair<int, int> &ul, int w, int h, double tol) {
    if (w == 0 || h == 0) return NULL;
    RGBAPixel avg = s.getAvg(ul, w, h);
    double var = s.getVar(ul, w, h);
    // cout << "var = " << var << endl;
    Node *node = new Node(ul, w, h, avg, var);
    if ((w == 1 && h == 1) || (var <= tol)) {
        // cout << "return node" << endl;
        return node;
    }

    // initialize partitions
    pair<int, int> ulNW = make_pair(ul.first, ul.second);
    pair<int, int> ulNE = make_pair(0,0);
    pair<int, int> ulSW = make_pair(0,0);
    pair<int, int> ulSE = make_pair(0,0);
    double varNW = 0;
    double varNE = 0;
    double varSW = 0;
    double varSE = 0;
    int xLine;
    int yLine;
    double minMaxVar = var;
    double currMaxVar = 0.0;


    // find partitions
    for (unsigned int j = 0; j < (unsigned int) h; j++) {
        for (unsigned int i = 0; i < (unsigned int) w; i++) {
                ulNE = make_pair(ul.first + i, ul.second);
                ulSW = make_pair(ul.first, ul.second + j);
                ulSE = make_pair(ul.first + i, ul.second + j);

                varNW = s.getVar(ulNW, i, j);
                varNE = s.getVar(ulNE, w - i, j);
                varSW = s.getVar(ulSW, i, h - j);
                varSE = s.getVar(ulSE, w - i, h - j);

            // select minMaxVar partition 
            currMaxVar = max(varNW, max(varNE, max(varSW, varSE)));
            if (currMaxVar < minMaxVar) {
                minMaxVar = currMaxVar;
                xLine = i;
                yLine = j;
                // cout << "select minMaxVar = " << minMaxVar << endl;
            }
        }
    }

    // cout << "x = " << xLine << endl;
    // cout << "y = " << yLine << endl;

    ulNE = make_pair(ul.first + xLine, ul.second);
    ulSW = make_pair(ul.first, ul.second + yLine);
    ulSE = make_pair(ul.first + xLine, ul.second + yLine);

    // build selected partitions/tree recursively
    node->NW = buildTree(s, ulNW, xLine, yLine, tol);
    node->NE = buildTree(s, ulNE, w - xLine, yLine, tol);
    node->SW = buildTree(s, ulSW, xLine, h - yLine, tol);
    node->SE = buildTree(s, ulSE, w - xLine, h - yLine, tol);

    // cout << "built tree" << endl;
    return node;
}

/**
 * Render SQtree and return the resulting image.
*/
PNG SQtree::render() {
    PNG img(root->width, root->height);
    create(root, root->upLeft, img);
    return img; 
}

/**
 * Delete allocated memory.
*/
void SQtree::clear() {
    destroy(root);
}

void SQtree::copy(const SQtree &other) {
    root = copyTree(other.root);
}

int SQtree::size() {
	return size(root);
}

int SQtree::size(Node *node) {
	if (node == NULL) return 0;
	return 1 + size(node->NW) + size(node->SW) + size(node->NE) + size(node->SE);
}

void SQtree::create(Node *node, pair<int, int> ul, PNG &img) {
    if (node == NULL)
        return;

    if (node->NW == NULL && node->NE == NULL && node->SE == NULL && node->SW == NULL) {
        for (int y = 0; y < node->height; y++) {
            for (int x = 0; x < node->width; x++) {
                *(img.getPixel(x + ul.first, y + ul.second)) = node->avg;
            }
        }
    } else {
        if (node->NW != NULL)
            create(node->NW, node->NW->upLeft, img);
        if (node->NE != NULL)
            create(node->NE, node->NE->upLeft, img);
        if (node->SE != NULL)
            create(node->SE, node->SE->upLeft, img);
        if (node->SW != NULL)
            create(node->SW, node->SW->upLeft, img);
    }
}

void SQtree::destroy(Node *&node) {
    if (node == NULL)
        return;
    if (node->NW != NULL)
        destroy(node->NW);
    if (node->NE != NULL)
        destroy(node->NE);
    if (node->SE != NULL)
        destroy(node->SE);
    if (node->SW != NULL)
        destroy(node->SW);

    delete node;
    node = NULL;
}

SQtree::Node *SQtree::copyTree(const Node *copy_root) {
    if (copy_root == NULL)
        return NULL;

    Node *newNode = new Node(copy_root->upLeft, copy_root->width, copy_root->height, copy_root->avg, copy_root->var);
    newNode->NW = copyTree(copy_root->NW);
    newNode->NE = copyTree(copy_root->NE);
    newNode->SE = copyTree(copy_root->SE);
    newNode->SW = copyTree(copy_root->SW);

    return newNode;
}