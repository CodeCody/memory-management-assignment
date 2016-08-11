#ifndef FRAME_H
#define FRAME_H
//struct to simulate a frame
struct Frame
{
	static unsigned FrameCount;//static to create Frame number
	unsigned FrameNum;//variable to hold frame number
	int counter;//counter used to determine page replacement
	int value;//value variable to hold page number

	Frame()
	{
		FrameCount++;//increment FrameCount
		value=-1;//initialize value to -1
		counter = -1;//initialize counter to -1
		FrameNum = FrameCount;//set frame number to frame count
	}

};

unsigned Frame::FrameCount = -1;//initialize frame count


#endif