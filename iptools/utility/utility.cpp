#include "utility.h"

#define MAXRGB 255
#define MINRGB 0

using namespace cv;

std::string utility::intToString(int number)
{
   std::stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

int utility::checkValue(int value)
{
	if (value > MAXRGB)
		return MAXRGB;
	if (value < MINRGB)
		return MINRGB;
	return value;
}

/*-----------------------------------------------------------------------**/
void utility::addGrey(image &src, image &tgt, int value, int x, int y, int width, int height)
{
    // Ensure the target image is of the same size as the source
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

    // Apply the operation only within the ROI
    for (int i = y; i < y + height && i < src.getNumberOfRows(); i++) {
        for (int j = x; j < x + width && j < src.getNumberOfColumns(); j++) {
            int newValue = checkValue(src.getPixel(i, j) + value);
            tgt.setPixel(i, j, newValue); // Set the new value only within the ROI
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::binarize(image &src, image &tgt, int threshold, int x, int y, int width, int height)
{
    // Ensure the target image is of the same size as the source
    tgt.resize(src.getNumberOfRows(), src.getNumberOfColumns());

    // Apply the operation only within the ROI
    for (int i = y; i < y + height && i < src.getNumberOfRows(); i++) {
        for (int j = x; j < x + width && j < src.getNumberOfColumns(); j++) {
            int pixelValue = src.getPixel(i, j) < threshold ? MINRGB : MAXRGB;
            tgt.setPixel(i, j, pixelValue); // Set the new value only within the ROI
        }
    }
}

/*-----------------------------------------------------------------------**/
void utility::scale(image &src, image &tgt, float ratio)
{
	int rows = (int)((float)src.getNumberOfRows() * ratio);
	int cols  = (int)((float)src.getNumberOfColumns() * ratio);
	tgt.resize(rows, cols);
	for (int i=0; i<rows; i++)
	{
		for (int j=0; j<cols; j++)
		{	
			int i2 = (int)floor((float)i/ratio);
			int j2 = (int)floor((float)j/ratio);
			if (ratio == 2) {
				tgt.setPixel(i,j,checkValue(src.getPixel(i2,j2)));
			}

			if (ratio == 0.5) {
				int value = src.getPixel(i2,j2) + src.getPixel(i2,j2+1) + src.getPixel(i2+1,j2) + src.getPixel(i2+1,j2+1);
				tgt.setPixel(i,j,checkValue(value/4));
			}
		}
	}
}

/*-----------------------------------------------------------------------**/
void utility::cv_gray(Mat &src, Mat &tgt)
{
	cvtColor(src, tgt, COLOR_BGR2GRAY);
}

/*-----------------------------------------------------------------------**/
void utility::cv_avgblur(Mat &src, Mat &tgt, int WindowSize)
{
	blur(src,tgt,Size(WindowSize,WindowSize));
}
