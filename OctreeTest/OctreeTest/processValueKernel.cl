__kernel void processValue(
	int fileRows,
	int rowSize,
	__global  float* fileValues1D,
	__global  float* result1D,
	int outputRowSize) {

	// Get the index of the current element to be processed
	int row = get_global_id(0);
	int col = get_global_id(1);
	int boxX, boxY, boxZ;
	// Do the operation
	//Result size: original + 3 + 8
	result1D[row*outputRowSize + col] = fileValues1D[row*rowSize + col];
	if (col == rowSize - 1)
	{
		
		result1D[row*outputRowSize + col + 1] = (int)fileValues1D[row * rowSize + 0] / 100;//boxX
		result1D[row*outputRowSize + col + 2] =  (int)fileValues1D[row * rowSize + 1] / 100;//boxY
		//printf("Values:%f Index: %d \n",fileValues1D[row * rowSize + 2],(int)fileValues1D[row * rowSize + 2] / 100);
		result1D[row*outputRowSize + col + 3]  = (int)fileValues1D[row * rowSize + 2] / 100;//boxZ
		float xmax = result1D[row*outputRowSize + col + 1]*100+100;
		float ymax = result1D[row*outputRowSize + col + 2]*100+100;
		float zmax = result1D[row*outputRowSize + col + 3]*100+100;
		float xmin = xmax - 200;
		float ymin = ymax - 200;
		float zmin = zmax - 200;
		float midX = (xmax+xmin)/2;
		float midY = (ymax+ymin)/2;
		float midZ = (zmax+zmin)/2;
		float x = result1D[row*outputRowSize + 1];
		float y = result1D[row*outputRowSize + 2];
		float z = result1D[row*outputRowSize + 3];
		float indexAns = 8;
		int i = 1;
		for(i = 1;i <=8 ; i++)
		{ 
			indexAns = 8;
			if (x <= midX)
			{
				indexAns = indexAns - 4;
				xmax = midX;
				midX = (midX - xmin) / 2 + xmin;

			}
			else
			{
				xmin = midX;
				midX = midX + (xmax - midX) / 2;
			}

			if (y <= midY)
			{
				indexAns = indexAns - 2;
				ymax = midY;
				midY = (midY - ymin) / 2 + ymin;

			}
			else
			{
				ymin = midY;
				midY = midY + (ymax - midY) / 2;
			}

			if (z <= midZ)
			{
				indexAns = indexAns - 1;
				zmax = midZ;
				midZ = (midZ - zmin) / 2 + zmin;

			}
			else
			{
				zmin = midZ;
				midZ = midZ + (zmax - midZ) / 2;
			}
			//printf("i:%d, index : %d, index Ans: %f, preValues: %f\n",i,row*outputRowSize + col + 3 + i,indexAns,result1D[row*outputRowSize + col + 3 ]);
			result1D[row*outputRowSize + col + 3 + i] = indexAns;	
		}
			
			
	}
	if (row*rowSize + col < 100)
	{
		//printf("processValue Kernel index: %d  Value: %f\n", row*rowSize + col, result1D[row*rowSize + col]);
	}
}