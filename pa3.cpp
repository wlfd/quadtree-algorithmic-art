// File:        pa3.cpp
// Author:      cs221 instructors
// Date:        2021-03-03
// Description: Partial test of PA3 functionality
//              Reads Point data from external files
//              Produces PNG images of the point sets

#include "sqtree.h"

int main() {

  // read in image
  PNG orig1;
  PNG orig2;
  PNG orig3;
  PNG orig4;
  PNG orig5;
  PNG orig6;
  PNG orig7;
  PNG orig8;
  PNG orig9;

  orig1.readFromFile("images/orig/small1.png");
  orig2.readFromFile("images/orig/small2.png");
  orig3.readFromFile("images/orig/pacman.png");
  orig4.readFromFile("images/orig/huckleberry_hound.png");
  orig5.readFromFile("images/orig/rocky.png");
  orig6.readFromFile("images/orig/rocky_bullwinkle.png");
  orig7.readFromFile("images/orig/colour.png");
  orig8.readFromFile("images/orig/klee-flora-on-sand.png");
  orig9.readFromFile("images/orig/stanley-totem-poles.png");
  
  // use it to build a sqtree (try other tolerances)
  SQtree t1(orig1,20000.0);
  SQtree t2(orig2,20000.0);
  SQtree t3(orig3,20000.0);
  SQtree t4(orig4,20000.0);
  SQtree t5(orig5,20000.0);
  SQtree t6(orig6,20000.0);
  SQtree t7(orig7,20000.0);
  SQtree t8(orig8,20000.0);
  SQtree t9(orig9,80000.0);

  // copy the tree (to show you can)
  SQtree tCopy(t1);

  // render the sqtree
  PNG pic1 = t1.render();
  PNG pic2 = t2.render();
  PNG pic3 = t3.render();
  PNG pic4 = t4.render();
  PNG pic5 = t5.render();
  PNG pic6 = t6.render();
  PNG pic7 = t7.render();
  PNG pic8 = t8.render();
  PNG pic9 = t9.render();

  // write it out
  pic1.writeToFile("images/out/small1.png");
  pic2.writeToFile("images/out/small2.png");
  pic3.writeToFile("images/out/pacman.png");
  pic4.writeToFile("images/out/huckleberry_hound.png");
  pic5.writeToFile("images/out/rocky.png");
  pic6.writeToFile("images/out/rocky_bullwinkle.png");
  pic7.writeToFile("images/out/colour.png");
  pic8.writeToFile("images/out/klee-flora-on-sand.png");
  pic9.writeToFile("images/out/stanley-totem-poles.png");
  cout << "rendered all images" << endl;

  return 0;
}
