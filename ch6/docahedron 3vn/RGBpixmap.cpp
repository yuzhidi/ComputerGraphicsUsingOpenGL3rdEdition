// RGBpixmap.cpp, 10/14/99, by F.S.Hill,Jr. - for ECE660
// fixed 11/7/98 to prevent openGL from trying to align pixels to 4 byte boundaries
// in memory, which it does by default.

#include "RGBpixmap.h"
fstream inf;  // global in this file for convenience
fstream outf; // ditto
//<<<<<<<<<<<<<<<<<<<<< putShort >>>>>>>>>>>>>>>>>>>>
void putShort(ushort i)
{ // write short in little-endian form
	outf.put((char)(i & 0xff));
	outf.put((char)((i >> 8) & 0xff));	
}
//<<<<<<<<<<<<<<<<<<<<< putLong >>>>>>>>>>>>>>>>>>>>
void putLong(ulong i)
{ // write long in little-endian form
	outf.put((char)(i & 0xff));
	outf.put((char)((i >> 8) & 0xff));	
	outf.put((char)((i >> 16) & 0xff));	
	outf.put((char)((i >> 24) & 0xff));	
}
//<<<<<<<<<<<<<<<<<<<<< getShort >>>>>>>>>>>>>>>>>>>>
ushort getShort()
{// read a short in little-endian form
	char ic;
	ushort ip;
	inf.get(ic); ip = ic;  //first byte is little one 
	inf.get(ic);  ip |= ((ushort)ic << 8); // or in high order byte
	return ip;
}
//<<<<<<<<<<<<<<<<<<<< getLong >>>>>>>>>>>>>>>>>>>
ulong getLong()
{  // get little-endian 4-byte value from file, compose along portably
	ulong ip = 0;
	char ic = 0;
	unsigned char uc = ic;
	inf.get(ic); uc = ic; ip = uc;
	inf.get(ic); uc = ic; ip |=((ulong)uc << 8);
	inf.get(ic); uc = ic; ip |=((ulong)uc << 16);
	inf.get(ic); uc = ic; ip |=((ulong)uc << 24);
	return ip;
}

// Color3 methods
Color3::Color3()
{
	red = green = blue = 0;
}

Color3::Color3(float r, float g, float b)
{
	red = r; green = g; blue = b;
}

Color3::Color3(Color3& c)
{
	red = c.red; green = c.green; blue = c.blue;
}

void Color3::set(float r, float g, float b)
{
	red = r; green = g; blue = b;
}

void Color3::set(Color3& c)
{
	red = c.red; green = c.green; blue = c.blue;
}

void Color3::add(float r, float g, float b)
{
	red += r; green += g; blue += b;
}

void Color3::add(Color3& src, Color3& refl)
{ // add the product of source color and reflection coefficient
	red   += src.red   * refl.red;
	green += src.green * refl.green;
	blue  += src.blue  * refl.blue;
}

void Color3::add(Color3& colr)
{ // add colr to this color
	red += colr.red;
	green += colr.green;
	blue += colr.blue;	
}

void Color3::build4tuple(float v[])
{// load 4-tuple with this color: v[3] = 1 for homogeneous
	v[0] = red; v[1] = green; v[2] = blue; v[3] = 1.0f;
}

void Color3::printColor()
{
	cout << "(" << red << ", " << green << ", " << blue << ")\n"; 
}

