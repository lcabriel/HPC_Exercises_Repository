////////////////////////////////////////// GLOBAL VARS DEFINITION ////////////////////////////////

Table image_data; //The table to contain the results
int xsize,ysize; //The variable to contain the size of the table

//////////////////////////////////////// ELABORATING DATA FUNCTION /////////////////////////////////
/* Having the results we need to find the maximum value. This certainly exists because we have posed
a maximum number of iteration in the C codes. Found the maximum value I have to transform all the other
in a color in the gray scale so a value between 0 and 255.*/

Table dataElaboration(Table ID){
   Table output=ID; 
   
   //FIND THE MAX
   int max=0;
   for(int j=0;j<ysize;j++){
     for(int i=0;i<xsize;i++){
       if(ID.getInt(j,i)>max) max=ID.getInt(j,i);
     }
   }
   
   //Now take each value, normalize it on the max and multiply for 255
   for(int j=0;j<ysize;j++){
     for(int i=0;i<xsize;i++){
       output.setInt(j,i,round(ID.getInt(j,i)*255.0/max));
     }
   }
   
   //return the table of the color of each pixel
   return output;
}

//This function is the "setup" function. In Processing is performed only one time at the beggining of the code
void setup(){
  
  //##########################################################################################################
  //########################### MODIFY HERE ##################################################################
  //##########################################################################################################
  
  size(1600,1600); //size of the image/viewport
  
  //##########################################################################################################
  //##########################################################################################################
  //##########################################################################################################
  
  //Load the results file and set the sizes
  image_data = loadTable("output.csv");
  xsize = image_data.getColumnCount();
  ysize = image_data.getRowCount();
  
  //Elaborate the data using the function above to pass from numbers to colors
  image_data = dataElaboration(image_data);

  background(0); //Set the black base 
  
  //Draw the image for the same time
  for(int j=0;j<ysize;j++){
    for(int i=0;i<xsize;i++){
      color c = color(image_data.getInt(j,i),image_data.getInt(j,i),image_data.getInt(j,i));
      set(i,j,c);
    }
  }
 
  save("output.bmp"); //Save the image in the directory
}

//This processing function is repeated at every frame. It simply redraw the image in the viewport while the code is working.
void draw(){
  for(int j=0;j<ysize;j++){
    for(int i=0;i<xsize;i++){
      color c = color(image_data.getInt(j,i),image_data.getInt(j,i),image_data.getInt(j,i));
      set(i,j,c);
    }
  }

}
