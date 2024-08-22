## Exercise 2c: Image Render

This directory contain the Image Render to visualize the data produced by the Codes. This visualiser is made using Processing 4, an open-source graphical engine based on Java. Unfortunately, if you do not have Processing 4 or Processing-Intellij, you will not be able to run this code, however, can be easily opened using any software able to open Java codes. I would have liked to attach a package version of this software, but there are some problem in customizing the size of the image according to the results file, thus I left the code unpacked.

This README works also as a documentation for this software.

### USING THE VISUALISER

- Using the visualiser is fairly simple and I will describe the procedure using Processing 4. Opened the file with the software, it is necessary to insert in this directory the results file. The name of the file produce by the C codes is "output.csv".

- In the code, there is a very visible commented region characterise by a text "MODIFY HERE". Inside, there is the call of a function named "size(x,y)". To render correctly the results you need to change the two values with number of pixels along $x$ and $y$. The default in the code is $size(1600,1600)$, so the image size will be 1600 pixel along x and along y.

- Adapted the code with your size, you have only to press to the Play button in the top left corner. The program should start and the image should appear in the directory. The image will be shown also in the open viewport. To close, simply close the window.

In any case, if there are some problems with viewport, please write me or I can show you more deeply during the exam.