//<<<<<<<<<<<<<<<<<< RGBPixmap:: readBmpFile>>>>>>>>>>>>>
int RGBpixmap:: readBMPFile(string fname) 
{  // Read into memory an mRGB image from an uncompressed BMP file.
	// return 0 on failure, 1 on success
	inf.open(fname.c_str(), ios::in|ios::binary); //must read raw binary char's.
	if(!inf)
	{
		cout << " can't open file: " << fname << endl;	
		return 0;
	}
	int k, row, col, numPadBytes, nBytesInRow;
	// read header information
	char ch1, ch2;
	inf.get(ch1); inf.get(ch2); // type is always 'BM'
	//cout << "file type = " << ch1 << ch2 << endl;
	ulong fileSize = getLong();
	ushort reserved1 =  getShort();     // always 0
	ushort reserved2= 	getShort();     // always 0 
	ulong offBits =		getLong();	    // offset to image - unreliable
	ulong headerSize =   getLong();      // always 40
	ulong numCols =		getLong();	    // number of columns in image
	ulong numRows = 		getLong();	    // number of rows in image
	ushort planes=    	getShort();     // always 1 
	ushort bitsPerPix = getShort();    // 8 or 24;only 24 bit case done 
	ulong compression = getLong();     // must be 0 for umcompressed 
	ulong imageSize = 	getLong();      // total bytes in image 
	ulong xPels =    	getLong();      // always 0 
	ulong yPels =    	getLong();      // always 0 
	ulong numLUTentries = getLong();   // 256 for 8 bit, otherwise 0 
	ulong impColors = 	getLong();      // always 0 
	/*	#define tell(str,val) cout << str << " = " << val << endl;
	tell("file size",fileSize);
	tell("reserved1", reserved1);
	tell("reserved2", reserved2);
	tell("offBits", offBits);
	tell("numCols", numCols);
	tell("numRows", numRows);
	tell("planes", planes);
	tell("bitsPerPixel", bitsPerPix);
	tell("image size", imageSize);
	tell("compression", compression);
	tell("xPels", xPels);
	tell("yPels", yPels);
	tell("numUTentries", numLUTentries);
	tell("impColors", impColors);
	*/
	if(bitsPerPix != 24) // error!
	{
		cout << "not a 24 bit/pixelimage!\n"; 
		inf.close(); 
		return 0;
	}
	// in BMP file, pad bytes inserted at end of each row so total number is a mult. of 4
	nBytesInRow = ((3 * numCols + 3)/4) * 4; // round up 3*numCols to next mult. of 4
	numPadBytes = nBytesInRow - 3 * numCols; // need this many
	nRows = numRows; // set class's data members
	nCols = numCols;
	cout << "numRows,numCols = " << numRows << "," << numCols << endl;
	cout.flush();
	pixel = new Color3[nRows * nCols]; //space for array in memory
	if(!pixel) 
		return 0; // out of memory!
	long count = 0;
	char dum,r,g,b;
	
	for(row = 0; row < nRows; row++) // read pixel values
	{
		for(col = 0; col < nCols; col++)
		{
			inf.get(b); inf.get(g); inf.get(r); // funny color order in BMP file
/*			pixel[count].red = 1.0 - (((float) r) / 255.0); 
			pixel[count].green = 1.0 - (((float) g) / 255.0);
			pixel[count++].blue = 1.0 - (((float) b) / 255.0);*/
			pixel[count].red = ((float) r) / 255.0; 
			pixel[count].green = ((float) g) / 255.0;
			pixel[count++].blue = ((float) b) / 255.0;
		}
      for(k = 0; k < numPadBytes ; k++) //skip over padding bytes at row's end
			inf >> dum;
	}
	inf.close();
	return 1; // success
}
//<<<<<<<<<<<<<<<<<< RGBpixmap:: writeBMPFile >>>>>>>>>>>>>>>>>
int RGBpixmap:: writeBMPFile(string fname)
{
	outf.open(fname.c_str(), ios::out | ios::binary);
	if( !outf){ cout << " can't open file!\n"; return 0;}
   if((nRows <= 0) || (nCols <= 0)){cout << "\n degenerate image!\n"; return 0;}
	
	// Must write a multiple of four bytes in each row of the image
	ushort nBytesInRow = ((3 * nCols + 3)/4) * 4; // round up 3 * nCols to next mult. of 4
   int numPadBytes = nBytesInRow - 3 * nCols; // num of pad bytes at end of each row
	ulong biSizeImage =  nBytesInRow * (ulong)nRows; // size of image
   //write pixmap FileHeader
   ushort bfType = 	0x4d42;	// 'BM';'B'=0x42, 'M'=0x4d
   putShort(bfType); // write it to file
   ulong bfSize = 54 + biSizeImage; //total size of image
	putLong(bfSize);
   putShort(0); putShort(0);// reserved 1 & 2
   putLong(ulong(54)); //bfOffBits: 54 - but not used in readers
   putLong(ulong(40)); // biSize - bytes in info header
   putLong(ulong(nCols)); putLong(ulong(nRows));
   putShort(ushort(1));   putShort(ushort(24)); // bit planes & bit count
   putLong(0L); // compression
	putLong(biSizeImage);
   putLong(0L); putLong(0L);//pelsPerMeterX and Y
   putLong(0L); putLong(0);//colors used & important colors
	//##############write bytes  ###############################
	long count = 0;
	for(int row = 0; row < nRows; row++)
	{
		for(int col = 0; col < nCols; col++)
		{
			outf.put(pixel[count].blue);
			outf.put(pixel[count].green);	
			outf.put(pixel[count++].red);
		}
		//now pad this row so num bytes is a mult of 4
		for(int k = 0; k < numPadBytes ; k++) //write dummy bytes to pad out row
			outf.put(char(0)); //padding bytes of 0
	}
	outf.close(); 
	return 1;  //success
}